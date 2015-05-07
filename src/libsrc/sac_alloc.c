#include <stdio.h>
#include <stdlib.h>
#include "dbg.h"
#include "sac.h"
#include "sacutil.h"

/* creat sac array */
sac *sacnewn(int ntr)
{
    int i=0;

    /* allocate space */
    sac *tr=NULL;
    tr = (sac *)calloc(ntr,sizeof(sac));
    check_mem(tr);

    /* initialize */
    for (i=0;i<ntr;i++) {
        tr[i].npts = 0;
        tr[i].iftype = 1;
        tr[i].nvhdr = 6;
    }

    return tr;

error:
    return NULL;
}

/* free sac array */
void sacfreen(sac *tr, int ntr)
{
    int i=0;
    /* free memories */
    if (tr) {
        for (i=0; i<ntr; i++) {
            if(tr[i].data) free(tr[i].data);
        }
        free(tr);
    }
}
