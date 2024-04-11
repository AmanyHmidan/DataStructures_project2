// 1200255 Amany Hmidan
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 50

typedef struct tree_node *tpointer;
struct tree_node{
    char name[N];
    char activeIngredient[N];
    char category[N];
    char indication[100];
    char expiryDate[N];
    int count;
    char company[N];

    tpointer Left;
    tpointer Right;
};

typedef tpointer TNode;
TNode myTree;
/////////////////////////////////
// global integers
int SIZE=13;     // this is for the size of the hash table
int COUNTER=0;   // this is to know when to rehash

typedef struct hash_node *hashTable;
struct hash_node{

    int status;     // status=0 means this index is empty, status=1 means the index is occupied
    char name[N];
    char activeIngredient[N];
    char category[N];
    char indication[100];
    char expiryDate[N];
    int count;
    char company[N];
};

hashTable table[13];  // this is the array (table) that will contain the hashed nodes
int collNUM=0;  // to indicate number of collisions

// TREE functions prototypes
TNode MakeEmpty( TNode T );
void displayMenu();
TNode readMedsFile(TNode t);
TNode createNewNode(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N]);
TNode Insert(  TNode T,char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N]);
void listALL_alphabetically(TNode T);
TNode insert_fromUser(TNode t);
TNode Find( char Name[N],TNode T );
void Find_UpdateInfo(char Name[N] , TNode T);
void updateQuantity(char Name[N] , TNode T);
void list_sameCategory(TNode t, char Category[N]);
TNode FindMin( TNode T );
TNode Delete( char Name[N], TNode T );
//char **split_indication(char Indication[N]);
void list_specificDisease(TNode t, char Disease[N]);
void Delete_fromCompany(TNode t,char Company[N]);
void print_in_file(TNode T,FILE *save_data);

/////////////////////////////////////
// HASH functions prototypes
int isPrime(int n);
int getNewSize(int oldSize);
int hash(char key[], int TableSize);
int getIndex(char key[], int size);
void insertNew_intoHashTable();
void readHashFile();
void Insert_FiletoTable(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N]);
void hashMenu();
void printHash();
int searchHash(char key[], int TableSize);
void printHashFile();
void deleteHash();
void insert_newTable(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N],hashTable arr[]);
void reHash();
//////////////////////////////////////

int found =0; // this variable is to check whether the category is found in any node or not
int found2=0; //this variable is to check whether the disease is found in any medicine indications
int found3=0;  //this variable is to check whether the company you wanted to delete medicines from is found or not

FILE *save_data;

int main() {

    // initializing the binary search tree which is called myTree by function makeEmpty
    myTree = MakeEmpty(myTree);

    displayMenu();
    return 0;
}

 TNode MakeEmpty( TNode T ){
    if( T != NULL ){
        MakeEmpty( T->Left );
        MakeEmpty( T->Right );
        free( T );
    }
    return NULL;
}

void displayMenu(){
    while(1){
        int ch;
        printf("\nSelect a choice:\n");
        printf("1- Read Meds.txt file and create the tree\n");
        printf("2- List the medicines in the tree in alphabetical order with their associated information\n");
        printf("3- Insert a new medicine\n");
        printf("4- Find a medicine and update it's info\n");
        printf("5- Update the quantity of a certain medicine\n");
        printf("6- List all medicines of same category\n");
        printf("7- List all medicines that treat a specific disease\n");
        printf("8- Delete a medicine\n");
        printf("9- Delete all medicines from a specific company\n");
        printf("10- Save all information to a file called Pharmacy.txt\n");
        printf("11- Display Hashing Menu\n");
        printf("12- Exit\n");

        scanf("%d",&ch);

        switch (ch) {
            case 1:
                myTree= readMedsFile(myTree);
                break;
            case 2:
                printf("Data Format:\n");
                printf("name:active ingredient:category:indication:expiry date:count:company\n");
                printf("_____________________________________________________________________\n");
                listALL_alphabetically(myTree);
                break;
            case 3:
                myTree=insert_fromUser(myTree);
                break;
            case 4:
                printf("Enter the name of medicine you want to update\n");
                char n[N];
                char line [100];
                gets(line);
                gets(n);
                Find_UpdateInfo(n,myTree);
                break;
            case 5:
                printf("Enter the name of medicine you want to update it's quantity\n");
                char f[N];
                char line5 [100];
                gets(line5);
                gets(f);
                updateQuantity(f,myTree);
                break;
            case 6:
                printf("Enter Category\n ");
                char a[N];
                char line1[100];
                gets(line1);
                gets(a);
                found=0;
                list_sameCategory(myTree,a);
                if(!found)
                    printf("Category %s is not available\n",a);
                break;
            case 7:
                printf("Enter disease name:\n");
                char c[N];
                char line3[100];
                gets(line3);
                gets(c);
                found2=0;
                list_specificDisease(myTree,c);
                if(!found2)
                    printf("Medicine that treats %s is not available\n",c);
                break;
            case 8:
                printf("Enter the name of medicine you want to delete\n");
                char b[N];
                char line2[100];
                gets(line2);
                gets(b);
                myTree=Delete(b,myTree);
                break;


            case 9:
                printf("Enter company name:\n");
                char d[N];
                char line4[100];
                gets(line4);
                gets(d);
                found3=0;
                Delete_fromCompany(myTree,d);
                if(!found3)
                    printf("Company %s is not found\n",d);
                break;

            case 10:
                save_data = fopen("Pharmacy.txt", "w");
                print_in_file(myTree,save_data);
                 fclose(save_data);
                break;

            case 11:
                hashMenu();
                break;

            case 12: exit(1);
        }
    }
}

TNode readMedsFile(TNode t){
    FILE *fp;
    fp = fopen("Meds.txt","r");

    if(fp == NULL){
        printf("File not found!");
        exit(1);
    }
//name:active ingredient:category:indication:expiry date:count:company
    char *token;
    char Name[N];
    char ActiveIngredient[N];
    char Category[N];
    char Indication[100];
    char ExpiryDate[N];
    int Count;
    char Company[N];

    char line[150]; // this array will hold a line from the file

    while(fgets(line,150,fp) != NULL){
        token = strtok(line,":");
        strcpy(Name,token);

        token = strtok(NULL,":");
        strcpy(ActiveIngredient,token);

        token = strtok(NULL,":");
        strcpy(Category,token);

        token = strtok(NULL,":");
        strcpy(Indication,token);

        token = strtok(NULL,":");
        strcpy(ExpiryDate,token);

        token = strtok(NULL,":");
        Count = atoi(token);

        token = strtok(NULL,":\n");
         //strncpy(Company,token, strlen(token)-1);
        strcpy(Company,token);

        //printf("%s,%s,%s,%s,%s,%d,%s\n",Name,ActiveIngredient,Category,Indication,ExpiryDate,Count,Company);

         //Insert(t,Name,ActiveIngredient,Category,Indication,ExpiryDate,Count,Company);

        if(t==NULL)
            t= Insert(t,Name,ActiveIngredient,Category,Indication,ExpiryDate,Count,Company);
        else
            Insert(t,Name,ActiveIngredient,Category,Indication,ExpiryDate,Count,Company);


    }
    printf("DONE INSERTING\n");
    fclose(fp);
    return t;
}

TNode createNewNode(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N]){
    //allocate memory for the node
    TNode node = (TNode)malloc(sizeof(struct tree_node));
    // set left and right pointers to NULL
    node ->Left = NULL;
    node ->Right = NULL;
    // fill the node with it's data
    strcpy(node->name,Name);
    strcpy(node->activeIngredient,ActiveIngredient);
    strcpy(node->category,Category);
    strcpy(node->indication,Indication);
    strcpy(node->expiryDate,Expire);
    node->count = Count;
    strcpy(node->company,Company);
    printf("node %s created\n",node->name);
    return node;
}
//name:active ingredient:category:indication:expiry date:count:company
TNode Insert(  TNode T,char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N]){
    if( T == NULL){
//create and return a 1-node tree
        //return(createNewNode(Name, ActiveIngredient, Category, Indication, Expire , Count, Company));
        TNode node = (TNode) malloc(sizeof (struct tree_node));
        T = node;
        node ->Left = NULL;
        node ->Right = NULL;
        strcpy(node->name,Name);
        strcpy(node->activeIngredient,ActiveIngredient);
        strcpy(node->category,Category);
        strcpy(node->indication,Indication);
        strcpy(node->expiryDate,Expire);
        node->count = Count;
        strcpy(node->company,Company);
    }
    else if(strcasecmp(Name,T->name) < 0  )
        T->Left = Insert( T->Left,Name, ActiveIngredient, Category, Indication, Expire , Count, Company);
    else if( strcasecmp(Name,T->name) > 0 )
        T->Right = Insert(T->Right,Name, ActiveIngredient, Category, Indication, Expire , Count, Company);
//else, name is in the tree already; do nothing

    return T;
}
//List the medicines in the tree in alphabetical order with their associated info
// alphabetical order means inorder
void listALL_alphabetically(TNode T){
    if(T==NULL){
       // printf("empty tree!\n");
        return;
    }
    else{
        listALL_alphabetically(T->Left);
        printf("%s:%s:%s:%s:%s:%d:%s\n",T->name,T->activeIngredient,T->category,T->indication,T->expiryDate,T->count,T->company);
        listALL_alphabetically(T->Right);
    }
}
TNode insert_fromUser(TNode t){
    char Name[N];
    char ActiveIngredient[N];
    char Category[N];
    char Indication[100];
    char Expire[N];
    int Count;
    char Company[N];
    char line[N];

    printf("Enter medicine's name\n");
    gets(line);
    fgets(Name,N,stdin);
    Name[strcspn(Name, "\n")] = 0;

    printf("Enter the active ingredient\n");
    fgets(ActiveIngredient,N,stdin);
    ActiveIngredient[strcspn(ActiveIngredient, "\n")] = 0;

    printf("Enter category\n");
    fgets(Category,N,stdin);
    Category[strcspn(Category, "\n")] = 0;

    printf("Enter indications (with , between them )\n");
    fgets(Indication,N,stdin);
    Indication[strcspn(Indication, "\n")] = 0;

    printf("Enter expiration date\n");
    fgets(Expire,N,stdin);
    Expire[strcspn(Expire, "\n")] = 0;

    printf("Enter count\n");
    scanf("%d",&Count);
    getchar();

    printf("Enter company\n");
    fgets(Company,N,stdin);
    Company[strcspn(Company, "\n")] = 0;

   t= Insert(t,Name,ActiveIngredient,Category,Indication,Expire,Count,Company);
   return t;

}

// Find function will return the node that has the wanted name, and NULL if name is not found
TNode Find(char Name[N] , TNode T){
    if( T == NULL)
        return NULL;
    else if( strcmp(Name,T->name) < 0  )
        return Find( Name, T->Left );
    else if( strcmp(Name,T->name) > 0  )
        return Find( Name, T->Right );
    else
        return T;
}
void Find_UpdateInfo(char Name[N] , TNode T){
    int ch;
    int co;
    char n[N],a[N],c[N],i[N],e[N],ca[N];
    char line[N];

    TNode P = Find(Name,T);
    if(P == NULL){
        printf("medicine not found\n");
    }
    else{
        printf("Choose what do you want to update\n");
        printf("1- Name\n2- active ingredient\n3- category\n4- indication\n5- expiry date\n6- count\n 7-company\n");
        scanf("%d",&ch);

        switch(ch){
            case 1:
                printf("Enter new name\n");
                gets(line);
                gets(n);
                strcpy(P->name,n);
                break;

            case 2:
                printf("Enter new active ingredient\n");
                gets(line);
                gets(a);
                strcpy(P->activeIngredient,a);
                break;
            case 3:
                printf("Enter new category\n");
                gets(line);
                gets(c);
                strcpy(P->category,c);
                break;
            case 4:
                printf("Enter new indication\n");
                gets(line);
                gets(i);
                strcpy(P->indication,i);
                break;
            case 5:
                printf("Enter new expiry date\n");
                gets(line);
                gets(e);
                strcpy(P->expiryDate,e);
                break;
            case 6:
                printf("Enter new count\n");
                scanf("%d",&co);
                getchar();
                P->count = co;
                break;

            case 7:
                printf("Enter new company\n");
                gets(line);
                gets(ca);
                strcpy(P->company,ca);
                break;
        }


    }
}
void updateQuantity(char Name[N] , TNode T){
    int c;
    TNode P = Find(Name,T);
    if(P == NULL){
        printf("medicine not found\n");
    }
    else{
        printf("Enter the updated quantity\n");
        scanf("%d",&c);
        P ->count = c;

    }
}
/*void list_sameCategory(TNode t, char Category[N]){
    if(strcmp(Category,t->category) < 0){
        if(t->Left)
            list_sameCategory(t ->Left, Category);
    }
    if(strcmp(Category,t->category) == 0)
        printf("%s,%s,%s,%s,%s,%d,%s\n",t->name,t->activeIngredient,t->category,t->indication,t->expiryDate,t->count,t->company);
     if(strcmp(Category,t->category) > 0){
         if(t->Right)
             list_sameCategory(t ->Right, Category);
     }

}*/
void list_sameCategory(TNode t, char Category[N]){

    if(t==NULL)
        return;
    if(strcasecmp(t->category,Category) == 0){
        printf("%s\n",t->name);
        found =1;
    }
    // send the left subtree to continue searching, and when the left is null it goes to search in the right subtree
    list_sameCategory(t->Left,Category);
    list_sameCategory(t->Right,Category);

}
void list_specificDisease(TNode t, char Disease[N]){
    if(t==NULL)
        return;
    char Indication[N];
    strcpy(Indication,t->indication);
   // char [10][20]diseases = split_indication(Indication);

    char data[10][20];
    int size=0;
    char *token;

    token = strtok(Indication,",");
    while(token != NULL){
        strcpy(data[size],token);
        size++;
        token = strtok(NULL,",");
    }
    for(int i=0; i<size; i++){
        if(strcasecmp(data[i],Disease)==0){
            printf("%s\n", t->name);
            found2=1;
        }
    }
    list_specificDisease(t->Left,Disease);
    list_specificDisease(t->Right,Disease);

}
/*char **split_indication(char Indication[N]){
    char data[10][20];
    int size=0;
    char *token;

    token = strtok(Indication,",");
    while(token != NULL){
        strcpy(data[size],token);
        size++;
        token = strtok(NULL,",");
    }
    return data;

}*/

// Function findMin will be used in function Delete because when deleting a node with two children
// you replace the key of this node with the smallest key of the right subtree then recursively delete that node (which is now empty)
TNode FindMin( TNode T ){
    if( T != NULL)
        while( T->Left != NULL)
            T = T->Left;
    return T;
}

TNode Delete( char Name[N], TNode T )
{
    TNode TmpCell;
    if( T == NULL )
        printf( "Element not found" );
    else if(strcmp(Name,T->name )  < 0 ) /* Go left */
        T->Left = Delete( Name, T->Left );
    else if(strcmp(Name, T->name) > 0) /* Go right */
        T->Right = Delete( Name, T->Right );
    else /* Found element to be deleted */
    if( T->Left && T->Right )
/* Two children */
    {
/* Replace with smallest in right
subtree */
        TmpCell = FindMin( T->Right );
        strcpy(T->name,TmpCell->name)   ;
        T->Right = Delete( T->name, T->Right );

    }
    else /* One or zero children */
    {
        TmpCell = T;
        if( T->Left == NULL )
/* Also handles 0 children */
            T = T->Right;
        else if( T->Right == NULL )
            T = T->Left;
        free( TmpCell );
    }
    return T;
} //end of Delete routine

void Delete_fromCompany(TNode t,char Company[N]){
    if(t==NULL)
        return;
    if(strcmp(t->company,Company) == 0){
        Delete(t->name,myTree);
        found3 =1;
    }
    // send the left subtree to continue searching, and when the left is null it goes to search in the right subtree
    Delete_fromCompany(t->Left,Company);
    Delete_fromCompany(t->Right,Company);
}

// this function prints the data into Pharmacy.txt file, then the hash function will read it
void print_in_file(TNode T,FILE *save_data){

    if (T != NULL){
        print_in_file(T->Left,save_data);
        fprintf(save_data,"%s:%s:%s:%s:%s:%d:%s\n",T->name,T->activeIngredient,T->category,T->indication,T->expiryDate,T->count,T->company);
        print_in_file(T->Right,save_data);
    }
}

// End of tree functions
////////////////////////////////////////////
// This part will contain functions related to hashing:

// this function returns 1 if the number is prime and 0 if it's not
int isPrime(int n) {
    int j, flag = 1;
    for (j = 2; j <= n / 2; ++j) {
        if (n % j == 0) {
            flag = 0;
            break;
        }
    }
    return flag;
}

//returns the new size of array which must be the first prime number > 2*old size
int getNewSize(int oldSize)
{
    int check= oldSize*2;
    int newSize;
    while(!isPrime(check))   // it exits the loop when check is a prime number
    {

        check++;
    }
    newSize=check;
    return newSize;
}

//return the index where the data must be inserted in the array by hashing
int hash(char key[], int TableSize) {
    int hashValue = 0;
    for (int i = 0; i < strlen(key); i++) {
        hashValue = (hashValue << 5) + key[i];
    }
    return (hashValue % TableSize);
}
// this function is to handle collosions
int getIndex(char key[], int tableSize) {
    int index = hash(key, tableSize);
    int i = 0;
    while (table[(index + i) % tableSize] != NULL && table[(index + i) % tableSize]->status != 0) {
        i++;
        collNUM ++;
    }
    return (index + i) % tableSize;
}

//insert a new record into the hash table
void insertNew_intoHashTable(){
    char Name[N];
    char ActiveIngredient[N];
    char Category[N];
    char Indication[100];
    char Expire[N];
    int Count;
    char Company[N];
    char line[N];

    printf("Enter medicine's name\n");
    gets(line);
    fgets(Name,N,stdin);
    Name[strcspn(Name, "\n")] = 0;

    printf("Enter the active ingredient\n");
    fgets(ActiveIngredient,N,stdin);
    ActiveIngredient[strcspn(ActiveIngredient, "\n")] = 0;

    printf("Enter category\n");
    fgets(Category,N,stdin);
    Category[strcspn(Category, "\n")] = 0;

    printf("Enter indications (with , between them )\n");
    fgets(Indication,N,stdin);
    Indication[strcspn(Indication, "\n")] = 0;

    printf("Enter expiration date\n");
    fgets(Expire,N,stdin);
    Expire[strcspn(Expire, "\n")] = 0;

    printf("Enter count\n");
    scanf("%d",&Count);
    getchar();

    printf("Enter company\n");
    fgets(Company,N,stdin);
    Company[strcspn(Company, "\n")] = 0;

    char key[4]; // create an array to hold the first 3 characters

    strncpy(key, Name, 3);
    key[3] = '\0';

   int index = getIndex(key,SIZE);

    strcpy(table[index]->name, Name);
    strcpy(table[index]->activeIngredient,ActiveIngredient);
    strcpy(table[index]->category, Category);
    strcpy(table[index]->indication, Indication);
    strcpy(table[index]->expiryDate, Expire);
    table[index]->count=Count;
    strcpy(table[index]->company, Company);
    printf("\nAdded successfully\n");

    table[index]->status=1;
    COUNTER++;

    // check if the data are more than the half of the array size to make a rehash
   if(COUNTER>=(SIZE*0.75) )
    {
        reHash();

    }
}

void readHashFile(){
    FILE *fp;
    fp = fopen("Pharmacy.txt","r");

    if(fp == NULL){
        printf("File not found!");
        exit(1);
    }
//name:active ingredient:category:indication:expiry date:count:company
    char *token;
    char Name[N];
    char ActiveIngredient[N];
    char Category[N];
    char Indication[100];
    char ExpiryDate[N];
    int Count;
    char Company[N];

    char line[150]; // this array will hold a line from the file

    while(fgets(line,150,fp) != NULL){
        token = strtok(line,":");
        strcpy(Name,token);

        token = strtok(NULL,":");
        strcpy(ActiveIngredient,token);

        token = strtok(NULL,":");
        strcpy(Category,token);

        token = strtok(NULL,":");
        strcpy(Indication,token);

        token = strtok(NULL,":");
        strcpy(ExpiryDate,token);

        token = strtok(NULL,":");
        Count = atoi(token);

        token = strtok(NULL,":\n");
        strcpy(Company,token);

        // insert each medicine into the hash table
        Insert_FiletoTable(Name,ActiveIngredient,Category,Indication,ExpiryDate,Count,Company);


    }
    printf("Insertion is done into the hash table\n");
    fclose(fp);
}

//this function inserts the data recieved from the file into the right index of table
void Insert_FiletoTable(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N])
{
    char key[4];
    strncpy(key, Name, 3);
    key[3] = '\0';

    int index = getIndex(key,SIZE);

    strcpy(table[index]->name, Name);
    strcpy(table[index]->activeIngredient,ActiveIngredient);
    strcpy(table[index]->category, Category);
    strcpy(table[index]->indication, Indication);
    strcpy(table[index]->expiryDate, Expire);
    table[index]->count=Count;
    strcpy(table[index]->company, Company);
    table[index]->status=1;
    COUNTER++;

    // check to make rehash or not
    if(COUNTER>=(SIZE*0.75) )
    {
        reHash();

    }

}

void hashMenu(){

    // first we must initialize the table by malloc
    for(int i=0; i<SIZE; i++){
        table[i]= (hashTable)malloc( sizeof( struct hash_node ));
        table[i]->status=0;
    }

     readHashFile();


    while(1){
        int ch;
        printf("\nSelect a choice\n");
        printf("1- Print hash table\n");
        printf("2- Print size of hash table\n");
        printf("3- Print the used hash function\n");
        printf("4-  Insert a new record into the hash table\n");
        printf("5-  Search for a specific medicine and print the number of collisions to reach the medicine\n");
        printf("6- Delete a specific record\n");
        printf("7- Save the data back to the file Pharmacy.txt\n");
        printf("8- exit\n");
        scanf("%d",&ch);

        switch(ch){
            case 1: printHash();
                break;
            case 2:
                printf("The hash table size = %d\n",SIZE);
                break;
            case 3:
                printf("The linear hashing was used: h(x)=(hashValue + i) mod tableSize , 0<=i<=tableSize\n");
                break;
            case 4:
                insertNew_intoHashTable();
                break;
            case 5:
                printf("Enter the name of medicine:\n");
                char k[N];
                char line6[100];
                gets(line6);
                gets(k);
               int res = searchHash(k,SIZE);
               if(res != -1){
                   printf("Medicine %s is found\n",k);
                   printf("number of collisions = %d\n",res);
               }

               else
                   printf("Medicine %s is not found\n",k);

              break;

            case 6:
                deleteHash();
                break;

            case 7:
                printHashFile();
                printf("Data saved back to file Pharmacy.txt\n");
                break;

            case 8: exit(1) ;

        }
    }
}

void printHash()
{
    printf("helooo\n");
    for(int i=0; i<SIZE; i++)
    {
        if(table[i]->status==1)
            printf("\n[%d]: %s----%s----%s----%s----%s----%d----%s\n", i, table[i]->name,table[i]->activeIngredient,table[i]->category,table[i]->indication,table[i]->expiryDate,table[i]->count,table[i]->company);
        else
            printf("\n[%d]: %s----%s----%s----%s----%s----%d----%s\n",i,NULL,NULL,NULL,NULL,NULL,0,NULL);
        printf("*************************************************************************************\n");
    }
}

int searchHash(char key[], int TableSize) {
    char n[4];
    strncpy(n, key, 3);
    n[3] = '\0';

    int hashValue = hash(n, TableSize);
    int index = hashValue;
    int collisionCount = 0;
    int found =0;

    while (table[index]->status != 0) {
        if (strcmp(table[index]->name, key) == 0) {
            found = 1;
            break;
        }
        index = (hashValue + collisionCount) % TableSize;
        collisionCount++;
    }

    // when table[index].status != 0 it means that this index is occupied
    // it enters the if statement, if the condition is true from the first time then it's found without any collision
    // if the condition is false, we step by 1 each time because it's linear hashing


    if (found) {
        return collisionCount;
    } else {
        return -1;   // if found=0 means the last index it entered was empty and key is not found
    }
}

void printHashFile()
{
    FILE* w=fopen("Pharmacy.txt", "w");
    for(int i=0; i<SIZE; i++)
    {
        if(table[i]->status==1)
            fprintf(save_data,"%s:%s:%s:%s:%s:%d:%s\n",table[i]->name,table[i]->activeIngredient,table[i]->category,table[i]->indication,table[i]->expiryDate,table[i]->count,table[i]->company);
    }
    fclose(w);
}

void deleteHash(){
    char key[N];
    char line[N];
    printf("Enter the name of medicine you want to delete\n");
    gets(line);
    fgets(key,N,stdin);
    key[strcspn(key, "\n")] = 0;

    for(int i=0; i<SIZE; i++){
        if((strcmp(key, table[i]->name)==0) && (table[i]->status==1)){
            table[i] ->status=0;
          //  table[i] = NULL;
            COUNTER --;
        }
    }
}

//this function is to make a new array and move the data from old array into it
// then free the old array and malloc it again with the new size and re enter the data into it
// so we get the old array rehashed
void reHash(){
    int oldSize= SIZE;
    SIZE= getNewSize(SIZE);
    hashTable newTable[SIZE];

    // initialize the new hash table
    for(int i=0; i<SIZE; i++){
        newTable[i]= (hashTable)malloc( sizeof( struct hash_node ));
        newTable[i]->status=0;
    }

    for(int i=0; i<oldSize; i++)
    {
        if(table[i]->status ==1)
        {
            insert_newTable(table[i]->name,table[i]->activeIngredient,table[i]->category,table[i]->indication,table[i]->expiryDate,table[i]->count,table[i]->company, newTable);

        }
        free(table[i]);
    }
    for(int i=0; i<SIZE; i++) {
        table[i] = (hashTable) malloc(sizeof(struct hash_node));
        table[i]->status = 0;
    }

    for(int i=0; i<SIZE; i++)
    {
        strcpy(table[i]->name, newTable[i]->name);
        strcpy(table[i]->activeIngredient, newTable[i]->activeIngredient);
        strcpy(table[i]->category, newTable[i]->category);
        strcpy(table[i]->indication, newTable[i]->indication);
        strcpy(table[i]->expiryDate, newTable[i]->expiryDate);
        table[i]->count = newTable[i]->count;
        strcpy(table[i]->company, newTable[i]->company);
        table[i]->status=newTable[i]->status;
        free(newTable[i]);
    }


}

void insert_newTable(char Name[N],char ActiveIngredient[N],char Category[N],char Indication[100],char Expire[N],int Count,char Company[N],hashTable arr[])
{
    int index;
    char n[4];
    strncpy(n, Name, 3);
    n[3] = '\0';
    index = getIndex(n, SIZE);


    strcpy(arr[index]->name, Name);
    strcpy(arr[index]->indication, ActiveIngredient);
    strcpy(arr[index]->category, Category);
    strcpy(arr[index]->indication, Indication);
    strcpy(arr[index]->expiryDate, Expire);
    arr[index]->count = Count;
    strcpy(arr[index]->company, Company);
    arr[index]->status=1;

}

