#include "frame.h"
#include "ui_frame.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    connect(&timer_frame, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(closeCamera()));
    connect(ui->open, SIGNAL(clicked()), this, SLOT(openCamera()));
    connect(ui->renew, SIGNAL(clicked()), this, SLOT(renewPicture()));
    connect(&timer_compare, SIGNAL(timeout()), this, SLOT(compareFrame()));
    showOrgPic();
}

Frame::~Frame()
{
    delete ui;
}

void Frame::openCamera()
{
    cap.open(1);
    timer_frame.start(FPS);
    timer_compare.start(CPS);
}

void Frame::closeCamera()
{
    timer_frame.stop();
    timer_compare.stop();
    cap.release();
}

void Frame::readFrame()
{
    cap >> img;
    cvtColor(img, img, CV_BGR2RGB);
    QImage im((unsigned char *)img.data, img.cols, img.rows, QImage::Format_RGB888);
    ui->label_1->setPixmap(QPixmap::fromImage(im));
}

void Frame::showOrgPic()
{
    img_org = imread("C:\\Users\\TungHsun\\Desktop\\17_autodetect\\GITHUB\\Autotest\\pic.jpg", IMREAD_COLOR );
    QImage p((unsigned char *)img_org.data, img_org.cols, img_org.rows, QImage::Format_RGB888);
    ui->label_2->setPixmap(QPixmap::fromImage(p));
}

void Frame::renewPicture()
{
    img_org = img;
    QImage p((unsigned char *)img_org.data, img_org.cols, img_org.rows, QImage::Format_RGB888);
    ui->label_2->setPixmap(QPixmap::fromImage(p));

}

void Frame::compareFrame()
{
    double result;
    img.convertTo(img, CV_32F);
    img_org.convertTo(img_org, CV_32F);
    result = compareHist(img,img_org,CV_COMP_CORREL);
    std::cout << result << endl;
}
