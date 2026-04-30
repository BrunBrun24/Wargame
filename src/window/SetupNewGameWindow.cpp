
#include "SetupNewGameWindow.h"
#include "../type.h"

//penser a enlever : 
#include "../game/game.h" 

#include <QComboBox>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QGroupBox>
#include <QRadioButton>

const QMap<Country, QString> countryNames = {
    {Country::France, "France"},
    {Country::Germany, "Allemagne"},
    {Country::UnitedKingdom, "Royaume-Uni"},
    {Country::Russia, "Russie"},
    {Country::Egypt, "Égypte"},
    {Country::Switzerland, "Suisse"},
    {Country::Japan, "Japon"},
    {Country::Spain, "Espagne"},
    {Country::Italy, "Italie"},
    {Country::UnitedStates, "États-Unis"}
};
SetupNewGameWindow::SetupNewGameWindow(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);
    
    // --- Partie Haute ---
    QLabel* titre = new QLabel("Configuration de la partie");
    titre->setStyleSheet("font-weight: bold; font-size: 18px;");
    titre->setAlignment(Qt::AlignCenter);

    // On ajoute une Liste d'element en rapport avec le pays.
    selecteur = new QComboBox();
    for(int i = 2; i <= 10; ++i ) {
        selecteur->addItem(QString("%1 Pays").arg(i), i); // On stocke la valeur int dans le userData
    }

    // --- Partie Centrale Dynamique ---
    // On crée un conteneur et son layout pour les joueurs
    QWidget* conteneurJoueurs = new QWidget();
    layoutJoueurs = new QVBoxLayout(conteneurJoueurs);
    
    // Optionnel : Ajouter un QScrollArea si le nombre de pays est grand
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(conteneurJoueurs);

    // --- Partie Basse ---
    QPushButton* backButton = new QPushButton("Retour");
    LancerPartieButton = new QPushButton("Lancer la partie");
    LancerPartieButton->setEnabled(false);
    QGridLayout* grilleBouton = new QGridLayout(); 
    grilleBouton->addWidget(backButton,0,1);
    grilleBouton->addWidget(LancerPartieButton,0,0);
    

    // Assemblage
    layoutPrincipal->addWidget(titre);
    layoutPrincipal->addWidget(new QLabel("Sélectionnez le nombre de participants :"));
    layoutPrincipal->addWidget(selecteur);
    layoutPrincipal->addWidget(scrollArea); // On ajoute la zone dynamique
    layoutPrincipal->addLayout(grilleBouton);
    

    // Connexions
    connect(backButton, &QPushButton::clicked, this, &SetupNewGameWindow::backToMain);
    
    // Connexion du sélecteur : quand l'index change, on appelle mettreAJourJoueurs
    connect(selecteur, &QComboBox::currentIndexChanged, [this](int index) {
        this->mettreAJourJoueurs(index);
        LancerPartieButton->setEnabled(false);
    });
    connect(LancerPartieButton, &QPushButton::clicked, this, &SetupNewGameWindow::lancerLaPartie);
    // Initialisation du premier affichage (pour 2 pays)
    mettreAJourJoueurs(0);
}

void SetupNewGameWindow::verifierFormulaireValide() {
    bool toutEstOk = true;

    for (const auto& listeBoutons : listeBoutonsJoueurs) {
        bool joueurAChoisi = false;
        for (QRadioButton* rb : listeBoutons) {
            int countryVal = rb->property("countryEnum").toInt();
            // Si un bouton est coché ET que ce n'est pas "Aucun"
            if (rb->isChecked() && countryVal != static_cast<int>(Country::Neutral)) {
                joueurAChoisi = true;
                break;
            }
        }
        
        if (!joueurAChoisi) {
            toutEstOk = false;
            break;
        }
    }

    LancerPartieButton->setEnabled(toutEstOk);
}

void SetupNewGameWindow::gererExclusivitePays()
{
    // 1. On identifie quels pays sont actuellement choisis
    QSet<int> paysOccupes;
    for (const auto& listeBoutons : listeBoutonsJoueurs) {
        for (QRadioButton* rb : listeBoutons) {
            int countryVal = rb->property("countryEnum").toInt();
            // Si le bouton est coché et n'est pas "Neutre"
            if (rb->isChecked() && countryVal != static_cast<int>(Country::Neutral)) {
                paysOccupes.insert(countryVal);
            }
        }
    }

    // 2. On parcourt tous les boutons pour griser ceux qui sont pris par d'autres
    for (const auto& listeBoutons : listeBoutonsJoueurs) {
        for (QRadioButton* rb : listeBoutons) {
            int countryVal = rb->property("countryEnum").toInt();

            // On ne touche jamais au bouton Neutre (toujours actif)
            if (countryVal == static_cast<int>(Country::Neutral)) continue;

            // Si le pays est occupé ET que ce n'est pas CE bouton qui le sélectionne
            if (paysOccupes.contains(countryVal) && !rb->isChecked()) {
                rb->setEnabled(false);
            } else {
                rb->setEnabled(true); // Le pays est libre ou c'est notre choix actuel
            }
        }
    }
}

void SetupNewGameWindow::mettreAJourJoueurs(int index)
{
    // 1. Nettoyage du layout et de la liste de stockage 
    //Tant que l'item est pas nullptr
    //(en sachant que layoutJoueurs->takeAt(0) c'est l'equivalent de dire Tableau[0])
    QLayoutItem* item;
    while ((item = layoutJoueurs->takeAt(0)) != nullptr) {
        //si c'est un widget, alors delete l'objet quand tout les évènement sont fini.
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }
    listeBoutonsJoueurs.clear(); // clear de la liste

    // 2. Récupération du nombre de joueurs
    int nbJoueurs = selecteur->itemData(index).toInt();

    // 3. Création des groupes par joueur
    for (int i = 1; i <= nbJoueurs; ++i) {
        QGroupBox* groupJoueur = new QGroupBox(QString("Joueur %1").arg(i));
        QGridLayout* gridLayout = new QGridLayout(); 
        QList<QRadioButton*> boutonsDuJoueur; // Pour stocker les radios de CE joueur

        // --- AJOUT DU BOUTON NEUTRE ---
        // On le place sur la première ligne (row 0), il prend toute la largeur (3 colonnes)
        QRadioButton* radioNeutre = new QRadioButton("Aucun");
        radioNeutre->setProperty("countryEnum", static_cast<int>(Country::Neutral));
        radioNeutre->setChecked(true); // Sélectionné par défaut
        gridLayout->addWidget(radioNeutre, 0, 0, 1, 3); //il est situé en 0, 0, il prend une colonne et 3 largeur.
        boutonsDuJoueur.append(radioNeutre);
        
        // Connexion immédiate pour l'exclusivité
        connect(radioNeutre, &QRadioButton::clicked, this, &SetupNewGameWindow::gererExclusivitePays); 
         connect(radioNeutre, &QRadioButton::clicked, this, &SetupNewGameWindow::verifierFormulaireValide);
        //On connecte le radioneutre au Exclusivité Pays

        // --- AJOUT DES PAYS ---
        int column = 0;
        int row = 1; // On commence à la ligne 1 car la ligne 0 est pour le Neutre
        int maxColumns = 3;

        for (auto it = countryNames.begin(); it != countryNames.end(); ++it) {
            QRadioButton* radio = new QRadioButton(it.value());
            radio->setProperty("countryEnum", static_cast<int>(it.key()));

            gridLayout->addWidget(radio, row, column);
            boutonsDuJoueur.append(radio);

            // Connexion : quand on clique, on vérifie ce qui doit être grisé ailleurs
            connect(radio, &QRadioButton::clicked, this, &SetupNewGameWindow::gererExclusivitePays);
            // Dans la boucle de création des pays :
            connect(radio, &QRadioButton::clicked, this, &SetupNewGameWindow::verifierFormulaireValide);

            // Et aussi pour le bouton Neutre (pour qu'il re-grise le bouton Lancer) :
           
            if (++column >= maxColumns) {
                column = 0;
                row++;
            }
        }

        groupJoueur->setLayout(gridLayout);
        layoutJoueurs->addWidget(groupJoueur);
        
        // On ajoute la liste de ce joueur à notre liste globale
        listeBoutonsJoueurs.append(boutonsDuJoueur);
    }
    
    layoutJoueurs->addStretch();
}

void SetupNewGameWindow::lancerLaPartie()
{
    // 1. On prépare la Map comme tu l'as déjà fait
    QMap<int, Country> configurationMatch;

    for (int i = 0; i < listeBoutonsJoueurs.size(); ++i) {
        int numeroJoueur = i + 1;
        for (QRadioButton* rb : listeBoutonsJoueurs[i]) {
            if (rb->isChecked()) {
                Country pays = static_cast<Country>(rb->property("countryEnum").toInt());
                configurationMatch.insert(numeroJoueur, pays);
            }
        }
    }

    // 2. CREATION DIRECTE DU JEU
    qDebug() << "Création de l'objet Game en cours...";
    
    // On crée l'objet Game. 
    // Comme ton constructeur Game(QMap) appelle _map(PaysDesJoueurs), 
    // cela va créer la Map et les Players automatiquement.
    Game* maPartie = new Game(configurationMatch);

    // 3. Lancer la logique de jeu
    maPartie->start_game();

    qDebug() << "Le jeu a été initialisé avec succès !";
    
    emit gameCreated(maPartie);
    // Note : Pour l'instant, le jeu tourne "en mémoire". 
    // Si tu n'as pas encore de fenêtre d'affichage pour Game, 
    // tu verras juste les messages de game.cpp et map.cpp dans la console.
}
