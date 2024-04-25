
#include <cstdlib>
#include <QDebug>
#include <ctime>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QString path = QCoreApplication::applicationDirPath();
    settings = new QSettings(QString("%1/dj_rest.ini").arg(path), QSettings::IniFormat);
    int time1 = settings->value("config/restTime").toInt();
    std::cout<< "rest time: " << time1 <<std::endl;
    if (time1 > 0 && time1 < 20 * 60) {
        this->restTime = time1;
    }
    createTrayIcon();
    ui->setupUi(this);
    this->currentBackgroundColor = this->createRandomColor();
    this->setWindowTitle("hava a rest");
    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowMaximizeButtonHint|Qt::WindowMinimizeButtonHint|Qt::WindowCloseButtonHint);
}

void MainWindow::init(uint screenId){
    this->screenId = screenId;
    this->screen = QGuiApplication::screens().at(screenId);
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
            this->stopResting();
        } else {
            if (imageData.size() > 0) {
                this->setBackgroundImage();
            } else {
                this->setBackgroundColor();
            }
            QString newTimeStr = this->refreshTime();
            this->changeLabel(newTimeStr);
            this->maxWindow();
            QRect rect = this->screen->availableGeometry();
            this->setGeometry(rect);
        }
    }
}

/**
 * @brief MainWindow::stopResting
 * 结束休息
 */
void MainWindow::stopResting() {
    this->isResting = false;
    this->changeLabel("工作中.....");
    setWindowState(Qt::WindowMinimized);
    this->hide();
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
    try {
        ImageDownloader* imageDownloader = new ImageDownloader();
        this->imageData = imageDownloader->getRandomImageData(this->screenId);
        this->setBackgroundImage();
    } catch(_exception) {
        qDebug("load image error");
        this->currentBackgroundColor = this->createRandomColor();
        this->imageData = NULL;
        this->setBackgroundColor();
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
    this->show();
    setWindowState(Qt::WindowMaximized);//初始状态最大化
}

/**
 * @brief MainWindow::refreshTime
 * 更新休息倒计时
 * @return QString
 */
QString MainWindow::refreshTime() {
    QDateTime timeDate = QDateTime::currentDateTime();  // 获取当前时间
    uint nowTime = timeDate.toTime_t();   	    // 将当前时间转为时间戳
    uint leftTime = restTime - (nowTime - restTimestamp);
    // 将剩余时间转换为分钟和秒
    uint seconds = leftTime % 60;
    uint minutes = leftTime / 60;
    QString labelTxt = QString("该休息了, 站起来活动一下吧.休息剩余时间:%1m%2s").arg(minutes).arg(seconds);
    return labelTxt;
}

/**
 * @brief MainWindow::changeLabel
 * 修改label内容，休息时间内每秒更新一次时间
 */
void MainWindow::changeLabel(QString labelTxt) {
    QLabel *label = this->ui->label;   
    label->setText(labelTxt);
    label->setStyleSheet("QLabel{background-color:rgb(255,255,255);}");
    label->setMargin(10);
    label->move(20, 10);
    label->adjustSize();
}



void MainWindow::changeEvent(QEvent *event) {
    if (isMinimized()) {
        if (this->tip_hide == false) {
            this->hide();
            systemIcon->showMessage("have a rest", "后台运行", QSystemTrayIcon::MessageIcon::Information, 300);
            this->tip_hide = true;
        }
    }
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
 * @brief MainWindow::closeEvent
 * 拦截close事件
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event) {
    setWindowState(Qt::WindowMinimized);
    event->ignore();
}

/**
 * @brief MainWindow::createTrayIcon
 * 创建托盘
 */
void MainWindow::createTrayIcon() {
    systemIcon = new QSystemTrayIcon(this);

    systemIcon->setToolTip("have a rest");     // 设置提示语
    systemIcon->setIcon(QIcon(":/icon.png"));   // 设置图标

    // 增加托盘菜单
    QMenu *menu = new QMenu();
    QAction *closeAction = new QAction("关闭");
    QAction *closeOneTimeAction = new QAction("跳过当前休息");
    menu->addAction(closeAction);
    menu->addAction(closeOneTimeAction);
    systemIcon->setContextMenu(menu);
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(closeOneTimeAction, SIGNAL(triggered(bool)), this, SLOT(closeOneTime()));
    systemIcon->show();
    // 关联点击拖盘事件
    connect(systemIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this, SLOT(on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason)));
    // 要在show之后调用
//    systemIcon->showMessage("have a rest", "程序正在后台运行",QSystemTrayIcon::MessageIcon::Information,500);
}

/**
 * @brief MainWindow::closeOneTime
 * 关闭本次休息
 * 当前仅对对应的窗口有效，如果又两个显示器则需要进行两次结束
 */
void MainWindow::closeOneTime() {
    this->stopResting();
}

/**
 * @brief MainWindow::close
 * 关闭程序
 */
void MainWindow::close() {
    QApplication::quit();
}

/**
 * @brief MainWindow::on_activatedSysTrayIcon
 * 点击托盘
 * @param reason
 */
void MainWindow::on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        this->show();
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

