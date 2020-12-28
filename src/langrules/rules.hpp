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

public:
  std::string eos_punct;
  std::string eos;
  std::string prepositive_abbrev;
  std::string number_abbrev;
  std::string abbrev;
  std::string sent_starters;

  std::map<string, std::shared_ptr<Rule>> rule_map_;

  Rules();

  bool debug_;

  // regexes
  inline static pcrecpp::RE_Options options_ = pcrecpp::RE_Options(PCRE_UTF8);

  static std::map<std::string, std::function<std::unique_ptr<Rules>()>>
      lang_map_;
  void static CreateLangRulesMap();

  static std::unique_ptr<Rules> CreateLangRules(string &lang, bool debug);

  pcrecpp::RE_Options Options();
  std::shared_ptr<Rule> GetRule(std::string rule_name);
  pcrecpp::RE GetRuleRegex(std::string rule_name);

  void ApplyReplace(std::string &text, std::string rule_name);

  // template function to allow arbitrary number of rulenames to be passed and
  // applied one after the other.
  template <typename... T>
  void ApplyReplace(std::string &text, std::string rule_name, T... rule_names) {
    ApplyReplace(text, rule_name);
    ApplyReplace(text, rule_names...);
  }

  virtual void ApplyAbbreviationReplacements(std::string &text);
  virtual void ApplyNumberReplacements(std::string &text);
  virtual void ApplyAdditionalReplacements(std::string &text);
};

#endif