#include "frame.h"
#include "ui_frame.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>

using namespace std;
using namespace cv;

bool triger_point = FALSE;
bool renew_picture = FALSE;

Frame::Frame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);
    connect(&timer_frame, SIGNAL(timeout()), this, SLOT(readFrame()));
    connect(ui->close, SIGNAL(clicked()), this, SLOT(closeCamera()));
    connect(ui->open, SIGNAL(clicked()), this, SLOT(openCamera()));
    connect(ui->renew, SIGNAL(clicked()), this, SLOT(renewPicture()));
    connect(ui->clear, SIGNAL(clicked()), this, SLOT(clearLog()));
    connect(&timer_compare, SIGNAL(timeout()), this, SLOT(compareFrame()));
}

Frame::~Frame()
{
    delete ui;
}

void Frame::openCamera()
{
    cap.open(0);
    timer_frame.start(FPS);
    renew_picture = FALSE;
}

void Frame::closeCamera()
{
    renew_picture = FALSE;
    timer_frame.stop();
    timer_compare.stop();
    ui->showResult->clear();
    ui->showThread->clear();
    cap.release();
}

void Frame::readFrame()
{
    cap >> img;
    cvtColor(img, img, CV_BGR2RGB);
    QImage im((unsigned char *)img.data, img.cols, img.rows, QImage::Format_RGB888);
    QImage imgScaled ;
    imgScaled = im.scaled(ui->label_1->size(),Qt::KeepAspectRatio);
    ui->label_1->setPixmap(QPixmap::fromImage(imgScaled));

    if (renew_picture == TRUE)
    {
        renew_picture = FALSE;
        img_org = img;
        QImage p((unsigned char *)img_org.data, img_org.cols, img_org.rows, QImage::Format_RGB888);
        imgScaled = p.scaled(ui->label_2->size(),Qt::KeepAspectRatio);
        ui->label_2->setPixmap(QPixmap::fromImage(imgScaled));
        clearLog();
        triger_point = TRUE;
        timer_compare.start(CPS);
    }
}

void Frame::renewPicture()
{
    timer_compare.stop();
    renew_picture = TRUE;
}

void Frame::compareFrame()
{
    double result;
    double threshold_level = -1.02;
    QString str;

    img.convertTo(img, CV_32F);
    img_org.convertTo(img_org, CV_32F);
    result  = compareHist(img,img_org,CV_COMP_CORREL);
    str.sprintf("%lf", result);

    ui->showThread->setText(str);

    if(threshold_level < result)
    {
        if (triger_point == TRUE)
        {
            ui->showResult->append(str);
            QDateTime time = QDateTime::currentDateTime();
            str = "Matched " + time.toString("yy-MM-dd hh:mm:ss");
            ui->showResult->append(str);
            triger_point = FALSE;
        }
    }
    else
    {
        if (triger_point == FALSE)
        {
            triger_point = TRUE;
        }
    }
}

void Frame::clearLog()
{
    ui->showResult->clear();
    triger_point = TRUE;
}
