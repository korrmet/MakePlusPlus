#ifndef BUILDER_HPP
#define BUILDER_HPP

#include "Independency/independency.hpp"

class builder
{ public:
    builder(independency::storage& storage,
            independency::storage& temp);
    bool build();
    void clean();

  private:
    independency::storage storage;
    independency::storage& temp;
    std::string ckstring(std::string input);
};

#endif // BUILDER_HPP
