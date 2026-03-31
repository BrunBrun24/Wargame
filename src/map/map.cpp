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

    _players.push_back(new Player(c));
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
  std::vector<std::pair<int, int>> spawn_points = _generate_players();
  _generate_resources(spawn_points);
}

void Map::render_debug() {
  // Définition d'un dictionnaire de couleurs pour les ressources (RGB)
  static const std::map<std::string, std::string> res_colors = {
      {"Res_Iron", "\033[38;2;160;82;45m"},        // Marron Terreux
      {"Res_Coal", "\033[38;2;40;40;40m"},         // Gris Anthracite
      {"Res_Gold", "\033[38;2;255;215;0m"},        // Jaune d'Or
      {"Res_Silver", "\033[38;2;192;192;192m"},    // Argent
      {"Res_Copper", "\033[38;2;184;115;51m"},     // Cuivre
      {"Res_Horse", "\033[38;2;139;69;19m"},       // Brun Selle
      {"Res_Oil", "\033[38;2;0;0;0m"},             // Noir profond
      {"Res_Uranium", "\033[38;2;50;255;50m"},     // Vert Radioactif
      {"Res_Aluminum", "\033[38;2;233;233;233m"},  // Aluminium clair
      {"Res_Corn", "\033[38;2;255;255;100m"},      // Maïs
      {"Res_Wheat", "\033[38;2;245;222;179m"},     // Blé (Wheat)
      {"Res_Rice", "\033[38;2;255;250;240m"},      // Riz (Floral White)
      {"Res_Wine", "\033[38;2;128;0;32m"},         // Bourgogne
      {"Res_Gems", "\033[38;2;0;255;255m"},        // Cyan (Diamant/Gems)
      {"Res_Fish", "\033[38;2;64;224;208m"},       // Turquoise (Eau)
      {"Res_Whale", "\033[38;2;25;25;112m"}        // Bleu Nuit
  };

  for (int row = 0; row < _size_h; row++) {
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < _size_w; col++) {
      Case& current_case = _cases[row][col];
      std::string element = current_case.get_description();

      char symbol = '?';
      std::string color = "\033[0m";

      // 1. Ville
      if (element == "City") {
        symbol = 'H';
        color = "\033[38;2;255;215;0m";
      }
      // 2. Unité militaire
      else if (element == "Warrior" || element == "Tank" ||
               element == "Archer" || element == "Swordsman") {
        symbol = 'U';
        color = "\033[38;2;255;0;0m";
      }
      // 3. Unité civile
      else if (element == "Settler" || element == "Worker") {
        symbol = 'S';
        color = "\033[38;2;255;165;0m";
      }
      // 4. Ressources
      else if (element.find("Res_") == 0) {
        symbol = 'R';
        // On cherche si la ressource est dans notre dictionnaire de couleurs
        if (res_colors.count(element)) {
          color = res_colors.at(element);
        } else {
          color = "\033[38;2;255;105;180m";  // Rose par défaut pour les oublis
        }
      }
      // 5. Bâtiment
      else if (element == "Mine") {
        symbol = 'M';
        color = "\033[38;2;128;128;128m";
      }
      // 6. Terrain
      else {
        if (element == "Ocean") {
          symbol = 'O';
          color = "\033[38;2;0;0;139m";
        } else if (element == "CoastLake") {
          symbol = 'C';
          color = "\033[38;2;0;191;255m";
        } else if (element == "Plains") {
          symbol = 'P';
          color = "\033[38;2;34;139;34m";
        } else if (element == "Snow") {
          symbol = 's';
          color = "\033[38;2;255;250;250m";
        } else if (element == "Mountains") {
          symbol = '^';
          color = "\033[38;2;139;69;19m";
        }
      }

      std::cout << color << "[" << symbol << "]\033[0m ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
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
      case1.distance_between(&case2).distance_traveled.size());
}

void Map::add_unit_to_case(Case* target_case, UnitName name, Player* player) {
  Unit* new_unit = _create_unit(name, player, target_case);

  // On ajoute l'unité sur la case
  target_case->add_unit(new_unit);

  // On associe l'unité au joueur
  player->add_unit(new_unit);
}

void Map::add_building_to_case(Case* target_case, BuildingName building) {
  // On ajoute le batiment sur le terrain
  target_case->set_building(building);
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
    _cases[r][c].set_terrain(TerrainsType::Plains);
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
      if (neighbor->get_terrain() == TerrainsType::Ocean &&
          (std::rand() % 100 < 30)) {
        neighbor->set_terrain(TerrainsType::Plains);
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
        if (_cases[row][col].get_terrain() == TerrainsType::Ocean) {
          // On vérifie si elle a un voisin qui est de la terre ou déjà une côte
          for (Case* neighbor : _cases[row][col].get_neighbors()) {
            TerrainsType n_type = neighbor->get_terrain();

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
      c->set_terrain(TerrainsType::CoastLake);
    }
  }
}

void Map::_generate_snow() {
  // Mettre la neige à moins de 5 blocs en haut
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
        _cases[row][col].set_terrain(TerrainsType::Snow);
      }
    }
  }

  // Mettre la neige à moins de 5 blocs en bas
  for (int row = _size_h - 4; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
        _cases[row][col].set_terrain(TerrainsType::Snow);
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
      if (current.get_terrain() == TerrainsType::Plains) {
        int land_neighbors = 0;

        // On compte combien de voisins sont de la terre
        for (Case* neighbor : current.get_neighbors()) {
          TerrainsType n_type = neighbor->get_terrain();
          if (n_type == TerrainsType::Plains ||
              n_type == TerrainsType::Mountains) {
            land_neighbors++;
          }
        }

        // Si la case est bien entourée (ex: 5 ou 6 voisins terrestres)
        // On ajoute un facteur chance (5%) pour ne pas avoir trop de montagnes
        if (land_neighbors >= 5 && (std::rand() % 100 < 5)) {
          current.set_terrain(TerrainsType::Mountains);
        }
      }
    }
  }
}

std::vector<std::pair<int, int>> Map::_generate_players() {
  std::vector<std::pair<int, int>> spawn_points;
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
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

        placed = true;
      }
    }
    // Si le dernier joueur n'a pas été placé on recréer la map
    if (!placed) {
      create_map();
      return spawn_points;
    }
  }

  return spawn_points;
}

void Map::_generate_resources(std::vector<std::pair<int, int>> spawn_points) {
  std::vector<std::pair<int, int>> resource_points;
  std::random_device rd;
  std::mt19937 g(rd());

  // 1. Placer les ressources nécessaires dans un rayon de 8 cases des joueurs
  std::vector<ResourceName> essentials = {
      ResourceName::Iron, ResourceName::Copper, ResourceName::Corn,
      ResourceName::Gold};

  for (auto& start : spawn_points) {
    for (ResourceName res : essentials) {
      bool placed = false;

      // On cherche dans un carré de 16x16 autour du départ pour optimiser
      for (int r = std::max(0, start.first - 8);
           r < std::min(_size_h, start.first + 8) && !placed; ++r) {
        for (int c = std::max(0, start.second - 8);
             c < std::min(_size_w, start.second + 8) && !placed; ++c) {
          TerrainsType t = _cases[r][c].get_terrain();

          // On vérifie si l'on peut placer la ressource
          if (_cases[r][c].get_resource() == ResourceName::None &&
              _is_resource_compatible(res, t)) {
            //  On vérifie la distance entre une même ressource
            bool spacing_ok = true;
            for (const auto& p : resource_points) {
              if (_get_hex_distance(r, c, p.first, p.second) < 3) {
                spacing_ok = false;
                break;
              }
            }

            if (spacing_ok) {
              _cases[r][c].set_resource(res);
              resource_points.push_back({r, c});
              placed = true;
            }
          }
        }
      }
    }
  }

  // 2. Placer les autres ressources sur la map
  // Liste de toutes les ressources à disperser
  std::vector<ResourceName> all_resources = {
      ResourceName::Horse,   ResourceName::Coal,     ResourceName::Oil,
      ResourceName::Uranium, ResourceName::Aluminum, ResourceName::Rice,
      ResourceName::Wheat,   ResourceName::Banana,   ResourceName::Pig,
      ResourceName::Cow,     ResourceName::Sheep,    ResourceName::Wine,
      ResourceName::Silk,    ResourceName::Gems,     ResourceName::Whale};

  for (ResourceName res : all_resources) {
    // Nombre de ressource à placer
    size_t to_place = 3 * _players.size();
    int attempts = 0;

    while ((to_place > 0) && (attempts < 100)) {
      int r = std::uniform_int_distribution<>(0, _size_h - 1)(g);
      int c = std::uniform_int_distribution<>(0, _size_w - 1)(g);

      TerrainsType t = _cases[r][c].get_terrain();

      // Si la case ne possède pas de ressources ET qu'elle est compatible à la
      // ressource que l'on veut mettre
      if ((_cases[r][c].get_resource() == ResourceName::None) &&
          _is_resource_compatible(res, t)) {
        bool spacing_ok = true;

        // Il faut une distance minimum de 4 cases entre une même ressource
        for (const auto& p : resource_points) {
          if (_get_hex_distance(r, c, p.first, p.second) < 4) {
            spacing_ok = false;
            break;
          }
        }

        if (spacing_ok) {
          _cases[r][c].set_resource(res);
          resource_points.push_back({r, c});
          to_place--;
        }
      }
      attempts++;
    }
  }
}

Unit* Map::_create_unit(UnitName name, Player* player, Case* c) {
  // 1. Cas des unités Neutres
  if (name == UnitName::Settler || name == UnitName::Worker) {
    return new Neutral(name, player, c);
  }

  // 2. Cas des unités Maritimes
  if (name == UnitName::Galley || name == UnitName::Caravel ||
      name == UnitName::Ironclad || name == UnitName::Destroyer ||
      name == UnitName::Submarine || name == UnitName::AircraftCarrier) {
    return new Maritime(name, player, c);
  }

  // 3. Cas des unités Aériennes
  if (name == UnitName::Biplane || name == UnitName::Fighter ||
      name == UnitName::JetFighter || name == UnitName::Bomber ||
      name == UnitName::JetBomber) {
    return new Aerial(name, player, c);
  }

  // 4. Par défaut : Unités Terrestres
  return new Terrestrial(name, player, c);
}

bool Map::_is_resource_compatible(ResourceName resource,
                                  TerrainsType terrain) const {
  switch (resource) {
    case ResourceName::Fish:
    case ResourceName::Whale:
    case ResourceName::Crab:
    case ResourceName::Clam:
      return (terrain == TerrainsType::CoastLake);

    case ResourceName::Fur:
    case ResourceName::Deer:
      return (terrain == TerrainsType::Snow || terrain == TerrainsType::Plains);

    case ResourceName::Iron:
    case ResourceName::Copper:
    case ResourceName::Coal:
    case ResourceName::Silver:
      return (terrain == TerrainsType::Plains);

    case ResourceName::Oil:
      return (terrain == TerrainsType::Plains ||
              terrain == TerrainsType::Snow ||
              terrain == TerrainsType::CoastLake);

    default:
      return (terrain == TerrainsType::Plains);
  }
}
