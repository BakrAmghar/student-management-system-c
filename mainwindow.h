#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// ADD THESE TWO LINES HERE:
#include <QtCharts>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateUI();
    void on_btn_save_clicked();
    void on_btn_manage_clicked();
    void on_btn_search_clicked();
    void on_btn_file_save_clicked();
    void on_btn_file_load_clicked();

private:
    Ui::MainWindow *ui;
    // ADD THESE POINTERS:
    QChart *mainChart;
    QChartView *mainChartView;
    QBarSeries *mainSeries;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
};
#endif