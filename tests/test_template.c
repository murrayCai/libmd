#include "md.h"

const char *dirTemplate = NULL;
const char *templateVarName = NULL;

void show_useage(){
    printf("test_template $dirPath $templateVar\n");
}

int main(int argc,char *argv[]){
    int ret = 0;
    if(argc < 3){
        show_useage();
        return 0;
    }
    dirTemplate = argv[1];
    templateVarName = argv[2];
    md_t *md = NULL;
    RL(md_init(&md),"md_init failed!\n");
    RL(md_load_templates(md,dirTemplate,templateVarName),
            "load templates(%s)[%s] failed!\n",dirTemplate,templateVarName);
    RL(md_print_templates(md),"md_print_templates() failed!\n");
    RL(md_free(&md),"md_free() failed!\n");
    mem_show();
    return ret;
}
