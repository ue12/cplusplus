#pragma once 

void cpp_fill_array(double* arr, const int& sz);

class FakeComputer{
public:
    FakeComputer(double* data, const int& sz): _data(data), _sz(sz){}

    void compute(){
        for(int i=0; i<this->_sz; i++){
            this->_data[i] /= (1.+i);
        }
    }

protected:
    double* _data; 
    int _sz;
};