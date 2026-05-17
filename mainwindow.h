#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include "schoolsystem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onLogin();
    void onLogout();

private:
    void showLanding();
    void applyGlobalStyle();

    SchoolSystem     m_sys;
    QStackedWidget*  m_stack;
    QWidget*         m_landing;
    QPushButton*     m_loginBtn;
};
