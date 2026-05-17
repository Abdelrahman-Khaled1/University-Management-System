#include "studentdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QTableWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>

static QLabel* infoLabel(const QString& key, const QString& val)
{
    auto* w = new QLabel(QString("<span style='color:#64748b;font-size:11px;'>%1</span><br>"
                                  "<span style='color:#e2e8f0;font-size:14px;font-weight:bold;'>%2</span>")
                          .arg(key, val));
    w->setWordWrap(true);
    return w;
}

StudentDashboard::StudentDashboard(Student* s, SchoolSystem* sys, QWidget* parent)
    : QWidget(parent), m_s(s), m_sys(sys)
{
    buildUI();
}

void StudentDashboard::buildUI()
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 20, 28, 20);
    root->setSpacing(18);

    // ── Header ────────────────────────────────────────────────
    auto* hdr = new QHBoxLayout;
    auto* title = new QLabel("👤  Student Dashboard");
    title->setObjectName("pageTitle");
    hdr->addWidget(title);
    hdr->addStretch();

    auto* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &StudentDashboard::logout);
    hdr->addWidget(logoutBtn);
    root->addLayout(hdr);

    // ── Info cards ───────────────────────────────────────────
    auto* infoBox = new QGroupBox("Profile");
    auto* infoGrid = new QGridLayout(infoBox);
    infoGrid->setSpacing(14);
    infoGrid->addWidget(infoLabel("Student ID",  m_s->id),      0, 0);
    infoGrid->addWidget(infoLabel("Full Name",   m_s->name),    0, 1);
    infoGrid->addWidget(infoLabel("Section",     m_s->section), 0, 2);
    infoGrid->addWidget(infoLabel("Email",       m_s->email),   1, 0);
    infoGrid->addWidget(infoLabel("Phone",       m_s->phone),   1, 1);

    double gpa = SchoolSystem::computeGPA(*m_s);
    auto* gpaLabel = new QLabel(QString("<span style='color:#64748b;font-size:11px;'>GPA</span><br>"
                                         "<span style='color:%1;font-size:20px;font-weight:bold;'>%2</span>")
                                 .arg(gpa >= 3.0 ? "#22c55e" : gpa >= 2.0 ? "#f59e0b" : "#ef4444")
                                 .arg(gpa));
    infoGrid->addWidget(gpaLabel, 1, 2);
    root->addWidget(infoBox);

    // ── Grades table ─────────────────────────────────────────
    auto* gradesBox = new QGroupBox("Grades (cs100 – cs104)");
    auto* gl = new QVBoxLayout(gradesBox);

    static const QString courses[] = {"cs100","cs101","cs102","cs103","cs104"};
    auto* table = new QTableWidget(5, 4);
    table->setHorizontalHeaderLabels({"Course","Prof Grade (0-20)","TA Grade (0-10)","Total (0-30)"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->hide();
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i = 0; i < 5; ++i) {
        table->setItem(i, 0, new QTableWidgetItem(courses[i]));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(m_s->GP[i])));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(m_s->GTA[i])));
        table->setItem(i, 3, new QTableWidgetItem(QString::number(m_s->GP[i] + m_s->GTA[i])));
    }
    gl->addWidget(table);
    root->addWidget(gradesBox);

    // ── Actions ───────────────────────────────────────────────
    auto* actBox = new QGroupBox("Account Settings");
    auto* actRow = new QHBoxLayout(actBox);

    auto* chgPwdBtn = new QPushButton("🔑  Change Password");
    chgPwdBtn->setObjectName("primaryBtn");
    connect(chgPwdBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString newPwd = QInputDialog::getText(this, "Change Password",
            "New password (6-20 chars, upper, lower, digit, one of $#@):",
            QLineEdit::Password, "", &ok);
        if (!ok || newPwd.isEmpty()) return;

        auto isValid = [](const QString& p) {
            if (p.size() < 6 || p.size() > 20) return false;
            bool l=false,u=false,d=false,sp=false;
            for (QChar c : p) {
                if (c.isLower()) l=true;
                else if (c.isUpper()) u=true;
                else if (c.isDigit()) d=true;
                else if (c=='$'||c=='#'||c=='@') sp=true;
            }
            return l&&u&&d&&sp;
        };

        if (!isValid(newPwd)) {
            QMessageBox::warning(this, "Invalid", "Password does not meet requirements.");
            return;
        }
        m_s->password = newPwd;
        m_sys->saveStudents();
        QMessageBox::information(this, "Success", "Password updated.");
    });
    actRow->addWidget(chgPwdBtn);

    auto* chgPhoneBtn = new QPushButton("📱  Change Phone");
    connect(chgPhoneBtn, &QPushButton::clicked, this, [this]() {
        bool ok;
        QString phone = QInputDialog::getText(this, "Change Phone", "New phone (11 digits):",
                                               QLineEdit::Normal, "", &ok);
        if (!ok || phone.isEmpty()) return;
        if (phone.size() != 11 || !std::all_of(phone.begin(), phone.end(),
                                                [](QChar c){ return c.isDigit(); })) {
            QMessageBox::warning(this, "Invalid", "Phone must be exactly 11 digits.");
            return;
        }
        m_s->phone = phone;
        m_sys->saveStudents();
        QMessageBox::information(this, "Success", "Phone updated.");
    });
    actRow->addWidget(chgPhoneBtn);
    actRow->addStretch();

    root->addWidget(actBox);
}
