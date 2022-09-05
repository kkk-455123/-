#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void PlotInit();  // 所有波形图初始化

private:
    Ui::MainWindow *ui;
    QTimer* timer;

    QVector<double> xCH4;  // x时间轴
    QVector<double> yCH4;  // 气体浓度信息

    void GraphInit(QCustomPlot*, QSharedPointer<QCPAxisTickerTime>&);  // 单个波形图初始化
    void WidgetInit();


// 槽函数
private slots:
    void PlotUpdate();  // 波形图更新
    void InfoUpdate();  // 传感器信息更新
//    void on_pushButton_clicked();  // 按钮点击事件
//    void on_pushButton_2_clicked();
    void SelectPage();  // 按钮点击切换页面

};
#endif // MAINWINDOW_H
