#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Pisah {
private:
  std::string lang_;

public:
  Pisah(std::string lang, bool debug);
  Pisah(std::string lang);
  std::vector<std::string> Segment(std::string text);
  bool debug_;
};
