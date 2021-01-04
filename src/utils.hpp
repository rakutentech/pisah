#include <string>
#include <string_view>

#include <pcrecpp.h>

class Utils {
public:
  static pcrecpp::StringPiece ToStringPiece(std::string_view text_sv);
  static std::string_view FromStringPiece(pcrecpp::StringPiece text_sp);
  static void FindAndReplaceAll(std::string &text, std::string orig, std::string repl);
  static void GlobalReplaceWithinMatch(std::string &text, pcrecpp::RE re, std::string orig, std::string repl);
};