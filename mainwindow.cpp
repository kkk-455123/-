#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer())
    , xCH4(10, 0), yCH4(10, 0)
{
    ui->setupUi(this);

    // 波形图初始化
    this->PlotInit();

    // 定时器设置
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlotUpdate()));
    timer->start();

}

void
MainWindow::PlotInit() {
//    for(int i = 0; i < 10; i++) {
//        xCH4[i] = i + 1;
//        yCH4[i] = xCH4[i] * xCH4[i];
//    }
    // 添加曲线
    ui->widget->addGraph();
    // 设置曲线0数据
    ui->widget->graph(0)->setData(xCH4, yCH4);
    // 设置样式
    // 坐标轴标签
    ui->widget->xAxis->setLabel("时间");
    ui->widget->yAxis->setLabel("气体浓度");
    // 坐标轴范围
    ui->widget->xAxis->setRange(0, 10);
    ui->widget->yAxis->setRange(0, 100);
    ui->widget->replot();
}

void
MainWindow::PlotUpdate() {
    // test
    static int value = 0;
    ui->progressBar->setValue(value++);
    if(value >100)
        timer->stop();

    // 模拟数据
    for(int i = 0; i < 10; i++) {
        yCH4[i] ++;
    }

    // 图一更新
    ui->widget->graph(0)->setData(xCH4, yCH4);
    ui->widget->replot();
    // 图二更新
}

void
MainWindow::InfoUpdate() {
    ui->listView->
}


MainWindow::~MainWindow()
{
    delete ui;
}

