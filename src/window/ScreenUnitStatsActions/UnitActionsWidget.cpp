#include "UnitActionsWidget.h"

#include <QPainter>
#include <QStyleOption>

#include "../../map/case.h"
#include "../../map/resource_database.h"
#include "../../units/units.h"
#include "QLabel"

UnitActionsWidget::UnitActionsWidget(QWidget* parent, const Unit* unit,
                                     const Case* currentCase)
    : QWidget(parent) {
  // Votre style (Hexadécimal pour l'opacité totale)
  this->setStyleSheet("background-color: #1a1d1f;");
  _setup_ui(unit, currentCase);
}

void UnitActionsWidget::paintEvent(QPaintEvent* event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UnitActionsWidget::_clear_layout() {
  QLayoutItem* item;
  // On vide le layout proprement sans supprimer le layout lui-même
  while ((item = layout()->takeAt(0)) != nullptr) {
    if (item->widget()) delete item->widget();
    delete item;
  }
}

/**
 * @brief Configure l'interface utilisateur avec toutes les actions possibles.
 * @param unit L'unité active.
 * @param currentCase La case occupée.
 */
void UnitActionsWidget::_setup_ui(const Unit* unit, const Case* currentCase) {
  // 1. Nettoyage et initialisation du layout
  if (layout()) {
    QLayoutItem* item;
    while ((item = layout()->takeAt(0)) != nullptr) {
      if (item->widget()) delete item->widget();
      delete item;
    }
  } else {
    new QHBoxLayout(this);
  }

  QHBoxLayout* actionLayout = qobject_cast<QHBoxLayout*>(this->layout());
  if (!unit || !currentCase) return;

  const Terrain& t = currentCase->get_terrain();
  const UnitName uName = unit->get_name();

  // --- ACTIONS UNIVERSELLES ---
  _create_action_button("Sommeil", UnitAction::Sleep, actionLayout);
  _create_action_button("Passer", UnitAction::SkipTurn, actionLayout);
  _create_action_button("Supprimer", UnitAction::Delete, actionLayout);

  // --- ACTIONS PAR TYPE D'UNITÉ ---

  // 1. LE COLON (SETTLER)
  if (uName == UnitName::Settler) {
    if (!currentCase->has_city()) {
      std::cout << "CITY";
      _create_action_button("Fonder Ville", UnitAction::BuildCity,
                            actionLayout);
    }
  }

  // 2. L'OUVRIER (WORKER)
  else if (uName == UnitName::Worker || uName == UnitName::WorkBoat) {
    // A. Améliorations basées sur la Ressource
    if (t.resource != ResourceName::None) {
      const Bonus& info = ResourceDatabase::get_info(t.resource);
      std::string label =
          ResourceDatabase::get_improvement_name_str(t.resource);
      _create_action_button(QString::fromStdString("Bâtir " + label),
                            info.resource_bonus.unit_action, actionLayout);
    }
    // B. Améliorations de terrain (Sans ressource)
    else {
      if (t.elevation == TerrainElevation::Hill) {
        _create_action_button("Bâtir Mine", UnitAction::BuildMine,
                              actionLayout);
      } else if (t.type == TerrainsType::Grassland ||
                 t.type == TerrainsType::Plains) {
        _create_action_button("Bâtir Ferme", UnitAction::BuildFarm,
                              actionLayout);
        _create_action_button("Bâtir Hameau", UnitAction::BuildCottage,
                              actionLayout);
      }
    }

    // C. Aménagements de Features (Forêt/Jungle)
    if (t.feature == TerrainFeature::Forest) {
      _create_action_button("Couper Forêt", UnitAction::ChopDownForest,
                            actionLayout);
      _create_action_button("Scierie", UnitAction::BuildLumberMill,
                            actionLayout);
      _create_action_button("Réserve", UnitAction::BuildForestPreserve,
                            actionLayout);
    } else if (t.feature == TerrainFeature::Jungle) {
      _create_action_button("Dégager Jungle", UnitAction::ChopDownForest,
                            actionLayout);
    }

    // D. Infrastructures & Spéciaux
    _create_action_button("Route", UnitAction::BuildRoad, actionLayout);
    _create_action_button("Fort", UnitAction::BuildFort, actionLayout);

    if (t.type == TerrainsType::Desert) {
      _create_action_button("Puits", UnitAction::BuildWell, actionLayout);
    }
  }

  // 3. BATEAU DE TRAVAIL (WORKBOAT)
  else if (uName == UnitName::WorkBoat && t.type == TerrainsType::Coast) {
    if (t.resource == ResourceName::Fish)
      _create_action_button("Pêcher", UnitAction::BuildFishingBoats,
                            actionLayout);
    else if (t.resource == ResourceName::Whale)
      _create_action_button("Baleinier", UnitAction::BuildWhalingBoats,
                            actionLayout);
  }

  // 4. UNITÉS MILITAIRES (Terrestres)
  else {
    _create_action_button("Retrancher", UnitAction::Fortify, actionLayout);
    _create_action_button("Piller", UnitAction::Pillage, actionLayout);

    // Uniquement si l'unité peut bombarder (ex: Catapulte)
    if (uName == UnitName::Catapult || uName == UnitName::Cannon) {
      _create_action_button("Bombarder", UnitAction::Bombard, actionLayout);
    }
  }

  actionLayout->addStretch();
  this->layout()->activate();
  this->update();
}

void UnitActionsWidget::_create_action_button(const QString& text,
                                              UnitAction action,
                                              QHBoxLayout* layout) {
  QPushButton* btn = new QPushButton(text, this);

  QString btnStyle =
      "QPushButton { background-color: #2c3e50; color: white; border: 1px "
      "solid #34495e; padding: 8px 15px; border-radius: 4px; font-size: 11px; }"
      "QPushButton:hover { background-color: #2980b9; }";
  btn->setStyleSheet(btnStyle);

  // Connexion du signal (exemple avec une lambda capturant l'action)
  connect(btn, &QPushButton::clicked, this, [this, action]() {
    // Emettre un signal vers le moteur de jeu
    // emit actionTriggered(action);
  });

  layout->addWidget(btn);
}