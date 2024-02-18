
#include <cstdlib>
#include <QDebug>
#include <ctime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("hava a rest");
    setWindowState(Qt::WindowMinimized);
    this->init();
}

void MainWindow::init(){
    _Timer = new QTimer(this);
    _Timer->setInterval(1000 * 60 * 60);	//一个小时
    connect(_Timer, &QTimer::timeout, this, [=](){
        this->timerFunc();
    });
    _Timer->start();//启动计时器
//    _Timer->stop();//停止计时器
}

/**
 * @brief MainWindow::timerFunc
 * 将window最大化，然后通过QPalette设置背景颜色
 */
void MainWindow::timerFunc()
{
    qDebug("run time event!");
    setWindowState(Qt::WindowMaximized);//初始状态最大化

    this->ui->label->setText("休息一下吧");
    //通过QPalette设置背景颜色
    QPalette palette(this->palette());
    QColor qcolor = this->createRandomColor();
    palette.setColor(QPalette::Background, qcolor);
    this->setPalette(palette);
}


/**
 * @brief MainWindow::createRandomColor
 * 创建随机颜色
 * @return
 */
QColor MainWindow::createRandomColor() {
    int max = 255;
    srand(time(NULL));
    int r = rand()%max;
    int g = rand()%max;
    int b = rand()%max;
    QColor qcolor;
    qcolor.setRgb(r, g, b);
    return qcolor;
}

MainWindow::~MainWindow()
{
    delete ui;
}

