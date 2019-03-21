/*

Янович Александр
дз 7

Реализован алгоритм Дейкстры.
Ожидаетсяя два параметра:
1. Имя файла с матрицей смежности. В первой строке число точек ("городов").
   Далее построчно сама матрица. Используется только верхняя половина (выше диагонали).
2. Номер точки, для которой применяется алгоритм. Точки считаются пронумерованными
   начиная с 1.

Выводятся кратчайшие расстояния и маршруты для всех точек.

Расчет примера из методички: main_7.exe matrix1.txt 1
Расчет примера с вебинара: main_7.exe matrix2.txt 7

*/

#include "util.h"

typedef struct {
    int *m;
    int size;
    int tmp;
} Matrix;

typedef int T;
typedef struct ItemStruct {
    T data;
    struct ItemStruct *prev;
    struct ItemStruct *next;
} Item;
typedef struct {
    Item *first;
    Item *last;
} Queue;

int getWeight(Matrix *m, int i, int j);
void setWeight(Matrix *m, int i, int j, int w);
int readMatrix(Matrix *m, const char *filename);
void printMatrix(Matrix *m);
void runDeikstra(Matrix *m, int point0, int *ww);
void printDeikstra(Matrix *m, int point0, int *ww, int p);
void queueInit(Queue *q);
void queuePush(Queue *q, T data);
T queuePop(Queue *q);
int queueIsEmpty(Queue *q);

int main(int argc, const char *argv[]) {
    if(argc != 3) {
        printf("usage:\nmain_7.exe matrix_filename town_number\n");
        return 1;
    }

    Matrix m;
    if(!readMatrix(&m, argv[1]))
        return 1;

    printMatrix(&m);

    int *ww = calloc((ull)m.size, sizeof (int));
    int point0 = 0;
    sscanf(argv[2], "%d", &point0);

    if(point0 <= 0) {
        printf("got invalid town number: %d\n", point0);
        return 1;
    }

    runDeikstra(&m, point0, ww);

    for(int p=1; p<=m.size; p++)
        if(p != point0)
            printDeikstra(&m, point0, ww, p);

    return 0;
}

int getWeight(Matrix *m, int i, int j) {
    if(i < 0 || i >= m->size || j < 0 || j >= m->size || i == j)
        return 0;
    return m->m[i * m->size + j];
}

void setWeight(Matrix *m, int i, int j, int w) {
    if(i >= 0 && i < m->size && j >= 0 && j < m->size && i != j) {
        m->m[i * m->size + j] = w;
        m->m[j * m->size + i] = w;
    }
}

int readMatrix(Matrix *m, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(f == NULL) {
        printf("can't open file %s\n", filename);
        return 0;
    }

    m->size = 0;
    fscanf(f, "%d", &m->size);
    if(m->size == 0) {
        fclose(f);
        printf("invalid matrix size\n");
        return 0;
    }

    m->m = calloc((ull)(m->size * m->size), sizeof (int));

    int tmp;
    for(int i=0; i<m->size - 1; i++) {
        for(int j=0; j<m->size; j++){
            fscanf(f, "%d", &tmp);
            if(j > i)
                setWeight(m, i, j, tmp);
        }
    }

    fclose(f);
    return 1;
}

void printMatrix(Matrix *m) {
    printf("\nmatrix size: %d\n", m->size);
    for(int i=0; i<m->size; i++) {
        for(int j=0; j<m->size; j++){
            printf("%3d ", getWeight(m, i, j));
        }
        printf("\n");
    }
}

void queueInit(Queue *q) {
    q->first = NULL;
    q->last = NULL;
}

void queuePush(Queue *q, T data) {
    Item *item = calloc(1, sizeof (Item));
    item->data = data;
    item->prev = NULL;
    item->next = q->first;
    q->first = item;
    if(item->next != NULL)
        item->next->prev = item;
    if(q->last == NULL)
        q->last = item;
}

T queuePop(Queue *q) {
    if(q->last == NULL)
        return -1;

    T ret = q->last->data;
    Item *last = q->last;
    q->last = last->prev;
    free(last);

    if(q->last == NULL)
        q->first = NULL;
    else
        q->last->next = NULL;

    return ret;
}

int queueIsEmpty(Queue *q) {
    return q->last == NULL ? 1 : 0;
}

void runDeikstra(Matrix *m, int point0, int *ww) {
    Queue q;
    queueInit(&q);

    queuePush(&q, point0 - 1);

    int w, c, wc;
    while(!queueIsEmpty(&q)) {
        c = queuePop(&q);
        wc = ww[c];

        for(int i=0; i<m->size; i++) {
            if(i != point0 - 1) {
                w = getWeight(m, i, c);
                if(w > 0) {
                    if(ww[i] == 0 || ww[i] > (wc + w)) {
                        ww[i] = wc + w;
                        queuePush(&q, i);
                    }
                }
            }
        }
    }
}

void printDeikstra(Matrix *m, int point0, int *ww, int p) {
    printf("dist %d to %d is %d: ", p, point0, ww[p - 1]);
    int c = p - 1;
    int found, w;

    while(1) {
        printf("%d - ", c+1);

        found = 0;
        for(int i=0; i<m->size; i++) {
            w = getWeight(m, c, i);
            if(w > 0 && ww[i] + w == ww[c]) {
                found = 1;
                c = i;
                break;
            }
        }

        if(found && c == point0 - 1) {
            printf("%d\n", point0);
            return;
        }

        if(!found) {
            printf("no path\n");
            return;
        }
    }
}
