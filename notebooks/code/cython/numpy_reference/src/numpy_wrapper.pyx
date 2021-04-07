cimport numpy as cnp 

cdef extern from "cpp_fill_array.h": 
    void cpp_fill_array(double*, const int& )
    cdef cppclass FakeComputer:
        FakeComputer(double* data, const int& sz)
        void compute()

cpdef fill_array( cnp.ndarray[cnp.float64_t] arr ):
    cpp_fill_array(<double*>arr.data, arr.size)

cdef class PyFakeComputer:
    cdef FakeComputer* _cptr
    def __cinit__(self, cnp.ndarray[cnp.float64_t, ndim=1] arr):

        self._cptr = new FakeComputer(<double*>arr.data, arr.size )

    def compute(self):
        self._cptr.compute()