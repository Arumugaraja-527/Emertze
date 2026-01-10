```md
# 📒 Address Book Management System (C)

A command-line Address Book application written in C that allows users to add, search, edit, delete, and persist contact information using file-based storage.

This project demonstrates real-world C programming practices including structures, file handling, modular design, and menu-driven applications. It is suitable for academic projects, interviews, and embedded systems foundations.

---

## 📌 Overview

The Address Book Management System provides a simple and efficient way to manage contact details through a terminal-based interface.
All contact data is stored persistently in a CSV file, ensuring data is retained even after the program terminates.

---

## 🚀 Key Highlights

- Pure C language implementation
- Persistent storage using CSV file
- Clean modular architecture
- Menu-driven interface
- Beginner-friendly and interview-ready
- No external libraries required

---

## ✨ Features

- Add new contacts
- Search contacts by name, phone number, or email
- Edit existing contact details
- Delete contacts
- Display all stored contacts
- Data persistence using file handling

---

## 🛠️ Tech Stack

Language: C  
Compiler: GCC  
Platform: Linux / Windows  

Core Concepts Used:
- Structures
- File Handling
- Modular Programming
- Header Files
- Pointers and Arrays
- Input Validation

---

## 📂 Project Structure

AddressBook/
│
├── main.c        # Application entry point & menu logic
├── contact.c     # Contact-related operations
├── file.c        # File read/write handling
├── contact.h     # Contact structure & function prototypes
├── file.h        # File operation prototypes
├── contacts.csv  # Persistent contact storage
└── README.md     # Project documentation

---

## ⚙️ Build Instructions

Compile the program using GCC:

gcc main.c contact.c file.c -o addressbook

---

## ▶️ Running the Application

Execute the compiled binary:

./addressbook

---

## 📋 Application Menu

1. Add Contact  
2. Search Contact  
3. Edit Contact  
4. Delete Contact  
5. Display All Contacts  
6. Exit  

---

## 🧠 Internal Design

Data Representation:
Each contact is represented using a struct.
Fields include Name, Phone Number, and Email ID.

Program Flow:
User selects an option from the menu.
The corresponding module function is executed.
Data is read from or written to contacts.csv.
Output is displayed to the user.

---

## 💾 File Storage Strategy

Contacts are stored in CSV (Comma-Separated Values) format.
This ensures data persistence and allows easy manual inspection if required.

---

## ⚠️ Error Handling & Validation

The program handles invalid menu choices, empty address book conditions, file access failures, and incorrect user input formats.
Clear messages guide the user during execution.

---

## 🎯 Learning Outcomes

This project helps in understanding file-based data management in C, structure-oriented programming, modular application design, real-world problem solving, and writing clean and maintainable code.

---

## 📈 Future Enhancements

Sorting contacts alphabetically
Import/export contacts
Password-protected address book
Binary file storage
GUI or mobile application interface

---

## 👨‍💻 Author

Arumugaraja K  
Embedded Systems & C Programming Enthusiast  
Focused on low-level programming and core C concepts

---

## ⭐ Support

If you found this project useful, consider giving the repository a star ⭐ to show your support.
```
