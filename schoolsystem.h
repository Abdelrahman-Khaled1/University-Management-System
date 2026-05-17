#pragma once
#include <QVector>
#include <QString>
#include "models.h"

// Central data store + file I/O
class SchoolSystem {
public:
    SchoolSystem();

    void loadAll();

    // ── File I/O ────────────────────────────────────────────
    void saveStudents();
    void saveProfessors();
    void saveTAs();
    void saveAdmins();

    // ── Auth ─────────────────────────────────────────────────
    Student*   loginStudent  (const QString& id, const QString& pwd);
    Professor* loginProfessor(const QString& id, const QString& pwd);
    TA*        loginTA        (const QString& id, const QString& pwd);
    AdminData* loginAdmin     (const QString& id, const QString& pwd);

    // ── Data access ──────────────────────────────────────────
    QVector<Student>&   students()   { return m_students; }
    QVector<Professor>& professors() { return m_profs;    }
    QVector<TA>&        tas()        { return m_tas;       }
    QVector<Course>&    courses()    { return m_courses;   }
    QVector<AdminData>& admins()     { return m_admins;    }

    // ── Admin operations ─────────────────────────────────────
    void addStudent   (const QString& name, const QString& phone);
    void addProfessor (const QString& name, const QString& course);
    void addTA        (const QString& name, const QString& course, const QString& section);
    void addAdmin     (const QString& name);

    bool deleteStudent  (const QString& id);
    bool deleteProfessor(const QString& id);
    bool deleteTA       (const QString& id);
    bool deleteAdmin    (const QString& id);

    // ── Grade operations ────────────────────────────────────
    bool profSetGrade(const QString& studentId, const QString& course, int grade);
    bool taSetGrade  (const QString& studentId, const QString& course,
                      const QString& section, int grade);

    // ── GPA ─────────────────────────────────────────────────
    static double computeGPA(const Student& s);

private:
    QVector<Student>   m_students;
    QVector<Professor> m_profs;
    QVector<TA>        m_tas;
    QVector<Course>    m_courses;
    QVector<AdminData> m_admins;

    static QString generateEmail(const QString& name);
    static bool    isValidPhone (const QString& phone);
    static QString generateID   (int max, const QString& prefix);
};
