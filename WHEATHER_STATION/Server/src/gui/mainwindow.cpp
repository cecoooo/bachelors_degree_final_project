#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    m_server = new Server();
    m_serverThread = new QThread(this);
    m_server->moveToThread(m_serverThread);

    connect(m_serverThread, &QThread::started, m_server, &Server::start);
    connect(this, &MainWindow::destroyed, this, &MainWindow::onShutdownServer);
    connect(m_server, &Server::clientConnected, this, &MainWindow::onClientConnected);
    connect(m_server, &Server::clientDisconnected, this, &MainWindow::onClientDisconnected);
    connect(m_server, &Server::serverStopped, m_serverThread, &QThread::quit);

    connect(ui->setSampleTimeBtn, &QPushButton::clicked, this, &MainWindow::onSetSampleTimeClicked);
    connect(ui->setThresholdBtn, &QPushButton::clicked, this, &MainWindow::onSetThresholdClicked);
    connect(ui->resetLogBtn, &QPushButton::clicked, this, &MainWindow::onResetLogClicked);
    connect(ui->downloadLogBtn, &QPushButton::clicked, this, &MainWindow::onDownloadLogClicked);
    connect(ui->shutdownBtn, &QPushButton::clicked, this, &MainWindow::onShutdownServer);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::onStartServer);
    connect(ui->showDataBtn, &QPushButton::clicked, this, &MainWindow::onShowClientDataClicked);


    m_serverThread->start();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onClientConnected(int socketId) {
    ui->clientListWidget->addItem(QString::number(socketId));
}

void MainWindow::onClientDisconnected(int socketId) {
    for (int i = 0; i < ui->clientListWidget->count(); ++i) {
        if (ui->clientListWidget->item(i)->text().toInt() == socketId) {
            delete ui->clientListWidget->takeItem(i);
            break;
        }
    }
}

int MainWindow::getSelectedClientSocket() {
    auto item = ui->clientListWidget->currentItem();
    return item ? item->text().toInt() : -1;
}

void MainWindow::onSetSampleTimeClicked() {
    int id = getSelectedClientSocket();
    bool ok;
    ushort val = ui->sampleTimeEdit->text().toUShort(&ok);
    if (!ok) return;
    QByteArray payload(reinterpret_cast<const char*>(&val), sizeof(val));
    QMetaObject::invokeMethod(m_server, [=] { m_server->sendCommandToClient(id, '0', payload); }, Qt::QueuedConnection);
}

void MainWindow::onSetThresholdClicked() {
    int id = getSelectedClientSocket();
    bool ok;
    ushort val = ui->thresholdEdit->text().toUShort(&ok);
    if (!ok) return;
    QByteArray payload(reinterpret_cast<const char*>(&val), sizeof(val));
    QMetaObject::invokeMethod(m_server, [=] { m_server->sendCommandToClient(id, '1', payload); }, Qt::QueuedConnection);
}

void MainWindow::onResetLogClicked() {
    int id = getSelectedClientSocket();
    QMetaObject::invokeMethod(m_server, [=] { m_server->sendCommandToClient(id, '2', {}); }, Qt::QueuedConnection);
}

void MainWindow::onDownloadLogClicked() {
    int id = getSelectedClientSocket();
    QMetaObject::invokeMethod(m_server, [=] { m_server->sendCommandToClient(id, '3', {}); }, Qt::QueuedConnection);
}

void MainWindow::onShutdownServer() {
    QMetaObject::invokeMethod(m_server, &Server::shutdown, Qt::QueuedConnection);
}

void MainWindow::onStartServer() {
    if (!m_server || m_serverThread->isRunning())
        return;

    m_server = new Server();
    m_server->moveToThread(m_serverThread);

    connect(m_serverThread, &QThread::started, m_server, &Server::start);
    connect(m_server, &Server::clientConnected, this, &MainWindow::onClientConnected);
    connect(m_server, &Server::clientDisconnected, this, &MainWindow::onClientDisconnected);
    connect(m_server, &Server::serverStopped, m_serverThread, &QThread::quit);

    m_serverThread->start();
}

void MainWindow::onShowClientDataClicked() {
    ui->databaseTextDisplay->clear();

    int clientId = getSelectedClientSocket();
    if (clientId == -1) {
        ui->databaseTextDisplay->setPlainText("No client selected.");
        return;
    }

    std::vector<std::string> records = g_dbManager.fetchClientData(clientId);
    if (records.empty()) {
        ui->databaseTextDisplay->setPlainText("No data found for the selected client.");
        return;
    }

    QStringList lines;
    for (const auto& record : records)
        lines << QString::fromStdString(record);

    ui->databaseTextDisplay->setPlainText(lines.join("\n"));
}