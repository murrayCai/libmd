#include "inc.h"

#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD_TEMPLATE
#endif

#define IS_TEMPLATE_VAR(str,strSize,var,varSize) \
    ({\
        int _it = 0,_is = 1;\
        if( (strSize) > (varSize) ){\
            for(;_it < (varSize); _it++){\
                if( (str)[_it] != var[_it] ){\
                    _is = 0;\
                    break;\
                }else{\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

int md_clear_templates(md_t *md){
    int ret = 0;
    R(NULL == md);
    int i = 0;
    md_template_t *pTemplate = NULL;
    for(; i < MDT_MAX; i++){
       pTemplate = md->templates + i;
       if( NULL != pTemplate && NULL != pTemplate->data && 0 < pTemplate->bufSize){
           R(FREE_TN(&(pTemplate->data),str_t,pTemplate->bufSize));
           R(FREE_TN(&(pTemplate->filePath),str_t,strlen(pTemplate->filePath) + 1 ));
       }
    }
    memset(md->templates,0,sizeof(md_template_t) * MDT_MAX);
    return ret;
}

int md_load_templates(md_t *md,const char *dirTemplate,const char *templateVarName){
    int ret = 0,errRet = 0;
    R(NULL == md);
    R(NULL == dirTemplate);
    R(NULL == templateVarName);
    R(access(dirTemplate,F_OK|R_OK));
    md->dirTemplate = dirTemplate;
    md->templateVarName = templateVarName;
    
    int i = 0;
    char fp[FILE_PATH_MAX] =  {0};
    FILE *f = NULL;
    char *buf = NULL;
    unsigned int templateVarNameLen = strlen(md->templateVarName);
    const char *templateName = NULL;
    char *tempFilePath = NULL;
    long size = 0;
    size_t readSize = 0;
    unsigned int isFound = 0;
    unsigned int bufSize = 0;

    for(; i < MDT_MAX; i++){
        f = NULL;
        size = 0;
        buf = NULL;
        readSize = 0;
        bufSize = 0;
        isFound = 0;
        tempFilePath = NULL;

        memset(fp,0,FILE_PATH_MAX); 
        templateName = MD_TYPE_NAME(i);
        snprintf(fp,FILE_PATH_MAX,"%s/%s%s",md->dirTemplate,templateName,MD_TEMPLATE_FILE_EXT);
        G_E(access(fp,F_OK|R_OK));
        G_E(NULL == (f = fopen(fp,"r")) );
        G_E(fseek(f,0,SEEK_END));
        G_E( -1 == (size = ftell(f)) );
        G_E(templateVarNameLen >= size);
        G_E(fseek(f,0,SEEK_SET));
        bufSize = size + 16;
        G_E(MALLOC_TN(&buf,str_t,bufSize));
        readSize = fread(buf,1,size,f);
        G_E(ferror(f));
        G_E(size != readSize);
        int j = 0;
        for(; j < size; j++){
            if( IS_TEMPLATE_VAR( buf + j, size - j, md->templateVarName, templateVarNameLen ) ){
                G_E(MALLOC_TN(&tempFilePath,str_t,strlen(fp) + 1));
                memcpy(tempFilePath,fp,strlen(fp));
                md->templates[i].data = buf;
                md->templates[i].filePath = tempFilePath;
                md->templates[i].dataSize = readSize;
                md->templates[i].bufSize = bufSize;
                md->templates[i].prefixPos = buf;
                md->templates[i].prefixLen = j;
                md->templates[i].suffixPos = buf + j + templateVarNameLen;
                md->templates[i].suffixLen = size - j - templateVarNameLen;
                isFound = 1;
            }else{

            }
        }
        R(fclose(f));
        f = NULL;
        G_E(0 == isFound);
    }
    md->isInitedTemplates = 1;
    return ret;

error:
    errRet = ret;
    LOGI("error file : %s\n",fp);
    if(NULL != f) R(fclose(f));
    if(NULL != buf) R(FREE_TN(&buf,str_t,bufSize));
    R(md_clear_templates(md));
    return errRet;
}

int md_print_templates(md_t *md){
    int ret = 0;
    int i = 0;
    R(NULL == md);
    for(; i < MDT_MAX; i++){
        if(NULL != md->templates[i].data){
            printf("[%d]\tfile : %s\n",i,md->templates[i].filePath);
            printf("data:[%s]\tprefix(%d):[%s]\tsuffix(%d):[%s]\n",
                    md->templates[i].data,
                    md->templates[i].prefixLen,md->templates[i].prefixPos,
                    md->templates[i].suffixLen,md->templates[i].suffixPos);
        }else{
        }
    }
    return ret;
}

