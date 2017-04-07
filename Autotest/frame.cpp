#include "frame.h"
#include "opencv2/opencv.hpp"

Frame::Frame(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(closeCamera()));
    connect(ui->open, SIGNAL(clicked()), this, SLOT(openCamera()));
}
Frame::~Frame()
{
    delete ui;
}
void Frame::openCamera()
{
    cap.open(0);
    timer.start(FPS);
}
void Frame::closeCamera()
{
    timer.stop();
    cap.release();
}
void Frame::readFrame()
{
    cap >> img;
    cvtColor(img, img, CV_BGR2RGB);
    QImage im((unsigned char *)img.data, img.cols, img.rows, QImage::Format_RGB888);
    ui->pic->setPixmap(QPixmap::fromImage(im));
}
