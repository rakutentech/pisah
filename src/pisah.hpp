#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Pisah {
private:
  const std::string lang_;

public:
  Pisah(const std::string &lang, bool debug);
  Pisah(const std::string &lang);
  std::vector<std::string> Segment(std::string text);
  bool debug_;
};
