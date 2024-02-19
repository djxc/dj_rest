#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QBuffer>

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
    void init();

    ~MainWindow();

private slots:
    void timerFunc();
    void resting();

private:
    uint restTime = 60 * 5;  // 休息时间s
    int workTime = 1000 * 60 * 60;  // 工作时间毫秒
    bool isInit = false;
    bool isResting = false;
    uint restTimestamp;
    QByteArray imageData;
    Ui::MainWindow *ui;
    QTimer* _Timer;

    void maxWindow();
    QColor createRandomColor();
    void setBackgroundColor();
    void setBackgroundImage();

};
#endif // MAINWINDOW_H
