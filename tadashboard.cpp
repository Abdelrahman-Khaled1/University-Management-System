#include "tadashboard.h"
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

TADashboard::TADashboard(TA* t, SchoolSystem* sys, QWidget* parent)
    : QWidget(parent), m_t(t), m_sys(sys)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 20, 28, 20);
    root->setSpacing(18);

    // ── Header ─────────────────────────────────────────────────
    auto* hdr = new QHBoxLayout;
    auto* title = new QLabel("📋  TA Dashboard");
    title->setObjectName("pageTitle");
    hdr->addWidget(title);
    hdr->addStretch();
    auto* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &TADashboard::logout);
    hdr->addWidget(logoutBtn);
    root->addLayout(hdr);

    // ── Profile ───────────────────────────────────────────────
    auto* infoBox = new QGroupBox("Profile");
    auto* ig = new QGridLayout(infoBox);
    ig->setSpacing(14);
    auto lbl = [](const QString& k, const QString& v) {
        return new QLabel(QString("<span style='color:#64748b;font-size:11px;'>%1</span><br>"
                                   "<span style='color:#e2e8f0;font-size:14px;font-weight:bold;'>%2</span>")
                          .arg(k, v));
    };
    ig->addWidget(lbl("TA ID",   m_t->id),      0, 0);
    ig->addWidget(lbl("Name",    m_t->name),    0, 1);
    ig->addWidget(lbl("Course",  m_t->course),  0, 2);
    ig->addWidget(lbl("Section", m_t->section), 1, 0);
    ig->addWidget(lbl("Email",   m_t->email),   1, 1);
    root->addWidget(infoBox);

    // ── Students in my section ────────────────────────────────
    auto* studBox = new QGroupBox(QString("Students in Section %1 / %2").arg(m_t->section, m_t->course));
    auto* sl = new QVBoxLayout(studBox);

    int courseIdx = courseCodeToIndex(m_t->course);
    auto& students = m_sys->students();

    auto* table = new QTableWidget;
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels({"ID","Name","Email","Prof Grade","TA Grade"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVector<int> myRows;
    for (int i = 0; i < students.size(); ++i)
        if (students[i].section == m_t->section) myRows.append(i);

    table->setRowCount(myRows.size());
    for (int r = 0; r < myRows.size(); ++r) {
        const auto& s = students[myRows[r]];
        table->setItem(r, 0, new QTableWidgetItem(s.id));
        table->setItem(r, 1, new QTableWidgetItem(s.name));
        table->setItem(r, 2, new QTableWidgetItem(s.email));
        table->setItem(r, 3, new QTableWidgetItem(courseIdx >= 0 ? QString::number(s.GP[courseIdx])  : "N/A"));
        table->setItem(r, 4, new QTableWidgetItem(courseIdx >= 0 ? QString::number(s.GTA[courseIdx]) : "N/A"));
    }
    sl->addWidget(table);
    root->addWidget(studBox);

    // ── Grade editor ─────────────────────────────────────────
    auto* gradeBox = new QGroupBox("Edit Student TA Grade (0-10)");
    auto* gl = new QHBoxLayout(gradeBox);

    auto* sidEdit  = new QLineEdit; sidEdit->setPlaceholderText("Student ID");
    auto* gradeSpn = new QSpinBox;  gradeSpn->setRange(0, 10);
    auto* applyBtn = new QPushButton("Apply Grade");
    applyBtn->setObjectName("primaryBtn");

    gl->addWidget(new QLabel("Student ID:"));
    gl->addWidget(sidEdit);
    gl->addWidget(new QLabel("TA Grade:"));
    gl->addWidget(gradeSpn);
    gl->addWidget(applyBtn);
    gl->addStretch();

    connect(applyBtn, &QPushButton::clicked, this, [=]() {
        bool ok = m_sys->taSetGrade(sidEdit->text().trimmed(),
                                    m_t->course, m_t->section, gradeSpn->value());
        if (ok) QMessageBox::information(this, "Success", "TA grade updated.");
        else    QMessageBox::warning   (this, "Error", "Student not found in your section.");
    });

    root->addWidget(gradeBox);
}
