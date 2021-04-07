from setuptools import setup, Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",]}

extensions = [
    Extension("pimodule1", sources=["src/pimodule1.pyx"], **ext_options),
    Extension("pimodule2", sources=["src/pimodule2.pyx"], **ext_options),
    Extension("pimodule3", sources=["src/pimodule3.pyx", "src/pi_core.cpp"], **ext_options)
]

setup(
    ext_modules=cythonize(extensions)
)