#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer())
    , xCH4(10, 0), yCH4(10, 0)
    , tcpServer(new QTcpServer(this))
{
    ui->setupUi(this);
    this->setWindowTitle("井下定位监测系统");
    ui->stackedWidget->setCurrentIndex(0);  // 首先显示环境监测菜单

    // 波形图初始化
    this->PlotInit();

    // 定时器设置
    timer->setInterval(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(PlotUpdate()));
    connect(timer, SIGNAL(timeout()), this, SLOT(InfoUpdate()));
    timer->start();

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::SelectPage);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::SelectPage);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::SelectPage);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::SelectPage);

    this->WidgetInit();
    this->AlertInit();
    this->PositionInit();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GraphInit(QCustomPlot* customplot, QSharedPointer<QCPAxisTickerTime>& timeTicker) {
    customplot->xAxis->setTicker(timeTicker);  // 将x轴设置为时间轴
    customplot->xAxis->setTickLabelRotation(30);  // 时间显示旋转角度

    // 坐标轴标签
    customplot->xAxis->setLabel("时间");
    QString yName;
    if(customplot == ui->widget_CO || customplot == ui->widget_H2S)
        yName = "浓度(ppm)";
    else yName = "浓度(%)";
    customplot->yAxis->setLabel(yName);
    // 坐标轴范围
//    ui->widget->xAxis->setRange(0, 10);
    customplot->yAxis->setRange(0, 100);
    // 添加曲线
    customplot->addGraph(customplot->xAxis, customplot->yAxis);
    customplot->setInteractions(QCP::iRangeDrag //可平移
                                | QCP::iRangeZoom //可滚轮缩放
                                | QCP::iSelectLegend );//可选中图例)

    // 设置样式
    QBrush brush(QColor(255, 50, 50, 100));
    QPen pen(Qt::red);
    customplot->graph(0)->setPen(pen);  // 设置line颜色
    customplot->graph(0)->setBrush(brush);  // 设置line下的填充颜色
//    // 设置曲线0数据
//    ui->widget->graph(0)->setData(xCH4, yCH4);
    customplot->replot();
}

void
MainWindow::PlotInit() {
    // 时间轴
    QSharedPointer<QCPAxisTickerTime> dateTicker(new QCPAxisTickerTime);  // 时间轴格式变量 必须要用智能指针
    dateTicker->setTimeFormat("%h:%m:%s");  // 显示格式 时:分:秒
    dateTicker->setTickCount(20);

    // 每个图表中波形图的初始化(暂不包括温湿度)
    this->GraphInit(ui->widget_CH4, dateTicker);
    this->GraphInit(ui->widget_H2S, dateTicker);
    this->GraphInit(ui->widget_CO, dateTicker);
    this->GraphInit(ui->widget_CO2, dateTicker);
    this->GraphInit(ui->widget_O2, dateTicker);
}

void
MainWindow::Interpolation(QCustomPlot* customplot, double x, double y, int count) {
    static QPoint newPoint;
    static QPoint lastPoint(0, 0);
    newPoint.setX(x);
    newPoint.setY(y);
    double dx = (newPoint.x() - lastPoint.x()) / 100.0;
    double dy = (newPoint.y() - lastPoint.y()) / 100.0;
    for(int i = 1; i <= count; i++) {
        double newX = lastPoint.x() + dx * i;
        double newY = lastPoint.y() + dy * i;
        customplot->graph(0)->addData(newX, newY);
    }
    lastPoint.setX(newPoint.x());
    lastPoint.setY(newPoint.y());
}

void
MainWindow::PlotUpdate() {
    // test 进度条
    static int value = 0;
    if(value <= 100)
        ui->progressBar->setValue(value++);

    // 当前时间作时间轴变量
    double nowTime = QTime::currentTime().hour()*3600 + QTime::currentTime().minute()*60 + QTime::currentTime().second();

    // 甲烷更新
    ui->widget_CH4->xAxis->setRange(nowTime - 10, nowTime);
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
    int yCH4 = qrand() % 10 + 5;  //qrand生成0~10随机数，+5生成5~15随机数
    ui->widget_CH4->graph(0)->addData(nowTime, yCH4);
    ui->widget_CH4->replot();

    // 硫化氢更新
    double yH2S = qrand() % 66 *0.1;
    ui->widget_H2S->xAxis->setRange(nowTime - 10, nowTime);
    ui->widget_H2S->yAxis->setRange(0, 10);
    ui->widget_H2S->graph(0)->addData(nowTime, yH2S);
    ui->widget_H2S->replot();

    // 一氧化碳更新
    double yCO = qrand() % 24;
    ui->widget_CO->xAxis->setRange(nowTime - 10, nowTime);
    ui->widget_CO->yAxis->setRange(0, 100);
    ui->widget_CO->graph(0)->addData(nowTime, yCO);
    ui->widget_CO->replot();

    // 二氧化碳更新
    double yCO2 = qrand() % 5 * 0.1;
    ui->widget_CO2->xAxis->setRange(nowTime - 10, nowTime);
    ui->widget_CO2->yAxis->setRange(0, 1);
    ui->widget_CO2->graph(0)->addData(nowTime, yCO2);
    ui->widget_CO2->replot();

    // 氧气更新
    double yO2 = qrand() % 5 + 20;
    ui->widget_O2->xAxis->setRange(nowTime - 10, nowTime);
    ui->widget_O2->yAxis->setRange(0, 100);
    ui->widget_O2->graph(0)->addData(nowTime, yO2);
    ui->widget_O2->replot();

}

void
MainWindow::InfoUpdate() {
    QString temp = QString::number(yCH4[0]);
    ui->label_2->setText("当前浓度:" + temp);
    ui->label_3->setText("峰值浓度:" + temp);
}

//void
//MainWindow::on_pushButton_clicked() {
//    ui->frame_5->setVisible(true);
//    timer->start();
//}

//void
//MainWindow::on_pushButton_2_clicked() {
//    ui->frame_5->setVisible(false);
//}

void
MainWindow::WidgetInit() {
    int width = this->width();
    int height = this->height();

    ui->label_title->setBaseSize(width, height*0.2);
    QSizePolicy sizePolicy = this->sizePolicy();
//    sizePolicy.setHorizontalPolicy(QSizePolicy::MinimumExpanding);
//    sizePolicy.setVerticalPolicy(QSizePolicy::Maximum);
//    ui->label_title->setSizePolicy(sizePolicy);
}

void
MainWindow::SelectPage() {
    //获取触发槽的是哪个部件所发出的信号，并获取到那个指针
    QPushButton *widget = static_cast<QPushButton*>(sender());
    if (widget == ui->pushButton)
    {
        ui->stackedWidget->setCurrentIndex(0);//根据触发的按钮来进行所要显示的QWidget
    }
    else if (widget == ui->pushButton_2)
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
    else if (widget == ui->pushButton_3)
    {
        ui->stackedWidget->setCurrentIndex(2);
    }
    else if (widget == ui->pushButton_4)
    {
        ui->stackedWidget->setCurrentIndex(3);
    }
}

void
MainWindow::AlertInit() {
    connect(ui->settingButtonUpdateCH4, &QPushButton::clicked, this, [=](){ ui->label_4->setText("报警阈值:" + QString::number(ui->doubleSpinBoxCH4->value(), 'f', 2));});
}

void
MainWindow::socketInit(uint port) {
    // 监听
    if(!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << tcpServer->errorString();
        close();
    }
    qDebug() << "listen...";
    // 检测新连接
    connect(tcpServer, &QTcpServer::newConnection, this, &MainWindow::acceptConnection);
    qDebug() << "detect...";
}

void
MainWindow::acceptConnection() {
    clientConnection = tcpServer->nextPendingConnection();
    connect(clientConnection, SIGNAL(readyRead()), this, SLOT(readMessage()));
    qDebug() << "connected";
    ui->textBrowser_2->setText("设备已连接 开始接收数据：");
}

void
MainWindow::readMessage() {
    QString str = clientConnection->readAll(); //或者 char buf[1024]; clientConnection->read(buf,1024);
    // 数据解析
    qDebug() << "new message:" + str;
    ui->textBrowser_2->append("device:" + str);
}

QString
MainWindow::getLocalIP() {
    QString IP;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    for (int i = 0; i < ipAddressesList.size(); ++i)
    {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
        ipAddressesList.at(i).toIPv4Address())
        {
            // 如果这个地址不是127.0.0.1，并且是IPv4的地址，就将其赋值给IP，并结束for循环
            IP = ipAddressesList.at(i).toString();
            break;
        }
    }
    // 如果IP为空，则将其赋值为127.0.0.1
    if (IP.isEmpty())
        IP = QHostAddress(QHostAddress::LocalHost).toString();
    return IP;
}

void
MainWindow::on_connButtonWIFI_clicked() {
    socketInit(ui->lineEditPort->text().toUInt());
    ui->labelIP->setText(getLocalIP());
}

void
MainWindow::on_disconnButtonWIFI_clicked() {
    tcpServer->close();
    ui->textBrowser_2->append("断开连接");
    Sleep(1000);
    ui->textBrowser_2->clear();
}

void
MainWindow::PositionInit() {
    ui->labelMap->setPixmap(QPixmap("./images/test.jpg"));
}

