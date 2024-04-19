/**
 * @file communicationbluetooth.cpp
 *
 * @brief Définition de la classe CommunicationBluetooth
 * @author Thomas HNIZDO
 * @version 0.2
 */
#include "communicationbluetooth.h"
#include <QDebug>
#include <unistd.h>

/**
 * @brief Constructeur de la classe CommunicationBluetooth
 */
CommunicationBluetooth::CommunicationBluetooth(QObject* parent) :
    QObject(parent), serveurBluetooth(nullptr), socketTablette(nullptr)
{
    qDebug() << Q_FUNC_INFO;

    initialiserFormatTrame();

    verifierBluetooth();
}

/**
 * @brief Destructeur de la classe Participant
 */
CommunicationBluetooth::~CommunicationBluetooth()
{
    qDebug() << Q_FUNC_INFO;
}

void CommunicationBluetooth::verifierBluetooth()
{
    if(appareilLocal.isValid())
    {
        qDebug() << Q_FUNC_INFO << "Bluetooth disponible";
        activerBluetooth();
        lireNomAppareil();
        rendreAppareilVisible();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Bluetooth n'est pas disponible.";
    }
}

void CommunicationBluetooth::activerBluetooth()
{
    appareilLocal.powerOn();
    qDebug() << Q_FUNC_INFO << "Bluetooth activé.";
}

void CommunicationBluetooth::lireNomAppareil()
{
    nomAppareilLocal     = appareilLocal.name();
    adresseAppareilLocal = appareilLocal.address();
    QString adresseMAC   = appareilLocal.address().toString();
    qDebug() << Q_FUNC_INFO << "nomAppareilLocal" << nomAppareilLocal
             << "adresseAppareilLocal" << adresseAppareilLocal.toString();
}

void CommunicationBluetooth::rendreAppareilVisible()
{
    appareilLocal.setHostMode(QBluetoothLocalDevice::HostDiscoverable);
    qDebug() << Q_FUNC_INFO;
}

void CommunicationBluetooth::demarrerServeur()
{
    if(!appareilLocal.isValid())
        return;

    if(serveurBluetooth == nullptr)
    {
        qDebug() << Q_FUNC_INFO;
        serveurBluetooth =
          new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);

        connect(serveurBluetooth,
                SIGNAL(newConnection()),
                this,
                SLOT(connecterTablette()));

        QBluetoothUuid uuid(QBluetoothUuid::Rfcomm);
        serviceInfo = serveurBluetooth->listen(uuid, serviceNom);
    }
}

void CommunicationBluetooth::arreterServeur()
{
    if(!appareilLocal.isValid() || serveurBluetooth == nullptr)
        return;

    // serviceInfo.unregisterService();

    if(socketTablette == nullptr)
        return;

    socketTablette->close();
    delete socketTablette;
    socketTablette = nullptr;
    delete serveurBluetooth;
    serveurBluetooth = nullptr;
    qDebug() << Q_FUNC_INFO;
}

void CommunicationBluetooth::connecterTablette()
{
    qDebug() << Q_FUNC_INFO;
    socketTablette = serveurBluetooth->nextPendingConnection();

    if(!socketTablette)
        return;
    connect(socketTablette,
            SIGNAL(disconnected()),
            this,
            SLOT(deconnecterTablette()));
    connect(socketTablette, SIGNAL(readyRead()), this, SLOT(recevoirTrame()));

    emit tabletteConnectee();
}

void CommunicationBluetooth::deconnecterTablette()
{
    qDebug() << Q_FUNC_INFO;
    emit tabletteDeconnectee();
}

void CommunicationBluetooth::recevoirTrame()
{
    QByteArray donneesRecues;

    donneesRecues        = socketTablette->readAll();
    const QString& trame = QString(donneesRecues);
    qDebug() << Q_FUNC_INFO << "trame" << trame;

    if(verifierTrame(trame))
    {
        // @todo Si la trame est valide et complète, alors procéder à son
        // décodage puis émettre les données extraites de la trame dans un
        // signal. Chaque signal est associé à un type de trame. Ces signaux
        // doivent être connectés dans l'objet ihmQuizzy pas dans cette classe.
        // Les slots peuvent être des méthodes de l'objet ihmQuizzy ou de
        // l'objet quizzy
    }
}

// Méthodes privées

bool CommunicationBluetooth::verifierTrame(const QString& trame)
{
    QRegExp regexTrame("^\\$(.*;.*|[^;]*)\n$");
    if(regexTrame.exactMatch(trame))
    {
        qDebug() << Q_FUNC_INFO << "Trame valide";

        // Vérifier le respect du protocole
        return verifierChampsTrame(trame);
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Trame non valide.";
        return false;
    }
}

bool CommunicationBluetooth::verifierChampsTrame(QString trame)
{
    trame.remove(0, 1);                // supprime le $
    trame.remove(trame.size() - 1, 1); // supprime le \n

    qDebug() << Q_FUNC_INFO << "Nb séparateurs"
             << trame.count(SEPARATEUR_DE_CHAMPS);
    QStringList champs         = trame.split(SEPARATEUR_DE_CHAMPS);
    int         nombreDeChamps = champs.size();
    qDebug() << Q_FUNC_INFO << "nombreDeChamps" << nombreDeChamps;
    qDebug() << Q_FUNC_INFO << "champs" << champs;

    // vérification du nombre de champs de la trame
    if(formatTrame.contains(trame.at(0)) &&
       formatTrame.value(trame.at(0)) == nombreDeChamps)
    {
        qDebug() << Q_FUNC_INFO << "Trame complète";
        return true;
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Trame incomplète";
        return false;
    }

    return true;
}

void CommunicationBluetooth::initialiserFormatTrame()
{
    formatTrame.insert('L', 0); // Lancer un quiz : $L\n
    formatTrame.insert(
      'I',
      3); // Indiquer un participant au quiz : $I;PID;NOM DU JOUEUR\n
    formatTrame.insert('G',
                       8); // Envoyer une question :
                           // $G;LIBELLE;R1;R2;R3;R4;NUMERO_REP_VALIDE;TEMPS\n
    formatTrame.insert('T',
                       1); // Signaler le démarrage (top) d’une question : $T\n
    formatTrame.insert('U', 4); // Indiquer la réponse choisie par un joueur :
                                // $U;PID_JOUEUR;NUMÉRO_REPONSE;TEMPS_REPONSE\n
    formatTrame.insert('H', 1); // Afficher la réponse : $H\n
    formatTrame.insert('S', 1); // Passer à la question suivante : $S\n
    formatTrame.insert('P', 1); // Revenir à la question précédente : $P\n
    formatTrame.insert('F', 1); // Finir un quiz : $F\n
}
