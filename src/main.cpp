#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

#include "berpecah.hpp"

int main(int argc, char *argv[]) {
	std::locale::global(std::locale("en_US.UTF-8"));
  std::string lang = "en";
  Berpecah b(lang, true);

  std::string text;
  std::string line;
  text.reserve(10000);

  while (std::getline(std::cin, line) && !std::cin.eof()) {
    if (!text.empty()) {
      text += "\n";
    }
    text += line;
  }
  auto pieces = b.Segment(text);
  for (int i = 0; i < pieces.size(); i++) {
    std::cout << i << " ||| " << pieces[i] << std::endl;
  }
};
