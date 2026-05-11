#pragma once
#include <QVBoxLayout>
#include <QWidget>
#include <QComboBox>
#include <QPushButton>

class Case;
class Unit;
class UnitStatsWidget;
class UnitActionsWidget;

class UnitControlPanel : public QObject {
    Q_OBJECT
public:
    // On accepte maintenant un vecteur d'unités au lieu d'une seule
    explicit UnitControlPanel(QWidget* parent, const std::vector<Unit*>& units, const Case* currentCase);
    bool isInterfaceHidden(){return isHidden;};
    void showAll();
    Unit* getSelectedUnit() const;
private slots:
    void onUnitChanged(int index);
    void hideAll(); // Slot pour masquer
private:
    bool isHidden;
    QPushButton* _close_btn; // Ajoute ceci
    UnitStatsWidget* _stats_widget;
    UnitActionsWidget* _actions_widget;
    QComboBox* _unit_selector;
    std::vector<Unit*> _units;
    const Case* _current_case;
    QWidget* _parent_ref; // Garder une référence pour repositionner si besoin
};