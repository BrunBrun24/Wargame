#include "game.h"

#include <QDebug>
#include <iostream>

Game::Game(int nb_player) : _nb_player(nb_player), _map(nb_player) {}

Game::Game(const QMap<int, Country>& PaysDesJoueurs) : _map(PaysDesJoueurs) {
  _nb_player = PaysDesJoueurs.size();
}

void Game::start_game() {
  qDebug() << "Game : Démarrage de la génération de la map...";
  _map.create_map();

  _players_list = _map.get_players();

  _current_player_index = 0;
  _turn_count = 1;

  if (!_players_list.empty()) {
    qDebug() << "--- DÉBUT DE LA PARTIE (TOUR 1) ---";
    Player* firstPlayer = _players_list[_current_player_index];

    // On initialise le tour du premier joueur (Reset PM etc.)
    firstPlayer->initialise_turn();

    qDebug() << "C'est au tour du Joueur ID:" << firstPlayer->get_id()
             << " Pays:" << static_cast<int>(firstPlayer->get_country());
  } else {
    qCritical() << "Erreur : Aucun joueur n'a été trouvé dans la Map !";
  }
}

Player* Game::get_current_player() {
  if (_players_list.empty()) {
    return nullptr;
  }
  return _players_list[_current_player_index];
}

void Game::next_turn() {
  if (_players_list.empty()) return;

  // 1. Passage à l'index suivant
  _current_player_index++;

  // 2. Boucle si on a dépassé le dernier joueur
  if (_current_player_index >= static_cast<int>(_players_list.size())) {
    _current_player_index = 0;
    _turn_count++;
    qDebug() << "--- TOUS LES JOUEURS ONT JOUÉ | DÉBUT DU TOUR" << _turn_count
             << " ---";
  }

  Player* nextPlayer = _players_list[_current_player_index];

  // 3. Gestion de l'élimination (on saute le tour si le joueur n'a plus de
  // villes) On ne vérifie la défaite qu'après quelques tours pour laisser le
  // temps de s'installer
  if (nextPlayer->is_dead() && _turn_count > 1) {
    qDebug() << "Le joueur" << nextPlayer->get_id()
             << "est éliminé. Passage au suivant.";
    next_turn();
    return;
  }

  // 4. On réactive les unités du nouveau joueur
  nextPlayer->initialise_turn();

  qDebug() << "Nouveau tour actif : Joueur" << nextPlayer->get_id();
}