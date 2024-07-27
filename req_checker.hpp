#ifndef REQ_CHECKER_HPP
#define REQ_CHECKER_HPP

#include "Independency/independency.hpp"

class req_checker
{ public:
  req_checker(independency::storage& storage);
  bool check();

  private:
  independency::storage& storage;
};

#endif // REQ_CHECKER_HPP
