#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

#include "berpecah.hpp"
#include "processor.hpp"

Berpecah::Berpecah(const std::string &lang) : lang_(lang) {
  std::cout << "Object Created" << std::endl;
}

// function to segment
std::vector<std::string_view> Berpecah::segment(std::string_view input_text) {

  // processor object
  Processor processor(input_text, lang_);

  std::vector<std::string_view> str_pieces;

  // process function
  str_pieces = processor.process();

  return str_pieces;
};
