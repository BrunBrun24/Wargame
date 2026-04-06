#pragma once

#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(UnitName name, Player* player, Case* case_unit);

  /** @brief Retourne une liste d'actions possibles que l'unité peut effectuer
   */
  virtual std::vector<UnitAction> get_unit_actions(const Unit* unit);
};
