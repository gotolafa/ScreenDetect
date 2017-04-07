#ifndef FRAME_H
#define FRAME_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>

using namespace cv;
#define FPS 30
namespace Ui {
class Frame;
}

class Frame : public QMainWindow
{
    Q_OBJECT

public:
    explicit Frame(QWidget *parent = 0);
    ~Frame();

private:
    Ui::Frame *ui;
    VideoCapture cap;
    QTimer timer;
    Mat img;
private slots:
    void openCamera();
    void closeCamera();
    void readFrame();
};

#endif // FRAME_H
