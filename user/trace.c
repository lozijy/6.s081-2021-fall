
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAXARG 50
int
main(int argc, char *argv[])
{
    char *nargv[MAXARG];
    int i;
    if (trace(atoi(argv[1])) < 0) {
    fprintf(2, "%s: trace failed\n", argv[0]);
    exit(1);
    }
    for(i = 2; i < argc && i < MAXARG; i++){
        nargv[i-2] = argv[i];
    }
    exec(nargv[0], nargv);
    exit(0);
}
