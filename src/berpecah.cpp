#include<iostream>
#include<memory>
#include<string>
#include<string_view>
#include<vector>
#include<pcrecpp.h>

#include "berpecah.hpp"

Berpecah::Berpecah(){
    std::cout << "Object Creater" << std::endl ; 
}

std::vector<std::string_view> Berpecah::segment(std::string_view inputText){
    std::vector<std::string_view> strPieces;
    strPieces.push_back(inputText);
    strPieces.push_back(inputText);
    return strPieces;
};
