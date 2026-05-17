#include "UnitControlPanel.h"
#include "UnitStatsWidget.h"
#include "UnitActionsWidget.h"
#include "units.h"
#include "../../type.h"
#include <QGridLayout>
#include <QPushButton>

UnitControlPanel::UnitControlPanel(QWidget* parent, const std::vector<Unit*>& units, const Case* currentCase) 
    : QObject(parent), _units(units), _current_case(currentCase), _parent_ref(parent) {
    
    if (_units.empty()) return;

    // 1. Création du sélecteur
    _unit_selector = new QComboBox(parent);
    for (Unit* u : _units) {
        std::string nameStr = UNIT_NAME_TO_STRING.at(u->get_name());
        _unit_selector->addItem(QString::fromStdString(nameStr));
    }
    _unit_selector->setFixedWidth(220);
    _unit_selector->setStyleSheet("background-color: #2c3e50; color: white; padding: 5px; font-weight: bold;");

    // 2. Création du bouton de fermeture
    _close_btn = new QPushButton("x", parent);
    _close_btn->setFixedSize(26, 26);
    _close_btn->setCursor(Qt::PointingHandCursor);
    _close_btn->setStyleSheet(
        "QPushButton { background-color: #c0392b; color: white; border-radius: 13px; font-weight: bold; font-size: 18px; border: none; }"
        "QPushButton:hover { background-color: #e74c3c; }"
    );
    
    // On le place une première fois
    _close_btn->move(parent->width() - 40, 10);

    // 3. Création des widgets initiaux via onUnitChanged
    // On initialise d'abord les membres à nullptr pour éviter les crashs au premier appel
    _stats_widget = nullptr;
    _actions_widget = nullptr;

    // Connexions
    connect(_unit_selector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitControlPanel::onUnitChanged);
    
    // Action de fermeture : cache tout directement
    connect(_close_btn, &QPushButton::clicked, this, [this]() {
        hideAll();
    });
    
    // Charger la première unité
    onUnitChanged(0);
}

void UnitControlPanel::onUnitChanged(int index) {
    if (index < 0 || index >= static_cast<int>(_units.size())) return;

    Unit* selectedUnit = _units[index];
    QGridLayout* layout = qobject_cast<QGridLayout*>(_parent_ref->layout());

    // 1. Suppression des anciens widgets dynamiques
    if (_stats_widget) {
        if (layout) layout->removeWidget(_stats_widget);
        _stats_widget->deleteLater();
        _stats_widget = nullptr;
    }
    if (_actions_widget) {
        if (layout) layout->removeWidget(_actions_widget);
        _actions_widget->deleteLater();
        _actions_widget = nullptr;
    }

    // 2. Création des nouveaux widgets
    _stats_widget = new UnitStatsWidget(_parent_ref, selectedUnit);
    _stats_widget->setFixedWidth(250);
    _actions_widget = new UnitActionsWidget(_parent_ref, selectedUnit, _current_case);
    _actions_widget->setFixedHeight(125);
    
    connect(_actions_widget, &UnitActionsWidget::actionTriggered, this, [this, selectedUnit](UnitAction action) {
        if (selectedUnit) {
            selectedUnit->get_order(action);
        }
        emit actionRelayed(action);
        this->hideAll();
    });

    if (layout) {
        // On s'assure que le sélecteur est bien dans le layout (au cas où il aurait été caché)
        layout->addWidget(_unit_selector, 0, 0, Qt::AlignTop | Qt::AlignLeft);
        layout->addWidget(_stats_widget, 0, 0, Qt::AlignRight);
        layout->addWidget(_actions_widget, 0, 0, Qt::AlignBottom);
        
        // On affiche TOUT
        showAll();

        // REPOSITIONNEMENT CRUCIAL DU BOUTON
        // On le déplace par rapport à la taille actuelle du parent au cas où elle a changé
        _close_btn->move(_parent_ref->width() - 40, 10);
        
        // On remonte tout au premier plan, surtout le bouton et le sélecteur
        _unit_selector->raise();
        _close_btn->raise(); 
    }
    
    _parent_ref->update();
}

void UnitControlPanel::hideAll() {
    if (_unit_selector) _unit_selector->hide();
    if (_stats_widget) _stats_widget->hide();
    if (_actions_widget) _actions_widget->hide();
    if (_close_btn) _close_btn->hide();
    isHidden = true;
}

void UnitControlPanel::showAll(){
    _unit_selector->show();
    _stats_widget->show();
    _actions_widget->show();
    _close_btn->show();
    isHidden = false;
}

Unit* UnitControlPanel::getSelectedUnit() const {
    if (_units.empty()) return nullptr;
    return _units[_unit_selector->currentIndex()];
}