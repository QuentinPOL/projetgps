#pragma once

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>
#include "PilotageLumiere.h"

class WebSocketQTDMX : public QWebSocketServer
{
    Q_OBJECT

public:
    explicit WebSocketQTDMX(const QHostAddress& address = QHostAddress::Any, quint16 port = 12345, QObject* parent = nullptr);
    ~WebSocketQTDMX();

    PilotageLumiere* piloteLum;
public slots:
    void onServerNewConnection(); // Connexion client
    void onClientDisconnected(); // Deconnexion client
    void onWebClientReadyRead(const QString& message); // Reception message client
};