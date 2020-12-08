#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Berpecah {
private:
  const std::string lang_;

public:
  Berpecah(const std::string &);
  std::vector<std::string> segment(std::string);
};
