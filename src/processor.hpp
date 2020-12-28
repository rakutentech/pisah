#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

#include "langrules/rules.hpp"

class Processor {

private:
  std::string text_; // original text to be processed
  std::string lang_; // 2-letter ISO code of the language
  std::unique_ptr<Rules> rules_;
  bool debug_; // debug flag

public:
  // Constructor that takes in the text to process and the language code.
  Processor(std::string text, const std::string &lang);
  Processor(std::string text, const std::string &lang, bool debug);

  // PCRE options object
  static pcrecpp::RE_Options options_;

  std::vector<std::string> process();
};