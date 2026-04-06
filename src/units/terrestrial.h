#pragma once

#include "units.h"

class Terrestrial : public Unit {
 public:
  Terrestrial(UnitName name, Player* player, Case* case_unit);

  /** @brief Retourne une liste d'actions possibles que l'unité peut effectuer
   */
  virtual std::vector<UnitAction> get_unit_actions(const Unit* unit);

  /**
   * @brief Vérifie si l'unité peut fonder une ville sur sa case actuelle.
   * @return true si l'unité est un colon et que l'emplacement respecte les
   * règles de distance.
   */
  bool can_build_city() const;

  /** @brief Fonde une ville, puis détruit l'unité colon */
  void found_city();
  
  bool chop_down_forest_is_possible();
  void chop_down_forest();
};
