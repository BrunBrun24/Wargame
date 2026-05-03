#include "city.h"

#include <algorithm>
#include <iostream>

#include "building_database.h"
#include "case.h"
#include "player.h"
#include "technology.h"
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
  // TODO
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
  int max_range = this->_data.culture.level;

  // 2. On récupère toutes les cases à portée
  std::vector<Case*> potential_cases =
      this->_city_case->get_cases_in_range(max_range);

  for (Case* c : potential_cases) {
    // Si la case n'appartient à personne
    if (c->get_player() == nullptr) {
      auto it = std::find(this->_squares_owned.begin(),
                          this->_squares_owned.end(), c);

      if (it == this->_squares_owned.end()) {
        // On annexe la case
        this->_squares_owned.push_back(c);
        c->set_player(this->_player);
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
  // On commence par ajouter les bonus des bâtiments
  for (BuildingName name : this->_data.buildings) {
    Yields y = BuildingDatabase::get_info(name).building_stats.yields;
    total.food *= 1 + y.food;
    total.production *= 1 + y.production;
    total.commerce *= 1 + y.commerce;
    total.culture *= 1 + y.culture;
    total.science *= 1 + y.science;
    total.happiness *= 1 + y.happiness;
    total.health *= 1 + y.health;
    total.sickness *= 1 + y.sickness;

    BuildingStats bs = BuildingDatabase::get_info(name).building_stats;
    total.food += bs.food;
    total.food += bs.culture;
    total.food += bs.happiness;
    total.food += bs.health;
    total.food += bs.sickness;
  }

  total.food -= calculate_food_consumption(total);
  this->_data.maintenance_costs = calculate_maintenance_costs(total);

  // 8. On met met à jour les rendements de la ville
  this->_data.food.yield = static_cast<int>(total.food);
  this->_data.culture.data.yield = static_cast<int>(total.culture);
  this->_data.production.yield = static_cast<int>(total.production);
  this->_data.commerce.yield = static_cast<int>(total.commerce);
  this->_data.science.yield = static_cast<int>(total.science);
  this->_data.health_yield = static_cast<int>(total.health);
  this->_data.sickness_yield = static_cast<int>(total.sickness);
  this->_data.happiness_yield = static_cast<int>(total.happiness);
}

void City::update_city() {
  update_food();
  update_culture();
  update_production();
  update_commerce();
}

void City::update_food() {
  // 1. On accumule la nourriture générée par la ville
  this->_data.food.accumulated += this->_data.food.yield;

  // 2. Croissance ou Famine
  if (_data.food.accumulated >= _data.get_growth_threshold()) {
    this->_data.population++;
    this->_data.food.accumulated = 0;
  } else if (_data.food.accumulated < 0 && _data.population > 1) {
    this->_data.population--;
    this->_data.food.accumulated = 0;
  }
}

int City::calculate_food_consumption(Yields total) const {
  // 1. Consommation : 2 nourritures par citoyen
  int consumption = this->_data.population * 2;

  // 2. Malus de santé
  // Si pop > limite santé, chaque point au-dessus consomme 1 nourriture de
  // plus
  if (this->_data.population > total.health) {
    consumption += (this->_data.population - static_cast<int>(total.health));
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
  ProductionOrder element = this->_build_queue.front();

  // On regarde si la production est finie
  if (_data.production.accumulated >= element.cost) {
    // On regarde si la production concernait une unité ou un bâtiment
    if (element.unit != UnitName::None) {
      Unit* new_unit = Unit::create_unit(element.unit, _player, _city_case);
      this->_city_case->add_unit(new_unit);
      element.unit = UnitName::None;
    } else {
      // On ajoute le nouveau bâtiment à la ville
      this->_data.buildings.push_back(element.building);
      element.building = BuildingName::None;
    }

    // On retire la production
    this->_build_queue.pop();

    // On réinitialise l'accumulation de la production
    this->_data.production.accumulated = 0;

    // On demande au joueur la nouvelle production
  }
}

ProductionAvailable City::production_available() const {
  ProductionAvailable pa;

  // On définit la borne max (Explorer est le dernier élément)
  const int max_unit_idx = static_cast<int>(UnitName::Explorer);

  for (int i = 0; i <= max_unit_idx; ++i) {
    UnitName unit = static_cast<UnitName>(i);

    // 1. Vérifier les technologies requises
    const auto& required_techs = UNIT_TECHNOLOGY.at(unit).technologies;
    const auto& player_techs = this->get_player()->get_technologies();

    bool has_all_techs = true;
    for (TechnologyName tech : required_techs) {
      auto it_t = std::find(player_techs.begin(), player_techs.end(), tech);

      if (it_t == player_techs.end()) {
        has_all_techs = false;  // Une technologie manque
        break;
      }
    }

    // 2. Vérifier les ressources requises
    bool has_all_resources = true;
    for (ResourceName resource_req : UNIT_TECHNOLOGY.at(unit).resource) {
      // On cherche la ressource dans l'inventaire du joueur
      if (this->get_player()->get_resources()[resource_req] <= 0) {
        has_all_resources = false;
        break;  // Il manque au moins une ressource
      }
    }

    // 3. Si toutes les conditions sont remplies, on l'ajoute
    if (has_all_techs) {
      pa.units.push_back(unit);
    }
  }

  // On définit la borne max (Walls est le dernier élément)
  const int max_building_idx = static_cast<int>(BuildingName::Walls);

  for (int i = 0; i <= max_building_idx; ++i) {
    BuildingName building = static_cast<BuildingName>(i);

    // 1. Vérifier si le bâtiment est déjà construit dans la ville
    auto it_b =
        std::find(_data.buildings.begin(), _data.buildings.end(), building);
    if (it_b != _data.buildings.end()) {
      continue;  // Déjà construit, on passe au bâtiment suivant
    }

    // 2. Vérifier les technologies requises
    const auto& required_techs =
        BuildingDatabase::get_info(building).required_tech;
    const auto& player_techs = this->get_player()->get_technologies();

    bool has_all_techs = true;
    for (TechnologyName tech : required_techs) {
      auto it_t = std::find(player_techs.begin(), player_techs.end(), tech);

      if (it_t == player_techs.end()) {
        has_all_techs = false;  // Une technologie manque
        break;
      }
    }

    // 3. Si toutes les conditions sont remplies, on l'ajoute
    if (has_all_techs) {
      pa.buildings.push_back(building);
    }
  }

  return pa;
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

void City::push_unit(UnitName unit) {
  this->_build_queue.push(
      {unit, BuildingName::None, UNIT_STATS.at(unit).production});
}

void City::push_building(BuildingName building) {
  this->_build_queue.push({UnitName::None, building,
                           BuildingDatabase::get_info(building)
                               .building_stats.required_production});
}

bool City::is_producing() {
  return !(_build_queue.front().building == BuildingName::None &&
           _build_queue.front().unit != UnitName::None);
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
