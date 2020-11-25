#include<memory>
#include<string>
#include<vector>

#include<pcrecpp.h>

class Berpecah{
    public:
        Berpecah();
        std::vector<std::string_view> segment(std::string_view);
};
