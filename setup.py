from distutils.core import setup, Extension

module1 = Extension('foohid',
                    sources = ['foohid.c'],
                    extra_link_args = ['-framework', 'IOKit'])

setup (name = 'foohid',
       version = '0.1',
       description = 'osx foohid iokit driver python wrapper',
       ext_modules = [module1])
