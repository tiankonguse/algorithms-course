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
    for (int k = 0; k < 12; k++) {
        printf("k=%d before ask: head=%d sz=%d\n", k, head, siz[head]);
        int rank = ask(1) + 1;
        printf("  rank=%d head=%d\n", rank, head);
        if (k % 2 == 0) move(rank, rank);        // t=0
        else move(rank, 2);                      // Top/Bottom 等价
        printf("  after move head=%d sz=%d\n", head, siz[head]);
    }
    printf("all done\n");
    return 0;
}
