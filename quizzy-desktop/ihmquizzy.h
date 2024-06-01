#ifndef IHMQUIZZY_H
#define IHMQUIZZY_H

/**
 * @file ihmquizzy.h
 *
 * @brief Déclaration de la classe IHMQuizzy
 * @author Thomas HNIZDO
 * @version 0.2
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
#define VERSION_APP "0.2"

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
#define PLEIN_ECRAN_RASPBERRY_PI

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
/**
 * @def FOND_VERT
 * @brief Pour la couleur restante du chronomètre
 */

#define FOND_VERT "#94fe8a"
/**
 * @def FOND_ROUGE
 * @brief Pour la couleur restante du chronomètre
 */
#define FOND_ROUGE "#fd5555"

/**
 * @def HAUTEUR_LABEL_LIBELLE
 * @brief Pour la hauteur des labels libelle
 */
#define HAUTEUR_LABEL_LIBELLE 100

/**
 * @def HAUTEUR_LABEL_CHRONOMETRE
 * @brief Pour la hauteur du label chronomètre
 */
#define HAUTEUR_LABEL_CHRONOMETRE 100

/**
 * @def LARGEUR_PROPOSITION
 * @brief Pour la largeur des propositions
 */
#define LARGEUR_PROPOSITION 150

/**
 * @def HAUTEUR_PROPOSITION
 * @brief Pour la hauteur des propositions
 */
#define HAUTEUR_PROPOSITION 150

/**
 * @def MARGE_LAYOUT_PROPOSITION
 * @brief Pour la marge du layout proposition
 */
#define MARGE_LAYOUT_PROPOSITION 100

/**
 * @def CHEMIN_LOGO
 * @brief Le chemin du logo
 */
#define CHEMIN_LOGO ":/image/logo.png"

/**
 * @def LARGEUR_LOGO
 * @brief Pour la largeur du logo
 */
#define LARGEUR_LOGO 500

/**
 * @def HAUTEUR_LOGO
 * @brief Pour la hauteur du logo
 */
#define HAUTEUR_LOGO 500

class Quizzy;
class Question;
class Participant;

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
    Quizzy* quizzy;           //!< association vers la classe Quizzy
    QTimer* minuteur;         //!< pour gérer le temps d'une question
    int     decompteQuestion; //!< pour le temps d'une question
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
    QVBoxLayout* layoutParticipant;
    QHBoxLayout* layoutInfoQuiz;
    QLabel*      labelParticipant;
    QLabel*      infoQuiz;
    // FenetreJeu
    QWidget*     fenetreJeu;
    QLabel*      titreFenetreJeu;
    QVBoxLayout* layoutPrincipalJeu;
    QHBoxLayout* layoutLibelle;
    QVBoxLayout* layoutPropositionReponse;
    QHBoxLayout* layoutPropositonAB;
    QHBoxLayout* layoutPropositonCD;
    QHBoxLayout* layoutPropositionChoixA;
    QHBoxLayout* layoutPropositionChoixB;
    QHBoxLayout* layoutPropositionChoixC;
    QHBoxLayout* layoutPropositionChoixD;
    QHBoxLayout* layoutChronometre;
    QLabel*      labelNombreTotal;
    QLabel*      labelQuestion;
    QLabel*      propositionReponseA;
    QLabel*      propositionReponseB;
    QLabel*      propositionReponseC;
    QLabel*      propositionReponseD;
    QLabel*      idPropositionReponseA;
    QLabel*      idPropositionReponseB;
    QLabel*      idPropositionReponseC;
    QLabel*      idPropositionReponseD;
    QLabel*      choixPropositionA;
    QLabel*      choixPropositionB;
    QLabel*      choixPropositionC;
    QLabel*      choixPropositionD;
    QLabel*      labelChronometre;
    QTimer*      timer;
    // FenetreResultats
    QWidget*     fenetreResultats;
    QVBoxLayout* layoutPrincipalResultat;
    QHBoxLayout* layoutParticipantResultat;
    QLabel*      titreFenetreResultats;
    QLabel*      nomParticipant;
    QLabel*      resultatParticipant;

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
    void configurerResponsiveLabels();
    void definirNomsObjets();
    void placerWidgetsFenetreJeu();
    void creerFenetreResultats();
    void initialiserEvenements();
    void afficherQuestion();
    void afficherNbQuestions(unsigned int numeroQuestion,
                             unsigned int nbQuestions);
    void afficherLibelleQuestion(const Question& question);
    void afficherPropositionsQuestion(const Question& question);
    void afficherTempsQuestion(const Question& question);
    void initialiserChronometre();
    void changerCouleurChronometre();
    void mettreAJourProposition(int numeroReponse, QString texte);
    void effacerChoixParticipants();
    void reinitialiserAffichage();
    void effacerFenetreParticipants();
    void effacerFenetreResultats();
    void afficherNumerosQuestionsCorrectes(Participant* participant);
    void afficherNombreBonnesReponses(Participant* participant,
                                      unsigned int nbQuestions);

  public:
    IHMQuizzy(QWidget* parent = 0);
    ~IHMQuizzy();

  public slots:
    void afficherFenetre(IHMQuizzy::Fenetre fenetre);
    void afficherFenetreAccueil();
    void afficherFenetreParticipants();
    void afficherFenetreJeu();
    void afficherFenetreResultats();
    void afficherDebutQuiz();
    void afficherLancementQuiz();
    void afficherPret();
    void afficherParticipant(QString pidJoueur, QString nomParticipant);
    void demarrerQuestion();
    void afficherDecompteQuestion();
    void afficherChoixParticipants();
    void afficherQuestionSuivante();
    void afficherResultats();
};

#endif // IHMQUIZZY_H
