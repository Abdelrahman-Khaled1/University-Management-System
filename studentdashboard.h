#pragma once
#include <QWidget>
#include "schoolsystem.h"

class StudentDashboard : public QWidget {
    Q_OBJECT
public:
    StudentDashboard(Student* s, SchoolSystem* sys, QWidget* parent = nullptr);
signals:
    void logout();
private:
    void buildUI();
    void refreshGrades();

    Student*      m_s;
    SchoolSystem* m_sys;
};
