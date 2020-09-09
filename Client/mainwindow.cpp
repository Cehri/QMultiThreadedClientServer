#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QTimer>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "clientconnection.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_client(new ClientConnection)
{
    ui->setupUi(this);
    ui->label_3->setText("Durum: Bağlı Değil");

    connect(m_client,SIGNAL(dataRead()), this,SLOT(putMessage()));
    connect(m_client, SIGNAL(errorHandled()), this, SLOT(socketErrorHandled()));
    connect(m_client->socket(),SIGNAL(connected()),this,SLOT(changeButtonsConnected()));
    connect(m_client->socket(), SIGNAL(disconnected()),this,SLOT(changeButtonsUnconnected()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_m_ConnBtn_clicked()
{
    ui->m_DisconnBtn->setEnabled(false);
    ui->m_ConnBtn->setEnabled(false);

    m_client->setIp(QHostAddress(ui->m_lineEditIP->text()));
    m_client->setPort(ui->m_lineEditPort->text().toInt());
    m_client->run();
}

void MainWindow::socketErrorHandled()
{
    QMessageBox::information(this, "Error", m_client->getConnStatus());

    changeButtonsUnconnected();
}

void MainWindow::changeButtonsConnected()
{
    ui->m_DisconnBtn->setEnabled(true);
    ui->m_ConnBtn->setEnabled(false);
    ui->m_ConnBtn->setText("Bağlandı");

    ui->label_3->setText(m_client->getConnStatus());
}

void MainWindow::on_m_DisconnBtn_clicked()
{
    ui->m_ConnBtn->setEnabled(false);
    ui->m_DisconnBtn->setEnabled(false);
    m_client->stop();
}

void MainWindow::changeButtonsUnconnected()
{
    ui->m_ConnBtn->setEnabled(true);
    ui->m_DisconnBtn->setEnabled(false);
    ui->m_ConnBtn->setText("Bağlan");

    ui->label_3->setText("Durum: Bağlı Değil");
}

void MainWindow::putMessage()
{
    QString str = QString(m_client->getBuff());
    QString oldStr = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textEdit->insertHtml(QString("<font color=grey> %1 </font>").arg(oldStr));
    ui->textEdit->insertHtml(QString("<font color=black> %1 </font>").arg(str));

    ui->textEdit->ensureCursorVisible();
}
