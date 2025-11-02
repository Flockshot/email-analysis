#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIRECTORY_SIZE 30

struct Email
{
    int id;
    char sender[50];
    char receipt[50];
    int date;
    int words;
};
typedef struct Email* Email;



Email *readEmails(char *directory, int *numOfFiles);
void menu();
Email *heapSort(Email *emails, int numOfEmails, int criteria);
Email binSearch(Email *sortedEmails, int numOfEmails, int searchKey);
void printEmails(Email *emails, int numOfEmails);


char *buildFilePathString(char *directory, int number);
int countWords(FILE *inFile);
void buildHeap(Email *emails, int numOfEmails, int criteria);
void heapify(Email *emails, int index, int numOfEmails, int criteria);
int getCriteriaValue(Email email, int criteria);
void swap(Email *emails, int swarpIndex1, int swapIndex2);
void printEmail(Email email);


int main()
{
    char *directory = malloc(sizeof(char) * DIRECTORY_SIZE);
    int numOfFiles;

    printf("Enter the path of the directory which contains data files: ");
    scanf("%s", directory);
    printf("Enter the number of data files: ");
    scanf("%d", &numOfFiles);

    Email *emails = readEmails(directory, &numOfFiles);

    if(numOfFiles>0)
    {
        int choice;
        int searchKey;

        do
        {
            menu();

            printf("Command: ");
            scanf("%d", &choice);

            switch(choice)
            {
                case 1:
                    printEmails(heapSort(emails, numOfFiles, 1), numOfFiles);
                    break;
                case 2:
                    printEmails(heapSort(emails, numOfFiles, 2), numOfFiles);
                    break;
                case 3:
                    printEmails(heapSort(emails, numOfFiles, 3), numOfFiles);
                    break;
                case 4:
                    printf("Enter the search key: ");
                    scanf("%d", &searchKey);
                    printEmail(binSearch(heapSort(emails, numOfFiles, 1), numOfFiles, searchKey));
                    printf("\n");
                    break;
            }
        }
        while(choice!=5);
    }

    printf("Goodbye!\n");
    return 0;
}

//Time complexity is O(n^2), where n is numOfFiles
//Since we have a loop, which repeats O(n) complexity function n times, (countWords)
Email *readEmails(char *directory, int *numOfFiles)
{
    Email *emails = (Email*) malloc(*numOfFiles * sizeof(Email));

    int i;

    for(i=1; i<=*numOfFiles; i++)
    {
        char *file = buildFilePathString(directory, i);

        FILE *inFile;
        inFile = fopen(file, "r");

        if(inFile == NULL)
        {
            printf("Invalid directory or file: %s\n", file);
            printf("Halting the read operation.\n");
            break;
        }

        Email email = (Email) malloc(sizeof(struct Email));

        fscanf(inFile, "%d\nFrom: %s\nTo: %s\nDate: %d\n", &email->id, email->sender, email->receipt, &email->date);

        email->words = countWords(inFile);

        emails[i-1] = email;

        fclose(inFile);
    }

    printf("\n%d emails have been read successfully!\n\n", (i-1));
    *numOfFiles = (i-1);

    return emails;
}

//Time complexity is O(1)
//Since there is no loop and each operation performs in constant time.
void menu()
{
        printf("Please choose one of the following options:\n");
        printf("\t(1) Display emails sorted by id\n");
        printf("\t(2) Display emails sorted by number of words\n");
        printf("\t(3) Display emails sorted by date\n");
        printf("\t(4) Search email by ID\n");
        printf("\t(5) Exit\n\n");
}

//Time complexity is O(n*lg(n)), where n = numOfEmails
//Since we have buildHeap function that also have O(n*lg(n))
//and then we have heapify function with O(lg(n)) repeating inside a loop n times.
Email *heapSort(Email *emails, int numOfEmails, int criteria)
{
    buildHeap(emails, numOfEmails, criteria);

    for(int i=numOfEmails-1; i>=1; i--)
    {
        swap(emails, 0, i);
        heapify(emails, 0, i, criteria);
    }

    return emails;
}

//Time complexity is O(lg(n))
//Since we have a loop, which repeats the constant time operations lg(n) times.
//The loop repeats lg(n) times as on every iteration the size is effectively halved.
Email binSearch(Email *sortedEmails, int numOfEmails, int searchKey)
{
    int min = 0;
    int max = numOfEmails-1;

    while(max-min > 0)
    {
        int mid = (min+max)/2;

        if(sortedEmails[mid]->id < searchKey)
            min = mid+1;
        else
            max = mid;
    }

    if(sortedEmails[min]->id == searchKey)
        return sortedEmails[min];
    else
        printf("Search key not found.\n");

    return NULL;
}

//Time complexity is O(n), where n = numOfEmails
//Since we have a loop, which repeats O(1) complexity function n times.
void printEmails(Email *emails, int numOfEmails)
{
    for(int i=0; i<numOfEmails; i++)
        printEmail(emails[i]);
    printf("\n");
}




//Time complexity is O(1)
//Since there is no loop and each operation performs in constant time.
char *buildFilePathString(char *directory, int number)
{
    char *file = (char*) malloc(sizeof(char) * DIRECTORY_SIZE);

    char fileNumber[3];
    sprintf(fileNumber, "%d", number);

    strcpy(file, directory);
    strcat(file, "\\");
    strcat(file, fileNumber);
    strcat(file, ".txt");

    return file;
}

//Time complexity is O(n), where n is the number of characters in the file.
//Since we have a loop, which repeats the constant time operations n times.
int countWords(FILE *inFile)
{
    if(inFile != NULL)
    {
        char currentFileChar = fgetc(inFile);
        int words = 0;
        while(currentFileChar != EOF)
        {
            if(currentFileChar==' ')
                words++;

            currentFileChar = fgetc(inFile);
        }
        return words==0 ? words : words+1;
    }

    return 0;
}

//Time complexity is O(n*lg(n)), where n = (numOfEmails/2)
//Since the function is calling heapify n times, which has O(lg(n)) complexity.
void buildHeap(Email *emails, int numOfEmails, int criteria)
{
    for(int i = (numOfEmails/2)-1; i>=0; i--)
        heapify(emails, i, numOfEmails, criteria);
}

//Time complexity is O(lg(n)), where n is the height of the tree.
//Since the function is recursive, and each time it is called, it jumps up the height of the tree.
//In best case it can also be O(1), if the index value is already the largest, meaning the subtree is sorted.
void heapify(Email *emails, int index, int numOfEmails, int criteria)
{
    int leftIndex = (2*index)+1;
    int rightIndex = (2*index)+2;

    Email left = emails[leftIndex];
    Email right = emails[rightIndex];

    int largest;
    int indexValue = getCriteriaValue(emails[index], criteria);

    if(leftIndex < numOfEmails && getCriteriaValue(left, criteria) > indexValue)
        largest = leftIndex;
    else
        largest = index;

    int largestValue = getCriteriaValue(emails[largest], criteria);

    if(rightIndex < numOfEmails && getCriteriaValue(right, criteria) > largestValue)
        largest = rightIndex;


    if(largest != index)
    {
        swap(emails, index, largest);
        heapify(emails, largest, numOfEmails, criteria);
    }
}

//Time complexity is O(1)
//Since there is no loop and each operation performs in constant time.
int getCriteriaValue(Email email, int criteria)
{
    switch(criteria)
    {
        case 1:
            return email->id;
        case 2:
            return email->words;
        case 3:
            return email->date;
    }

    return 0;
}

//Time complexity is O(1)
//Since there is no loop and each operation performs in constant time.
void swap(Email *emails, int swarpIndex1, int swapIndex2)
{
    Email temp = emails[swarpIndex1];
    emails[swarpIndex1] = emails[swapIndex2];
    emails[swapIndex2] = temp;
}

//Time complexity is O(1)
//Since there is no loop and each operation performs in constant time.
void printEmail(Email email)
{
    if(email!=NULL)
        printf("\nId: %d\nSender: %s\nRecipient: %s\nDate: %d\nWords: %d\n", email->id, email->sender, email->receipt, email->date, email->words);
}
