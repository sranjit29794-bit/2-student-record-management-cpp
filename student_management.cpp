#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

class Student {
private:
    int id, age;
    string name;
    float marks;

public:
    // Setters
    void setId(int i)       { id = i; }
    void setAge(int a)      { age = a; }
    void setName(string n)  { name = n; }
    void setMarks(float m)  { marks = m; }

    // Getters
    int    getId()    { return id; }
    int    getAge()   { return age; }
    string getName()  { return name; }
    float  getMarks() { return marks; }

    void inputData() {
        cout << "\nEnter Student ID: ";  cin >> id;
        cin.ignore();
        cout << "Enter Name: ";          getline(cin, name);
        cout << "Enter Age: ";           cin >> age;
        cout << "Enter Marks: ";         cin >> marks;
    }

    void displayRow() {
        cout << setw(6)  << left << id
             << setw(20) << left << name
             << setw(6)  << left << age
             << setw(8)  << left << fixed << setprecision(2) << marks << "\n";
    }

    string serialize() {
        return to_string(id) + "|" + name + "|" + to_string(age) + "|" + to_string(marks);
    }

    bool deserialize(const string& line) {
        stringstream ss(line);
        string t;
        try {
            if (!getline(ss, t, '|')) return false; id    = stoi(t);
            if (!getline(ss, name, '|')) return false;
            if (!getline(ss, t, '|')) return false; age   = stoi(t);
            if (!getline(ss, t))       return false; marks = stof(t);
        } catch (...) { return false; }
        return true;
    }
};

class StudentManager {
private:
    const string filename = "students.txt";
    const string tempfile = "temp.txt";

public:
    void addStudent() {
        Student s;
        s.inputData();

        ofstream file(filename, ios::app);
        if (!file) { cout << "Error: cannot open file.\n"; return; }
        file << s.serialize() << "\n";
        cout << "\nStudent Added Successfully!\n";
    }

    void displayStudents() {
        ifstream file(filename);
        if (!file) { cout << "\nNo records found.\n"; return; }

        cout << "\n"
             << setw(6)  << left << "ID"
             << setw(20) << left << "Name"
             << setw(6)  << left << "Age"
             << setw(8)  << left << "Marks" << "\n";
        cout << string(40, '-') << "\n";

        string line;
        Student s;
        while (getline(file, line)) {
            if (s.deserialize(line))
                s.displayRow();
        }
    }

    void updateStudent() {
        int searchId;
        cout << "\nEnter Student ID to Update: "; cin >> searchId;

        ifstream file(filename);
        if (!file) { cout << "No records found.\n"; return; }
        ofstream temp(tempfile);

        string line;
        Student s;
        bool found = false;

        while (getline(file, line)) {
            if (!s.deserialize(line)) continue;
            if (s.getId() == searchId) {
                found = true;
                cout << "\nEnter New Name: ";  cin.ignore(); 
                string n; getline(cin, n); s.setName(n);
                int a;    cout << "Enter New Age: ";   cin >> a; s.setAge(a);
                float m;  cout << "Enter New Marks: "; cin >> m; s.setMarks(m);
            }
            temp << s.serialize() << "\n";
        }

        file.close(); temp.close();
        remove(filename.c_str()); rename(tempfile.c_str(), filename.c_str());
        cout << (found ? "\nRecord Updated Successfully!\n" : "\nStudent ID Not Found!\n");
    }

    void deleteStudent() {
        int searchId;
        cout << "\nEnter Student ID to Delete: "; cin >> searchId;

        ifstream file(filename);
        if (!file) { cout << "No records found.\n"; return; }
        ofstream temp(tempfile);

        string line;
        Student s;
        bool found = false;

        while (getline(file, line)) {
            if (!s.deserialize(line)) continue;
            if (s.getId() == searchId) { found = true; continue; }
            temp << s.serialize() << "\n";
        }

        file.close(); temp.close();
        remove(filename.c_str()); rename(tempfile.c_str(), filename.c_str());
        cout << (found ? "\nRecord Deleted Successfully!\n" : "\nStudent ID Not Found!\n");
    }

    void menu() {
        int choice;
        do {
            cout << "\n===== STUDENT MANAGEMENT SYSTEM =====\n"
                 << "1. Add Student\n"
                 << "2. Display Students\n"
                 << "3. Update Student\n"
                 << "4. Delete Student\n"
                 << "5. Exit\n"
                 << "Enter Choice: ";
            cin >> choice;
            switch (choice) {
                case 1: addStudent();     break;
                case 2: displayStudents(); break;
                case 3: updateStudent();  break;
                case 4: deleteStudent();  break;
                case 5: cout << "\nThank You!\n"; break;
                default: cout << "\nInvalid Choice!\n";
            }
        } while (choice != 5);
    }
};

int main() {
    StudentManager sm;
    sm.menu();
    return 0;
}