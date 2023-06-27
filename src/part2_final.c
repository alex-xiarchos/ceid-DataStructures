#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUCKETS 11  //μεταβλητή για hashing
#define TRUE 1  //το 1 είναι μπανάλ
#define FALSE 0 //και το 0.

struct hash *hashTable = NULL;  //μεταβλητή για hashing

typedef struct student  //struct εγγραφής φοιτητή
{
    char AM[10];
    char name[20];
    char surname[20];
    char grade[10];
    struct student *sameGrade;  //χρησιμοποιείται στο Β ερώτημα
    struct student *left;
    struct student *right;
    struct student *next; //χρησιμοποιείται στο Γ ερώτημα
} Student, *Studentptr;

struct hash { //struct hash
    Student *head;
    int count;
};

Studentptr newStudent(char *AM, char *name, char *surname, char *grade)    //εισαγωγή νέας εγγραφής
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

void inorder(Studentptr root) { //διάβασμα αριστερά, ρίζα, δεξιά
    if (root != NULL) {
        inorder(root->left);
        printf("%s %s %s %s\n", root->AM, root->name, root->surname, root->grade);
        inorder(root->right);
    }
}

int stringToKey(char *AM) { //δημιουργεί τα hashes
    int i, sum = 0;
    for (i = 0; i < strlen(AM); i++) {
        sum += AM[i];
    }
    return sum % BUCKETS;
}

int stringCompare(char string1[], char string2[]) { //ναι υπάρχει ήδη στην C, αλλά δεν την κάναμε και τζάμπα
    //επιστρέφει 1 αν string1 < string2, -1 για > , 0 για ισα
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
    /* Αν το δέντρο ειναι άδειο, επέστρεψε μια νέα εγγραφή */
    if (root == NULL)
        return student;

    /* Αλλιώς, αναδρομή στο δένδρο */
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
    Studentptr *array = (Studentptr *) malloc(20 * sizeof(Studentptr));  //γραμμές

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

    if (stringCompare(student->AM, AM) == 1)  //ο φοιτητής είναι δεξιά
        return searchStudent(student->right, AM);

    return searchStudent(student->left, AM);  //αλλιώς είναι αριστερά
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
    //αναζήτηση φοιτητή
    if (student == NULL)
        return NULL;
    if (stringCompare(chosenAM, student->AM) == -1)
        student->right = deleteStudent(student->right, chosenAM);
    else if (stringCompare(chosenAM, student->AM) == 1)
        student->left = deleteStudent(student->left, chosenAM);
    else {
        //κανένα παιδί
        if (student->left == NULL && student->right == NULL) {
            free(student);
            return NULL;
        }
            //ένα παιδί
        else if (student->left == NULL || student->right == NULL) {
            Studentptr tempStudent;
            if (student->left == NULL)
                tempStudent = student->right;
            else
                tempStudent = student->left;
            free(student);
            return tempStudent;
        }
            //δύο παιδιά
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
    // νέο node στην λίστα
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
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
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
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
}


void deleteFromHash(char AM[]) {
    Studentptr temp, myNode;
    int hashIndex = stringToKey(AM);
    int flag = 0;
    myNode = hashTable[hashIndex].head;
    if (!myNode) {
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
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
        printf("\nΟ φοιτητής διαγράφτηκε.\n");
    else
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
}

void editInHash(int choice, char AM[], char newstring[]) {
    Studentptr editNode;
    int hashIndex = stringToKey(AM);
    int flag = 0;
    editNode = hashTable[hashIndex].head;
    if (!editNode) {
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
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
        printf("\nΔεν βρέθηκε τέτοια εγγραφή\n");
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
            printf("%s %s %s %s", myNode->AM, myNode->name, myNode->surname, myNode->grade);
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

    //ΑΡΧΙΚΟΠΟΙΗΣΗ ΔΕΝΤΡΟΥ
    root = insertByAM(root, students[0]);
    for (j = 1; j < 19; j++) {
        root = insertByAM(root, students[j]);
    }

    printf("\n===[ΔΔΑ ΜΕ ΒΑΣΗ ΑΜ]===\n");
    while (strlen("Tsipras") > strlen("Koulis")) { //παντα true ειναι, χεχε
        printf("\n\nΤι γουστάρεις;\n");
        printf("(1) Απεικόνιση με ενδο-διατεταγμένη διάσχιση\n");
        printf("(2) Αναζήτηση βάσει του ΑΜ\n");
        printf("(3) Τροποποίηση τιμής βάσει του ΑΜ\n");
        printf("(4) Διαγραφή βάσει του ΑΜ\n");
        printf("(5) Έξοδος\n");

        scanf("%d", &userChoice);

        if (userChoice == 1)
            inorder(root);
        else if (userChoice == 2) {
            printf("Δώσε ΑΜ για αναζήτηση: ");
            scanf("%s", chosenAM);
            tempStudent = searchStudent(root, chosenAM);
            printf("%s %s %s %s", tempStudent->AM, tempStudent->name, tempStudent->surname, tempStudent->grade);
        } else if (userChoice == 3) {
            while (flag == TRUE) {
                printf("Δώσε ΑΜ για τροποποίηση πληροφοριών: ");
                scanf("%s", chosenAM);
                subflag = TRUE;
                while (subflag == TRUE) {
                    printf("Μπορείς να αλλάξεις οποιαδήποτε τιμή πέρα από το ΑΜ:\n");
                    printf("(1) Αλλαγή ονόματος\n(2) Αλλαγή επωνύμου\n(3) Αλλαγή βαθμού\n");
                    printf("Τι θέλεις να αλλάξεις; ");
                    scanf("%d", &userSubChoice);
                    printf("Δώσε καινούρια τιμή για την επιλογή σου: ");
                    scanf("%s", newString);
                    tempStudent = editStudent(root, userSubChoice, chosenAM, newString);
                    printf("Καινούριες τιμές:\n");
                    printf("%s %s %s %s\n", tempStudent->AM, tempStudent->name, tempStudent->surname,
                           tempStudent->grade);
                    printf("\nΓράψε (1) αν επιθυμείς και άλλη αλλαγή στο ιδιο άτομο,\n"
                                   "(2) για να αλλάξεις ΑΜ και \n(0) για επιστροφή στο μενού: ");
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
            printf("Δώσε AM για διαγραφή: ");
            scanf("%s", chosenAM);
            root = deleteStudent(root, chosenAM);
            printf("Ο φοιτητής διαγράφτηκε.");
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


    printf("\n===[ΔΔΑ ΜΕ ΒΑΣΗ ΒΑΘΜΟΛΟΓΙΑ]===\n");
    while (strlen("Tsipras") > strlen("Koulis")) { //παντα true ειναι, χεχε
        printf("\n\nΤι γουστάρεις;\n");
        printf("(1) Εύρεση των φοιτητών με την ΧΕΙΡΟΤΕΡΗ βαθμολογία\n");
        printf("(2) Εύρεση των φοιτητών με την ΚΑΛΥΤΕΡΗ βαθμολογία\n");
        printf("(3) Έξοδος\n");

        scanf("%d", &userChoice);

        if (userChoice == 1) {
            printf("Μαθητές με την χειρότερη βαθμολογία:\n");
            tempStudent = root;
            while (tempStudent != NULL) {
                tempStudent = findMinGrades(tempStudent);
                if (tempStudent != NULL)
                    printf("%s %s %s %s\n", tempStudent->AM, tempStudent->name, tempStudent->surname,
                           tempStudent->grade);
            }
        } else if (userChoice == 2) {
            printf("Μαθητές με την καλύτερη βαθμολογία:\n");
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
        printf("\n\nΤι γουστάρεις;\n");
        printf("(1) Προβολή του πίνακα κατακερματισμού\n");
        printf("(2) Αναζήτηση βάσει του ΑΜ\n");
        printf("(3) Τροποποίηση βάσει του ΑΜ\n");
        printf("(4) Διαγραφή βάσει του ΑΜ\n");
        printf("(5) Έξοδος\n");

        scanf("%d", &choice);

        if (choice == 2) {
            printf("Δώσε ΑΜ για αναζήτηση: ");
            scanf("%s", chosenAM);
            searchInHash(chosenAM);
        } else if (choice == 3) {
            while (flag == TRUE) {
                printf("Δώσε ΑΜ για τροποποίηση στοιχείων: ");
                scanf("%s", chosenAM);
                subflag = TRUE;
                while (subflag == TRUE) {
                    printf("Μπορείς να αλλάξεις οποιαδήποτε τιμή πέρα από το ΑΜ:\n");
                    printf("(1) Αλλαγή ονόματος\n(2) Αλλαγή επωνύμου\n(3) Αλλαγή βαθμού\n");
                    printf("Τι θέλεις να αλλάξεις; ");
                    scanf("%d", &subchoice);
                    printf("Δώσε καινούρια τιμή για την επιλογή σου: ");
                    scanf("%s", newString);
                    editInHash(subchoice, chosenAM, newString);
                    printf("\nΓράψε (1) αν επιθυμείς και άλλη αλλαγή για το ίδιο άτομο,\n"
                                   "(2) για να αλλάξεις ΑΜ \n(0) για επιστροφή στο μενού: ");
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
            printf("Δώσε ΑΜ για διαγραφή: ");
            scanf("%s", chosenAM);
            deleteFromHash(chosenAM);
        } else if (choice == 5)
            break;
        else if (choice == 1)
            display();
        else
            printf("Ουψς έκανες λάθος!\nΕλπίζουμε την επόμενη φορά να ΞΕΡΕΙΣ ΝΑ ΜΕΤΡΑΣ ΩΣ ΤΟ 5.\n");
    }

}

void fireworks() {
    printf("\nΟΥΟΥΟΥΟΥΟΥΟΥΟΥΟΥ!\n");
    printf("                                   .''.       \n");
    printf("       .''.      .        *''*    :_\\/_:     . \n");
    printf("      :_\\/_:   _\\(/_  .:.*_\\/_*   : /\\ :  .'.:.'.\n");
    printf("  .''.: /\\ :   ./)\\   ':'* /\\* :  '..'.  -=:o:=-\n");
    printf(" :_\\/_:'.:::.    ' *''*    * '.\\'/.' _\\(/_'.':'.'\n");
    printf(" : /\\ : :::::     *_\\/_*     -= o =-  /)\\    '  *\n");
    printf("  '..'  ':::'     * /\\ *     .'/.\\'.   '\n");
    printf("      *            *..*         :\n");
    printf("       *\n");
    printf("       *\n");

}

void intro() {
    printf("██████╗  █████╗  █████╗ ███╗   ███╗ ██████╗ ██╗\n");
    printf("██╔══██╗██╔══██╗██╔══██╗████╗ ████║██╔═══██╗██║\n");
    printf("██████╔╝███████║╚█████╔╝██╔████╔██║██║   ██║██║\n");
    printf("██╔══██╗██╔══██║██╔══██╗██║╚██╔╝██║██║   ██║██║\n");
    printf("██████╔╝██║  ██║╚█████╔╝██║ ╚═╝ ██║╚██████╔╝██║\n");
    printf("╚═════╝ ╚═╝  ╚═╝ ╚════╝ ╚═╝     ╚═╝ ╚═════╝ ╚═╝\n");
}

int main() {
    int userChoice, userSubChoice, flag = TRUE, subFlag = TRUE;

    intro();
    printf("\n===[ΒΑΘΜΟΛΟΓΙΟ ΦΟΙΤΗΤΩΝ]===\n");

    printf("Επιλογές φόρτωσης αρχείου:\n");
    printf("  (1) για δομή ΔΔΑ\n  (2) για δομή Hashing με αλυσίδες\n");
    printf("Επίλεξε: ");
    scanf("%d", &userChoice);

    if (userChoice == 1) {
        printf("Πάτα (1) για φόρτωση με βάση ΑΜ και\n(2) με βάση την βαθμολογία του\n");
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
