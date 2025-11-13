#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIRECTORY_SIZE 30

typedef char* string;

struct Email
{
    int id;
    char sender[51];
    char receipt[51];
    int date;
    int words;
    string content;
};
typedef struct Email* Email;

struct GraphHead
{
    int count;
    struct GraphVertex *first;
};
typedef struct GraphHead* Graph;

struct GraphVertex
{
    struct GraphVertex *next;
    char data[51];
    int totalWordsReceived;
    int inDegree;
    int outDegree;
    int processed;
    struct GraphArc *firstArc;
};
typedef struct GraphVertex* Vertex;

struct GraphArc
{
    Vertex destination;
    int weight;
    struct GraphArc* nextArc;
};
typedef struct GraphArc* Arc;


struct Node
{
    struct Node *next;
    Vertex vertex;
};
typedef struct Node* List;







Graph readEmails(char *directory, int *numOfFiles);
Graph createGraph();
void createVertex(Graph head, string data);
int createEdge(Graph head, string from, string to, int words);
void printGraph(Graph head);
int checkPath(Graph head, string from, string to);


//////////////////////////////////////////////////////


char *buildFilePathString(char *directory, int number);
string readContent(FILE *inFile, int* words);
Vertex getVertex(Graph head, string data);



int main()
{
    char *directory = malloc(sizeof(char) * DIRECTORY_SIZE);
    int numOfFiles;

    printf("Enter the path of the directory which contains data files: ");
    scanf("%s", directory);
    printf("Enter the number of data files: ");
    scanf("%d", &numOfFiles);

    Graph head = readEmails(directory, &numOfFiles);

    printGraph(head);

    char from[51];
    char to[51];
    printf("\nEnter the first person: ");
    scanf("%s", from);
    printf("Enter the second person: ");
    scanf("%s", to);

    int havePath = checkPath(head, from, to);

    if(havePath)
        printf("\nThere exists a path from %s to %s\n", from, to);
    else
        printf("\nNo path exists from %s to %s\n", from, to);

    return 0;
}



Graph readEmails(char *directory, int *numOfFiles)
{
    Email *emails = (Email*) malloc(*numOfFiles * sizeof(Email));

    int i;

    Graph head = createGraph();

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
        email->content = readContent(inFile, &email->words);


        createVertex(head, email->sender);
        createVertex(head, email->receipt);
        int error = createEdge(head, email->sender, email->receipt, email->words);

        if(error == -1)
            printf("Edge not created for file number %d because the sender does not exist\n", i);
        else if(error == -2)
            printf("Edge not created for file number %d because the receiver does not exist\n", i);



        //printf("Content: %s\n", email->content);
        //printf("Words: %d\n", email->words);


        emails[i-1] = email;

        fclose(inFile);
    }


    printf("\n%d emails have been read successfully!\n\n", (i-1));
    *numOfFiles = (i-1);

    return head;
}


Graph createGraph()
{
    Graph head = (Graph) malloc(sizeof(struct GraphHead));
    head->count=0;
    head->first = NULL;
    return head;
}


void createVertex(Graph head, string data)
{
    Vertex vertex = (Vertex) malloc(sizeof(struct GraphVertex));

    vertex->next = NULL;
    vertex->inDegree = 0;
    vertex->outDegree = 0;
    vertex->processed = 0;
    vertex->totalWordsReceived = 0;
    vertex->firstArc = NULL;
    strcpy(vertex->data, data);

    if(head->first == NULL)
        head->first = vertex;
    else
    {
        Vertex temp = head->first;

        while(temp->next != NULL)
        {
            if(!strcmp(temp->data, vertex->data))
                return;
            temp = temp->next;
        }

        if(!strcmp(temp->data, vertex->data))
            return;

        temp->next = vertex;
    }

    head->count++;
}


int createEdge(Graph head, string from, string to, int words)
{
    Arc arc = (Arc) malloc(sizeof(struct GraphArc));
    Vertex fromVertex = getVertex(head, from);
    Vertex toVertex = getVertex(head, to);

    if(fromVertex == NULL)
        return -1;

    if(toVertex == NULL)
        return -2;

    fromVertex->outDegree++;
    toVertex->inDegree++;

    arc->destination = toVertex;
    arc->weight = words;
    arc->nextArc = NULL;

    toVertex->totalWordsReceived += words;


    if(fromVertex->firstArc == NULL)
        fromVertex->firstArc = arc;
    else
    {
        Arc temp = fromVertex->firstArc;

        if(temp->destination == toVertex)
        {
            temp->weight += words;
            return 1;
        }

        while(temp->nextArc != NULL)
        {
            if(temp->destination == toVertex)
            {
                temp->weight += words;
                return 1;
            }

            temp = temp->nextArc;
        }

        temp->nextArc=arc;
    }

    return 1;
}

void printGraph(Graph head)
{
    printf("\nThe resulting graph's adjacency list:\n");

    List maxEmailList = (List) malloc(sizeof(struct Node));
    maxEmailList->next = NULL;
    maxEmailList->vertex = NULL;
    List maxWordsList = (List) malloc(sizeof(struct Node));
    maxWordsList->next = NULL;
    maxWordsList->vertex = NULL;


    Vertex tempVertex = head->first;

    while(tempVertex != NULL)
    {
        printf("%s", tempVertex->data);

        Arc tempArc = tempVertex->firstArc;
        while(tempArc != NULL)
        {
            printf(" -> %s | %d", tempArc->destination->data, tempArc->weight);
            tempArc = tempArc->nextArc;
        }
        printf("\n");

        if(maxEmailList->vertex == NULL)
        {
            maxEmailList->vertex = tempVertex;
        }
        else
        {
            if(tempVertex->outDegree > maxEmailList->vertex->outDegree)
            {
                maxEmailList->vertex = tempVertex;
                maxEmailList->next = NULL;
            }
            else if(tempVertex->outDegree == maxEmailList->vertex->outDegree)
            {
                maxEmailList->next = (List) malloc(sizeof(struct Node));
                maxEmailList->next->next = NULL;
                maxEmailList->vertex = tempVertex;
            }
        }

        if(maxWordsList->vertex == NULL)
            maxWordsList->vertex = tempVertex;
        else
        {
            if(tempVertex->totalWordsReceived > maxWordsList->vertex->totalWordsReceived)
            {
                maxWordsList->vertex = tempVertex;
                maxWordsList->next = NULL;
            }
            else if(tempVertex->totalWordsReceived == maxWordsList->vertex->totalWordsReceived)
            {
                maxWordsList->next = (List) malloc(sizeof(struct Node));
                maxWordsList->next->next = NULL;
                maxWordsList->vertex = tempVertex;
            }
        }

        tempVertex = tempVertex->next;
    }

    printf("\nPeople with the maximum number of emails sent:\n");
    List tempList = maxEmailList;
    do
    {
        if(tempList->vertex != NULL)
            printf("%s sent %d emails\n", tempList->vertex->data, tempList->vertex->outDegree);
        tempList = tempList->next;
    }
    while(tempList != NULL);


    printf("\nPeople with the maximum number of words received:\n");
    tempList = maxWordsList;
    do
    {
        if(tempList->vertex != NULL)
            printf("%s received %d words\n", tempList->vertex->data, tempList->vertex->totalWordsReceived);
        tempList = tempList->next;
    }
    while(tempList != NULL);
}


int checkPath(Graph head, string from, string to)
{
    Vertex tempVertex = head->first;
    while(tempVertex != NULL)
    {
        if(!strcmp(tempVertex->data, from))
        {
            Arc tempArc = tempVertex->firstArc;
            while(tempArc != NULL)
            {
                if(!strcmp(tempArc->destination->data, to))
                    return 1;
                tempArc = tempArc->nextArc;
            }
            return 0;
        }
        tempVertex = tempVertex->next;
    }
    return 0;
}



/////////////////////////////////////////////////////////////////////


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

string readContent(FILE *inFile, int* words)
{
    if(inFile != NULL)
    {
        int sizeIncrement = 10;
        string temp = (string) calloc(sizeIncrement, sizeof(char));
        int i = 0;
        *words = 0;

        char currentFileChar = fgetc(inFile);
        while(currentFileChar != EOF)
        {
            if(currentFileChar==' ')
                (*words)++;
            if(i%sizeIncrement == 0)
                temp = (string) realloc(temp, i+sizeIncrement);
            temp[i] = currentFileChar;
            i++;
            currentFileChar = fgetc(inFile);
        }

        if((*words)!=0)
            (*words) += 1;

        string content = (string) calloc(i-1, sizeof(char));
        strncpy(content, temp, i-1);

        free(temp);

        return content;
    }

    return NULL;
}

Vertex getVertex(Graph head, string data)
{
    Vertex vertex = head->first;

    while(vertex != NULL && strcmp(vertex->data, data))
        vertex = vertex->next;

    return vertex;
}
