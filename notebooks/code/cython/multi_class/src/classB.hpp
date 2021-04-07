#pragma once 

#include <string>

class CppB{
    int _count; 
    std::string _name; 

public: 
    CppB(const std::string& name );

    std::string getName();

    int isAttached();

    void attached();

};