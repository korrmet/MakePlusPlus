#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include "Independency/independency.hpp"

class expression
{ public:
  expression(independency::storage& storage);

  bool expand(std::string expression, std::string& result);

  private:
  independency::storage& storage;
};

#endif // EXPRESSION_HPP
