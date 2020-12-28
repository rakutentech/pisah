#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Berpecah {
private:
  const std::string lang_;

public:
  Berpecah(const std::string &lang, bool debug);
  Berpecah(const std::string &lang);
  std::vector<std::string> Segment(std::string text);
  bool debug_;
};
