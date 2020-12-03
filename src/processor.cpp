#include <string>
#include <string_view>
#include <vector>

#include <pcrecpp.h>

#include "processor.hpp"
#include "utils.hpp"

Processor::Processor(std::string_view text, const std::string &lang)
    : text_(text), lang_(lang){};

pcrecpp::RE_Options Processor::options = pcrecpp::RE_Options(PCRE_UTF8);

const pcrecpp::RE Processor::re_fullstop = pcrecpp::RE("([^\\.]*\\.)", options);

std::vector<std::string_view> Processor::process() {

  std::vector<std::string_view> str_list;
  pcrecpp::StringPiece sent;

  // StringPiece
  pcrecpp::StringPiece text_sp = Utils::strview_to_strpiece(text_);

  // read the string in a loop
  while (re_fullstop.Consume(&text_sp, &sent)) {
    str_list.push_back(Utils::strpiece_to_strview(sent));
  }
  return str_list;
}