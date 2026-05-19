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

void UnitActionsWidget::_setup_ui(const Unit* unit, const Case* currentCase) {
  // Nettoyage et initialisation du layout
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

  if (uName == UnitName::Settler) {
    qDebug() << "COLON";
  }

  std::vector<UnitAction> actions = unit->get_unit_actions();

  for (UnitAction action : actions) {
    switch (action) {
      case UnitAction::BuildCity:
        qDebug() << "VILLE";
        _create_action_button("Found city", UnitAction::BuildCity,
                              actionLayout);
        break;

      case UnitAction::GoToMove:
        _create_action_button("Go to move", UnitAction::GoToMove, actionLayout);
        break;

      case UnitAction::Sleep:
        _create_action_button("Sleep", UnitAction::Sleep, actionLayout);
        break;

      case UnitAction::Wake:
        _create_action_button("Wake", UnitAction::Wake, actionLayout);
        break;

      case UnitAction::Fortify:
        _create_action_button("Fortify", UnitAction::Fortify, actionLayout);
        break;

      case UnitAction::Delete:
        _create_action_button("Delete", UnitAction::Delete, actionLayout);
        break;

      case UnitAction::Pillage:
        _create_action_button("Pillage", UnitAction::Pillage, actionLayout);
        break;

      case UnitAction::BuildRoad:
        _create_action_button("Build Road", UnitAction::BuildRoad,
                              actionLayout);
        break;

      case UnitAction::BuildFarm:
        _create_action_button("Build Farm", UnitAction::BuildFarm,
                              actionLayout);
        break;

      case UnitAction::BuildMine:
        _create_action_button("Build Mine", UnitAction::BuildMine,
                              actionLayout);
        break;

      case UnitAction::BuildCamp:
        _create_action_button("Build Camp", UnitAction::BuildCamp,
                              actionLayout);
        break;

      case UnitAction::BuildCottage:
        _create_action_button("Build Cottage", UnitAction::BuildCottage,
                              actionLayout);
        break;

      case UnitAction::BuildForestPreserve:
        _create_action_button("Build Forest Preserve",
                              UnitAction::BuildForestPreserve, actionLayout);
        break;

      case UnitAction::BuildPasture:
        _create_action_button("Build Pasture", UnitAction::BuildPasture,
                              actionLayout);
        break;

      case UnitAction::BuildPlantation:
        _create_action_button("Build Plantation", UnitAction::BuildPlantation,
                              actionLayout);
        break;

      case UnitAction::BuildQuarry:
        _create_action_button("Build Quarry", UnitAction::BuildQuarry,
                              actionLayout);
        break;

      case UnitAction::BuildLumberMill:
        _create_action_button("Build Lumber Mill", UnitAction::BuildLumberMill,
                              actionLayout);
        break;

      case UnitAction::ChopDownForest:
        _create_action_button("Chop Down Forest", UnitAction::ChopDownForest,
                              actionLayout);
        break;

      case UnitAction::BuildFishingBoats:
        _create_action_button("Build Fishing Boats",
                              UnitAction::BuildFishingBoats, actionLayout);
        break;

      case UnitAction::BuildWatermill:
        _create_action_button("Build Watermill", UnitAction::BuildWatermill,
                              actionLayout);
        break;

      case UnitAction::BuildWell:
        _create_action_button("Build Well", UnitAction::BuildWell,
                              actionLayout);
        break;

      case UnitAction::BuildWorkshop:
        _create_action_button("Build Workshop", UnitAction::BuildWorkshop,
                              actionLayout);
        break;

      case UnitAction::BuildWinery:
        _create_action_button("Build Winery", UnitAction::BuildWinery,
                              actionLayout);
        break;

      case UnitAction::BuildWindmill:
        _create_action_button("Build Windmill", UnitAction::BuildWindmill,
                              actionLayout);
        break;

      case UnitAction::BuildWhalingBoats:
        _create_action_button("Build Whaling Boats",
                              UnitAction::BuildWhalingBoats, actionLayout);
        break;

      case UnitAction::BuildOffshorePlatform:
        _create_action_button("Build Offshore Platform",
                              UnitAction::BuildOffshorePlatform, actionLayout);
        break;

      case UnitAction::BuildFort:
        _create_action_button("Build Fort", UnitAction::BuildFort,
                              actionLayout);
        break;
    }
  }

  // // 1. Settler
  // if (uName == UnitName::Settler) {
  //   if (!currentCase->has_city()) {
  //     std::cout << "CITY";

  //   }

  //   // 2. Worker
  //   else if (uName == UnitName::Worker || uName == UnitName::WorkBoat) {
  //     // A. Améliorations basées sur la Ressource
  //     if (t.resource != ResourceName::None) {
  //       const Bonus& info = ResourceDatabase::get_info(t.resource);
  //       std::string label =
  //           ResourceDatabase::get_improvement_name_str(t.resource);
  //       _create_action_button(QString::fromStdString("Bâtir " + label),
  //                             info.resource_bonus.unit_action, actionLayout);
  //     }
  //     // B. Améliorations de terrain (Sans ressource)
  //     else {
  //       if (t.elevation == TerrainElevation::Hill) {
  //         _create_action_button("Bâtir Mine", UnitAction::BuildMine,
  //                               actionLayout);
  //       } else if (t.type == TerrainsType::Grassland ||
  //                  t.type == TerrainsType::Plains) {
  //         _create_action_button("Bâtir Ferme", UnitAction::BuildFarm,
  //                               actionLayout);
  //         _create_action_button("Bâtir Hameau", UnitAction::BuildCottage,
  //                               actionLayout);
  //       }
  //     }

  //     // C. Aménagements de Features (Forêt/Jungle)
  //     if (t.feature == TerrainFeature::Forest) {
  //       _create_action_button("Couper Forêt", UnitAction::ChopDownForest,
  //                             actionLayout);
  //       _create_action_button("Scierie", UnitAction::BuildLumberMill,
  //                             actionLayout);
  //       _create_action_button("Réserve", UnitAction::BuildForestPreserve,
  //                             actionLayout);
  //     } else if (t.feature == TerrainFeature::Jungle) {
  //       _create_action_button("Dégager Jungle", UnitAction::ChopDownForest,
  //                             actionLayout);
  //     }

  //     // D. Infrastructures & Spéciaux
  //     _create_action_button("Route", UnitAction::BuildRoad, actionLayout);
  //     _create_action_button("Fort", UnitAction::BuildFort, actionLayout);

  //     if (t.type == TerrainsType::Desert) {
  //       _create_action_button("Puits", UnitAction::BuildWell, actionLayout);
  //     }
  //   }

  //   // 3. Workboat
  //   else if (uName == UnitName::WorkBoat && t.type == TerrainsType::Coast) {
  //     if (t.resource == ResourceName::Fish)
  //       _create_action_button("Pêcher", UnitAction::BuildFishingBoats,
  //                             actionLayout);
  //     else if (t.resource == ResourceName::Whale)
  //       _create_action_button("Baleinier", UnitAction::BuildWhalingBoats,
  //                             actionLayout);
  //   }

  //   // 4. Unité militaire terrestre
  //   else {
  //     _create_action_button("Retrancher", UnitAction::Fortify, actionLayout);

  //     // Uniquement si l'unité peut bombarder (ex: Catapulte)
  //     if (uName == UnitName::Catapult || uName == UnitName::Cannon) {
  //       _create_action_button("Bombarder", UnitAction::Bombard,
  //       actionLayout);
  //     }
  //   }

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
      "solid #34495e; padding: 8px 15px; border-radius: 4px; font-size: "
      "11px; }"
      "QPushButton:hover { background-color: #2980b9; }";
  btn->setStyleSheet(btnStyle);
  btn->setCursor(Qt::PointingHandCursor);
  connect(btn, &QPushButton::clicked, this,
          [this, action]() { emit actionTriggered(action); });

  layout->addWidget(btn);
}