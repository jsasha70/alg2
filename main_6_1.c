/*

Янович Александр
дз 6 задача 1

1. Реализовать простейшую хэш-функцию. На вход функции подается строка, на выходе сумма кодов символов.
(я сделал чуть более сложную функцию - придумал сам)

*/

#include "util.h"

int main() {
    char s[1000];
    int hash;
    int tmp;
    int i;
    unsigned char c;

    while (1) {
        enterString("enter string (0 to exit)", s, 999);

        if(strcmp(s, "0") == 0) break;
        hash = 0;
        i = 0;

        while(s[i]) {
            c = (unsigned char) s[i++];
            hash <<= (c & 3);
            hash += c;
            tmp = hash & 0xff0000;
            tmp >>= 16;
            hash = (hash & 0xffff) ^ tmp;
        }
        printf("hash: %04x\n", hash);
    }

    return 0;
}
