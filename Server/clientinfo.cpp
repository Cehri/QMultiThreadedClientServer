#include <QMessageBox>
#include <QDebug>
#include "clientinfo.h"
#include "clientthread.h"
#include "mainwindow.h"

int ClientInfo::nextID = 0;

ClientInfo::ClientInfo(QTcpSocket *socket, QObject *parent) : QObject(parent), m_socket(socket)
{
    //m_socket = new QTcpSocket();
    qDebug() << "ClientInfo ctor!";
    id = ++nextID;
}

ClientInfo::~ClientInfo()
{
    //delete m_socket;
}

void ClientInfo::writeSocket(QTcpSocket* s, const std::string &str)
{
     s->write(QByteArray::fromStdString(str));
}

void ClientInfo::socketErrorHandler(QAbstractSocket::SocketError se)
{
    QString str;

    switch (se) {
        case QAbstractSocket::ConnectionRefusedError:
            str = "Server cannot run or refused";
            break;
        case QAbstractSocket::RemoteHostClosedError:
            str = "Remote host closed";
            break;
        default:
            str = QString("Socket error: %1").arg(se);
            break;
    }
    qDebug() << str;

    //MainWindow *mw = static_cast<MainWindow *>(parent());
    //mw->removeOneClient(this);

}
