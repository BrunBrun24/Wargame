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

  /** @brief Calcule la distance entre deux coordonnées */
  int distance_between(const Coordinate c1, const Coordinate c2) const;

  /** @brief Ajoute une unité sur la case */
  void add_unit_to_case(Case* target_case, const UnitName name,
                        Player* player) const;

  /** @brief Ajoute un bâtiment sur la case */
  void add_building_to_case(Case* target_case,
                            const BuildingName building) const;

  void create_map();
  void render_debug() const;
  void delete_player(Player* player);

  std::vector<Player*> get_players() { return _players; }
  std::vector<std::vector<Case>>& get_cases() { return _cases; }

 private:
  int _size_h;
  int _size_w;
  std::vector<Player*> _players;
  std::vector<std::vector<Case>> _cases;

  /** @brief Établit les liens de voisinage entre les cases */
  void _link_hex_neighbors(const int row, const int col);

  /** @brief Calcule la distance entre points sur une grille hexagonale */
  int _get_hex_distance(const int r1, const int c1, const int r2,
                        const int c2) const;

  /** @brief Vérifie si une ressource peut apparaître sur un type de terrain
   * spécifique. */
  bool _is_resource_compatible(const ResourceName resource,
                               const TerrainsType terrain) const;

  /** @brief Factory pour instancier le bon type d'unité (Aérienne, Terrestre,
   * etc). */
  Unit* _create_unit(const UnitName name, Player* player, Case* c) const;

  void _reset_map();
  void _generate_snow();
  void _generate_ocean();
  void _generate_plains();
  void _generate_coasts();
  void _generate_mountains();
  bool _try_place_players(std::vector<std::pair<int, int>>& spawn_points);
  void _generate_resources(const std::vector<std::pair<int, int>> spawn_points);
  
  int _get_continent_size(int const row, int const col) const;
};