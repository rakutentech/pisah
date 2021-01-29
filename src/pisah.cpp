#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <pcrecpp.h>

#include "pisah.hpp"
#include "processor.hpp"

Pisah::Pisah(const std::string &lang, bool debug)
    : lang_(lang), debug_(debug) {}
Pisah::Pisah(const std::string &lang) : lang_(lang), debug_(false) {}

  // function to segment
  std::vector<std::string> Pisah::Segment(std::string text) {

  // processor object
  Processor processor{text, lang_, debug_};

  // process function
  std::vector<std::string> str_pieces = processor.process();

  return str_pieces;
};
