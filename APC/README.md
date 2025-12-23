# 🧮 APC – Arbitrary Precision Calculator

APC (Arbitrary Precision Calculator) is a **command-line application written in C** that performs **mathematical operations on extremely large integers**, far beyond the limits of standard C data types like `int`, `long`, or `long long`.

This project demonstrates how **big-number arithmetic** can be implemented using **linked lists** and low-level programming concepts.

---

## 📖 Table of Contents

- [Introduction](#introduction)
- [Why APC?](#why-apc)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Project Structure](#project-structure)
- [Compilation](#compilation)
- [Usage](#usage)
- [Examples](#examples)
- [How It Works](#how-it-works)
- [Error Handling](#error-handling)
- [Learning Outcomes](#learning-outcomes)
- [Author](#author)

---

## 📌 Introduction

In C programming, numeric data types have **fixed size limits**. APC overcomes this limitation by storing numbers **digit-by-digit in linked lists**, allowing calculations on numbers with **hundreds or even thousands of digits**.

---

## ❓ Why APC?

| Data Type | Maximum Digits |
|---------|----------------|
| `int` | ~10 |
| `long long` | ~19 |
| **APC** | ♾️ Unlimited |

👉 APC is useful for **educational purposes**, **algorithm understanding**, and **low-level arithmetic implementation**.

---

## ✨ Features

- ➕ Addition of very large numbers  
- ➖ Subtraction of very large numbers  
- ✖️ Multiplication of very large numbers  
- ➗ Division of very large numbers  
- 🔢 Supports **positive and negative integers**
- 📦 Uses **Linked Lists** for number storage
- ⚙️ Pure C implementation (no big integer libraries)
- 💻 Command-line based interface

---

## 🛠️ Technologies Used

- **Language:** C  
- **Compiler:** GCC  
- **Concepts:**
  - Linked Lists  
  - Dynamic Memory Allocation  
  - Pointers  
  - Modular Programming  
  - Command Line Arguments  

---

## 📂 Project Structure

APC/
main.c        → Program entry point  
add.c         → Addition logic  
sub.c         → Subtraction logic  
mul.c         → Multiplication logic  
div.c         → Division logic  
list.c        → Linked list helper functions  
apc.h         → Header file (structures & prototypes)  
README.md     → Project documentation  

---

## ⚙️ Compilation

Compile all source files using GCC:

```bash
gcc *.c -o apc
```

---

## ▶️ Usage

Run the program using the format:

```bash
./apc <operand1> <operator> <operand2>
```

### 🔢 Supported Operators

| Operator | Operation |
|--------|-----------|
| `+` | Addition |
| `-` | Subtraction |
| `x` | Multiplication |
| `/` | Division |

---

## 🧪 Examples

### ➕ Addition
```bash
./apc 123456789123456789 + 987654321987654321
```

### ➖ Subtraction
```bash
./apc 1000000000000 - 999999999999
```

### ✖️ Multiplication
```bash
./apc 123456789 x 987654321
```

### ➗ Division
```bash
./apc 1000000000 / 25
```

---

## 🧠 How It Works

### 🔹 Number Representation
- Each digit is stored in a node of a linked list
- Numbers are processed digit by digit

### 🔹 Arithmetic Logic
- Operations are performed like manual calculations
- Carry and borrow are handled explicitly
- Division uses repeated subtraction logic

### 🔹 Memory Management
- Uses `malloc()` for dynamic allocation
- Uses `free()` to avoid memory leaks

---

## ❗ Error Handling

The program handles:
- Invalid operators  
- Division by zero  
- Non-numeric inputs  
- Missing command-line arguments  

Clear error messages are displayed to guide the user.

---

## 🎯 Learning Outcomes

This project helps you learn:
- Arbitrary precision arithmetic
- Linked list based number processing
- Real-world C programming
- Writing modular and maintainable code

---

## 👨‍💻 Author

**Arumuga Raja**  
Embedded Systems & C Programming Enthusiast  
Passionate about low-level programming and problem solving  

---

## ⭐ Support

If you like this project, don’t forget to **star ⭐ the repository**!

