#include "units.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
/*

pour faiblesse : 
On parcours le fichier csv ligne par ligne, prendre le premier element, il doit être == au nom. 
ensuite on prend la ligne, et on ajoute dans Avantage avec Avantage.push_back(element de la colonne)


*/
std::string to_string_UnitName(UnitName name){
  switch (name) {
        case UnitName::Warrior: return "Warrior";
        case UnitName::Swordsman: return "Swordsman";
        case UnitName::Musketman: return "Musketman";
        case UnitName::Infantry: return "Infantry";
        case UnitName::MechanizedInfantry: return "MechanizedInfantry";
        case UnitName::Archer: return "Archer";
        case UnitName::Crossbowman: return "Crossbowman";
        case UnitName::FieldCannon: return "FieldCannon";
        case UnitName::MachineGun: return "MachineGun";
        case UnitName::Horseman: return "Horseman";
        case UnitName::Knight: return "Knight";
        case UnitName::Cuirassier: return "Cuirassier";
        case UnitName::Tank: return "Tank";
        case UnitName::ModernArmor: return "ModernArmor";
        case UnitName::Galley: return "Galley";
        case UnitName::Caravel: return "Caravel";
        case UnitName::Ironclad: return "Ironclad";
        case UnitName::Destroyer: return "Destroyer";
        case UnitName::Submarine: return "Submarine";
        case UnitName::AircraftCarrier: return "AircraftCarrier";
        case UnitName::Biplane: return "Biplane";
        case UnitName::Fighter: return "Fighter";
        case UnitName::JetFighter: return "JetFighter";
        case UnitName::Bomber: return "Bomber";
        case UnitName::JetBomber: return "JetBomber";

        default: return "Unknown";
    }
}

std::vector<int> Avantages_troupes(UnitName name){
    std::ifstream file("./src/files/csv/matrix_unit_avantages.csv");
    if (!file.is_open()) {
      std::cerr << "Impossible d'ouvrir le fichier !\n";
      //si vraiment sa arrive la ya un gros problème lol
    }
    else{
        std::string line;
        // Lire l'en-tête
        std::getline(file, line);
        while (std::getline(file, line)) {
            // Trouver la première virgule pour taille du mot, 
            size_t pos = line.find(',');
            std::string first_cell = line.substr(0, pos);

            if (first_cell == to_string_UnitName(name)) {
                std::vector<int> values;
                size_t start = pos + 1;
                size_t end;

                // Découper toutes les cellules restantes
                while ((end = line.find(',', start)) != std::string::npos) {
                    values.push_back(std::stoi(line.substr(start, end - start)));
                    start = end + 1;
                }
                // Dernière cellule
                if (start < line.size()) { 
                    values.push_back(std::stoi(line.substr(start)));
                }

                return values;
            }
        }

     // si nom non trouvé
    }
    return {};
}

int Unit::_id_counter = 0;
//sens_troupes : 
std::vector<UnitName> sens_troupes = {UnitName::Warrior, UnitName::Swordsman, UnitName::Musketman, UnitName::Infantry, UnitName::MechanizedInfantry, UnitName::Archer, UnitName::Crossbowman, UnitName::FieldCannon, UnitName::MachineGun, UnitName::Horseman, UnitName::Knight, UnitName::Cuirassier, UnitName::Tank, UnitName::ModernArmor, UnitName::Galley, UnitName::Caravel, UnitName::Ironclad, UnitName::Destroyer, UnitName::Submarine, UnitName::AircraftCarrier, UnitName::Biplane, UnitName::Fighter, UnitName::JetFighter, UnitName::Bomber, UnitName::JetBomber};
Unit::Unit(UnitName name, Country country, int pv, int speed, int power,
           int defense, int range, std::vector<TerrainsType> allow_terrain)
    : _name(name),
      _pv(pv),
      _country(country),
      _speed(speed),
      _power(power),
      _defense(defense),
      _range(range),
      allow_terrain(allow_terrain),
      _avantage(Avantages_troupes(name)) {
  this->_id = _id_counter++;
}

Unit::~Unit() {}

void Unit::attack(Unit* ennemy) {
  if (ennemy->get_country() != _country) {
    ennemy->set_pv(_power);
    this->set_pv(ennemy->get_power() - ennemy->get_def());

    if (ennemy->get_pv() <= 0) {
      delete ennemy;
    }

    if (_pv <= 0) {
      delete this;
    }
  }
};

bool Unit::find_terrain(const TerrainsType& target_terrain) const {
  auto it =
      std::find(allow_terrain.begin(), allow_terrain.end(), target_terrain);

  return it != allow_terrain.end();
}