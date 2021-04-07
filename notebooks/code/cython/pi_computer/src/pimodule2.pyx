
def compute_pi( nbpoint ):
    cdef double s = 0
    cdef double l = 1./nbpoint
    cdef int i
    cdef double x;
    for i in range(nbpoint):
        x = l * ( i + 0.5 )
        s += l * ( 4. / (1. + x**2 ) )
    return s
