#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>

class ClientConnection : public QThread
{
    Q_OBJECT
public:
    explicit ClientConnection(QObject *parent = 0);
    ~ClientConnection();

    QTcpSocket *socket(){return m_clientSocket;}

    QString getLocalAddress();
    int getLocalPort();
    QString getPeerAddress();
    int getPeerPort();

    void setIp(QHostAddress i){m_ip = i;}
    void setPort(int p){m_port = p;}

    void run();
    void stop();

    QString getConnStatus(){return m_connStatus;}
    char* getBuff(){return buffer;}

signals:
    void dataRead();
    void errorHandled();

private slots:
    void connectedHandler();
    void socketErrorHandler(QAbstractSocket::SocketError se);
    void readyReadHandler();
    void disconnectedHandler();
private:
    QTcpSocket *m_clientSocket;
    QHostAddress m_ip;
    int m_port;
    QString m_connStatus;
    char *buffer;
};

#endif // CLIENTCONNECTION_H
