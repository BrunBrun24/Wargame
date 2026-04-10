#pragma once

#include <map>
#include <memory>
#include <vector>

#include "../type.h"

class Unit;
class Case;
class City;

using ResourceInventory = std::map<ResourceName, int>;

struct PlayerIncome {
  int gold = 0;
  int science = 0;
};

class Player {
 public:
  Player(Country country);
  ~Player();

  /** @brief Permet à l'utilisateur de choisir son pays dans le terminal */
  static Country choice_country(const std::vector<Country>& excluded_countries);

  /** @brief Méthode calculant l'income et qui sera appelé à chaque
   * déplacement d'unité */
  int depense_gold() const;

  /** @brief Initialise le début du tour.
   * Change l'income du joueur en fonction de la production de chaque ville.
   * Réinitialise les PMs des de chaque unité */
  void initialise_turn();

  void clear_units();
  void add_unit(Unit* unit);
  void remove_unit(Unit* unit);

  void add_improvement(Case* c);
  void remove_improvement(Case* c);

  void clear_citys();
  void add_city(City* c);
  void remove_city(City* c);

  void add_improvement_resource(ResourceName name) {
    _improvements_resources[name] += 1;
  };

  /** @return Retourne la case où se trouve la capitale */
  Case* get_city_capital();

  int get_id() const { return _id; }
  Country get_country() const { return _country; }
  std::vector<City*>& get_citys() { return _citys; }
  PlayerIncome& get_income() { return _income; }
  ResourceInventory get_resources() { return _resources; }

 private:
  static int _id_counter;

  int _id;
  Country _country;
  std::vector<Unit*> _units;
  std::vector<Case*> _improvements;
  std::vector<City*> _citys;
  PlayerIncome _income;
  ResourceInventory _resources;  // Nombre de ressources possédant le joueur
  ResourceInventory
      _improvements_resources;  // Nombre de bâtiments générant une ressource
};