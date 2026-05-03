#pragma once

#include <map>
#include <memory>
#include <vector>

#include "../type.h"

class Unit;
class Case;
class City;

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

  /** @return Retourne la case où se trouve la capitale */
  Case* get_city_capital();

  bool is_dead() const { return this->_cities.empty(); }

  /**
   * @brief Gère le début du tour pour le joueur.
   * Vérifie les productions des villes et les recherches technologiques.
   */
  void start_turn();

  /**
   * @brief Boucle principale du tour qui attend que toutes les unités aient
   * agi.
   */
  void process_turn_actions();

  /**
   * @brief Vérifie s'il reste des unités capables d'agir.
   * @return true si au moins une unité est active.
   */
  bool has_active_units() const;

  void add_resource(ResourceName name) { _resources[name] += 1; };
  void sub_resource(ResourceName name) { _resources[name] -= 1; };

  int get_id() const { return _id; }
  Country get_country() const { return _country; }
  std::vector<Unit*> get_units() const { return _units; }
  std::vector<City*>& get_citys() { return _cities; }
  PlayerIncome& get_income() { return _income; }
  std::map<ResourceName, int> get_resources() { return _resources; }
  std::vector<TechnologyName> const get_technologies() { return _technologies; }

 private:
  static int _id_counter;

  int _id;
  Country _country;
  std::vector<Unit*> _units;
  std::vector<Case*> _improvements;
  std::vector<City*> _cities;
  PlayerIncome _income;
  std::map<ResourceName, int> _resources;  // Ressources que possède le joueur
  TechnologyName _current_research;
  std::vector<TechnologyName>
      _technologies;  // Technologie que possède le joueur

  /**
   * @brief Parcourt toutes les villes pour s'assurer qu'elles produisent
   * quelque chose.
   */
  void _check_city_productions();

  /** @brief Vérifie si une recherche est en cours, sinon en demande une
   * nouvelle.
   */
  void _check_active_research();
};