#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

class Berpecah {
private:
  const std::string lang_;

public:
  Berpecah(const std::string &);
  std::vector<std::string_view> segment(std::string_view);
};
