import foohid

for i in range(1, 10):
    foohid.create("FooHID {0}".format(i), "xxx", "sn %i" % i, 5, 6)

print(foohid.list())

for i in range(1, 10):
    foohid.destroy("FooHID {0}".format(i))
