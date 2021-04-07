cimport wrap_classA
cimport wrap_classB

cimport pyclassB

cdef class PyCppA: 
    cdef wrap_classA.CppA* _ptr; 
    def __cinit__(self, name):
        self._ptr = new wrap_classA.CppA( name.encode("utf-8") );

    def getName(self):
        name = self._ptr.getName()
        return name.decode()

    def attach(self, pyclassB.PyCppB b):
        self._ptr.attach( b._ptr )

    def getAttachedName(self):
        name = self._ptr.getAttachedName()
        return name.decode()