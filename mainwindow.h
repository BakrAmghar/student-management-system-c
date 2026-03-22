#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QLabel>
#include <QVariantAnimation>
#include <QSoundEffect>

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
    void on_btn_save_clicked();
    void on_btn_manage_clicked();
    void on_btn_search_clicked();
    void on_btn_file_save_clicked();
    void on_btn_file_load_clicked();
    void on_input_search_id_textChanged(const QString &arg1);
    void on_btn_export_clicked();
    void on_btn_sort_id_clicked();
    void on_btn_sort_name_clicked();
    void on_btn_sort_grade_clicked();
    void on_btn_sort_execute_clicked();
    void on_btn_theme_toggle_clicked();

private:
    Ui::MainWindow *ui;
    void updateUI(QString filter = "");
    void exportAsPDF();
    void exportAsExcel();

    void shakeWidget(QWidget *widget);
    void pulseWidget(QWidget *widget);
    void showToast(QString message);
    void startTicker(int start, int end);

    // CRITICAL: Initialize to nullptr to prevent SIGSEGV in applyTheme()
    QLabel *toastLabel = nullptr;
    int lastCount = 0;
    int currentSortMode;

    QSoundEffect *soundSuccess = nullptr;
    QSoundEffect *soundError = nullptr;

    void addToLog(QString message);

    bool isDarkMode = true;
    void applyTheme();

    QChart *mainChart = nullptr;
    QBarSeries *mainSeries = nullptr;
    QBarCategoryAxis *axisX = nullptr;
    QValueAxis *axisY = nullptr;
    QChartView *mainChartView = nullptr;
};
#endif