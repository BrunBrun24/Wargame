#include "city.h"

#include <algorithm>
#include <iostream>

#include "case.h"
#include "player.h"
#include "units.h"

const std::map<city_level, culture_threshold> CITY_CULTURE = {
    {1, 10},
    {2, 100},
    {3, 500},
    {4, 5000},
};

int City::_id_counter = 0;

City::City(Case* city_case, Player* player, bool is_capital)
    : _id(_id_counter++),
      _city_case(city_case),
      _player(player),
      _is_capital(is_capital) {
  _player->add_city(this);
  // Lors de la création de la ville, on trace ses frontières
  this->check_for_expansion();

  // Appelez la méthode pour que l'utilisateur choisisse ce qu'il veut
  // construire dans la ville
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

void City::check_for_expansion() {
  // 1. On définit la portée maximale actuelle basée sur le niveau de culture
  // Niveau 1 = Rayon 1 (voisins directs), Niveau 2 = Rayon 2, etc.
  int max_range = _data.culture.level;

  // 2. On récupère toutes les cases à portée
  std::vector<Case*> potential_cases =
      _city_case->get_cases_in_range(max_range);

  for (Case* c : potential_cases) {
    // Si la case n'appartient à personne
    if (c->get_player() == nullptr) {
      auto it = std::find(_squares_owned.begin(), _squares_owned.end(), c);

      if (it == _squares_owned.end()) {
        // On annexe la case
        _squares_owned.push_back(c);
        c->set_player(_player);
      }
    }
  }
}

void City::update_yields() {
  Yields total;

  // 1. Rendement de base
  total.health += 2;
  total.happiness += 4;

  // Rendement du centre-ville (Minimum 2 Food, 1 Prod, 1 Commerce)
  total.food += 2;
  total.production += 1;
  total.commerce += 1;

  if (this->_is_capital) {
    total.happiness += 1;
    total.commerce += 8;
    total.culture += 2;
  }

  // 2. Met à jour le bonheur et le mécontentement de la ville
  total.happiness += calculate_happiness();
  total.unhappiness += calculate_unhappiness();

  // 3. Déterminer les coefficients de priorité
  double food_weight = 1.0;
  double prod_weight = 1.0;
  double comm_weight = 1.0;

  // Si la nourriture accumulée est faible ou négative
  if (this->_data.food.accumulated < 5) {
    food_weight = 3.0;
  }

  // 4. Trier les voisins avec ces coefficients
  std::vector<Case*> neighbors = this->_city_case->get_neighbors();

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

  // 5. Détermine le nombre de citoyens qui peuvent travailler
  int citizens_work = 0;
  // S'il y a plus de mécontentement que de bonheur
  // Alors pour chaque mécontentement en trop c'est un citoyen en moins à
  // travailler
  if (this->_data.unhappiness_yield > this->_data.happiness_yield) {
    citizens_work = this->_data.population - (this->_data.unhappiness_yield -
                                              this->_data.happiness_yield);
  } else {
    citizens_work = this->_data.population;
  }

  // 6. Affectation de la population sur les meilleures cases possédées
  int assigned_citizens = 0;
  for (Case* neighbor : neighbors) {
    if (assigned_citizens >= citizens_work) break;

    // La case doit appartenir au joueur
    if (neighbor->get_player() == this->_city_case->get_player()) {
      Yields y = neighbor->get_total_yields();
      total.food += y.food;
      total.production += y.production;
      total.commerce += y.commerce;
      total.culture += y.culture;
      total.science += y.science;
      total.happiness += y.happiness;
      total.health += y.health;
      total.sickness += y.sickness;
      assigned_citizens++;
    }
  }

  // 7. On calcule les rendements nets de la ville

  // On commence par ajouter les bonus des bâtiments donnant de la santé
  // Pour pouvoir calculer la nourriture qui dépend de la santé

  total.food -= calculate_food_consumption(total);
  this->_data.maintenance_costs = calculate_maintenance_costs(total);

  // On ajoute les bonus des bâtiments pour le reste des ressources

  // 8. On met met à jour les rendements de la ville
  this->_data.food.yield = total.food;
  this->_data.culture.data.yield = total.culture;
  this->_data.production.yield = total.production;
  this->_data.commerce.yield = total.commerce;
  this->_data.science.yield = total.science;
  this->_data.health_yield = total.health;
  this->_data.sickness_yield = total.sickness;
  this->_data.happiness_yield = total.happiness;
}

void City::update_city() {
  update_food();
  update_culture();
  update_production();
  update_commerce();
}

void City::update_food() {
  // 1. On accumule la nourriture générée par la ville
  _data.food.accumulated += _data.food.yield;

  // 2. Croissance ou Famine
  if (_data.food.accumulated >= _data.get_growth_threshold()) {
    _data.population++;
    _data.food.accumulated = 0;
  } else if (_data.food.accumulated < 0 && _data.population > 1) {
    _data.population--;
    _data.food.accumulated = 0;
  }
}

int City::calculate_food_consumption(Yields total) const {
  // 1. Consommation : 2 nourritures par citoyen
  int consumption = this->_data.population * 2;

  // 2. Malus de santé
  // Si pop > limite santé, chaque point au-dessus consomme 1 nourriture de
  // plus
  if (this->_data.population > total.health) {
    consumption += (this->_data.population - total.health);
  }

  return consumption;
}

void City::update_culture() {
  // On accumule la culture générée par la ville
  this->_data.culture.data.accumulated += this->_data.culture.data.yield;

  // On regarde s'il faut étendre les frontière de la ville*
  int next_level = this->_data.culture.level + 1;
  if (CITY_CULTURE.count(next_level)) {
    if (this->_data.culture.data.accumulated >= CITY_CULTURE.at(next_level)) {
      this->_data.culture.level = next_level;
      this->check_for_expansion();
    }
  }
}

void City::update_production() {
  // On accumule la production générée par la ville
  _data.production.accumulated += _data.production.yield;

  // On récupère la production actuelle
  ProductionOrder element = _build_queue.front();

  // On regarde si la production est finie
  if (_data.production.accumulated >= element.cost) {
    // On regarde si la production concernait une unité ou un bâtiment
    if (element.unit != UnitName::None) {
      Unit* new_unit = Unit::create_unit(element.unit, _player, _city_case);
      _city_case->add_unit(new_unit);
    } else {
      // On ajoute le nouveau bâtiment à la ville
    }

    // On retire la production
    this->_build_queue.pop();

    // On réinitialise l'accumulation de la production
    this->_data.production.accumulated = 0;

    // On demande au joueur la nouvelle production
  }
}

int City::calculate_maintenance_costs(Yields total) const {
  double costs = 0;

  // Le nombre de citoyens coûte de l'or (0.22 par citoyen)
  costs += this->_data.population;

  // La distance avec la capitale coûte de l'or (0.2 par case de distance)
  costs += this->get_city_case()->calculate_distance_between(
               this->get_player()->get_city_capital()) *
           0.2;

  return static_cast<int>(costs);
}

void City::update_commerce() {
  this->_data.commerce.accumulated += this->_data.commerce.yield;
}

int City::calculate_happiness() const {
  int happiness = 0;

  // Une unité militaire dans la ville rapporte +1 de bonheur
  for (Unit* unit : this->get_city_case()->get_units()) {
    if (unit->is_military()) {
      happiness++;
    }
  }

  return happiness;
}

int City::calculate_unhappiness() const {
  // Mettre à jour le mécontentement
  // Le mécontentement est égal au nombre de populations dans la ville
  return this->_data.population;
}
