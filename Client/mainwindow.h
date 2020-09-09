#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientconnection.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_m_ConnBtn_clicked();
    void on_m_DisconnBtn_clicked();
    void putMessage();
    void changeButtonsConnected();
    void socketErrorHandled();
    void changeButtonsUnconnected();
private:
    Ui::MainWindow *ui;
    ClientConnection *m_client;
};

#endif // MAINWINDOW_H
