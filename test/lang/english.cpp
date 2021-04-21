#include <iostream>
#include <string>
#include <vector>

#include "doctest.h"

#include "test_utils.hpp"
#include "pisah.hpp"

TEST_SUITE_BEGIN("English Tests");

TEST_CASE("Golden Rule Tests") {

    //std::string lang = "en";
    //Pisah pisah(lang);
    Pisah pisah("en");
    //Rules are from: https://github.com/diasks2/pragmatic_segmenter
    std::vector<std::pair<std::string, std::vector<std::string>>> testcases =
                {//1
                {"Hello World. My name is Jonas.",
                std::vector<std::string>{ "Hello World.","My name is Jonas." }
                },
                //2
                {"What is your name? My name is Jonas.",
                std::vector<std::string>{ "What is your name?","My name is Jonas." }
                },
                //3
                {"There it is! I found it.",
                std::vector<std::string>{ "There it is!","I found it." }
                },
                //4
                {"My name is Jonas E. Smith.",
                std::vector<std::string>{ "My name is Jonas E. Smith." }
                },
                //5
                {"Please turn to p. 55.",
                std::vector<std::string>{ "Please turn to p. 55." }
                },
                //6
                {"Were Jane and co. at the party?",
                std::vector<std::string>{ "Were Jane and co. at the party?" }
                },
                //7
                {"They closed the deal with Pitt, Briggs & Co. at noon.",
                std::vector<std::string>{ "They closed the deal with Pitt, Briggs & Co. at noon." }
                },
                //8
                {"Let's ask Jane and co. They should know.",
                std::vector<std::string>{ "Let's ask Jane and co.","They should know." }
                },
                //9
                {"They closed the deal with Pitt, Briggs & Co. It closed yesterday.",
                std::vector<std::string>{ "They closed the deal with Pitt, Briggs & Co.","It closed yesterday." }
                },
                //10
                {"I can see Mt. Fuji from here.",
                std::vector<std::string>{ "I can see Mt. Fuji from here." }
                },
                //11
                {"St. Michael's Church is on 5th st. near the light.",
                std::vector<std::string>{ "St. Michael's Church is on 5th st. near the light." }
                },
                //12
                {"That is JFK Jr.'s book.",
                std::vector<std::string>{ "That is JFK Jr.'s book." }
                },
                //13
                {"I visited the U.S.A. last year.",
                std::vector<std::string>{ "I visited the U.S.A. last year." }
                },
                //14
                {"I live in the E.U. How about you?",
                std::vector<std::string>{ "I live in the E.U.","How about you?" }
                },
                //15
                {"I live in the U.S. How about you?",
                std::vector<std::string>{ "I live in the U.S.","How about you?" }
                },
                //16
                {"I work for the U.S. Government in Virginia.",
                std::vector<std::string>{ "I work for the U.S. Government in Virginia." }
                },
                //17
                {"I have lived in the U.S. for 20 years.",
                std::vector<std::string>{ "I have lived in the U.S. for 20 years." }
                },
                //18 -- HARD -- OTHER SENTENCE SPLITTERS CANNOT DO THIS.
                // {"At 5 a.m. Mr. Smith went to the bank. He left the bank at 6 P.M. Mr. Smith then went to the store.",
                // std::vector<std::string>{ "At 5 a.m. Mr. Smith went to the bank.","He left the bank at 6 P.M.","Mr. Smith then went to the store." }
                // },
                //19
                {"She has $100.00 in her bag.",
                std::vector<std::string>{ "She has $100.00 in her bag." }
                },
                //20
                {"She has $100.00. It is in her bag.",
                std::vector<std::string>{ "She has $100.00.","It is in her bag." }
                },
                //21
                {"He teaches science (He previously worked for 5 years as an engineer.) at the local University.",
                std::vector<std::string>{ "He teaches science (He previously worked for 5 years as an engineer.) at the local University." }
                },
                //22
                {"Her email is Jane.Doe@example.com. I sent her an email.",
                std::vector<std::string>{ "Her email is Jane.Doe@example.com.","I sent her an email." }
                },
                //23
                {"The site is: https://www.example.50.com/new-site/awesome_content.html. Please check it out.",
                std::vector<std::string>{ "The site is: https://www.example.50.com/new-site/awesome_content.html.","Please check it out." }
                },
                //24
                {"She turned to him, 'This is great.' she said.",
                std::vector<std::string>{ "She turned to him, 'This is great.' she said." }
                },
                //25
                {"She turned to him, \"This is great.\" she said.",
                std::vector<std::string>{ "She turned to him, \"This is great.\" she said." }
                },
                //26
                {"She turned to him, \"This is great.\" She held the book out to show him.",
                std::vector<std::string>{ "She turned to him, \"This is great.\"","She held the book out to show him." }
                },
                //27
                {"Hello!! Long time no see.",
                std::vector<std::string>{ "Hello!!","Long time no see." }
                },
                //28
                {"Hello?? Who is there?",
                std::vector<std::string>{ "Hello??","Who is there?" }
                },
                //29
                {"Hello!? Is that you?",
                std::vector<std::string>{ "Hello!?","Is that you?" }
                },
                //30
                {"Hello?! Is that you?",
                std::vector<std::string>{ "Hello?!","Is that you?" }
                },
                // //31 - NOT HANDLED
                // {"1.) The first item 2.) The second item",
                // std::vector<std::string>{ "1.) The first item","2.) The second item" }
                // },
                // //32 - NOT HANDLED
                // {"1.) The first item. 2.) The second item.",
                // std::vector<std::string>{ "1.) The first item.","2.) The second item." }
                // },
                // //33 - NOT HANDLED
                // {"1) The first item 2) The second item",
                // std::vector<std::string>{ "1) The first item","2) The second item" }
                // },
                // //34 - NOT HANDLED
                // {"1) The first item. 2) The second item.",
                // std::vector<std::string>{ "1) The first item.","2) The second item." }
                // },
                // //35 - NOT HANDLED
                // {"1. The first item 2. The second item",
                // std::vector<std::string>{ "1. The first item","2. The second item" }
                // },
                // //36 - NOT HANDLED
                // {"1. The first item. 2. The second item.",
                // std::vector<std::string>{ "1. The first item.","2. The second item." }
                // },
                // //37 - NOT HANDLED
                // {"• 9. The first item • 10. The second item",
                // std::vector<std::string>{ "• 9. The first item","• 10. The second item" }
                // },
                // //38 - NOT HANDLED
                // {"⁃9. The first item ⁃10. The second item",
                // std::vector<std::string>{ "⁃9. The first item","⁃10. The second item" }
                // },
                // //39 - NOT HANDLED
                // {"a. The first item b. The second item c. The third list item",
                // std::vector<std::string>{ "a. The first item","b. The second item","c. The third list item" }
                // },
                // //40 - NOT HANDLED. - CAN ONLY BE HANDLED WITH CLEANING
                // { "This is a sentence\ncut off in the middle because pdf.",
                // std::vector<std::string>{ "This is a sentence\ncut off in the middle because pdf."}
                // },
                // //41 - NOT HANDLED - CAN ONLY BE HANDLED WITH CLEANING
                // { "It was a cold \nnight in the city.",
                // std::vector<std::string>{ "It was a cold night in the city."}
                // },
                // //42
                // { "features\ncontact manager\nevents, activities\n",
                // std::vector<std::string>{ "features", "contact manager", "events, activities" }
                // },
                //43
                {"You can find it at N°. 1026.253.553. That is where the treasure is.",
                std::vector<std::string>{ "You can find it at N°. 1026.253.553.","That is where the treasure is." }
                },
                //44
                {"She works at Yahoo! in the accounting department.",
                std::vector<std::string>{ "She works at Yahoo! in the accounting department." }
                },
                //45
                {"We make a good team, you and I. Did you see Albert I. Jones yesterday?",
                std::vector<std::string>{ "We make a good team, you and I.","Did you see Albert I. Jones yesterday?" }
                },
                //46
                {"Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”",
                std::vector<std::string>{ "Thoreau argues that by simplifying one’s life, “the laws of the universe will appear less complex. . . .”" }
                },
                //47
                {"\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55).",
                std::vector<std::string>{ "\"Bohr [...] used the analogy of parallel stairways [...]\" (Smith 55)." }
                },
                //48
                {"If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . . Next sentence.",
                std::vector<std::string>{ "If words are left off at the end of a sentence, and that is all that is omitted, indicate the omission with ellipsis marks (preceded and followed by a space) and then indicate the end of the sentence with a period . . . .","Next sentence." }
                },
                //49
                {"I never meant that.... She left the store.",
                std::vector<std::string>{ "I never meant that....","She left the store." }
                },
                //50
                {"I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it.",
                std::vector<std::string>{ "I wasn’t really ... well, what I mean...see . . . what I'm saying, the thing is . . . I didn’t mean it." }
                },
                //51
                {"One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds. . . . The practice was not abandoned. . . .",
                std::vector<std::string>{ "One further habit which was somewhat weakened . . . was that of combining words into self-interpreting compounds.",". . . The practice was not abandoned. . . ." }
                },
                // // 52 - NOT HANDLED REQUIRES CLEANING
                // {"Hello world.Today is Tuesday.Mr. Smith went to the store and bought 1,000.That is a lot.",
                // std::vector<std::string>{ "Hello world.", "Today is Tuesday.", "Mr. Smith went to the store and bought 1,000.", "That is a lot." }
                // }

                };

    run_tests(testcases, pisah);

}

TEST_CASE("Custom Tests") {
    Pisah pisah("en");
    std::vector<std::pair<std::string, std::vector<std::string>>> testcases =

                {//1
                {"..\n.",
                std::vector<std::string>{ "..\n." }
                },
                //2
                {"\n",
                std::vector<std::string>{  }
                }
                };
    run_tests(testcases, pisah);

}
