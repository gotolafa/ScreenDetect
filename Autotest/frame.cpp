#include "frame.h"
#include "ui_frame.h"
#include "opencv/highgui.h"
#include "opencv/cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
#include <QLineEdit>
#include <QSlider>
#include <QHBoxLayout>
#include <QLabel>

#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion

#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur

using namespace std;
using namespace cv;

Mat img_result;

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
    cap.open(1);
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
    QImage p((unsigned char *)img_org.data, img_org.cols, img_org.rows, QImage::Format_RGB888);
    QImage imgScaled1;
    QImage imgScaled2;
    imgScaled1 = im.scaled(ui->label_1->size(),Qt::KeepAspectRatio);
    ui->label_1->setPixmap(QPixmap::fromImage(imgScaled1));
    imgScaled2 = p.scaled(ui->label_2->size(),Qt::KeepAspectRatio);
    ui->label_2->setPixmap(QPixmap::fromImage(imgScaled2));

    if (renew_picture == TRUE)
    {
        renew_picture = FALSE;
        img_org = img.clone();
        clearLog();
        timer_compare.start(CPS);
        triger_point = TRUE;
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
    double threshold_level = 90;
    double threshold_level2 = 20;
    QString str;
    double result1;
    Scalar result2;
    double result3;

    result1 = getPSNR(img_org,img);
    str.sprintf("%lf", result1);
    ui->showThread_2->setText(str);
    result2 = getMSSIM(img_org,img);
    result3 =((result2.val[2] * 100)+(result2.val[1] * 100)+(result2.val[0] * 100))/3;
    str.sprintf("%lf", result3);
    ui->showThread->setText(str);

    if((threshold_level < result3) && (threshold_level2 < result1))
    {
        if (triger_point == TRUE)
        {
            ui->showResult->append(str);
            str.sprintf("%lf", result1);
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

/*http://docs.opencv.org/2.4/doc/tutorials/highgui/video-input-psnr-ssim/video-input-psnr-ssim.html#videoinputpsnrmssim*/

double Frame::getPSNR(const Mat& I1, const Mat& I2)
{
    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    Scalar s = sum(s1);        // sum elements per channel

    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels

    if( sse <= 1e-10) // for small values return zero
        return 0;
    else
    {
        double mse  = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}

Scalar Frame::getMSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    Mat I1, I2;
    i1.convertTo(I1, d);            // cannot calculate on one byte large values
    i2.convertTo(I2, d);

    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);

    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);

    Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

    Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return mssim;
}
