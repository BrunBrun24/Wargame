#include "map.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>
#include <unordered_set>

#include "aerial.h"
#include "maritime.h"
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
    Country c = Player::choice_country(taken_countries);

    // On ajoute le nouveau pays à la liste d'exclusion pour le prochain tour
    taken_countries.push_back(c);

    _players.push_back(new Player(c));
  }
  std::cout << std::endl;
}

Map::~Map() {
  for (Player* p : _players) {
    delete p;
  }
  _players.clear();
}

int Map::distance_between(const Coordinate c1, const Coordinate c2) const {
  Case* case1 = const_cast<Case*>(&_cases[c1.x][c1.y]);
  Case* case2 = const_cast<Case*>(&_cases[c2.x][c2.y]);

  return static_cast<int>(
      case1->distance_between(case2).distance_traveled.size());
}

void Map::add_unit_to_case(Case* target_case, const UnitName name,
                           Player* player) const {
  Unit* new_unit = _create_unit(name, player, target_case);

  // On ajoute l'unité sur la case
  target_case->add_unit(new_unit);

  // On associe l'unité au joueur
  player->add_unit(new_unit);
}

void Map::add_building_to_case(Case* target_case,
                               const BuildingName building) const {
  // On ajoute le batiment sur le terrain
  target_case->set_building(building);
}

void Map::create_map() {
  bool success = false;
  while (!success) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 1. On vide tout avant de (re)commencer
    _reset_map();

    // 2. On génère le terrain
    _generate_ocean();  // Remet tout en Ocean + recrée les liens
    _generate_plains();
    _generate_snow();
    _generate_coasts();
    _generate_mountains();

    // 3. On tente de placer les joueurs
    // On change la signature de _generate_players pour qu'il renvoie un bool
    std::vector<std::pair<int, int>> spawn_points;
    if (_try_place_players(spawn_points)) {
      _generate_resources(spawn_points);
      success = true;
    }
    // Si success est false, la boucle recommence et _reset_map() nettoie tout
  }
}

void Map::render_debug() const {
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
      const Case current_case = _cases[row][col];
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

void Map::delete_player(Player* player) {
  // On cherche le joueur par son ID
  auto it = std::find_if(_players.begin(), _players.end(), [&](const auto& p) {
    return p->get_id() == player->get_id();
  });

  if (it != _players.end()) {
    delete *it;
    _players.erase(it);
  }
}

void Map::_link_hex_neighbors(const int row, const int col) {
  Case* current = &_cases[row][col];

  // Voisins constants (Gauche et Droite) avec boucle
  // À gauche : si col == 0, le voisin est à l'extrémité droite (_size_w - 1)
  int left_col = (col > 0) ? col - 1 : _size_w - 1;
  current->add_neighbor(&_cases[row][left_col]);

  // À droite : si col == _size_w - 1, le voisin est à l'extrémité gauche (0)
  int right_col = (col < _size_w - 1) ? col + 1 : 0;
  current->add_neighbor(&_cases[row][right_col]);

  // Voisins Verticaux (Haut et Bas)
  if (row % 2 == 0) {
    // Ligne paire
    if (row > 0) {
      current->add_neighbor(&_cases[row - 1][col]);
      // Diagonale haut-gauche avec boucle
      int diag_left = (col > 0) ? col - 1 : _size_w - 1;
      current->add_neighbor(&_cases[row - 1][diag_left]);
    }
    if (row < _size_h - 1) {
      current->add_neighbor(&_cases[row + 1][col]);
      // Diagonale bas-gauche avec boucle
      int diag_left = (col > 0) ? col - 1 : _size_w - 1;
      current->add_neighbor(&_cases[row + 1][diag_left]);
    }
  } else {
    // Ligne impaire
    if (row > 0) {
      current->add_neighbor(&_cases[row - 1][col]);
      // Diagonale haut-droite avec boucle
      int diag_right = (col < _size_w - 1) ? col + 1 : 0;
      current->add_neighbor(&_cases[row - 1][diag_right]);
    }
    if (row < _size_h - 1) {
      current->add_neighbor(&_cases[row + 1][col]);
      // Diagonale bas-droite avec boucle
      int diag_right = (col < _size_w - 1) ? col + 1 : 0;
      current->add_neighbor(&_cases[row + 1][diag_right]);
    }
  }
}

int Map::_get_hex_distance(const int r1, const int c1, const int r2,
                           const int c2) const {
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

bool Map::_is_resource_compatible(const ResourceName resource,
                                  const TerrainsType terrain) const {
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

Unit* Map::_create_unit(const UnitName name, Player* player, Case* c) const {
  auto it = UNIT_TYPE.find(name);

  if (it != UNIT_TYPE.end()) {
    UnitType type = it->second;

    switch (type) {
      case UnitType::Air:
      case UnitType::Missile:
        return new Aerial(name, player, c);
        break;

      case UnitType::Naval:
        return new Maritime(name, player, c);
        break;

      default:
        return new Terrestrial(name, player, c);
        break;
    }
  }

  throw std::runtime_error("Le nom de l'unité n'existe pas dans UNIT_TYPE !");
}

void Map::_reset_map() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      // Supprime les unités proprement s'il y en a
      for (Unit* u : _cases[row][col].get_units()) {
        delete u;
      }
      // Réinitialise la case
      _cases[row][col] = Case(TerrainsType::Ocean);
      _cases[row][col].set_country(Country::Neutral);
      _cases[row][col].set_player(nullptr);
    }
  }
  // Vide les listes de pointeurs des joueurs
  for (auto* p : _players) {
    p->clear_units();
  }
}

void Map::_generate_snow() {
  // Mettre la neige sur les 3 premières lignes du haut
  for (int row = 0; row < 3; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
        _cases[row][col].set_terrain(TerrainsType::Snow);
      }
    }
  }

  // Mettre la neige sur les 3 premières lignes du bas
  for (int row = _size_h - 3; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
        _cases[row][col].set_terrain(TerrainsType::Snow);
      }
    }
  }
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
  // Placer des graines de terre
  int num_seeds = 4;
  int nb_player = _players.size();
  if (nb_player <= 2) {
    num_seeds = 6;
  } else if (nb_player <= 4) {
    num_seeds = 3;
  } else if (nb_player <= 6) {
    num_seeds = 3;
  } else if (nb_player < 8) {
    num_seeds = 4;
  }

  // chance de transformer le voisin en terre s'il est encore de l'eau
  int luck = 37;
  if (nb_player <= 2) {
    luck = 50;
  } else if (nb_player <= 4) {
    luck = 50;
  } else if (nb_player <= 6) {
    luck = 50;
  } else if (nb_player < 8) {
    luck = 50;
  }

  std::vector<Case*> land_cases;

  for (int i = 0; i < num_seeds; ++i) {
    int r = std::rand() % _size_h;
    int c = std::rand() % _size_w;
    _cases[r][c].set_terrain(TerrainsType::Plains);
    land_cases.push_back(&_cases[r][c]);
  }

  // Propagation (on étend la terre à certains voisins)
  int nb_iteration = 825;
  if (_players.size() - 1 >= 8) nb_iteration += 125;

  for (size_t iteration = 0; iteration < (nb_iteration * _players.size());
       ++iteration) {
    if (land_cases.empty()) break;

    // On choisit une case de terre existante au hasard
    int idx = std::rand() % land_cases.size();
    Case* current = land_cases[idx];

    const std::vector<Case*>& neighbors = current->get_neighbors();

    for (Case* neighbor : neighbors) {
      // 30% de
      if (neighbor->get_terrain() == TerrainsType::Ocean &&
          (std::rand() % 100 < luck)) {
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

bool Map::_try_place_players(std::vector<std::pair<int, int>>& spawn_points) {
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      if (_cases[row][col].get_terrain() == TerrainsType::Plains) {
        // On définit une taille minimale pour qu'un départ soit juste
        if (_get_continent_size(row, col) >= 80) {
          coordinate_possible.push_back({row, col});
        }
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

      // On regarde si les autres joueurs sont à plus de 20 blocs de la case
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
      std::cout << "ON RECREER LA MAP" << std::endl;
      return false;
    };
  }

  return true;
}

void Map::_generate_resources(
    const std::vector<std::pair<int, int>> spawn_points) {
  std::vector<std::pair<int, int>> resource_points;
  std::random_device rd;
  std::mt19937 g(rd());

  // 1. Placer les ressources nécessaires dans un rayon de 8 cases des joueurs
  std::vector<ResourceName> essentials = {
      ResourceName::Iron, ResourceName::Copper, ResourceName::Corn};

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

int Map::_get_continent_size(const int row, const int col) const {
  // 1. Utilisation d'un set de pointeurs pour le "visited"
  std::unordered_set<const Case*> visited;
  std::vector<const Case*> stack;

  const Case* start_node = &_cases[row][col];

  // On ne commence que si c'est de la terre
  if (start_node->get_terrain() != TerrainsType::Plains) return 0;

  stack.push_back(start_node);
  visited.insert(start_node);

  int count = 0;
  while (!stack.empty()) {
    const Case* current = stack.back();
    stack.pop_back();
    count++;

    for (const Case* neighbor : current->get_neighbors()) {
      // On ne compte que les plaines non visitées (On ignore l'Océan/Montagne)
      if (neighbor->get_terrain() == TerrainsType::Plains &&
          visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        stack.push_back(neighbor);
      }
    }
  }
  return count;
}
