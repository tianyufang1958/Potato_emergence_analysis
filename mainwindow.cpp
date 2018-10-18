#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <QFileDialog>
#include <iostream>
#include <fstream>

#include <QTextStream>

using namespace cv;
using namespace std;

int idx = 0;
int sec = 1;
int row = 1;
int win = 20;
bool win_change = false;
Mat img_ori;
Mat dst, dst_line;
Mat thresh_thresh_0;
Mat thresh_thresh;

void CallBackFunc_area(int event, int x, int y, int flags, void* userdata)
{

    if  ( event == EVENT_RBUTTONDOWN )
       {
           int height = 1700;    //1700   // 1000 top // 1200
           int width = 150;
           int total_num=0;
           int area = 0;
           int total_area=0;

           ofstream myfile;
           myfile.open ("cv_raw.txt", ios::app);

           cout << "Left top (" << x << ", " << y << ")" << endl;

           rectangle(img_ori, Point(x,y), Point(x+width, y+height), Scalar(255,0,0), 3, 8);

           Mat img_crop = img_ori(Rect(x,y,width, height));
           Mat thresh_crop = thresh_thresh(Rect(x,y,width, height));

           Mat thresh_crop_copy = thresh_crop.clone();
           vector<vector<Point> > contours;
           vector<Vec4i> hierarchy;
           findContours(thresh_crop, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

           for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end(); )
               {
                   if (it->size() < 35  )
                       it=contours.erase(it);
                   else
                       ++it;
               }

         //  drawContours(img_crop, contours, -1, Scalar(255, 255, 255), 2);

           vector<vector<Point> > contours_poly( contours.size() );
           vector<Rect> boundRect( contours.size() );

           for( int i = 0; i < contours.size(); i++ )
              {
               approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
               boundRect[i] = boundingRect( Mat(contours_poly[i]) );
              }

           for (int i = 0; i < boundRect.size(); i++)
           {
               int height = boundRect[i].br().y - boundRect[i].tl().y;
               int num;
               if (height <= 40)
               {
                   num = 1;
               }
               else if (height > 40 && height <= 80)
               {
                   num = 2;
               }
               else if (height > 80 && height <= 120)
               {
                   num = 3;
               }
               else if (height > 120 && height <= 160)
               {
                   num = 4;
               }
               else if (height > 160 && height <= 200)
               {
                   num = 5;
               }
               else if (height > 200 && height <= 240)
               {
                   num = 6;
               }
               else if (height > 240 && height <= 280)
               {
                   num = 7;
               }
               else if (height > 280 && height <= 320)
               {
                   num = 8;
               }
               else if (height > 320 && height <= 360)
               {
                   num = 9;
               }

               else if (height > 360 && height <= 400)
               {
                   num = 10;
               }
               else if (height > 400 && height <= 440)
               {
                   num = 11;
               }
               else if (height > 440 && height <= 480)
               {
                   num = 12;
               }
               else if (height > 480 && height <= 520)
               {
                   num = 13;
               }
               else if (height > 520 && height <= 560)
               {
                   num = 14;
               }

               stringstream ss;
               ss << num;
               string str = ss.str();


               putText(img_crop, str, Point(boundRect[i].br().x-25, boundRect[i].br().y+15), FONT_HERSHEY_COMPLEX_SMALL, 1.0, Scalar(0,0,255), 2, 8);

               rectangle(img_crop, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 0, 0), 2);
               total_num = total_num + num;

               area = contourArea(contours[i]);
               total_area = total_area + area;
               cout<<area<<endl;

               myfile<<num<<" "<<area<<" "<<endl;
           }
          // myfile<<endl;
           cout<<"Plant number = "<<total_num<<endl;

           double per = double(total_area) / double(1200 * 950);
           cout<<"total area = "<< total_area<<".  "<<"Percentage = "<<per<<endl;

           imshow("Original", img_ori);
          // namedWindow("Crop image", WINDOW_NORMAL);
          // imshow("Crop image", img_crop);

          // imwrite("emergence.jpg", img_ori);

           waitKey(0);
       }

}

void CallBackFunc_line(int event, int x, int y, int flags, void* userdata)
{

    if  ( event == EVENT_RBUTTONDOWN )
    {
        int line_length = 1540;
        int interval = 34;

        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        line(dst_line, Point(x,y), Point (x,y+line_length), Scalar(0,0,255), 3, 8);
        for (int i = 0; i < 45; i++)
        {
            circle(dst_line, Point(x, y+i*interval), 5, Scalar(255,0,0), CV_FILLED, 8);
        }

        imshow("Original", dst_line);

        waitKey(0);
    }

}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    ofstream myfile;
    myfile.open ("f5.txt", ios::app);

    if  ( event == EVENT_RBUTTONDOWN )
    {
        idx++;
        int sum_canopy = 0;

        rectangle(dst, Point(x-win,y-win), Point(x+win, y+win), Scalar(255, 0, 0), 1, 8);

       // cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        for (int i = win*-1; i <= win; i++)
        {
            for (int j = win*-1; j < win; j++)
            {
                if (thresh_thresh.at<uchar>(y+i,x+j) > 200)
                {
                    sum_canopy++;
                }
            }
        }
        imshow("Original", dst);

        myfile <<row<<" "<<sec<<" "<<idx<<" "<< sum_canopy<<endl;
        cout<<row<<" "<<sec<<" "<<idx<<" "<< sum_canopy<<endl;
        waitKey(0);
    }

}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_binary_clicked()
{
    QString selectedFilter;
    QString fileName_s = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("JPEG (*.png)"), &selectedFilter);

    String fileName = fileName_s.toStdString();
    Mat img_ori = imread(fileName);
    vector<Mat> RGB;
    split(img_ori, RGB);
    Mat R = RGB[2];
    Mat G = RGB[1];
    Mat B = RGB[0];

    Mat gray = 2*G - R - B;
    Mat thresh;
   // threshold(gray, thresh, 22, 255, THRESH_BINARY);
    threshold(gray, thresh, 0, 255, THRESH_OTSU);
    Mat thresh_rotate;
    Point2f src_center1(thresh.cols/2.0F, thresh.rows/2.0F);
    Mat rot_mat1 = getRotationMatrix2D(src_center1, 19, 1.0);
    warpAffine(thresh, thresh_rotate, rot_mat1, thresh.size());

    Mat thresh_copy = thresh_rotate.clone();
    vector<vector<Point> > contours;
    findContours(thresh_copy, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);

    for (vector<vector<Point> >::iterator it = contours.begin(); it!=contours.end(); )
    {
        if (it->size() > 30  )
        {
            it=contours.erase(it);
        }
        else
            ++it;
    }

    drawContours(thresh_rotate, contours, -1, Scalar(0), CV_FILLED);

    Point2f src_center(img_ori.cols/2.0F, img_ori.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, 19, 1.0);
    warpAffine(img_ori, dst, rot_mat, img_ori.size());

    imwrite("5.png", dst);
    imwrite("5_thresh.png", thresh_rotate);

}

void MainWindow::on_pushButton_parameters_clicked()
{
    QString selectedFilter;
    QString fileName_s = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("JPEG (*.png)"), &selectedFilter);
    String fileName = fileName_s.toStdString();

    size_t position = fileName.find(".");
    string extractName = (string::npos == position)? fileName : fileName.substr(0, position);

    String fileName_thresh = extractName + "_thresh.png";

    cout<<fileName_thresh<<endl;

    img_ori = imread(fileName);

    Point2f src_center(img_ori.cols/2.0F, img_ori.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, 19, 1.0);
    warpAffine(img_ori, dst, rot_mat, img_ori.size());

    Mat thresh_ori = imread(fileName_thresh);
    Mat thresh_ori_rotate;
    Point2f src_center1(thresh_ori.cols/2.0F, thresh_ori.rows/2.0F);
    Mat rot_mat1 = getRotationMatrix2D(src_center1, 19, 1.0);
    warpAffine(thresh_ori, thresh_ori_rotate, rot_mat1, thresh_ori.size());

    Mat thresh_gray;
    cvtColor(thresh_ori_rotate, thresh_gray, CV_RGB2GRAY);
    threshold(thresh_gray, thresh_thresh, 50, 255, CV_THRESH_BINARY);

    namedWindow("Original", WINDOW_NORMAL);
   // namedWindow("Display", WINDOW_NORMAL);

    setMouseCallback("Original", CallBackFunc, NULL);

    imshow("Original", dst);
   // imshow("Display", thresh_thresh);

    waitKey(0);
}

void MainWindow::on_pushButton_row_clicked()
{
    row++;
    idx = 0;
    sec = 1;
}

void MainWindow::on_pushButton_Win_clicked()
{
    if (win_change == false)
    {
        win = 14;
        win_change = true;
        ui->label_win_size->setText(QString::number(win));
    }

    else
    {
        win = 20;
        win_change = false;
        ui->label_win_size->setText(QString::number(win));
    }
}

void MainWindow::on_pushButton_sec_clicked()
{
    sec++;
    idx = 1;
}

void MainWindow::on_pushButton_line_clicked()
{
    QString selectedFilter;
    QString fileName_s = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("JPEG (*.png)"), &selectedFilter);
    String fileName = fileName_s.toStdString();

    size_t position = fileName.find(".");
    string extractName = (string::npos == position)? fileName : fileName.substr(0, position);

    img_ori = imread(fileName);

    Point2f src_center(img_ori.cols/2.0F, img_ori.rows/2.0F);
    Mat rot_mat = getRotationMatrix2D(src_center, 0, 1.0);
    warpAffine(img_ori, dst_line, rot_mat, img_ori.size());


    namedWindow("Original", WINDOW_NORMAL);

    setMouseCallback("Original", CallBackFunc_line, NULL);

    imshow("Original", dst_line);

    waitKey(0);
}

void MainWindow::on_pushButton_area_processing_clicked()
{
    QString selectedFilter;
    QString fileName_s = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("JPEG (*.png)"), &selectedFilter);
    String fileName = fileName_s.toStdString();

    size_t position = fileName.find(".");
    string extractName = (string::npos == position)? fileName : fileName.substr(0, position);

    String fileName_thresh = extractName + "_thresh.png";

    cout<<fileName_thresh<<endl;

    img_ori = imread(fileName);

    Mat thresh_ori = imread(fileName_thresh);
    Mat thresh_gray;
    cvtColor(thresh_ori, thresh_gray, CV_RGB2GRAY);
    threshold(thresh_gray, thresh_thresh, 50, 255, CV_THRESH_BINARY);

    namedWindow("Original", WINDOW_NORMAL);

    setMouseCallback("Original", CallBackFunc_area, NULL);

    imshow("Original", img_ori);

    waitKey(0);

}

void MainWindow::on_pushButton_row_2_clicked()
{
    QString selectedFilter;
    QString fileName_s = QFileDialog::getOpenFileName(this, tr("Open"), QDir::currentPath(), tr("JPEG (*.png)"), &selectedFilter);

    String fileName = fileName_s.toStdString();
    Mat img_ori = imread(fileName);
  /*  vector<Mat> RGB;
    split(img_ori, RGB);
    Mat R = RGB[2];
    Mat G = RGB[1];
    Mat B = RGB[0];

    Mat gray = 2*G - R - B;
    Mat thresh;
   // threshold(gray, thresh, 22, 255, THRESH_BINARY);
    threshold(gray, thresh, 25, 255, THRESH_BINARY);*/

    Mat dst, cdst;
    Canny(img_ori, dst, 50, 200, 3);
    cvtColor(dst, cdst, CV_GRAY2BGR);

    vector<Vec4i> lines;
    HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
      Vec4i l = lines[i];
      line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
    }

    namedWindow("Thresh", WINDOW_NORMAL);
    imshow("Thresh", cdst);
    waitKey(0);
}
