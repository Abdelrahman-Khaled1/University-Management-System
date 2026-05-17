#include "schoolsystem.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / load
// ─────────────────────────────────────────────────────────────────────────────
SchoolSystem::SchoolSystem() { loadAll(); }

void SchoolSystem::loadAll()
{
    // ── Students ─────────────────────────────────────────────
    m_students.clear();
    QFile fs("Students.txt");
    if (fs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fs);
        in.readLine(); // header
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList f = line.split(',');
            if (f.size() < 15) continue;
            Student s;
            s.id      = f[0];
            s.name    = f[1];
            s.section = f[2];
            for (int i = 0; i < 5; ++i) {
                s.GP [i] = f[3 + i*2    ].toInt();
                s.GTA[i] = f[3 + i*2 + 1].toInt();
            }
            s.email    = f[13];
            s.phone    = f[14];
            s.password = f.size() > 15 ? f[15] : "";
            m_students.append(s);
        }
        fs.close();
    }

    // ── Professors ───────────────────────────────────────────
    m_profs.clear();
    QFile fp("professors.txt");
    if (fp.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fp);
        in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList f = line.split(',');
            if (f.size() < 5) continue;
            Professor p;
            p.id = f[0]; p.name = f[1]; p.course = f[2];
            p.email = f[3]; p.password = f[4];
            m_profs.append(p);
        }
        fp.close();
    }

    // ── TAs ──────────────────────────────────────────────────
    m_tas.clear();
    QFile ft("tas.txt");
    if (ft.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&ft);
        in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList f = line.split(',');
            if (f.size() < 6) continue;
            TA t;
            t.id = f[0]; t.name = f[1]; t.course = f[2];
            t.section = f[3]; t.email = f[4]; t.password = f[5];
            m_tas.append(t);
        }
        ft.close();
    }

    // ── Courses ──────────────────────────────────────────────
    m_courses.clear();
    QFile fc("courses.txt");
    if (fc.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fc);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                Course c; c.code = line;
                m_courses.append(c);
            }
        }
        fc.close();
    }

    // ── Admins ───────────────────────────────────────────────
    m_admins.clear();
    QFile fa("admins.txt");
    if (fa.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fa);
        in.readLine();
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;
            QStringList f = line.split(',');
            if (f.size() < 4) continue;
            AdminData a;
            a.id = f[0]; a.name = f[1]; a.email = f[2]; a.password = f[3];
            m_admins.append(a);
        }
        fa.close();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Save
// ─────────────────────────────────────────────────────────────────────────────
void SchoolSystem::saveStudents()
{
    QFile f("Students.txt");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&f);
    out << "ID,Name,Section,GP1,GTA1,GP2,GTA2,GP3,GTA3,GP4,GTA4,GP5,GTA5,Gmail,Phone,Password\n";
    for (const auto& s : m_students) {
        out << s.id << "," << s.name << "," << s.section;
        for (int i = 0; i < 5; ++i) out << "," << s.GP[i] << "," << s.GTA[i];
        out << "," << s.email << "," << s.phone << "," << s.password << "\n";
    }
}

void SchoolSystem::saveProfessors()
{
    QFile f("professors.txt");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&f);
    out << "ID,Name,Course,Email,Password\n";
    for (const auto& p : m_profs)
        out << p.id << "," << p.name << "," << p.course << "," << p.email << "," << p.password << "\n";
}

void SchoolSystem::saveTAs()
{
    QFile f("tas.txt");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&f);
    out << "ID,Name,Course,Section,Email,Password\n";
    for (const auto& t : m_tas)
        out << t.id << "," << t.name << "," << t.course << ","
            << t.section << "," << t.email << "," << t.password << "\n";
}

void SchoolSystem::saveAdmins()
{
    QFile f("admins.txt");
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    QTextStream out(&f);
    out << "ID,Name,Email,Password\n";
    for (const auto& a : m_admins)
        out << a.id << "," << a.name << "," << a.email << "," << a.password << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// Auth
// ─────────────────────────────────────────────────────────────────────────────
Student* SchoolSystem::loginStudent(const QString& id, const QString& pwd)
{
    for (auto& s : m_students)
        if (s.id == id && s.password == pwd) return &s;
    return nullptr;
}
Professor* SchoolSystem::loginProfessor(const QString& id, const QString& pwd)
{
    for (auto& p : m_profs)
        if (p.id == id && p.password == pwd) return &p;
    return nullptr;
}
TA* SchoolSystem::loginTA(const QString& id, const QString& pwd)
{
    for (auto& t : m_tas)
        if (t.id == id && t.password == pwd) return &t;
    return nullptr;
}
AdminData* SchoolSystem::loginAdmin(const QString& id, const QString& pwd)
{
    for (auto& a : m_admins)
        if (a.id == id && a.password == pwd) return &a;
    return nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers
// ─────────────────────────────────────────────────────────────────────────────
QString SchoolSystem::generateEmail(const QString& name)
{
    QString n = name; n.replace(' ', '.');
    return n + "@college.edu";
}

bool SchoolSystem::isValidPhone(const QString& phone)
{
    if (phone.size() != 11) return false;
    for (QChar c : phone) if (!c.isDigit()) return false;
    return true;
}

QString SchoolSystem::generateID(int max, const QString& prefix)
{
    return prefix + QString("%1").arg(max + 1, 3, 10, QChar('0'));
}

// ─────────────────────────────────────────────────────────────────────────────
// Admin operations
// ─────────────────────────────────────────────────────────────────────────────
void SchoolSystem::addStudent(const QString& name, const QString& phone)
{
    int maxId = 0;
    for (const auto& s : m_students) {
        int n = s.id.mid(1).toInt();
        if (n > maxId) maxId = n;
    }
    Student s;
    s.id       = generateID(maxId, "S");
    s.name     = name;
    s.phone    = phone;
    s.section  = "0";
    s.email    = generateEmail(name);
    s.password = "pass_" + s.id;
    for (int i = 0; i < 5; ++i) { s.GP[i] = 0; s.GTA[i] = 0; }
    m_students.append(s);
    saveStudents();
}

void SchoolSystem::addProfessor(const QString& name, const QString& course)
{
    int maxId = 0;
    for (const auto& p : m_profs) {
        int n = p.id.mid(1).toInt();
        if (n > maxId) maxId = n;
    }
    Professor p;
    p.id       = generateID(maxId, "P");
    p.name     = name;
    p.course   = course;
    p.email    = generateEmail(name);
    p.password = "pass_" + p.id;
    m_profs.append(p);
    saveProfessors();
}

void SchoolSystem::addTA(const QString& name, const QString& course, const QString& section)
{
    int maxId = 0;
    for (const auto& t : m_tas) {
        int n = t.id.mid(1).toInt();
        if (n > maxId) maxId = n;
    }
    TA t;
    t.id       = generateID(maxId, "T");
    t.name     = name;
    t.course   = course;
    t.section  = section;
    t.email    = generateEmail(name);
    t.password = "pass_" + t.id;
    m_tas.append(t);
    saveTAs();
}

void SchoolSystem::addAdmin(const QString& name)
{
    int maxId = 0;
    for (const auto& a : m_admins) {
        int n = a.id.mid(1).toInt();
        if (n > maxId) maxId = n;
    }
    AdminData a;
    a.id       = generateID(maxId, "A");
    a.name     = name;
    a.email    = generateEmail(name);
    a.password = "pass_" + a.id;
    m_admins.append(a);
    saveAdmins();
}

bool SchoolSystem::deleteStudent(const QString& id)
{
    for (int i = 0; i < m_students.size(); ++i) {
        if (m_students[i].id == id) { m_students.remove(i); saveStudents(); return true; }
    }
    return false;
}
bool SchoolSystem::deleteProfessor(const QString& id)
{
    for (int i = 0; i < m_profs.size(); ++i) {
        if (m_profs[i].id == id) { m_profs.remove(i); saveProfessors(); return true; }
    }
    return false;
}
bool SchoolSystem::deleteTA(const QString& id)
{
    for (int i = 0; i < m_tas.size(); ++i) {
        if (m_tas[i].id == id) { m_tas.remove(i); saveTAs(); return true; }
    }
    return false;
}
bool SchoolSystem::deleteAdmin(const QString& id)
{
    for (int i = 0; i < m_admins.size(); ++i) {
        if (m_admins[i].id == id) { m_admins.remove(i); saveAdmins(); return true; }
    }
    return false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Grade operations
// ─────────────────────────────────────────────────────────────────────────────
bool SchoolSystem::profSetGrade(const QString& studentId, const QString& course, int grade)
{
    int idx = courseCodeToIndex(course);
    if (idx == -1 || grade < 0 || grade > 20) return false;
    for (auto& s : m_students) {
        if (s.id == studentId) { s.GP[idx] = grade; saveStudents(); return true; }
    }
    return false;
}

bool SchoolSystem::taSetGrade(const QString& studentId, const QString& course,
                               const QString& section, int grade)
{
    int idx = courseCodeToIndex(course);
    if (idx == -1 || grade < 0 || grade > 10) return false;
    for (auto& s : m_students) {
        if (s.id == studentId && s.section == section) {
            s.GTA[idx] = grade; saveStudents(); return true;
        }
    }
    return false;
}

double SchoolSystem::computeGPA(const Student& s)
{
    double total = 0;
    for (int i = 0; i < 5; ++i) {
        double courseTotal  = s.GP[i] + s.GTA[i];
        double normalized   = courseTotal / 30.0;
        total += normalized * 4.0;
    }
    double gpa = total / 5.0;
    return std::round(gpa * 100.0) / 100.0;
}
