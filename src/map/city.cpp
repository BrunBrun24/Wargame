#include "city.h"

#include <algorithm>
#include <iostream>

#include "case.h"
#include "player.h"
#include "units.h"

const std::map<int, Culture> CITY_CULTURE = {
    {1, {10, 9}},    {2, {100, 21}},   {3, {500, 37}},
    {4, {5000, 61}}, {5, {50000, 81}},
};

int City::_id_counter = 0;

City::City(Case* city_case, Player* player)
    : _id(_id_counter++),
      _health_limit(5),
      _happiness_limit(5),
      _player(player),
      _city_case(city_case) {
  _player->add_city(this);
  // Appelez la méthode pour que l'utilisateur choisisse ce qu'il veut constrire
  // dans la ville
}

City::~City() {
  // 1. On se retire de la case où l'on se trouve
  if (_city_case != nullptr) {
    _city_case->set_city(nullptr);
  }

  // 2. On se retire de la liste du joueur
  if (_player != nullptr) {
    _player->remove_city(this);
  }
}

Yields City::calculate_yields() const {
  // 1. Déterminer les coefficients de priorité
  double food_weight = 1.0;
  double prod_weight = 1.0;
  double comm_weight = 1.0;

  // Si la nourriture accumulée est faible ou négative
  if (_data.food.accumulated < 5) {
    food_weight = 3.0;
  }

  // 2. Trier les voisins avec ces coefficients
  std::vector<Case*> neighbors = _city_case->get_neighbors();

  std::sort(neighbors.begin(), neighbors.end(), [&](Case* a, Case* b) {
    Yields yA = a->get_total_yields();
    Yields yB = b->get_total_yields();

    // Calcul du score pondéré
    double scoreA = (yA.food * food_weight) + (yA.production * prod_weight) +
                    (yA.commerce * comm_weight);
    double scoreB = (yB.food * food_weight) + (yB.production * prod_weight) +
                    (yB.commerce * comm_weight);

    return scoreA > scoreB;
  });

  // 3. Affectation de la population sur les meilleures cases possédées
  Yields total;
  int assigned_citizens = 0;
  for (Case* neighbor : neighbors) {
    if (assigned_citizens >= _data.population) break;

    // La case doit appartenir au joueur
    if (neighbor->get_player() == _city_case->get_player()) {
      Yields y = neighbor->get_total_yields();
      total.food += y.food;
      total.production += y.production;
      total.commerce += y.commerce;
      assigned_citizens++;
    }
  }

  // 4. Rendement du centre-ville (Minimum 2 Food, 1 Prod, 1 Commerce)
  if (is_capital()) {
    total.food += 2;
    total.production += 1;
    total.commerce += 1;
  } else {
    total.happiness += 1;
    total.commerce += 8;
    total.culture += 1;
  }

  return total;
}

void City::update_data() {
  Yields new_yields = calculate_yields();

  // Mise à jour des yields
  _data.food.yield = new_yields.food;
  _data.culture.yield = new_yields.culture;
  _data.production_yield = new_yields.production;
  _data.commerce_yield = new_yields.commerce;
  _data.science_yield = new_yields.science;

  update_food();
  update_production();
}

void City::update_food() {
  // 1. Consommation : 2 nourritures par citoyen
  int consumption = _data.population * 2;

  // 2. Malus de santé
  // Si pop > limite santé, chaque point au-dessus consomme 1 nourriture de plus
  if (_data.population > _health_limit) {
    consumption += (_data.population - _health_limit);
  }

  _data.food.accumulated += (_data.food.yield - consumption);

  // 3. Croissance ou Famine
  if (_data.food.accumulated >= _data.get_growth_threshold()) {
    _data.population++;
    _data.food.accumulated = 0;
  } else if (_data.food.accumulated < 0 && _data.population > 1) {
    _data.population--;
    _data.food.accumulated = 0;
  }
}

void City::update_production() {
  // On récupère la production actuelle
  ProductionOrder element = _build_queue.front();

  element.accumulated += _data.production_yield;

  // On regarde si la production est finie
  if (element.accumulated >= element.cost) {
    // On regarde si la production concernait une unité ou un bâtiment
    if (element.unit != UnitName::None) {
      Unit* new_unit = Unit::create_unit(element.unit, _player, _city_case);
      _city_case->add_unit(new_unit);
    } else {
      // On ajoute le nouveau bâtiment à la ville
    }

    // On demande au joueur la nouvelle production
  }
}
