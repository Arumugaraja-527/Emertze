#ifndef INVERTED_H
#define INVERTED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 28
#define SUCCESS 1
#define FAILURE 0

typedef struct MainNode main_t;
typedef struct SubNode sub_t;

// Structure for Sub Node (file occurrence of each word)
struct SubNode 
{
    char file_name[100];    // File in which word occurs
    int word_count;         // Count of word occurrences in file
    sub_t *s_link;          // Next subnode (next file for this word)
};

// Structure for Main Node (unique word entry)
struct MainNode 
{
    char word[100];         // Word itself
    main_t *m_link;         // Next main node (next word)
    int file_count;         // Number of files containing this word
    sub_t *ms_link;         // First subnode (file list for this word)
};

// Structure for Hash Table (buckets for fast lookup)
typedef struct Hashtable 
{
    int index;
    main_t *h_link;         // Points to first main node in bucket
} hash_t;

// Structure for singly linked list of files (input files)
typedef struct Node  // store valid file name                                                                        '                                                                
{
    char file_name[100];    // Input file name
    struct Node *link;      // Next input file
} Node;

int read_and_validate(int argc, char *argv[], Node **head);
int valid_filename(char *filename);
int is_file_empty(FILE *fptr);
int is_duplicate(Node *head, char *filename);
void insert_file(Node **head, char *filename);

int is_hash_empty(hash_t *hash);
void insert_to_database(hash_t *hash, int index, char *word, char *file_name, int word_count);
int remove_file(Node **head,char *filename);

int create_database(hash_t *hash, Node **head);
int search_database(hash_t *hash, char *s);
int display_database(hash_t *hash);
int Save_database(hash_t *hash,char *f);
int update_database(hash_t *hash, Node **head, char *filename);

#endif