from setuptools import setup, Extension
from Cython.Distutils import build_ext
from Cython.Build import cythonize

ext_options = {
    'language':"c++",
    'extra_compile_args':["-O3",],
    'extra_objects': ["libFake.a"]}

extensions = [
    Extension("pyclassA", sources=["src/pyclassA.pyx", ], **ext_options),
    Extension("pyclassB", sources=["src/pyclassB.pyx", ], **ext_options),
]

setup(
    ext_modules=cythonize(extensions)
)