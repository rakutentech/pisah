#include <iostream>
#include <memory>
#include <pcrecpp.h>
#include <string>
#include <string_view>
#include <vector>

#include "berpecah.hpp"

int main(int argc, char *argv[]) {

  std::string lang = "en";
  Berpecah b(lang);
  std::string sample = "This is a sample text. Hello World.";
  std::string_view sv = sample;
  auto pieces = b.segment(sv);
  for (auto piece : pieces) {
    std::cout << "hi" << piece << std::endl;
  }
};
