#include "ihmquizzy.h"
#include "quizzy.h"
#include <QDebug>

/**
 * @file ihmquizzy.cpp
 *
 * @brief Définition de la classe IHMQuizzy
 * @author Thomas HNIZDO
 * @version 0.1
 */

/**
 * @brief Constructeur de la classe IHMQuizzy
 *
 * @fn IHMQuizzy::IHMQuizzy
 * @param parent L'adresse de l'objet parent, si nullptr IHMQuizzy sera la
 * fenêtre principale de l'application
 */
IHMQuizzy::IHMQuizzy(QWidget* parent) :
    QWidget(parent), quizzy(new Quizzy(this))
{
    qDebug() << Q_FUNC_INFO;

    creerFenetres();
    afficherFenetreAccueil();
#ifdef PLEIN_ECRAN_RASPBERRY_PI showFullScreen();
// showMaximized();
#endif
}

IHMQuizzy::~IHMQuizzy()
{
    qDebug() << Q_FUNC_INFO;
}

void IHMQuizzy::afficherFenetre(IHMQuizzy::Fenetre fenetre)
{
    qDebug() << Q_FUNC_INFO << "fenetre" << fenetre;
    fenetres->setCurrentIndex(fenetre);
}

void IHMQuizzy::afficherFenetreAccueil()
{
    afficherFenetre(Fenetre::FenetreAccueil);
}

void IHMQuizzy::afficherFenetreParticipants()
{
    afficherFenetre(Fenetre::FenetreParticipants);
}

void IHMQuizzy::afficherFenetreJeu()
{
    afficherFenetre(Fenetre::FenetreJeu);
}

void IHMQuizzy::afficherFenetreResultats()
{
    afficherFenetre(Fenetre::FenetreResultats);
}

void IHMQuizzy::creerFenetres()
{
    fenetres = new QStackedWidget(this);
    creerFenetreAccueil();
    creerFenetreParticipants();
    creerFenetreJeu();
    creerFenetreResultats();

    initialiserFenetres();
}

void IHMQuizzy::creerFenetreAccueil()
{
    fenetreAccueil             = new QWidget(this);
    QVBoxLayout* layoutAccueil = new QVBoxLayout(fenetreAccueil);
    titreFenetreAccueil        = new QLabel("QUIZZY", this);
    messageAttente = new QLabel("En attente des participants...", this);

    titreFenetreAccueil->setObjectName("titreAccueil");
    messageAttente->setObjectName("messageAttente");

    titreFenetreAccueil->setAlignment(Qt::AlignCenter);
    messageAttente->setAlignment(Qt::AlignCenter);

    layoutAccueil->addWidget(titreFenetreAccueil);
    layoutAccueil->addWidget(messageAttente);

    fenetres->addWidget(fenetreAccueil);
}

void IHMQuizzy::creerFenetreParticipants()
{
    fenetreParticipants          = new QWidget(this);
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(fenetreParticipants);
    titreFenetreParticipants     = new QLabel("Liste des participants", this);

    titreFenetreParticipants->setAlignment(Qt::AlignCenter);
    titreFenetreParticipants->setObjectName("titreParticipants");
    layoutPrincipal->addWidget(titreFenetreParticipants);

    creerListeParticipants(layoutPrincipal);

    fenetres->addWidget(fenetreParticipants);
}

/**
 * @todo Créer un classe spécifique pour les participants
 */
void IHMQuizzy::creerListeParticipants(QVBoxLayout* layoutPrincipal)
{
    QStringList listeParticipants;
    listeParticipants << "Participant 1"
                      << "Participant 2";
    for(const QString& participant: listeParticipants)
    {
        QWidget*     widgetParticipant = new QWidget(this);
        QVBoxLayout* layoutParticipant = new QVBoxLayout(widgetParticipant);
        QLabel*      labelParticipant  = new QLabel(participant, this);

        layoutParticipant->setContentsMargins(100, 10, 100, 10);
        layoutParticipant->addWidget(labelParticipant);
        layoutPrincipal->addWidget(widgetParticipant);
    }
}

/**
 * @todo Faire en sorte que Qmap et Qstring soit dans la classe Question
 */

void IHMQuizzy::creerLayouts()
{
    layoutPrincipal          = new QVBoxLayout(fenetreJeu);
    layoutLibelle            = new QHBoxLayout();
    layoutPropositionReponse = new QVBoxLayout();
    layoutPropositonAB       = new QHBoxLayout();
    layoutPropositonCD       = new QHBoxLayout();
    layoutChronometre        = new QHBoxLayout();
}

void IHMQuizzy::creerLabels()
{
    labelNombreTotal    = new QLabel("0/0", this);
    labelQuestion       = new QLabel("Question : ", this);
    propositionReponseA = new QLabel("Proposition A", this);
    propositionReponseB = new QLabel("Proposition B", this);
    propositionReponseC = new QLabel("Proposition C", this);
    propositionReponseD = new QLabel("Proposition D", this);
    labelChronometre    = new QLabel("00:00", this);
}

void IHMQuizzy::creerFenetreJeu()
{
    fenetreJeu = new QWidget(this);
    creerLayouts();
    creerLabels();
    layoutLibelle->addWidget(labelNombreTotal);
    layoutLibelle->addWidget(labelQuestion);
    layoutPrincipal->addLayout(layoutLibelle);
    layoutPropositonAB->addWidget(propositionReponseA);
    layoutPropositonAB->addWidget(propositionReponseB);
    layoutPropositonCD->addWidget(propositionReponseC);
    layoutPropositonCD->addWidget(propositionReponseD);
    layoutPropositionReponse->addLayout(layoutPropositonAB);
    layoutPropositionReponse->addLayout(layoutPropositonCD);
    layoutPrincipal->addLayout(layoutPropositionReponse);
    layoutChronometre->addWidget(labelChronometre);
    layoutPrincipal->addLayout(layoutChronometre);

    fenetres->addWidget(fenetreJeu);
}

void IHMQuizzy::creerFenetreResultats()
{
    fenetreResultats             = new QWidget(this);
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(fenetreResultats);
    titreFenetreResultats        = new QLabel("Résultats", this);
    layoutPrincipal->addWidget(titreFenetreResultats);

    fenetres->addWidget(fenetreResultats);
}

void IHMQuizzy::initialiserFenetres()
{
    QVBoxLayout* layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->setSpacing(0);
    layoutPrincipal->setContentsMargins(0, 0, 0, 0);
    layoutPrincipal->addWidget(fenetres);
    setLayout(layoutPrincipal);
    setMinimumSize(QSize(TAILLE_LARGEUR_ECRAN_MIN, TAILLE_HAUTEUR_ECRAN_MIN));
    setWindowTitle(QString(NOM_APP) + QString(" v") + QString(VERSION_APP));
}
