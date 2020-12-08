#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <iostream> //DEBUG

#include <pcrecpp.h>

#include "utils.hpp"
#include "langrules/rules.hpp"
#include "processor.hpp"

Processor::Processor(std::string text, const std::string &lang)
    : text_(text), lang_(lang){
      rules_ = Rules::CreateLangRules(lang_);
    };

pcrecpp::RE_Options Processor::options_ = pcrecpp::RE_Options(PCRE_UTF8);

// Regex for matching sentences that end in fullstops.
//const pcrecpp::RE Processor::re_fullstop = pcrecpp::RE("([^\\.]*\\.)", options_);

// Returns a vector of string_view objects where each string_view object
// represents an indivdual sentence.
std::vector<std::string> Processor::process() {

  std::vector<std::string> str_list;
  pcrecpp::StringPiece sent;
  
  // TODO: consider removing / explaining this
  //std::string text_conv = text_sp.as_string(); //TODO: optimize
  
  // std::cout << "before" << text_ << std::endl;
  //pcrecpp::RE("\n").GlobalReplace("ABC", &text_) ; 
  rules_->ApplyReplace("NewlineRule",text_);

  // (A) TODO: list replacement
  
  // (B) abbreviation replacement
  
  // (1) Possessive Abbrevation: http://rubular.com/r/yqa4Rit8EY
  // PossessiveAbbreviationRule = Rule(r"\.(?='s\s)|\.(?='s$)|\.(?='s\Z)", '∯')
  rules_->ApplyReplace("PossessiveAbbreviationRule", text_);
  
  // (2) self.lang.KommanditgesellschaftRule (Skipped because too specific) http://rubular.com/r/NEv265G2X2
  
  // (3) SingleUpperCaseLetterAtStartOfLineRule: https://rubular.com/r/e3H6kwnr6H
  // Starting with a single upper case character followed by a period.
  rules_->ApplyReplace("SingleUpperCaseLetterAtStartOfLineRule", text_);

  // -- stub --
  pcrecpp::StringPiece text_sp = Utils::to_strpiece(text_);
  while (rules_->GetRuleRegex("NewLine").FindAndConsume(&text_sp, &sent)) {
    str_list.push_back(sent.as_string());

  }
  

  return str_list;
}