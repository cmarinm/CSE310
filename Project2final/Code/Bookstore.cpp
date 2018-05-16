#include "Bookstore.h"
#include "prime.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;
// Global Variables used
HashTableEntry **hashtable;
int tablesize;
int gensize;
Genre *genrearr;
prime *primechecker = new prime();// from Prime.cpp, helps to check for prime numbers in order to initialize hashtable

/*
 * Helps insert a BST node into its corresponding genre tree, as well as creates a hashtable node and inserts it at the
 * correct position in the hashtable
 */
void Bookstore::insert(char *genre, BST *newnode) {
    int i = 0;
    bool genrefound = false;
    while(i < gensize){ //Looks for genre in array
        if(strcmp(genrearr[i].genre, genre) == 0){
            genrefound = true;
            break;

        }
        else i++;
    }
    if(!genrefound){
        printf("Genre not Carried, unable to complete request.\n");
        return;
    }
    bool insert = false;
    BST *temp = genrearr[i].root;
    if(genrearr[i].root == NULL) {
        genrearr[i].root = newnode;
        insert = true;
    }
    while(!insert){ //inserts bst node into genre tree
            if(strcmp(temp->info.title, newnode->info.title) < 0){
                if(temp->right == NULL) {
                    temp->right = newnode;
                    newnode->parent = temp;
                    insert = true;
                }
                else
                    temp = temp->right;
            }
            else{
                if(temp->left == NULL){
                    temp->left = newnode;
                    newnode->parent = temp;
                    insert = true;
                }
                else
                    temp = temp->left;
            }


    }
    //create hastable node and insert it into right position
    HashTableEntry *hashnode = new HashTableEntry();
    hashnode->book = newnode;
    strcpy(hashnode->title, newnode->info.title);
    int pos;
    int sum = key(hashnode->title);
    pos = h(sum);//insert into hashtable
    hashnode->next = hashtable[pos];
    hashtable[pos] = hashnode;

}
//calculates key for title (sum of all ascii values)
int Bookstore::key(char *title) {
    int sum = 0;
    for (int i = 0; i<TITLE_LEN; i++){//calculates key for title (sum of all ascii values)
        sum = sum + title[i];
    }
    return sum;
}
int Bookstore::h(int k) {// hash function k mod m
    return k % tablesize;
}
void Bookstore::BuyBook(char *title, char *genre, char *author, char *publisher, char *format, float price, int quantity) {
    int sum = key(title);
    bool found = false;
    int pos = h(sum);
    HashTableEntry *temp;
    temp = hashtable[pos];
    while((!found) && (temp != NULL)){
        if(strcmp(title, temp->title) == 0)
            found = true;
        else
            temp = temp->next;
    }
    if(!found) {
        printf("Book Not currently carried, adding to bookstore...\n");
        BST *newnode = new BST();
        strcpy(newnode->info.title,title);
        strcpy(newnode->info.author,author);
        strcpy(newnode->info.publisher,publisher);
        strcpy(newnode->info.formats[0].format,format);
        newnode->info.formats[0].price = price;
        newnode->info.formats[0].quantity = quantity;
        newnode->left = NULL;
        newnode->right = NULL;
        insert(genre, newnode);

    }
    else{
        //check for publisher & author match
        if((strcmp(publisher,temp->book->info.publisher) == 0)&&(strcmp(author,temp->book->info.author) == 0)){
            //look for format, if format is not found adds format to list
            bool formatfound = false;
            for(int i = 0; i<NO_FORMATS;i++){
                if(strcmp(format, temp->book->info.formats[i].format) == 0){
                    formatfound = true;
                    temp->book->info.formats[i].quantity = temp->book->info.formats[i].quantity +1;
                    //after update, print updated info on book:
                    printf("Title:%s\nAuthor:%s\nPublisher:%s\nFormats:\n",temp->book->info.title, temp->book->info.author, temp->book->info.publisher);
                    for(int j = 0; j< NO_FORMATS; j++){
                        if(strcmp(temp->book->info.formats[j].format,"Empty") != 0){
                            printf("Format: %s\nPrice:%f\nQuantity:%d\n", temp->book->info.formats[j].format, temp->book->info.formats[j].price, temp->book->info.formats[j].quantity);
                        }
                    }
                    break;
                }
            }
            if(!formatfound){
                for(int j = 0; j<NO_FORMATS; j++){
                    if(strcmp(temp->book->info.formats[j].format, "") == 0){
                        strcpy(temp->book->info.formats[j].format, format);
                        temp->book->info.formats[j].price = price;
                        temp->book->info.formats[j].quantity = quantity;
                        //after update, print updated book info:
                        printf("Title:%s\nAuthor:%s\nPublisher:%s\nFormats:\n",temp->book->info.title, temp->book->info.author, temp->book->info.publisher);
                        for(int a = 0; a< NO_FORMATS; a++){
                            if(strcmp(temp->book->info.formats[a].format,"Empty") != 0){
                                printf("Format: %s\nPrice:%f\nQuantity:%d\n", temp->book->info.formats[a].format, temp->book->info.formats[a].price, temp->book->info.formats[a].quantity);
                            }
                        }
                        break;
                    }
                    else if(j == 3)
                        printf("Format list is full, Invalid format. Unable to complete request.\n");
                }
            }
        }
        else
            printf("Author or publisher not matched with book in store, unable to complete request.\n");
    }

}
/*
 * This function removes given node from its genre BST. Check if node has children, if so, do something different
 * for each different case(only a left, only a right, or both). Procedure gotten from website (Reference in State document).
 */
void Bookstore::remove_BST(BST *node) {
    int rl = (strcmp(node->info.title, node->parent->info.title));//used to know if node is left or right child
    if((node->left == NULL) && (node->right == NULL)){ // node has no children
        if(rl > 0)
            node->parent->right = NULL;
        if(rl < 0)
            node->parent->left = NULL;
}
    else{
        if(node->left == NULL){ // node has a right child
            if(rl >0)
                node->parent->right = node->right;
            if(rl < 0)
                node->parent->left = node->right;
            delete node;
            return;
        }
        if(node->right == NULL){// node has a left child
            if(rl > 0)
                node->parent->right = node->left;
            if(rl < 0)
                node->parent->left = node->left;
            delete node;
            return;
        }
        else{ //node has two children
// find minimum node in right subtree, copy min into current node, fix Hashtable pointers, then delete min (now a duplicate)
            BST *min;
            min = node->right;
            while(min->left != NULL)
                min = min->left;

            strcpy(node->info.title, min->info.title);
            strcpy(node->info.author, min->info.author);
            strcpy(node->info.publisher, min->info.publisher);
            for(int i = 0; i< NO_FORMATS; i++){
                strcpy(node->info.formats[i].format, min->info.formats[i].format);
                node->info.formats[i].price = min->info.formats[i].price;
                node->info.formats[i].quantity = min->info.formats[i].quantity;
            }

            int sum = key(min->info.title);
            int pos = h(sum);
            HashTableEntry *temp = hashtable[pos];
            while(temp != NULL){
                if(strcmp(temp->title, min->info.title) == 0)
                    temp->book = node;
                else temp = temp->next;
            }

            remove_BST(min);
        }
    }
}
/*
 * This function removes the given node from its chained list in the hashtable
 * First, check if it node is the first node in the list, else look at the next's until
 * it is found, then redirect the "parent's" pointer to eliminate given node from the list.
 */
void Bookstore::remove_Hash(HashTableEntry *node) {
    int sum = key(node->title);
    int pos = h(sum);
    bool deleted = false;
    if(strcmp(node->title,hashtable[pos]->title) == 0){
        hashtable[pos] = hashtable[pos]->next;
        deleted = true;
    }
    else{
        HashTableEntry *temp = hashtable[pos];
        while(!deleted){
            if(strcmp(node->title,temp->next->title) == 0){
                temp->next = temp->next->next;
                deleted = true;
            } else temp = temp->next;
        }
    }
    delete node;

}
void Bookstore::check_stock(HashTableEntry *temp) {
    bool formats_outstock = true;
    for(int i = 0; i < NO_FORMATS; i++){
        if(temp->book->info.formats[i].quantity > 0)
            formats_outstock = false;
    }
    if(formats_outstock){
        remove_Hash(temp);
        remove_BST(temp->book);
    }

}
void Bookstore::SellBook(char *genre, char *title, char *format) {
    bool found = false;
    int sum = key(title);
    int pos = h(sum);
    HashTableEntry *temp;
    temp = hashtable[pos];
    while((!found) && (temp!= NULL)){
        if(strcmp(title, temp->title) == 0)
            found = true;
        else
            temp = temp->next;
    }
    if(!found)
        printf("Book not carried.\n");
    else{
        bool formatfound = false;
        for(int i = 0; i < NO_FORMATS;i++){
            if(strcmp(format, temp->book->info.formats[i].format) == 0){
                if(temp->book->info.formats[i].quantity != 0){
                    formatfound = true;
                    temp->book->info.formats[i].quantity = temp->book->info.formats[i].quantity -1;
                    printf("Book sold, checkout complete.\n");
                    check_stock(temp);
                }
                else
                    if(temp->book->info.formats[i].quantity <= 0){
                        printf("Format out of stock.\n");
                        check_stock(temp);
                    }
                break;

            }
        }
        if(!formatfound)
            printf("Format not found in list (Invalid format). Unable to complete request.\n");
    }

}
/*
 * This function looks for book in hashtable, if found gets the pointer to bst node, and prints the information
 */
void Bookstore::FindBook(char *title) {
    int sum = key(title);
    bool found = false;
    int pos = h(sum);
    HashTableEntry *temp;
    temp = hashtable[pos];
    // looks for title at linked list in table position, if found, prints information on bst node, else "Book not carried"
    while((temp != NULL) && (!found)){
        if(strcmp(title, temp->title) == 0){
            found = true;
            printf("Title:%s\nAuthor:%s\nPublisher:%s\nFormats:\n",temp->book->info.title, temp->book->info.author, temp->book->info.publisher);
            for(int j = 0; j< NO_FORMATS; j++){
                if(strcmp(temp->book->info.formats[j].format,"Empty") != 0){
                    printf("Format: %s\nPrice:%f\nQuantity:%d\n", temp->book->info.formats[j].format, temp->book->info.formats[j].price, temp->book->info.formats[j].quantity);
                }
            }

        }
        else temp = temp->next;
    }
    if(!found)
        printf("Book Not Carried.\n");
}
/*
 * This function prints all nodes in a Binary tree by reverse order traversal, print left, print self, print right
 */
void Bookstore::BSTprinter(BST *root) {
    if(root != NULL) {
        if (root->left != NULL)
            BSTprinter(root->left);
        printf("%s\n", root->info.title);
        if (root->right != NULL)
            BSTprinter(root->right);
    }
}
/*
 * This function does the same as function above, but given low or high, only prints if low < title < high.
 */
void Bookstore::BSTrangeprnt(BST *root, char *low, char *high) {
    if(root != NULL) {
        if (root->left != NULL)
            BSTrangeprnt(root->left, low, high);
        if ((strcmp(low, root->info.title) <= 0) && (strcmp(high, root->info.title) >= 0))
            printf("%s\n", root->info.title);
        if (root->right != NULL)
            BSTrangeprnt(root->right, low, high);
    }
}
/*
 * This function helps find genre to print.
 */
void Bookstore::FindGenre(char *type) {

    bool found = false;
    int bst =0;
    //Look through genre array looking for matching genre, if found, calls BSTprinter to print the Tree.
    // Else Prints Genre not Carried
    for(int i =0; i<gensize;i++){
        if(strcmp(type,genrearr[i].genre) == 0){
            bst = i;
            found = true;
            break;
        }
    }
    if(found){
        if(genrearr[bst].root != NULL)
            BSTprinter(genrearr[bst].root);
        else
            printf("No Books in this Genre.\n");
    }
    else
        printf("Genre Not Carried.\n");

}
/*
 * This function does the same as above function, but with ranges low and high so that it calls BSTrangeprnt with low and high parameters
 */
void Bookstore::Range(char *genre, char *low, char *high) {
    bool found = false;
    int bst = 0;
    for(int i =0; i<gensize;i++){
        if(strcmp(genre,genrearr[i].genre) == 0){
            bst = i;
            found = true;
        }

    }

    if(found){
        if(genrearr[bst].root != NULL)
            BSTrangeprnt(genrearr[bst].root,low,high);
        else
            printf("No Books in this Genre.\n");
    }
    else
        printf("Genre Not Carried.\n");
}
/*
 * This function does the same as FindBook, Ideally, the only thing that changed would be not not
 * print the book info other than the formats and their prices/amount But somehow, without the title,author,publisher
 * print statement, the function does not work(?)
 */
void Bookstore::Price(char *title) {
    int sum = key(title);
    int pos = h(sum);
    bool found = false;

    HashTableEntry *temp;
    temp = hashtable[pos];
    while((temp != NULL) && (!found)){
        if(strcmp(title, temp->title) == 0){
            found = true;
            for(int j = 0; j< NO_FORMATS; j++){
                if(strcmp(temp->book->info.formats[j].format,"Empty") != 0){
                    printf("Format: %s\nPrice:%f\nQuantity:%d\n", temp->book->info.formats[j].format, temp->book->info.formats[j].price, temp->book->info.formats[j].quantity);
                }
            }

        }
        else temp = temp->next;
    }
    if(!found)
        printf("Book Not Carried.\n");
}
/*
 * Intiialized hashtable to null
 */
void Bookstore::initHT() {
    for(int i = 0; i<tablesize;i++){
        hashtable[i] = NULL;
    }
}
int Bookstore::mainb() {
    run();
    delete primechecker;
    return 0;
}
/*
 * This function reads in the data from stdin, creates and inserts nodes with book informtion
 * into their respective tree and hashtable.
 */
void Bookstore::read() {

    //scan the genres and create genre array
    scanf("%d",&gensize);
    genrearr = new Genre[gensize];
    cin.ignore();
    for(int i = 0; i<gensize; i++){
        char genre[GENRE_LEN];
        cin.getline(genre,GENRE_LEN,'\n');
        strcpy(genrearr[i].genre, genre);
        genrearr[i].root = NULL;
    }

    //create hashtable
    int m = 0;
    scanf("%d",&m);

    for(int j = 2*m;j< 2*m+100;j++){
        if(primechecker->TestForPrime(j)) {
            tablesize = j;
            break;
        }
    }
    hashtable = new HashTableEntry *[tablesize];
    initHT();
    // read in book data and fill BST's and hashtable

    for(int i = 0; i<m;i++){
        cin.ignore();
        char genre[GENRE_LEN];
        char title[TITLE_LEN];
        char author[AUTHOR_LEN];
        char publisher[PUBLISHER_LEN];
        int f=0;
        BST *newnode = new BST();
        cin.getline(genre, GENRE_LEN, '\n');
        cin.getline(title, TITLE_LEN, '\n');
        cin.getline(author, AUTHOR_LEN, '\n');
        cin.getline(publisher, PUBLISHER_LEN, '\n');
        strcpy(newnode->info.title, title);
        strcpy(newnode->info.author, author);
        strcpy(newnode->info.publisher, publisher);
        scanf("%d",&f);
        for(int j=0;j<f;j++){
            char format[FORMAT_LEN];
            float price;
            int quantity;
            scanf("%s", format);
            scanf("%f", &price);
            scanf("%d",&quantity);

            strcpy(newnode->info.formats[j].format, format);
            newnode->info.formats[j].price = price;
            newnode->info.formats[j].quantity = quantity;
        }
        newnode->left = NULL;
        newnode->right = NULL;
        insert(genre,newnode);

    }


}
/*
 * This function calls read to start the program, then read in the queries and calls
 * necessary function to perform them
 */
void Bookstore::run() {
    read();
    // read in the queries and run the program
    int q;
    cin.ignore();
    scanf("%d", &q);
    for(int i = 1; i <= q;i++){
        char firstw[6]="";
        scanf("%s", firstw);

        if(strncmp(firstw, "sell", 2) == 0){
            char ignore[10];
            char genre[GENRE_LEN] = "";
            char title[TITLE_LEN]="";
            char format[FORMAT_LEN]="";

            scanf(" %s",ignore);
            scanf(" \"%[^\"]\"",genre);
            scanf(" \"%[^\"]\"",title);
            scanf(" %s",format);
            printf("Selling %s in %s format.\n",title,format);
            SellBook(genre,title,format);
            printf("\n");

        }
        if(strncmp(firstw, "buy", 2) == 0){
            char ignore2[10];
            char genre[GENRE_LEN]="";
            char title[TITLE_LEN]="";
            char author[AUTHOR_LEN]="";
            char publisher[PUBLISHER_LEN]="";
            char format[FORMAT_LEN]="";
            float price = 0;
            int quantity = 0;

            scanf(" %s",ignore2);
            scanf(" \"%[^\"]\"",genre);
            scanf(" \"%[^\"]\"",title);
            scanf(" \"%[^\"]\"",author);
            scanf(" \"%[^\"]\"",publisher);
            scanf(" %s", format);
            scanf("%f",&price);
            scanf("%d", &quantity);

            printf("Buying: %s book in: %s format.\n",title,format);
            BuyBook(title,genre,author,publisher,format,price,quantity);
            printf("\n");
        }
        if(strncmp(firstw,"price",2) == 0){
            char title[TITLE_LEN]="";
            scanf(" \"%[^\"]\"\n",title);
            printf("Looking price for %s.\n",title);
            Price(title);
            printf("\n");
        }
        if(strncmp(firstw,"range",2) == 0){
            char genre[GENRE_LEN]="";
            char low[50]="";
            char high[50]="";
            scanf(" \"%[^\"]\"",genre);
            scanf(" \"%[^\"]\"",low);
            scanf(" \"%[^\"]\"\n",high);
            printf("Printing %s genre, in between %s and %s\n", genre,low,high);
            Range(genre,low,high);
            printf("\n");

        }
        if(strncmp(firstw,"find",2) == 0){
            char secndw[6]="";
            scanf(" %s",secndw);
            if(strncmp(secndw,"book",2) == 0){
                char title2[TITLE_LEN]="";
                scanf(" \"%[^\"]\"\n",title2);
                printf("Finding book %s.\n",title2);
                FindBook(title2);
                printf("\n");
            }
            if(strncmp(secndw,"genre",2) == 0){
                char type[GENRE_LEN]="";
                scanf(" \"%[^\"]\"\n",type);
                printf("Finding %s genre.\n",type);
                FindGenre(type);
                printf("\n");

            }
        }
    }

}
/*
 * Main Function, creates a bookstore, calls its main function, then deletes new'd global elements
 * to avoid memory leaks
 */
int main(){
    Bookstore *bookstr = new Bookstore();
    bookstr->mainb();
    printf("All queries completed, thank you for using this service! PROGRAM_END\n");
    delete []hashtable;
    delete []genrearr;
    delete bookstr;
    return 0;
}

