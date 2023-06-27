#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS 11  //metavliti gia hashing
#define TRUE 1  //to 1 einai mpanal
#define FALSE 0 //kai to 0.

struct hash *hashTable = NULL;  //metavliti gia hashing

typedef struct student  //struct eggrafis foitith
{
    char AM[10];
    char name[20];
    char surname[20];
    char grade[10];
    struct student *sameGrade;  //xrhsimopoieitai sto B erwthma
    struct student *left;
    struct student *right;
    struct student *next; //xrhsimopoieitai sto G erwthma
} Student, *Studentptr;

struct hash { //struct hash
    Student *head;
    int count;
};

Studentptr newStudent(char *AM, char *name, char *surname, char *grade)    //eisagwgh neas eggrafhs
{
    Studentptr newStudent = (Studentptr) malloc(sizeof(Student));
    strcpy(newStudent->AM, AM);
    strcpy(newStudent->name, name);
    strcpy(newStudent->surname, surname);
    strcpy(newStudent->grade, grade);
    newStudent->sameGrade = NULL;
    newStudent->left = NULL;
    newStudent->right = NULL;
    newStudent->next = NULL;
    return newStudent;
}

void inorder(Studentptr root) { //diavasma aristera, riza, deksia
    if (root != NULL) {
        inorder(root->left);
        printf("%s %s %s %s\n", root->AM, root->name, root->surname, root->grade);
        inorder(root->right);
    }
}

int stringToKey(char *AM) { //dhmioyrgei ta hashes
    int i, sum = 0;
    for (i = 0; i < strlen(AM); i++) {
        sum += AM[i];
    }
    return sum % BUCKETS;
}

int stringCompare(char string1[], char string2[]) { //nai yparxei hdh sthn C, alla den thn kaname tzampa
    //epistrefei 1 an string1 < string2, -1 gia > , 0 gia isa
    int i = 0, flag = TRUE, returnValue = FALSE;

    while (i < 8) {
        if (string1[i] < string2[i]) {
            returnValue = 1;
            break;
        } else if (string1[i] > string2[i]) {
            returnValue = -1;
            break;
        } else if (string1[i] == string2[i]) {
            i++;
            if (i == 9)
                returnValue = 0;
        }
    }

    return returnValue;
}

Studentptr insertByAM(Studentptr root, Studentptr student) {
    /* An to dentro einai adeio epestrepse mia nea eggrafh */
    if (root == NULL)
        return student;

    /* Alliws anadromh sto dentro */
    if ((stringCompare(student->AM, root->AM)) == 1)
        root->left = insertByAM(root->left, student);
    else if ((stringCompare(student->AM, root->AM)) == -1)
        root->right = insertByAM(root->right, student);
    return root;
}

Studentptr insertByGrade(Studentptr root, Studentptr student) {

    if (root == NULL)
        return student;

    if (atoi(student->grade) < atoi(root->grade))
        root->left = insertByGrade(root->left, student);
    else if (atoi(student->grade) > atoi(root->grade))
        root->right = insertByGrade(root->right, student);
    else if (atoi(student->grade) == atoi(root->grade))
        root->sameGrade = insertByGrade(root->sameGrade, student);

    return root;
}

Student **read_file() {
    FILE *fpointer;
    fpointer = fopen("Foitites-Vathmologio-DS.txt", "r");

    int line = 0, i = 0;
    Studentptr tempStudent = newStudent("", "", "", "");
    Studentptr *array = (Studentptr *) malloc(20 * sizeof(Studentptr));  //grammes

    char string[100];

    while (!feof(fpointer)) {
        fgets(string, 100, fpointer);

        fscanf(fpointer, "%s %s %s %s", tempStudent->AM, tempStudent->name, tempStudent->surname, tempStudent->grade);
        array[line] = newStudent(tempStudent->AM, tempStudent->name, tempStudent->surname, tempStudent->grade);
        line++;
        i++;
    }

    fclose(fpointer);

    return array;
}

Studentptr searchStudent(Studentptr student, char AM[]) {
    if (student == NULL || strcmp(student->AM, AM) == 0)
        return student;

    if (stringCompare(student->AM, AM) == 1)  //foithths deksia
        return searchStudent(student->right, AM);

    return searchStudent(student->left, AM);  //aristera
}

Studentptr editStudent(Studentptr student, int userChoice, char chosenAM[], char newString[]) {
    Studentptr studentPointer = searchStudent(student, chosenAM);
    if (userChoice == 1)
        strcpy(studentPointer->name, newString);
    else if (userChoice == 2)
        strcpy(studentPointer->surname, newString);
    else if (userChoice == 3)
        strcpy(studentPointer->grade, newString);
    return studentPointer;
}

Studentptr findMinStudent(Studentptr student) {
    if (student == NULL)
        return NULL;
    else if (student->left != NULL)
        return findMinStudent(student->left);
    return student;
}

Studentptr deleteStudent(Studentptr student, char chosenAM[]) {
    //anazhthsh foithth
    if (student == NULL)
        return NULL;
    if (stringCompare(chosenAM, student->AM) == -1)
        student->right = deleteStudent(student->right, chosenAM);
    else if (stringCompare(chosenAM, student->AM) == 1)
        student->left = deleteStudent(student->left, chosenAM);
    else {
        //kanena paidi
        if (student->left == NULL && student->right == NULL) {
            free(student);
            return NULL;
        }
            //ena paidi
        else if (student->left == NULL || student->right == NULL) {
            Studentptr tempStudent;
            if (student->left == NULL)
                tempStudent = student->right;
            else
                tempStudent = student->left;
            free(student);
            return tempStudent;
        }
            //dyo
        else {
            Studentptr tempStudent = findMinStudent(student->right);
            strcpy(student->AM, tempStudent->AM);
            strcpy(student->name, tempStudent->name);
            strcpy(student->surname, tempStudent->surname);
            strcpy(student->grade, tempStudent->grade);
            student->right = deleteStudent(student->right, tempStudent->AM);
        }
    }
    return student;
}

Studentptr findMinGrades(Studentptr student) {
    static int FLAG = TRUE, counter = 0;

    if (student->left == NULL)
        FLAG = FALSE;

    if (FLAG == TRUE) {
        if (student == NULL)
            return NULL;
        else if (student->left != NULL || student->sameGrade != NULL)
            if (student->left != NULL)
                return findMinGrades(student->left);
    } else if (FLAG == FALSE) {
        if (counter == 0) {
            counter++;
            return student;
        }
        student = student->sameGrade;
        return student;
    }

    return student;
}

Studentptr findMaxGrades(Studentptr student) {
    static int FLAG = TRUE, counter = 0;

    if (student->right == NULL)
        FLAG = FALSE;

    if (FLAG == TRUE) {
        if (student == NULL)
            return NULL;
        else if (student->right != NULL || student->sameGrade != NULL)
            if (student->right != NULL)
                return findMinGrades(student->right);
    } else if (FLAG == FALSE) {
        if (counter == 0) {
            counter++;
            return student;
        }
        student = student->sameGrade;
        return student;
    }

    return student;
}

Studentptr insertToHash(Studentptr n) {
    int hashIndex = stringToKey(n->AM);
    Studentptr newNode = newStudent(n->AM, n->name, n->surname, n->grade);
    if (!hashTable[hashIndex].head) {
        hashTable[hashIndex].head = newNode;
        hashTable[hashIndex].count = 1;
        return newNode;
    }
    // neo node sth lista
    newNode->next = (hashTable[hashIndex].head);

    // update the head of the list and number of
    // nodes in the current bucket
    hashTable[hashIndex].head = newNode;
    hashTable[hashIndex].count++;
    return n;
}

void searchInHash(char AM[]) {
    Studentptr n;
    int hashIndex = stringToKey(AM);
    int flag = 0;
    n = hashTable[hashIndex].head;
    if (!n) {
        printf("\nDen vrethke tetoia eggrafh\n");
    }
    while (n != NULL) {
        if (strcmp(n->AM, AM) == 0) {
            printf("%s %s %s %s", n->AM, n->name, n->surname, n->grade);
            flag = 1;
            break;
        }
        n = n->next;
    }
    if (!flag)
        printf("\nDen vrethke tetoia eggrafh\n");
}


void deleteFromHash(char AM[]) {
    Studentptr temp, myNode;
    int hashIndex = stringToKey(AM);
    int flag = 0;
    myNode = hashTable[hashIndex].head;
    if (!myNode) {
        printf("\nDen vrethke tetoia eggrafh\n");
        return;
    }
    temp = myNode;
    while (myNode != NULL) {
        if (strcmp(myNode->AM, AM) == 0) {
            flag = 1;
            if (myNode == hashTable[hashIndex].head)
                hashTable[hashIndex].head = myNode->next;
            else
                temp->next = myNode->next;
            hashTable[hashIndex].count--;
            free(myNode);
            break;
        }
        temp = myNode;
        myNode = myNode->next;
    }
    if (flag)
        printf("\nO foithths diagrafthke.\n");
    else
        printf("\nDen vrethke tetoia eggrafh\n");
}

void editInHash(int choice, char AM[], char newstring[]) {
    Studentptr editNode;
    int hashIndex = stringToKey(AM);
    int flag = 0;
    editNode = hashTable[hashIndex].head;
    if (!editNode) {
        printf("\nDen vrethke tetoia eggrafh\n");
    }
    while (editNode != NULL) {
        if (strcmp(editNode->AM, AM) == 0) {
            if (choice == 1)
                strcpy(editNode->name, newstring);
            if (choice == 2)
                strcpy(editNode->surname, newstring);
            if (choice == 3)
                strcpy(editNode->grade, newstring);
            printf("%s %s %s %s\n", editNode->AM, editNode->name, editNode->surname, editNode->grade);
            flag = 1;
            break;
        }
        editNode = editNode->next;
    }
    if (!flag) {
        printf("\nDen vrethke tetoia eggrafh\n");
    }
}


void display() {
    Studentptr myNode;
    int i;
    for (i = 0; i < BUCKETS; i++) {
        if (hashTable[i].count == 0)
            continue;
        myNode = hashTable[i].head;
        if (!myNode)
            continue;
        printf("\nData at index %d in Hash Table:\n", i);
        while (myNode != NULL) {
            printf("%s", myNode->AM);
            /* printf("%s", myNode->name);
            printf("%s", myNode->surname);
            printf("%s", myNode->grade); */
            printf("\n");
            myNode = myNode->next;
        }
    }
}

void userMenuA() {
    int userChoice, userSubChoice, flag = TRUE, subflag, j = 0;
    char chosenAM[20], newString[20];
    Studentptr root = NULL;
    Studentptr tempStudent;

    struct student **students = read_file();

    //ARXIKOPOIHSH DENTROY
    root = insertByAM(root, students[0]);
    for (j = 1; j < 19; j++) {
        root = insertByAM(root, students[j]);
    }

    printf("\n===[DDA ME VASH AM]===\n");
    while (strlen("Tsipras") > strlen("Koulis")) { //panta true einai, xexe
        printf("\n\nTi goustareis?\n");
        printf("(1) In-order apeikonhsh\n");
        printf("(2) Anazhthsh vasei AM\n");
        printf("(3) Tropopoihsh vasei AM\n");
        printf("(4) Diagrafh vasei AM\n");
        printf("(5) Eksodos\n");

        scanf("%d", &userChoice);

        if (userChoice == 1)
            inorder(root);
        else if (userChoice == 2) {
            printf("Dwse AM gia anazhthsh: ");
            scanf("%s", chosenAM);
            tempStudent = searchStudent(root, chosenAM);
            printf("%s %s %s %s", tempStudent->AM, tempStudent->name, tempStudent->surname, tempStudent->grade);
        } else if (userChoice == 3) {
            while (flag == TRUE) {
                printf("Dwse AM gia tropopoihsh plhroforiwn: ");
                scanf("%s", chosenAM);
                subflag = TRUE;
                while (subflag == TRUE) {
                    printf("Mporeis na allakseis opoiadhpote timh pera apo to AM:\n");
                    printf("(1) Allagw onomatos\n(2) Allagh epwnymoy\n(3) Allagh vathmou\n");
                    printf("Τι θέλεις να αλλάξεις; ");
                    scanf("%d", &userSubChoice);
                    printf("Dwse kainouria timh gia thn epilogh sou: ");
                    scanf("%s", newString);
                    tempStudent = editStudent(root, userSubChoice, chosenAM, newString);
                    printf("Kainouries times:\n");
                    printf("%s %s %s %s\n", tempStudent->AM, tempStudent->name, tempStudent->surname,
                           tempStudent->grade);
                    printf("\nGrapse (1) an epithymeis kai allh allagh sto idio atomo,\n"
                                   "(2) gia na allakseis AM \n(0) gia epistrofh sto menu: ");
                    scanf("%d", &userSubChoice);
                    if (userSubChoice == 0) {
                        subflag = FALSE;
                        flag = FALSE;
                    } else if (userSubChoice == 2)
                        subflag = FALSE;
                    printf("\n");
                }
            }
        } else if (userChoice == 4) {
            printf("Dwse AM gia diagrafh: ");
            scanf("%s", chosenAM);
            root = deleteStudent(root, chosenAM);
            printf("O fothths diagrafthke.");
        } else if (userChoice == 5)
            break;
    }
}

void userMenuB() {
    int userChoice, flag = TRUE, i = 0, j = 0;
    Studentptr root = NULL;

    Studentptr *students = read_file();
    Studentptr tempStudent;

    root = insertByGrade(root, students[0]);

    for (j = 1; j < 19; j++) {
        root = insertByGrade(root, students[j]);
    }


    printf("\n===[DDA ME VASH VATHMOLOGIA]===\n");
    while (strlen("Tsipras") > strlen("Koulis")) { //παντα true ειναι, χεχε
        printf("\n\nTi goustareis;\n");
        printf("(1) Eyresh twn foithtwn me thn xeiroterh vathmologia\n");
        printf("(2) Eyresh twn foithtwn me tnh kalyterh vathmologia\n");
        printf("(3) Έξοδος\n");

        scanf("%d", &userChoice);

        if (userChoice == 1) {
            printf("Mathtes me thn xeiroterh vathmologia:\n");
            tempStudent = root;
            while (tempStudent != NULL) {
                tempStudent = findMinGrades(tempStudent);
                if (tempStudent != NULL)
                    printf("%s %s %s %s\n", tempStudent->AM, tempStudent->name, tempStudent->surname,
                           tempStudent->grade);
            }
        } else if (userChoice == 2) {
            printf("Mathtes me thn kalyterh vamthmologia:\n");
            tempStudent = root;
            while (tempStudent != NULL) {
                tempStudent = findMaxGrades(tempStudent);
                if (tempStudent != NULL)
                    printf("%s %s %s %s\n", tempStudent->AM, tempStudent->name, tempStudent->surname,
                           tempStudent->grade);
            }
        } else if (userChoice == 3)
            break;
    }
}

void userMenuC() {
    int j, choice, subchoice, flag = TRUE, subflag = TRUE;
    char chosenAM[20], newString[20];
    Studentptr foititis = NULL;
    struct student **students = read_file();
    hashTable = (struct hash *) calloc(BUCKETS, sizeof(struct hash));

    for (j = 0; j < 19; j++) {
        foititis = insertToHash(students[j]);
    }
    while (strlen("Arxigos") > strlen("Helena")) {
        printf("\n\nTi goustareis;\n");
        printf("(1) Provolh toy pinaka katakermatismou\n");
        printf("(2) Anazhthsh vash tou AM\n");
        printf("(3) Tropopoihsh vasei toy AM\n");
        printf("(4) Diagrafh vasei toy AM\n");
        printf("(5) Eksodos\n");

        scanf("%d", &choice);

        if (choice == 2) {
            printf("Dwse AM gia anazhthsh: ");
            scanf("%s", chosenAM);
            searchInHash(chosenAM);
        } else if (choice == 3) {
            while (flag == TRUE) {
                printf("Dwse Am gia tropopoihsh stoixeiwn: ");
                scanf("%s", chosenAM);
                subflag = TRUE;
                while (subflag == TRUE) {
                  printf("Mporeis na allakseis opoiadhpote timh pera apo to AM:\n");
                  printf("(1) Allagw onomatos\n(2) Allagh epwnymoy\n(3) Allagh vathmou\n");
                  printf("Ti theleis na allakseis; ");
                  scanf("%d", &subchoice);
                  printf("Dwse kainouria timh gia thn epilogh sou: ");
                    scanf("%s", newString);
                    editInHash(subchoice, chosenAM, newString);
                    printf("\nGrapse (1) an epithymeis kai allh allagh gia to idio atomo,\n"
                                   "(2) gia na allakseis AM\n (0) gia epistrofh sto menu: ");
                    scanf("%d", &subchoice);
                    if (subchoice == 0) {
                        subflag = FALSE;
                        flag = FALSE;
                    } else if (subchoice == 2)
                        subflag = FALSE;
                    printf("\n");
                }
            }

        } else if (choice == 4) {
            printf("Dwse AM gia diagrafh: ");
            scanf("%s", chosenAM);
            deleteFromHash(chosenAM);
        } else if (choice == 5)
            break;
        else if (choice == 1)
            display();
        else
            printf("Oyps ekanes lathos!\nElpizoume thn epomenh fora na KSEREIS NA METRAS WS TO 5.\n");
    }

}

void fireworks() {
    printf("\nOYOYOYOYOYOYOYOY!\n");
    printf("                                   .''.       \n");
    printf("       .''.      .        *''*    :_\\/_:     . \n");
    printf("      :_\\/_:   _\\(/_  .:.*_\\/_*   : /\\ :  .'.:.'.\n");
    printf("  .''.: /\\ :   ./)\\   ':'* /\\\* :  '..'.  -=:o:=-\n");
    printf(" :_\\/_:'.:::.    ' *''*    * '.\\'/.' _\\(/_'.':'.'\n");
    printf(" : /\\ : :::::     *_\\/_*     -= o =-  /)\\    '  *\n");
    printf("  '..'  ':::'     * /\\ *     .'/.\\'.   '\n");
    printf("      *            *..*         :\n");
    printf("       *\n");
    printf("       *\n");

}

void intro() {
    printf("  ____          ___  __  __  ____ _____ \n");
    printf(" |  _ \\   /\\   / _ \\|  \\/  |/ __ \\_   _|\n");
    printf(" | |_) | /  \\ | (_) | \\  / | |  | || |  \n");
    printf(" |  _ < / /\\ \\ > _ <| |\\/| | |  | || |  \n");
    printf(" | |_) / ____ \\ (_) | |  | | |__| || |_ \n");
    printf(" |____/_/    \\_\\___/|_|  |_|\\____/_____|\n");

}

int main() {
    int userChoice, userSubChoice, flag = TRUE, subFlag = TRUE;

    intro();
    printf("\n===[VATHMOLOGIO FOITHTWN]===\n");

    printf("Epiloges fortwshs arxeioy:\n");
    printf("  (1) gia domh DDA\n  (2) gia domh Hashing me alysides\n");
    printf("Epilekse: ");
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        printf("Pata (1) gia fortwsh me vash AM\n(2) me vash thn vathmologia tou\n");
        scanf("%d", &userSubChoice);
        if (userSubChoice == 1)
            userMenuA();
        else if (userSubChoice == 2)
            userMenuB();
    } else if (userChoice == 2) {
        userMenuC();
    }

    fireworks();
}
