#pragma once

#include <QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QDebug>
#include <QTimer>

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
    void onShowChartsClicked();

private:
    Ui::MainWindow* ui;
    Server* m_server;
    QThread* m_serverThread;

    int getSelectedClientSocket();

    void setupCharts();
    void updateCharts();

    QLineSeries *m_tempSeries;
    QLineSeries *m_aqiSeries;
    QChart *m_tempChart;
    QChart *m_aqiChart;
    QChartView *m_tempChartView;
    QChartView *m_aqiChartView;
    QValueAxis *m_xAxisTemp;
    QValueAxis *m_yAxisTemp;
    QValueAxis *m_xAxisAQI;
    QValueAxis *m_yAxisAQI;

    QTimer* m_chartUpdateTimer;
    int m_currentChartClientId;

    int m_chartUpdateCounter = 0;  // Used as X-axis
};
