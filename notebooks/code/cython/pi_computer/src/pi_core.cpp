

double compute_pi_c( const int& nbpoint){
    double s = 0;
    double l = 1./nbpoint;
    int i;
    double x;
    for(int i=0; i<nbpoint; i++){
        x = l * ( i + 0.5 );
        s += l * ( 4. / (1. + x*x ) );
    }
    return s; 
}