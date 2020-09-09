#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QString>
#include <QThread>
#include <QTcpSocket>
#include <string>

#include "clientinfo.h"

class ClientThread : public QThread
{
    Q_OBJECT
public:
    explicit ClientThread(QTcpSocket *socket = Q_NULLPTR, QString name = "John Doe");
    ~ClientThread();
    void run();
    int exec();
    ClientInfo* Clinf(){return ci;}
    void setWriteData(std::string dt){/*qDebug() << "setWriteData ";*/ m_writeData = dt;}
    void setWriteMode(bool v){m_write = v;}
    QTcpSocket *getSocket(){return sock;}
    void setSocket(QTcpSocket*s){sock=s;}
    QString getName(){return m_name;}
    public slots:
    void push();
    void breakPush();
signals:
    void threadCreated();
    void putMessage(QTcpSocket* s, const std::string &str);
private:
    QTcpSocket *sock;
    ClientInfo *ci;
    std::string m_writeData;
    bool m_write;
    QString m_name;
};

#endif // CLIENTTHREAD_H
