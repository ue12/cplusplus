cimport wrap_classB 


cdef class PyCppB:

    def __cinit__(self, name):
        self._ptr = new wrap_classB.CppB(name.encode("utf-8"))

    def getName(self):
        return self._ptr.getName()

    def isAttached(self):
        return self._ptr.isAttached()




