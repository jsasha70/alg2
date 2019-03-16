#include "util.h"


int main(int argc, const char *argv[]) {
    char s[1000];
    enterString("enter string", s, 999);
    int hash = 0;
    int tmp;
    int i = 0;
    unsigned char c;
    while(s[i]) {
        c = (unsigned char) s[i++];
        hash <<= (c & 3);
        hash += c;
        tmp = hash & 0xff0000;
        tmp >>= 16;
        hash = (hash & 0xffff) ^ tmp;
    }
    printf("hash: %d\n", hash);

    return 0;
}
