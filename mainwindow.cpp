#include "mainwindow.h"
#include "logindialog.h"
#include "studentdashboard.h"
#include "profdashboard.h"
#include "tadashboard.h"
#include "admindashboard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    setWindowTitle("School Management System");
    setMinimumSize(900, 620);
    applyGlobalStyle();

    m_stack = new QStackedWidget(this);
    setCentralWidget(m_stack);

    // Build landing page
    m_landing = new QWidget;
    auto* vl = new QVBoxLayout(m_landing);
    vl->setAlignment(Qt::AlignCenter);
    vl->setSpacing(20);

    auto* icon = new QLabel("🎓");
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet("font-size: 72px;");
    vl->addWidget(icon);

    auto* title = new QLabel("School Management System");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #e2e8f0; font-size: 32px; font-weight: 800;");
    vl->addWidget(title);

    auto* sub = new QLabel("Students · Professors · TAs · Admins");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("color: #64748b; font-size: 15px;");
    vl->addWidget(sub);

    vl->addSpacing(20);

    m_loginBtn = new QPushButton("  Login");
    m_loginBtn->setFixedSize(180, 48);
    m_loginBtn->setStyleSheet(R"(
        QPushButton {
            background: #3b82f6;
            color: white;
            border: none;
            border-radius: 10px;
            font-size: 16px;
            font-weight: bold;
        }
        QPushButton:hover { background: #2563eb; }
    )");
    connect(m_loginBtn, &QPushButton::clicked, this, &MainWindow::onLogin);

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch();
    btnRow->addWidget(m_loginBtn);
    btnRow->addStretch();
    vl->addLayout(btnRow);

    m_stack->addWidget(m_landing);   // index 0
    m_stack->setCurrentIndex(0);
}

void MainWindow::applyGlobalStyle()
{
    qApp->setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #0f172a;
            color: #e2e8f0;
            font-family: 'Segoe UI', Arial, sans-serif;
        }
        QTableWidget {
            background: #1e293b;
            color: #e2e8f0;
            gridline-color: #334155;
            border: 1px solid #334155;
            border-radius: 6px;
        }
        QTableWidget::item:selected {
            background: #3b82f6;
            color: white;
        }
        QHeaderView::section {
            background: #1e3a5f;
            color: #93c5fd;
            border: none;
            padding: 6px;
            font-weight: bold;
        }
        QLineEdit, QSpinBox, QComboBox {
            background: #1e293b;
            color: #e2e8f0;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 6px 10px;
        }
        QLineEdit:focus, QSpinBox:focus, QComboBox:focus {
            border-color: #3b82f6;
        }
        QComboBox QAbstractItemView {
            background: #1e293b;
            color: #e2e8f0;
            selection-background-color: #3b82f6;
        }
        QPushButton {
            background: #1e3a5f;
            color: #93c5fd;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 7px 18px;
            font-size: 13px;
        }
        QPushButton:hover { background: #1e4d7b; border-color: #3b82f6; }
        QPushButton#dangerBtn {
            background: #450a0a;
            color: #fca5a5;
            border-color: #7f1d1d;
        }
        QPushButton#dangerBtn:hover { background: #7f1d1d; }
        QPushButton#primaryBtn {
            background: #3b82f6;
            color: white;
            border: none;
            font-weight: bold;
        }
        QPushButton#primaryBtn:hover { background: #2563eb; }
        QGroupBox {
            border: 1px solid #334155;
            border-radius: 8px;
            margin-top: 14px;
            color: #94a3b8;
            font-size: 12px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 4px;
        }
        QLabel#pageTitle {
            font-size: 22px;
            font-weight: bold;
            color: #e2e8f0;
        }
        QLabel#infoKey {
            color: #64748b;
            font-size: 12px;
        }
        QLabel#infoVal {
            color: #e2e8f0;
            font-size: 14px;
            font-weight: bold;
        }
        QMessageBox { background: #1e293b; color: #e2e8f0; }
        QScrollBar:vertical {
            background: #0f172a;
            width: 8px;
        }
        QScrollBar::handle:vertical {
            background: #334155;
            border-radius: 4px;
        }
    )");
}

void MainWindow::onLogin()
{
    LoginDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted) return;

    QString role = dlg.role();
    QString id   = dlg.userId();
    QString pwd  = dlg.password();

    if (role == "Student") {
        Student* s = m_sys.loginStudent(id, pwd);
        if (!s) { QMessageBox::warning(this, "Login Failed", "Invalid ID or password."); return; }
        auto* dash = new StudentDashboard(s, &m_sys, this);
        connect(dash, &StudentDashboard::logout, this, &MainWindow::onLogout);
        m_stack->addWidget(dash);
        m_stack->setCurrentWidget(dash);
    }
    else if (role == "Professor") {
        Professor* p = m_sys.loginProfessor(id, pwd);
        if (!p) { QMessageBox::warning(this, "Login Failed", "Invalid ID or password."); return; }
        auto* dash = new ProfDashboard(p, &m_sys, this);
        connect(dash, &ProfDashboard::logout, this, &MainWindow::onLogout);
        m_stack->addWidget(dash);
        m_stack->setCurrentWidget(dash);
    }
    else if (role == "TA") {
        TA* t = m_sys.loginTA(id, pwd);
        if (!t) { QMessageBox::warning(this, "Login Failed", "Invalid ID or password."); return; }
        auto* dash = new TADashboard(t, &m_sys, this);
        connect(dash, &TADashboard::logout, this, &MainWindow::onLogout);
        m_stack->addWidget(dash);
        m_stack->setCurrentWidget(dash);
    }
    else { // Admin
        AdminData* a = m_sys.loginAdmin(id, pwd);
        if (!a) { QMessageBox::warning(this, "Login Failed", "Invalid ID or password."); return; }
        auto* dash = new AdminDashboard(&m_sys, this);
        connect(dash, &AdminDashboard::logout, this, &MainWindow::onLogout);
        m_stack->addWidget(dash);
        m_stack->setCurrentWidget(dash);
    }
}

void MainWindow::onLogout()
{
    // Remove all widgets except landing (index 0)
    while (m_stack->count() > 1) {
        QWidget* w = m_stack->widget(1);
        m_stack->removeWidget(w);
        delete w;
    }
    m_stack->setCurrentIndex(0);
}
