#include "logindialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>

LoginDialog::LoginDialog(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("School System — Login");
    setFixedSize(380, 240);

    setStyleSheet(R"(
        QDialog {
            background: #0f172a;
        }
        QLabel {
            color: #94a3b8;
            font-size: 13px;
        }
        QComboBox, QLineEdit {
            background: #1e293b;
            color: #e2e8f0;
            border: 1px solid #334155;
            border-radius: 6px;
            padding: 6px 10px;
            font-size: 13px;
        }
        QComboBox:focus, QLineEdit:focus {
            border-color: #3b82f6;
        }
        QComboBox QAbstractItemView {
            background: #1e293b;
            color: #e2e8f0;
            selection-background-color: #3b82f6;
        }
        QPushButton#loginBtn {
            background: #3b82f6;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 8px 24px;
            font-size: 13px;
            font-weight: bold;
        }
        QPushButton#loginBtn:hover { background: #2563eb; }
        QPushButton#cancelBtn {
            background: #334155;
            color: #94a3b8;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-size: 13px;
        }
        QPushButton#cancelBtn:hover { background: #475569; }
    )");

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 24, 28, 24);
    mainLayout->setSpacing(16);

    auto* title = new QLabel("🎓  School System");
    title->setStyleSheet("color: #e2e8f0; font-size: 18px; font-weight: bold;");
    mainLayout->addWidget(title);

    auto* form = new QFormLayout();
    form->setSpacing(10);
    form->setLabelAlignment(Qt::AlignRight);

    m_roleBox = new QComboBox;
    m_roleBox->addItems({"Student", "Professor", "TA", "Admin"});
    form->addRow("Role:", m_roleBox);

    m_idEdit = new QLineEdit;
    m_idEdit->setPlaceholderText("e.g. S001");
    form->addRow("ID:", m_idEdit);

    m_pwdEdit = new QLineEdit;
    m_pwdEdit->setEchoMode(QLineEdit::Password);
    m_pwdEdit->setPlaceholderText("password");
    form->addRow("Password:", m_pwdEdit);

    mainLayout->addLayout(form);

    auto* btnRow = new QHBoxLayout;
    btnRow->addStretch();

    m_cancelBtn = new QPushButton("Cancel");
    m_cancelBtn->setObjectName("cancelBtn");
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    btnRow->addWidget(m_cancelBtn);

    m_loginBtn = new QPushButton("Login");
    m_loginBtn->setObjectName("loginBtn");
    m_loginBtn->setDefault(true);
    connect(m_loginBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnRow->addWidget(m_loginBtn);

    mainLayout->addLayout(btnRow);
}

QString LoginDialog::role()     const { return m_roleBox->currentText(); }
QString LoginDialog::userId()   const { return m_idEdit->text().trimmed(); }
QString LoginDialog::password() const { return m_pwdEdit->text(); }
