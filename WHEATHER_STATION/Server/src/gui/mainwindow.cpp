#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <sstream>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow),
    m_tempSeries(new QLineSeries()), m_aqiSeries(new QLineSeries()),
    m_tempChart(new QChart()), m_aqiChart(new QChart()),
    m_xAxisTemp(new QValueAxis()), m_yAxisTemp(new QValueAxis()),
    m_xAxisAQI(new QValueAxis()), m_yAxisAQI(new QValueAxis()),
    m_chartUpdateTimer(new QTimer(this)), m_currentChartClientId(-1) 
{
    ui->setupUi(this);
    connect(ui->showChartsBtn, &QPushButton::clicked, this, &MainWindow::onShowChartsClicked);
    connect(m_chartUpdateTimer, &QTimer::timeout, this, &MainWindow::updateCharts);

    setupCharts();

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
    delete m_tempSeries;
    delete m_aqiSeries;
    delete m_tempChart;
    delete m_aqiChart;
    delete m_xAxisTemp;
    delete m_yAxisTemp;
    delete m_xAxisAQI;
    delete m_yAxisAQI;
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
    g_dbManager.clearAllData();
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


void MainWindow::onShowChartsClicked() {
    int clientId = getSelectedClientSocket();
    if (clientId < 0) return;

    m_currentChartClientId = clientId;
    updateCharts(); // Immediate update
    m_chartUpdateTimer->start(1000); // Update every 2 seconds
}

void MainWindow::setupCharts() {
    m_tempChart->legend()->hide();
    m_tempChart->addSeries(m_tempSeries);
    m_tempChart->addAxis(m_xAxisTemp, Qt::AlignBottom);
    m_tempChart->addAxis(m_yAxisTemp, Qt::AlignLeft);
    m_tempSeries->attachAxis(m_xAxisTemp);
    m_tempSeries->attachAxis(m_yAxisTemp);
    m_tempChart->setTitle("Temperature Over Time");

    m_aqiChart->legend()->hide();
    m_aqiChart->addSeries(m_aqiSeries);
    m_aqiChart->addAxis(m_xAxisAQI, Qt::AlignBottom);
    m_aqiChart->addAxis(m_yAxisAQI, Qt::AlignLeft);
    m_aqiSeries->attachAxis(m_xAxisAQI);
    m_aqiSeries->attachAxis(m_yAxisAQI);
    m_aqiChart->setTitle("AQI Over Time");

    ui->temperatureChartView->setChart(m_tempChart);
    ui->aqiChartView->setChart(m_aqiChart);
}

void MainWindow::updateCharts() {
    if (m_currentChartClientId < 0) return;

    std::vector<std::pair<float, int>> data = g_dbManager.fetchLastNotificationData(m_currentChartClientId, 50);

    m_tempSeries->clear();
    m_aqiSeries->clear();

    int index = 0;
    float minTemp = FLT_MAX, maxTemp = FLT_MIN;
    int minAQI = INT_MAX, maxAQI = INT_MIN;

    for (const auto& [temp, aqi] : data) {
        m_tempSeries->append(index, temp);
        m_aqiSeries->append(index, aqi);
        minTemp = std::min(minTemp, temp);
        maxTemp = std::max(maxTemp, temp);
        minAQI = std::min(minAQI, aqi);
        maxAQI = std::max(maxAQI, aqi);
        ++index;
    }

    m_xAxisTemp->setRange(0, std::max(1, static_cast<int>(data.size()) - 1));
    m_yAxisTemp->setRange(minTemp - 1, maxTemp + 1);

    m_xAxisAQI->setRange(0, std::max(1, static_cast<int>(data.size()) - 1));
    m_yAxisAQI->setRange(minAQI - 1, maxAQI + 1);
}