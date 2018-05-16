//
// Created by Cesar on 10/10/2016.
//

#ifndef BOOKSTORE_H
#define BOOKSTORE_H
#include "prime.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#define GENRE_LEN       25
#define	TITLE_LEN		100
#define	AUTHOR_LEN		50
#define	PUBLISHER_LEN	50
#define	NO_FORMATS		4
#define	FORMAT_LEN		10

struct Format{
    char format[ FORMAT_LEN ] ="Empty"; // Format, i.e., Paperback, Hardcover, Kindle, Audible
    float price; // Price in $ of the book in the given format
    int quantity; // Number of books in the inventory of the given format
};

struct BookInfo{
    char title[ TITLE_LEN ]; // Title of the book
    char author[ AUTHOR_LEN ]; // Name of the author
    char publisher[ PUBLISHER_LEN ]; // Publisher of the book
    struct Format formats[ NO_FORMATS ]; // Book formats carried
};

struct BST{ // A binary search tree
    struct BookInfo info; // Information about the book
    struct BST *left;  // Pointer to the left subtree
    struct BST *right;  // Pointer to the right subtree
    struct BST *parent; //Pointer to the parent node
};

struct Genre{
    char genre[ GENRE_LEN ]; // Type of genre
    struct BST *root;  // Pointer to root of search tree for this genre
};

struct HashTableEntry{
    char title[ TITLE_LEN ]; // Title of the book
    struct BST *book; // Pointer to node in tree containing the book's information
    struct HashTableEntry *next; // Pointer to next node in chain
};
 class Bookstore{
 public:
     int mainb();
     void run();
     int key(char *title);
     void check_stock(HashTableEntry *temp);
     void remove_BST(BST *node);
     void remove_Hash(HashTableEntry *node);
     void BSTprinter(BST *root);
     void BSTrangeprnt(BST *root, char *low, char *high);
     void insert(char *genre, BST *newnode);
     void initHT();
     void read();
     int h(int k);
     void FindBook(char *title);
     void FindGenre(char *type);
     void Range(char *genre, char *low, char *high);
     void Price(char *title);
     void SellBook(char *genre, char *title, char *format);
     void BuyBook(char *title, char *genre, char *author, char*publisher, char *format, float price, int quantity);

 };
#endif //BOOKSTORE_H
