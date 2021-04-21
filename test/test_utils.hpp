#include <vector>
#include <string>

#include "doctest.h"
#include "utils.hpp"

#include "pisah.hpp"

void run_tests(std::vector<std::pair<std::string, std::vector<std::string>> > testcases, Pisah pisah){
    for (auto testcase : testcases)
    {
            auto expected = testcase.second;
            std::vector<std::string> output = pisah.Segment(testcase.first);
            std::string s;
            s += "\nINPUT:\t" + testcase.first;
            s += "\nOUTPUT:\t" ;
            for(int i = 0; i < output.size(); i++) {
                Utils::RTrim(output[i]);
                s += output[i] + " ||| ";
            }
            s += "\nEXPECT:\t";
            for(int i = 0; i < expected.size(); i++){
                Utils::RTrim(expected[i]);
                s += expected[i] + " ||| ";
            }
            CHECK_MESSAGE(output == expected, s);
        // && std::equal(output.begin(), output.end(), expected.begin()));
    }
}
