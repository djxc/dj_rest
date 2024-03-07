#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QBuffer>
#include<QScreen>
#include <QDesktopWidget>
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QEvent>
#include <QSettings>

#include "imagedownloader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    MainWindow(QWidget *parent = nullptr);
    void resizeEvent(QResizeEvent *re);
    void changeEvent(QEvent *event);
    void closeEvent(QCloseEvent *event);
    void init(uint screenId);

    ~MainWindow();


private slots:
    void timerFunc();
    void resting();
    void close();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);

private:
    uint restTime = 10;//60 * 5;  // 休息时间s
    int workTime = 1000 * 60 * 60;  // 工作时间毫秒
    bool isInit = false;
    bool isResting = false;
    uint restTimestamp;
    QByteArray imageData;
    QColor currentBackgroundColor;
    Ui::MainWindow *ui;
    QTimer* _Timer;        
    QScreen* screen;
    QSystemTrayIcon* systemIcon;
    bool tip_hide = false;
    QSettings *settings;

    void maxWindow();
    QColor createRandomColor();
    void setBackgroundColor();
    void setBackgroundImage();
    void changeLabel(QString labelTxt);
    QString refreshTime();
    void createTrayIcon();

};
#endif // MAINWINDOW_H
