#include "Troupes.h"

void Troupes::attaque(Troupes * ennemy){
    if (ennemy->get_ekip() != _ekip){
        ennemy->change_pv(_force);
        this->change_pv(ennemy->get_force() - ennemy->get_def());
        if (ennemy->get_pv() <= 0){
            delete ennemy;
        }
        if (_pv <= 0){
            delete this;
        }
    }
};