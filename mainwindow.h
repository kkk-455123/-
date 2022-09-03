#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // 绘制波形图
    void PlotInit();

private:
    Ui::MainWindow *ui;
    QTimer* timer;

    QVector<double> xTime;  // x时间轴
    QVector<double> yCH4;  // 气体浓度信息


// 槽函数
private slots:
    void PlotUpdate();
    void InfoUpdate();
};
#endif // MAINWINDOW_H
