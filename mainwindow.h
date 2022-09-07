#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer* timer;  // 计时器

    QVector<double> xCH4;  // x时间轴
    QVector<double> yCH4;  // 气体浓度信息

    // socket
    QTcpServer* tcpServer;  // server对象
    QTcpSocket* clientConnection;  // 客户端连接

    // 环境监测菜单配置
    void GraphInit(QCustomPlot*, QSharedPointer<QCPAxisTickerTime>&);  // 单个波形图初始化
    void PlotInit();  // 所有波形图初始化
    void Interpolation(QCustomPlot*, double, double, int);
    void WidgetInit();

    // 报警菜单配置
    void AlertInit();

    // 通信菜单配置
    void socketInit(uint);  // 设置监听

    // 定位菜单配置
    void PositionInit();

// 槽函数
private slots:
    void PlotUpdate();  // 波形图更新
    void InfoUpdate();  // 传感器信息更新
    void SelectPage();  // 按钮点击切换页面

    // TCP通信相关(WIFI)
    QString getLocalIP();
    void acceptConnection();  // 接受客户端新连接
    void readMessage();  // 读数据
    void on_connButtonWIFI_clicked();  // 点击按钮连接
    void on_disconnButtonWIFI_clicked();  // 点击按钮断开连接

};
#endif // MAINWINDOW_H
