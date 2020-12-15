#ifndef LANGRULES_RULES
#define LANGRULES_RULES

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>

#include <pcrecpp.h>

class Rule {
private:
  pcrecpp::RE regex_;
  std::string pat_;
  std::string repl_;

public:
  Rule(std::string pat);
  Rule(std::string pat, std::string repl);
  Rule(std::string pat, const pcrecpp::RE_Options &options);
  Rule(std::string pat, std::string repl, const pcrecpp::RE_Options &options);
  void Replace(std::string &text);
  pcrecpp::RE Regex();
};

class Rules {
private:
  std::string eos_punct;
  std::string eos;
  std::string prepositive_abbrev;
  std::string number_abbrev;
  std::string abbrev;

  std::map<string, std::shared_ptr<Rule>> rule_map_;

public:
  Rules();

  bool debug_;

  // regexes
  inline static pcrecpp::RE_Options options_ = pcrecpp::RE_Options(PCRE_UTF8);

  static std::map<std::string, std::function<std::unique_ptr<Rules>()>>
      lang_map_;
  void static CreateLangRulesMap();

  static std::unique_ptr<Rules> CreateLangRules(string &);

  pcrecpp::RE_Options &options();
  std::shared_ptr<Rule> GetRule(std::string rule_name);
  pcrecpp::RE GetRuleRegex(std::string rule_name);
  void ApplyReplace(std::string rule_name, std::string &text);
};

#endif