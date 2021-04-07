from libcpp cimport bool 
from libcpp.vector cimport vector 

cdef extern from "Stack.h": 
    cdef cppclass IntStack:
        IntStack(const int&)
        int pop()
        void push(const int&)
        bool isEmpty()
        vector[int] getStorage()

cdef class CppIntStack:
    cdef IntStack* _stack

    def __cinit__(self, int sz):
        self._stack = new IntStack(sz)

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