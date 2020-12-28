#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <pcrecpp.h>

#include "berpecah.hpp"
#include "processor.hpp"

Berpecah::Berpecah(const std::string &lang, bool debug)
    : lang_(lang), debug_(debug) {}
Berpecah::Berpecah(const std::string &lang) : lang_(lang), debug_(false) {}

// function to segment
std::vector<std::string> Berpecah::Segment(std::string text) {

  // processor object
  Processor processor{text, lang_, debug_};

  std::vector<std::string> str_pieces;

  // process function

  str_pieces = processor.process();

  return str_pieces;
};
