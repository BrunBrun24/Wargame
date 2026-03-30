#include "map.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

#include "aerial.h"
#include "maritime.h"
#include "neutral.h"
#include "player_utils.h"
#include "terrestrial.h"

Map::Map(int nb_player) {
  if (nb_player <= 2) {
    _size_h = 44;
    _size_w = 26;
  } else if (nb_player <= 4) {
    _size_h = 60;
    _size_w = 38;
  } else if (nb_player <= 6) {
    _size_h = 74;
    _size_w = 46;
  } else if (nb_player <= 8) {
    _size_h = 84;
    _size_w = 54;
  } else {
    _size_h = 96;
    _size_w = 60;
  }

  _cases.resize(_size_h, std::vector<Case>(_size_w));

  std::vector<Country> taken_countries;

  // On créer le nombre de joueur demandé
  for (int i = 0; i < nb_player; i++) {
    std::cout << "\nJoueur " << i + 1;

    // On passe la liste des pays déjà pris
    Country c = PlayerParser::choice_country(taken_countries);

    // On ajoute le nouveau pays à la liste d'exclusion pour le prochain tour
    taken_countries.push_back(c);

    _players.push_back(std::make_unique<Player>(c));
  }
  std::cout << std::endl;
}

void Map::create_map() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  _generate_ocean();
  _generate_plains();
  _generate_snow();
  _generate_coasts();
  _generate_mountains();
  _generate_players();
  _generate_buildings();
}

void Map::render_debug() {
  for (int row = 0; row < _size_h; row++) {
    // Décalage pour le rendu hexagonal
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < _size_w; col++) {
      Case& current_case = _cases[row][col];
      std::string element = current_case.get_description();
      char symbol = ' ';
      std::string color = "\033[0m";  // Reset par défaut

      // 1. Détermination du symbole et de la couleur selon l'élément
      // prioritaire
      if (element == "City") {
        symbol = 'H';                    // H pour Hotel de ville / City
        color = "\033[38;2;255;215;0m";  // Or
      } else if (element == "Warrior" || element == "Tank" ||
                 element == "Archer") {
        // Tu peux ajouter d'autres unités ici ou faire un test générique
        symbol = 'U';                  // U pour Unité
        color = "\033[38;2;255;0;0m";  // Rouge vif
      } else if (element == "Settler" || element == "Worker") {
        symbol = 'S';                    // S pour Support / Settler
        color = "\033[38;2;255;165;0m";  // Orange
      } else if (element == "GoldMine" || element == "IronMine" ||
                 element == "Oil") {
        symbol = 'B';                      // B pour Bâtiment
        color = "\033[38;2;200;200;200m";  // Gris clair
      } else if (element == "AAAA") {
        symbol = 'Z';                  // B pour Bâtiment
        color = "\033[38;2;255;0;0m";  // Gris clair
        // 2. Si c'est un terrain (cas par défaut de get_priority_element)
      }
      // 2. Si c'est un terrain (cas par défaut de get_priority_element)
      else {
        if (element == "Ocean") {
          symbol = 'O';
          color = "\033[38;2;0;0;150m";
        } else if (element == "CoastLake") {
          symbol = 'C';
          color = "\033[38;2;0;180;220m";
        } else if (element == "Plains") {
          symbol = 'P';
          color = "\033[38;2;50;160;50m";
        } else if (element == "Snow") {
          symbol = 'S';
          color = "\033[38;2;255;255;255m";
        } else if (element == "Mountains") {
          symbol = 'M';
          color = "\033[38;2;110;90;70m";
        }
      }

      // Affichage de la case avec sa couleur
      std::cout << color << "[" << symbol << "]\033[0m ";
    }
    std::cout << std::endl;
  }
}

void Map::delete_player(Player& player) {
  // On cherche le joueur par son ID
  auto it = std::find_if(_players.begin(), _players.end(), [&](const auto& p) {
    return p->get_id() == player.get_id();
  });

  if (it != _players.end()) {
    _players.erase(it);
  }
}

int Map::distance_between(Coordinate c1, Coordinate c2) {
  Case& case1 = _cases[c1.x][c1.y];
  Case& case2 = _cases[c2.x][c2.y];

  return static_cast<int>(
      case1.distance_between(case2).distance_traveled.size());
}

void Map::add_unit_to_case(Case* target_case, UnitName name,
                           std::unique_ptr<Player>& player) {
  std::unique_ptr<Unit> new_unit =
      _create_unit(name, player->get_country(), target_case);

  // On ajoute l'unité sur la case
  target_case->add_unit(new_unit.get());

  // On associe l'unité au joueur
  player->add_unit(std::move(new_unit));
}

void Map::add_building_to_case(Case* target_case, BuildingType type) {
  // On ajoute le batiment sur le terrain
  target_case->get_terrain().set_building(type);
}

void Map::_link_hex_neighbors(int row, int col) {
  Case* current = &_cases[row][col];

  // Voisins constants (Gauche et Droite)
  if (col > 0) current->add_neighbor(&_cases[row][col - 1]);
  if (col < _size_w - 1) current->add_neighbor(&_cases[row][col + 1]);

  if (row % 2 == 0) {
    // Ligne PAIRE
    // En haut (Gauche / Droite)
    if (row > 0) {
      current->add_neighbor(&_cases[row - 1][col]);
      if (col > 0) current->add_neighbor(&_cases[row - 1][col - 1]);
    }
    // En bas (Gauche / Droite)
    if (row < _size_h - 1) {
      current->add_neighbor(&_cases[row + 1][col]);
      if (col > 0) current->add_neighbor(&_cases[row + 1][col - 1]);
    }
  } else {
    // Ligne IMPAIRE
    // En haut (Gauche / Droite)
    if (row > 0) {
      current->add_neighbor(&_cases[row - 1][col]);
      if (col < _size_w - 1) current->add_neighbor(&_cases[row - 1][col + 1]);
    }
    // En bas (Gauche / Droite)
    if (row < _size_h - 1) {
      current->add_neighbor(&_cases[row + 1][col]);
      if (col < _size_w - 1) current->add_neighbor(&_cases[row + 1][col + 1]);
    }
  }
}

int Map::_get_hex_distance(int r1, int c1, int r2, int c2) const {
  // Conversion Row/Col (offset) vers Cube (x, y, z)
  int x1 = c1 - (r1 + (r1 & 1)) / 2;
  int z1 = r1;
  int y1 = -x1 - z1;

  int x2 = c2 - (r2 + (r2 & 1)) / 2;
  int z2 = r2;
  int y2 = -x2 - z2;

  // La distance hexagonale est le maximum des différences absolues des axes
  return std::max({std::abs(x1 - x2), std::abs(y1 - y2), std::abs(z1 - z2)});
}

void Map::_generate_ocean() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      _cases[row][col] = Case(TerrainsType::Ocean);
      _link_hex_neighbors(row, col);
    }
  }
}

void Map::_generate_plains() {
  // Placer des graines de terre (ex: 5 graines)
  int num_seeds = 10;
  std::vector<Case*> land_cases;

  for (int i = 0; i < num_seeds; ++i) {
    int r = std::rand() % _size_h;
    int c = std::rand() % _size_w;
    _cases[r][c].set_terrain_type(TerrainsType::Plains);
    land_cases.push_back(&_cases[r][c]);
  }

  // Propagation (on étend la terre à certains voisins)
  for (int iteration = 0; iteration < (750 * _players.size()); ++iteration) {
    if (land_cases.empty()) break;

    // On choisit une case de terre existante au hasard
    int idx = std::rand() % land_cases.size();
    Case* current = land_cases[idx];

    const std::vector<Case*>& neighbors = current->get_neighbors();

    for (Case* neighbor : neighbors) {
      // 30% de chance de transformer le voisin en terre s'il est encore de
      // l'eau
      if (neighbor->get_terrain().get_terrain_type() == TerrainsType::Ocean &&
          (std::rand() % 100 < 30)) {
        neighbor->set_terrain_type(TerrainsType::Plains);
        land_cases.push_back(neighbor);
      }
    }
  }
}

void Map::_generate_coasts() {
  // On répète 3 fois pour la distance de 3 cases
  for (int dist = 0; dist < 2; dist++) {
    std::vector<Case*> ocean_to_coast;

    // On parcourt toute la carte
    for (int row = 0; row < _size_h; row++) {
      for (int col = 0; col < _size_w; col++) {
        // Si la case est de l'eau
        if (_cases[row][col].get_terrain().get_terrain_type() ==
            TerrainsType::Ocean) {
          // On vérifie si elle a un voisin qui est de la terre ou déjà une côte
          for (Case* neighbor : _cases[row][col].get_neighbors()) {
            TerrainsType n_type = neighbor->get_terrain().get_terrain_type();

            if (n_type == TerrainsType::Plains ||
                n_type == TerrainsType::CoastLake ||
                n_type == TerrainsType::Snow) {
              ocean_to_coast.push_back(&_cases[row][col]);
              break;  // Pas besoin de vérifier les autres voisins
            }
          }
        }
      }
    }

    // On transforme les cases trouvées en CoastLake
    for (Case* c : ocean_to_coast) {
      c->set_terrain_type(TerrainsType::CoastLake);
    }
  }
}

void Map::_generate_snow() {
  // Mettre la neige à moins de 5 blocs en haut
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        _cases[row][col].set_terrain_type(TerrainsType::Snow);
      }
    }
  }

  // Mettre la neige à moins de 5 blocs en bas
  for (int row = _size_h - 4; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        _cases[row][col].set_terrain_type(TerrainsType::Snow);
      }
    }
  }
}

void Map::_generate_mountains() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      Case& current = _cases[row][col];

      // On ne place des montagnes que sur les Plaines (pas sur l'Océan ou la
      // Neige)
      if (current.get_terrain().get_terrain_type() == TerrainsType::Plains) {
        int land_neighbors = 0;

        // On compte combien de voisins sont de la terre
        for (Case* neighbor : current.get_neighbors()) {
          TerrainsType n_type = neighbor->get_terrain().get_terrain_type();
          if (n_type == TerrainsType::Plains ||
              n_type == TerrainsType::Mountains) {
            land_neighbors++;
          }
        }

        // Si la case est bien entourée (ex: 5 ou 6 voisins terrestres)
        // On ajoute un facteur chance (15%) pour ne pas avoir trop de montagnes
        if (land_neighbors >= 5 && (std::rand() % 100 < 10)) {
          current.set_terrain_type(TerrainsType::Mountains);
        }
      }
    }
  }
}

void Map::_generate_players() {
  std::vector<std::pair<int, int>> spawn_points;
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        coordinate_possible.push_back({row, col});
      }
    }
  }

  // On mélange le vecteur des coordonnées possibles pour que le placement soit
  // aléatoire
  // On créer générateur de nombres aléatoires (moteur Mersenne Twister)
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(coordinate_possible.begin(), coordinate_possible.end(), g);

  for (auto& player : _players) {
    bool placed = false;

    while (!placed && !coordinate_possible.empty()) {
      // On récupère la dernière coordonnée
      std::pair<int, int> coord = coordinate_possible.back();
      int r = coord.first;
      int c = coord.second;

      coordinate_possible.pop_back();

      bool too_close = false;
      for (const auto& other : spawn_points) {
        int dist = _get_hex_distance(r, c, other.first, other.second);

        if (dist < 20) {
          too_close = true;
          break;
        }
      }

      if (!too_close) {
        spawn_points.push_back({r, c});

        // On ajoute un colon et un warrior sur la case en question
        add_unit_to_case(&_cases[r][c], UnitName::Warrior, player);
        add_unit_to_case(&_cases[r][c], UnitName::Settler, player);

        std::cout << "[Spawn] Joueur placé en (" << r << ", " << c << ")"
                  << std::endl;
        placed = true;
      }
    }
    // Si le dernier joueur n'a pas été placé on recréer la map
    if (!placed) {
      std::cout
          << "[Echec] Plus de cases disponibles. Relance de la génération..."
          << std::endl
          << std::endl;
      create_map();
      return;
    }
  }

  std::cout << std::endl;
}

void Map::_generate_buildings() {
  std::vector<std::pair<int, int>> buildings_points;
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain().get_terrain_type() ==
          TerrainsType::Plains) {
        coordinate_possible.push_back({row, col});
      }
    }
  }

  // On mélange le vecteur des coordonnées possibles pour que le placement soit
  // aléatoire
  // On créer générateur de nombres aléatoires (moteur Mersenne Twister)
  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(coordinate_possible.begin(), coordinate_possible.end(), g);

  std::map<int, BuildingType> dict_building = {{1, BuildingType::GoldMine},
                                               {2, BuildingType::IronMine},
                                               {3, BuildingType::Oil}};

  std::vector<std::pair<int, int>> coordinate_possible_copy =
      coordinate_possible;

  for (auto const& [id, type] : dict_building) {
    int building_to_be_placed = 3 * _players.size();
    coordinate_possible = coordinate_possible_copy;

    while (building_to_be_placed > 0 && !coordinate_possible.empty()) {
      // On récupère la dernière coordonnée
      std::pair<int, int> coord = coordinate_possible.back();
      int r = coord.first;
      int c = coord.second;

      coordinate_possible.pop_back();

      if (_cases[r][c].get_terrain().get_building() !=
          BuildingType::NoBuilding) {
        continue;
      }

      bool too_close = false;
      for (const auto& other : buildings_points) {
        int dist = _get_hex_distance(r, c, other.first, other.second);

        if (dist < 5) {
          too_close = true;
          break;
        }
      }

      if (!too_close) {
        buildings_points.push_back({r, c});

        // On ajoute le batiment
        add_building_to_case(&_cases[r][c], type);

        std::cout << "Bâtiment placé en (" << r << ", " << c << ")"
                  << std::endl;

        building_to_be_placed -= 1;
      }
    }
  }

  std::cout << std::endl;
}

std::unique_ptr<Unit> Map::_create_unit(UnitName name, Country country,
                                        Case* c) {
  // 1. Cas des unités Neutres
  if (name == UnitName::Settler || name == UnitName::Worker) {
    return std::make_unique<Neutral>(name, country, c);
  }

  // 2. Cas des unités Maritimes
  if (name == UnitName::Galley || name == UnitName::Caravel ||
      name == UnitName::Ironclad || name == UnitName::Destroyer ||
      name == UnitName::Submarine || name == UnitName::AircraftCarrier) {
    return std::make_unique<Maritime>(name, country, c);
  }

  // 3. Cas des unités Aériennes
  if (name == UnitName::Biplane || name == UnitName::Fighter ||
      name == UnitName::JetFighter || name == UnitName::Bomber ||
      name == UnitName::JetBomber) {
    return std::make_unique<Aerial>(name, country, c);
  }

  // 4. Par défaut : Unités Terrestres
  return std::make_unique<Terrestrial>(name, country, c);
}
