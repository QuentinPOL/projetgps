#include <QtCore/QCoreApplication>
#include "WebSocketQTDMX.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    WebSocketQTDMX server;

    return a.exec();
}
