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
    Pisah pisah("en");

    std::vector<std::pair<std::string, std::vector<std::string>>> testcases =
                {
                {"Hello World. My name is Jonas.",
                std::vector<std::string>{ "Hello World. ","My name is Jonas." }
                },
                {"What is your name? My name is Jonas.",
                std::vector<std::string>{ "What is your name? ","My name is Jonas." }
                },
                {"There it is! I found it.",
                std::vector<std::string>{ "There it is! ","I found it." }
                },
                {"My name is Jonas E. Smith.",
                std::vector<std::string>{ "My name is Jonas E. Smith." }
                },
                {"Please turn to p. 55.",
                std::vector<std::string>{ "Please turn to p. 55." }
                },
                {"Were Jane and co. at the party?",
                std::vector<std::string>{ "Were Jane and co. at the party?" }
                },
                {"They closed the deal with Pitt, Briggs & Co. at noon.",
                std::vector<std::string>{ "They closed the deal with Pitt, Briggs & Co. at noon." }
                },
                {"Let's ask Jane and co. They should know.",
                std::vector<std::string>{ "Let's ask Jane and co. ","They should know." }
                },
                {"They closed the deal with Pitt, Briggs & Co. It closed yesterday.",
                std::vector<std::string>{ "They closed the deal with Pitt, Briggs & Co. ","It closed yesterday." }
                },
                {"I can see Mt. Fuji from here.",
                std::vector<std::string>{ "I can see Mt. Fuji from here." }
                },
                {"St. Michael's Church is on 5th st. near the light.",
                std::vector<std::string>{ "St. Michael's Church is on 5th st. near the light." }
                },
                {"That is JFK Jr.'s book.",
                std::vector<std::string>{ "That is JFK Jr.'s book." }
                },
                {"I visited the U.S.A. last year.",
                std::vector<std::string>{ "I visited the U.S.A. last year." }
                },
                {"I live in the E.U. How about you?",
                std::vector<std::string>{ "I live in the E.U. ","How about you?" }
                },
                {"I live in the U.S. How about you?",
                std::vector<std::string>{ "I live in the U.S. ","How about you?" }
                },
                {"I work for the U.S. Government in Virginia.",
                std::vector<std::string>{ "I work for the U.S. Government in Virginia." }
                },
                {"I have lived in the U.S. for 20 years.",
                std::vector<std::string>{ "I have lived in the U.S. for 20 years." }
                },
                {"At 5 a.m. Mr. Smith went to the bank. He left the bank at 6 P.M. Mr. Smith then went to the store.",
                std::vector<std::string>{ "At 5 a.m. Mr. Smith went to the bank. ","He left the bank at 6 P.M. ","Mr. Smith then went to the store." }
                },
                {"She has $100.00 in her bag.",
                std::vector<std::string>{ "She has $100.00 in her bag." }
                },
                {"She has $100.00. It is in her bag.",
                std::vector<std::string>{ "She has $100.00. ","It is in her bag." }
                },
                {"He teaches science (He previously worked for 5 years as an engineer.) at the local University.",
                std::vector<std::string>{ "He teaches science (He previously worked for 5 years as an engineer.) at the local University." }
                },
                {"Her email is Jane.Doe@example.com. I sent her an email.",
                std::vector<std::string>{ "Her email is Jane.Doe@example.com. ","I sent her an email." }
                },
                {"The site is: https://www.example.50.com/new-site/awesome_content.html. Please check it out.",
                std::vector<std::string>{ "The site is: https://www.example.50.com/new-site/awesome_content.html. ","Please check it out." }
                },
                {"She turned to him, 'This is great.' she said.",
                std::vector<std::string>{ "She turned to him, 'This is great.' she said." }
                },
                {"She turned to him, \"This is great.\" she said.",
                std::vector<std::string>{ "She turned to him, \"This is great.\" she said." }
                },
                {"She turned to him, \"This is great.\" She held the book out to show him.",
                std::vector<std::string>{ "She turned to him, \"This is great.\" ","She held the book out to show him." }
                },
                {"Hello!! Long time no see.",
                std::vector<std::string>{ "Hello!! ","Long time no see." }
                },
                {"Hello?? Who is there?",
                std::vector<std::string>{ "Hello?? ","Who is there?" }
                },
                {"Hello!? Is that you?",
                std::vector<std::string>{ "Hello!? ","Is that you?" }
                },
                {"Hello?! Is that you?",
                std::vector<std::string>{ "Hello?! ","Is that you?" }
                },
                {"1.) The first item 2.) The second item",
                std::vector<std::string>{ "1.) The first item ","2.) The second item" }
                },
                {"1.) The first item. 2.) The second item.",
                std::vector<std::string>{ "1.) The first item. ","2.) The second item." }
                },
                {"1) The first item 2) The second item",
                std::vector<std::string>{ "1) The first item ","2) The second item" }
                },
                {"1) The first item. 2) The second item.",
                std::vector<std::string>{ "1) The first item. ","2) The second item." }
                },
                {"1. The first item 2. The second item",
                std::vector<std::string>{ "1. The first item ","2. The second item" }
                },
                {"1. The first item. 2. The second item.",
                std::vector<std::string>{ "1. The first item. ","2. The second item." }
                },
                {"• 9. The first item • 10. The second item",
                std::vector<std::string>{ "• 9. The first item ","• 10. The second item" }
                },
                {"⁃9. The first item ⁃10. The second item",
                std::vector<std::string>{ "⁃9. The first item ","⁃10. The second item" }
                },
                {"a. The first item b. The second item c. The third list item",
                std::vector<std::string>{ "a. The first item ","b. The second item ","c. The third list item" }
                },
                {"You can find it at N°. 1026.253.553. That is where the treasure is.",
                std::vector<std::string>{ "You can find it at N°. 1026.253.553. ","That is where the treasure is." }
                },
                {"She works at Yahoo! in the accounting department.",
                std::vector<std::string>{ "She works at Yahoo! in the accounting department." }
                },
                {"We make a good team, you and I. Did you see Albert I. Jones yesterday?",
                std::vector<std::string>{ "We make a good team, you and I. ","Did you see Albert I. Jones yesterday?" }
                },
                {"Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”",
                std::vector<std::string>{ "Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”" }
                },
                {"\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55).",
                std::vector<std::string>{ "\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55)." }
                },
                {"If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . Next sentence.",
                std::vector<std::string>{ "If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . ","Next sentence." }
                },
                {"I never meant that.... She left the store.",
                std::vector<std::string>{ "I never meant that.... ","She left the store." }
                },
                {"I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it.",
                std::vector<std::string>{ "I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it." }
                },
                {"One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. . . . The practice was not abandoned. . . .",
                std::vector<std::string>{ "One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. ",". . . The practice was not abandoned. . . ." }
                }


                };


    for (auto testcase : testcases)
    {
            auto expected = testcase.second;
            std::vector<std::string> output = pisah.Segment(testcase.first);
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