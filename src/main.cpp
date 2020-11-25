#include <pcrecpp.h>
#include <iostream>
#include <string>
#include <string_view>
#include <memory>
#include <vector>

#include "berpecah.hpp"


int main(int argc, char *argv[]) {

    Berpecah b;
    std::string sample = "This is a sample text. Hello World!";
    std::string_view sv = sample;
    auto pieces = b.segment(sv);
    for (auto piece : pieces){
         std::cout << piece << std::endl;
    }
    
};
    