#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QRegularExpression>
#include <string>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->closeBtn->setEnabled(false);
    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);
    m_server.setParent(this);
    m_server.startServer();
    //connect(m_server.server(), SIGNAL(newConnection()), &m_server, SLOT(newConnectionHandler()));
    connect(&m_server,SIGNAL(handledConn()), this,SLOT(newConnectionHandler()));
    connect(this,SIGNAL(sendClicked()),&m_server, SLOT(timeredSendData()));
    connect(this,SIGNAL(stopClicked()), &m_server, SLOT(stopData()));
    connect(&m_server, SIGNAL(sock_disconnected()), this,SLOT(removeClient()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newConnectionHandler()
{
    qDebug() << "MainWindow::newConnectionHandler()";
    ui->sendBtn->setEnabled(true);
    qDebug() << "connection string: " << m_server.getConnStr();
    QListWidgetItem *lvi = new QListWidgetItem(m_server.getConnStr());
    qDebug() << "QListWidgetItem *lvi inited";
    ui->m_listWidgetClients->addItem(lvi);
    qDebug() << "ui->m_listWidgetClients->addItem(lvi) success!";
}

void MainWindow::on_pushButton_3_clicked()
{
    QString fPath = QFileDialog::getOpenFileName(this, tr("Open TXT File"), "/home", tr("TXT Files (*.txt)"));
    ui->lineEdit->setText(fPath);

    QFile file(fPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString content = file.readAll();

    ui->textEditData->setText(content);
    m_server.setDataFileContent(content);
    m_server.setDataPacket();

    file.close();
}

void MainWindow::removeOneClient(ClientInfo* ci)
{
    qDebug() << "MainWindow::removeOneClient";
    m_server.getClients().removeOne(ci);
    removeClient();
}

void MainWindow::removeClient()
{
    qDebug() << "MainWindow::removeClient";
    //m_clients.removeOne(cliInfo);
    ui->m_listWidgetClients->clear();

    if (m_server.getClients().size() == 0) {
        ui->closeBtn->setEnabled(true);
        ui->sendBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        m_server.stopData();
    }

    for (ClientInfo *ci : m_server.getClients()) {
        QString str = QString("%1, %2").arg(ci->socket()->peerAddress().toString().remove(0,7)).arg(ci->socket()->peerPort());
        ui->m_listWidgetClients->addItem(str);
    }

}


void MainWindow::on_runBtn_clicked()
{
    ui->closeBtn->setEnabled(true);
    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    m_server.startServer();

    ui->runBtn->setEnabled(false);
}

void MainWindow::on_closeBtn_clicked()
{
    ui->runBtn->setEnabled(true);
    ui->sendBtn->setEnabled(false);
    ui->stopBtn->setEnabled(false);

    m_server.close();

    ui->m_listWidgetClients->clear();
    ui->closeBtn->setEnabled(false);
}

void MainWindow::on_sendBtn_clicked()
{
    ui->stopBtn->setEnabled(true);

    m_server.setWriteData(ui->textEditData->toPlainText().toStdString());

    ui->sendBtn->setEnabled(false);
    emit sendClicked();
}

void MainWindow::on_stopBtn_clicked()
{
    ui->sendBtn->setEnabled(true);
    std::string str = "STOP";
    m_server.setWriteData(str);
    ui->stopBtn->setEnabled(false);
    emit stopClicked();
}
