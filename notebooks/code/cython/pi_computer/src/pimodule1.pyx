
def compute_pi( nbpoint ):
    s = 0
    l = 1./nbpoint
    for i in range(nbpoint):
        x = l * ( i + 0.5 )
        s += l * ( 4. / (1. + x**2 ) )
    return s
