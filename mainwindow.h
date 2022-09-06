#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>

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
    void WidgetInit();

    // 报警菜单配置
    void AlertInit();

    // 通信配置
    void socketInit();  // 设置监听
    // 改成按钮点击连接

// 槽函数
private slots:
    void PlotUpdate();  // 波形图更新
    void InfoUpdate();  // 传感器信息更新
//    void on_pushButton_clicked();  // 按钮点击事件
//    void on_pushButton_2_clicked();
    void SelectPage();  // 按钮点击切换页面

    // 通信相关
    void acceptConnection();  // 接受客户端新连接
    void readMessage();  // 读数据

};
#endif // MAINWINDOW_H
