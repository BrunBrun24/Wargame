#include "MainWindow.h"

#include <QMessageBox>
#include <QStackedWidget>

#include "../game/player.h"
#include "../map/city.h"
#include "Mainmenu.h"
#include "MyGLWidget.h"
#include "OptionMenu.h"
#include "ScreenProductions/CityProductionWidget.h"
#include "SetupNewGameWindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  stackedWidget = new QStackedWidget(this);

  mainMenu = new Mainmenu(this);
  optionMenu = new OptionMenu(this);
  setupmenu = new SetupNewGameWindow(this);
  test = new MyGLWidget(this);

  // 2. CONFIGURATION DE L'INTERFACE FLOTTANTE
  productionInterface = new CityProductionWidget(this);
  productionInterface->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
  productionInterface->setWindowModality(Qt::WindowModal);
  productionInterface->setFixedSize(400, 500);
  // On simplifie le style pour tester la validité
  productionInterface->setStyleSheet(
      "CityProductionWidget { "
      "  background-color: #000000; "
      "  border: 2px solid #555555; "
      "  border-radius: 10px; "
      "} "
      "QGroupBox { "
      "  background-color: #1A1A1A; "
      "  color: white; "
      "  border: 1px solid #333; "
      "  margin-top: 10px; "
      "  padding: 5px; "
      "} "
      "QScrollArea, QWidget#qt_scrollarea_viewport, #unit_container, "
      "#building_container { "
      "  background-color: transparent; "
      "  border: none; "
      "}");
  productionInterface->hide();

  // 3. AJOUT À LA PILE (Uniquement les écrans principaux)
  stackedWidget->addWidget(mainMenu);
  stackedWidget->addWidget(optionMenu);
  stackedWidget->addWidget(setupmenu);
  stackedWidget->addWidget(test);

  setCentralWidget(stackedWidget);
  this->setMinimumSize(800, 600);
  // Connecte les signal
  // On connecte mainMenu, avec le signal menuChanged
  // sur soi même, on fais la fonction lambda qui suis
  // elle change le widget actuel.
  connect(mainMenu, &Mainmenu::menuChanged, this, [=](int menu) {
    if (menu == 3) {
      stackedWidget->setCurrentWidget(
          optionMenu);  // Change la "feuille" visible
    } else if (menu == 1) {
      stackedWidget->setCurrentWidget(setupmenu);
    }
  });

  // Connection pour revenir au menu principal
  // elle font la même chose, juste pas du même endroit.
  connect(optionMenu, &OptionMenu::backToMain, this,
          [=]() { stackedWidget->setCurrentWidget(mainMenu); });
  connect(setupmenu, &SetupNewGameWindow::backToMain, this,
          [=]() { stackedWidget->setCurrentWidget(mainMenu); });
  connect(setupmenu, &SetupNewGameWindow::gameCreated, this, [=](Game* game) {
    // 1. Configuration de base du jeu
    test->setMapPtr(game->getMap());
    test->setGamePtr(game);

    // On affiche l'écran de jeu (MyGLWidget)
    stackedWidget->setCurrentWidget(test);

    // 2. Connexions des signaux de l'interface de production (UNE SEULE FOIS)
    // On utilise hide() car c'est un overlay flottant, pas une page du
    // stackedWidget
    connect(this->productionInterface, &CityProductionWidget::unitSelected,
            this, [=](UnitName unit) {
              if (this->cityInEdition) {
                qDebug() << "[MainWindow] Unité ajoutée à la file :"
                         << (int)unit;
                this->cityInEdition->push_unit(unit);
                this->productionInterface->hide();
                test->update();  // Rafraîchit l'affichage OpenGL
              }
            });

    connect(this->productionInterface, &CityProductionWidget::buildingSelected,
            this, [=](BuildingName building) {
              if (this->cityInEdition) {
                qDebug() << "[MainWindow] Bâtiment ajouté à la file :"
                         << (int)building;
                this->cityInEdition->push_building(building);
                this->productionInterface->hide();
                test->update();
              }
            });

    // 3. Connexion dynamique des villes existantes et gestion du Tour 1
    for (Player* p : game->get_players()) {
      for (City* c : p->get_citys()) {
        // Signal émis quand une ville finit de produire ou demande un choix
        connect(c, &City::productionRequired, this,
                [=](City* city, ProductionAvailable pa) {
                  this->cityInEdition = city;
                  this->productionInterface->updateProductionList(pa);

                  // Affichage flottant centré
                  this->productionInterface->show();
                  this->productionInterface->raise();
                  this->productionInterface->move(
                      this->rect().center() -
                      this->productionInterface->rect().center());
                });

        // CAS SPÉCIFIQUE : TOUR 1 (Capitale)
        if (c->is_capital()) {
          qDebug() << "[MainWindow] Ouverture auto : Production Capitale ID"
                   << c->get_id();
          this->cityInEdition = c;
          this->productionInterface->updateProductionList(
              c->production_available());

          this->productionInterface->show();
          this->productionInterface->raise();

          // Centrage immédiat
          this->productionInterface->move(
              this->rect().center() -
              this->productionInterface->rect().center());
        }
      }
    }
  });
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  // Appelle d'abord l'implémentation de base
  QMainWindow::resizeEvent(event);

  // Si le menu de production existe et est affiché, on le recentre
  if (productionInterface && productionInterface->isVisible()) {
    productionInterface->move(this->rect().center() -
                              productionInterface->rect().center());
  }
}