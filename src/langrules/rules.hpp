#ifndef LANGRULES_RULES
#define LANGRULES_RULES

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

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
  std::string eos_punct_repl;
  std::string prepositive_abbrev;
  std::string number_abbrev;
  std::string abbrev;
  std::string sent_starters;
  std::string file_exts;
  std::string exclaim_words;

  std::vector<std::pair<std::string,std::string>> punct_replacements;

  std::map<std::string, std::shared_ptr<Rule>> rule_map_;

  Rules();

  bool debug_;

  // regexes
  inline static pcrecpp::RE_Options options_ = pcrecpp::RE_Options(PCRE_UTF8);

  static std::map<std::string, std::function<std::unique_ptr<Rules>()>>
      lang_map_;
  void static CreateLangRulesMap();

  static std::unique_ptr<Rules> CreateLangRules(std::string &lang, bool debug);

  static pcrecpp::RE_Options Options();
  std::shared_ptr<Rule> GetRule(std::string rule_name);
  pcrecpp::RE GetRuleRegex(std::string rule_name);


  virtual void SetupRules();

  void ApplyReplace(std::string &text, std::string rule_name);

  // template function to allow arbitrary number of rulenames to be passed and
  // applied one after the other.
  template <typename... T>
  void ApplyReplace(std::string &text, std::string rule_name, T... rule_names) {
    ApplyReplace(text, rule_name);
    ApplyReplace(text, rule_names...);
  }

  // TODO: support regex pat. now only string patterns are supported
  // overloaded function for single replacement
  void ApplyReplaceWithinMatch(std::string &text, std::string rule_name, std::string pat, std::string repl);
  // for a list of replacements within match
  void ApplyReplaceWithinMatch(std::string &text, std::string rule_name, std::vector<std::pair<std::string, std::string>> replacement_list, bool inverse = false);

  // customizable functions that can be modified by
  // inheirted language rules
  virtual void ApplyAbbreviationReplacements(std::string &text);
  virtual void ApplyNumberReplacements(std::string &text);
  virtual void ApplyBetweenPunctuationReplacements(std::string &text);

  // function to apply the sequence of rules
  virtual void ApplyRules(std::string &text);

  // post-process the sentences
  virtual void PostProcess(std::string &text);
};

#endif
