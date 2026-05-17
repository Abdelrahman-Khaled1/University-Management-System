#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include "schoolsystem.h"

class AdminDashboard : public QWidget {
    Q_OBJECT
public:
    AdminDashboard(SchoolSystem* sys, QWidget* parent = nullptr);
signals:
    void logout();
private:
    void buildStudentsTab(QTabWidget* tabs);
    void buildProfTab    (QTabWidget* tabs);
    void buildTATab      (QTabWidget* tabs);
    void buildAdminTab   (QTabWidget* tabs);
    void refreshTable(QTableWidget* t, int tabIdx);

    SchoolSystem* m_sys;
};
