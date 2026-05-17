#pragma once
#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget* parent = nullptr);

    QString role()     const;
    QString userId()   const;
    QString password() const;

private:
    QComboBox* m_roleBox;
    QLineEdit* m_idEdit;
    QLineEdit* m_pwdEdit;
    QPushButton* m_loginBtn;
    QPushButton* m_cancelBtn;
};
