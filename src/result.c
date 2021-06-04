#include "inc.h"

#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD_RESULT
#endif

#define MD_FILE_INIT_LINES_COUNT 32
int md_result_init(md_result_t **result){
    int ret = 0;
    R(NULL == result);
    R(NULL != *result);

    R(MALLOC_T(result,md_result_t));
    RC(arr_init(&((*result)->lines),MD_FILE_INIT_LINES_COUNT),{
            FREE_T(result,md_result_t);
            });
    return ret;
}

int md_result_free(md_result_t **r){
    int ret = 0;

    R(NULL == r);
    R(NULL == *r);
    R(NULL == (*r)->lines);

    int lineCount = (*r)->lines->used;
    int i = 0;
    md_result_line_t *pLine = NULL;

    for(; i < lineCount; i++){
        R(arr_index((void **)&pLine,(*r)->lines,i));
        R(md_result_line_free(&pLine));
    }
    R(arr_free(&((*r)->lines)));
    R(FREE_T(r,md_result_t));
    return ret;
}

int md_result_table_cell_print(md_table_cell_t *cell,unsigned int index){
    int ret = 0;
    R(NULL == cell);
    printf("\t\t[cell][%u][%u:%s]\t[%s](%u)\n",index,
            cell->type,MD_TABLE_CELL_NAME(cell->type),
            cell->pos,cell->len);
    return ret;
}

int md_result_content_print(md_result_content_t *content){
    int ret = 0;
    R(NULL == content);
    md_table_cell_t *cell = NULL;
    printf("\t[content][%u](%s)\t",content->type,MD_TYPE_NAME(content->type));
    printf("prefix[%s](%u)\tcontent[%s](%u)\tsuffix[%s](%u)\n",
            content->prefixPos,content->prefixLen,
            content->contentPos,content->contentLen,
            content->suffixPos,content->suffixLen);

    if(MDT_CONTENT_LINK_IMAGE == content->type){
        printf("description[%s](%u)\turl[%s](%u)\ttitle[%s](%u)\n",
                content->linkDescriptionPos,content->linkDescriptionLen,content->linkUrlPos,content->linkUrlLen,
                content->linkTitlePos,content->linkTitleLen);
    }else if(MDT_CONTENT_TABLE_HEADER == content->type || 
            MDT_CONTENT_TABLE_ROW == content->type || 
            MDT_CONTENT_TABLE_DEFINE == content->type){
        int i = 0;
        for(; i < content->cellsUsed; i++){
            R(arr_index((void **)(&cell),content->cells,i));
            R(md_result_table_cell_print(cell,i));
        }

    }else{

    }
    return 0;
}

int md_result_line_print(md_result_line_t *rl){
    int ret = 0;
    R(NULL == rl);
    int isNlChar2ZeroChar = 0;
    char *tmpC = (char *)(rl->data);
    if(CHAR_NL == tmpC[rl->dataSize - 1]){
        tmpC[rl->dataSize - 1] = CHAR_ZERO;
        isNlChar2ZeroChar = 1;
    }

    printf("[line][%u]:\ttype[%d](%s)\tdata[%s]\tsize[%u]\tprefix[%s][%u]\tcontents[%u]\tcodeName[%s]\n",
            rl->lineNo,rl->type,MD_TYPE_NAME(rl->type),rl->data,rl->dataSize,
            rl->prefixPos,rl->prefixLen,rl->contents->used,rl->lineCodeNamePos);
    int i = 0;
    md_result_content_t *content = NULL;
    for(; i < rl->contentsUsed; i++){
        R(arr_index((void **)(&content),rl->contents,i));
        R(md_result_content_print(content));
    }
    printf("\n");

    if(isNlChar2ZeroChar){
        tmpC[rl->dataSize - 1] = CHAR_NL;
    }

    return ret;
}

int md_result_print(md_result_t *result){
    int ret = 0;
    R(NULL == result);
    printf("matching[%d]\n",result->matchingIndex);

    int i = 0;
    md_result_line_t *rl = NULL;
    for(; i < result->lines->used; i++){
        R(arr_index((void **)(&rl),result->lines,i));
        R(md_result_line_print(rl));
    }

    return ret;
}

int md_result_content_free(md_result_content_t **content){
    int ret = 0;
    R(NULL == content);
    R(NULL == (*content));
    if( NULL != (*content)->cells ){
        R(ARR_FREE_WITH_DATA( &((*content)->cells), md_table_cell_t ));
    }
    R(FREE_T(content,md_result_content_t));
    return ret;
}

int md_result_content_init(md_result_content_t **content){
    int ret = 0;
    R(NULL == content);
    R(NULL != (*content));
    R(MALLOC_T(content,md_result_content_t));
    R(ARR_INIT_WITH_DATA(&((*content)->cells),MD_TABLE_CELLS_INIT_COUNT,md_table_cell_t));
    return ret;
}

int md_result_contents_free(arr_t **contents){
    int ret = 0;
    R(NULL == contents);
    R(NULL == (*contents));

    int contentCount = (*contents)->used;
    int i = 0;
    md_result_content_t *pContent = NULL;

    for(; i < contentCount; i++){
        R(arr_index((void **)&pContent,(*contents),i));
        if( NULL != pContent->cells ){
            R(ARR_FREE_WITH_DATA( &(pContent->cells), md_table_cell_t ));
        }else{
            R(1);
        }
    }

    R(ARR_FREE_WITH_DATA( contents , md_result_content_t ));

    return ret;
}

int md_result_contents_init(arr_t **contents){
    int ret = 0;
    int j = 0;
    md_result_content_t *content = NULL;
    R(NULL == contents);
    R(NULL != (*contents));
    R(ARR_INIT_WITH_DATA(contents,MD_FILE_LINE_INIT_CONTENTS_COUNT,md_result_content_t));
    int i = 0;
    for(; i < MD_FILE_LINE_INIT_CONTENTS_COUNT; i++){
        G_E(arr_index((void **)&content,(*contents),i));
        G_E(ARR_INIT_WITH_DATA(&(content->cells),MD_TABLE_CELLS_INIT_COUNT,md_table_cell_t));
    }
    return ret;

error:
    if(NULL != (*contents)){
        R(arr_index((void **)&content,(*contents),0));
        FREE_TN(&content,md_result_content_t,MD_FILE_LINE_INIT_CONTENTS_COUNT);
        arr_free(contents);
    }
    return ret;
}

int md_table_cells_get_unused(md_table_cell_t **dist,md_result_content_t *c){
    int ret = 0;
    R(NULL == dist);
    R(NULL != (*dist));
    R(NULL == c);
    R(NULL == c->cells);
    if(c->cellsUsed >= c->cells->used){
        R(ARR_ADD_WITH_DATA(dist,c->cells,md_table_cell_t));
    }else{
        R(arr_index((void **)dist,c->cells,c->cellsUsed));
    }
    return ret;
}

int md_result_contents_get_unused(md_result_content_t **dist,md_result_line_t *rl){
    int ret = 0;
    R(NULL == dist);
    R(NULL != (*dist));
    R(NULL == rl);
    R(NULL == rl->contents);
    if(rl->contentsUsed >= rl->contents->used){
        R(ARR_ADD_WITH_DATA(dist,rl->contents,md_result_content_t));
    }else{
        R(arr_index((void **)dist,rl->contents,rl->contentsUsed));
    }
    return ret;
}

int md_result_line_init(md_result_line_t **rl){
    int ret = 0;
    R(NULL == rl);
    R(NULL != *rl);

    R(MALLOC_T(rl,md_result_line_t));
    RC(md_result_contents_init(&((*rl)->contents)),{
            FREE_T(rl,md_result_line_t);
            });
    return ret;
}

int md_result_line_free(md_result_line_t **rl){
    int ret = 0;
    R(NULL == rl);
    R(NULL == *rl);
    R(NULL == (*rl)->contents);

    R(md_result_contents_free( &((*rl)->contents) ));
    if(NULL != (*rl)->data){
        R(FREE_TN(&((*rl)->data),str_t,((*rl)->bufSize)));
    }
    R(FREE_T(rl,md_result_line_t));
    return ret;
}
