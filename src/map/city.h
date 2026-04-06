#include <queue>
#include <string>

#include "type.h"

class Case;
class Player;

struct Culture {
  int culture;
  int cases;  // Cases possédées autour de la ville
};

extern const std::map<int, Culture> CITY_CULTURE;

struct ResourceProgress {
  int accumulated;         // Ce qui est en stock
  int yield;               // Ce qui est produit ce tour
  double bonus_buildings;  // Bonus en % (ex: +0.25% avec une bibliothèque)

  /** @brief Calcul le gain réel net */
  int get_net_gain() const {
    return static_cast<int>(yield * (1.0 + bonus_buildings));
  }
};

struct CityData {
  ResourceProgress food = {0, 0, 0};
  ResourceProgress culture = {0, 0, 0};
  int production_yield = 0;
  int commerce_yield = 0;
  int science_yield = 0;
  int population = 0;

  /** @brief Calcule le seuil de nourriture nécessaire pour le prochain citoyen
   */
  int get_growth_threshold() const { return 20 + (population * 2); };
};

struct ProductionOrder {
  UnitName unit = UnitName::None;
  // BuildingName building = BuildingName::None;
  int cost;         // Coût total en production
  int accumulated;  // Production déjà accumulée
};

class City {
 public:
  City() = default;
  City(Case* city_case, Player* player);
  ~City();

  /** @brief Calcule le rendement brut de la ville */
  Yields calculate_yields() const;

  void update_data();
  void update_food();
  void update_production();

  /** @brief Ajoute une unité à la file */
  void push_unit(UnitName unit);

  // /** @brief Ajoute un bâtiment à la file */
  // void push_building(BuildingName building);

  bool is_capital() const { return (_id == 1); }
  int get_id() const { return _id; }

  CityData get_city_data() { return _data; }

  void set_city_case(Case* c) { _city_case = c; }
  void set_player(Player* p) { _player = p; }

  Player* get_player() const { return _player; }
  Case* get_city_case() const { return _city_case; }

 private:
  static int _id_counter;

  int _id;
  int _health_limit;
  int _happiness_limit;
  CityData _data;
  std::queue<ProductionOrder> _build_queue;
  Player* _player;
  Case* _city_case;
};