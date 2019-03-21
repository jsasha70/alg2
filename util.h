#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned long long ull;
typedef unsigned char uc;

double enterDouble(char *text) {
    printf("%s: ", text);
    double ret;
    scanf("%lf", &ret);
    return ret;
}

int enterInt(char *text) {
    printf("%s: ", text);
    int ret;
    scanf("%d", &ret);
    return ret;
}

void enterInt2(char *text, int *n, int *m) {
    printf("%s: ", text);
    scanf("%d %d", n, m);
}

void enterString(char *text, char *s, int maxLen) {
    if(text != NULL) printf("%s: ", text);

    int i = 0;
    char c;
    while(1) {
        scanf("%c", &c);
        if(c=='\n') {
            if(i>0) break;
            else continue;
        }
        s[i++] = c;
        if(i >= maxLen) break;
    }
    s[i] = 0;
}

// int main(int argc, const char *argv[])  ;
