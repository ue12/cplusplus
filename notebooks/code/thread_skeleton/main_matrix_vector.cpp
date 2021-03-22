#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <chrono>
#include <functional>
#include <algorithm>
#include <omp.h>



// ON TOUCHE A RIEN ICI //
template<typename T, typename ...Args>
void benchmark(const std::string& name, const uint& repeat, std::function<std::vector<T>(const Matrix<T>&, const std::vector<T>& vec, Args... args)> mult, const Matrix<T>& m, const std::vector<T>& v, Args... args){
    std::vector<T> out; 
    auto start = std::chrono::high_resolution_clock::now();
    for( uint i=0; i<repeat; i++){
        out = mult(m, v, args...);
    }
    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << name << " (" << repeat << " runs) :" <<std::chrono::duration_cast<std::chrono::milliseconds>(stop-start).count()/repeat << " ms" << std::endl;
} 



template<typename T>
class Matrix{
    unsigned int _rows;
    unsigned int _cols;
    std::vector<T> _data;


    unsigned int idx(const uint& i, const uint& j) const {
        return i*this->_cols + j;
    }



    public: 
    Matrix(const int m, const int n):  _rows(m), _cols(n){
        _data.resize(this->_rows*this->_cols);
    }

    T& operator()(const unsigned int i, const unsigned int j){
        return this->_data[this->idx(i, j)];
    }

    T operator()(const unsigned int i, const unsigned int j) const {
        return this->_data[this->idx(i, j)];
    }

    uint rows() const {
        return this->_rows;
    }

    uint cols() const {
        return this->_cols;
    }

};

template<typename T>
std::vector<T> sequential_mult(const Matrix<T>& mat, const std::vector<T>& vec){
    std::vector<T> out(mat.rows());
    for( uint i=0; i<mat.rows(); i++){
        out[i] = 0.;
        for( uint j=0; j<mat.cols(); j++){
            out[i] += mat(i,j)*vec[j];
        }
    }
    return out;
}

template<typename T>
std::vector<T> thread_mult(const Matrix<T>& mat, const std::vector<T>& vec, const int& n_threads){
  //  TODO 
}

template<typename T>
std::vector<T> openmp_mult(const Matrix<T>& mat, const std::vector<T>& vec, const int n_threads){
  // TODO 
}


int main(int argc, char* argv[]){
    if( argc != 3 ){
        std::cerr << "Usage : mat_vec n_row n_col" << std::endl;
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(1.0, 2.0);
    const uint n_row = atoi(argv[1]);
    const uint n_col = atoi(argv[2]);
    const uint repeat = 100;

    Matrix<double> mat(n_row, n_col);
    std::vector<double> vec(n_col);

    for( uint i=0; i<n_row; i++){
        for( uint j=0; j<n_col; j++){
            mat(i,j) = dis(gen);
        }
    }

    for( uint j=0; j<n_col; j++){
        vec[j] = dis(gen);
    }


    benchmark("sequential",repeat, sequential_mult<double>, mat, vec );
    benchmark("thread #2",repeat, thread_mult<double>, mat, vec, 2 );
    benchmark("thread #4",repeat, thread_mult<double>, mat, vec, 4 );
    benchmark("thread #8",repeat, thread_mult<double>, mat, vec, 8 );

    benchmark("OpenMP #2",repeat, openmp_mult<double>, mat, vec, 2 );
    benchmark("OpenMP #4",repeat, openmp_mult<double>, mat, vec, 4 );
    benchmark("OpenMP #8",repeat, openmp_mult<double>, mat, vec, 8 );

    return 0; 
}

