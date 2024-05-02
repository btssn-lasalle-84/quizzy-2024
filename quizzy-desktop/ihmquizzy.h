#ifndef IHMQUIZZY_H
#define IHMQUIZZY_H

/**
 * @file ihmquizzy.h
 *
 * @brief Déclaration de la classe IHMQuizzy
 * @author Thomas HNIZDO
 * @version 0.1
 */

#include <QtWidgets>
#include <QVector>
#include <QTimer>

/**
 * @def NOM_APP
 * @brief Le nom de l'application
 */
#define NOM_APP "Quizzy"

/**
 * @def VERSION_APP
 * @brief La version de l'application
 */
#define VERSION_APP "0.1"

/**
 * @def TAILLE_LARGEUR_ECRAN_MIN
 * @brief Pour le mode plein écran sur la Raspberry Pi
 */
#define TAILLE_LARGEUR_ECRAN_MIN 1920

/**
 * @def TAILLE_HAUTEUR_ECRAN_MIN
 * @brief Pour le mode plein écran sur la Raspberry Pi
 */
#define TAILLE_HAUTEUR_ECRAN_MIN 1024

/**
 * @def PLEIN_ECRAN_RASPBERRY_PI
 * @brief Pour le mode plein écran sur la Raspberry Pi
 */
//#define PLEIN_ECRAN_RASPBERRY_PI

/**
 * @def TOP_SECONDE
 * @brief Pour la gestion du chronomètre en millisecondes
 */
#define TOP_SECONDE 1000

/**
 * @def ECHEANCE_CHRONOMETRE
 * @brief Pour la fin du chronomètre en secondes
 */
#define ECHEANCE_CHRONOMETRE 3

#define FOND_VERT  "#94fe8a"
#define FOND_ROUGE "#fd5555"

class Quizzy;
class Question;

/**
 * @class IHMQuizzy
 * @brief Déclaration de la classe IHMQuizzy
 * @details Cette classe gère l'affichage du Quiz sur l'écran de la Raspberry Pi
 */
class IHMQuizzy : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @enum Fenetre
     * @brief Définit les différentes fenêtres de la GUI
     *
     */
    enum Fenetre
    {
        FenetreAccueil = 0,
        FenetreParticipants,
        FenetreJeu,
        FenetreResultats,
        NbEcrans
    };

  private:
    Quizzy*                quizzy;    //!< association vers la classe Quizzy
    QVector<Question*>     questions; //!< conteneur de Question pour le quiz
    QTimer*                minuteur;  //!< pour gérer le temps d'une question
    int                    decompteQuestion; //!< pour le temps d'une question
    QMap<int, QStringList> choixParticipants;
    // Les ressources de la GUI
    QStackedWidget* fenetres;
    // FenetreAccueil
    QWidget* fenetreAccueil;
    QLabel*  titreFenetreAccueil;
    QLabel*  messageAttente;
    // FenetreParticipants
    QWidget*     fenetreParticipants;
    QLabel*      titreFenetreParticipants;
    QVBoxLayout* layoutPrincipalParticipants;
    QHBoxLayout* layoutInfoQuiz;
    QLabel*      infoQuiz;
    // FenetreJeu
    QWidget*     fenetreJeu;
    QLabel*      titreFenetreJeu;
    QVBoxLayout* layoutPrincipalJeu;
    QHBoxLayout* layoutLibelle;
    QVBoxLayout* layoutPropositionReponse;
    QHBoxLayout* layoutPropositonAB;
    QHBoxLayout* layoutPropositonCD;
    QHBoxLayout* layoutChronometre;
    QLabel*      labelNombreTotal;
    QLabel*      labelQuestion;
    QLabel*      propositionReponseA;
    QLabel*      propositionReponseB;
    QLabel*      propositionReponseC;
    QLabel*      propositionReponseD;
    QLabel*      labelChronometre;
    QTimer*      timer;
    // FenetreResultats
    QWidget* fenetreResultats;
    QLabel*  titreFenetreResultats;

    void initialiserFenetres();
    void creerFenetres();
    void creerFenetreAccueil();
    void creerFenetreParticipants();
    void creerLayoutsFenetreParticipants();
    void creerWidgetsFenetreParticipants();
    void placerWidgetsFenetreParticipants();
    void creerFenetreJeu();
    void creerLayoutsFenetreJeu();
    void creerWidgetsFenetreJeu();
    void placerWidgetsFenetreJeu();
    void creerFenetreResultats();
    void initialiserEvenements();
    void afficherParticipant(QString pidJoueur, QString participant);
    void afficherQuestion();
    void afficherNbQuestions(unsigned int numeroQuestion,
                             unsigned int nbQuestions);
    void afficherLibelleQuestion(const Question& question);
    void afficherPropositionsQuestion(const Question& question);
    void afficherTempsQuestion(const Question& question);
    void initialiserChronometre();
    void changerCouleurChronometre();
    void effacerChoix();
    void mettreAJourChoix(QString pidJoueur,
                          int     numeroReponse,
                          int     tempsReponse);
    void afficherChoixParticipants();
    void mettreAJourProposition(int numeroReponse, QString texte);

  public:
    IHMQuizzy(QWidget* parent = 0);
    ~IHMQuizzy();

  public slots:
    void afficherFenetre(IHMQuizzy::Fenetre fenetre);
    void afficherFenetreAccueil();
    void afficherFenetreParticipants();
    void afficherFenetreJeu();
    void afficherFenetreResultats();
    void lancerQuiz();
    void ajouterParticipant(QString pidJoueur, QString participant);

    void ajouterNouvelleQuestion(QString     libelle,
                                 QStringList propositions,
                                 int         reponseValide,
                                 int         temps);
    void demarrerQuestion();
    void afficherDecompteQuestion();
    void traiterChoixReponse(QString pidJoueur,
                             int     numeroReponse,
                             int     tempsReponse);
    void afficherChoixReponse();
};

#endif // IHMQUIZZY_H
