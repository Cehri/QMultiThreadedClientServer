#ifndef SERVER_H
#define SERVER_H

#include <QTimer>
#include <QtNetwork>
#include <QString>
#include <QVector>
#include <QThreadPool>
#include <string>
#include "clientinfo.h"
#include "clientthread.h"
#include "datapacket.h"

class Server : public QObject {
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    void startServer(int port= DefPort);
    QTcpServer *server() { return m_tcp_server; }
    void setWriteData(std::string dt);
    std::string getWriteData(){return m_writeData;}
    QString getConnStr(){return m_conn_str;}
    QString getDataFileContent() {return m_data_file_content;}
    void setDataFileContent(QString str) {m_data_file_content = str;}
    void setDataPacket();
    DataPacket getDataPacket() {return m_data_pack;}
    const static int DefPort = 5050;
    void appendClient(ClientInfo *ci){m_clients.append(ci);}
    void removeClient(ClientInfo *ci){m_clients.removeOne(ci);}
    QVector<ClientInfo *> getClients(){return m_clients;}
    int getQTimerId() {return timer->timerId(); }
signals:
    void handledConn();
    void transactionComplete();
    void sd();
    void bsd();
    void sock_disconnected();
public slots:
    void newConnectionHandler();
    void close();
    void sendData();
    void stopData();
    void timeredSendData();
    void disconnectedSocket();
    void socketStateChanged(QAbstractSocket::SocketState se);
private:
    QTimer *timer;
    QTcpServer *m_tcp_server;
    std::string m_writeData;
    QVector<ClientInfo *> m_clients;
    QString m_conn_str;
    QVector<ClientThread*> m_activeThreads;
    QMap<QTcpSocket*, QString> m_socket_thread;
    QString m_data_file_content;
    DataPacket m_data_pack;
    int counter;
};

#endif // SERVER_H
