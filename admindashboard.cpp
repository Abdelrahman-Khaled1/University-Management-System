#include "admindashboard.h"
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
#include <QTabWidget>
#include <QScrollArea>
#include <QFormLayout>

// ─── Helper: section widget with a table + add/delete controls ───────────────
static QTableWidget* makeTable(const QStringList& headers)
{
    auto* t = new QTableWidget(0, headers.size());
    t->setHorizontalHeaderLabels(headers);
    t->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    t->verticalHeader()->hide();
    t->setEditTriggers(QAbstractItemView::NoEditTriggers);
    t->setSelectionBehavior(QAbstractItemView::SelectRows);
    t->setSelectionMode(QAbstractItemView::SingleSelection);
    return t;
}

// ─────────────────────────────────────────────────────────────────────────────
AdminDashboard::AdminDashboard(SchoolSystem* sys, QWidget* parent)
    : QWidget(parent), m_sys(sys)
{
    auto* root = new QVBoxLayout(this);
    root->setContentsMargins(28, 20, 28, 20);
    root->setSpacing(16);

    // Header
    auto* hdr = new QHBoxLayout;
    auto* title = new QLabel("🔧  Admin Dashboard");
    title->setObjectName("pageTitle");
    hdr->addWidget(title);
    hdr->addStretch();
    auto* logoutBtn = new QPushButton("Logout");
    logoutBtn->setObjectName("dangerBtn");
    connect(logoutBtn, &QPushButton::clicked, this, &AdminDashboard::logout);
    hdr->addWidget(logoutBtn);
    root->addLayout(hdr);

    // Tabs
    auto* tabs = new QTabWidget;
    tabs->setStyleSheet(R"(
        QTabWidget::pane { border: 1px solid #334155; border-radius: 6px; }
        QTabBar::tab {
            background: #1e293b; color: #64748b;
            padding: 8px 20px; border-radius: 4px 4px 0 0;
        }
        QTabBar::tab:selected { background: #1e3a5f; color: #93c5fd; }
    )");

    buildStudentsTab(tabs);
    buildProfTab    (tabs);
    buildTATab      (tabs);
    buildAdminTab   (tabs);

    root->addWidget(tabs);
}

// ─────────────────────────────────────────────────────────────────────────────
// Students tab
// ─────────────────────────────────────────────────────────────────────────────
void AdminDashboard::buildStudentsTab(QTabWidget* tabs)
{
    auto* page = new QWidget;
    auto* vl   = new QVBoxLayout(page);
    vl->setContentsMargins(12,12,12,12);
    vl->setSpacing(12);

    auto* table = makeTable({"ID","Name","Section","Email","Phone","GPA"});

    auto refresh = [=]() {
        auto& v = m_sys->students();
        table->setRowCount(v.size());
        for (int r = 0; r < v.size(); ++r) {
            const auto& s = v[r];
            table->setItem(r,0,new QTableWidgetItem(s.id));
            table->setItem(r,1,new QTableWidgetItem(s.name));
            table->setItem(r,2,new QTableWidgetItem(s.section));
            table->setItem(r,3,new QTableWidgetItem(s.email));
            table->setItem(r,4,new QTableWidgetItem(s.phone));
            table->setItem(r,5,new QTableWidgetItem(
                QString::number(SchoolSystem::computeGPA(s),'f',2)));
        }
    };
    refresh();
    vl->addWidget(table);

    // Add form
    auto* addBox = new QGroupBox("Add Student");
    auto* fl = new QHBoxLayout(addBox);
    auto* nameEdit  = new QLineEdit; nameEdit->setPlaceholderText("Full name");
    auto* phoneEdit = new QLineEdit; phoneEdit->setPlaceholderText("11-digit phone");
    auto* addBtn    = new QPushButton("Add");
    addBtn->setObjectName("primaryBtn");

    connect(addBtn, &QPushButton::clicked, this, [=]() {
        QString name  = nameEdit->text().trimmed();
        QString phone = phoneEdit->text().trimmed();
        if (name.isEmpty()) { QMessageBox::warning(this,"Error","Name cannot be empty."); return; }
        if (phone.size()!=11 || !std::all_of(phone.begin(),phone.end(),[](QChar c){return c.isDigit();})) {
            QMessageBox::warning(this,"Error","Phone must be exactly 11 digits."); return;
        }
        m_sys->addStudent(name, phone);
        refresh();
        nameEdit->clear(); phoneEdit->clear();
        QMessageBox::information(this,"Added",
            "Student added. ID: " + m_sys->students().last().id +
            "\nDefault password: " + m_sys->students().last().password);
    });

    fl->addWidget(new QLabel("Name:")); fl->addWidget(nameEdit);
    fl->addWidget(new QLabel("Phone:")); fl->addWidget(phoneEdit);
    fl->addWidget(addBtn);
    vl->addWidget(addBox);

    // Delete
    auto* delBox = new QGroupBox("Delete Student");
    auto* dl = new QHBoxLayout(delBox);
    auto* delIdEdit = new QLineEdit; delIdEdit->setPlaceholderText("Student ID");
    auto* delBtn    = new QPushButton("Delete");
    delBtn->setObjectName("dangerBtn");
    connect(delBtn, &QPushButton::clicked, this, [=]() {
        auto res = QMessageBox::question(this,"Confirm","Delete student "+delIdEdit->text()+"?");
        if (res != QMessageBox::Yes) return;
        bool ok = m_sys->deleteStudent(delIdEdit->text().trimmed());
        if (ok) { refresh(); QMessageBox::information(this,"Deleted","Student removed."); }
        else      QMessageBox::warning(this,"Error","ID not found.");
        delIdEdit->clear();
    });
    dl->addWidget(new QLabel("ID:")); dl->addWidget(delIdEdit);
    dl->addWidget(delBtn); dl->addStretch();
    vl->addWidget(delBox);

    tabs->addTab(page, "👥 Students");
}

// ─────────────────────────────────────────────────────────────────────────────
// Professors tab
// ─────────────────────────────────────────────────────────────────────────────
void AdminDashboard::buildProfTab(QTabWidget* tabs)
{
    auto* page = new QWidget;
    auto* vl   = new QVBoxLayout(page);
    vl->setContentsMargins(12,12,12,12); vl->setSpacing(12);

    auto* table = makeTable({"ID","Name","Course","Email"});

    auto refresh = [=]() {
        auto& v = m_sys->professors();
        table->setRowCount(v.size());
        for (int r = 0; r < v.size(); ++r) {
            table->setItem(r,0,new QTableWidgetItem(v[r].id));
            table->setItem(r,1,new QTableWidgetItem(v[r].name));
            table->setItem(r,2,new QTableWidgetItem(v[r].course));
            table->setItem(r,3,new QTableWidgetItem(v[r].email));
        }
    };
    refresh(); vl->addWidget(table);

    // Add
    auto* addBox = new QGroupBox("Add Professor");
    auto* fl = new QHBoxLayout(addBox);
    auto* nameEdit   = new QLineEdit; nameEdit->setPlaceholderText("Full name");
    auto* courseEdit = new QLineEdit; courseEdit->setPlaceholderText("cs100..cs104");
    auto* addBtn     = new QPushButton("Add"); addBtn->setObjectName("primaryBtn");
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) { QMessageBox::warning(this,"Error","Name required."); return; }
        m_sys->addProfessor(nameEdit->text().trimmed(), courseEdit->text().trimmed());
        refresh(); nameEdit->clear(); courseEdit->clear();
        QMessageBox::information(this,"Added","Professor added. ID: "+m_sys->professors().last().id);
    });
    fl->addWidget(new QLabel("Name:")); fl->addWidget(nameEdit);
    fl->addWidget(new QLabel("Course:")); fl->addWidget(courseEdit);
    fl->addWidget(addBtn);
    vl->addWidget(addBox);

    // Delete
    auto* delBox = new QGroupBox("Delete Professor");
    auto* dl = new QHBoxLayout(delBox);
    auto* delIdEdit = new QLineEdit; delIdEdit->setPlaceholderText("Professor ID");
    auto* delBtn    = new QPushButton("Delete"); delBtn->setObjectName("dangerBtn");
    connect(delBtn, &QPushButton::clicked, this, [=]() {
        auto res = QMessageBox::question(this,"Confirm","Delete professor "+delIdEdit->text()+"?");
        if (res != QMessageBox::Yes) return;
        bool ok = m_sys->deleteProfessor(delIdEdit->text().trimmed());
        if (ok) { refresh(); QMessageBox::information(this,"Deleted","Professor removed."); }
        else      QMessageBox::warning(this,"Error","ID not found.");
        delIdEdit->clear();
    });
    dl->addWidget(new QLabel("ID:")); dl->addWidget(delIdEdit);
    dl->addWidget(delBtn); dl->addStretch();
    vl->addWidget(delBox);

    tabs->addTab(page, "🏫 Professors");
}

// ─────────────────────────────────────────────────────────────────────────────
// TAs tab
// ─────────────────────────────────────────────────────────────────────────────
void AdminDashboard::buildTATab(QTabWidget* tabs)
{
    auto* page = new QWidget;
    auto* vl   = new QVBoxLayout(page);
    vl->setContentsMargins(12,12,12,12); vl->setSpacing(12);

    auto* table = makeTable({"ID","Name","Course","Section","Email"});

    auto refresh = [=]() {
        auto& v = m_sys->tas();
        table->setRowCount(v.size());
        for (int r = 0; r < v.size(); ++r) {
            table->setItem(r,0,new QTableWidgetItem(v[r].id));
            table->setItem(r,1,new QTableWidgetItem(v[r].name));
            table->setItem(r,2,new QTableWidgetItem(v[r].course));
            table->setItem(r,3,new QTableWidgetItem(v[r].section));
            table->setItem(r,4,new QTableWidgetItem(v[r].email));
        }
    };
    refresh(); vl->addWidget(table);

    auto* addBox = new QGroupBox("Add TA");
    auto* fl = new QHBoxLayout(addBox);
    auto* nameEdit    = new QLineEdit; nameEdit->setPlaceholderText("Name");
    auto* courseEdit  = new QLineEdit; courseEdit->setPlaceholderText("cs100..cs104");
    auto* sectionEdit = new QLineEdit; sectionEdit->setPlaceholderText("Section");
    auto* addBtn      = new QPushButton("Add"); addBtn->setObjectName("primaryBtn");
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) { QMessageBox::warning(this,"Error","Name required."); return; }
        m_sys->addTA(nameEdit->text().trimmed(), courseEdit->text().trimmed(), sectionEdit->text().trimmed());
        refresh(); nameEdit->clear(); courseEdit->clear(); sectionEdit->clear();
        QMessageBox::information(this,"Added","TA added. ID: "+m_sys->tas().last().id);
    });
    fl->addWidget(new QLabel("Name:")); fl->addWidget(nameEdit);
    fl->addWidget(new QLabel("Course:")); fl->addWidget(courseEdit);
    fl->addWidget(new QLabel("Section:")); fl->addWidget(sectionEdit);
    fl->addWidget(addBtn);
    vl->addWidget(addBox);

    auto* delBox = new QGroupBox("Delete TA");
    auto* dl = new QHBoxLayout(delBox);
    auto* delIdEdit = new QLineEdit; delIdEdit->setPlaceholderText("TA ID");
    auto* delBtn    = new QPushButton("Delete"); delBtn->setObjectName("dangerBtn");
    connect(delBtn, &QPushButton::clicked, this, [=]() {
        auto res = QMessageBox::question(this,"Confirm","Delete TA "+delIdEdit->text()+"?");
        if (res != QMessageBox::Yes) return;
        bool ok = m_sys->deleteTA(delIdEdit->text().trimmed());
        if (ok) { refresh(); QMessageBox::information(this,"Deleted","TA removed."); }
        else      QMessageBox::warning(this,"Error","ID not found.");
        delIdEdit->clear();
    });
    dl->addWidget(new QLabel("ID:")); dl->addWidget(delIdEdit);
    dl->addWidget(delBtn); dl->addStretch();
    vl->addWidget(delBox);

    tabs->addTab(page, "📋 TAs");
}

// ─────────────────────────────────────────────────────────────────────────────
// Admins tab
// ─────────────────────────────────────────────────────────────────────────────
void AdminDashboard::buildAdminTab(QTabWidget* tabs)
{
    auto* page = new QWidget;
    auto* vl   = new QVBoxLayout(page);
    vl->setContentsMargins(12,12,12,12); vl->setSpacing(12);

    auto* table = makeTable({"ID","Name","Email"});

    auto refresh = [=]() {
        auto& v = m_sys->admins();
        table->setRowCount(v.size());
        for (int r = 0; r < v.size(); ++r) {
            table->setItem(r,0,new QTableWidgetItem(v[r].id));
            table->setItem(r,1,new QTableWidgetItem(v[r].name));
            table->setItem(r,2,new QTableWidgetItem(v[r].email));
        }
    };
    refresh(); vl->addWidget(table);

    auto* addBox = new QGroupBox("Add Admin");
    auto* fl = new QHBoxLayout(addBox);
    auto* nameEdit = new QLineEdit; nameEdit->setPlaceholderText("Name");
    auto* addBtn   = new QPushButton("Add"); addBtn->setObjectName("primaryBtn");
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        if (nameEdit->text().trimmed().isEmpty()) { QMessageBox::warning(this,"Error","Name required."); return; }
        m_sys->addAdmin(nameEdit->text().trimmed());
        refresh(); nameEdit->clear();
        QMessageBox::information(this,"Added","Admin added. ID: "+m_sys->admins().last().id+
            "\nPassword: "+m_sys->admins().last().password);
    });
    fl->addWidget(new QLabel("Name:")); fl->addWidget(nameEdit);
    fl->addWidget(addBtn); fl->addStretch();
    vl->addWidget(addBox);

    auto* delBox = new QGroupBox("Delete Admin");
    auto* dl = new QHBoxLayout(delBox);
    auto* delIdEdit = new QLineEdit; delIdEdit->setPlaceholderText("Admin ID");
    auto* delBtn    = new QPushButton("Delete"); delBtn->setObjectName("dangerBtn");
    connect(delBtn, &QPushButton::clicked, this, [=]() {
        auto res = QMessageBox::question(this,"Confirm","Delete admin "+delIdEdit->text()+"?");
        if (res != QMessageBox::Yes) return;
        bool ok = m_sys->deleteAdmin(delIdEdit->text().trimmed());
        if (ok) { refresh(); QMessageBox::information(this,"Deleted","Admin removed."); }
        else      QMessageBox::warning(this,"Error","ID not found.");
        delIdEdit->clear();
    });
    dl->addWidget(new QLabel("ID:")); dl->addWidget(delIdEdit);
    dl->addWidget(delBtn); dl->addStretch();
    vl->addWidget(delBox);

    tabs->addTab(page, "🔧 Admins");
}
