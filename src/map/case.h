#include <vector>

#include "terrains.h"
#include "units.h"

class Case {
 public:
  void add_neighbor(Case* neighbor);
  void add_unit(Case& c, Unit& unit);
  void delete_unit(Case& target_case, Unit& unit_to_move);

  bool movement_is_possible(const Case& target_case, const Unit unit) const;
  bool movement_is_possible_rec(const Case& target_case, const Unit unit,
                                int speed,
                                std::vector<const Case*>& visited) const;
  void movement(Case& target_case, Unit& unit_to_move);

 private:
  std::vector<Case*> _neighbors;  // Cases adjacentes
  std::vector<Unit*> _units;      // Unité sur la case
  Terrains _terrains;
};