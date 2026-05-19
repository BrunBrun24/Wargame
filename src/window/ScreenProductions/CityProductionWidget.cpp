#include "CityProductionWidget.h"

#include "../../map/building_database.h"
#include "../../type.h"

CityProductionWidget::CityProductionWidget(QWidget* parent) : QWidget(parent) {
  _main_layout = new QVBoxLayout(this);
  _main_layout->setSpacing(10);

  // Section UNITÉS
  QGroupBox* unitBox = new QGroupBox("Unités disponibles", this);
  QVBoxLayout* unitBoxLayout = new QVBoxLayout(unitBox);

  _unit_container = new QWidget();
  _unit_layout = new QVBoxLayout(_unit_container);
  _unit_layout->setAlignment(Qt::AlignTop);  // Aligner les boutons en haut

  unitBoxLayout->addWidget(_create_scroll_area(_unit_container));
  _main_layout->addWidget(unitBox, 1);  // Poids de 1 pour l'équilibre visuel

  // Section BÂTIMENTS
  QGroupBox* bldBox = new QGroupBox("Bâtiments disponibles", this);
  QVBoxLayout* bldBoxLayout = new QVBoxLayout(bldBox);

  _building_container = new QWidget();
  _building_layout = new QVBoxLayout(_building_container);
  _building_layout->setAlignment(Qt::AlignTop);

  bldBoxLayout->addWidget(_create_scroll_area(_building_container));
  _main_layout->addWidget(bldBox, 1);

  // Style général (CSS optionnel pour faire "pro")
  this->setStyleSheet(
      "QGroupBox { font-weight: bold; font-size: 14px; border: 2px solid gray; "
      "border-radius: 5px; margin-top: 10px; }"
      "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 "
      "3px; }"
      "QPushButton { background-color: #444; color: white; border-radius: 3px; "
      "padding: 8px; text-align: left; }"
      "QPushButton:hover { background-color: #666; }"
      "QPushButton:pressed { background-color: #222; }");
}

QScrollArea* CityProductionWidget::_create_scroll_area(QWidget* content) {
  QScrollArea* scroll = new QScrollArea(this);
  scroll->setWidgetResizable(true);
  scroll->setWidget(content);
  scroll->setFrameShape(QFrame::NoFrame);  // Enlever la bordure interne moche
  scroll->setHorizontalScrollBarPolicy(
      Qt::ScrollBarAlwaysOff);  // Uniquement vertical
  return scroll;
}

void CityProductionWidget::updateProductionList(const ProductionAvailable& pa) {
  _clear_layout(_unit_layout);
  _clear_layout(_building_layout);

  // Ajout des boutons d'unités
  for (UnitName unit : pa.units) {
    if (unit == UnitName::None) continue;

    QPushButton* btn = new QPushButton(_unit_to_string(unit), this);
    // On peut même ajouter une icône ici plus tard : btn->setIcon(QIcon(...));

    connect(btn, &QPushButton::clicked, [this, unit]() {
      qDebug() << "[Widget] Bouton unité cliqué pour ID:" << (int)unit;
      emit unitSelected(unit);
    });

    _unit_layout->addWidget(btn);
  }

  // Ajout des boutons de bâtiments
  for (BuildingName bld : pa.buildings) {
    if (bld == BuildingName::None) continue;

    QPushButton* btn = new QPushButton(_building_to_string(bld), this);
    connect(btn, &QPushButton::clicked, [this, bld]() {
      qDebug() << "[Widget] Bouton bâtiment cliqué pour ID:" << (int)bld;
      emit buildingSelected(bld);
    });
    _building_layout->addWidget(btn);
  }
}

void CityProductionWidget::_clear_layout(QLayout* layout) {
  if (!layout) return;
  while (QLayoutItem* item = layout->takeAt(0)) {
    if (QWidget* widget = item->widget()) {
      widget->deleteLater();
    }
    delete item;
  }
}

QString CityProductionWidget::_unit_to_string(UnitName unit) {
  return QString("%1").arg(UNIT_NAME_TO_STRING.at(unit));
}

QString CityProductionWidget::_building_to_string(BuildingName building) {
  return QString("%1")
      .arg(BuildingDatabase::get_building_name_str(building));
}