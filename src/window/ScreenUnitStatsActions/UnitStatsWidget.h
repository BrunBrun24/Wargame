#pragma once
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QWidget>

class Unit;

class UnitStatsWidget : public QWidget {
  Q_OBJECT
 public:
  explicit UnitStatsWidget(QWidget* parent, const Unit* unit);
  ~UnitStatsWidget() = default;

 protected:
  void paintEvent(QPaintEvent* event);

 private:
  /**
   * @brief Initialise les composants graphiques du widget
   */
  void _setup_ui(const Unit* unit);

  /**
   * @brief Ajoute les labels de bonus de puissance au layout
   * @param unit L'unité dont on veut afficher les bonus
   * @param layout Le layout parent où insérer les labels
   */
  void _display_power_bonuses(const Unit* unit, QVBoxLayout* layout);

  QProgressBar* _hp_bar;
  QLabel* _atk_label;
  QLabel* _unit_type_label;
};