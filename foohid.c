#include <Python.h>
#include <IOKit/IOKitLib.h>

#define FOOHID_SERVICE "it_unbit_foohid"

#define FOOHID_CREATE 0
#define FOOHID_DESTROY 1
#define FOOHID_SEND 2
#define FOOHID_LIST 3

static int foohid_connect(io_connect_t *conn) {
    io_iterator_t iterator;
    io_service_t service;
    kern_return_t ret = IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(FOOHID_SERVICE), &iterator);
    if (ret != KERN_SUCCESS) return -1;

    while ((service = IOIteratorNext(iterator)) != IO_OBJECT_NULL) {
        ret = IOServiceOpen(service, mach_task_self(), 0, conn);
        if (ret == KERN_SUCCESS) {
            IOObjectRelease(iterator);
            return 0;
        }
    }

    IOObjectRelease(iterator);
    return -1;
}

static void foohid_close(io_connect_t conn) {
    IOServiceClose(conn);
}

static PyObject *foohid_create(PyObject *self, PyObject *args) {
    char *name;
    Py_ssize_t name_len;
    char *descriptor;
    Py_ssize_t descriptor_len;
    char *serial;
    Py_ssize_t serial_len;
    unsigned int vendor_id;
    unsigned int product_id;

    if (!PyArg_ParseTuple(args, "s#s#s#II", &name, &name_len, &descriptor, &descriptor_len,
        &serial, &serial_len, &vendor_id, &product_id)) {
        return NULL;
    }

    if (name_len == 0 || descriptor_len == 0) {
        return PyErr_Format(PyExc_ValueError, "invalid values");
    }

    io_connect_t conn;
    if (foohid_connect(&conn)) {
        return PyErr_Format(PyExc_SystemError, "unable to open " FOOHID_SERVICE " service");
    }

    uint64_t input[8];
    input[0] = (uint64_t) name;
    input[1] = (uint64_t) name_len;
    input[2] = (uint64_t) descriptor;
    input[3] = (uint64_t) descriptor_len;
    input[4] = (uint64_t) serial;
    input[5] = (uint64_t) serial_len;
    input[6] = (uint64_t) vendor_id;
    input[7] = (uint64_t) product_id;

    kern_return_t ret = IOConnectCallScalarMethod(conn, FOOHID_CREATE, input, 8, NULL, 0);
    foohid_close(conn);

    if (ret != KERN_SUCCESS) {
        return PyErr_Format(PyExc_SystemError, "unable to create device");
    }

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject *foohid_send(PyObject *self, PyObject *args) {
    char *name;
    Py_ssize_t name_len;
    char *descriptor;
    Py_ssize_t descriptor_len;

    if (!PyArg_ParseTuple(args, "s#s#", &name, &name_len, &descriptor, &descriptor_len)) {
        return NULL;
    }

    if (name_len == 0 || descriptor_len == 0) {
        return PyErr_Format(PyExc_ValueError, "invalid values");
    }

    io_connect_t conn;
    if (foohid_connect(&conn)) {
        return PyErr_Format(PyExc_SystemError, "unable to open " FOOHID_SERVICE " service");
    }

    uint64_t input[4];
    input[0] = (uint64_t) name;
    input[1] = (uint64_t) name_len;
    input[2] = (uint64_t) descriptor;
    input[3] = (uint64_t) descriptor_len;

    kern_return_t ret = IOConnectCallScalarMethod(conn, FOOHID_SEND, input, 4, NULL, 0);
    foohid_close(conn);

    if (ret != KERN_SUCCESS) {
        return PyErr_Format(PyExc_SystemError, "unable to send hid message");
    }

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject *foohid_destroy(PyObject *self, PyObject *args) {
    char *name;
    Py_ssize_t name_len;
    
    if (!PyArg_ParseTuple(args, "s#", &name, &name_len)) {
        return NULL;
    }

    if (name_len == 0) {
        return PyErr_Format(PyExc_ValueError, "invalid name");
    }

    io_connect_t conn;
    if (foohid_connect(&conn)) {
        return PyErr_Format(PyExc_SystemError, "unable to open " FOOHID_SERVICE " service");
    }

    uint64_t input[2];
    input[0] = (uint64_t) name;
    input[1] = (uint64_t) name_len;

    kern_return_t ret = IOConnectCallScalarMethod(conn, FOOHID_DESTROY, input, 2, NULL, 0);
    foohid_close(conn);

    if (ret != KERN_SUCCESS) {
        return PyErr_Format(PyExc_SystemError, "unable to destroy hid device");
    }

    Py_INCREF(Py_True);
    return Py_True;
}

static PyObject *foohid_list(PyObject *self, PyObject *args) {

    if (!PyArg_ParseTuple(args, "")) {
        return NULL;
    }

    io_connect_t conn;
    if (foohid_connect(&conn)) {
        return PyErr_Format(PyExc_SystemError, "unable to open " FOOHID_SERVICE " service");
    }

    uint32_t output_count = 2;
    uint64_t output[] = {0, 0};

    uint16_t buf_len = 4096;
    char *buf = malloc(buf_len);
    if (!buf) {
        return PyErr_Format(PyExc_MemoryError, "unable to allocate memory");
    }

    uint32_t input_count = 2;
    uint64_t input[input_count];

    for(;;) {
        input[0] = (uint64_t) buf;
        input[1] = (uint64_t) buf_len;
        kern_return_t ret = IOConnectCallScalarMethod(conn, FOOHID_LIST, input, input_count, output, &output_count);
        foohid_close(conn);
        if (ret != KERN_SUCCESS) {
            free(buf);
            return PyErr_Format(PyExc_SystemError, "unable to list hid devices");
        }
        // all is fine
        if (output[0] == 0) {
            PyObject *ret = PyTuple_New(output[1]);
            uint64_t i;
            char *ptr = buf;
            for(i=0;i<output[1];i++) {
                PyTuple_SetItem(ret, i, PyString_FromString(ptr));
                ptr+=strlen(ptr)+1;
            }
            free(buf);
            return ret;
        }
        // realloc memory
        buf_len = output[0];
        char *tmp = realloc(buf, buf_len);
        if (!tmp) {
            free(buf);
            return PyErr_Format(PyExc_MemoryError, "unable to allocate memory");
        }
        buf = tmp;
    }

}


static PyMethodDef foohidMethods[] = {
    {"create", foohid_create, METH_VARARGS, "create a new foohid device"},
    {"destroy", foohid_destroy, METH_VARARGS, "destroy a foohid device"},
    {"send", foohid_send, METH_VARARGS, "send a hid message to a foohid device"},
    {"list", foohid_list, METH_VARARGS, "list the currently available foohid devices"},
    {NULL, NULL, 0, NULL}
};

PyMODINIT_FUNC initfoohid(void) {
    Py_InitModule("foohid", foohidMethods);
}
