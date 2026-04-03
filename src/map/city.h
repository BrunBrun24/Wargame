#include "type.h"

class Case;
class Player;

class City {
 public:
  City() = default;
  City(Case* city_case, Player* player);
  ~City();

  /** @brief Calcule le rendement brut de la ville */
  Yields calculate_base_yields(const Case* city_case) const;

  /** @brief Calcule le seuil de nourriture nécessaire pour le prochain citoyen
   */
  int get_growth_threshold() const { return 10 + (_population * 2); };

  void process_consumption_food(Yields& final_yields);
  void process_consumption_commerce(Yields& final_yields);
  void process_consumption_production(Yields& final_yields);

  int get_id() const { return _id; }

  void set_city_case(Case* c) { _city_case = c; }
  void set_player(Player* p) { _player = p; }

  Player* get_player() const { return _player; }
  Case* get_case_city() const { return _city_case; }

 private:
  static int _id_counter;

  int _id;
  int _population;
  int _accumulated_food;
  int _accumulated_production;
  int _health_limit;
  int _happiness_limit;
  UnitName _current_production;
  Player* _player;
  Case* _city_case;
};