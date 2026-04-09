#include "OptionMenu.h"

#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

OptionMenu::OptionMenu(QWidget* parent)
    : QWidget(parent)
{
    QLabel* titre = new QLabel("OPTIONS DE RÉSOLUTION");
    titre->setStyleSheet("font-weight: bold; font-size: 18px;");

    QComboBox *selecteur = new QComboBox();
    selecteur->addItem("800, 600"); //Resolution de base;
    selecteur->setCurrentIndex(0);
    selecteur->addItem("1280, 720");
    selecteur->addItem("1600, 900");
    selecteur->addItem("1920, 1080");

    selecteur->setMaximumWidth(300);

    QPushButton* backButton = new QPushButton("Retour");
    backButton->setMaximumWidth(200);

    QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);

    layoutPrincipal->addStretch(); // Pousse tout vers le bas
    
    layoutPrincipal->addWidget(titre, 0, Qt::AlignCenter);
    layoutPrincipal->addSpacing(20); // Petit espace vide
    layoutPrincipal->addWidget(selecteur, 0, Qt::AlignCenter);
    layoutPrincipal->addSpacing(10);
    layoutPrincipal->addWidget(backButton, 0, Qt::AlignCenter);
    
    layoutPrincipal->addStretch(); // Pousse tout vers le haut

    this->setLayout(layoutPrincipal);

    // Connect le bouton
    connect(backButton, &QPushButton::clicked, this, &OptionMenu::backToMain);
    connect(selecteur,&QComboBox::currentIndexChanged, this, [=](int index){
        switch(index){
            case 0:
                this->window()->resize(800,600);break;
            case 1:
                this->window()->resize(1280,720);break;
            case 2:
                this->window()->resize(1600,900);break;
            case 3:
                this->window()->resize(1920,1080);break;
        }
    });
}
