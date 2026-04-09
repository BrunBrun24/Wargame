#include "SetupNewGameWindow.h"

#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>


SetupNewGameWindow::SetupNewGameWindow(QWidget* parent)
    : QWidget(parent)
{
    //Setup de combobox pour choisir le nombre de joueurs
    QLabel* titre = new QLabel("Nombre de Pays");
    titre->setStyleSheet("font-weight: bold; font-size: 18px;");
    int MaxNumberCountryPlayers = 10;
    QComboBox *selecteur = new QComboBox();
    for(int i = 2; i <= MaxNumberCountryPlayers; ++i ){ //je mets 10 comme base
        selecteur->addItem(QString("%1 Pays").arg(i));
    }
    selecteur->setCurrentIndex(0);//On mets en base 2 joueurs.

    //on créer le bouton pour revenir.
    QPushButton* backButton = new QPushButton("Retour");
    backButton->setMaximumWidth(200);

    //on créer le layout.
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);

    layoutPrincipal->addStretch(); // Pousse tout vers le bas
    
    layoutPrincipal->addWidget(titre, 0);
    layoutPrincipal->addSpacing(20); // Petit espace vide
    layoutPrincipal->addWidget(selecteur, 0);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addWidget(backButton, 0);
    
    layoutPrincipal->addStretch(); // Pousse tout vers le haut

    this->setLayout(layoutPrincipal); //On ajoute ce layout dans la page

    // Connect le bouton
    connect(backButton, &QPushButton::clicked, this, &SetupNewGameWindow::backToMain);
}