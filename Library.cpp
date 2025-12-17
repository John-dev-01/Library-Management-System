#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Book {
private:
    string title;
    string author;
    string isbn;
    bool available;

public:
    Book(string t, string a, string i) {
        title = t;
        author = a;
        isbn = i;
        available = true;
    }

    string getTitle() {
        return title;
    }

    string getAuthor() {
        return author;
    }

    string getISBN() {
        return isbn;
    }

    bool isAvailable() {
        return available;
    }

    void setAvailable(bool status) {
        available = status;
    }

    void setTitle(string t) {
        title = t;
    }

    void setAuthor(string a) {
        author = a;
    }

    void setISBN(string i) {
        isbn = i;
    }
};

class LibraryUser {
private:
    string userID;
    string name;
    vector<string> borrowedBooks;

public:
    LibraryUser(string id, string n) {
        userID = id;
        name = n;
    }

    string getUserID() {
        return userID;
    }

    string getName() {
        return name;
    }

    vector<string> getBorrowedBooks() {
        return borrowedBooks;
    }

    void setUserID(string id) {
        userID = id;
    }

    void setName(string n) {
        name = n;
    }

    void borrowBook(string isbn) {
        borrowedBooks.push_back(isbn);
    }

    void returnBook(string isbn) {
        for(int i = 0; i < borrowedBooks.size(); i++) {
            if(borrowedBooks[i] == isbn) {
                borrowedBooks.erase(borrowedBooks.begin() + i);
                break;
            }
        }
    }

    void displayBorrowedBooks() {
        if(borrowedBooks.size() == 0) {
            cout << "No borrowed books" << endl;
        } else {
            for(int i = 0; i < borrowedBooks.size(); i++) {
                cout << borrowedBooks[i] << endl;
            }
        }
    }
};

class Library {
private:
    vector<Book> books;
    vector<LibraryUser> users;

public:
    Library() {
        loadBooksFromFile();
        loadUsersFromFile();
    }

    void loadBooksFromFile() {
        ifstream file("books.txt");
        if(file.is_open()) {
            string title, author, isbn;
            bool available;
            while(getline(file, title)) {
                getline(file, author);
                getline(file, isbn);
                file >> available;
                file.ignore();
                Book b(title, author, isbn);
                b.setAvailable(available);
                books.push_back(b);
            }
            file.close();
        }
    }

    void loadUsersFromFile() {
        ifstream file("users.txt");
        if(file.is_open()) {
            string userID, name;
            while(getline(file, userID)) {
                getline(file, name);
                LibraryUser u(userID, name);
                int count;
                file >> count;
                file.ignore();
                for(int i = 0; i < count; i++) {
                    string isbn;
                    getline(file, isbn);
                    u.borrowBook(isbn);
                }
                users.push_back(u);
            }
            file.close();
        }
    }

    void saveBooksToFile() {
        ofstream file("books.txt");
        for(int i = 0; i < books.size(); i++) {
            file << books[i].getTitle() << endl;
            file << books[i].getAuthor() << endl;
            file << books[i].getISBN() << endl;
            file << books[i].isAvailable() << endl;
        }
        file.close();
    }

    void saveUsersToFile() {
        ofstream file("users.txt");
        for(int i = 0; i < users.size(); i++) {
            file << users[i].getUserID() << endl;
            file << users[i].getName() << endl;
            vector<string> borrowed = users[i].getBorrowedBooks();
            file << borrowed.size() << endl;
            for(int j = 0; j < borrowed.size(); j++) {
                file << borrowed[j] << endl;
            }
        }
        file.close();
    }

    void addBook(string title, string author, string isbn) {
        Book b(title, author, isbn);
        books.push_back(b);
        saveBooksToFile();
        cout << "Book added successfully" << endl;
    }

    void removeBook(string isbn) {
        for(int i = 0; i < books.size(); i++) {
            if(books[i].getISBN() == isbn) {
                books.erase(books.begin() + i);
                saveBooksToFile();
                cout << "Book removed successfully" << endl;
                return;
            }
        }
        cout << "Book not found" << endl;
    }

    void registerUser(string userID, string name) {
        LibraryUser u(userID, name);
        users.push_back(u);
        saveUsersToFile();
        cout << "User registered successfully" << endl;
    }

    void removeUser(string userID) {
        for(int i = 0; i < users.size(); i++) {
            if(users[i].getUserID() == userID) {
                users.erase(users.begin() + i);
                saveUsersToFile();
                cout << "User removed successfully" << endl;
                return;
            }
        }
        cout << "User not found" << endl;
    }

    void displayAllBooks() {
        cout << "All Books:" << endl;
        for(int i = 0; i < books.size(); i++) {
            cout << "Title: " << books[i].getTitle() << endl;
            cout << "Author: " << books[i].getAuthor() << endl;
            cout << "ISBN: " << books[i].getISBN() << endl;
            if(books[i].isAvailable()) {
                cout << "Status: Available" << endl;
            } else {
                cout << "Status: Borrowed" << endl;
            }
            cout << endl;
        }
    }

    void displayAllUsers() {
        cout << "All Users:" << endl;
        for(int i = 0; i < users.size(); i++) {
            cout << "User ID: " << users[i].getUserID() << endl;
            cout << "Name: " << users[i].getName() << endl;
            cout << endl;
        }
    }

    void borrowBook(string isbn, string userID) {
        int bookIndex = -1;
        int userIndex = -1;

        for(int i = 0; i < books.size(); i++) {
            if(books[i].getISBN() == isbn) {
                bookIndex = i;
                break;
            }
        }

        for(int i = 0; i < users.size(); i++) {
            if(users[i].getUserID() == userID) {
                userIndex = i;
                break;
            }
        }

        if(bookIndex == -1) {
            cout << "Book not found" << endl;
            return;
        }

        if(userIndex == -1) {
            cout << "User not found" << endl;
            return;
        }

        if(!books[bookIndex].isAvailable()) {
            cout << "Book is already borrowed" << endl;
            return;
        }

        books[bookIndex].setAvailable(false);
        users[userIndex].borrowBook(isbn);
        saveBooksToFile();
        saveUsersToFile();
        cout << "Book borrowed successfully" << endl;
    }

    void returnBook(string isbn, string userID) {
        int bookIndex = -1;
        int userIndex = -1;

        for(int i = 0; i < books.size(); i++) {
            if(books[i].getISBN() == isbn) {
                bookIndex = i;
                break;
            }
        }

        for(int i = 0; i < users.size(); i++) {
            if(users[i].getUserID() == userID) {
                userIndex = i;
                break;
            }
        }

        if(bookIndex == -1) {
            cout << "Book not found" << endl;
            return;
        }

        if(userIndex == -1) {
            cout << "User not found" << endl;
            return;
        }

        vector<string> borrowed = users[userIndex].getBorrowedBooks();
        bool hasBorrowed = false;
        for(int i = 0; i < borrowed.size(); i++) {
            if(borrowed[i] == isbn) {
                hasBorrowed = true;
                break;
            }
        }

        if(!hasBorrowed) {
            cout << "User has not borrowed this book" << endl;
            return;
        }

        books[bookIndex].setAvailable(true);
        users[userIndex].returnBook(isbn);
        saveBooksToFile();
        saveUsersToFile();
        cout << "Book returned successfully" << endl;
    }

    void displayBorrowedBooks(string userID) {
        int userIndex = -1;
        for(int i = 0; i < users.size(); i++) {
            if(users[i].getUserID() == userID) {
                userIndex = i;
                break;
            }
        }

        if(userIndex == -1) {
            cout << "User not found" << endl;
            return;
        }

        cout << "Borrowed books for " << users[userIndex].getName() << ":" << endl;
        vector<string> borrowed = users[userIndex].getBorrowedBooks();
        if(borrowed.size() == 0) {
            cout << "No borrowed books" << endl;
        } else {
            for(int i = 0; i < borrowed.size(); i++) {
                for(int j = 0; j < books.size(); j++) {
                    if(books[j].getISBN() == borrowed[i]) {
                        cout << books[j].getTitle() << endl;
                        break;
                    }
                }
            }
        }
    }
};

int main() {
    Library library;

    library.addBook("1984", "George Orwell", "978-0451524935");
    library.addBook("To Kill a Mockingbird", "Harper Lee", "978-0061120084");

    library.registerUser("001", "Alice");
    library.registerUser("002", "Bob");

    library.borrowBook("978-0451524935", "001");
    library.borrowBook("978-0061120084", "002");

    library.displayAllBooks();
    library.displayAllUsers();

    library.returnBook("978-0451524935", "001");

    library.displayBorrowedBooks("001");

    return 0;
}
