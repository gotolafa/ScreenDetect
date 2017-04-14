#ifndef FRAME_H
#define FRAME_H

#include <QMainWindow>
#include <QtWidgets>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <QDialog>

using namespace cv;
#define FPS 30
#define CPS 30
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
    QTimer timer_frame;
    QTimer timer_compare;
    Mat img_vedio;
    Mat img;
    Mat img_org;


private slots:
    void openCamera();
    void closeCamera();
    void readFrame();
    //void showOrgPic();
    void compareFrame();
    void renewPicture();
    void clearLog();
    double getPSNR(const Mat& I1, const Mat& I2);
    Scalar getMSSIM( const Mat& i1, const Mat& i2);

    //QLabel *label_number;
};

#endif // FRAME_H
