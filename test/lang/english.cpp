#include <iostream>
#include <string>
#include <vector>

#include "pisah.hpp"
#include "doctest.h"


int factorial(int number) { return number <= 1 ? number : factorial(number - 1) * number; }

TEST_SUITE_BEGIN("English Tests");


TEST_CASE("Golden Rule Tests") {
    
    //std::string lang = "en";
    //Pisah pisah(lang);
    //Pisah pisah("en");

    std::vector<std::pair<std::string, std::vector<std::string>>> testcases = 
                {
                    {"This is a test. How are you?", 
                    std::vector<std::string>{"This is a test.", "How are you?"} 
                    }
                };

    std::vector<std::string> output = {"This is a test.", "How are you?"};

    for (auto testcase : testcases)
    {   
        SUBCASE("");
        auto expected = testcase.second;
        std::string s;
        s += "\nINPUT:\t" + testcase.first;
        s += "\nOUTPUT:\t" ;
        for(int i = 0; i < output.size(); i++) {
            s += output[i] + " ||| ";
        }
        s += "\nEXPECT:\t";
        for(int i = 0; i < expected.size(); i++){
            s += expected[i] + " ||| ";
        }
        CHECK_MESSAGE(output == expected, s);
        
        
        // && std::equal(output.begin(), output.end(), expected.begin()));
    }
    
    //CHECK(factorial(1) == 1);
    //CHECK(factorial(2) == 2);
    //CHECK(factorial(3) == 6);
    //CHECK(factorial(10) == 3628800);
}