

cdef extern from "pi_core.h":
    double compute_pi_c(const int& n)

def compute_pi(int n):
    return compute_pi_c(n)
