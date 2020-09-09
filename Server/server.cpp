#include "server.h"
#include "mainwindow.h"
#include <QByteArray>
#include <QMap>
#include <typeinfo>

static unsigned long long id;

Server::Server(QObject *parent) : QObject(parent)
{
    m_tcp_server = new QTcpServer(this);
    timer = new QTimer();

    connect(m_tcp_server, SIGNAL(newConnection()),this, SLOT(newConnectionHandler()));
    connect(timer,SIGNAL(timeout()),this,SLOT(sendData()));
    timer->setInterval(1000);

    counter = 0;
}

Server::~Server()
{
    delete m_tcp_server;
}

void Server::startServer(int port)
{
    if (!m_tcp_server->isListening())
        m_tcp_server->listen(QHostAddress::Any, port);
}

void Server::newConnectionHandler()
{
    if (id == std::numeric_limits<unsigned long long>::max())
        id = 0ul;
    id++;
    QTcpSocket* socket = m_tcp_server->nextPendingConnection();
    QString str = QString::number(id) + "_" +QString::number(socket->socketDescriptor());
    ClientThread *clientThread = new ClientThread(socket,str);

    m_socket_thread.insert(socket, str);
    m_activeThreads.append(clientThread);
    appendClient(clientThread->Clinf());

    clientThread->start();

    m_conn_str = QString("%1, %2").arg(socket->peerAddress().toString().remove(0,7)).arg(socket->peerPort());

    connect(clientThread,SIGNAL(finished()),clientThread, SLOT(deleteLater()));
    connect(this,SIGNAL(sd()), clientThread, SLOT(push()));
    connect(this,SIGNAL(bsd()), clientThread, SLOT(breakPush()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnectedSocket()));
    connect(socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(socketStateChanged(QAbstractSocket::SocketState)));

    emit handledConn();
}

void Server::socketStateChanged(QAbstractSocket::SocketState se)
{
    qDebug() << "socketStateChanged";
    qDebug() << se;

}

void Server::disconnectedSocket()
{
    qDebug() << "Server::disconnectedSocket()";
    QTcpSocket *s;

    QMapIterator<QTcpSocket*, QString> i(m_socket_thread);
    while (i.hasNext()) {
        i.next();
        qDebug() << "sock desc: " << i.key()->socketDescriptor() << " state : " << i.key()->state() << " is open : " << i.key()->isOpen() << " thread id " << i.value();
        s = i.key();
        if (s->state() == QAbstractSocket::UnconnectedState) {
            for (ClientThread *ct : m_activeThreads) {
                QString str1 = ct->getName();
                QString str2 = i.value();
                int compVal = QString::compare(str1, str2, Qt::CaseInsensitive);
                if (compVal == 0) {
                    qDebug() << "yakaladım siliyorum..";
                    m_socket_thread.remove(s);
                    s->close();
                    m_activeThreads.removeOne(ct);
                    removeClient(ct->Clinf());
                    ct->requestInterruption();
                    ct->wait();
                    ct->exit(0);
                    if (ct->isFinished()) {
                        qDebug() << "thread finished!";
                        ct->~ClientThread();
                    }
                }
            }

        }
    }

    emit sock_disconnected();
}

void Server::timeredSendData()
{
    timer->start();
    //timer->start(1000);
}

void Server::sendData()
{
    /*
    qDebug() << "Threads:";
    for (ClientThread* ct : m_activeThreads)
        qDebug() << ct->getName();
    qDebug() << "\n";
    qDebug() << "Clients:";
    for (ClientInfo *ci : m_clients)
        qDebug() << ci->getId();
    qDebug() << "\n";
    QMapIterator<QTcpSocket*, QString> i(m_socket_thread);
    while (i.hasNext()) {
        i.next();
        qDebug() << "sock desc: " << i.key()->socketDescriptor() << " state : " << i.key()->state() << " is open : " << i.key()->isOpen() << " thread id " << i.value();
    }

    qDebug() << "Server::sendData()";
    qDebug() << "active thread count: " << m_activeThreads.size();
    */
    for (ClientThread *ct : m_activeThreads) {
        qDebug() << QTime().currentTime().msecsSinceStartOfDay() << "counter : " << counter++ << " timer_id: "  << this->getQTimerId() << " "<< ct->getName() << m_activeThreads.count() << "   " << QString::fromStdString(getWriteData());
        ct->setWriteData(getWriteData());
    }
    emit sd();
}

void Server::stopData()
{
    timer->stop();
    sendData();
    emit bsd();
}

void Server::close()
{
    for (ClientInfo *ci : m_clients) {
        ci->socket()->close();
    }
    m_tcp_server->close();
}

void Server::setWriteData(std::string dt)
{
    m_writeData = "";
    m_writeData = dt;
}

void Server::setDataPacket()
{
    auto chunks = m_data_file_content.split("\n", QString::SkipEmptyParts);
    auto chunksCount = chunks.count();

    if (chunksCount > 2) {
        qDebug() << "Control the datas.txt! There should be 2 chunks!";
        return;
    }

    if (chunksCount == 1) {
        chunks = m_data_file_content.split(" ", QString::SkipEmptyParts);
        chunksCount = chunks.count();

        if (chunksCount > 2) {
            qDebug() << "Control the datas.txt! There should be 2 chunks!";
            return;
        }
    }

    m_data_pack.chunk001 = chunks.at(0).toLocal8Bit().toInt();
    m_data_pack.chunk002 = chunks.at(1).toLocal8Bit().toInt();

}
