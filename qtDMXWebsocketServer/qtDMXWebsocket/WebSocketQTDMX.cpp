#include "WebSocketQTDMX.h"
#include <QDebug>

WebSocketQTDMX::WebSocketQTDMX(const QHostAddress& address, quint16 port, QObject* parent)
    : QWebSocketServer(QStringLiteral("WebSocket Server"), QWebSocketServer::NonSecureMode, parent)
{
    piloteLum = nullptr;

    if (this->listen(address, port))
    {
        qDebug() << "Server listening on adress" << address;
        qDebug() << "Server listening on port" << port;

        connect(this, &WebSocketQTDMX::newConnection, this, &WebSocketQTDMX::onServerNewConnection); // on conecte signal et slots

        //On créer l'objet pour gérer les lumières
        piloteLum = new PilotageLumiere(NULL, NULL, NULL, NULL, NULL);
    }
    else
    {
        qCritical() << "Failed to listen on port" << port;
    }
}

WebSocketQTDMX::~WebSocketQTDMX()
{
    this->close();
}

void WebSocketQTDMX::onServerNewConnection()
{
    if (sender() == this) // Si c'est un objet webSocket (nous)
    {
        qDebug() << "Un client Web viens de se connecter ! ";

        // [WebSocket]
        QWebSocket* clientWeb = this->nextPendingConnection();

        // [WebSocket]
        QObject::connect(clientWeb, &QWebSocket::textMessageReceived, this, &WebSocketQTDMX::onWebClientReadyRead);
        QObject::connect(clientWeb, &QWebSocket::disconnected, this, &WebSocketQTDMX::onClientDisconnected);

        clientWeb->sendTextMessage("Connecté !");
    }
}

void WebSocketQTDMX::onClientDisconnected()
{
    if (sender() == this) // Si c'est un objet webSocket (nous)
    {
        // [WebSocket]
        QWebSocket* objWeb = qobject_cast<QWebSocket*>(sender());

        // [WebSocket]
        QObject::disconnect(objWeb, &QWebSocket::textMessageReceived, this, &WebSocketQTDMX::onWebClientReadyRead);
        QObject::disconnect(objWeb, &QWebSocket::disconnected, this, &WebSocketQTDMX::onClientDisconnected);

        // [WebSocket]
        objWeb->sendTextMessage("Déconnecté !");
        objWeb->deleteLater();
    }
}

void WebSocketQTDMX::onWebClientReadyRead(const QString& message)
{
    QWebSocket* objWeb = qobject_cast<QWebSocket*>(sender());

    //qDebug() << "Status de connexion : Message client web = " << message;
    //onSendMessageButtonClicked(nullptr, objWeb, message); // Envoie du message au Client Web

    QJsonObject jsonMessage = QJsonDocument::fromJson(message.toUtf8()).object(); // On décode en objet JSON 
    qDebug() << jsonMessage;

    if (objWeb != nullptr) // Si c'est un objet Websocket
    {
        if (objWeb->state() == QAbstractSocket::ConnectedState)
        {
            // On va faire quelque chose selon l'entete de cette réponse
            if (jsonMessage["type"].toString() == "trameDMX512") // Si c'est une authentification
            {
                if (piloteLum->isInterfaceOpen() > 0)
                {
                    // On recupere l'adresse de la lumiere
                    piloteLum->setAdressValue(jsonMessage["adressLum"].toInt());

                    // Ensuite les couleurs
                    piloteLum->setRedValue(jsonMessage["redValue"].toInt());
                    piloteLum->setGreenValue(jsonMessage["greenValue"].toInt());
                    piloteLum->setBlueValue(jsonMessage["blueValue"].toInt());
                    piloteLum->setWhiteValue(jsonMessage["whiteValue"].toInt());

                    // Et enfin le checbox pour savoir si on fait un changement automatique de couleur
                    if(jsonMessage["changeAutoLum"].toBool() == true)
                    {
                        // Voici un exemple de comment est construite la trame ainsi que comment elle est construite
                        // exemple si la lumiere est sur le canal 12, alors l'octet dmxBlock[12] contient
                        // un ensemble de 8 bits representant les informations à envoyer à la lumiere : intensite couleur ect
                        // Attention une lumiere peut avoir plusieur cannaux
                        // exemple une led 3 couleurs aura 3 cannaux pour rouge vert bleu
                        // donc la deuxieme lampe devra etre configure sur un canal 4

                        int time = QTime::currentTime().msecsSinceStartOfDay();  // Obtient le temps actuel en millisecondes
                        piloteLum->setRedValue((time / 10) % 256);    // Dégradé rouge
                        piloteLum->setGreenValue((time / 15) % 256);  // Dégradé vert
                        piloteLum->setBlueValue((time / 20) % 256);   // Dégradé bleu
                        piloteLum->setWhiteValue((time / 25) % 256);   // Dégradé bleu
                    }

                    piloteLum->sendTrame(); // On ennvoie la trame
                }
            }
        }
    }
}