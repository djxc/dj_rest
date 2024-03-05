
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
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint);
    setWindowState(Qt::WindowMinimized);
    this->init();
}

void MainWindow::init(){
    _Timer = new QTimer(this);
    _Timer->setInterval(workTime);	//一个小时
//    _Timer->setInterval(1000 * 30);	//30秒
    connect(_Timer, SIGNAL(timeout()), SLOT(timerFunc()));
    _Timer->start();//启动计时器    
//    _Timer->stop();//停止计时器

    QTimer* sub_timer;
    sub_timer = new QTimer(this);
    sub_timer->setInterval(1000);
    connect(sub_timer, SIGNAL(timeout()), SLOT(resting()));
    sub_timer->start();
}

/**
 * @brief MainWindow::resting
 * 休息过程中持续弹出对话框干扰用户
 */
void MainWindow::resting() {
    if (this->isResting) {
        QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
        uint now = timeDate.toTime_t();   	    // 将当前时间转为时间戳
        if ((now - this->restTimestamp) > restTime) {
            this->isResting = false;
            setWindowState(Qt::WindowMinimized);
        }
        if (this->isResting) {
            if (imageData.size() > 0) {
                this->setBackgroundImage();
            } else {
                this->setBackgroundColor();
            }
            this->changeLabel();
            this->maxWindow();
        }
    }
}

/**
 * @brief MainWindow::timerFunc
 * 休息时触发函数
 * 1、记录当前休息的时间
 * 2、随机获取图片或颜色作为背景
 */
void MainWindow::timerFunc()
{       
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    this->restTimestamp = timeDate.toTime_t();   	    // 将当前时间转为时间戳
    // 随机选择是背景图片还是单一颜色,如果获取web图片失败则采用颜色背景
    int r = rand()%100;
    qDebug("run time event! background type: %d; now: %d", r, this->restTimestamp);
    this->currentBackgroundColor = this->createRandomColor();
    if (r > 5) {
        ImageDownloader* imageDownloader = new ImageDownloader();
        this->imageData = imageDownloader->getRandomImageData();
        try {
            this->setBackgroundImage();
        } catch(_exception) {
            qDebug("load image error");    
            this->setBackgroundColor();
        }
    } else {
        this->setBackgroundColor();
        this->imageData = NULL;
    }
    this->isResting = true;
}

/**
 * @brief MainWindow::setBackgroundColor
 * 设置背景颜色
 */
void MainWindow::setBackgroundColor() {
    //通过QPalette设置背景颜色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, this->currentBackgroundColor);
    this->setPalette(palette);
}

/**
 * @brief MainWindow::setBackgroundImage
 * 设置背景图片
 */
void MainWindow::setBackgroundImage() {
    //通过QPalette设置背景图片
    QPalette pa(this->palette());
    QImage img = QImage();
    img.loadFromData(imageData);
    img = img.scaled(this->size());
    QBrush pic = QBrush(img);
    pa.setBrush(QPalette::Window, pic);
    this->setAutoFillBackground(true);
    this->setPalette(pa);   
}

/**
 * @brief MainWindow::maxWindow
 * 最大化窗口
 */
void MainWindow::maxWindow() {
    this->setFocus();
    setWindowState(Qt::WindowMaximized);//初始状态最大化
}

/**
 * @brief MainWindow::changeLabel
 * 修改label内容，休息时间内每秒更新一次时间
 */
void MainWindow::changeLabel() {
    QLabel *label = this->ui->label;
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    uint nowTime = timeDate.toTime_t();   	    // 将当前时间转为时间戳
    uint leftTime = restTime - (nowTime - restTimestamp);
    // 将剩余时间转换为分钟和秒
    uint seconds = leftTime % 60;
    uint minutes = leftTime / 60;
    QString labelTxt = QString("该休息了, 站起来活动一下吧.休息剩余时间:%1m%2s").arg(minutes).arg(seconds);
    label->setText(labelTxt);
    label->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    label->setMargin(10);
    label->move(20, 10);
    label->adjustSize();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    if (this->isInit) {
        if (imageData.size() > 0) {
            setBackgroundImage();
        } else {
            setBackgroundColor();
        }
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

