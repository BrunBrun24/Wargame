#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "../type.h"

class Case;
class Player;

struct OrderRequest {
  UnitAction action = UnitAction::None;  // Action à effectuer
  int turns = 0;  // Nombre de Tours qui reste à effectuer pour les aménagements
  Case* target = nullptr;  // Case cible pour le déplacement
};

class Unit {
 public:
  Unit(UnitName name, Player* player, Case* case_unit,
       std::vector<TerrainsType> allow_terrain);
  virtual ~Unit();

  /** @brief Factory pour instancier le bon type d'unité (Aérienne, Terrestre,
   * etc). */
  static Unit* create_unit(const UnitName name, Player* player, Case* c);
  static std::vector<UnitName> get_all_units();
  static ImprovementName action_to_improvement(UnitAction action);
  static std::string get_unit_type_name(UnitType type);

  /** @brief Détermine si l'on peut atteindre la case en partant de l'unité */
  Course can_move_to(const Case* target_case);

  /** @brief Déplace l'unité sur la case */
  void execute_movement(Course course);

  /** @brief Evalue sur quelle case faut il déplacer l'unité et s'il faut oui ou
   * non ajouter des ordres dans la file d'attente */
  void go_to_move(Case* target_case);

  void move_to_city(Case* target_case);

  /** @brief Logique commune de calcul de force modifiée */
  double get_modified_strength_vs(const Unit* opponent,
                                  bool is_attacking) const;

  /** * @brief Estime les chances de victoire de l'unité lors d'un assaut.
   * @param defender L'unité cible.
   * @return Boléen retournant si true si l'attaquant gagne et false sinon
   */
  bool predict_victory_chance(const Unit* defender) const;

  /** * @brief Calcule la force de combat modifiée de l'attaquant contre un
   * défenseur spécifique.
   * @param defender L'unité qui subit l'attaque.
   * @return La puissance d'attaque finale après application de tous les bonus
   * (terrain, type, etc.).
   */
  double get_modified_attack_strength(const Unit* defender) const {
    return get_modified_strength_vs(defender, true);
  }

  /** * @brief Calcule la force de combat modifiée du défenseur contre cette
   * unité.
   * @param attacker L'unité qui initie l'attaque.
   * @return La puissance de défense finale après application des bonus de
   * terrain et de ville.
   */
  double get_modified_defense_strength(const Unit* attacker) const {
    return get_modified_strength_vs(attacker, false);
  }

  /**
   * @brief Calcule les dégâts subis par le vainqueur après un combat.
   * @param winner_power Puissance totale du vainqueur.
   * @param loser_power Puissance totale du perdant.
   * @return Le nombre de PV à retirer au vainqueur.
   */
  int calculate_survivor_damage(const Unit* defender) const;

  /** @brief Combat entre deux unités. */
  void fight(Unit* defender);

  /** @brief Soigne l'unité */
  void heal();

  /** @brief Vérifie si l'unité peut se déplacer sur un type de terrain
   * spécifique. */
  bool find_terrain(const TerrainsType& target_terrain) const;

  /** @brief Retourne une liste d'actions possibles que l'unité peut effectuer
   */
  virtual std::vector<UnitAction> get_unit_actions();

  /** @brief Récupère l'ordre donné par l'utilisateur et si elle est réalisable
   * pour ce tour alors l'exécuter sinon la mettre dans la file d'attente. */
  void get_order(UnitAction action, Case* target_case = nullptr);

  /** @brief Applique immédiatement les effets d'une action sur le jeu. */
  void execute_action(const UnitAction action);

  /** @brief Gère la progression de la file d'ordres au fil des tours. */
  void execute_orders();

  virtual void found_city() {};
  virtual void chop_down_forest() {};
  bool pillage_is_possible() const;
  void pillage();
  bool can_build_improvement(ImprovementName name);
  void build_improvement(ImprovementName name);
  bool is_military() const;
  bool contains_pending_orders() const;
  UnitStats get_stats() const { return stats; }
  std::vector<TerrainsType> get_allow_terrain() const { return allow_terrain; }

  void set_PM(const double n) {
    this->stats.PM = std::max(0.0, this->stats.PM - n);
    if (this->stats.PM == 0) {
      this->switch_active();
    }
  }

  void set_PM_null() {
    this->stats.PM = 0;
    this->switch_active();
  }

  int get_id() const { return id; }
  UnitName get_name() const { return name; }

  Player* get_player() const { return player; }
  void set_player(Player* p) { player = p; }

  Case* get_case_unit() const { return case_unit; }
  void set_unit_case(Case* c) { case_unit = c; }

  bool is_active() const { return active; }
  void switch_active() { active = !active; }

  bool is_on_guard() const { return on_guard; }
  void switch_on_guard() { on_guard = !on_guard; }

 protected:
  int id;
  UnitName name;
  Player* player;
  Case* case_unit;
  UnitStats stats;
  OrderRequest orders;
  std::vector<TerrainsType> allow_terrain;
  bool active;    // L'unité en attente d'ordre
  bool on_guard;  // L'unité se protège

 private:
  static int _id_counter;

  /** @brief On capture les unités civiles et on déplace l'unité qui les a
   * capturées */
  void _handle_capture_and_move(Case* target_case);
};
