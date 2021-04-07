from libcpp cimport bool 
from libcpp.vector cimport vector 

cdef extern from "StackGeneric.hpp": 
    cdef cppclass StackGeneric[T]:
        StackGeneric(const int&)
        T pop()
        void push(const T&)
        bool isEmpty()
        vector[T] getStorage()

cdef class CppStackGenericInt:
    cdef StackGeneric[int]* _stack

    def __cinit__(self, int sz):
        self._stack = new StackGeneric[int](sz)

    def __dealloc__(self):
        del self._stack

    def push(self, int x):
        self._stack.push(x)

    def pop(self):
        return self._stack.pop()

    def isEmpty(self):
        return self._stack.isEmpty()

    def getStorage(self):
        return self._stack.getStorage()

cdef class CppStackGenericDouble:
    cdef StackGeneric[double]* _stack

    def __cinit__(self, int sz):
        self._stack = new StackGeneric[double](sz)

    def __dealloc__(self):
        del self._stack

    def push(self, double x):
        self._stack.push(x)

    def pop(self):
        return self._stack.pop()

    def isEmpty(self):
        return self._stack.isEmpty()

    def getStorage(self):
        return self._stack.getStorage()