#pragma once

#include <QMainWindow>
#include "Server.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onClientConnected(int socketId);
    void onClientDisconnected(int socketId);
    void onSetSampleTimeClicked();
    void onSetThresholdClicked();
    void onResetLogClicked();
    void onDownloadLogClicked();
    void onShutdownServer();
    void onStartServer();
    void onShowClientDataClicked();

private:
    Ui::MainWindow* ui;
    Server* m_server;
    QThread* m_serverThread;

    int getSelectedClientSocket();
};
