import foohid

for i in xrange(10):
    foohid.create("FooHID {0}".format(i), "xxx", "SN 123", 1, 2)

print(foohid.list())

for i in xrange(10):
    foohid.destroy("FooHID {0}".format(i))
