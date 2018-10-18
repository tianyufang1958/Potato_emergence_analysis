#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_exit_clicked();

    void on_pushButton_binary_clicked();

    void on_pushButton_parameters_clicked();

    void on_pushButton_row_clicked();

    void on_pushButton_Win_clicked();

    void on_pushButton_sec_clicked();

    void on_pushButton_line_clicked();

    void on_pushButton_area_processing_clicked();

    void on_pushButton_row_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
