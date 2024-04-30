#include "ihmquizzy.h"
#include "quizzy.h"
#include "question.h"
#include "participant.h"
#include "communicationbluetooth.h"
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
    QWidget(parent), quizzy(new Quizzy(this)), minuteur(new QTimer(this))
{
    qDebug() << Q_FUNC_INFO;

    creerFenetres();
    afficherFenetreAccueil();
    initialiserEvenements();

#ifdef PLEIN_ECRAN_RASPBERRY_PI
    showFullScreen();
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

void IHMQuizzy::lancerQuiz()
{
    if(quizzy->getNbQuestions() > 0)
    {
        afficherQuestion();
    }
}

void IHMQuizzy::ajouterParticipant(QString pidJoueur, QString participant)
{
    if(quizzy->ajouterParticipant(pidJoueur, participant))
    {
        afficherParticipant(pidJoueur, participant);
        afficherFenetreParticipants();
    }
}

void IHMQuizzy::ajouterNouvelleQuestion(QString     libelle,
                                        QStringList propositions,
                                        int         reponseValide,
                                        int         temps)
{
    if(quizzy->getNbParticipants() > 0)
    {
        quizzy->ajouterQuestion(libelle, propositions, reponseValide, temps);
        // @todo Afficher prêt à lancer le quiz
    }
}

void IHMQuizzy::demarrerQuestion()
{
    if(quizzy->getEtat() == Quizzy::Etat::QuizLance &&
       quizzy->getQuestion() != nullptr)
    {
        initialiserChronometre();
    }
}

void IHMQuizzy::afficherDecompteQuestion()
{
    if(fenetres->currentIndex() == Fenetre::FenetreJeu)
    {
        changerCouleurChronometre();
        labelChronometre->setText(QString::number(decompteQuestion) + "s");
        decompteQuestion--;
        if(decompteQuestion < 0)
        {
            minuteur->stop();
            labelChronometre->setText(QString::number(0) + "s");
            labelChronometre->setStyleSheet("background-color: #f9e4b7");
        }
    }
}

// Méthodes privées

void IHMQuizzy::initialiserFenetres()
{
    fenetres                     = new QStackedWidget(this);
    QVBoxLayout* layoutPrincipal = new QVBoxLayout();
    layoutPrincipal->setSpacing(0);
    layoutPrincipal->setContentsMargins(0, 0, 0, 0);
    layoutPrincipal->addWidget(fenetres);
    setLayout(layoutPrincipal);
    setMinimumSize(QSize(TAILLE_LARGEUR_ECRAN_MIN, TAILLE_HAUTEUR_ECRAN_MIN));
    setWindowTitle(QString(NOM_APP) + QString(" v") + QString(VERSION_APP));
}

void IHMQuizzy::creerFenetres()
{
    initialiserFenetres();
    creerFenetreAccueil();
    creerFenetreParticipants();
    creerFenetreJeu();
    creerFenetreResultats();
}

void IHMQuizzy::creerFenetreAccueil()
{
    fenetreAccueil             = new QWidget(this);
    QVBoxLayout* layoutAccueil = new QVBoxLayout(fenetreAccueil);
    titreFenetreAccueil        = new QLabel("QUIZZY", this);
    titreFenetreAccueil->setObjectName("titreAccueil");
    titreFenetreAccueil->setAlignment(Qt::AlignCenter);
    messageAttente = new QLabel("En attente des participants...", this);
    messageAttente->setObjectName("messageAttente");
    messageAttente->setAlignment(Qt::AlignCenter);
    layoutAccueil->addWidget(titreFenetreAccueil);
    layoutAccueil->addWidget(messageAttente);
    fenetres->addWidget(fenetreAccueil);
}

void IHMQuizzy::creerFenetreParticipants()
{
    fenetreParticipants = new QWidget(this);
    creerLayoutsFenetreParticipants();
    creerWidgetsFenetreParticipants();
    placerWidgetsFenetreParticipants();

    fenetres->addWidget(fenetreParticipants);
}

void IHMQuizzy::creerLayoutsFenetreParticipants()
{
    layoutPrincipalParticipants = new QVBoxLayout(fenetreParticipants);
}

void IHMQuizzy::creerWidgetsFenetreParticipants()
{
    titreFenetreParticipants = new QLabel("Liste des participants", this);
    titreFenetreParticipants->setAlignment(Qt::AlignCenter);
    titreFenetreParticipants->setObjectName("titreParticipants");
}

void IHMQuizzy::placerWidgetsFenetreParticipants()
{
    layoutPrincipalParticipants->addWidget(titreFenetreParticipants);
}

void IHMQuizzy::creerFenetreJeu()
{
    fenetreJeu = new QWidget(this);
    creerLayoutsFenetreJeu();
    creerWidgetsFenetreJeu();
    placerWidgetsFenetreJeu();
    fenetres->addWidget(fenetreJeu);
}

void IHMQuizzy::creerLayoutsFenetreJeu()
{
    layoutPrincipalJeu       = new QVBoxLayout(fenetreJeu);
    layoutLibelle            = new QHBoxLayout();
    layoutPropositionReponse = new QVBoxLayout();
    layoutPropositonAB       = new QHBoxLayout();
    layoutPropositonCD       = new QHBoxLayout();
    layoutChronometre        = new QHBoxLayout();
}

void IHMQuizzy::creerWidgetsFenetreJeu()
{
    labelNombreTotal    = new QLabel("0/0", this);
    labelQuestion       = new QLabel("", this);
    propositionReponseA = new QLabel("A", this);
    propositionReponseB = new QLabel("B", this);
    propositionReponseC = new QLabel("C", this);
    propositionReponseD = new QLabel("D", this);
    labelChronometre    = new QLabel("00:00", this);
}

void IHMQuizzy::placerWidgetsFenetreJeu()
{
    layoutLibelle->addWidget(labelNombreTotal);
    layoutLibelle->addWidget(labelQuestion);
    layoutPrincipalJeu->addLayout(layoutLibelle);
    layoutPropositonAB->addWidget(propositionReponseA);
    layoutPropositonAB->addWidget(propositionReponseB);
    layoutPropositonCD->addWidget(propositionReponseC);
    layoutPropositonCD->addWidget(propositionReponseD);
    layoutPropositionReponse->addLayout(layoutPropositonAB);
    layoutPropositionReponse->addLayout(layoutPropositonCD);
    layoutPrincipalJeu->addLayout(layoutPropositionReponse);
    layoutChronometre->addWidget(labelChronometre);
    layoutPrincipalJeu->addLayout(layoutChronometre);
}

void IHMQuizzy::creerFenetreResultats()
{
    fenetreResultats             = new QWidget(this);
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(fenetreResultats);
    titreFenetreResultats        = new QLabel("Résultats", this);
    layoutPrincipal->addWidget(titreFenetreResultats);

    fenetres->addWidget(fenetreResultats);
}

void IHMQuizzy::initialiserEvenements()
{
    connect(quizzy->getCommunicationTablette(),
            SIGNAL(debutQuiz()),
            quizzy,
            SLOT(gererDebutQuiz()));
    connect(quizzy,
            SIGNAL(affichagePremiereQuestion()),
            this,
            SLOT(lancerQuiz()));
    connect(quizzy->getCommunicationTablette(),
            SIGNAL(nouveauParticipant(QString, QString)),
            this,
            SLOT(ajouterParticipant(QString, QString)));
    connect(quizzy->getCommunicationTablette(),
            SIGNAL(nouvelleQuestion(QString, QStringList, int, int)),
            this,
            SLOT(ajouterNouvelleQuestion(QString, QStringList, int, int)));
    connect(quizzy->getCommunicationTablette(),
            SIGNAL(debutQuestion()),
            this,
            SLOT(demarrerQuestion()));
    connect(quizzy->getCommunicationTablette(),
            SIGNAL(choixReponse(QString, int, int)),
            this,
            SLOT(afficherChoixReponse(QString, int, int)));
    // @todo Faire la connexion signal/slot des signaux émis par l'objet
    // communicationTablette
}

void IHMQuizzy::afficherParticipant(QString pidJoueur, QString participant)
{
    QWidget*     widgetParticipant = new QWidget(this);
    QVBoxLayout* layoutParticipant = new QVBoxLayout(widgetParticipant);
    QLabel*      labelParticipant  = new QLabel(participant, this);
    layoutParticipant->setContentsMargins(100, 10, 100, 10);
    layoutParticipant->addWidget(labelParticipant);
    layoutPrincipalParticipants->addWidget(widgetParticipant);
}

void IHMQuizzy::afficherQuestion()
{
    Question* question = quizzy->getQuestion();
    afficherNbQuestions(quizzy->getIndexQuestionActuelle() + 1,
                        quizzy->getNbQuestions());
    afficherLibelleQuestion(*question);
    afficherPropositionsQuestion(*question);
    afficherTempsQuestion(*question);
    afficherFenetreJeu();
}

void IHMQuizzy::afficherNbQuestions(unsigned int numeroQuestion,
                                    unsigned int nbQuestions)
{
    qDebug() << Q_FUNC_INFO << "numeroQuestion" << numeroQuestion
             << "nbQuestions" << nbQuestions;
    labelNombreTotal->setText(QString("Question n°") +
                              QString::number(numeroQuestion));
}

void IHMQuizzy::afficherLibelleQuestion(const Question& question)
{
    labelQuestion->setText(question.getLibelle());
}

void IHMQuizzy::afficherPropositionsQuestion(const Question& question)
{
    QMap<char, QString> propositions = question.getPropositions();
    propositionReponseA->setStyleSheet("background-color: #f9b7b7"); // Rouge
    propositionReponseA->setText("A. " + propositions['A']);
    propositionReponseB->setStyleSheet("background-color: #b7f9ba"); // Vert
    propositionReponseB->setText("B. " + propositions['B']);
    propositionReponseC->setStyleSheet("background-color: #f6f476"); // Jaune
    propositionReponseC->setText("C. " + propositions['C']);
    propositionReponseD->setStyleSheet("background-color: #b7baf9"); // Bleu
    propositionReponseD->setText("D. " + propositions['D']);
}

void IHMQuizzy::afficherTempsQuestion(const Question& question)
{
    if(question.getDuree() > 0)
    {
        labelChronometre->setText(QString::number(question.getDuree()) + "s");
        labelChronometre->setVisible(true);
    }
    else
    {
        labelChronometre->setVisible(false);
    }
}

void IHMQuizzy::initialiserChronometre()
{
    decompteQuestion = quizzy->getQuestion()->getDuree();
    qDebug() << Q_FUNC_INFO << "decompteQuestion" << decompteQuestion;

    disconnect(minuteur,
               SIGNAL(timeout()),
               this,
               SLOT(afficherDecompteQuestion()));

    if(minuteur->isActive())
        minuteur->stop();

    if(decompteQuestion > 0)
    {
        connect(minuteur,
                SIGNAL(timeout()),
                this,
                SLOT(afficherDecompteQuestion()));

        minuteur->start(TOP_SECONDE);
    }
}

void IHMQuizzy::changerCouleurChronometre()
{
    QString couleur;
    if(decompteQuestion > ECHEANCE_CHRONOMETRE)
    {
        couleur = FOND_VERT;
    }
    else
    {
        couleur = FOND_ROUGE;
    }
    labelChronometre->setStyleSheet("background-color: " + couleur);
}

void IHMQuizzy::afficherChoixReponse(QString pidJoueur,
                                     int     numeroReponse,
                                     int     tempsReponse)
{
    if(!quizzy->estParticipantActuel(pidJoueur))
    {
        qDebug() << Q_FUNC_INFO << "pidJoueur non participant :" << pidJoueur;
        return;
    }
    QString nomParticipant = quizzy->getNomDuParticipant(pidJoueur);
    choixParticipants[numeroReponse].append(nomParticipant);
    QString texte = "<br><small>Choisi par : </small>" + nomParticipant;

    switch(numeroReponse)
    {
        case 1:
            propositionReponseA->setStyleSheet(
              "background-color: #f9b7b7; border: 3px solid red");
            propositionReponseA->setText(propositionReponseA->text() + texte);
            break;
        case 2:
            propositionReponseB->setStyleSheet(
              "background-color: #b7f9ba; border: 3px solid red");
            propositionReponseB->setText(propositionReponseB->text() + texte);
            break;
        case 3:
            propositionReponseC->setStyleSheet(
              "background-color: #f6f476; border: 3px solid red");
            propositionReponseC->setText(propositionReponseC->text() + texte);
            break;
        case 4:
            propositionReponseD->setStyleSheet(
              "background-color: #b7baf9; border: 3px solid red");
            propositionReponseD->setText(propositionReponseD->text() + texte);
            break;
        default:
            break;
    }

    qDebug() << Q_FUNC_INFO << "pidJoueur:" << pidJoueur
             << "choix:" << numeroReponse;

    quizzy->verifierReponse(pidJoueur, numeroReponse);
}
