#pragma once
#include <QVBoxLayout>
#include <QWidget>

class Case;
class Unit;
class UnitStatsWidget;
class UnitActionsWidget;

class UnitControlPanel
    : public QObject {  // Devient un QObject pour gérer la logique
  Q_OBJECT
 public:
  // On passe le widget parent (MainWindow) pour y ancrer les fenêtres
  explicit UnitControlPanel(QWidget* parent, const Unit* unit, const Case* currentCase);

 private:
  UnitStatsWidget* _stats_widget;
  UnitActionsWidget* _actions_widget;
};