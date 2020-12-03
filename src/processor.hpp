#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

class Processor {

private:
  std::string_view text_; // original text to be processed
  const std::string
      lang_; // 2-letter ISO code of the language, e.g. en, zh, ja e3t

public:
  Processor(std::string_view, const std::string &);

  static pcrecpp::RE_Options options;

  // regexes
  static const pcrecpp::RE re_fullstop;

  std::vector<std::string_view> process();
};