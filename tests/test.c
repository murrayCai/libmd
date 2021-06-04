#include "md.h"
#define FILE_PATH_MAX 256

const char *testMdFileName = "test.md";

char fp[FILE_PATH_MAX] = {0};
md_t *md = NULL;

int get_test_md_path(){
    int ret = 0;

    char *currFilePath = __FILE__;
    size_t currFilePathSize = strlen(currFilePath);
    int i = currFilePathSize - 1;
    size_t dirPathSize = 0;
    for(; i >= 0; i--){
        if('/' == currFilePath[i]){
            dirPathSize = i;
            break;
        }
    }
    memset(fp,0,FILE_PATH_MAX);
    R(dirPathSize >= FILE_PATH_MAX - 1);
    snprintf(fp,dirPathSize + 2,"%s/",currFilePath);
    snprintf(fp + dirPathSize + 1, FILE_PATH_MAX - dirPathSize - 1, "%s",testMdFileName);
    return ret;
}

int main(int argc,char *argv[]){
    int ret = 0;
    if(argc > 1){
        testMdFileName = argv[1];
    }
    RL(get_test_md_path(),"get_test_md_path() failed!\n");
    printf("test.md file path : %s\n",fp);
    RL(md_init(&md),"md_init() failed!\n");

    md_result_t *result = NULL;
    RL(md_result_init(&result),"md_result_init() failed!\n");
    RL(md_parse_file(md,result,fp),"md_parse_file() failed!\n");
    RL(md_result_print(result),"md_result_print() failed!\n");

    RL(md_result_free(&result),"md_result_free() failed!\n");
    RL(md_free(&md),"md_free() failed!");

    mem_show();

    return ret;
}
