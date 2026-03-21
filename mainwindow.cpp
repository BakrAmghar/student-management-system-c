#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
#include <QFileDialog>
#include <QTextDocument>
#include <QPrinter>
#include <QFile>
#include <QTextStream>
#include <QtCharts>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QPropertyAnimation>
#include <QEasingCurve>
#include <QTimer>
#include <QApplication>
#include <QVariantAnimation>
#include <QDateTime>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    this->setStyleSheet(
        /* Main Background */
        "QMainWindow { background-color: #1a1a2e; } "
        "QWidget#centralwidget { background-color: #1a1a2e; } "

        /* Group Boxes (Updated style with a subtle glow) */
        "QGroupBox { "
        "   color: #ffffff; font-weight: bold; "
        "   border: 1px solid #34495e; border-radius: 12px; "
        "   margin-top: 15px; padding-top: 15px; background-color: #16213e; "
        "} "

        /* Input Fields (Kept white background but added better borders) */
        "QLineEdit { "
        "   background-color: #ffffff; color: #2c3e50; "
        "   border: 2px solid #0f3460; border-radius: 6px; "
        "   padding: 8px; font-size: 13px; "
        "} "
        "QLineEdit:focus { "
        "   border: 2px solid #3498db; background-color: #f7f9fb; "
        "} "

        /* Labels */
        "QLabel { color: #dcdde1; font-size: 12px; font-weight: bold; } "

        /* List Widgets (Modernized with better spacing) */
        "QListWidget { "
        "   background-color: #0f3460; color: #f5f6fa; "
        "   border-radius: 10px; padding: 10px; border: 1px solid #16213e; "
        "} "
        "QListWidget::item { padding: 10px; border-bottom: 1px solid #1a1a2e; } "
        "QListWidget::item:selected { background-color: #3498db; color: white; border-radius: 6px; } "

        /* PRETTY BUTTONS*/
        "QPushButton { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3d5a80, stop:1 #293d56); "
        "   color: white; border-radius: 8px; padding: 10px; font-weight: bold; "
        "   border: 1px solid #1a252f; "
        "} "
        "QPushButton:hover { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4e74a6, stop:1 #3d5a80); "
        "   border: 1px solid #3498db; "
        "} "

        /* Specific Button Color Logic */
        "QPushButton#btn_save { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #27ae60, stop:1 #1e8449); "
        "} "
        "QPushButton#btn_save:hover { background: #2ecc71; } "

        "QPushButton#btn_delete { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #e74c3c, stop:1 #c0392b); "
        "} "
        "QPushButton#btn_delete:hover { background: #ff7675; } "

        "QPushButton#btn_sort_execute { "
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #8e44ad, stop:1 #702963); "
        "} "
        "QPushButton#btn_sort_execute:hover { background: #a29bfe; } "
        );

    // --- SOUND INITIALIZATION ---
    soundSuccess = new QSoundEffect(this);
    soundSuccess->setSource(QUrl("qrc:/new/prefix1/assets/success.wav"));
    soundSuccess->setVolume(0.5f);

    soundError = new QSoundEffect(this);
    soundError->setSource(QUrl("qrc:/new/prefix1/assets/error.wav"));
    soundError->setVolume(0.5f);

    // --- VALIDATORS ---
    QDoubleValidator *gradeValidator = new QDoubleValidator(0.0, 20.0, 2, this);
    gradeValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->input_add_grade->setValidator(gradeValidator);

    // --- CHART SETUP ---
    mainChart = new QChart();
    mainSeries = new QBarSeries();
    mainChart->addSeries(mainSeries);
    mainChart->setTitle("📊 Live Grade Distribution");
    mainChart->setBackgroundBrush(QBrush(QColor("#2c3e50")));
    mainChart->setTitleBrush(QBrush(QColor("#ffffff")));

    // SMART ANIMATION: Set chart animation to AllAnimations for smooth bar transitions
    mainChart->setAnimationOptions(QChart::AllAnimations);
    mainChart->setAnimationDuration(700);

    axisX = new QBarCategoryAxis();
    QStringList categories; categories << "<10" << "10-12" << "12-16" << "16-20";
    axisX->append(categories);
    axisX->setLabelsColor(QColor("#bdc3c7"));
    mainChart->addAxis(axisX, Qt::AlignBottom);
    mainSeries->attachAxis(axisX);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%d");
    axisY->setLabelsColor(QColor("#bdc3c7"));
    mainChart->addAxis(axisY, Qt::AlignLeft);
    mainSeries->attachAxis(axisY);

    mainChartView = new QChartView(mainChart, ui->chartContainer);
    mainChartView->setRenderHint(QPainter::Antialiasing);
    mainChartView->resize(ui->chartContainer->size());
    mainChartView->show();

    // --- TOAST NOTIFICATION ---
    toastLabel = new QLabel(this);
    toastLabel->hide();
    toastLabel->setAlignment(Qt::AlignCenter);
    toastLabel->setStyleSheet("background-color: #27ae60; color: white; padding: 12px; border-radius: 8px; font-weight: bold;");

    currentSortMode = 1;
    addToLog("SYSTEM: Application Engine Started.");
    updateUI();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::addToLog(QString message) {
    enregistrer_log(message.toLocal8Bit().constData());
    QString timeStr = QTime::currentTime().toString("HH:mm:ss");
    ui->list_logs->insertItem(0, QString("[%1] %2").arg(timeStr).arg(message));
}

void MainWindow::startTicker(int start, int end) {
    QVariantAnimation *anim = new QVariantAnimation(this);
    anim->setDuration(600);
    anim->setStartValue(start);
    anim->setEndValue(end);
    anim->setEasingCurve(QEasingCurve::OutQuad);
    connect(anim, &QVariantAnimation::valueChanged, [this](const QVariant &value) {
        ui->lbl_total_val->setText(QString::number(value.toInt()));
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::showToast(QString message) {
    toastLabel->setText(" ✅ " + message + " ");
    toastLabel->adjustSize();
    int x = (this->width() - toastLabel->width()) / 2;
    int y = this->height() - 70;
    toastLabel->move(x, y + 40);
    toastLabel->show();
    QPropertyAnimation *anim = new QPropertyAnimation(toastLabel, "pos");
    anim->setDuration(400);
    anim->setStartValue(QPoint(x, y + 40));
    anim->setEndValue(QPoint(x, y));
    anim->setEasingCurve(QEasingCurve::OutCubic);
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(2500, [this]() { toastLabel->hide(); });
}

void MainWindow::shakeWidget(QWidget *widget) {
    soundError->play();
    QPropertyAnimation *anim = new QPropertyAnimation(widget, "pos");
    anim->setDuration(400);
    QPoint startPos = widget->pos();
    anim->setKeyValueAt(0, startPos);
    anim->setKeyValueAt(0.1, startPos + QPoint(-10, 0));
    anim->setKeyValueAt(0.3, startPos + QPoint(10, 0));
    anim->setKeyValueAt(1, startPos);
    widget->setStyleSheet("border: 2px solid #e74c3c; background-color: #4c2b2b; color: white;");
    anim->start(QAbstractAnimation::DeleteWhenStopped);
    QTimer::singleShot(1000, [widget]() { widget->setStyleSheet(""); });
}

void MainWindow::pulseWidget(QWidget *widget) {
    // Animation 1: Color Flash
    QString originalStyle = widget->styleSheet();
    widget->setStyleSheet("background-color: #2ecc71; color: white; border: 1px solid white;");

    // Animation 2: Scale/Bounce effect using Geometry
    QPropertyAnimation *anim = new QPropertyAnimation(widget, "geometry");
    anim->setDuration(200);
    QRect geom = widget->geometry();
    anim->setStartValue(geom);
    anim->setKeyValueAt(0.5, QRect(geom.x()-2, geom.y()-2, geom.width()+4, geom.height()+4));
    anim->setEndValue(geom);
    anim->setEasingCurve(QEasingCurve::OutBack);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    QTimer::singleShot(500, [widget, originalStyle]() { widget->setStyleSheet(originalStyle); });
}

void MainWindow::updateUI(QString filter) {
    ui->list_students->clear();
    etudiant *temp = head;
    int count = 0; float sum = 0;
    int fail = 0, pass = 0, good = 0, excel = 0;

    while(temp != nullptr) {
        QString idStr = QString::fromLocal8Bit(temp->id);
        QString nomStr = QString::fromLocal8Bit(temp->nom);
        if(!filter.isEmpty() && !idStr.contains(filter, Qt::CaseInsensitive) && !nomStr.contains(filter, Qt::CaseInsensitive)) {
            temp = temp->next; continue;
        }

        QListWidgetItem *item = new QListWidgetItem(QString("ID: %1 | Name: %2 | Grade: %3").arg(idStr).arg(nomStr).arg(temp->moyenne));
        if (temp->moyenne < 10) item->setForeground(QColor("#e74c3c"));
        else if (temp->moyenne >= 16) { item->setForeground(QColor("#50fa7b")); item->setFont(QFont("Arial", 9, QFont::Bold)); }
        else item->setForeground(QColor("#f1c40f"));
        ui->list_students->addItem(item);

        if (temp->moyenne < 10) fail++;
        else if (temp->moyenne < 12) pass++;
        else if (temp->moyenne < 16) good++;
        else excel++;
        sum += temp->moyenne; count++;
        temp = temp->next;
    }

    ui->lbl_avg_val->setText(count > 0 ? QString::number(sum/count, 'f', 2) : "0.00");

    // SMART ANIMATION: The chart bars will now "grow" and slide smoothly
    mainSeries->clear();
    QBarSet *set = new QBarSet("Students");
    *set << fail << pass << good << excel;
    set->setColor(QColor("#3498db"));
    mainSeries->append(set);
    axisY->setRange(0, std::max({fail, pass, good, excel}) + 1);

    startTicker(lastCount, count);
    lastCount = count;
}

void MainWindow::on_btn_save_clicked() {
    QString id = ui->input_add_id->text();
    QString name = ui->input_add_name->text();
    QString gradeStr = ui->input_add_grade->text();

    if(id.isEmpty() || name.isEmpty() || gradeStr.isEmpty()) {
        if(id.isEmpty()) shakeWidget(ui->input_add_id);
        if(name.isEmpty()) shakeWidget(ui->input_add_name);
        if(gradeStr.isEmpty()) shakeWidget(ui->input_add_grade);
        return;
    }

    float gVal = gradeStr.toFloat();
    if(gVal < 0 || gVal > 20) {
        QMessageBox::critical(this, "Grade Error", "Grade must be between 0 and 20!");
        shakeWidget(ui->input_add_grade);
        return;
    }

    if(InsertionQueue(id.toLocal8Bit().data(), gVal, name.toLocal8Bit().data())) {
        soundSuccess->play();
        pulseWidget(ui->btn_save);
        showToast("Student Added!");
        addToLog("SUCCESS: Added student " + id + " (" + name + ")");
        updateUI();
        ui->input_add_id->clear(); ui->input_add_name->clear(); ui->input_add_grade->clear();
    } else {
        shakeWidget(ui->input_add_id);
        addToLog("ERROR: Duplicate ID attempt (" + id + ")");
        QMessageBox::warning(this, "Duplicate ID", "This ID already exists!");
    }
}

void MainWindow::on_btn_manage_clicked() {
    QString theId = ui->input_manage_id->text();
    if(theId.isEmpty()) return;
    etudiant* res = RechercheID_GUI(theId.toLocal8Bit().data());
    if(!res) {
        soundError->play();
        addToLog("NOT FOUND: Search failed for ID " + theId);
        return;
    }

    struct TempData { QString id; QString name; float grade; };
    TempData *data = new TempData{theId, QString::fromLocal8Bit(res->nom), res->moyenne};

    QDialog popup(this);
    popup.setWindowTitle("Manage Student");
    popup.setStyleSheet("background-color: #1e272e; color: white;");
    QVBoxLayout *layout = new QVBoxLayout(&popup);
    QPushButton *btnMod = new QPushButton("Update Information ✍️");
    QPushButton *btnDel = new QPushButton("Delete Student 🗑️");
    layout->addWidget(new QLabel("⚙️ Managing ID: " + theId));
    layout->addWidget(btnMod); layout->addWidget(btnDel);

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

        connect(chId, &QPushButton::clicked, [=, &popup]() { bool ok; QString val = QInputDialog::getText(&popup, "ID", "New ID:", QLineEdit::Normal, data->id, &ok); if (ok && !val.isEmpty()) data->id = val; });
        connect(chName, &QPushButton::clicked, [=, &popup]() { bool ok; QString val = QInputDialog::getText(&popup, "Name", "New Name:", QLineEdit::Normal, data->name, &ok); if (ok && !val.isEmpty()) data->name = val; });
        connect(chGrade, &QPushButton::clicked, [=, &popup]() { bool ok; double val = QInputDialog::getDouble(&popup, "Grade", "New Grade:", (double)data->grade, 0, 20, 2, &ok); if (ok) data->grade = (float)val; });

        connect(confirm, &QPushButton::clicked, [=, &popup]() {
            if (Modifier_GUI(theId.toLocal8Bit().data(), data->id.toLocal8Bit().data(), data->name.toLocal8Bit().data(), data->grade)) {
                soundSuccess->play();
                addToLog("MODIFY: Record " + theId + " updated.");
                popup.accept();
            } else {
                soundError->play();
                QMessageBox::critical(&popup, "Error", "ID already exists! Choose a unique ID.");
            }
        });
    });

    connect(btnDel, &QPushButton::clicked, [&]() {
        Supprimer_GUI(theId.toLocal8Bit().data());
        soundSuccess->play();
        addToLog("DELETE: Student " + theId + " removed.");
        popup.accept();
    });

    if (popup.exec() == QDialog::Accepted) { updateUI(); SauvegarderListe(); }
    delete data;
}

void MainWindow::on_btn_file_save_clicked() {
    SauvegarderListe();
    soundSuccess->play();
    pulseWidget(ui->btn_file_save);
    showToast("Data Saved Successfully");
    addToLog("FILE_OP: All records exported to etudiants.txt");
}

void MainWindow::on_btn_file_load_clicked() {
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Load", "⚠️ Replace current list with file data?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        LoadEtudiants();
        soundSuccess->play();
        addToLog("FILE_OP: Data imported from etudiants.txt");
        updateUI();
    }
}

void MainWindow::on_btn_export_clicked() {
    QMessageBox msgBox;
    msgBox.setWindowTitle("Export Records");
    msgBox.setText("Choose your export format:");
    QPushButton *pdfBtn = msgBox.addButton("PDF Report 📄", QMessageBox::ActionRole);
    QPushButton *excelBtn = msgBox.addButton("Excel/CSV 📊", QMessageBox::ActionRole);
    msgBox.addButton(QMessageBox::Cancel);
    msgBox.exec();
    if (msgBox.clickedButton() == pdfBtn) exportAsPDF();
    else if (msgBox.clickedButton() == excelBtn) exportAsExcel();
}

void MainWindow::exportAsPDF() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save PDF", "Student_Report.pdf", "*.pdf");
    if (fileName.isEmpty()) return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    QString html = "<html><head><style>table { border-collapse: collapse; width: 100%; } th, td { border: 1px solid black; padding: 8px; text-align: left; } th { background-color: #3498db; color: white; } h1 { text-align: center; color: #2c3e50; }</style></head><body><h1>EMSI Student Management Report</h1><table><tr><th>ID</th><th>Student Name</th><th>Grade (/20)</th></tr>";
    etudiant *temp = head;
    while(temp) { html += QString("<tr><td>%1</td><td>%2</td><td>%3</td></tr>").arg(temp->id).arg(temp->nom).arg(temp->moyenne); temp = temp->next; }
    html += "</table></body></html>";
    QTextDocument doc; doc.setHtml(html); doc.print(&printer);
    soundSuccess->play();
    addToLog("EXPORT: PDF Report generated.");
    QMessageBox::information(this, "Success", "PDF Report generated!");
}

void MainWindow::exportAsExcel() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save CSV", "Student_Data.csv", "*.csv");
    if (fileName.isEmpty()) return;
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file); out << "ID,Name,Grade\n";
        etudiant *temp = head;
        while(temp) { out << QString("%1,%2,%3\n").arg(temp->id).arg(temp->nom).arg(temp->moyenne); temp = temp->next; }
        file.close();
        soundSuccess->play();
        addToLog("EXPORT: CSV Data exported.");
        QMessageBox::information(this, "Success", "Excel-ready CSV generated!");
    }
}

void MainWindow::on_input_search_id_textChanged(const QString &arg1) { updateUI(arg1); }

void MainWindow::on_btn_search_clicked() {
    QString sid = ui->input_search_id->text();
    for(int i = 0; i < ui->list_students->count(); ++i) {
        if(ui->list_students->item(i)->text().contains(sid, Qt::CaseInsensitive)) {
            ui->list_students->item(i)->setSelected(true);
            ui->list_students->scrollToItem(ui->list_students->item(i));
            return;
        }
    }
}

// --- SORTING BUTTONS WITH SELECTION STATES ---
void MainWindow::on_btn_sort_id_clicked() {
    currentSortMode = 1;
    ui->btn_sort_id->setStyleSheet("background-color: #2980b9; border: 2px solid #3498db; color: white; font-weight: bold;");
    ui->btn_sort_name->setStyleSheet("");
    ui->btn_sort_grade->setStyleSheet("");
    showToast("Sorting Mode: ID");
}

void MainWindow::on_btn_sort_name_clicked() {
    currentSortMode = 2;
    ui->btn_sort_name->setStyleSheet("background-color: #2980b9; border: 2px solid #3498db; color: white; font-weight: bold;");
    ui->btn_sort_id->setStyleSheet("");
    ui->btn_sort_grade->setStyleSheet("");
    showToast("Sorting Mode: Name");
}

void MainWindow::on_btn_sort_grade_clicked() {
    currentSortMode = 3;
    ui->btn_sort_grade->setStyleSheet("background-color: #2980b9; border: 2px solid #3498db; color: white; font-weight: bold;");
    ui->btn_sort_id->setStyleSheet("");
    ui->btn_sort_name->setStyleSheet("");
    showToast("Sorting Mode: Grade");
}

void MainWindow::on_btn_sort_execute_clicked() {
    TrierListe(currentSortMode);
    soundSuccess->play();
    pulseWidget(ui->btn_sort_execute);

    ui->list_students->clear();
    etudiant *temp = head;
    int r = 1;
    while(temp) {
        QString rankStr = "";
        if (currentSortMode == 3) {
            if (r == 1) rankStr = "🥇 ";
            else if (r == 2) rankStr = "🥈 ";
            else if (r == 3) rankStr = "🥉 ";
            else rankStr = QString("#%1 ").arg(r);
        }
        ui->list_students->addItem(QString("%1ID: %2 | %3 | %4/20").arg(rankStr).arg(temp->id).arg(temp->nom).arg(temp->moyenne));
        temp = temp->next; r++;
    }

    addToLog("ACTION: Sorting algorithm executed successfully.");
    SauvegarderListe();

    // Reset sort button colors after execution
    QTimer::singleShot(1000, [this]() {
        ui->btn_sort_id->setStyleSheet("");
        ui->btn_sort_name->setStyleSheet("");
        ui->btn_sort_grade->setStyleSheet("");
    });
}