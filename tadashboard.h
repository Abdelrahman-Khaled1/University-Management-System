#pragma once
#include <QWidget>
#include "schoolsystem.h"

class TADashboard : public QWidget {
    Q_OBJECT
public:
    TADashboard(TA* t, SchoolSystem* sys, QWidget* parent = nullptr);
signals:
    void logout();
private:
    TA*           m_t;
    SchoolSystem* m_sys;
};
