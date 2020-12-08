#include <string>
#include <string_view>

#include <pcrecpp.h>

class Utils {
public:
  static pcrecpp::StringPiece to_strpiece(std::string_view);
  static std::string_view from_strpiece(pcrecpp::StringPiece);
};