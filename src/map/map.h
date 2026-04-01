#pragma once

#include <memory>
#include <vector>

#include "case.h"
#include "player.h"
#include "type.h"

struct Coordinate {
  int x;
  int y;
};

using vector_coordinates = std::vector<Coordinate>;

class Map {
 public:
  Map(int nb_player);
  ~Map();

  void create_map();
  void render_debug();
  void delete_player(Player& player);

  /** @brief Calcule la distance entre deux coordonnées */
  int distance_between(Coordinate c1, Coordinate c2);

  /** @brief Ajoute une unité sur la case */
  void add_unit_to_case(Case* target_case, UnitName name, Player* player);

  /** @brief Ajoute un bâtiment sur la case */
  void add_building_to_case(Case* target_case, BuildingName building);

  std::vector<std::vector<Case>>& get_cases() { return _cases; }
  std::vector<Player*> get_players() { return _players; }

 private:
  /** @brief Établit les liens de voisinage entre les cases (logique hexagonale)
   */
  void _link_hex_neighbors(int row, int col);

  /** @brief Calcule la distance entre points sur une grille hexagonale */
  int _get_hex_distance(int r1, int c1, int r2, int c2) const;

  void _reset_map();
  void _generate_ocean();
  void _generate_plains();
  void _generate_snow();
  void _generate_coasts();
  void _generate_mountains();
  bool _try_place_players(std::vector<std::pair<int, int>>& spawn_points);
  void _generate_resources(std::vector<std::pair<int, int>> spawn_points);

  /** @brief Vérifie si une ressource peut apparaître sur un type de terrain
   * spécifique. */
  bool _is_resource_compatible(ResourceName resource,
                               TerrainsType terrain) const;

  /** @brief Factory pour instancier le bon type d'unité (Aérienne, Terrestre,
   * etc). */
  Unit* _create_unit(UnitName name, Player* player, Case* c);

  int _size_h;
  int _size_w;

  std::vector<std::vector<Case>> _cases;
  std::vector<Player*> _players;
};