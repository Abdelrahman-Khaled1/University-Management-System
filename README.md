# School Management System — Qt GUI

A complete Qt 5/6 GUI front-end for the School Management System.

## Project Structure

```
SchoolSystem/
├── SchoolSystem.pro        # qmake project file
├── main.cpp
├── models.h                # Data structures (Student, Professor, TA, …)
├── schoolsystem.h/.cpp     # Business logic + file I/O
├── mainwindow.h/.cpp       # App shell + login routing
├── logindialog.h/.cpp      # Role-selector + credentials dialog
├── studentdashboard.h/.cpp # Student view: profile, grades, change pwd/phone
├── profdashboard.h/.cpp    # Professor view: student table, grade editor, delete
├── tadashboard.h/.cpp      # TA view: section students, TA grade editor
└── admindashboard.h/.cpp   # Admin tabs: full CRUD for all roles
```

## Build Instructions

### Prerequisites
- Qt 5.12+ or Qt 6 (Widgets module)
- A C++17 compiler (gcc/clang/MSVC)

### Option A — Qt Creator (recommended)
1. Open **Qt Creator**.
2. **File → Open File or Project** → select `SchoolSystem.pro`.
3. Configure a kit (Desktop Qt 5 or 6).
4. Click **Build** (Ctrl+B) then **Run** (Ctrl+R).

### Option B — Command line
```bash
cd SchoolSystem
qmake SchoolSystem.pro
make          # or: mingw32-make on Windows
./SchoolSystem
```

## Data Files
The app reads the same CSV files as the original program. Place them in the
**working directory** (usually the build output folder):

| File            | Contents            |
|-----------------|---------------------|
| `Students.txt`  | Student records      |
| `professors.txt`| Professor records    |
| `tas.txt`       | TA records           |
| `admins.txt`    | Admin accounts       |
| `courses.txt`   | One course code/line |

If a file is missing the app starts with an empty list for that role and will
create the file on the first save.

## Default Passwords
Auto-generated passwords follow the pattern `pass_<ID>` (e.g. `pass_S001`).
Students can change their password from the dashboard (must satisfy the
complexity rule: 6–20 chars, upper + lower + digit + one of `$#@`).

## Features by Role

### Student
- View profile (ID, name, section, email, phone)
- View grades per course (Prof grade 0-20 + TA grade 0-10 + total)
- Live GPA display (colour-coded: green ≥ 3.0, amber ≥ 2.0, red < 2.0)
- Change password (with validation)
- Change phone (11-digit validation)

### Professor
- View profile
- Scrollable table of all students with their Prof grade and GPA
- Set/update a student's Prof grade for the professor's course
- Delete any student

### TA
- View profile
- Table of students **in the TA's section** with grades
- Set/update a student's TA grade (only within own section)

### Admin
- Tabbed interface for Students / Professors / TAs / Admins
- Each tab: live table + Add form + Delete by ID
- New records report generated ID and default password in a dialog


## Authors

This project was developed by:

- **Abdelrahman Elsayed**
- **Abdelaziz Amr**
- **Ahmad Saied**