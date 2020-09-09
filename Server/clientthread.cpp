#include <QDebug>
#include <QMessageBox>
#include <QTimer>
#include "clientthread.h"
#include "clientinfo.h"
#include "server.h"

ClientThread::ClientThread(QTcpSocket *socket, QString name) : m_name(name)
{
    qDebug() << "ClientThread::ClientThread ctor";

    qRegisterMetaType<std::string>("std::string");

    ci = new ClientInfo(socket, this);
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), ci, SLOT(socketErrorHandler(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(putMessage(QTcpSocket*, const std::string&)),ci,SLOT(writeSocket(QTcpSocket*, const std::string&)));

    emit threadCreated();
}

ClientThread::~ClientThread()
{
   delete ci;
}


void ClientThread::run()
{
    qDebug() << "ClientThread::run()";
    qDebug() << "write data: " << QString::fromStdString(m_writeData);
    exec();
}

int ClientThread::exec()
{

    qDebug() << "ClientThread::exec()";
    qDebug() << "write data: " << QString::fromStdString(m_writeData);
    qDebug() << "is client socket open? " << Clinf()->socket()->isOpen();

    m_write = false;
    while (!this->isInterruptionRequested()) {
        while (m_write) {
            //qDebug() << "m_write " << m_write;
            //qDebug() << "m_writeData " << QString::fromStdString(this->m_writeData);
            emit putMessage(ci->socket(),m_writeData);
            //ci->socket()->write(QByteArray::fromStdString(m_writeData));
            m_write = false;
        }

    }
    return 1;

}

void ClientThread::push()
{
    //qDebug() << "ClientThread::push()";
    m_write = true;
}

void ClientThread::breakPush()
{
    //qDebug() << "ClientThread::breakPush()";
    m_write = false;
}

