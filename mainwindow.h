#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

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
    // This matches your Save button
    void on_btn_save_clicked();

    // This matches your Manage (Update/Delete) button
    void on_btn_manage_clicked();

    // This matches your Search button
    void on_btn_search_clicked();

    // This matches the File Save/Load buttons
    void on_btn_file_save_clicked();
    void on_btn_file_load_clicked();

    // --- NEW: This matches the Live Search bar ---
    void on_input_search_id_textChanged(const QString &arg1);

    //EXPORT button
    void on_btn_export_clicked();

private:
    Ui::MainWindow *ui;

    // --- NEW: Added the filter parameter here ---
    void updateUI(QString filter = "");

    //Exporting functions:
    void exportAsPDF();
    void exportAsExcel();

    // Charting variables
    QChart *mainChart;
    QBarSeries *mainSeries;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChartView *mainChartView;
};
#endif // MAINWINDOW_H