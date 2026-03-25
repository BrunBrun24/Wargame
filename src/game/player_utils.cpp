#include "player_utils.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

namespace PlayerParser {

Country choice_country(const std::vector<Country>& excluded_countries) {
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

}  // namespace PlayerParser
