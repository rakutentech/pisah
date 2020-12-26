#include <chrono>
#include <iostream> //DEBUG
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

#include "langrules/rules.hpp"
#include "processor.hpp"
#include "utils.hpp"

Processor::Processor(std::string text, const std::string &lang)
    : text_(text), lang_(lang) {
  debug_ = true;
  rules_ = Rules::CreateLangRules(lang_, debug_);
};

pcrecpp::RE_Options Processor::options_ = pcrecpp::RE_Options(PCRE_UTF8);

// Regex for matching sentences that end in fullstops.
// const pcrecpp::RE Processor::re_fullstop = pcrecpp::RE("([^\\.]*\\.)",
// options_);

// Returns a vector of string_view objects where each string_view object
// represents an indivdual sentence.
std::vector<std::string> Processor::process() {

  std::vector<std::string> str_list;
  pcrecpp::StringPiece sent;

  const auto before = std::chrono::system_clock::now();

  // TODO: consider removing / explaining this
  // std::string text_conv = text_sp.as_string(); //TODO: optimize

  // std::cout << "before" << text_ << std::endl;
  // pcrecpp::RE("\n").GlobalReplace("ABC", &text_) ;
  rules_->ApplyReplace(text_, "NewlineRule");

  // (A) TODO: list replacement

  // (B) abbreviation replacement
  rules_->ApplyAbbreviationReplacements(text_);

  // (C) number replacements
  rules_->ApplyNumberReplacements(text_);

  // -- stub --
  pcrecpp::StringPiece text_sp = Utils::to_strpiece(text_);
  while (rules_->GetRuleRegex("NewLineRegex").FindAndConsume(&text_sp, &sent)) {
    str_list.push_back(sent.as_string());
  }

  const std::chrono::duration<double> duration =
      (std::chrono::system_clock::now() - before) * 1000;
  if (debug_ == true) {
    std::cerr << "In total, took " << duration.count() << "ms"
              << std::endl; // DEBUG
  }

  return str_list;
}