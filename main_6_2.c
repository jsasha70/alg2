/*

Янович Александр
дз 6 задача 2

2. Переписать программу, реализующее двоичное дерево поиска.
а) Добавить в него обход дерева различными способами;
б) Реализовать поиск в двоичном дереве поиска;
в) *Добавить в программу обработку командной строки с помощью которой можно указывать
из какого файла считывать данные, каким образом обходить дерево.

*/

#include "util.h"


typedef int T;

typedef struct NodeStruct {
    struct NodeStruct *left;
    struct NodeStruct *right;
    T data;
    int tmp; // чтобы убрать предупреждение про padding
} Node;

void printUsage(void);
Node* createTree(int argc, const char *argv[]);
int getNumbersFromFile(const char *fileName, int **ar);
int getNumbersFromString(const char *str, int **ar);
Node* getNewNode(T data);
void addNode(Node *tree, Node *node);
void runFind(Node *tree, const char *param);
void runPrint(Node *tree, const char *param);
int find(Node *tree, T data, char *path);
void printTree(Node *tree, const char *typ);

int main(int argc, const char *argv[]) {
    if(argc == 1 || argc < 4 || argc > 5) {
        printUsage();
        return 1;
    }

    Node *tree = createTree(argc, argv);
    if(tree == NULL) return 1;

    const char *cmd = argv[argc - 2];
    const char *param = argv[argc - 1];

    if(strcmp(cmd, "f") == 0) {
        runFind(tree, param);
    } else if(strcmp(cmd, "p") == 0) {
        runPrint(tree, param);
    } else {
        printUsage();
        return 1;
    }

    return 0;
}

void printUsage() {
    printf("usage:\n");
    printf("main_6_2.exe filename command param\n");
    printf("main_6_2.exe s \"numbers\" command param\n");
    printf("\n\"numbers\" - list of numbers separated by spaces, with first number being count of other numbers\n");
    printf("\n  commands:\n");
    printf("    f - find the number in tree (param - number to find)\n");
    printf("    p - print the tree (param - order of print: lkr, klr, ...; k - root, l - left, r - right)\n");
    printf("\nexamples:\n");
    printf("main_6_2.exe data.txt f 123\n");
    printf("main_6_2.exe s \"3 15 123 49\" p lkr\n\n");
}

Node* createTree(int argc, const char *argv[]) {
    int count, *ar;

    if(strcmp(argv[1], "s") == 0) { // список элементов - в строке
        if(argc != 5) {
            printUsage();
            return NULL;
        }
        count = getNumbersFromString(argv[2], &ar);
    } else { // список элементов - в файле
        if(argc != 4) {
            printUsage();
            return NULL;
        }
        count = getNumbersFromFile(argv[1], &ar);
    }

    if(count < 1) {
        return NULL;
    }

    Node *tree = getNewNode(ar[0]);
    for(int i=1; i<count; i++)
        addNode(tree, getNewNode(ar[i]));

    return tree;
}

int getNumbersFromFile(const char *fileName, int **ar) {
    FILE *f = fopen(fileName, "r");
    if(f == NULL) {
        printf("cab't open file %s\n", fileName);
        return 0;
    }

    int count = 0;
    fscanf(f, "%d", &count);

    if(count < 1) {
        fclose(f);
        printf("invalid array length: %d\n", count);
        return 0;
    }

    *ar = calloc((ull)count, sizeof (int));

    for(int i=0; i<count; i++) {
        fscanf(f, "%d", *ar + i);
    }

    fclose(f);
    return count;
}

const char* skipSpaces(const char *str) {
    const char *s = str;
    while(*s == ' ') s++;
    return s;
}

const char* skipNonSpaces(const char *str) {
    const char *s = str;
    while(*s != ' ' && *s != 0) s++;
    return s;
}

int getNumbersFromString(const char *str, int **ar) {
    const char *s = skipSpaces(str);
    if(*s == 0) {
        printf("invalid number list\n");
        return 0;
    }

    int count = 0;
    sscanf(s, "%d", &count);
    if(count < 1) {
        printf("invalid array length: %d\n", count);
        return 0;
    }

    *ar = calloc((ull)count, sizeof (int));

    for(int i=0; i< count; i++) {
        s = skipNonSpaces(s);
        s = skipSpaces(s);
        if(*s == 0) {
            printf("invalid number list\n");
            return 0;
        }
        sscanf(s, "%d", *ar + i);
    }

    return count;
}

Node* getNewNode(T data) {
    Node *ret = (Node*)malloc(sizeof(Node));
    ret->data = data;
    ret->left = NULL;
    ret->right = NULL;
    return ret;
}

void addNode(Node *tree, Node *node) {
    if(node->data > tree->data) {
        if(tree->right) addNode(tree->right, node);
        else tree->right = node;
    } else {
        if(tree->left) addNode(tree->left, node);
        else tree->left = node;
    }
}

void runFind(Node *tree, const char *param) {
    int num = 0;
    sscanf(param, "%d", &num);
    char path[10000];
    path[0] = '.';
    path[1] = 0;
    if(find(tree, num, path+1))
        printf("%d found: %s\n", num, path);
    else
        printf("%d not found (%s)\n", num, path);
}

int find(Node *node, T data, char *path) {
    if(data == node->data)
        return 1;

    if(data > node->data) {
        if(node->right != NULL) {
            path[0] = 'r';
            path[1] = 0;
            return find(node->right, data, path + 1);
        }
    } else {
        if(node->left != NULL) {
            path[0] = 'l';
            path[1] = 0;
            return find(node->left, data, path + 1);
        }
    }
    return 0;
}

void runPrint(Node *tree, const char *param) {
    if(strlen(param) != 3) {
        printUsage();
        return;
    }

    for(int i=0; i<3; i++) {
        if(param[i] != 'k' && param[i] != 'l' && param[i] != 'r') {
            printUsage();
            return;
        }
    }

    if(param[0] == param[1] || param[1] == param[2] || param[2] == param[0]) {
        printUsage();
        return;
    }

    printTree(tree, param);
    printf("\n");
}

void printTree(Node *tree, const char *typ) {
    for(int i=0; i<3; i++) {
        switch (typ[i]) {
        case 'l':
            if(tree->left) printTree(tree->left, typ);
            break;
        case 'r':
            if(tree->right) printTree(tree->right, typ);
            break;
        default:
            printf("%d ", tree->data);
        }
    }
}
