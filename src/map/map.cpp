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

void Map::add_improvement_to_case(Case* target_case,
                                  const ImprovementName improvement) const {
  // On ajoute le batiment sur le terrain
  target_case->get_terrain().improvement = improvement;
}

void Map::create_map() {
  bool success = false;
  while (!success) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // On vide tout avant de (re)commencer
    _reset_map();

    _generate_ocean();
    _generate_plains();
    _apply_latitudinal_climate();
    _generate_hills_and_peaks();
    _generate_vegetation();
    _generate_coasts();

    // On tente de placer les joueurs
    std::vector<std::pair<int, int>> spawn_points;
    if (_try_place_players(spawn_points)) {
      _generate_resources(spawn_points);
      success = true;
    }
    // Si success est false, la boucle recommence et _reset_map() nettoie tout
  }
}

void Map::render_debug() const {
  // Dictionnaire étendu pour les ressources
  static const std::map<std::string, std::string> res_colors = {
      {"Res_Iron", "\033[38;2;160;82;45m"},        // Marron
      {"Res_Coal", "\033[38;2;40;40;40m"},         // Anthracite
      {"Res_Gold", "\033[38;2;255;215;0m"},        // Or
      {"Res_Silver", "\033[38;2;192;192;192m"},    // Argent
      {"Res_Copper", "\033[38;2;184;115;51m"},     // Cuivre
      {"Res_Horse", "\033[38;2;139;69;19m"},       // Brun
      {"Res_Oil", "\033[38;2;0;0;0m"},             // Noir
      {"Res_Uranium", "\033[38;2;50;255;50m"},     // Vert Fluo
      {"Res_Aluminum", "\033[38;2;233;233;233m"},  // Gris clair
      {"Res_Corn", "\033[38;2;255;255;102m"},      // Jaune Maïs
      {"Res_Wheat", "\033[38;2;245;222;179m"},     // Blé
      {"Res_Rice", "\033[38;2;255;250;240m"},      // Blanc Riz
      {"Res_Wine", "\033[38;2;128;0;32m"},         // Bordeaux
      {"Res_Gems", "\033[38;2;0;255;255m"},        // Cyan
      {"Res_Fish", "\033[38;2;64;224;208m"},       // Turquoise
      {"Res_Whale", "\033[38;2;25;25;112m"}        // Bleu Nuit
  };

  for (int row = 0; row < _size_h; row++) {
    if (row % 2 != 0) std::cout << "  ";

    for (int col = 0; col < _size_w; col++) {
      const Case& current_case = _cases[row][col];
      std::string element = current_case.get_description();

      char symbol = '?';
      std::string color = "\033[0m";

      // --- Priorité d'affichage (Ville > Unité > Ressource > Bâtiment >
      // Terrain) ---

      if (element == "City") {
        symbol = 'H';  // Home / Habitation
        color =
            "\033[38;2;255;255;255;48;2;255;215;0m";  // Texte blanc sur fond Or
      } else if (element == "Unit") {
        symbol = 'U';
        color = "\033[38;2;255;0;0m";  // Rouge
      } else if (element.find("Res_") == 0) {
        symbol = '$';  // Symbole monétaire pour les ressources
        if (res_colors.count(element)) color = res_colors.at(element);
      }
      // --- Terrains et Caractéristiques ---
      else {
        if (element == "Ocean") {
          symbol = 'O';
          color = "\x1b[38;5;63m";  // Bleu profond
        } else if (element == "Coast") {
          symbol = 'c';
          color = "\x1b[38;5;51m";  // Bleu clair
        } else if (element == "Ice") {
          symbol = 'I';
          color = "\033[38;2;240;248;255m";  // Blanc bleuté
        } else if (element == "Desert") {
          symbol = 'D';
          color = "\x1b[38;5;11m";  // Sable / Desert Sand
        } else if (element == "Plains") {
          symbol = 'P';
          color = "\x1b[38;5;48m";  // Vert olive clair
        } else if (element == "Grassland") {
          symbol = 'G';
          color = "\x1b[38;5;48m";  // Vert herbe
        } else if (element == "Tundra") {
          symbol = 'T';
          color = "\033[38;2;112;128;144m";  // Gris ardoise
        } else if (element == "Mountain") {
          symbol = '^';
          color = "\x1b[38;5;88m";
        } else if (element == "Hill") {
          symbol = 'n';  // Forme de colline
          color = "\x1b[38;5;171m";
        } else if (element == "Forest") {
          symbol = 'F';
          color = "\x1b[38;5;198m";  // Vert forêt sombre
        } else if (element == "Jungle") {
          symbol = 'J';
          color = "\033[38;2;127;255;0m";  // Vert fluo / Chartreuse
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

bool Map::_is_near_land(const int row, const int col) const {
  // On scanne un cercle de rayon 2 autour de la ressource
  for (int r = std::max(0, row - 2); r < std::min(_size_h, row + 3); ++r) {
    for (int c = std::max(0, col - 2); c < std::min(_size_w, col + 3); ++c) {
      // On calcule la distance hexagonale réelle pour être précis
      if (_get_hex_distance(row, col, r, c) <= 2) {
        TerrainsType t = _cases[r][c].get_terrain().type;
        // Si ce n'est ni de l'océan ni de la côte, c'est de la terre
        if (t != TerrainsType::Ocean && t != TerrainsType::Coast) {
          return true;
        }
      }
    }
  }
  return false;
}

bool Map::_is_resource_compatible(const ResourceName resource, int r,
                                  int c) const {
  // 1. On récupère la case et ses propriétés
  const Case& current_case = _cases[r][c];
  const Terrain& t = current_case.get_terrain();

  TerrainsType type = t.type;
  TerrainFeature feat = t.feature;
  TerrainElevation elev = t.elevation;

  switch (resource) {
    // --- Maritime ---
    case ResourceName::Fish:
    case ResourceName::Whale:
    case ResourceName::Clam:
    case ResourceName::Crab:
      // Vérification du terrain ET de la proximité de la terre
      if (type == TerrainsType::Coast || type == TerrainsType::Ocean) {
        return _is_near_land(r, c);
      }
      return false;

    // --- Strategic & Production ---
    case ResourceName::Iron:
    case ResourceName::Copper:
    case ResourceName::Coal:
    case ResourceName::Aluminum:
      return (type != TerrainsType::Ocean && type != TerrainsType::Ice);

    case ResourceName::Horse:
      return (type == TerrainsType::Plains ||
              type == TerrainsType::Grassland) &&
             (feat == TerrainFeature::None);

    case ResourceName::Uranium:
      return (type == TerrainsType::Tundra || type == TerrainsType::Desert ||
              type == TerrainsType::Plains);

    case ResourceName::Marble:
    case ResourceName::Stone:
      return (type == TerrainsType::Desert || type == TerrainsType::Plains ||
              type == TerrainsType::Tundra);

    // --- Food ---
    case ResourceName::Corn:
    case ResourceName::Wheat:
    case ResourceName::Rice:
      return (type == TerrainsType::Plains ||
              type == TerrainsType::Grassland) &&
             (feat == TerrainFeature::None);

    case ResourceName::Banana:
    case ResourceName::Sugar:
      return (feat == TerrainFeature::Jungle) ||
             (type == TerrainsType::Grassland);

    case ResourceName::Deer:
    case ResourceName::Fur:
      return (feat == TerrainFeature::Forest) ||
             (type == TerrainsType::Tundra) || (type == TerrainsType::Ice);

    case ResourceName::Cow:
    case ResourceName::Pig:
    case ResourceName::Sheep:
      return (type == TerrainsType::Grassland ||
              type == TerrainsType::Plains) &&
             (feat == TerrainFeature::None);

    // --- Commerce / Luxury ---
    case ResourceName::Gold:
    case ResourceName::Silver:
    case ResourceName::Gems:
      return (elev == TerrainElevation::Hill) ||
             (type == TerrainsType::Desert && resource == ResourceName::Gold);

    case ResourceName::Wine:
      return (type == TerrainsType::Plains || type == TerrainsType::Grassland);

    case ResourceName::Silk:
    case ResourceName::Dye:
    case ResourceName::Spices:
    case ResourceName::Incense:
      return (feat == TerrainFeature::Forest ||
              feat == TerrainFeature::Jungle || type == TerrainsType::Desert);

    case ResourceName::Ivory:
      return (type == TerrainsType::Plains || type == TerrainsType::Desert);

    default:
      return false;
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
      _cases[row][col] = Case({TerrainsType::Ocean});
      _cases[row][col].set_country(Country::Neutral);
      _cases[row][col].set_player(nullptr);
    }
  }
  // Vide les listes de pointeurs des joueurs
  for (auto* p : _players) {
    p->clear_units();
  }
}

void Map::_generate_ocean() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      _cases[row][col] = Case({TerrainsType::Ocean});
      _link_hex_neighbors(row, col);
    }
  }
}

void Map::_generate_plains() {
  // Placer des graines de terre
  int num_seeds = 4;
  size_t nb_player = _players.size();
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
    _cases[r][c].get_terrain().type = TerrainsType::Plains;
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
      // Chance de propagation de la terre
      if (neighbor->get_terrain().type == TerrainsType::Ocean &&
          (std::rand() % 100 < luck)) {
        neighbor->get_terrain().type = TerrainsType::Plains;
        land_cases.push_back(neighbor);
      }
    }
  }
}

void Map::_apply_latitudinal_climate() {
  for (int row = 0; row < _size_h; row++) {
    // Calcul de la position relative (0.0 = Pôle Nord, 0.5 = Équateur, 1.0 =
    // Pôle Sud)
    double relative_pos = static_cast<double>(row) / (_size_h - 1);
    double dist_from_equator = std::abs(relative_pos - 0.5) * 2.0;

    for (int col = 0; col < _size_w; col++) {
      Case& c = _cases[row][col];
      if (c.get_terrain().type == TerrainsType::Ocean) continue;

      if (dist_from_equator > 0.92) {
        c.get_terrain().type = TerrainsType::Ice;
      } else if (dist_from_equator > 0.70) {
        c.get_terrain().type =
            (std::rand() % 100 < 30
                 ? TerrainsType::Tundra
                 : (std::rand() % 100 < 65 ? TerrainsType::Plains
                                           : TerrainsType::Grassland));
      } else if (dist_from_equator < 0.15) {
        // Zone Équatoriale : Majorité de Grassland (pour la Jungle)
        c.get_terrain().type = (std::rand() % 100 < 75 ? TerrainsType::Grassland
                                                       : TerrainsType::Desert);
      } else {
        // Zones tempérées : Mélange Plains / Grassland
        c.get_terrain().type =
            (std::rand() % 100 < 50 ? TerrainsType::Plains
                                    : TerrainsType::Grassland);
      }
    }
  }
}

void Map::_generate_hills_and_peaks() {
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      Case& c = _cases[row][col];
      if (c.get_terrain().type == TerrainsType::Ocean) continue;

      int roll = std::rand() % 100;
      // 4% de pics montagneux (Infranchissable)
      if (roll < 4) {
        c.get_terrain().elevation = TerrainElevation::Mountain;
      } else if (roll < 25) {
        // 25% de collines (Bonus défense/prod)
        if (c.get_terrain().type != TerrainsType::Ice) {
          c.get_terrain().elevation = TerrainElevation::Hill;
          c.get_terrain().set_defense(0.25);
        }
      }
    }
  }
}

void Map::_generate_vegetation() {
  for (int row = 0; row < _size_h; row++) {
    double rel_pos = static_cast<double>(row) / (_size_h - 1);
    double dist_equator = std::abs(rel_pos - 0.5) * 2.0;

    for (int col = 0; col < _size_w; col++) {
      Case& c = _cases[row][col];
      if (c.get_terrain().elevation == TerrainElevation::Mountain) continue;

      TerrainsType t = c.get_terrain().type;
      int roll = std::rand() % 100;

      // Jungle (Uniquement zone chaude + Grassland)
      if (dist_equator < 0.2 && t == TerrainsType::Grassland) {
        if (roll < 60) {
          c.get_terrain().feature = TerrainFeature::Jungle;
          c.get_terrain().set_defense(0.5);
        }
      }
      // Forêt (Plains, Grassland, Tundra)
      else if (t != TerrainsType::Desert && t != TerrainsType::Ocean &&
               t != TerrainsType::Ice) {
        if (roll < 25) {
          c.get_terrain().feature = TerrainFeature::Forest;
          c.get_terrain().set_defense(0.5);
        }
      }
    }
  }
}

void Map::_generate_coasts() {
  // On met la côte à une case de la terre
  for (int dist = 0; dist < 1; dist++) {
    std::vector<Case*> ocean_to_coast;

    // On parcourt toute la carte
    for (int row = 0; row < _size_h; row++) {
      for (int col = 0; col < _size_w; col++) {
        // Si la case est de l'eau
        if (_cases[row][col].get_terrain().type == TerrainsType::Ocean) {
          // On vérifie si elle a un voisin qui est de la terre ou déjà une côte
          for (Case* neighbor : _cases[row][col].get_neighbors()) {
            TerrainsType n_type = neighbor->get_terrain().type;

            if (n_type == TerrainsType::Plains ||
                n_type == TerrainsType::Desert ||
                n_type == TerrainsType::Grassland ||
                n_type == TerrainsType::Tundra ||
                n_type == TerrainsType::Coast || n_type == TerrainsType::Ice) {
              ocean_to_coast.push_back(&_cases[row][col]);
              break;  // Pas besoin de vérifier les autres voisins
            }
          }
        }
      }
    }

    // On transforme les cases trouvées en Coast
    for (Case* c : ocean_to_coast) {
      c->get_terrain().type = TerrainsType::Coast;
    }
  }
}

bool Map::_try_place_players(std::vector<std::pair<int, int>>& spawn_points) {
  std::vector<std::pair<int, int>> coordinate_possible;

  // On récupère toutes les coordonnées la ou l'on peut placer les joueurs
  for (int row = 0; row < _size_h; row++) {
    for (int col = 0; col < _size_w; col++) {
      TerrainsType t = _cases[row][col].get_terrain().type;
      if ((t == TerrainsType::Plains) || (t == TerrainsType::Grassland) ||
          (t == TerrainsType::Tundra)) {
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
  // On va stocker les points par type de ressource pour une recherche rapide
  // Map : <Nom de la ressource, Liste des positions {r, c}>
  std::map<ResourceName, std::vector<std::pair<int, int>>> resource_locations;

  std::random_device rd;
  std::mt19937 g(rd());

  // 1. Ressources de départ
  std::vector<ResourceName> essentials = {
      ResourceName::Corn, ResourceName::Iron, ResourceName::Cow};

  for (auto& start : spawn_points) {
    for (ResourceName res : essentials) {
      bool placed = false;
      for (int radius = 1; radius <= 5 && !placed; ++radius) {
        for (int r = start.first - radius; r <= start.first + radius && !placed;
             ++r) {
          for (int c = start.second - radius;
               c <= start.second + radius && !placed; ++c) {
            if (r < 0 || r >= _size_h || c < 0 || c >= _size_w) continue;

            if (_cases[r][c].get_terrain().resource == ResourceName::None &&
                _is_resource_compatible(res, r, c)) {
              _cases[r][c].get_terrain().resource = res;
              resource_locations[res].push_back({r, c});
              placed = true;
            }
          }
        }
      }
    }
  }

  // 2. On ajoute les autres ressources
  std::vector<ResourceName> all_resources = {
      ResourceName::Aluminum, ResourceName::Coal,    ResourceName::Copper,
      ResourceName::Horse,    ResourceName::Ivory,   ResourceName::Oil,
      ResourceName::Uranium,  ResourceName::Marble,  ResourceName::Stone,
      ResourceName::Banana,   ResourceName::Clam,    ResourceName::Cow,
      ResourceName::Crab,     ResourceName::Deer,    ResourceName::Fish,
      ResourceName::Pig,      ResourceName::Rice,    ResourceName::Sheep,
      ResourceName::Wheat,    ResourceName::Sugar,   ResourceName::Whale,
      ResourceName::Dye,      ResourceName::Fur,     ResourceName::Gems,
      ResourceName::Gold,     ResourceName::Incense, ResourceName::Silk,
      ResourceName::Silver,   ResourceName::Spices,  ResourceName::Wine};

  for (ResourceName res : all_resources) {
    int to_place = static_cast<int>(_players.size() * 2);
    int attempts = 0;

    while (to_place > 0 &&
           attempts <
               250) {  // Un peu plus d'attempts pour compenser le spacing
      int r = std::uniform_int_distribution<>(0, _size_h - 1)(g);
      int c = std::uniform_int_distribution<>(0, _size_w - 1)(g);

      if (_cases[r][c].get_terrain().resource == ResourceName::None &&
          _is_resource_compatible(res, r, c)) {
        // --- LOGIQUE DE SPACING CORRIGÉE ---
        bool spacing_ok = true;

        // On ne vérifie la distance QUE pour les ressources identiques
        if (resource_locations.count(res)) {
          for (const auto& pos : resource_locations.at(res)) {
            if (_get_hex_distance(r, c, pos.first, pos.second) < 3) {
              spacing_ok = false;
              break;
            }
          }
        }

        if (spacing_ok) {
          _cases[r][c].get_terrain().resource = res;
          resource_locations[res].push_back({r, c});
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
  TerrainsType type = start_node->get_terrain().type;
  if ((type != TerrainsType::Plains) && (type != TerrainsType::Grassland) &&
      (type != TerrainsType::Tundra))
    return 0;

  stack.push_back(start_node);
  visited.insert(start_node);

  int count = 0;
  while (!stack.empty()) {
    const Case* current = stack.back();
    stack.pop_back();
    count++;

    for (const Case* neighbor : current->get_neighbors()) {
      type = neighbor->get_terrain().type;
      // On ne compte que les plaines non visitées (On ignore l'Océan/Montagne)
      if (((type == TerrainsType::Plains) ||
           (type == TerrainsType::Grassland) ||
           (type == TerrainsType::Tundra)) &&
          visited.find(neighbor) == visited.end()) {
        visited.insert(neighbor);
        stack.push_back(neighbor);
      }
    }
  }
  return count;
}
