from setuptools import setup, Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",]}

extensions = [
    Extension("stackcpp", sources=["src/stackcpp.pyx", "src/Stack.cpp"], **ext_options),
    Extension("stackcpp_generic", sources=["src/stackcpp_generic.pyx"], **ext_options),
]

setup(
    ext_modules=cythonize(extensions)
)