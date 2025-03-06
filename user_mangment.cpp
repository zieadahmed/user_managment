#include <iostream>
#include <vector>
#include <string>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

#define MAX_USERS 10

struct User {
    string username;
    string password;
};

vector<User> users;

void register_user();
int login_user();
void input_credentials(string& username, string& password);

int main() {
    int option;
    while (true) {
        cout << "\nWelcome to User Management";
        cout << "\n1. Register";
        cout << "\n2. Login";
        cout << "\n3. Exit";
        cout << "\nSelect an option: ";
        cin >> option;
        cin.ignore(); // Consume extra newline

        switch (option) {
            case 1:
                register_user();
                break;
            case 2: {
                int user_index = login_user();
                if (user_index >= 0) {
                    cout << "\nLogin successful! Welcome, " << users[user_index].username << "!\n";
                } else {
                    cout << "\nLogin failed! Incorrect username or password.\n";
                }
                break;
            }
            case 3:
                cout << "\nExiting Program.\n";
                return 0;
            default:
                cout << "\nInvalid option. Please try again.\n";
                break;
        }
    }
    return 0;
}

void register_user() {
    if (users.size() >= MAX_USERS) {
        cout << "\nMaximum " << MAX_USERS << " users are supported! No more registrations allowed!\n";
        return;
    }

    User newUser;
    cout << "\nRegister a new user";
    input_credentials(newUser.username, newUser.password);
    users.push_back(newUser);
    cout << "\nRegistration successful!\n";
}

int login_user() {
    string username, password;
    input_credentials(username, password);

    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].username == username && users[i].password == password) {
            return i;
        }
    }
    return -1;
}

void input_credentials(string& username, string& password) {
    cout << "\nEnter username: ";
    getline(cin, username);

    cout << "Enter password (masking enabled): ";
    fflush(stdout);
    password = "";

#ifdef _WIN32
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !password.empty()) {
            cout << "\b \b";
            password.pop_back();
        } else if (ch != '\b') {
            password += ch;
            cout << "*";
        }
    }
#else
    struct termios old_props, new_props;
    tcgetattr(STDIN_FILENO, &old_props);
    new_props = old_props;
    new_props.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_props);
    
    getline(cin, password);
    
    tcsetattr(STDIN_FILENO, TCSANOW, &old_props);
#endif

    cout << "\n";
}

