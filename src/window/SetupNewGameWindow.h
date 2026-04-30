#pragma once

#include "../type.h"
#include "../map/map.h"
#include "../game/game.h"
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>
#include <QRadioButton>
class SetupNewGameWindow : public QWidget
{
    Q_OBJECT
public:
    SetupNewGameWindow(QWidget* parent = nullptr);
    void setMapPtr(Map* map) { 
        _mapData = map; 
        update(); // Force OpenGL à redessiner avec les nouvelles données
    }
signals:
    void backToMain(); // Si tu veux revenir au menu principal
    void gameCreated(Game* game);
private slots:
    void lancerLaPartie();
private: 
    QVBoxLayout* layoutJoueurs;
    QComboBox* selecteur;      
    void mettreAJourJoueurs(int index);
    QList<QList<QRadioButton*>> listeBoutonsJoueurs;
    void gererExclusivitePays(); // La fonction qui grise les pays déjà pris
    void verifierFormulaireValide();
    QPushButton* LancerPartieButton;
    Map* _mapData = nullptr; // Pointeur vers la logique de jeu
};
