#include "UnitStatsWidget.h"

#include <QPainter>
#include <QStyleOption>

#include "../../type.h"
#include "../../units/units.h"

UnitStatsWidget::UnitStatsWidget(QWidget* parent, const Unit* unit)
    : QWidget(parent) {
  // Force le widget à peindre son propre fond
  this->setAutoFillBackground(true);

  // Indique que le widget ne sera pas transparent (optimisation de rendu)
  this->setAttribute(Qt::WA_OpaquePaintEvent);

  // Votre style (Hexadécimal pour l'opacité totale)
  this->setStyleSheet("background-color: #1a1d1f;");

  _setup_ui(unit);
}

void UnitStatsWidget::paintEvent(QPaintEvent* event) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void UnitStatsWidget::_display_power_bonuses(const Unit* unit,
                                             QVBoxLayout* layout) {
  if (!unit) return;

  // Récupération des bonus depuis la map globale
  const PowerBonus& bonuses = UNIT_POWER_BONUS.at(unit->get_name());

  // Titre pour la section Bonus
  QLabel* bonusHeader = new QLabel("BONUS DE COMBAT", this);
  bonusHeader->setStyleSheet(
      "font-weight: bold; color: #f1c40f; margin-top: 10px;");
  layout->addWidget(bonusHeader);

  auto addBonusLabel = [&](const QString& text) {
    QLabel* label = new QLabel(text, this);
    label->setStyleSheet("color: #2ecc71; font-size: 11px; padding-left: 5px;");
    layout->addWidget(label);
  };

  // 1. Bonus par Type d'unité
  for (const auto& b : bonuses.units_types) {
    if (b.attack != 0.0) {
      QString typeName =
          QString::fromStdString(Unit::get_unit_type_name(b.type));
      addBonusLabel(QString("+%1% vs %2").arg(b.attack * 100).arg(typeName));
    }
  }

  // 2. Bonus par Nom d'unité spécifique
  for (const auto& b : bonuses.units_names) {
    if (b.attack != 0.0) {
      // Ici, vous devrez peut-être convertir UnitName en String via une méthode
      // similaire
      addBonusLabel(QString("+%1% vs Unité spécifique").arg(b.attack * 100));
    }
  }

  // 3. Bonus de Terrain
  for (const auto& b : bonuses.terrain) {
    if (b.attack != 0.0) {
      addBonusLabel(QString("+%1% en terrain élevé").arg(b.attack * 100));
    }
  }

  // 4. Bonus de Ville
  if (bonuses.city.attack != 0.0) {
    addBonusLabel(
        QString("+%1% en attaque de ville").arg(bonuses.city.attack * 100));
  }
  if (bonuses.city.defense != 0.0) {
    addBonusLabel(
        QString("+%1% en défense de ville").arg(bonuses.city.defense * 100));
  }
}

/**
 * @brief Initialise les composants graphiques et affiche les données de l'unité
 */
void UnitStatsWidget::_setup_ui(const Unit* unit) {
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setContentsMargins(15, 20, 15, 20);
  layout->setSpacing(15);

  // 1. Titre
  QLabel* title = new QLabel("STATISTIQUES", this);
  title->setStyleSheet("font-weight: bold; font-size: 18px; color: #ecf0f1;");
  layout->addWidget(title);

  // 2. Initialisation des composants (toujours les créer, même si unit est nul)
  _hp_bar = new QProgressBar(this);
  _atk_label = new QLabel(this);
  _unit_type_label = new QLabel(this);

  // Style de la barre HP
  _hp_bar->setTextVisible(true);
  _hp_bar->setAlignment(Qt::AlignCenter);
  _hp_bar->setStyleSheet(
      "QProgressBar {"
      "   border: 1px solid grey;"
      "   border-radius: 5px;"
      "   text-align: center;"
      "   background-color: #34495e;"
      "   color: white;"
      "   font-weight: bold;"
      "}"
      "QProgressBar::chunk {"
      "   background-color: #27ae60;"
      "   border-radius: 4px;"
      "}");

  // 3. Remplissage des données
  if (unit) {
    UnitStats stats = unit->get_stats();
    UnitName name = unit->get_name();

    // On récupère les HP max depuis la DB pour la plage de la barre
    int max_hp = UNIT_STATS.at(name).hp;
    _hp_bar->setRange(0, max_hp);
    _hp_bar->setValue(static_cast<int>(stats.hp));
    _hp_bar->setFormat(QString("%1 / %2").arg(stats.hp).arg(max_hp));

    _atk_label->setText(QString("Force Attaque : %1").arg(stats.power));

    // Récupération du type via la méthode de traduction
    std::string type_name = Unit::get_unit_type_name(UNIT_TYPE.at(name));
    _unit_type_label->setText(
        QString("Type d'unité : %1").arg(QString::fromStdString(type_name)));
  } else {
    _hp_bar->setRange(0, 100);
    _hp_bar->setValue(0);
    _hp_bar->setFormat("-- / --");
    _atk_label->setText("Force Attaque : --");
    _unit_type_label->setText("Type d'unité : --");
  }

  // Ajout au layout
  layout->addWidget(new QLabel("Points de Vie :", this));
  layout->addWidget(_hp_bar);
  layout->addWidget(_atk_label);
  layout->addWidget(_unit_type_label);
  _display_power_bonuses(unit, layout);

  layout->addStretch();
}
