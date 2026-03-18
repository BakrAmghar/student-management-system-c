#include "mainwindow.h"
#include "./ui_mainwindow.h"

// IMPORTANT: Ensure these headers are in your project
extern "C" {
#include "etudiant.h"
#include "file.h"
}

#include <QDoubleValidator>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>

// --- ADDED CHART HEADERS ---
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Limit grade input to 0.00 - 20.00
    QDoubleValidator *gradeValidator = new QDoubleValidator(0.0, 20.0, 2, this);
    gradeValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->input_add_grade->setValidator(gradeValidator);

    // --- 1. INITIALIZE THE GRAPH ---
    mainChart = new QChart();
    mainSeries = new QBarSeries();
    mainChart->addSeries(mainSeries);
    mainChart->setTitle("📊 Live Grade Distribution");
    mainChart->setAnimationOptions(QChart::SeriesAnimations);

    axisX = new QBarCategoryAxis();
    QStringList categories;
    categories << "<10" << "10-12" << "12-16" << "16-20";
    axisX->append(categories);
    mainChart->addAxis(axisX, Qt::AlignBottom);
    mainSeries->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    mainChart->addAxis(axisY, Qt::AlignLeft);
    mainSeries->attachAxis(axisY);

    // Put the chart inside your chartContainer widget
    mainChartView = new QChartView(mainChart, ui->chartContainer);
    mainChartView->setRenderHint(QPainter::Antialiasing);

    // Manual Resize to match the container size you set in UI Designer
    mainChartView->resize(ui->chartContainer->size());

    updateUI();
}

MainWindow::~MainWindow() {
    delete ui;
}

// DRAWS THE LIST AND THE GRAPH ON SCREEN
void MainWindow::updateUI() {
    ui->list_students->clear();
    ui->list_stats->clear();

    etudiant *temp = head;
    int count = 0;
    float sum = 0;

    // Buckets for the graph
    int fail = 0, pass = 0, good = 0, excel = 0;

    while(temp != nullptr) {
        QString info = QString("ID: %1 | Name: %2 | Grade: %3")
        .arg(temp->id)
            .arg(temp->nom)
            .arg(temp->moyenne);
        ui->list_students->addItem(info);

        // Logic for Graph Buckets
        if (temp->moyenne < 10) fail++;
        else if (temp->moyenne < 12) pass++;
        else if (temp->moyenne < 16) good++;
        else excel++;

        sum += temp->moyenne;
        count++;
        temp = temp->next;
    }

    ui->list_stats->addItem("--- STATS ---");
    ui->list_stats->addItem(QString("Total Students: %1").arg(count));
    if(count > 0) {
        ui->list_stats->addItem(QString("Class Average: %1").arg(sum/count));
    }

    // --- 2. REFRESH THE GRAPH BARS ---
    mainSeries->clear();
    QBarSet *set = new QBarSet("Students");
    *set << fail << pass << good << excel;
    set->setColor(QColor("#3498db")); // Nice EMSI Blue
    mainSeries->append(set);

    // Auto-adjust the Y axis so it looks good
    int maxStudents = std::max({fail, pass, good, excel});
    axisY->setRange(0, maxStudents + 1);
}

// ADD STUDENT BUTTON
void MainWindow::on_btn_save_clicked() {
    QString id = ui->input_add_id->text();
    QString name = ui->input_add_name->text();
    QString gradeStr = ui->input_add_grade->text();
    float grade = gradeStr.toFloat();

    if(id.isEmpty() || name.isEmpty() || gradeStr.isEmpty()) {
        ui->list_stats->addItem("⚠️ Error: Missing information!");
        return;
    }

    InsertionQueue(id.toLocal8Bit().data(), grade, name.toLocal8Bit().data());
    updateUI();

    ui->input_add_id->clear();
    ui->input_add_name->clear();
    ui->input_add_grade->clear();
    ui->list_stats->addItem("✅ Student added to memory!");
}

// MANAGE (MODIFY/DELETE) BUTTON
void MainWindow::on_btn_manage_clicked() {
    QString theId = ui->input_manage_id->text();
    if(theId.isEmpty()) return;

    etudiant* res = RechercheID_GUI(theId.toLocal8Bit().data());
    if(!res) {
        ui->list_stats->addItem("⚠️ Student not found!");
        return;
    }

    struct TempData {
        QString id;
        QString name;
        float grade;
    };
    TempData *data = new TempData{theId, QString::fromLocal8Bit(res->nom), res->moyenne};

    QDialog popup(this);
    popup.setWindowTitle("Manage Student");
    popup.setStyleSheet("background-color: #1e272e; color: white;");
    QVBoxLayout *layout = new QVBoxLayout(&popup);

    QPushButton *btnMod = new QPushButton("Update Information ✍️");
    QPushButton *btnDel = new QPushButton("Delete Student 🗑️");
    layout->addWidget(new QLabel("⚙️ Managing ID: " + theId));
    layout->addWidget(btnMod);
    layout->addWidget(btnDel);

    QString stl = "QPushButton { background-color: #34495e; color: white; padding: 10px; border-radius: 5px; }";
    btnMod->setStyleSheet(stl + "QPushButton { background-color: #e67e22; }");
    btnDel->setStyleSheet(stl + "QPushButton { background-color: #c0392b; }");

    connect(btnMod, &QPushButton::clicked, [=, &popup, &layout]() {
        btnMod->hide(); btnDel->hide();
        QPushButton *chId = new QPushButton("Change ID (" + data->id + ")");
        QPushButton *chName = new QPushButton("Change Name (" + data->name + ")");
        QPushButton *chGrade = new QPushButton("Change Grade (" + QString::number(data->grade) + ")");
        QPushButton *confirm = new QPushButton("SAVE ALL CHANGES ✅");

        layout->addWidget(chId); layout->addWidget(chName); layout->addWidget(chGrade); layout->addWidget(confirm);

        connect(chId, &QPushButton::clicked, [=, &popup]() {
            bool ok; QString val = QInputDialog::getText(&popup, "ID", "New ID:", QLineEdit::Normal, data->id, &ok);
            if (ok && !val.isEmpty()) { data->id = val; }
        });
        connect(chName, &QPushButton::clicked, [=, &popup]() {
            bool ok; QString val = QInputDialog::getText(&popup, "Name", "New Name:", QLineEdit::Normal, data->name, &ok);
            if (ok && !val.isEmpty()) { data->name = val; }
        });
        connect(chGrade, &QPushButton::clicked, [=, &popup]() {
            bool ok; double val = QInputDialog::getDouble(&popup, "Grade", "New Grade:", (double)data->grade, 0, 20, 2, &ok);
            if (ok) { data->grade = (float)val; }
        });
        connect(confirm, &QPushButton::clicked, [=, &popup]() {
            Modifier_GUI(theId.toLocal8Bit().data(), data->id.toLocal8Bit().data(), data->name.toLocal8Bit().data(), data->grade);
            popup.accept();
        });
    });

    connect(btnDel, &QPushButton::clicked, [&]() {
        Supprimer_GUI(theId.toLocal8Bit().data());
        popup.accept();
    });

    if (popup.exec() == QDialog::Accepted) {
        updateUI();
        SauvegarderListe();
        delete data;
    } else {
        delete data;
    }
}

// SEARCH BUTTON
void MainWindow::on_btn_search_clicked() {
    QString sid = ui->input_search_id->text();
    if(sid.isEmpty()) return;
    etudiant* res = RechercheID_GUI(sid.toLocal8Bit().data());
    if(res) {
        for(int i = 0; i < ui->list_students->count(); ++i) {
            if(ui->list_students->item(i)->text().contains(sid)) {
                ui->list_students->item(i)->setSelected(true);
                ui->list_students->scrollToItem(ui->list_students->item(i));
                return;
            }
        }
    }
}

// SAVE TO FILE BUTTON
void MainWindow::on_btn_file_save_clicked() {
    SauvegarderListe();
    ui->list_stats->addItem("✅ System: Data Saved to etudiants.txt!");
}

// LOAD FROM FILE BUTTON
void MainWindow::on_btn_file_load_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Load",
                                                              "⚠️Replace current list with etudiants.txt?",
                                                              QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        LoadEtudiants();
        updateUI();

        if (ui->list_students->count() == 0) {
            ui->list_stats->addItem("📂 File empty or not found in build folder.");
        } else {
            ui->list_stats->addItem("✅ Loaded successfully!");
        }
    }
}