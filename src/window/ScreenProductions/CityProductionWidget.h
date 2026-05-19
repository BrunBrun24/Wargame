#ifndef CITY_PRODUCTION_WIDGET_H
#define CITY_PRODUCTION_WIDGET_H

#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "../../map/city.h"
#include "../../type.h"

class CityProductionWidget : public QWidget {
  Q_OBJECT

 public:
  explicit CityProductionWidget(QWidget* parent = nullptr);
  void updateProductionList(const ProductionAvailable& pa);

 signals:
  void unitSelected(UnitName unit);
  void buildingSelected(BuildingName building);

 private:
  QVBoxLayout* _main_layout;

  // Conteneurs pour les boutons (ceux qui vont dans la ScrollArea)
  QWidget* _unit_container;
  QVBoxLayout* _unit_layout;

  QWidget* _building_container;
  QVBoxLayout* _building_layout;

  // Fonction utilitaire pour créer une zone de défilement stylisée
  QScrollArea* _create_scroll_area(QWidget* content);
  void _clear_layout(QLayout* layout);

  QString _unit_to_string(UnitName unit);
  QString _building_to_string(BuildingName building);
};

#endif