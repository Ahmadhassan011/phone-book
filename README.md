# Phone Book Project

## Introduction
This project is a simple phone book application that allows users to store, search, and manage contact information efficiently.

## Features
- Add new contacts
- Delete contacts
- Search for contacts by name or phone number
- Prefix search for contacts
- Display all contacts
- Display call history (most calls, recent calls, longest calls)

## Installation
1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/phone-book.git
    ```
2. Navigate to the project directory:
    ```sh
    cd phone-book
    ```
3. Compile the project:
    ```sh
    g++ -Iinclude -o phonebook src/*.cpp main.cpp
    ```

## Usage
1. Run the application:
    ```sh
    ./phonebook
    ```
2. Follow the on-screen instructions to manage your contacts and call history.

## File Structure
- [main.cpp](/main.cpp): Entry point of the application
- [include](/include/): Header files
- [src](/src/): Source files
- [contacts.csv](/contacts.csv): Stores contact information
- [callHistory.csv](/callHistory.csv): Stores call history
