#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "units.h"

namespace UnitParser {

UnitName string_to_unit_name(const std::string& name) {
  static const std::map<std::string, UnitName> lookup = {
      {"Warrior", UnitName::Warrior},
      {"Swordsman", UnitName::Swordsman},
      {"Musketman", UnitName::Musketman},
      {"Infantry", UnitName::Infantry},
      {"MechanizedInfantry", UnitName::MechanizedInfantry},
      {"Archer", UnitName::Archer},
      {"Crossbowman", UnitName::Crossbowman},
      {"FieldCannon", UnitName::FieldCannon},
      {"MachineGun", UnitName::MachineGun},
      {"Horseman", UnitName::Horseman},
      {"Knight", UnitName::Knight},
      {"Cuirassier", UnitName::Cuirassier},
      {"Tank", UnitName::Tank},
      {"ModernArmor", UnitName::ModernArmor},
      {"Galley", UnitName::Galley},
      {"Caravel", UnitName::Caravel},
      {"Ironclad", UnitName::Ironclad},
      {"Destroyer", UnitName::Destroyer},
      {"Submarine", UnitName::Submarine},
      {"AircraftCarrier", UnitName::AircraftCarrier},
      {"Biplane", UnitName::Biplane},
      {"Fighter", UnitName::Fighter},
      {"JetFighter", UnitName::JetFighter},
      {"Bomber", UnitName::Bomber},
      {"JetBomber", UnitName::JetBomber}};

  auto it = lookup.find(name);
  if (it != lookup.end()) {
    return it->second;
  }
  std::string error_msg =
      "ERREUR FATALE : L'unite '" + name +
      "' est introuvable dans le dictionnaire de correspondance. " +
      "Verifiez l'orthographe dans le fichier CSV.";

  throw std::runtime_error(error_msg);
}

StrengthWeaknessMatrix load_strength_weakness_matrix() {
  StrengthWeaknessMatrix matrix;
  std::ifstream file("../../src/files/csv/matrix_units_strength_weakness.csv");

  if (!file.is_open()) {
    std::cerr << "Impossible d'ouvrir le fichier CSV." << std::endl;
    return matrix;
  }

  std::string line;
  int line_count = 1;        // Pour le débug
  std::getline(file, line);  // Ignore l'en-tête

  // L'ordre des colonnes doit correspondre au vecteur statique
  // _unit_type_order On suppose que l'ordre du CSV est le même que UnitName
  std::vector<UnitName> order = {UnitName::Warrior,
                                 UnitName::Swordsman,
                                 UnitName::Musketman,
                                 UnitName::Infantry,
                                 UnitName::MechanizedInfantry,
                                 UnitName::Archer,
                                 UnitName::Crossbowman,
                                 UnitName::FieldCannon,
                                 UnitName::MachineGun,
                                 UnitName::Horseman,
                                 UnitName::Knight,
                                 UnitName::Cuirassier,
                                 UnitName::Tank,
                                 UnitName::ModernArmor,
                                 UnitName::Galley,
                                 UnitName::Caravel,
                                 UnitName::Ironclad,
                                 UnitName::Destroyer,
                                 UnitName::Submarine,
                                 UnitName::AircraftCarrier,
                                 UnitName::Biplane,
                                 UnitName::Fighter,
                                 UnitName::JetFighter,
                                 UnitName::Bomber,
                                 UnitName::JetBomber};

  try {
    while (std::getline(file, line)) {
      std::stringstream ss(line);
      std::string attacker_str;
      std::getline(ss, attacker_str,
                   ',');  // Récupérer le nom de l'attaquant (1ère colonne)

      // Convertir le string en UnitName
      UnitName attacker = string_to_unit_name(attacker_str);

      std::string value_str;
      int col_index = 0;
      while (std::getline(ss, value_str, ',')) {
        if (!value_str.empty()) {
          double val = std::stod(value_str);
          UnitName target = order[col_index];
          matrix[attacker][target] = val;
          col_index++;
        }
      }
      line_count++;
    }
    return matrix;
  } catch (const std::exception& e) {
    std::cerr << "\n========================================" << std::endl;
    std::cerr << "ERREUR DE CHARGEMENT (Ligne " << line_count << ")"
              << std::endl;
    std::cerr << e.what() << std::endl;
    std::cerr << "========================================\n" << std::endl;
    exit(1);
  }
}

}  // namespace UnitParser