#include "units.h"

class Neutral : public Unit {
 public:
  Neutral(UnitName name, Player* player, Case* case_unit);
};