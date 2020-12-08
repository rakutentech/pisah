#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <pcrecpp.h>

#include "berpecah.hpp"
#include "processor.hpp"

Berpecah::Berpecah(const std::string &lang) : lang_(lang) {
}

// function to segment
std::vector<std::string> Berpecah::segment(std::string input_text) {

  // processor object
  Processor processor { input_text, lang_};

  std::vector<std::string> str_pieces;

  // process function
  str_pieces = processor.process();
  return str_pieces;
};
