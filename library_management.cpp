#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <limits>
using namespace std;

class Book {
private:
    int id;
    string title;
    string author;
    bool issued;
    int issuedTo;

public:
    Book() : id(0), issued(false), issuedTo(0) {}

    void addBook() {
        cout << "\nBook ID     : ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Book Title  : ";
        getline(cin, title);

        cout << "Author      : ";
        getline(cin, author);

        issued = false;
        issuedTo = 0;
    }

    void display() const {
        cout << left
             << setw(8)  << id
             << setw(28) << title.substr(0, 26)
             << setw(22) << author.substr(0, 20)
             << (issued ? "Issued" : "Available") << '\n';
    }

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool isIssued() const { return issued; }

    void issue(int memberId) {
        issued = true;
        issuedTo = memberId;
    }

    void returnBook() {
        issued = false;
        issuedTo = 0;
    }
};

class Member {
private:
    int id;
    string name;

public:
    Member() : id(0) {}

    void addMember() {
        cout << "\nMember ID   : ";
        cin >> id;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Member Name : ";
        getline(cin, name);
    }

    void display() const {
        cout << left
             << setw(12) << id
             << name << '\n';
    }

    int getId() const { return id; }
};

// ---------- FILE OPERATIONS ----------

void addBook() {
    Book book;
    book.addBook();

    ofstream file("books.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&book), sizeof(book));
    file.close();

    cout << "\n✓ Book added successfully.\n";
}

void displayBooks() {
    Book book;
    ifstream file("books.dat", ios::binary);

    cout << "\n==================== BOOK CATALOG ====================\n";
    cout << left
         << setw(8)  << "ID"
         << setw(28) << "TITLE"
         << setw(22) << "AUTHOR"
         << "STATUS\n";
    cout << "-------------------------------------------------------\n";

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book))) {
        book.display();
        found = true;
    }

    if (!found)
        cout << "No books available.\n";

    file.close();
}

void addMember() {
    Member member;
    member.addMember();

    ofstream file("members.dat", ios::binary | ios::app);
    file.write(reinterpret_cast<char*>(&member), sizeof(member));
    file.close();

    cout << "\n✓ Member registered successfully.\n";
}

void displayMembers() {
    Member member;
    ifstream file("members.dat", ios::binary);

    cout << "\n================ MEMBERS =================\n";
    cout << left << setw(12) << "MEMBER ID"
         << "NAME\n";
    cout << "------------------------------------------\n";

    bool found = false;

    while (file.read(reinterpret_cast<char*>(&member), sizeof(member))) {
        member.display();
        found = true;
    }

    if (!found)
        cout << "No members registered.\n";

    file.close();
}

// ---------- SEARCH ----------

void searchBook() {
    string keyword;
    Book book;
    bool found = false;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter title or author: ";
    getline(cin, keyword);

    ifstream file("books.dat", ios::binary);

    cout << "\n================ SEARCH RESULTS ================\n";

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book))) {
        if (book.getTitle().find(keyword) != string::npos ||
            book.getAuthor().find(keyword) != string::npos) {

            book.display();
            found = true;
        }
    }

    file.close();

    if (!found)
        cout << "No matching books found.\n";
}

// ---------- ISSUE BOOK ----------

void issueBook() {
    int bookId, memberId;
    Book book;

    cout << "\nBook ID   : ";
    cin >> bookId;

    cout << "Member ID : ";
    cin >> memberId;

    fstream file("books.dat", ios::binary | ios::in | ios::out);

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book))) {

        if (book.getId() == bookId) {

            if (book.isIssued()) {
                cout << "\n✗ Book is already issued.\n";
                file.close();
                return;
            }

            book.issue(memberId);

            file.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
            file.write(reinterpret_cast<char*>(&book), sizeof(book));

            file.close();

            cout << "\n✓ Book issued successfully.\n";
            return;
        }
    }

    file.close();
    cout << "\n✗ Book not found.\n";
}

// ---------- RETURN BOOK ----------

void returnBook() {
    int bookId;
    Book book;

    cout << "\nEnter Book ID: ";
    cin >> bookId;

    fstream file("books.dat", ios::binary | ios::in | ios::out);

    while (file.read(reinterpret_cast<char*>(&book), sizeof(book))) {

        if (book.getId() == bookId) {

            if (!book.isIssued()) {
                cout << "\n✗ This book is already available.\n";
                file.close();
                return;
            }

            book.returnBook();

            file.seekp(-static_cast<int>(sizeof(Book)), ios::cur);
            file.write(reinterpret_cast<char*>(&book), sizeof(book));

            file.close();

            cout << "\n✓ Book returned successfully.\n";
            return;
        }
    }

    file.close();
    cout << "\n✗ Book not found.\n";
}

// ---------- MAIN MENU ----------

int main() {
    int choice;

    do {
        cout << "\n\n";
        cout << "╔══════════════════════════════════════════╗\n";
        cout << "║          SMART LIBRARY SYSTEM            ║\n";
        cout << "╠══════════════════════════════════════════╣\n";
        cout << "║  1. Add Book                             ║\n";
        cout << "║  2. View All Books                       ║\n";
        cout << "║  3. Add Member                           ║\n";
        cout << "║  4. View Members                         ║\n";
        cout << "║  5. Search Book                          ║\n";
        cout << "║  6. Issue Book                           ║\n";
        cout << "║  7. Return Book                          ║\n";
        cout << "║  0. Exit                                 ║\n";
        cout << "╚══════════════════════════════════════════╝\n";

        cout << "\nSelect Option: ";
        cin >> choice;

        switch (choice) {
            case 1: addBook(); break;
            case 2: displayBooks(); break;
            case 3: addMember(); break;
            case 4: displayMembers(); break;
            case 5: searchBook(); break;
            case 6: issueBook(); break;
            case 7: returnBook(); break;

            case 0:
                cout << "\n✓ Library System Closed. Goodbye!\n";
                break;

            default:
                cout << "\n✗ Invalid option. Try again.\n";
        }

    } while (choice != 0);

    return 0;
}