#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <pcrecpp.h>

#include "../utils.hpp"
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

// TODO: change options to Rule class.?
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
  eos_punct = u8"。．\\.！\\!\\?？";


  prepositive_abbrev =
      "adm|attys|brig|capt|cmdr|col|cpl|det|dr|gen|gov|ing|lt|maj|mr|mrs|ms|mt|"
      "messrs|mssrs|prof|ph|rep|reps|rev|sen|sens|sgt|st|supt|v|vs|fig";
  number_abbrev = "art|ext|no|nos|p|pp";
  abbrev =
      "adj|adm|adv|al|ala|alta|apr|arc|ariz|ark|art|assn|asst|attys|aug|ave|"
      "bart|bld|bldg|blvd|brig|bros|btw|cal|calif|capt|cl|cmdr|co|col|colo|"
      "comdr|con|conn|corp|cpl|cres|ct|d\\.phil|dak|dec|del|dept|det|dist|dr|"
      "dr\\.phil|dr\\.philos|drs|e\\.g|ens|esp|esq|etc|exp|expy|ext|feb|fed|"
      "fla|ft|fwy|fy|ga|gen|gov|hon|hosp|hr|hway|hwy|i\\.e|ia|id|ida|ill|inc|"
      "ind|"
      "ing|insp|is|jan|jr|jul|jun|kan|kans|ken|ky|la|lt|ltd|maj|man|mar|mass|"
      "may|md|me|med|messrs|mex|mfg|mich|min|minn|miss|mlle|mm|mme|mo|mont|mr|"
      "mrs|ms|msgr|mssrs|mt|mtn|neb|nebr|nev|no|nos|nov|nr|oct|ok|okla|ont|op|"
      "ord|ore|p|pa|pd|pde|penn|penna|pfc|ph|ph\\.d|pl|plz|pp|prof|pvt|que|rd|"
      "rs|ref|rep|reps|res|rev|rt|sask|sec|sen|sens|sep|sept|sfc|sgt|sr|st|"
      "supt|surg|tce|tenn|tex|univ|usafa|u\\.s|ut|va|v|ver|viz|vs|vt|wash|wis|"
      "wisc|wy|wyo|yuk|fig";
  sent_starters =
      "A|Being|Did|For|He|How|However|I|In|It|Millions|More|She|That|The|"
      "There|They|We|What|When|Where|Who|Why";
  file_exts =
      "jpe?g|png|gif|tiff?|pdf|ps|docx?|xlsx?|svg|bmp|tga|exif|odt|html?|"
      "txt|rtf|bat|sxw|xml|zip|exe|msi|blend|wmv|mp[34]|pptx?|flac|rb|cpp|cs|js";

  // TODO: Bug on  ǃʼOǃKung this word both exclamations not recognized
  // there are two kind of exclamations that look the same. ignored for the time
  // being
  exclaim_words =  "!Xũ|!Kung|ǃʼOǃKung|!Kung-Ekoka|!Xuun|!Hu|!Khung|!Ku|!ung|!Xo|!Xû|!Xung|!Xun|Yahoo!|Y!|J Yum!";

  // punctuationr peplacement list
  punct_replacements =  { {".",u8"∯"}, {u8"。",u8"&ᓰ&"}, {u8"．",u8"&ᓱ&"}, {u8"！",u8"&ᓳ&"},
                          {"!",u8"&ᓴ&"}, {"?", u8"&ᓷ&"}, {u8"？",u8"&ᓸ&"}, {"'",u8"&⎋&"} };

  debug_ = true;


};

void Rules::SetupRules(){

  // Uncomment if you want reverse replacement for any rule
  // eos_punct_repl = "";
  // for (auto p: punct_replacements){
  //   if (eos_punct_repl != ""){
  //     eos_punct_repl += "|";
  //   }
  //    eos_punct_repl += p.second;
  // }


  // TODO: make rules() function ot invoked to create all rules with the set of variables

  // Replacement Regexes
     rule_map_.emplace("NewLineDosRule", std::make_unique<Rule>(Rule("\\r\\n", "\n")));
     rule_map_.emplace("NewLineMacRule", std::make_unique<Rule>(Rule("\\r", "\n")));

  // ABBREVIATION RULES
  // http://rubular.com/r/yqa4Rit8EY --> Ex: Jr.'s --> Jr∯'s
  rule_map_.emplace(
      "PossessiveAbbreviationRule",
      std::make_unique<Rule>(Rule("\\.(?='s(\\p{Z}|[" + eos_punct + "]|\\Z))", u8"∯")));

  // http://rubular.com/r/e3H6kwnr6H --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace(
      "SingleUpperCaseLetterAtStartOfLineRule",
      std::make_unique<Rule>(Rule("(?<=^\\p{Lu})\\.(?=\\p{Z})", u8"∯")));

  // http://rubular.com/r/gitvf0YWH4 --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace(
      "SingleUpperCaseLetterRule",
      std::make_unique<Rule>(Rule("(?<=\\p{Z}[A-Z])\\.(?=\\p{Z})", u8"∯")));

  // https://regex101.com/r/VhPPOT/2/ --> preceeded by space or at the start of
  // the string,  followed by uppercase char, space, or ":[0-9]+"
  rule_map_.emplace(
      "PrepositiveAbbreviationRule",
      std::make_unique<Rule>(Rule("((?:\\A|\\p{Z}|["+eos_punct+"])(?:" + prepositive_abbrev +
                                      "))\\.((?-i)[[:upper:]]|\\p{Z}|:\\p{N}+)",
                                  u8"\\1∯\\2", Options().set_caseless(true))));

  // https://regex101.com/r/eCgMwp/1// --> preceeded by space or at the start of
  // the string, followed by number, or "\s*("
  rule_map_.emplace(
      "NumberAbbreviationRule",
      std::make_unique<Rule>(
          Rule("((?:\\A|\\p{Z})(?:" + number_abbrev + "))\\.(\\p{Z}*(\\p{N}|\\())",
               u8"\\1∯\\2")));

  // https://regex101.com/r/5eZc0s/2/ --> preceeded by space or at the start of
  // the string, followed by ?,.:
  rule_map_.emplace(
      "AbbreviationRule",
      std::make_unique<Rule>(Rule(
          "((?:\\A|\\p{Z}|["+eos_punct+"]})(?:" + abbrev +
              "))\\.((\\.|\\:|-|\\?|\\,)|(\\p{Z}(\\p{Ll}|I\\p{Z}|I'm|I'll|\\d|\\()))",
          u8"\\1∯\\2", Options().set_caseless(true))));

  // https://regex101.com/r/wR4fJ6/17 : A period(.) preceeded by .[a-z] or
  // \b[a-z] and followed by [a-z]. or [a-z]\b or a space followed by a non
  // uppercase character.
  rule_map_.emplace("LetterPeriodAbbreviationRule",
                    std::make_unique<Rule>(Rule(
                        "(?<=\\.[a-z]|\\b[a-z])\\.(?=[a-z]\\.|[a-z]\\b|\\p{Z})",
                        "∯", Options().set_caseless(true))));

  // https://regex101.com/r/lOiT5h/1/. A.m. and
  rule_map_.emplace(
      "AmPmAbbreviationRule",
      std::make_unique<Rule>(Rule("(?<= [APap]∯[Mm])∯(?=\\p{Z}\\p{Lu})", ".")));

  // https://regex101.com/r/TerB5D/2/: few abbreviations followed by usual
  // sentence starters (default=English)
  rule_map_.emplace(
      "AbbreviationSentStarterRule",
      std::make_unique<Rule>(
          Rule("(U∯S|U\\.S|U∯K|E∯U|E\\.U|U∯S∯A|U\\.S\\.A|I|i.v)∯(?=\\p{Z}+((?-i)" +
                   sent_starters + ")\\b)",
               "\\1.", Options().set_caseless(true))));

  // NUMBER RULES
  // https://regex101.com/r/AizR2R/1/ --> e.g. 3.14 --> 3∯1.4
  rule_map_.emplace("PeriodBeforeNumberRule",
                    std::make_unique<Rule>(Rule("\\.(\\p{N})", u8"∯\\1")));

  // https://regex101.com/r/PtrPu6/2/
  rule_map_.emplace("NumberAfterPeriodBeforeLetterRule",
                    std::make_unique<Rule>(Rule("(\\d)\\.(\\P{Z})", u8"\\1∯\\2")));

  // https://regex101.com/r/PtrPu6/4/: numbering lists of upto 3 digits
  // (covers starting of the line and upto three digits)
  rule_map_.emplace("NewLineNumberPeriodSpaceLetterRule",
                    std::make_unique<Rule>(Rule(
                        "((?:\\R|\\A)\\d{1,3})\\.(\\p{Z}*\\P{Z})", u8"\\1∯\\2")));

  // ADDITIONAL RULES
  // https://regex101.com/r/i4YWZI/2/ alphanum period alphanum
  //, typically in  e-mails or usernames without spaces..
  rule_map_.emplace(
      "WithMultiplePeriodsAndEmailRule",
      std::make_unique<Rule>(Rule("([a-zA-Z0-9_])(\\.)([a-zA-Z0-9_])", u8"\\1∯\\3")));

  // https://regex101.com/r/6JGVof/1/. period after letter followed by degree symbol (°) followed by numbers
  rule_map_.emplace(
      "GeolocationRule",
      std::make_unique<Rule>(Rule("(\\p{L}°)\\.(\\p{Z}*\\p{N}+)", u8"\\1∯\\2" )));

  // https://regex101.com/r/aUASgT/1/
  rule_map_.emplace("FileFormatRule",
      std::make_unique<Rule>(Rule("\\.(" + file_exts + ")", u8"∯\\1", Options().set_caseless(true) )));

  // ELLIPSES RULES

  // https://regex101.com/r/GwwKpt/1/: replace three periods with spaces and followed by lowercase.
  rule_map_.emplace(
      "ThreePeriodSpacesRule",
      std::make_unique<Rule>(Rule("(\\p{Z}\\.){3}\\p{Z}", u8" ∯ ∯ ∯ " )));

  // https://regex101.com/r/JXrOZG/1: replace three periods if ellipses followed by another period
  rule_map_.emplace(
      "FourConsecutivePeriodRule",
      std::make_unique<Rule>(Rule("(?<=\\P{Z})\\.{3}(?=\\.\\p{Z}\\p{Lu})", u8"∯∯∯" )));

  // https://regex101.com/r/pTlZiM/1: replace two periods in ellipses, if followed by uppercase letter.
  rule_map_.emplace(
      "ThreeConsecutivePeriodRule",
      std::make_unique<Rule>(Rule("\\.\\.\\.(\\p{Z}*\\p{Lu})", u8"∯∯.\\1" )));

  // replace all three periods (only to be applied afer the previous rules.)
  rule_map_.emplace(
      "OtherThreePeriodRule",
      std::make_unique<Rule>(Rule("\\.\\.\\.", u8"∯∯∯" )));

  // punct rules
  rule_map_.emplace("PeriodRule", std::make_unique<Rule>(Rule("\\.", u8"∯")));
  rule_map_.emplace("PeriodRevertRule",
                    std::make_unique<Rule>(Rule("∯", "\\.")));

  // OTHER REGEXES
  // -------------------------------

  rule_map_.emplace("ExclaimWordsRegex",
                    std::make_unique<Rule>(Rule("(?<=\\p{Z}|\\A)(" + exclaim_words + ")(?=\\p{Z}|\\Z)")));

  // add newline in between if quotes end  sentence
  rule_map_.emplace("QuoteEndSentence",
                    std::make_unique<Rule>(Rule(u8"([.?][\'\"’”]\\p{Zs})(\\p{Lu})","\\1╿\\2" )));

  // Rubular: http://rubular.com/r/2YFrKWQUYi
  rule_map_.emplace("BetweenNeutralQuotes",
                    std::make_unique<Rule>(Rule("(?<=\\p{Z}|\\A)((?<nquote>['\"])([^'\"]|'\\p{L})*\\k<nquote>)")));

  // NOTE: make sure the complete pattern is captured for
  // replacing within match
  rule_map_.emplace("BetweenSlantedSingleQuotes",
                    std::make_unique<Rule>(Rule(u8"(?<=\\p{Z}|\\A)(‘([^’]|’\\p{L})*’)")));

  rule_map_.emplace("BetweenSlantedDoubleQuotes",
                    std::make_unique<Rule>(Rule(u8"(“[^”]*”)")));


  rule_map_.emplace("BetweenSquareBrackets",
                    std::make_unique<Rule>(Rule(u8"(\\[[^\\]]*\\])")));


  rule_map_.emplace("BetweenParanthesis",
                    std::make_unique<Rule>(Rule(u8"(\\([^\\)]*\\))")));

  rule_map_.emplace("BetweenArrowQuotes",
                    std::make_unique<Rule>(Rule(u8"(«[^»]*»)")));

  rule_map_.emplace("BetweenBlockQuotes",
                    std::make_unique<Rule>(Rule(u8"(「[^」]*」)")));

  rule_map_.emplace("BetweenDoubleBlockQuotes",
                    std::make_unique<Rule>(Rule(u8"(『[^』]*』)")));

rule_map_.emplace("NewLineRegex",
               std::make_unique<Rule>(Rule(u8"(.+?(?:╿|\\Z|(?:(?:["+eos_punct+"]|\\R)\\p{Zs}*)+))", Options().set_multiline(true) )));

}
// Rule functions which can be overridden in specific language classes
void Rules::ApplyAbbreviationReplacements(std::string &text) {
  ApplyReplace(text, "PossessiveAbbreviationRule",
               "SingleUpperCaseLetterAtStartOfLineRule",
               "SingleUpperCaseLetterRule", "PrepositiveAbbreviationRule",
               "NumberAbbreviationRule", "AbbreviationRule",
               "LetterPeriodAbbreviationRule", "AmPmAbbreviationRule",
               "AbbreviationSentStarterRule");
};

// function to replace periods within numbers
void Rules::ApplyNumberReplacements(std::string &text) {
  ApplyReplace(text, "PeriodBeforeNumberRule",
               "NumberAfterPeriodBeforeLetterRule",
               "NewLineNumberPeriodSpaceLetterRule");
};

// function to replace EOS punctuations within certain sets of punctuations like [], "" etc.
void Rules::ApplyBetweenPunctuationReplacements(std::string &text){
  ApplyReplaceWithinMatch(text, "ExclaimWordsRegex", "!", "&ᓴ&");
  ApplyReplace(text, "QuoteEndSentence");
  ApplyReplaceWithinMatch(text, "BetweenNeutralQuotes", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenSlantedSingleQuotes", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenSlantedDoubleQuotes", punct_replacements);
  // TODO: replace many of these with conditional regexes if possible
  ApplyReplaceWithinMatch(text, "BetweenSquareBrackets", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenParanthesis", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenArrowQuotes", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenBlockQuotes", punct_replacements);
  ApplyReplaceWithinMatch(text, "BetweenDoubleBlockQuotes", punct_replacements);
  // TODO: SKIPPED: between em-dashes?
}

// apply replacement rules
void Rules::ApplyRules(std::string &text) {

  ApplyReplace(text, "NewLineDosRule", "NewLineMacRule");

  // TODO: list replacements are not done. special case handling
  //(i), (ii), etc. and 1. 2. 3. list etc.

  ApplyAbbreviationReplacements(text);
  ApplyNumberReplacements(text);

  ApplyReplace(text, "WithMultiplePeriodsAndEmailRule",
               "GeolocationRule",
               "FileFormatRule");

  ApplyReplace(text, "ThreePeriodSpacesRule",
               "FourConsecutivePeriodRule",
               "ThreeConsecutivePeriodRule",
               "OtherThreePeriodRule");

  ApplyBetweenPunctuationReplacements(text);

  // TODO:? exclamation rules (\!(?=\,\s[a-z]) and '\!(?=\s[a-z]) (too specific.)

}

// substitute the punctuations  back
void Rules::PostProcess(std::string &text){
    std::string orig, repl;
    for (auto punct_subst : punct_replacements){
        orig = punct_subst.first;
        repl = punct_subst.second;
        Utils::FindAndReplaceAll(text, repl, orig);
    }
    Utils::FindAndReplaceAll(text, u8"╿", "");


}

// create rules map for leach language.
std::map<std::string, std::function<std::unique_ptr<Rules>()>>
    Rules::lang_map_ = []() {
      std::map<std::string, std::function<std::unique_ptr<Rules>()>> lang_map;
      lang_map["en"] = [] { return std::make_unique<EnglishRules>(); };
      return lang_map;
    }();

// create the rules object using a factory method
// set debug flag.
std::unique_ptr<Rules> Rules::CreateLangRules(std::string &lang, bool debug) {
  std::unique_ptr<Rules> rules = Rules::lang_map_[lang]();
  rules->debug_ = debug;
  return rules;
};

// Getter function that returns a copy of the options object to enable
// modification of the preset flags.
pcrecpp::RE_Options Rules::Options() { return Rules::options_; };

std::shared_ptr<Rule> Rules::GetRule(std::string rule_name) {
  if (rule_map_.count(rule_name) == 0) {
    throw std::invalid_argument("No such rule " + rule_name);
  }
  return rule_map_[rule_name];
};

// Fnction to get the regex from the rule
pcrecpp::RE Rules::GetRuleRegex(std::string rule_name) {
  auto rule = GetRule(rule_name);
  return rule->Regex();
};

void Rules::ApplyReplace(std::string &text, std::string rule_name) {
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

void Rules::ApplyReplaceWithinMatch(std::string &text, std::string rule_name, std::vector<std::pair<std::string, std::string>> replacement_list, bool inverse) {
  const auto before = std::chrono::system_clock::now();
  Utils::GlobalReplaceWithinMatch(text, GetRuleRegex(rule_name), replacement_list, inverse);
  const std::chrono::duration<double> duration =
      (std::chrono::system_clock::now() - before) * 1000;

  if (debug_ == true) {
    std::cerr << "After applying " << rule_name << " (took " << duration.count()
              << "ms) :\n"
              << text << "\n"
              << std::endl; // DEBUG
  }
}

void Rules::ApplyReplaceWithinMatch (std::string &text, std::string rule_name, std::string pat, std::string repl) {
  std::vector<std::pair<std::string, std::string>> replacement_list = { {pat, repl} };
  ApplyReplaceWithinMatch(text, rule_name, replacement_list);
}
