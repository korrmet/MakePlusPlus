#ifndef CONFIGURATOR_HPP
#define CONFIGURATOR_HPP

#include "Independency/independency.hpp"
#define root independency::path()

class configurator
{ public:
  configurator(independency::storage& storage, 
               independency::storage& temp);

  configurator& force(bool enable = true);
  bool run();

  private:
  independency::storage& storage;
  independency::storage& temp;
  bool force_flag;
};

#endif // CONFIGURATOR_HPP
