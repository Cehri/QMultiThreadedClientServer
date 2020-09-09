#include <QDebug>
#include <QMessageBox>
#include <cstdlib>

#include "clientconnection.h"
#include "mainwindow.h"

ClientConnection::ClientConnection(QObject *parent) : QThread(parent)
{
    m_clientSocket = new QTcpSocket();
    buffer = new char;

    connect(m_clientSocket, SIGNAL(readyRead()), this, SLOT(readyReadHandler()));
    connect(m_clientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketErrorHandler(QAbstractSocket::SocketError)));
    connect(m_clientSocket, SIGNAL(connected()),this,SLOT(connectedHandler()));
    connect(m_clientSocket, SIGNAL(disconnected()), this, SLOT(disconnectedHandler()));
}

ClientConnection::~ClientConnection()
{
    delete m_clientSocket;
    delete[] buffer;
}

QString ClientConnection::getLocalAddress()
{
    return m_clientSocket->localAddress().toString();
}

int ClientConnection::getLocalPort()
{
    return m_clientSocket->localPort();
}

QString ClientConnection::getPeerAddress()
{
    return m_clientSocket->peerAddress().toString();
}

int ClientConnection::getPeerPort()
{
    return m_clientSocket->peerPort();
}

void ClientConnection::run()
{
    m_clientSocket->connectToHost(m_ip, m_port);
}

void ClientConnection::stop()
{
    m_clientSocket->disconnectFromHost();

    this->quit();
    this->wait();
}

void ClientConnection::readyReadHandler()
{
    qDebug() << "*initialized buffer: " << buffer;
    buffer[0] = '\0';

    int result;
    result = m_clientSocket->read(buffer, 1000);
    buffer[result] = '\0';

    qDebug() << "buffer data: " << buffer;
/*
    if (buffer[0] == 'S' && buffer[1] == 'T' && buffer[2] == 'O' && buffer[3] == 'P') {
        qDebug() << "Stop condition!";

        for (int i = 0; i < 1000; i++)
            buffer[i] = '\0';
    }
*/
    emit dataRead();
}

void ClientConnection::connectedHandler()
{
    m_connStatus = QString("Durum: Bağlandı\nYerel IP:%1, Yerel Port:%2\nSunucu IP:%3, Sunucu Port:%4").arg(getLocalAddress()).
            arg(getLocalPort()).arg(getPeerAddress()).arg(getPeerPort());
}

void ClientConnection::disconnectedHandler()
{
    qDebug() << "ClientConnection::disconnectedHandler()";

}

void ClientConnection::socketErrorHandler(QAbstractSocket::SocketError se)
{
    switch (se) {
        case QAbstractSocket::ConnectionRefusedError:
            m_connStatus = "Sunucu çalışmıyor veya bağlantı reddedildi.";
            break;
        case QAbstractSocket::RemoteHostClosedError:
            m_connStatus = "Sunucu kapandı.";
            break;
        default:
            m_connStatus = QString("Socket error: %1").arg(se);
            break;
    }

    emit errorHandled();
}
