#include "internetstatus.h"
#include <QTcpSocket>

InternetStatus::InternetStatus(QObject *parent)
    : QObject{parent}
{

}

bool InternetStatus::hasInternetConnection() const
{
    QTcpSocket checkConnectionSocket;
    checkConnectionSocket.connectToHost("google.com", 443); // 443 for HTTPS or use Port 80 for HTTP
    checkConnectionSocket.waitForConnected(2000);

    const bool isConnected = checkConnectionSocket.state() == QTcpSocket::ConnectedState;

    checkConnectionSocket.close();
    return isConnected;
}
