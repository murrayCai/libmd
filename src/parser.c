#include "inc.h"

#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD_PARSER
#endif

static int get_line_type(md_result_line_t *rl){
    int ret = 0;
    R(NULL == rl);
    R(NULL == rl->data);

    rl->type = MDT_NONE;
    const char *str = rl->prefixPos;
    char c = str[0];
    unsigned int size = rl->dataSize - (rl->prefixPos - rl->data);
    unsigned int prefixSize = 0;

    if('#' == c){
        if(IS_LINE_TITLE_1(str,size)){
            rl->type = MDT_LINE_TITLE_1;
            rl->prefixLen = 2;
        }else if( IS_LINE_TITLE_2(str,size) ){
            rl->type = MDT_LINE_TITLE_2;
            rl->prefixLen = 3;
        }else if( IS_LINE_TITLE_3(str,size)  ){
            rl->type = MDT_LINE_TITLE_3;
            rl->prefixLen = 4;
        }else if( IS_LINE_TITLE_4(str,size)  ){
            rl->type = MDT_LINE_TITLE_4;
            rl->prefixLen = 5;
        }else if( IS_LINE_TITLE_5(str,size) ){
            rl->type = MDT_LINE_TITLE_5;
            rl->prefixLen = 6;
        }else if( IS_LINE_TITLE_6(str,size)  ){
            rl->type = MDT_LINE_TITLE_6;
            rl->prefixLen = 7;
        }else{

        }
    }else if(':' == c){
        if ( IS_LINE_TABLE_DEFINE(str,size)  ){
            rl->type = MDT_LINE_TABLE_DEFINE;
            rl->prefixLen = 0;
        }else{

        }
    }else if('+' == c){
        if( IS_LINE_UN_SORT_LIST(str,size)  ){
            rl->type = MDT_LINE_LIST_UN_SORT;
            rl->prefixLen = 2;
        }else{

        }
    }else if('-' == c){

        if( IS_LINE_SPLIT(str,size) ){
            rl->type = MDT_LINE_SPLIT;
            rl->prefixLen = size;
        }else if( IS_LINE_UN_SORT_LIST(str,size)  ){
            rl->type = MDT_LINE_LIST_UN_SORT;
            rl->prefixLen = 2;
        }else if ( IS_LINE_TABLE_DEFINE(str,size)  ){
            rl->type = MDT_LINE_TABLE_DEFINE;
            rl->prefixLen = 0;
        }else{

        }
    }else if('*' == c){
        if( IS_LINE_SPLIT(str,size) ){
            rl->type = MDT_LINE_SPLIT;
            rl->prefixLen = size;
        }else if( IS_LINE_UN_SORT_LIST(str,size)  ){
            rl->type = MDT_LINE_LIST_UN_SORT;
            rl->prefixLen = 2;
        }else{


        }
    }else if('_' == c){
        if( IS_LINE_SPLIT(str,size) ){
            rl->type = MDT_LINE_SPLIT;
            rl->prefixLen = size;
        }else{

        }
    }else if('`' == c){
        if( IS_LINE_CODE_TAG(str,size) ){
            if( IS_LINE_CODE_SUFFIX(str,size) ){
                rl->type = MDT_LINE_CODE_SUFFIX;
                rl->prefixLen = 3;
            }else if( IS_LINE_CODE_PREFIX(str,size) ){
                rl->type = MDT_LINE_CODE_PREFIX;
                rl->prefixLen = 3;
            }else{
            }
        }else{

        }
    }else if('>' == c){
        int refLv = GET_LINE_REF_LV(str,size,&prefixSize);
        if(0 < refLv){
            rl->type = MDT_LINE_REF_1 + refLv - 1;
            rl->prefixLen = prefixSize;
        }else{

        }
    }else if(IS_NUM(c)){
        if(IS_LINE_SORTED_LIST(str,size,&prefixSize)){
            rl->type = MDT_LINE_LIST_SORTED;
            rl->prefixLen = prefixSize;
        }else{

        }
    }else{

    }
    return ret;
}

static int parse_line_table_define(md_result_line_t *rl){
    int ret = 0;
    int retVal = ret;
    R(NULL == rl);
    R(NULL == rl->result);
    R(NULL == rl->data);
    R(NULL == rl->contents);

    const char *data = rl->prefixPos + rl->prefixLen;
    unsigned int dataLen = rl->dataSize - (rl->prefixPos - rl->data) - rl->prefixLen;
    R(0 > dataLen);

    unsigned int contentIsUsed = 0;
    md_result_t *r = rl->result;
    md_result_content_t *content = NULL;
    R(md_result_contents_get_unused(&content,rl));
    int i = 0;

    char c = CHAR_ZERO,prefixC = CHAR_ZERO;
    md_table_cell_type_e cellType = MDTCT_LEFT;
    int isSlashStart = 0;
    int isAlignLeft = 0;
    const char *cellStart = data;
    md_table_cell_t *cellTmp;

    for(; i < dataLen; i++){
        c = data[i];
        R(!( '-' == c || ':' == c || '|' == c || isspace(c) || CHAR_CR == c || CHAR_NL == c));

        if('|' == c || CHAR_NL == c){
            cellTmp = NULL;
            R(md_table_cells_get_unused(&cellTmp,content));
            cellTmp->type = cellType;
            cellTmp->pos = cellStart;
            cellTmp->len = data + i - cellStart;
            content->cellsUsed++;

            cellType = MDTCT_LEFT;
            isSlashStart = 0;
            isAlignLeft = 0;
            cellStart = data + i + 1;

            contentIsUsed = 1;

        }else if(':' == c){
            if(isSlashStart){
                if(isAlignLeft){
                    cellType = MDTCT_MIDDLE;
                }else{
                    cellType = MDTCT_RIGHT;
                }
            }else{
                isAlignLeft = 1;
            }

        }else if('-' == c){
            isSlashStart = 1;
        }else{

        }
    }
    if(contentIsUsed){
        rl->contentsUsed++;
        content->type = MDT_CONTENT_TABLE_DEFINE;
    }else{
        R(1);
    }
    return ret;
}

static int parse_line_table_contents(md_result_line_t *rl,md_result_line_t *rlTableDefine,int isMatchingHeader){
    int ret = 0;
    int j = 0,errVal = ret;
    R(NULL == rl);
    R(NULL == rl->data);
    R(0 >= rl->dataSize);
    R(NULL == rl->contents);
    if(!isMatchingHeader){
        R(0 != rl->contentsUsed);
    }

    R(NULL == rlTableDefine);
    R(0 >= rlTableDefine->dataSize);
    R(NULL == rlTableDefine->contents);
    R(1 != rlTableDefine->contentsUsed);

    const char *data = rl->prefixPos + rl->prefixLen;
    unsigned int dataLen = rl->dataSize - (rl->prefixPos - rl->data) - rl->prefixLen;
    R(0 >= dataLen);

    md_result_content_t *content = NULL;
    if(isMatchingHeader){
        R(md_result_content_init(&content));
    }else{
        R(ARR_FIRST(&content,rl->contents));
    }

    md_result_content_t *contentDefine = NULL;
    R(ARR_FIRST(&contentDefine,rlTableDefine->contents));
    R(NULL == contentDefine);
    R(NULL == contentDefine->cells);
    R(0 >= contentDefine->cellsUsed);

    unsigned int tableCellsCount = contentDefine->cellsUsed;
    arr_t *cellStartPos = NULL;
    R(arr_init(&cellStartPos,tableCellsCount));

    int i =0;
    char c = CHAR_ZERO, prefixC = CHAR_ZERO;
    const char *lastStartPos = data, *currStartPos = NULL, *nextStartPos = NULL;
    unsigned int cellIndex = 0;
    unsigned int rlCellsCount = 0;

    for(; i < dataLen; i++){
        c = data[i];
        if(CHAR_SLASH == prefixC){
            prefixC = c;
        }else{
            if('|' == c){
                G_E(arr_add(cellStartPos,(void *)lastStartPos));
                lastStartPos = data + i + 1;
            }else if ( CHAR_NL == c ){
                G_E(arr_add(cellStartPos,(void *)lastStartPos));
            }else{

            }
        }
    }
    if(isMatchingHeader){
        G_E(cellStartPos->used != tableCellsCount);
    }
    
    int k = 0;
    md_table_cell_t *cellTmp = NULL,*cellTmpDefine = NULL;
    for(; k < cellStartPos->used; k++){
        currStartPos = NULL;
        nextStartPos = NULL;
        cellTmpDefine = NULL;
        cellTmp = NULL;

        G_E(md_table_cells_get_unused(&cellTmp,content));
        if(k >= tableCellsCount){
            cellTmp->type = MDTCT_LEFT; 
        }else{
            G_E(arr_index((void **)&cellTmpDefine,contentDefine->cells,k));
            cellTmp->type = cellTmpDefine->type;
        }
        G_E(arr_index((void **)&currStartPos,cellStartPos,k));
        cellTmp->pos = currStartPos;

        if( k >= cellStartPos->used - 1 ){
            cellTmp->len = dataLen - (currStartPos - data);
        }else{
            G_E(arr_index((void **)&nextStartPos,cellStartPos,k + 1));
            cellTmp->len = nextStartPos - currStartPos - 1;
        }

        content->cellsUsed++;
        rl->contentsUsed = 1;
    }

    if(isMatchingHeader){
        R(arr_shift(rl->contents,content));
    }

    if(NULL != cellStartPos) arr_free(&cellStartPos);
    return ret;
error:
    errVal = ret;
    if(NULL != cellStartPos) arr_free(&cellStartPos);
    if(NULL != content){
        if(isMatchingHeader){
            R(md_result_content_free(&content));
        }
    }
    return errVal;
}

static int parse_line_contents(md_result_line_t *rl){
    int ret = 0;
    R(NULL == rl);
    R(NULL == rl->data);
    R(0 >= rl->dataSize);

    const char *data = rl->prefixPos + rl->prefixLen;
    unsigned int dataLen = rl->dataSize - ( rl->prefixPos - rl->data ) - rl->prefixLen;
    R(0 > dataLen);

    unsigned int i = 0,iTmp = 0;
    char c = CHAR_ZERO;
    md_result_content_t *content = NULL;
    unsigned int contentSize = 0;
    unsigned int prefixSize = 0;
    unsigned int suffixSize = 0;
    md_type_e currType = MDT_NONE;
    char prefixC = CHAR_ZERO;

    const char *linkImageDescriptionPos = NULL;
    unsigned int linkImageDescriptionLen = 0;
    const char *linkImageUrlPos = NULL;
    unsigned int linkImageUrlLen = 0;
    const char *linkImageTitlePos = NULL;
    unsigned int linkImageTitleLen = 0;
    unsigned int isSuccess = 0;


    for(; i < dataLen; i++){
        contentSize = 0;
        currType = MDT_NONE;
        prefixSize = 0;
        suffixSize = 0;
        iTmp = i;
        linkImageDescriptionPos = NULL;
        linkImageDescriptionLen = 0;
        linkImageUrlPos = NULL;
        linkImageUrlLen = 0;
        linkImageTitlePos = NULL;
        linkImageTitleLen = 0;
        isSuccess = 0;

        c = data[i];
        if(CHAR_SLASH == prefixC){
            prefixC = c;
        }else{
            if('*' == c || '_' == c){
                if( IS_FONT_BOLD_ITALIC(data + i,dataLen-i,&contentSize) ){
                    i += contentSize;
                    currType = MDT_CONTENT_FONT_BOLD_ITALIC;
                    prefixSize = 3;
                    suffixSize = 3;
                }else if( IS_FONT_BOLD(data + i,dataLen - i, &contentSize) ){
                    i += contentSize;
                    currType = MDT_CONTENT_FONT_BOLD;
                    prefixSize = 2;
                    suffixSize = 2;
                }else if( IS_FONT_ITALIC(data + i, dataLen - i, &contentSize)  ){
                    i += contentSize;
                    currType = MDT_CONTENT_FONT_ITALIC;
                    prefixSize = 1;
                    suffixSize = 1;
                }else{
                    currType = MDT_NONE;
                }

            }else if('~' == c){
                if( IS_FONT_MIDDLE_LINE( data + i,dataLen - i,&contentSize) ){
                    i += contentSize;
                    currType = MDT_CONTENT_FONT_MIDDLE_LINE;
                    prefixSize = 2;
                    suffixSize = 2;
                }else{
                    currType = MDT_NONE;
                }
            }else if('<' == c){
                if( IS_FONT_BOTTOM_LINE(data + i,dataLen - i, &contentSize) ){
                    i += contentSize;
                    currType = MDT_CONTENT_FONT_BOTTOM_LINE;
                    prefixSize = 3;
                    suffixSize = 4;
                }else if( IS_LINK( data + i, dataLen - i, &contentSize ) ){
                    i += contentSize;
                    currType = MDT_CONTENT_LINK;
                    prefixSize = 1;
                    suffixSize = 1;
                }else{
                    currType = MDT_NONE;
                }
            }else if('!' == c){
                R(get_link_image(data + i, dataLen - i, &contentSize, 
                            &linkImageDescriptionPos, &linkImageDescriptionLen,
                            &linkImageUrlPos,&linkImageUrlLen,
                            &linkImageTitlePos,&linkImageTitleLen,&isSuccess));
                if(isSuccess){
                    i += contentSize;
                    currType = MDT_CONTENT_LINK_IMAGE;
                    prefixSize = 0;
                    suffixSize = 0;
                }else{
                    currType = MDT_NONE;
                }
            }else if('[' == c){
                R(get_link_description(data + i, dataLen - i, &contentSize, 
                            &linkImageDescriptionPos, &linkImageDescriptionLen,
                            &linkImageUrlPos,&linkImageUrlLen,
                            &linkImageTitlePos,&linkImageTitleLen,&isSuccess));
                printf("desc[%d][%s]\turl[%d][%s]\ttitle[%d][%s]\tsuccess[%d]\n",
                        linkImageDescriptionLen,linkImageDescriptionPos,
                        linkImageUrlLen,linkImageUrlPos,
                        linkImageTitleLen,linkImageTitlePos,
                        isSuccess);
                if(isSuccess){
                    i += contentSize;
                    currType = MDT_CONTENT_LINK_TEXT;
                    prefixSize = 0;
                    suffixSize = 0;

                }else{
                    currType = MDT_NONE;
                }
            }else{
                currType = MDT_NONE;
            }

            if(MDT_NONE != currType){
                content = NULL;
                R(md_result_contents_get_unused(&content,rl));
                content->type = currType;
                content->prefixPos = data + iTmp;
                content->prefixLen = prefixSize;
                content->contentPos = data + iTmp + prefixSize;
                content->contentLen = contentSize - prefixSize - suffixSize;
                content->suffixPos = data + i - suffixSize;
                content->suffixLen = suffixSize;
                rl->contentsUsed++;
                if(MDT_CONTENT_LINK == currType){
                    content->linkDescriptionPos = content->contentPos;
                    content->linkDescriptionLen = content->contentLen;
                    content->linkUrlPos = content->contentPos;
                    content->linkUrlLen = content->contentLen;
                    content->linkTitlePos = content->contentPos;
                    content->linkTitleLen = content->contentLen;
                }else if( MDT_CONTENT_LINK_TEXT == currType || 
                        MDT_CONTENT_LINK_IMAGE == currType ){
                    content->linkDescriptionPos = linkImageDescriptionPos;
                    content->linkDescriptionLen = linkImageDescriptionLen;
                    content->linkUrlPos = linkImageUrlPos;
                    content->linkUrlLen = linkImageUrlLen;
                    content->linkTitlePos = linkImageTitlePos;
                    content->linkTitleLen = linkImageTitleLen;
                }else{

                }
            }else{

            }
        }
    }
    return ret;
}

static int parse_line(md_result_t *r){
    int ret = 0;
    R(NULL == r);
    R(NULL == r->lines);
    md_result_content_t *content = NULL;

    md_result_line_t *rl = NULL;
    R(arr_index((void **)&rl,r->lines,r->matchingIndex));
    unsigned int size = rl->dataSize;
    R(0 >= size);

    md_result_line_t *rlPrev = NULL;
    if(r->matchingIndex > 0){
        R(arr_index((void **)&rlPrev,r->lines,r->matchingIndex - 1));
    }

    unsigned int i = 0,startPos = 0;
    char c = CHAR_ZERO;
    int isLinePrefix = 1;

    for(; i < size; i++){
        c = rl->data[i];
        if(1 == isLinePrefix){
            if(isspace(c)) {
                if(i >= size - 1){ // empty line
                    if(r->isMatchingCodeTag || r->isMatchingCodeAuto){
                        rl->type = MDT_LINE_CODE;
                    }else{
                        rl->type = MDT_LINE_EMPTY;
                    }
                    if(r->isMatchingTable){
                        r->isMatchingTable = 0;
                        r->lastLineTableDefine = NULL;
                    }
                }else{
                    continue;
                }
            }else{
                isLinePrefix = 0;
                rl->prefixPos = rl->data + i;

// [area matching] check if code line auto with >4 prefix space
                if(i >= 4){
                    rl->type = MDT_LINE_CODE;
                    rl->isAutoCodeLine = 1;
                    r->isMatchingCodeAuto = 1;
                    r->isMatchingTable = 0;
                    r->lastLineTableDefine = NULL;
                    break;
                }else{
                    if(r->isMatchingCodeAuto){
                        r->isMatchingCodeAuto = 0;
                    }
                }

                R(get_line_type(rl));
                unsigned int remainDataLen = rl->dataSize - (rl->prefixPos - rl->data) - rl->prefixLen;

// [area matching] check if code line with tag
                if(r->isMatchingCodeTag){
                    if( MDT_LINE_CODE_SUFFIX == rl->type ){
                        if(i <= r->matchingCodeTagSpacePrefixLen){
                            int n1 = i;
                            for(; n1 < rl->dataSize; n1++){
                                if('`' != rl->data[n1]) break;
                            }
                            if(n1 - i >= r->matchingCodeTagPrefixLen){
                                rl->type = MDT_LINE_CODE_SUFFIX;
                                r->isMatchingCodeTag = 0;
                                r->matchingCodeTagPrefixLen = 0;
                                r->matchingCodeTagSpacePrefixLen = 0;
                            }else{
                                rl->type = MDT_LINE_CODE;
                            }
                        }else{
                            rl->type = MDT_LINE_CODE;
                        }
                    }else{
                        rl->type = MDT_LINE_CODE;
                    }
                    break;
                }

// [area matching] check if table matching
                if(r->isMatchingTable) {
                    R(NULL == r->lastLineTableDefine);   
                    if( MDT_NONE == rl->type ){
                        if(parse_line_table_contents(rl,r->lastLineTableDefine,0)){
                            // L_I(1,"could not matched table content at line[%u]!\n",rl->lineNo);
                            MC_ERR_CLEAR();
                            r->isMatchingTable = 0;
                            r->lastLineTableDefine = NULL;
                        }else{
                            rl->type = MDT_LINE_TABLE_ROW;
                            content = NULL;
                            R(ARR_FIRST(&content,rl->contents));
                            content->type = MDT_CONTENT_TABLE_ROW;
                            break;
                        }
                    }else{
                        r->isMatchingTable = 0;
                        r->lastLineTableDefine = NULL;
                    }
                }
// regular matching
normal_match:
                if( (MDT_LINE_TITLE_1 <= rl->type && MDT_LINE_TITLE_6 >= rl->type ) || 
                        (MDT_LINE_REF_1 <= rl->type && MDT_LINE_REF_16 >= rl->type) ||
                        MDT_LINE_LIST_UN_SORT == rl->type || MDT_LINE_LIST_SORTED == rl->type || 
                        MDT_NONE == rl->type){
                    if(remainDataLen > 0){
                        R(parse_line_contents(rl));
                    }else{

                    }
                }else if( MDT_LINE_CODE_PREFIX == rl->type || MDT_LINE_CODE_SUFFIX == rl->type){
                    rl->type = MDT_LINE_CODE_PREFIX;
                    int n = i;
                    for(; n < rl->dataSize; n++){
                        if('`' != rl->data[n]) break;
                    }
                    rl->lineCodeNamePos = rl->data + n;
                    rl->prefixLen = n - i;
                    r->isMatchingCodeTag = 1;
                    r->matchingCodeTagPrefixLen = n -i;
                    r->matchingCodeTagSpacePrefixLen = i;
                } else if( MDT_LINE_TABLE_DEFINE == rl->type ){
                    if(NULL != rlPrev){
                        R(parse_line_table_define(rl));
                        int pret = parse_line_table_contents(rlPrev,rl,1);
                        if(pret){
                            // L_I(pret,"could not parsed table header at line[%u],defined cols [%u]!\n",rlPrev->lineNo,rl->contentsUsed);
                            MC_ERR_CLEAR();
                            rl->type = MDT_NONE;
                            rl->contentsUsed = 0;
                            content = NULL;
                            R(ARR_FIRST(&content,rl->contents));
                            content->cellsUsed = 0;
                        }else{
                            rlPrev->type = MDT_LINE_TABLE_HEADER;
                            r->isMatchingTable = 1;
                            r->lastLineTableDefine = rl;
                            content = NULL;
                            R(ARR_FIRST(&content,rlPrev->contents));
                            content->type = MDT_CONTENT_TABLE_HEADER;
                        }
                    }else{
                        rl->type = MDT_NONE;
                    }
                }else{

                }
                break;
            }
        }else{

        }
    }
    return ret;
}

int md_parse_file(md_t *md,md_result_t *result,const char *filePath){
    int ret = 0;
    R(NULL == md);
    R(NULL == result);
    R(NULL == filePath);

    FILE *f = fopen(filePath,"r");
    R(NULL == f);


    char *lineBuf = NULL; 
    R(MALLOC_TN(&lineBuf,str_t,MD_FILE_LINE_BUF_INIT_SIZE));
    size_t lineBufSize = MD_FILE_LINE_BUF_INIT_SIZE;
    unsigned int readSize = 0;
    unsigned int lineNo = 0;
    md_result_line_t *line = NULL;

    do{
        if(NULL == fgets(lineBuf + readSize,lineBufSize - readSize,f)){
            G_E(ferror(f));
            FREE_TN(&lineBuf,str_t,MD_FILE_LINE_BUF_INIT_SIZE);
            goto end;
        }else{
            readSize = strlen(lineBuf);
            if(readSize >= lineBufSize - 1){
                if(lineBuf[readSize - 1] == CHAR_NL){
                }else{
                    G_E(REALLOC_TN(&lineBuf,str_t,&lineBufSize));
                    continue;
                }
            }else{

            }
            R(md_result_line_init(&line));

            line->lineNo = lineNo + 1;
            line->data = lineBuf;
            line->dataSize = readSize;
            line->prefixPos = lineBuf;
            line->prefixLen = 0;
            line->bufSize = lineBufSize;
            line->result = result;

            G_E(arr_add(result->lines,line));
            result->matchingIndex = lineNo;

            G_E(parse_line(result));

            lineBuf = NULL;
            line = NULL;

            R(MALLOC_TN(&lineBuf,str_t,MD_FILE_LINE_BUF_INIT_SIZE));
            readSize = 0;
            lineBufSize = MD_FILE_LINE_BUF_INIT_SIZE;
            lineNo++;
        }

    }while(1);
end:
    fclose(f);
    return ret;
error:
    if(NULL != line){
        md_result_line_free(&line);
    }
    fclose(f);
    return ret;
}


