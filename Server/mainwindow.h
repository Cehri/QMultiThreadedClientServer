#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QtNetwork>
#include <QVector>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addClientMessage(const QString &str);
public slots:
    void removeClient();
    void removeOneClient(ClientInfo* ci);
signals:
    void sendClicked();
    void stopClicked();
private slots:
    void newConnectionHandler();
private:
    Ui::MainWindow *ui;
    Server m_server;
private slots:
    void on_pushButton_3_clicked();
    void on_runBtn_clicked();
    void on_sendBtn_clicked();
    void on_closeBtn_clicked();
    void on_stopBtn_clicked();
};

#endif // MAINWINDOW_H
