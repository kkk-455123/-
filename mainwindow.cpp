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
    connect(timer, SIGNAL(timeout()), this, SLOT(InfoUpdate()));
    timer->start();

}

void MainWindow::GraphInit(QCustomPlot* customplot, QSharedPointer<QCPAxisTickerTime>& timeTicker) {
    customplot->xAxis->setTicker(timeTicker);  // 将x轴设置为时间轴
    customplot->xAxis->setTickLabelRotation(30);  // 时间显示旋转角度

    // 设置样式
    // 坐标轴标签
    customplot->xAxis->setLabel("时间");
    customplot->yAxis->setLabel("气体浓度");
    // 坐标轴范围
//    ui->widget->xAxis->setRange(0, 10);
    customplot->yAxis->setRange(0, 100);
    // 添加曲线
    customplot->addGraph(ui->widget->xAxis, ui->widget->yAxis);
    customplot->setInteractions(QCP::iRangeDrag //可平移
                                | QCP::iRangeZoom //可滚轮缩放
                                | QCP::iSelectLegend );//可选中图例)
//    // 设置曲线0数据
//    ui->widget->graph(0)->setData(xCH4, yCH4);
    ui->widget->replot();
}

void
MainWindow::PlotInit() {
    // 时间轴
    QSharedPointer<QCPAxisTickerTime> dateTicker(new QCPAxisTickerTime);  // 时间轴格式变量 必须要用智能指针
    dateTicker->setTimeFormat("%h:%m:%s");  // 显示格式 时:分:秒
    dateTicker->setTickCount(20);

    this->GraphInit(ui->widget, dateTicker);
    this->GraphInit(ui->widget_2, dateTicker);
}

void
MainWindow::PlotUpdate() {
    // test 进度条
    static int value = 0;
    if(value <= 100)
        ui->progressBar->setValue(value++);

    double nowTime = QTime::currentTime().hour()*3600 + QTime::currentTime().minute()*60 + QTime::currentTime().second();
    ui->widget->xAxis->setRange(nowTime - 10, nowTime);

    // setdata方式
//    for (int i = 0; i < 10; i++) {
//        xCH4[i] = nowTime + i;
//        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
//        yCH4[i] = qrand()%100;  // qrand生成四位随机数，%10保留个位
//        qDebug() << yCH4[i];
//    }
//    ui->widget->graph(0)->setData(xCH4, yCH4);

    // adddata方式
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int y = qrand()%100;  // qrand生成四位随机数，%10保留个位
    ui->widget->graph(0)->addData(nowTime, y);

    // 图一更新
//    ui->widget->graph(0)->setData(xCH4, yCH4);
//    ui->widget->graph(0)->addData(nowTime, value);
    ui->widget->replot();

    // 图二更新
}

void
MainWindow::InfoUpdate() {
    QString temp = QString::number(yCH4[0]);
    ui->label_2->setText("当前浓度:" + temp);
    ui->label_3->setText("峰值浓度:" + temp);
}

void
MainWindow::on_pushButton_clicked() {
    ui->frame_5->setVisible(true);
    timer->start();
}

void
MainWindow::on_pushButton_2_clicked() {
    ui->frame_5->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

