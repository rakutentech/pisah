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
      "a|being|did|for|he|how|however|i|in|it|millions|more|she|that|the|"
      "there|they|we|what|when|where|who|why";
  file_exts = 
      "jpe?g|png|gif|tiff?|pdf|ps|docx?|xlsx?|svg|bmp|tga|exif|odt|html?|"
      "txt|rtf|bat|sxw|xml|zip|exe|msi|blend|wmv|mp[34]|pptx?|flac|rb|cpp|cs|js";

  // TODO: Bug on  ǃʼOǃKung this word both exclamations not recognized
  // there are two kind of exclamations that look the same. ignored for the time 
  // being
  exclaim_words =  "!Xũ|!Kung|ǃʼOǃKung|!Kung-Ekoka|!Xuun|!Hu|!Khung|!Ku|!ung|!Xo|!Xû|!Xung|!Xun|Yahoo!|Y!|J Yum!";

  debug_ = true;

  // Replacement Regexes
  rule_map_.emplace("NewlineRule", std::make_unique<Rule>(Rule("\n", "\r")));

  // ABBREVIATION RULES
  // http://rubular.com/r/yqa4Rit8EY --> Ex: Jr.'s --> Jr∯'s
  rule_map_.emplace(
      "PossessiveAbbreviationRule",
      std::make_unique<Rule>(Rule("\\.(?='s(\\s|" + eos + "))", u8"∯")));

  // http://rubular.com/r/e3H6kwnr6H --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace(
      "SingleUpperCaseLetterAtStartOfLineRule",
      std::make_unique<Rule>(Rule("(?<=^[A-Z])\\.(?=\\s)", u8"∯")));

  // http://rubular.com/r/gitvf0YWH4 --> Example: ^Q. This --> Q∯ This
  rule_map_.emplace(
      "SingleUpperCaseLetterRule",
      std::make_unique<Rule>(Rule("(?<=\\s[A-Z])\\.(?=\\s)", u8"∯")));

  // https://regex101.com/r/VhPPOT/2/ --> preceeded by space or at the start of
  // the string,  followed by uppercase char, space, or ":[0-9]+"
  rule_map_.emplace(
      "PrepositiveAbbreviationRule",
      std::make_unique<Rule>(Rule("((?:\\A|\\s)(?:" + prepositive_abbrev +
                                      "))\\.((?-i)[[:upper:]]|\\s|:\\d+)",
                                  u8"\\1∯\\2", Options().set_caseless(true))));

  // https://regex101.com/r/eCgMwp/1// --> preceeded by space or at the start of
  // the string, followed by number, or "\s*("
  rule_map_.emplace(
      "NumberAbbreviationRule",
      std::make_unique<Rule>(
          Rule("((?:\\A|\\s)(?:" + number_abbrev + "))\\.(\\s*(\\d|\\())",
               u8"\\1∯\\2")));

  // https://regex101.com/r/5eZc0s/2/ --> preceeded by space or at the start of
  // the string, followed by ?,.:
  rule_map_.emplace(
      "AbbreviationRule",
      std::make_unique<Rule>(Rule(
          "((?:\\A|\\s)(?:" + abbrev +
              "))\\.((\\.|\\:|-|\\?|\\,)|(\\s([a-z]|I\\s|I'm|I'll|\\d|\\()))",
          u8"\\1∯\\2")));

  // https://regex101.com/r/wR4fJ6/17 : A period(.) preceeded by .[a-z] or
  // \b[a-z] and followed by [a-z]. or [a-z]\b or a space followed by a non
  // uppercase character.
  rule_map_.emplace("LetterPeriodAbbreviationRule",
                    std::make_unique<Rule>(Rule(
                        "(?<=\\.[a-z]|\\b[a-z])\\.(?=[a-z]\\.|[a-z]\\b|\\s)",
                        "∯", Options().set_caseless(true))));

  // https://regex101.com/r/lOiT5h/1/. A.m. and
  rule_map_.emplace(
      "AmPmAbbreviationRule",
      std::make_unique<Rule>(Rule("(?<= [APap]∯[Mm])∯(?=\\s\\p{Lu})", ".")));

  // https://regex101.com/r/TerB5D/2/: few abbreviations followed by usual
  // sentence starters (default=English)
  rule_map_.emplace(
      "AbbreviationSentStarterRule",
      std::make_unique<Rule>(
          Rule("(U∯S|U\\.S|U∯K|E∯U|E\\.U|U∯S∯A|U\\.S\\.A|I|i.v)∯(?=\\s+(" +
                   sent_starters + ")\\b)",
               "\\1.", Options().set_caseless(true))));

  // NUMBER RULES
  // https://regex101.com/r/AizR2R/1/ --> e.g. 3.14 --> 3∯1.4
  rule_map_.emplace("PeriodBeforeNumberRule",
                    std::make_unique<Rule>(Rule("\\.(\\d)", u8"∯\\1")));

  // https://regex101.com/r/PtrPu6/2/
  rule_map_.emplace("NumberAfterPeriodBeforeLetterRule",
                    std::make_unique<Rule>(Rule("(\\d)\\.(\\S)", u8"\\1∯\\2")));

  // https://regex101.com/r/PtrPu6/4/: numbering lists of upto 3 digits
  // (covers starting of the line and upto three digits)
  rule_map_.emplace("NewLineNumberPeriodSpaceLetterRule",
                    std::make_unique<Rule>(Rule(
                        "((?:\\r|\\A)\\d{1,3})\\.(\\s*\\S)", u8"\\1∯\\2")));

  // ADDITIONAL RULES
  // https://regex101.com/r/i4YWZI/2/ alphanum period alphanum (only considering
  // lowercased), typically in  e-mails or usernames without spaces..
  rule_map_.emplace(
      "WithMultiplePeriodsAndEmailRule",
      std::make_unique<Rule>(Rule("([a-z0-9_])(\\.)([a-z0-9_])", u8"\\1∮\\3")));

  // https://regex101.com/r/6JGVof/1/. period after letter followed by degree symbol (°) followed by numbers
  rule_map_.emplace(
      "GeolocationRule",
      std::make_unique<Rule>(Rule("([a-zA-Z]°)\\.(\\s*\\d+)", u8"\\1∯\\2" )));

  // https://regex101.com/r/aUASgT/1/
  rule_map_.emplace("FileFormatRule",
      std::make_unique<Rule>(Rule("\\.(" + file_exts + ")", u8"∯\\1", Options().set_caseless(true) )));

  // ELLIPSES RULES

  // https://regex101.com/r/GwwKpt/1/: replace three periods with spaces and followed by lowercase.
  rule_map_.emplace(
      "ThreePeriodSpacesRule",
      std::make_unique<Rule>(Rule("(\\s\\.){3}\\s", u8"∯ ∯ ∯ " )));  

  // https://regex101.com/r/JXrOZG/1: replace three periods if ellipses followed by another period
  rule_map_.emplace(
      "FourConsecutivePeriodRule",
      std::make_unique<Rule>(Rule("(?<=\\S)\\.{3}(?=\\.\\s\\p{Lu})", u8"∯∯∯" )));  

  // https://regex101.com/r/pTlZiM/1: replace two periods in ellipses, if followed by uppercase letter.
  rule_map_.emplace(
      "ThreeConsecutivePeriodRule",
      std::make_unique<Rule>(Rule("\\.\\.\\.(\\s+\\p{Lu})", u8"∯∯.\\1" )));  

  // replace all three periods (only to be applied afer the previous rules.)
  rule_map_.emplace(
      "OtherThreePeriodRule",
      std::make_unique<Rule>(Rule("\\.\\.\\.", u8"∯∯∯" )));  

  // punct rules
  rule_map_.emplace("PeriodRule", std::make_unique<Rule>(Rule("\\.", u8"∯")));
  rule_map_.emplace("PeriodRevertRule",
                    std::make_unique<Rule>(Rule("∯", "\\.")));

  // Other Regexes

  rule_map_.emplace("ExclaimWordsRegex",
                    std::make_unique<Rule>(Rule("(?<=\\s|\\A)(" + exclaim_words + ")(?=\\s|\\Z)")));

  rule_map_.emplace("NewLineRegex",
                    std::make_unique<Rule>(Rule("([^\\.]+(?:\\n|\\Z))\\s*")));
};

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

// all other additional replacement rules
void Rules::ApplyAdditionalReplacements(std::string &text) {
  ApplyReplace(text, "WithMultiplePeriodsAndEmailRule",
               "GeolocationRule",
               "FileFormatRule");
  ApplyReplace(text, "ThreePeriodSpacesRule",
               "FourConsecutivePeriodRule",
               "ThreeConsecutivePeriodRule",
               "OtherThreePeriodRule");
  ApplyReplaceWithinMatch(text, "ExclaimWordsRegex", "!", "&ᓴ&");

}

// function to replace continuous period abbreviations e.g. U.S.A, I.T. etc.
// TODO: remove. has been replaced with a regex substitution.
// void Rules::ApplyMultiPeriodReplacement(std::string &text) {
//   pcrecpp::StringPiece text_sp = Utils::to_strpiece(text);
//   std::string piece;
//   const auto before = std::chrono::system_clock::now();
//   std::string rule_name = "MultiPeriodAbbreviationRegex" ;
//   while (pcrecpp::RE("(\\b[a-z]\\.)").FindAndConsume(&text_sp, &piece)) {
//     std::cerr <<  "CC" << piece << piece.data() - text_sp.data() <<
//     std::endl; ApplyReplace(piece, "PeriodRule");
//   }
//   const std::chrono::duration<double> duration =
//       (std::chrono::system_clock::now() - before) * 1000;

//   if (debug_ == false) {
//     std::cerr << "After applying " << rule_name << " (took " <<
//     duration.count()
//               << "ms) :\n"
//               << text << "\n"
//               << std::endl; // DEBUG
//   }
// };

std::map<std::string, std::function<std::unique_ptr<Rules>()>>
    Rules::lang_map_ = []() {
      std::map<std::string, std::function<std::unique_ptr<Rules>()>> lang_map;
      lang_map["en"] = [] { return std::make_unique<EnglishRules>(); };
      return lang_map;
    }();

std::unique_ptr<Rules> Rules::CreateLangRules(std::string &lang, bool debug) {
  std::unique_ptr<Rules> rules = Rules::lang_map_[lang]();
  rules->debug_ = debug;
  return rules;
};

// Getter function that returns a copy of the options object to enable
// modification of the preset flags.
pcrecpp::RE_Options Rules::Options() { return Rules::options_; };

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

void Rules::ApplyReplaceWithinMatch(std::string &text, std::string rule_name, std::vector<std::pair<std::string, std::string>> replacement_list) {
  const auto before = std::chrono::system_clock::now();
  Utils::GlobalReplaceWithinMatch(text, GetRuleRegex(rule_name), replacement_list );
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
