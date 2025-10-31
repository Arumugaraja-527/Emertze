#ifndef CONTACT_H
#define CONTACT_H

#define MAX_CONTACTS 100
#define NAME_LENGTH 50
#define PHONE_LENGTH 20
#define EMAIL_LENGTH 50

typedef struct
{
    char name[NAME_LENGTH];
    char phone[PHONE_LENGTH];
    char email[EMAIL_LENGTH];
} Contact;

typedef struct
{
    int contactCount;
    Contact contacts[100];
} AddressBook;

void createContact(AddressBook *addressBook);
void searchContact(AddressBook *addressBook);
void editContact(AddressBook *addressBook);
void deleteContact(AddressBook *addressBook);
void listContacts(AddressBook *addressBook);
void initialize(AddressBook *addressBook);
void saveContactsToFile(AddressBook *addressBook);

int is_valid_name(char *name);
int is_valid_phone(char *phone);
int is_valid_email(char *email);
int isDuplicatePhone(AddressBook *addressBook, const char *phone);
int isDuplicateEmail(AddressBook *addressBook, const char *mail);
int searchData(AddressBook *addressBook, int choice);

#endif
