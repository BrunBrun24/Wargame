#include <queue>
#include <string>

#include "../type.h"

class Case;
class Player;

/**
 * @brief Seuils de culture par niveau.
 * Key:   Niveau de la ville (int)
 * Value: Points de culture requis pour atteindre ce niveau (int)
 */
using city_level = int;
using culture_threshold = int;

extern const std::map<city_level, culture_threshold> CITY_CULTURE;

struct ResourceProgress {
  int accumulated;         // Ce qui est en stock
  int yield;               // Ce qui est produit ce tour
  double bonus_buildings;  // Bonus en % (ex: +0.25% avec une bibliothèque)
};

struct CityCulture {
  int level;
  ResourceProgress data;
};

struct CityData {
  ResourceProgress food = {0, 0, 0};
  CityCulture culture = {
      1,
      {2, 0, 0}};  // A changé après l'implémentation des technologie (2 -> 0)
  ResourceProgress production = {0, 0, 0};
  ResourceProgress commerce = {0, 0, 0};
  ResourceProgress science = {0, 0, 0};
  int health_yield = 2;  // De base toute les villes ont deux de santé
  int sickness_yield = 0;
  int happiness_yield = 4;
  int unhappiness_yield = 1;
  int population = 1;
  int maintenance_costs = 0;  // Ce que la ville coûte en Or

  /** @brief Calcule le seuil de nourriture nécessaire pour le prochain citoyen
   */
  int get_growth_threshold() const { return 20 + (population * 2); }
};

struct ProductionOrder {
  UnitName unit = UnitName::None;
  // BuildingName building = BuildingName::None;
  int cost;  // Coût total en production
};

class City {
 public:
  City() = default;
  City(Case* city_case, Player* player, bool is_capital);
  ~City();

  void check_for_expansion();

  /** @brief Calcule le rendement de la ville */
  void calculate_yields();

  void update_city();

  int calculate_happiness() const;
  int calculate_unhappiness() const;

  void update_food();
  int calculate_food_consumption(Yields total) const;

  void update_culture();
  void update_production();

  void update_commerce();
  int calculate_maintenance_costs(Yields total) const;

  /** @brief Ajoute une unité à la file */
  void push_unit(UnitName unit);

  // /** @brief Ajoute un bâtiment à la file */
  // void push_building(BuildingName building);

  int get_id() const { return _id; }
  bool is_capital() const { return _is_capital; }

  void set_city_case(Case* c) { _city_case = c; }
  void set_player(Player* p) { _player = p; }

  Player* get_player() const { return _player; }
  Case* get_city_case() const { return _city_case; }

 private:
  static int _id_counter;

  int _id;
  bool _is_capital;
  CityData _data;
  std::queue<ProductionOrder> _build_queue;
  Player* _player;
  Case* _city_case;
  std::vector<Case*> _squares_owned;
};
