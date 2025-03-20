#sgn
from setuptools import setup, Extension

module1 = Extension('ExasolParser',
                    sources=['src/EntryPoint.cpp', 'src/ExslParser.cpp'],
                    include_dirs=['include'],
                    language='c++',
                    extra_compile_args=['-std=c++14']
                  )
setup(name='ExasolParser',
      version='1.0',
      description='This is an example module',
      ext_modules=[module1]
    )