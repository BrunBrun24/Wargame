#pragma once

#include "units.h"
#include "../type.h"

class Maritime : public Unit {
 public:
  Maritime(UnitName name, Player* player, Case* case_unit);

  /** @brief Retourne une liste d'actions possibles que l'unité peut effectuer
   */
  std::vector<UnitAction> get_unit_actions() const override;

  private:
    CapacityTransport _capacity_transport;
};