#include "pisah.hpp"

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>
#include <args/args.hxx>


int main(int argc, char *argv[]) {

    args::ArgumentParser parser("Pisah - Sentence Splitter");
    args::ValueFlag<std::string>lang(parser, "language", "langauge code of the text ", {'l', "language"}, "en");
    args::Flag debug(parser, "verbose", "to enable logging ", {'v', "verbose"});
    
    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

  std::locale::global(std::locale("en_US.UTF-8"));

  Pisah pisah(args::get(lang), debug);//args::get(lang), debug);

  std::string text;
  std::string line;
  text.reserve(10000);

  while (std::getline(std::cin, line) && !std::cin.eof()) {
    if (!text.empty()) {
      text += "\n";
    }
    text += line;
  }
  auto pieces = pisah.Segment(text);
  for (int i = 0; i < pieces.size(); i++) {
    if (debug == true){
      std::cerr << i << " | " << " ";
    }
    std::cout << pieces[i] ;
    if (pieces[i].back() != '\n'){
      std::cout << std::endl;
    }
  }
};
