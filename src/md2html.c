#include "inc.h"
#include <math.h>
#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD_MD2HTML
#endif

#define CEILU(x,y) ( ( ((x)%(y)) > 0 ) ? ((x) / (y) + 1) : ((x) / (y)) )

#define GET_RESULT_LINE_PREFIX_SIZE(rl) \
    ({\
        int _size = 0;md_result_content_t *pContent = NULL;\
        if(1 <= (rl)->dataSize ){\
            switch(rl->type){\
                case MDT_NONE :\
                case MDT_LINE_TITLE_1 :\
                case MDT_LINE_TITLE_2 :\
                case MDT_LINE_TITLE_3 :\
                case MDT_LINE_TITLE_4 :\
                case MDT_LINE_TITLE_5 :\
                case MDT_LINE_TITLE_6 :\
                case MDT_LINE_LIST_UN_SORT:\
                case MDT_LINE_LIST_SORTED:\
                case MDT_LINE_REF_1:\
                case MDT_LINE_REF_2:\
                case MDT_LINE_REF_3:\
                case MDT_LINE_REF_4:\
                case MDT_LINE_REF_5:\
                case MDT_LINE_REF_6:\
                case MDT_LINE_REF_7:\
                case MDT_LINE_REF_8:\
                case MDT_LINE_REF_9:\
                case MDT_LINE_REF_10:\
                case MDT_LINE_REF_11:\
                case MDT_LINE_REF_12:\
                case MDT_LINE_REF_13:\
                case MDT_LINE_REF_14:\
                case MDT_LINE_REF_15:\
                case MDT_LINE_REF_16:\
                    if((rl)->contentsUsed > 0){\
                        R(ARR_FIRST(&pContent,(rl)->contents));\
                        _size = pContent->prefixPos - (rl)->prefixPos - (rl)->prefixLen;\
                    }else{\
                        _size = (rl)->dataSize - ( (rl)->prefixPos - (rl)->data ) - (rl)->prefixLen;\
                    }\
                    break;\
                case MDT_LINE_EMPTY :\
                case MDT_LINE_TABLE_DEFINE:\
                case MDT_LINE_TABLE_HEADER:\
                case MDT_LINE_TABLE_ROW:\
                case MDT_LINE_CODE_PREFIX:\
                case MDT_LINE_CODE:\
                case MDT_LINE_CODE_SUFFIX:\
                case MDT_LINE_SPLIT:\
                    _size = 0;\
                    break;\
                default :\
                    G_E(1);\
            }\
        }else{\
            (_size) = 0;\
        }\
        G_EL(_size > (rl)->dataSize,"[%d]\tdataSize(%u)\tdata(%s)\tprefix(%s)(%u)\n",(rl)->lineNo,(rl)->dataSize,(rl)->data,(rl)->prefixPos,(rl)->prefixLen);\
        (_size);\
     })
#define CHECK_BUF_SIZE() \
    do{\
        if(currNeedSize > bufSize - bufUsed){ \
            needMoreBufSize = currNeedSize - (bufSize - bufUsed); \
            needMoreBufSizeLv = CEILU(needMoreBufSize,bufSize); \
            needMallocBufLv = CEILU(needMoreBufSizeLv,2); \
            printf("presize(%lu)\tused(%u)\t",bufSize,bufUsed);\
            bufSize *= needMallocBufLv;\
            printf("need(%u)\tmore(%u)\tmoreLv(%u)\tmallocLv(%u)\tsize(%lu)\n",currNeedSize,needMoreBufSize,needMoreBufSizeLv,needMallocBufLv,bufSize);\
            G_E(REALLOC_TN(&buf,str_t,&bufSize)); \
        }\
    }while(0)

#define MEMCPY_MD_DATA_2_BUF(_data_,_len_)\
    do{\
        if(_len_ > 0){\
            int _it = 0,_used = 0;\
            for(; _it < (_len_); _it++){\
                if('<' == (_data_)[_it]){\
                    buf[bufUsed++] = '&';\
                    buf[bufUsed++] = 'l';\
                    buf[bufUsed++] = 't';\
                    buf[bufUsed++] = ';';\
                }else if('>' == (_data_)[_it]){\
                    buf[bufUsed++] = '&';\
                    buf[bufUsed++] = 'g';\
                    buf[bufUsed++] = 't';\
                    buf[bufUsed++] = ';';\
                }else{\
                    buf[bufUsed++] = (_data_)[_it];\
                }\
                if(bufUsed >= bufSize){ G_E(REALLOC_TN(&buf,str_t,&bufSize)); }\
            }\
        }\
    }while(0)

#define MEMCPY_CODE_2_BUF(_data_,_len_)\
    do{\
        if(_len_ > 0){\
            int _it = 0,_used = 0;\
            for(; _it < (_len_); _it++){\
                if('<' == (_data_)[_it]){\
                    buf[bufUsed++] = '&';\
                    buf[bufUsed++] = 'l';\
                    buf[bufUsed++] = 't';\
                    buf[bufUsed++] = ';';\
                }else if('>' == (_data_)[_it]){\
                    buf[bufUsed++] = '&';\
                    buf[bufUsed++] = 'g';\
                    buf[bufUsed++] = 't';\
                    buf[bufUsed++] = ';';\
                }else if(CHAR_NL == (_data_)[_it]){\
                    buf[bufUsed++] = '<';\
                    buf[bufUsed++] = 'B';\
                    buf[bufUsed++] = 'R';\
                    buf[bufUsed++] = '>';\
                }else{\
                    buf[bufUsed++] = (_data_)[_it];\
                }\
            }\
        }\
    }while(0)

#define MEMCPY_2_BUF(_data_,_len_)\
    do{\
        if(_len_ > 0){\
            if(CHAR_NL == (_data_)[(_len_) - 1]){\
                memcpy(buf + bufUsed, (_data_), (_len_) - 1);\
                bufUsed += _len_ - 1;\
            }else{\
                memcpy(buf + bufUsed, (_data_), (_len_));\
                bufUsed += _len_;\
            }\
        }\
    }while(0)

int md_result_2_mem_html(md_t *md,md_result_t *result,char **dist,unsigned int *pBufSize,unsigned int *pDataSize){
    int ret = 0,errRet = 0;
    R(NULL == md);
    R(NULL == result);
    R(NULL == dist);
    R(NULL != (*dist));
    R(0 == md->isInitedTemplates);
    R(NULL == pBufSize);
    R(NULL == pDataSize);

    int iLine = 0,iContent = 0,iCell = 0;
    unsigned int currNeedSize = 0;
    unsigned int currTemplatePrefixSize = 0;
    md_template_t *currLineTemplate = NULL;
    md_template_t *currContentTemplate = NULL;
    md_template_t *currHtmlTemplate = md->templates + MDT_TEMPLATE_HTML;
    md_template_t *currHeaderTemplate = md->templates + MDT_TEMPLATE_HEADER;
    md_template_t *currBodyTemplate = md->templates + MDT_TEMPLATE_BODY;
    md_template_t *currContainerTemplate = md->templates + MDT_TEMPLATE_BODY_CONTAINER;
    md_template_t *codePrefixTemplate = md->templates + MDT_LINE_CODE_PREFIX;
    md_template_t *codeSuffixTemplate = md->templates + MDT_LINE_CODE_SUFFIX;
    md_template_t *tableSuffixTemplate = md->templates + MDT_TEMPLATE_TABLE_SUFFIX;
    md_template_t *emptyTemplate = md->templates + MDT_LINE_EMPTY;
    md_template_t *linkTemplate = md->templates + MDT_CONTENT_LINK;
    md_template_t *linkTextTemplate = md->templates + MDT_CONTENT_LINK_TEXT;
    md_template_t *linkImageTemplate = md->templates + MDT_CONTENT_LINK_IMAGE;

    md_result_line_t *lastResultLine = NULL;
    md_result_line_t *resultLine = NULL;
    md_result_line_t *nextResultLine = NULL;
    unsigned int currResultLinePrefixSize = 0;
    const char *currResultLinePrefixPos = NULL;
    const char *currResultLineSuffixPos = NULL;
    unsigned int currResultLineCodeNeedReplacedCharsCount = 0;
    unsigned int iChar = 0;
    unsigned int isInAutoCodeLine = 0;
    
    md_result_content_t *resultContent = NULL;
    md_table_cell_t *currTableCell = NULL;

    size_t bufSize = MD_HTML_BUF_INIT_SIZE;
    unsigned int bufUsed = 0;
    unsigned int needMoreBufSize = 0;
    unsigned int needMoreBufSizeLv = 0;
    unsigned int needMallocBufLv = 0;
    char *buf = NULL;
    R(MALLOC_TN(&buf,str_t,MD_HTML_BUF_INIT_SIZE));
    // write html,header,body,container prefix
    currNeedSize = currHeaderTemplate->prefixLen + currHeaderTemplate->prefixLen + currHeaderTemplate->suffixLen +
        currBodyTemplate->prefixLen + currContainerTemplate->prefixLen;
    CHECK_BUF_SIZE();
    MEMCPY_2_BUF(currHtmlTemplate->data,currHtmlTemplate->prefixLen);
    MEMCPY_2_BUF(currHeaderTemplate->data,currHeaderTemplate->prefixLen);
    MEMCPY_2_BUF(currHeaderTemplate->suffixPos,currHeaderTemplate->suffixLen);
    MEMCPY_2_BUF(currBodyTemplate->data,currBodyTemplate->prefixLen);
    MEMCPY_2_BUF(currContainerTemplate->data,currContainerTemplate->prefixLen);

    // write md's
    for(; iLine < result->lines->used; iLine++){
        G_E(arr_index((void **)&resultLine,result->lines,iLine));

        // white empty line
        if(MDT_LINE_EMPTY == resultLine->type){
            currNeedSize += emptyTemplate->prefixLen + emptyTemplate->suffixLen;
            CHECK_BUF_SIZE();
            MEMCPY_2_BUF(emptyTemplate->prefixPos,emptyTemplate->prefixLen);
            MEMCPY_2_BUF(emptyTemplate->suffixPos,emptyTemplate->suffixLen);
            continue;
        }

        currNeedSize = 0;
        // whence auto code line, we should copy code prefix line first
        if(MDT_LINE_CODE == resultLine->type && 1 == resultLine->isAutoCodeLine && 0 == isInAutoCodeLine ){
            if( NULL != lastResultLine && ( MDT_LINE_CODE_PREFIX == lastResultLine->type || MDT_LINE_CODE == lastResultLine->type )){

            }else{
                currNeedSize += codePrefixTemplate->prefixLen + strlen("AUTO") + codePrefixTemplate->suffixLen;
                CHECK_BUF_SIZE();
                MEMCPY_2_BUF(codePrefixTemplate->prefixPos,codePrefixTemplate->prefixLen);
                MEMCPY_2_BUF("AUTO",strlen("AUTO"));
                MEMCPY_2_BUF(codePrefixTemplate->suffixPos,codePrefixTemplate->suffixLen);
                isInAutoCodeLine = 1;
            }
        }
        currLineTemplate = md->templates + resultLine->type;
        currTemplatePrefixSize = currLineTemplate->prefixLen;
        currResultLinePrefixSize = GET_RESULT_LINE_PREFIX_SIZE(resultLine);
        currNeedSize = currTemplatePrefixSize + currResultLinePrefixSize;
        CHECK_BUF_SIZE();
        // write line prefix
        MEMCPY_2_BUF(currLineTemplate->data,currTemplatePrefixSize);
        MEMCPY_MD_DATA_2_BUF(resultLine->prefixPos + resultLine->prefixLen, currResultLinePrefixSize);
        currResultLineSuffixPos = resultLine->prefixPos + resultLine->prefixLen + currResultLinePrefixSize;
        // write contents 
        iContent = 0;
        for(; iContent < resultLine->contentsUsed; iContent++){
            resultContent = NULL;
            G_E(arr_index((void **)&resultContent,resultLine->contents,iContent));
            // write before content prefixPos contents
            if(currResultLineSuffixPos < resultContent->prefixPos){
                currNeedSize = resultContent->prefixPos - currResultLineSuffixPos;
                CHECK_BUF_SIZE();
                MEMCPY_MD_DATA_2_BUF(currResultLineSuffixPos,currNeedSize);
            }
            currContentTemplate = md->templates + resultContent->type;
            G_E(NULL == currContentTemplate);
            // write contents
            if( MDT_CONTENT_FONT_ITALIC == resultContent->type ||
                MDT_CONTENT_FONT_BOLD == resultContent->type ||
                MDT_CONTENT_FONT_BOLD_ITALIC == resultContent->type ||
                MDT_CONTENT_FONT_MIDDLE_LINE == resultContent->type ||
                MDT_CONTENT_FONT_BOTTOM_LINE == resultContent->type){
                currNeedSize = resultContent->contentLen + currContentTemplate->prefixLen + currContentTemplate->suffixLen;
                CHECK_BUF_SIZE();
                MEMCPY_2_BUF(currContentTemplate->prefixPos,currContentTemplate->prefixLen);
                MEMCPY_MD_DATA_2_BUF(resultContent->contentPos,resultContent->contentLen);
                MEMCPY_2_BUF(currContentTemplate->suffixPos,currContentTemplate->suffixLen);

            }else if( MDT_CONTENT_LINK_TEXT == resultContent->type ||
                    MDT_CONTENT_LINK_IMAGE == resultContent->type ||  
                    MDT_CONTENT_LINK == resultContent->type){
                const char *htmlLinkUrlPrefix = NULL;
                const char *htmlLinkTitlePrefix = NULL;
                const char *htmlLinkDescriptionPrefix = NULL;
                const char *htmlLinkSuffix = NULL;
                if(MDT_CONTENT_LINK_IMAGE == resultContent->type){
                    htmlLinkUrlPrefix = "<div><img class='md-link-img' src='";
                    htmlLinkTitlePrefix = "' title='";
                    htmlLinkDescriptionPrefix = "'/><span class='md-link-description'>";
                    htmlLinkSuffix = "</span></div>";
                }else{
                    htmlLinkUrlPrefix = "<a href='";
                    htmlLinkTitlePrefix = "' title='";
                    htmlLinkDescriptionPrefix = "' target='_blank' class='md-link'><span class='md-link-description'>";
                    htmlLinkSuffix = "</span></a>";
                }
                unsigned int _contentLen = strlen(htmlLinkUrlPrefix) + resultContent->linkUrlLen + 
                        strlen(htmlLinkTitlePrefix) + resultContent->linkTitleLen + 
                        strlen(htmlLinkDescriptionPrefix) + resultContent->linkDescriptionLen 
                        + strlen(htmlLinkSuffix); 
                currNeedSize = _contentLen + currContentTemplate->prefixLen + currContentTemplate->suffixLen;
                CHECK_BUF_SIZE();
                MEMCPY_2_BUF(currContentTemplate->prefixPos,currContentTemplate->prefixLen);
                MEMCPY_2_BUF(htmlLinkUrlPrefix,strlen(htmlLinkUrlPrefix));
                MEMCPY_MD_DATA_2_BUF(resultContent->linkUrlPos,resultContent->linkUrlLen);
                MEMCPY_2_BUF(htmlLinkTitlePrefix,strlen(htmlLinkTitlePrefix));
                MEMCPY_MD_DATA_2_BUF(resultContent->linkTitlePos,resultContent->linkTitleLen);
                MEMCPY_2_BUF(htmlLinkDescriptionPrefix,strlen(htmlLinkDescriptionPrefix));
                MEMCPY_MD_DATA_2_BUF(resultContent->linkDescriptionPos,resultContent->linkDescriptionLen);
                MEMCPY_2_BUF(htmlLinkSuffix,strlen(htmlLinkSuffix));
                MEMCPY_2_BUF(currContentTemplate->suffixPos,currContentTemplate->suffixLen);
            }else if( MDT_CONTENT_TABLE_HEADER == resultContent->type ||
                    MDT_CONTENT_TABLE_ROW == resultContent->type ||
                    MDT_CONTENT_TABLE_DEFINE == resultContent->type ){
                const char *htmlTableHeaderLeftPrefix = 
                    "<th class='md-table-header-cell md-table-header-cell-left' aligin='left'>";
                const char *htmlTableHeaderMiddlePrefix = 
                    "<th class='md-table-header-cell md-table-header-cell-middle' aligin='center'>";
                const char *htmlTableHeaderRightPrefix = 
                    "<th class='md-table-header-cell md-table-header-cell-right' aligin='right'>";
                const char *htmlTableHeaderSuffix = "</th>";
                
                const char *htmlTableRowLeftPrefix = 
                    "<td class='md-table-row-cell md-table-row-cell-left' aligin='left'>";
                const char *htmlTableRowMiddlePrefix = 
                    "<td class='md-table-row-cell md-table-row-cell-middle' aligin='center'>";
                const char *htmlTableRowRightPrefix = 
                    "<td class='md-table-row-cell md-table-row-cell-right' aligin='right'>";
                const char *htmlTableRowSuffix = "</td>";

                if(MDT_CONTENT_TABLE_DEFINE == resultContent->type){
                    currResultLineSuffixPos = resultLine->data + resultLine->dataSize;
                }else{
                    for(iCell = 0; iCell < resultContent->cellsUsed; iCell++){
                        G_E(arr_index((void **)&currTableCell,resultContent->cells,iCell));
                        unsigned int cellTagLen = 0;
                        if(MDTCT_LEFT == currTableCell->type){
                            cellTagLen = MDT_CONTENT_TABLE_HEADER == resultContent->type ?
                                (strlen(htmlTableHeaderLeftPrefix) + strlen(htmlTableHeaderSuffix)) :
                                (strlen(htmlTableRowLeftPrefix) + strlen(htmlTableRowSuffix));
                        }else if(MDTCT_RIGHT == currTableCell->type){
                            cellTagLen = MDT_CONTENT_TABLE_HEADER == resultContent->type ?
                                (strlen(htmlTableHeaderRightPrefix) + strlen(htmlTableHeaderSuffix)) :
                                (strlen(htmlTableRowRightPrefix) + strlen(htmlTableRowSuffix));
                        }else if(MDTCT_MIDDLE == currTableCell->type){
                            cellTagLen = MDT_CONTENT_TABLE_HEADER == resultContent->type ?
                                (strlen(htmlTableHeaderMiddlePrefix) + strlen(htmlTableHeaderSuffix)) :
                                (strlen(htmlTableRowMiddlePrefix) + strlen(htmlTableRowSuffix));
                        }else{
                            G_E(1);
                        }

                        currNeedSize = cellTagLen + currTableCell->len; 
                        CHECK_BUF_SIZE();
                        if(MDTCT_LEFT == currTableCell->type){
                            if( MDT_CONTENT_TABLE_HEADER == resultContent->type ){
                                MEMCPY_2_BUF(htmlTableHeaderLeftPrefix,strlen(htmlTableHeaderLeftPrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableHeaderSuffix,strlen(htmlTableHeaderSuffix));
                            }else{
                                MEMCPY_2_BUF(htmlTableRowLeftPrefix,strlen(htmlTableRowLeftPrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableRowSuffix,strlen(htmlTableRowSuffix));
                            }
                        }else if(MDTCT_RIGHT == currTableCell->type){
                            if( MDT_CONTENT_TABLE_HEADER == resultContent->type ){
                                MEMCPY_2_BUF(htmlTableHeaderRightPrefix,strlen(htmlTableHeaderRightPrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableHeaderSuffix,strlen(htmlTableHeaderSuffix));
                            }else{
                                MEMCPY_2_BUF(htmlTableRowRightPrefix,strlen(htmlTableRowRightPrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableRowSuffix,strlen(htmlTableRowSuffix));
                            }
                        }else if(MDTCT_MIDDLE == currTableCell->type){
                            if( MDT_CONTENT_TABLE_HEADER == resultContent->type ){
                                MEMCPY_2_BUF(htmlTableHeaderMiddlePrefix,strlen(htmlTableHeaderMiddlePrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableHeaderSuffix,strlen(htmlTableHeaderSuffix));
                            }else{
                                MEMCPY_2_BUF(htmlTableRowMiddlePrefix,strlen(htmlTableRowMiddlePrefix));
                                MEMCPY_MD_DATA_2_BUF(currTableCell->pos,currTableCell->len);
                                MEMCPY_2_BUF(htmlTableRowSuffix,strlen(htmlTableRowSuffix));
                            }
                        }else{
                            G_E(1);
                        }
                        if(currResultLineSuffixPos < currTableCell->pos + currTableCell->len){
                            currResultLineSuffixPos = currTableCell->pos + currTableCell->len;
                        }
                    }
                }
            }else{
                G_E(1);
            }
            if(resultContent->suffixPos + resultContent->suffixLen > currResultLineSuffixPos){
                currResultLineSuffixPos = resultContent->suffixPos + resultContent->suffixLen;
            }
        }
        // write line suffix
        unsigned int resultLineSuffixSize = 0;
        if(MDT_LINE_CODE == resultLine->type){ // we should replace all '<' and '>'
            for(iChar = 0; iChar < resultLine->dataSize; iChar++){
                if('<' == resultLine->data[iChar] || '>' == resultLine->data[iChar]){
                    currNeedSize += 3;
                }else if(CHAR_NL == resultLine->data[iChar]){
                    currNeedSize += 4; // <br>
                }
            }
            resultLineSuffixSize = resultLine->dataSize;
        }else{
            resultLineSuffixSize = resultLine->dataSize - (currResultLineSuffixPos - resultLine->data);
            if( resultLineSuffixSize > 0 && CHAR_NL == currLineTemplate->data[currLineTemplate->dataSize - 1]){
                resultLineSuffixSize--;
            }
        }
        currNeedSize = resultLineSuffixSize + currLineTemplate->suffixLen;
        CHECK_BUF_SIZE();
        if(MDT_LINE_CODE == resultLine->type){ // we should replace all '<' and '>'
            MEMCPY_CODE_2_BUF(resultLine->data,resultLineSuffixSize);
        }else{
            MEMCPY_MD_DATA_2_BUF(currResultLineSuffixPos,resultLineSuffixSize);
        }
        MEMCPY_2_BUF(currLineTemplate->suffixPos,currLineTemplate->suffixLen);

        // we should write the suffix of auto code line
        if(1 == isInAutoCodeLine && MDT_LINE_CODE == resultLine->type && 1 == resultLine->isAutoCodeLine){
            unsigned int isNeedWriteAutoCodeLineSuffix = 0;
            if(iLine >= result->lines->used - 1){
               isNeedWriteAutoCodeLineSuffix = 1; 
            }else{
                G_E(arr_index((void **)&nextResultLine,result->lines,iLine + 1));
                if( MDT_LINE_CODE != nextResultLine->type || 0 == nextResultLine->isAutoCodeLine ){
                    isNeedWriteAutoCodeLineSuffix = 1;
                }
            }
            if(isNeedWriteAutoCodeLineSuffix){
                currNeedSize = codeSuffixTemplate->prefixLen + strlen("AUTO") + codeSuffixTemplate->suffixLen;
                CHECK_BUF_SIZE();
                MEMCPY_2_BUF(codeSuffixTemplate->prefixPos,codeSuffixTemplate->prefixLen);
                MEMCPY_2_BUF("AUTO",strlen("AUTO"));
                MEMCPY_2_BUF(codeSuffixTemplate->suffixPos,codeSuffixTemplate->suffixLen);
                isInAutoCodeLine = 0;
            }
        }

        // write table suffix
        if(MDT_LINE_TABLE_DEFINE == resultLine->type || MDT_LINE_TABLE_ROW == resultLine->type){
            unsigned int isNeedWriteTableSuffix = 0;
            if(iLine >= result->lines->used - 1){
                isNeedWriteTableSuffix = 1;
            }else{
                G_E(arr_index((void **)&nextResultLine,result->lines,iLine + 1));
                if( MDT_LINE_TABLE_ROW != nextResultLine->type){
                    isNeedWriteTableSuffix = 1;
                }
            }
            if(isNeedWriteTableSuffix){
                currNeedSize = tableSuffixTemplate->prefixLen + tableSuffixTemplate->suffixLen;
                CHECK_BUF_SIZE();
                MEMCPY_2_BUF(tableSuffixTemplate->prefixPos,tableSuffixTemplate->prefixLen);
                MEMCPY_2_BUF(tableSuffixTemplate->suffixPos,tableSuffixTemplate->suffixLen);
            }

        }
        lastResultLine = resultLine;
    }
    // write html,header,body,container suffix
    currNeedSize = currHeaderTemplate->suffixLen + currBodyTemplate->suffixLen + currContainerTemplate->suffixLen;
    CHECK_BUF_SIZE();
    MEMCPY_2_BUF(currContainerTemplate->suffixPos,currContainerTemplate->suffixLen);
    MEMCPY_2_BUF(currBodyTemplate->suffixPos,currBodyTemplate->suffixLen);
    MEMCPY_2_BUF(currHtmlTemplate->suffixPos,currHtmlTemplate->suffixLen);

    (*dist) = buf;
    (*pBufSize) = bufSize;
    (*pDataSize) = bufUsed;
    return ret;
error:
    errRet = ret;
    if(NULL != buf) R(FREE_TN(&buf,str_t,bufSize));
    return errRet;
}

int md_parse_file_2_mem_html(md_t *md,const char *filePath,
        char **dist,unsigned int *distSize,unsigned int *distDataSize){
    int ret = 0,errRet = 0;
    R(NULL == md);
    R(0 == md->isInitedTemplates);
    R(NULL == filePath);
    R(NULL == dist);
    R(NULL != (*dist));
    R(access(filePath,F_OK|R_OK));
    md_result_t *result = NULL;
    R(md_result_init(&result));
    G_E(md_parse_file(md,result,filePath));
    G_E(md_result_2_mem_html(md,result,dist,distSize,distDataSize));
    G_E(md_result_free(&result));
    return ret;
error:
    errRet = ret;
    if(NULL != result) R(md_result_free(&result));
    return errRet;
}
int md_parse_file_2_file_html(md_t *md,const char *from,const char *to){
    int ret = 0,errRet = 0;
    char *buf = NULL;
    unsigned int bufSize = 0;
    unsigned int dataSize = 0;
    unsigned int writedSize = 0;
    FILE *f = NULL;

    R(NULL == md);
    R(NULL == from);
    R(NULL == to);
    R(access(from,F_OK|R_OK));
    R(NULL == (f = fopen(to,"w")) );
    G_E(md_parse_file_2_mem_html(md,from,&buf,&bufSize,&dataSize));
    writedSize = fwrite(buf,1,dataSize,f);
    G_EL(dataSize != writedSize,"(%u/%u)\t%s\n",writedSize,dataSize,buf);
    R(fclose(f));
    R(FREE_TN(&buf,str_t,bufSize));
    return ret;
error:
    errRet = ret;
    if(NULL != f) R(fclose(f));
    if(NULL != buf){
        R(FREE_TN(&buf,str_t,bufSize));
    }
    return errRet;
}
