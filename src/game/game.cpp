#include "game.h"

#include <iostream>

Game::Game(int nb_player) : _nb_player(nb_player), _map(nb_player) {}

Game::Game(const QMap<int, Country>& PaysDesJoueurs) : _map(PaysDesJoueurs) {
  _nb_player = PaysDesJoueurs.size();
}

void Game::start_game() {
  // On créer la map
  _map.create_map();
  _map.render_debug();

  turn_by_turn();
}

void Game::turn_by_turn() {
  std::vector<Player*> players = _map.get_players();
  int turn_count = 1;

  while (players.size() > 1) {
    std::cout << "\n========== TOUR " << turn_count
              << " ==========" << std::endl;

    for (auto it = players.begin(); it != players.end();) {
      Player* player = *it;
      if (player == nullptr) {
        it = players.erase(it);
        continue;
      }

      // Phase de jeu
      this->play(player);

      // Vérification après le tour (au cas où il aurait perdu sa dernière ville
      // pendant le tour adverse)
      if (player->is_dead()) {
        std::cout << "Le joueur " << player->get_id() << " a perdu la partie."
                  << std::endl;
        it = players.erase(it);  // On le retire de la boucle de jeu
      } else {
        ++it;
      }
    }

    turn_count++;

    // Condition de victoire
    if (players.size() == 1) {
      std::cout << "Le joueur " << players[0]->get_id() << " a gagné !"
                << std::endl;
    }
  }
}

// void Game::play(Player* player) {
//   // 1. Vérification de survie
//   if (player->is_dead()) {
//     std::cout << "Le joueur " << player->get_id()
//               << " n'a plus de villes et est éliminé." << std::endl;
//     return;
//   }

//   // 2. Initialisation du tour (Production et Technologie)
//   player->start_turn();

//   // 3. Phase d'action des unités
//   // Tant que le joueur a des unités qui peuvent encore bouger ou agir
//   while (player->has_active_units()) {
//     std::cout << "En attente d'ordres pour les unités de " << player->get_id()
//               << "..." << std::endl;

//     // Ici, tu appelleras ton interface pour sélectionner une unité
//     // Pour l'instant, on simule ou on attend une commande.
//     this->_handle_unit_orders(player);

//     // Optionnel : permettre de passer le tour manuellement pour sortir de la
//     // boucle
//     if (this->_user_wants_to_end_turn()) {
//       break;
//     }
//   }

//   std::cout << "Fin du tour pour " << player->get_id() << std::endl;
// }