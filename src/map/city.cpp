#include "city.h"

#include <algorithm>
#include <iostream>

#include "case.h"
#include "player.h"

int City::_id_counter = 0;

City::City(Case* city_case, Player* player)
    : _id(_id_counter++),
      _population(1),
      _accumulated_food(0),
      _accumulated_production(0),
      _health_limit(5),
      _happiness_limit(5),
      _player(player),
      _city_case(city_case) {
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

Yields City::calculate_base_yields(const Case* city_case) const {
  // 1. Déterminer les coefficients de priorité
  double food_weight = 1.0;
  double prod_weight = 1.0;
  double comm_weight = 1.0;

  // Si la nourriture accumulée est faible ou négative
  if (_accumulated_food < 5) {
    food_weight = 3.0;
  }

  // 2. Trier les voisins avec ces coefficients
  std::vector<Case*> neighbors = city_case->get_neighbors();

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
    if (assigned_citizens >= _population) break;

    // La case doit appartenir au joueur
    if (neighbor->get_player() == city_case->get_player()) {
      Yields y = neighbor->get_total_yields();
      total.food += y.food;
      total.production += y.production;
      total.commerce += y.commerce;
      assigned_citizens++;
    }
  }

  return total;
}

void City::process_consumption_food(Yields& final_yields) {
  // 1. Consommation : 2 nourritures par citoyen
  int consumption = _population * 2;

  // 2. Malus de santé
  // Si pop > limite santé, chaque point au-dessus consomme 1 nourriture de plus
  if (_population > _health_limit) {
    consumption += (_population - _health_limit);
  }

  _accumulated_food += (final_yields.food - consumption);

  // 3. Croissance ou Famine
  if (_accumulated_food >= get_growth_threshold()) {
    _population++;
    _accumulated_food = 0;
  } else if (_accumulated_food < 0 && _population > 1) {
    _population--;
    _accumulated_food = 0;
  }
}

void City::process_consumption_commerce(Yields& final_yields) {}

void City::process_consumption_production(Yields& final_yields) {}
