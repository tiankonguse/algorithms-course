#include "bookcase_impl2.h"
#include <cstdio>
#include <cstring>
int main() {
    memset(num,0,sizeof(num)); memset(fa,0,sizeof(fa));
    memset(ls,0,sizeof(ls)); memset(rs,0,sizeof(rs));
    memset(siz,0,sizeof(siz)); memset(pos,0,sizeof(pos));
    head=0; cnt=0;
    add(0); add(1); add(2);
    printf("init head=%d sz=%d\n", head, siz[head]);
    int rank = ask(1) + 1;
    printf("ask rank=%d head=%d\n", rank, head);
    printf("calling move(%d,%d)\n", rank, rank);
    move(rank, rank);
    printf("move done head=%d sz=%d\n", head, siz[head]);
    return 0;
}
