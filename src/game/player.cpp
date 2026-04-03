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
  // On vide les unités
  for (Unit* u : _units) {
    // On détache l'unité de la case AVANT de la supprimer pour éviter
    // que le destructeur de l'unité ne tente de modifier un Player en cours de
    // destruction.
    u->set_unit_case(nullptr);
    u->set_player(nullptr);
    delete u;
  }

  for (City* c : _citys) {
    c->set_city_case(nullptr);
    c->set_player(nullptr);
    delete c;
  }
}

Country Player::choice_country(const std::vector<Country>& excluded_countries) {
  int choix = 0;
  Country selected;

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
  for (Unit* unit : _units) {
    if (unit != nullptr) {
      // 1. On prévient la case que l'unité n'existe plus
      if (unit->get_case_unit() != nullptr) {
        unit->get_case_unit()->remove_unit(unit);
      }
      // 2. On libère la mémoire
      delete unit;
    }
  }
  // 3. On vide le vecteur d'adresses désormais invalides
  _units.clear();
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

void Player::add_building(Case* c) {
  if (c) {
    _buildings.push_back(c);
  }
}

void Player::remove_building(Case* c) {
  // On cherche le bâtiment par son ID
  auto it = std::find_if(_buildings.begin(), _buildings.end(),
                         [&](Case* ca) { return ca->get_id() == c->get_id(); });

  if (it != _buildings.end()) {
    _buildings.erase(it);
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
      // 1. Calcule le rendement brut de la ville
      Yields raw_yields = c->calculate_base_yields(c->get_case_city());

      // 2. Mets à jour la ville
      c->process_consumption_food(raw_yields);
      c->process_consumption_production(raw_yields);
      c->process_consumption_commerce(raw_yields);

      // 3. Mets à jour les ressources que possède le joueur
    }
  }

  // 2. Gestion des unités
}
