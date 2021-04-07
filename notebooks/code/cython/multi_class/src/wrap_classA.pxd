cimport wrap_classB 
from libcpp.string cimport string

cdef extern from "classA.hpp": 
    cdef cppclass CppA:
        CppA(const string&)
        void attach(wrap_classB.CppB* )
        string getName()
        string getAttachedName()

