#pragma once 

#include <vector>

class IntStack{
    public: 
        IntStack(const int& );

        void push(const int&);
        int pop();
        std::vector<int> getStorage();
        bool isEmpty();

    protected: 
        int _pos; 
        std::vector<int> _values;

};