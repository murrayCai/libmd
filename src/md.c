#include "inc.h"

#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD
#endif

int md_init(md_t **md){
    int ret = 0;
    R(NULL == md);
    R(NULL != *md);

    R(MALLOC_T(md,md_t));

    return ret;
}

int md_free(md_t **md){
    int ret = 0;
    R(NULL == md);
    R(NULL == *md);
    R(md_clear_templates(*md));
    R(FREE_T(md,md_t));
    return ret;
}
