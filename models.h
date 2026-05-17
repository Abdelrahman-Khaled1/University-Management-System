#pragma once
#include <QString>
#include <QVector>

struct Student {
    QString id, name, section, email, phone, password;
    int GP[5]  = {0,0,0,0,0};
    int GTA[5] = {0,0,0,0,0};
};

struct Professor {
    QString id, name, course, email, password;
};

struct TA {
    QString id, name, course, section, email, password;
};

struct Course {
    QString code;
};

struct AdminData {
    QString id, name, email, password;
};

// Utility: map course code → index 0‥4
inline int courseCodeToIndex(const QString& code) {
    if (code == "cs100") return 0;
    if (code == "cs101") return 1;
    if (code == "cs102") return 2;
    if (code == "cs103") return 3;
    if (code == "cs104") return 4;
    return -1;
}
