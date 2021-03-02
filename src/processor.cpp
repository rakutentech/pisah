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

Processor::Processor(std::string text, const std::string &lang, bool debug)
    : text_(text), lang_(lang), debug_(debug) {
  rules_ = Rules::CreateLangRules(lang_, debug_);
  rules_->SetupRules();
};

Processor::Processor(std::string text, const std::string &lang)
    : Processor::Processor(text, lang, false) {}

// Returns a vector of string objects where each string object
// represents an indivdual sentence.
std::vector<std::string> Processor::process() {

  std::vector<std::string> str_list;
  //pcrecpp::StringPiece sent;
  std::string sent;
  const auto before = std::chrono::system_clock::now();
  text_.append("\n");
  rules_->ApplyRules(text_);

  // -- stub --
  //pcrecpp::StringPiece text_sp = Utils::ToStringPiece(text_);

  // while (rules_->GetRuleRegex("NewLineRegex").FindAndConsume(&text_sp, &sent)) {
  //   //std::string sent_str = sent.as_string();
  //   std::cerr << "PIECE" << sent << std::endl;
  //   str_list.push_back(sent);
  //   }
  str_list = Utils::FindAndConsumeAll(text_, rules_->GetRuleRegex("NewLineRegex"));

  for (auto &sent : str_list){
    rules_->PostProcess(sent);
  }
  // prev_i = 0
  // for(size_t i = 0; i < text_.length();)
  // {
  //     int csize = Utils::FindCodePointSize(text_, i);
  //     if(rules_->GetRuleRegex("EndOfSentenceRegex").FullMatch(text_.substr(i, csize))) {
  //       sent = text_.substr(prev_i, i)
  //     }
  //     std::cout << text_.substr(i, csize) << x << " ";

  //     i += csize;
  // }

  const std::chrono::duration<double> duration =
      (std::chrono::system_clock::now() - before) * 1000;
  if (debug_ == true) {
    std::cerr << "In total, took " << duration.count() << "ms"
              << std::endl; // DEBUG
  }

  return str_list;
}