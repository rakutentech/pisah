#include <string>
#include <string_view>

#include <pcrecpp.h>

#include "utils.hpp"

pcrecpp::StringPiece Utils::strview_to_strpiece(std::string_view text_sv) {
  pcrecpp::StringPiece text_sp(text_sv.data(), text_sv.size());
  return text_sp;
}

std::string_view Utils::strpiece_to_strview(pcrecpp::StringPiece text_sp) {
  std::string_view text_sv(text_sp.data(), size_t(text_sp.size()));
  return text_sv;
}
