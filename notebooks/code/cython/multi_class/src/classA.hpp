#pragma once 

#include <string>

class CppB;

class CppA{
    std::string _name; 
    CppB* _ptr;

public: 
    CppA(const std::string& name );

    void attach( CppB* b);

    std::string getName();
    std::string getAttachedName();
};