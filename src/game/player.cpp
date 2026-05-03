#include "player.h"

#include <iostream>
#include <limits>
#include <random>

#include "algorithm"
#include "case.h"
#include "city.h"
#include "units.h"

int Player::_id_counter = 0;

Player::Player(Country country) : _id(_id_counter++), _country(country) {}

Player::~Player() {
  // On utilise une boucle robuste pour les tests
  while (!_units.empty()) {
    Unit* u = _units.back();
    delete u;  // Note: Le destructeur de Unit va appeler remove_unit
  }

  while (!_cities.empty()) {
    City* c = _cities.back();
    delete c;
  }
}

Country Player::choice_country(const std::vector<Country>& excluded_countries) {
  int choix = 0;
  Country selected = Country::Neutral;

  // Helper pour savoir si le pays est déjà pris
  auto is_taken = [&](int val) {
    Country c = static_cast<Country>(val);
    return std::find(excluded_countries.begin(), excluded_countries.end(), c) !=
           excluded_countries.end();
  };

  const std::vector<std::string> countryNames = {
      "None",   "France", "Germany",      "United Kingdom",
      "Russia", "Egypt",  "Switzerland",  "Japan",
      "Spain",  "Italy",  "United States"};

  do {
    std::cout << "\n--- Choisissez votre pays ---\n";
    // On commence à 1 pour ignorer "None" (index 0)
    for (size_t i = 1; i < countryNames.size(); ++i) {
      // On n'affiche le pays que s'il n'est pas déjà pris
      if (!is_taken(static_cast<int>(i))) {
        std::cout << i << ". " << countryNames[i] << "\n";
      }
    }

    std::cout << "Votre choix : ";

    if (!(std::cin >> choix)) {
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      continue;
    }

    selected = static_cast<Country>(choix);

    // On vérifie si le choix est valide ET pas déjà pris
    if (is_taken(choix)) {
      std::cout << "Erreur : Ce pays est deja pris !\n";
      choix = 0;  // Force la répétition de la boucle
    }

  } while (choix < 1 || choix > 10);

  return selected;
}

int Player::depense_gold() const {
  int gold_yield = 0;

  int units_enemy_territory = 0;
  for (Unit* unit : _units) {
    // On compte le nombre d'unités qui sont sur le territoire ennemie
    if (unit->get_case_unit()->get_player() != this) {
      units_enemy_territory++;
    }
  }

  // Si le joueur à plus de 24 unités, chaque unité en plus coûtera 1 or
  if (this->_units.size() > 24) {
    gold_yield += static_cast<int>(_units.size() - 24);
  }

  // Si le joueur a des unités dans un territoire ennemie elle coûtent de
  // l'argent
  // 4 unités seront gratuites mais ensuite chaques unités coûtera 0.5 or
  if (units_enemy_territory > 4) {
    gold_yield += static_cast<int>((units_enemy_territory - 4) * 0.5);
  }

  return gold_yield;
}

void Player::initialise_turn() {
  int gold_yield = 0;

  // 1. Mets à jour la production des villes et l'income du joueur
  for (City* city : this->_cities) {
    city->update_yields();
    city->update_city();

    CityData data = city->get_data();
    gold_yield += data.commerce.yield;
  }

  // 2. Réinitialise les PMs de chaque unité
  for (Unit* unit : this->_units) {
    unit->set_PM(UNIT_STATS.at(unit->get_name()).PM);
  }

  // 3. Mets à jour l'income en or pour l'affichage
  this->_income.gold = gold_yield - this->depense_gold();
}

void Player::clear_units() {
  while (!_units.empty()) {
    Unit* u = _units.back();
    delete u;  // Note: Le destructeur de Unit va appeler remove_unit
  }
}

void Player::add_unit(Unit* unit) {
  if (unit) {
    _units.push_back(unit);
  }
}

void Player::remove_unit(Unit* unit) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_units.begin(), _units.end(), [&](Unit* u) {
    return u->get_id() == unit->get_id();
  });

  if (it != _units.end()) {
    _units.erase(it);
  }
}

void Player::add_improvement(Case* c) {
  if (c) {
    _improvements.push_back(c);
  }
}

void Player::remove_improvement(Case* c) {
  // On cherche le bâtiment par son ID
  auto it = std::find_if(_improvements.begin(), _improvements.end(),
                         [&](Case* ca) { return ca->get_id() == c->get_id(); });

  if (it != _improvements.end()) {
    _improvements.erase(it);
  }
}

void Player::clear_citys() {
  for (City* city : _cities) {
    if (city != nullptr) {
      delete city;
    }
  }
  _cities.clear();
}

void Player::add_city(City* city) {
  if (city) {
    _cities.push_back(city);
  }
}

void Player::remove_city(City* city) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_cities.begin(), _cities.end(), [&](City* c) {
    return c->get_id() == city->get_id();
  });

  if (it != _cities.end()) {
    _cities.erase(it);
  }
}

Case* Player::get_city_capital() {
  for (City* c : this->_cities) {
    if (c->is_capital()) {
      return c->get_city_case();
    }
  }

  // Si le joueur n'a pas de capital
  return nullptr;
}

void Player::start_turn() {
  // 1. Vérification des productions des villes
  this->_check_city_productions();

  // 2. Vérification de la recherche technologique
  this->_check_active_research();
}

// void Player::process_turn_actions() {
//   // Tant qu'il reste au moins une unité avec des points de mouvement ou des
//   // actions
//   while (this->_has_active_units()) {
//     // TODO
//     // Ici, on attend l'entrée utilisateur pour sélectionner une unité et lui
//     // donner un ordre
//     this->__handle_unit_selection_and_orders();
//   }

//   std::cout << "Toutes les unités sont inactives. Fin du tour." << std::endl;
// }

bool Player::has_active_units() const {
  for (const Unit* unit : this->_units) {
    if (unit->is_active()) {
      return true;
    }
  }
  return false;
}

// void Player::_check_city_productions() {
//   for (City* city : this->_cities) {
//     // Si la ville n'a plus rien à produire
//     if (!city->is_producing()) {
//       std::cout << "La ville " << city->get_id()
//                 << " n'a plus de production en cours." << std::endl;

//       // TODO
//       // Demander à l'utilisateur quelle unité/bâtiment il veut construire
//       ProductionItem* new_item = this->__prompt_user_for_production(city);
//       // On l'ajoute à la file d'attente
//       city->push_unit(new_item);
//       city->push_building(new_item);
//     }
//   }
// }

// void Player::_check_active_research() {
//   if (this->_current_research == TechnologyName::None) {
//     std::cout << "Aucune recherche technologique en cours !" << std::endl;

//     // TODO
//     // On demande à l'utilisateur la technologie à rechercher
//     TechnologyName tech = this->prompt_user_for_technology();
//     this->_current_research = tech;
//   }
// }
