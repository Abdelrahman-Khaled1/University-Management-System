#pragma once
#include <QWidget>
#include "schoolsystem.h"

class ProfDashboard : public QWidget {
    Q_OBJECT
public:
    ProfDashboard(Professor* p, SchoolSystem* sys, QWidget* parent = nullptr);
signals:
    void logout();
private:
    Professor*    m_p;
    SchoolSystem* m_sys;
};
