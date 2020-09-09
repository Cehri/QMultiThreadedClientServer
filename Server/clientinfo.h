#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QObject>
#include <QTcpSocket>
#include <string>

class ClientInfo : public QObject
{
    Q_OBJECT
public:
    explicit ClientInfo(QTcpSocket *socket = Q_NULLPTR, QObject *parent = 0);
    ~ClientInfo();
    QTcpSocket *socket() { return m_socket; }
    int getId(){return id;}
protected:
    static int nextID;
public slots:
    void writeSocket(QTcpSocket* s, const std::string &str);
    void socketErrorHandler(QAbstractSocket::SocketError se);
private:
    QTcpSocket *m_socket;
    int id;
};

#endif // CLIENTINFO_H
