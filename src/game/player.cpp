#include "player.h"

#include <iostream>
#include <limits>

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

  while (!_citys.empty()) {
    City* c = _citys.back();
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
  for (City* city : _citys) {
    if (city != nullptr) {
      delete city;
    }
  }
  _citys.clear();
}

void Player::add_city(City* city) {
  if (city) {
    _citys.push_back(city);
  }
}

void Player::remove_city(City* city) {
  // On cherche l'unité par son ID
  auto it = std::find_if(_citys.begin(), _citys.end(), [&](City* c) {
    return c->get_id() == city->get_id();
  });

  if (it != _citys.end()) {
    _citys.erase(it);
  }
}

void Player::process_turn() {
  // 1. Gestion des ressources
  for (City* c : _citys) {
    if (c) {
      c->update_city();
    }
  }

  // 2. Gestion des unités
}

Case* Player::get_city_capital() {
  for (City* c : this->_citys) {
    if (c->is_capital()) {
      return c->get_city_case();
    }
  }

  // Si le joueur n'a pas de capital
  return nullptr;
}