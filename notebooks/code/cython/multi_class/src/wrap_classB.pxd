from libcpp.string cimport string

cdef extern from "classB.hpp": 
    cdef cppclass CppB:
        CppB(const string& )
        string getName()
        int isAttached()


