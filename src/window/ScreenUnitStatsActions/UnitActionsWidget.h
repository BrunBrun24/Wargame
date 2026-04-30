#pragma once
#include <QGridLayout>
#include <QPushButton>
#include <QWidget>

#include "../type.h"

class Unit;
class Case;

class UnitActionsWidget : public QWidget {
  Q_OBJECT
 public:
  explicit UnitActionsWidget(QWidget* parent, const Unit* unit,
                             const Case* currentCase);

 protected:
  void paintEvent(QPaintEvent* event);

 private:
  void _clear_layout();
  void _setup_ui(const Unit* unit, const Case* currentCase);
  void _create_action_button(const QString& text, UnitAction action,
                             QHBoxLayout* layout);
};