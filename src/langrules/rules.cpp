#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <pcrecpp.h>

#include "english_rules.hpp"
#include "rules.hpp"

Rule::Rule(std::string pat, std::string repl,
           const pcrecpp::RE_Options &options)
    : regex_(pcrecpp::RE(pat, options)), pat_(std::move(pat)),
      repl_(std::move(repl)){};

Rule::Rule(std::string pat, const pcrecpp::RE_Options &options)
    : regex_(pcrecpp::RE(pat, options)), pat_(std::move(pat)){};

Rule::Rule(std::string pat, std::string repl)
    : Rule::Rule(pat, repl, Rules::options_){};

Rule::Rule(std::string pat) : Rule::Rule(pat, Rules::options_){};

// function to get regex
pcrecpp::RE Rule::Regex() { return regex_; }

// function to apply regex to replace.
void Rule::Replace(std::string &text) {
  // if there is a replacement stirng for the rule.
  if (!repl_.empty()) {
    regex_.GlobalReplace(repl_, &text);
  } else {
    throw std::logic_error("Replacement string is not set for the rule " +
                           pat_);
  }
}

Rules::Rules() {

  // Constants
  eos_punct = "[\\!\\?\\.]";
  eos = "" + eos + "|\\n|\\r|\\z";
  prepositive_abbrev =
      "adm|attys|brig|capt|cmdr|col|cpl|det|dr|gen|gov|ing|lt|maj|mr|mrs|ms|mt|"
      "messrs|mssrs|prof|ph|rep|reps|rev|sen|sens|sgt|st|supt|v|vs|fig";
  number_abbrev = "art|ext|no|nos|p|pp";
  abbrev =
      "adj|adm|adv|al|ala|alta|apr|arc|ariz|ark|art|assn|asst|attys|aug|ave|"
      "bart|bld|bldg|blvd|brig|bros|btw|cal|calif|capt|cl|cmdr|co|col|colo|"
      "comdr|con|conn|corp|cpl|cres|ct|d\\.phil|dak|dec|del|dept|det|dist|dr|"
      "dr\\.phil|dr\\.philos|drs|e\\.g|ens|esp|esq|etc|exp|expy|ext|feb|fed|"
      "fla|ft|fwy|fy|ga|gen|gov|hon|hosp|hr|hway|hwy|i.e|ia|id|ida|ill|inc|ind|"
      "ing|insp|is|jan|jr|jul|jun|kan|kans|ken|ky|la|lt|ltd|maj|man|mar|mass|"
      "may|md|me|med|messrs|mex|mfg|mich|min|minn|miss|mlle|mm|mme|mo|mont|mr|"
      "mrs|ms|msgr|mssrs|mt|mtn|neb|nebr|nev|no|nos|nov|nr|oct|ok|okla|ont|op|"
      "ord|ore|p|pa|pd|pde|penn|penna|pfc|ph|ph\\.d|pl|plz|pp|prof|pvt|que|rd|"
      "rs|ref|rep|reps|res|rev|rt|sask|sec|sen|sens|sep|sept|sfc|sgt|sr|st|"
      "supt|surg|tce|tenn|tex|univ|usafa|u\\.s|ut|va|v|ver|viz|vs|vt|wash|wis|"
      "wisc|wy|wyo|yuk|fig";
  debug_ = true;

  // Replacement Regexes
  rule_map_.emplace("NewlineRule", std::make_unique<Rule>(Rule("\n", "\n")));

  // http://rubular.com/r/yqa4Rit8EY --> Ex: Jr.'s --> Jr∯'s
  rule_map_.emplace(
      "PossessiveAbbreviationRule",
      std::make_unique<Rule>(Rule("\\.(?='s(\\s|" + eos + "))", u8"∯")));

  // http://rubular.com/r/e3H6kwnr6H --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace(
      "SingleUpperCaseLetterAtStartOfLineRule",
      std::make_unique<Rule>(Rule("(?<=^[A-Z])\\.(?=\\s)", u8"∯", options())));

  // http://rubular.com/r/gitvf0YWH4 --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace("SingleUpperCaseLetterRule",
                    std::make_unique<Rule>(
                        Rule("(?<=\\s[A-Z])\\.(?=\\s)", u8"∯", options())));

  // https://regex101.com/r/VhPPOT/2/ --> preceeded by space or at the start of
  // the string,  followed by uppercase char, space, or ":[0-9]+"
  rule_map_.emplace(
      "PrepositiveAbbreviationRule",
      std::make_unique<Rule>(Rule("((?:\\A|\\s)(?:" + prepositive_abbrev +
                                      "))\\.((?-i)[[:upper:]]|\\s|:\\d+)",
                                  u8"\\1∯\\2", options().set_caseless(true))));

  // https://regex101.com/r/eCgMwp/1// --> preceeded by space or at the start of
  // the string, followed by number, or "\s*("
  rule_map_.emplace(
      "NumberAbbreviationRule",
      std::make_unique<Rule>(
          Rule("((?:\\A|\\s)(?:" + number_abbrev + "))\\.(\\s*(\\d|\\())",
               u8"\\1∯\\2", options().set_caseless(true))));

  // https://regex101.com/r/5eZc0s/2/ --> preceeded by space or at the start of
  // the string, followed by ?,.:
  rule_map_.emplace(
      "AbbreviationRule",
      std::make_unique<Rule>(Rule(
          "((?:\\A|\\s)(?:" + abbrev +
              "))\\.((\\.|\\:|-|\\?|\\,)|(\\s([a-z]|I\\s|I'm|I'll|\\d|\\()))",
          u8"\\1∯\\2", options())));

  // Other Regexes
  rule_map_.emplace("NewLine", std::make_unique<Rule>(
                                   Rule("([^\\.]+.(\\n|\\z))\\s*", options())));
};

std::map<std::string, std::function<std::unique_ptr<Rules>()>>
    Rules::lang_map_ = []() {
      std::map<std::string, std::function<std::unique_ptr<Rules>()>> lang_map;
      lang_map["en"] = [] { return std::make_unique<EnglishRules>(); };
      return lang_map;
    }();

std::unique_ptr<Rules> Rules::CreateLangRules(std::string &lang) {
  std::unique_ptr<Rules> rules = Rules::lang_map_[lang]();
  return rules;
};

pcrecpp::RE_Options &Rules::options() { return Rules::options_; };

std::shared_ptr<Rule> Rules::GetRule(std::string rule_name) {
  if (!rule_map_.count(rule_name) > 0) {
    throw std::invalid_argument("No such rule " + rule_name);
  }
  return rule_map_[rule_name];
};

pcrecpp::RE Rules::GetRuleRegex(std::string rule_name) {
  auto rule = GetRule(rule_name);
  return rule->Regex();
};

void Rules::ApplyReplace(std::string rule_name, std::string &text) {
  const auto before = std::chrono::system_clock::now();
  GetRule(rule_name)->Replace(text);
  const std::chrono::duration<double> duration =
      (std::chrono::system_clock::now() - before) * 1000;

  if (debug_ == true) {
    std::cerr << "After applying " << rule_name << " (took " << duration.count()
              << "ms) :\n"
              << text << "\n"
              << std::endl; // DEBUG
  }
}
