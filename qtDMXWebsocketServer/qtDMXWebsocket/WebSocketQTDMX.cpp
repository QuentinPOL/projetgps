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

        //On cr�er l'objet pour g�rer les lumi�res
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

        clientWeb->sendTextMessage("TEST");
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
        objWeb->deleteLater();
    }
}

void WebSocketQTDMX::onWebClientReadyRead(const QString& message)
{
    QWebSocket* objWeb = qobject_cast<QWebSocket*>(sender());

    //qDebug() << "Status de connexion : Message client web = " << message;
    //onSendMessageButtonClicked(nullptr, objWeb, message); // Envoie du message au Client Web

    QJsonObject jsonMessage = QJsonDocument::fromJson(message.toUtf8()).object(); // On d�code en objet JSON 
    qDebug() << jsonMessage;

    if (objWeb != nullptr) // Si c'est un objet Websocket
    {
        if (objWeb->state() == QAbstractSocket::ConnectedState)
        {
            // On va faire quelque chose selon l'entete de cette r�ponse
            if (jsonMessage["type"].toString() == "trameDMX512") // Si c'est une authentification
            {
                //if (piloteLum->isInterfaceOpen() > 0)
                //{
                    // On recupere l'adresse de la lumiere
                int adressLum = jsonMessage["adressLum"].toInt();

                // Ensuite les couleurs
                int red = jsonMessage["redValue"].toInt();
                int green = jsonMessage["greenValue"].toInt();
                int blue = jsonMessage["blueValue"].toInt();
                int white = jsonMessage["whiteValue"].toInt();

                // Et enfin le checbox pour savoir si on fait un changement automatique de couleur
                bool changeAutoLum = jsonMessage["changeAutoLum"].toBool();

                // Voici un exemple de comment est construite la trame ainsi que comment elle est construite
                // exemple si la lumiere est sur le canal 12, alors l'octet dmxBlock[12] contient
                // un ensemble de 8 bits representant les informations � envoyer � la lumiere : intensite couleur ect
                // Attention une lumiere peut avoir plusieur cannaux
                // exemple une led 3 couleurs aura 3 cannaux pour rouge vert bleu
                // donc la deuxieme lampe devra etre configure sur un canal 4

                // Si on veut faire un d�filement de couleur automatique
                
                //}
            }
        }
    }
}