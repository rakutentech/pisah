#include <string>
#include <string_view>

#include <pcrecpp.h>

class Utils {
public:
  static pcrecpp::StringPiece strview_to_strpiece(std::string_view);
  static std::string_view strpiece_to_strview(pcrecpp::StringPiece);
};