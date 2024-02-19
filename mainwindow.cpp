
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
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowMinimized);
    this->init();
}

void MainWindow::init(){
    _Timer = new QTimer(this);
    _Timer->setInterval(workTime);	//一个小时
//    _Timer->setInterval(1000 * 30);	//10秒
    connect(_Timer, SIGNAL(timeout()), SLOT(timerFunc()));
    _Timer->start();//启动计时器    
//    _Timer->stop();//停止计时器

    QTimer* sub_timer;
    sub_timer = new QTimer(this);
    sub_timer->setInterval(1000 * 2);
    connect(sub_timer, SIGNAL(timeout()), SLOT(resting()));
    sub_timer->start();
}

void MainWindow::resting() {
    if (this->isResting) {
        QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
        uint now = timeDate.toTime_t();   	    // 将当前时间转为时间戳

        if ((now - this->restTimestamp) > restTime) {
            this->isResting = false;
        }
        if (imageData.size() > 0) {
            this->setBackgroundImage();
        } else {
            this->setBackgroundColor();
        }
    }
}

/**
 * @brief MainWindow::timerFunc
 * 将window最大化，然后通过QPalette设置背景颜色
 */
void MainWindow::timerFunc()
{   
    this->isResting = true;
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    this->restTimestamp = timeDate.toTime_t();   	    // 将当前时间转为时间戳
    // 随机选择是背景图片还是单一颜色,如果获取web图片失败则采用颜色背景
    int r = rand()%100;
    qDebug("run time event! background type: %d; now: %d", r, this->restTimestamp);
    if (r > 5) {
        ImageDownloader* imageDownloader = new ImageDownloader();
        QByteArray imageData = imageDownloader->getRandomImageData();
        this->imageData = imageData;
        this->setBackgroundImage();
    } else {
        this->setBackgroundColor();
        this->imageData = NULL;
    }

}

void MainWindow::setBackgroundColor() {
    //通过QPalette设置背景颜色
    QPalette palette(this->palette());
    QColor qcolor = this->createRandomColor();
    palette.setColor(QPalette::Background, qcolor);
    this->setPalette(palette);
    this->maxWindow();
}


void MainWindow::setBackgroundImage() {

    //通过QPalette设置背景图片
    QPalette pa(this->palette());
    QImage img = QImage();
    img.loadFromData(imageData);
    img = img.scaled(this->size());
    QBrush *pic = new QBrush(img);
    pa.setBrush(QPalette::Window, *pic);
    this->setAutoFillBackground(true);
    this->setPalette(pa);
    this->maxWindow();
}

void MainWindow::maxWindow() {
    setWindowState(Qt::WindowMaximized);//初始状态最大化
    this->ui->label->setText("休息一下吧");
    this->setFocus();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (this->isInit) {
        setBackgroundImage();
    } else {
        this->isInit = true;
    }
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

