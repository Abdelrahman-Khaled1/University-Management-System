#include "profdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

ProfDashboard::ProfDashboard(Professor* p, SchoolSystem* sys, QWidget* parent)
    : QWidget(parent), m_p(p), m_sys(sys)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 20, 28, 20);
    root->setSpacing(18);

    // ── Header ─────────────────────────────────────────────────
    auto* hdr = new QHBoxLayout;
    auto* title = new QLabel("🏫  Professor Dashboard");
    title->setObjectName("pageTitle");
    hdr->addWidget(title);
    hdr->addStretch();
    auto* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &ProfDashboard::logout);
    hdr->addWidget(logoutBtn);
    root->addLayout(hdr);

    // ── Profile ───────────────────────────────────────────────
    auto* infoBox = new QGroupBox("Profile");
    auto* infoGrid = new QGridLayout(infoBox);
    infoGrid->setSpacing(14);
    auto lbl = [](const QString& k, const QString& v) {
        return new QLabel(QString("<span style='color:#64748b;font-size:11px;'>%1</span><br>"
                                   "<span style='color:#e2e8f0;font-size:14px;font-weight:bold;'>%2</span>")
                          .arg(k, v));
    };
    infoGrid->addWidget(lbl("Professor ID", m_p->id),     0, 0);
    infoGrid->addWidget(lbl("Name",         m_p->name),   0, 1);
    infoGrid->addWidget(lbl("Course",       m_p->course), 0, 2);
    infoGrid->addWidget(lbl("Email",        m_p->email),  1, 0);
    root->addWidget(infoBox);

    // ── Students table ────────────────────────────────────────
    auto* studBox = new QGroupBox("Students in System");
    auto* sl = new QVBoxLayout(studBox);

    auto buildTable = [&]() {
        auto* table = new QTableWidget;
        int idx = courseCodeToIndex(m_p->course);

        QStringList headers = {"ID","Name","Section","Email",
                               "Prof Grade (0-20)","GPA"};
        table->setColumnCount(headers.size());
        table->setHorizontalHeaderLabels(headers);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->verticalHeader()->hide();
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);

        auto& students = m_sys->students();
        table->setRowCount(students.size());
        for (int r = 0; r < students.size(); ++r) {
            const auto& s = students[r];
            table->setItem(r, 0, new QTableWidgetItem(s.id));
            table->setItem(r, 1, new QTableWidgetItem(s.name));
            table->setItem(r, 2, new QTableWidgetItem(s.section));
            table->setItem(r, 3, new QTableWidgetItem(s.email));
            table->setItem(r, 4, new QTableWidgetItem(
                idx >= 0 ? QString::number(s.GP[idx]) : "N/A"));
            table->setItem(r, 5, new QTableWidgetItem(
                QString::number(SchoolSystem::computeGPA(s), 'f', 2)));
        }
        return table;
    };

    auto* table = buildTable();
    sl->addWidget(table);
    root->addWidget(studBox);

    // ── Grade editor ─────────────────────────────────────────
    auto* gradeBox = new QGroupBox("Edit Student Grade (Prof: 0-20)");
    auto* gl = new QHBoxLayout(gradeBox);

    auto* sidEdit   = new QLineEdit; sidEdit->setPlaceholderText("Student ID");
    auto* gradeSpn  = new QSpinBox;  gradeSpn->setRange(0, 20);
    auto* applyBtn  = new QPushButton("Apply Grade");
    applyBtn->setObjectName("primaryBtn");

    gl->addWidget(new QLabel("Student ID:"));
    gl->addWidget(sidEdit);
    gl->addWidget(new QLabel("Grade:"));
    gl->addWidget(gradeSpn);
    gl->addWidget(applyBtn);
    gl->addStretch();

    connect(applyBtn, &QPushButton::clicked, this, [=]() {
        bool ok = m_sys->profSetGrade(sidEdit->text().trimmed(), m_p->course, gradeSpn->value());
        if (ok) QMessageBox::information(this, "Success", "Grade updated.");
        else    QMessageBox::warning   (this, "Error", "Student not found or invalid course.");
    });
    root->addWidget(gradeBox);

    // ── Delete student ────────────────────────────────────────
    auto* delBox = new QGroupBox("Remove Student");
    auto* dl = new QHBoxLayout(delBox);
    auto* delIdEdit = new QLineEdit; delIdEdit->setPlaceholderText("Student ID to remove");
    auto* delBtn    = new QPushButton("Delete Student");
    delBtn->setObjectName("dangerBtn");
    connect(delBtn, &QPushButton::clicked, this, [=]() {
        auto res = QMessageBox::question(this, "Confirm",
            "Delete student " + delIdEdit->text().trimmed() + "?");
        if (res != QMessageBox::Yes) return;
        bool ok = m_sys->deleteStudent(delIdEdit->text().trimmed());
        QMessageBox::information(this, ok ? "Deleted" : "Not Found",
                                 ok ? "Student removed." : "ID not found.");
    });
    dl->addWidget(new QLabel("ID:"));
    dl->addWidget(delIdEdit);
    dl->addWidget(delBtn);
    dl->addStretch();
    root->addWidget(delBox);
}
