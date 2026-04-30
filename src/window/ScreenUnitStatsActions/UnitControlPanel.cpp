#include "UnitControlPanel.h"
#include "UnitStatsWidget.h"
#include "UnitActionsWidget.h"
#include <QVBoxLayout>

/**
 * @brief Constructeur du gestionnaire d'interface de contrôle.
 * Il crée et positionne la fenêtre de statistiques et la barre d'actions
 * par-dessus le widget parent (généralement le conteneur de jeu).
 */
UnitControlPanel::UnitControlPanel(QWidget* parent, const Unit* unit, const Case* currentCase) 
    : QObject(parent) {
    
    // 1. Création de la fenêtre de statistiques (Colonne à droite)
    // On la crée avec 'parent' comme parent pour qu'elle soit "flottante" par-dessus le jeu
    _stats_widget = new UnitStatsWidget(parent, unit);
    _stats_widget->setFixedWidth(250);
    
    // 2. Création de la barre d'actions (Bandeau en bas)
    _actions_widget = new UnitActionsWidget(parent, unit, currentCase);
    _actions_widget->setFixedHeight(125);

    // Note : Pour que ces widgets se placent correctement (droite et bas),
    // le 'parent' (MainWindow/gameContainer) doit idéalement utiliser 
    // un QGridLayout où ces widgets sont ajoutés avec les flags d'alignement.
    
    // Si le parent utilise un QGridLayout (recommandé) :
    if (parent->layout()) {
        QGridLayout* layout = qobject_cast<QGridLayout*>(parent->layout());
        if (layout) {
            // Stats : en haut à droite, s'étend sur toute la hauteur disponible
            layout->addWidget(_stats_widget, 0, 0, Qt::AlignRight);
            
            // Actions : en bas, s'étend sur toute la largeur
            layout->addWidget(_actions_widget, 0, 0, Qt::AlignBottom);
            
            // On s'assure qu'ils sont au premier plan
            _stats_widget->raise();
            _actions_widget->raise();
        }
    }
}