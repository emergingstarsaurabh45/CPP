#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <cstdio>
using namespace std;

struct Student {
    int id;
    string name;
    string branch;
    float marks[5];
};

// Calculate average
float average(const Student& s) {
    float total = 0;
    for (float mark : s.marks)
        total += mark;
    return total / 5;
}

// Grade calculation
char grade(float avg) {
    if (avg >= 90) return 'A';
    if (avg >= 80) return 'B';
    if (avg >= 70) return 'C';
    if (avg >= 60) return 'D';
    return 'F';
}

// Add student
void addStudent() {
    Student s;

    cout << "\nStudent ID: ";
    cin >> s.id;
    cin.ignore();

    cout << "Full Name: ";
    getline(cin, s.name);

    cout << "Branch: ";
    getline(cin, s.branch);

    cout << "\nEnter marks for 5 subjects:\n";
    for (int i = 0; i < 5; i++) {
        cout << "Subject " << i + 1 << ": ";
        cin >> s.marks[i];
    }

    ofstream file("students.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&s), sizeof(s));
    file.close();

    cout << "\n✓ Student registered successfully!\n";
}

// Display all students
void displayStudents() {
    Student s;
    ifstream file("students.dat", ios::binary);

    cout << "\n================ STUDENT DIRECTORY ================\n";
    cout << left << setw(8) << "ID"
         << setw(22) << "NAME"
         << setw(18) << "BRANCH"
         << setw(10) << "AVG"
         << "GRADE\n";
    cout << "---------------------------------------------------\n";

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        found = true;

        float avg = average(s);

        cout << left << setw(8) << s.id
             << setw(22) << s.name.substr(0, 20)
             << setw(18) << s.branch.substr(0, 16)
             << setw(10) << fixed << setprecision(2) << avg
             << grade(avg) << '\n';
    }

    if (!found)
        cout << "No student records available.\n";

    file.close();
}

// Search student
void searchStudent() {
    int id;
    Student s;

    cout << "\nEnter Student ID: ";
    cin >> id;

    ifstream file("students.dat", ios::binary);
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == id) {
            float avg = average(s);

            cout << "\n========== STUDENT PROFILE ==========\n";
            cout << "ID       : " << s.id << '\n';
            cout << "Name     : " << s.name << '\n';
            cout << "Branch   : " << s.branch << '\n';
            cout << "Average  : " << fixed << setprecision(2) << avg << '\n';
            cout << "Grade    : " << grade(avg) << '\n';

            cout << "\nSubject Marks:\n";
            for (int i = 0; i < 5; i++)
                cout << "Subject " << i + 1 << " : " << s.marks[i] << '\n';

            found = true;
            break;
        }
    }

    file.close();

    if (!found)
        cout << "\n✗ Student not found.\n";
}

// Update student
void updateStudent() {
    int id;
    Student s;

    cout << "\nEnter Student ID to update: ";
    cin >> id;

    fstream file("students.dat",
                 ios::binary | ios::in | ios::out);

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == id) {
            cin.ignore();

            cout << "New Name: ";
            getline(cin, s.name);

            cout << "New Branch: ";
            getline(cin, s.branch);

            cout << "\nEnter new marks:\n";
            for (int i = 0; i < 5; i++) {
                cout << "Subject " << i + 1 << ": ";
                cin >> s.marks[i];
            }

            file.seekp(-static_cast<int>(sizeof(Student)), ios::cur);
            file.write(reinterpret_cast<char*>(&s), sizeof(s));

            found = true;
            cout << "\n✓ Record updated successfully!\n";
            break;
        }
    }

    file.close();

    if (!found)
        cout << "\n✗ Student not found.\n";
}

// Delete student
void deleteStudent() {
    int id;
    Student s;

    cout << "\nEnter Student ID to delete: ";
    cin >> id;

    ifstream input("students.dat", ios::binary);
    ofstream temp("temp.dat", ios::binary);

    bool found = false;

    while (input.read(reinterpret_cast<char*>(&s), sizeof(s))) {
        if (s.id == id) {
            found = true;
            continue;
        }

        temp.write(reinterpret_cast<char*>(&s), sizeof(s));
    }

    input.close();
    temp.close();

    remove("students.dat");
    rename("temp.dat", "students.dat");

    cout << (found
        ? "\n✓ Student record deleted.\n"
        : "\n✗ Student not found.\n");
}

// Main
int main() {
    int choice;

    do {
        cout << "\n\n";
        cout << "╔══════════════════════════════════════╗\n";
        cout << "║       SMART STUDENT MANAGEMENT       ║\n";
        cout << "╠══════════════════════════════════════╣\n";
        cout << "║  1. Register Student                 ║\n";
        cout << "║  2. View Student Directory           ║\n";
        cout << "║  3. Search Student                   ║\n";
        cout << "║  4. Update Student                   ║\n";
        cout << "║  5. Delete Student                   ║\n";
        cout << "║  0. Exit                             ║\n";
        cout << "╚══════════════════════════════════════╝\n";

        cout << "\nSelect option: ";
        cin >> choice;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0:
                cout << "\nSystem closed. Goodbye!\n";
                break;
            default:
                cout << "\nInvalid option.\n";
        }

    } while (choice != 0);

    return 0;
}