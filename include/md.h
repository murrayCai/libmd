#ifndef __MD_H__
#define __MD_H__

#include <ctype.h>
#include "mc.h"

typedef enum{
    MDT_NONE,
    MDT_LINE_EMPTY,
    MDT_LINE_TITLE_1,
    MDT_LINE_TITLE_2,
    MDT_LINE_TITLE_3,
    MDT_LINE_TITLE_4,
    MDT_LINE_TITLE_5,
    MDT_LINE_TITLE_6,
    MDT_LINE_TABLE_HEADER,
    MDT_LINE_TABLE_DEFINE,
    MDT_LINE_TABLE_ROW,
    MDT_LINE_LIST_UN_SORT,
    MDT_LINE_LIST_SORTED,
    MDT_LINE_CODE_PREFIX,
    MDT_LINE_CODE,
    MDT_LINE_CODE_SUFFIX,
    MDT_LINE_SPLIT,
    
    MDT_LINE_REF_1,
    MDT_LINE_REF_2,
    MDT_LINE_REF_3,
    MDT_LINE_REF_4,
    MDT_LINE_REF_5,
    MDT_LINE_REF_6,
    MDT_LINE_REF_7,
    MDT_LINE_REF_8,
    MDT_LINE_REF_9,
    MDT_LINE_REF_10,
    MDT_LINE_REF_11,
    MDT_LINE_REF_12,
    MDT_LINE_REF_13,
    MDT_LINE_REF_14,
    MDT_LINE_REF_15,
    MDT_LINE_REF_16,
    
    MDT_CONTENT_FONT_ITALIC,
    MDT_CONTENT_FONT_BOLD,
    MDT_CONTENT_FONT_BOLD_ITALIC,
    MDT_CONTENT_FONT_MIDDLE_LINE,
    MDT_CONTENT_FONT_BOTTOM_LINE,
    MDT_CONTENT_LINK,
    MDT_CONTENT_LINK_TEXT,
    MDT_CONTENT_LINK_IMAGE,

    MDT_CONTENT_TABLE_HEADER,
    MDT_CONTENT_TABLE_DEFINE,
    MDT_CONTENT_TABLE_ROW,

    MDT_TEMPLATE_HTML,
    MDT_TEMPLATE_HEADER,
    MDT_TEMPLATE_BODY,
    MDT_TEMPLATE_BODY_CONTAINER,
    MDT_TEMPLATE_TABLE_SUFFIX,

    MDT_MAX,
}md_type_e;
#define MD_TYPE_NAME(t) \
    ({\
        const char *name = NULL;\
        switch(t){\
            case MDT_NONE :                     name = "none";      break;\
            case MDT_LINE_EMPTY :               name = "empty";      break;\
            case MDT_LINE_TITLE_1  :            name = "title_1"; break;\
            case MDT_LINE_TITLE_2 :             name = "title_2"; break;\
            case MDT_LINE_TITLE_3 :             name = "title_3"; break;\
            case MDT_LINE_TITLE_4 :             name = "title_4"; break;\
            case MDT_LINE_TITLE_5:              name = "title_5"; break;\
            case MDT_LINE_TITLE_6:              name = "title_6"; break;\
            case MDT_LINE_TABLE_DEFINE:         name = "table_define"; break;\
            case MDT_LINE_TABLE_HEADER:         name = "table_header"; break;\
            case MDT_LINE_TABLE_ROW:            name = "table_row"; break;\
            case MDT_LINE_LIST_UN_SORT:         name = "list_un_sort"; break;\
            case MDT_LINE_LIST_SORTED:          name = "list_sorted"; break;\
            case MDT_LINE_CODE_PREFIX:          name = "code_prefix"; break;\
            case MDT_LINE_CODE:                 name = "code"; break;\
            case MDT_LINE_CODE_SUFFIX:          name = "code_suffix"; break;\
            case MDT_LINE_SPLIT:                name = "split"; break;\
            case MDT_LINE_REF_1:                name = "ref_1"; break;\
            case MDT_LINE_REF_2:                name = "ref_2"; break;\
            case MDT_LINE_REF_3:                name = "ref_3"; break;\
            case MDT_LINE_REF_4:                name = "ref_4"; break;\
            case MDT_LINE_REF_5:                name = "ref_5"; break;\
            case MDT_LINE_REF_6:                name = "ref_6"; break;\
            case MDT_LINE_REF_7:                name = "ref_7"; break;\
            case MDT_LINE_REF_8:                name = "ref_8"; break;\
            case MDT_LINE_REF_9:                name = "ref_9"; break;\
            case MDT_LINE_REF_10:               name = "ref_10"; break;\
            case MDT_LINE_REF_11:               name = "ref_11"; break;\
            case MDT_LINE_REF_12:               name = "ref_12"; break;\
            case MDT_LINE_REF_13:               name = "ref_13"; break;\
            case MDT_LINE_REF_14:               name = "ref_14"; break;\
            case MDT_LINE_REF_15:               name = "ref_15"; break;\
            case MDT_LINE_REF_16:               name = "ref_16"; break;\
            case MDT_CONTENT_FONT_ITALIC:       name = "font_italic"; break;\
            case MDT_CONTENT_FONT_BOLD:         name = "font_bold"; break;\
            case MDT_CONTENT_FONT_BOLD_ITALIC:  name = "font_bold_italic"; break;\
            case MDT_CONTENT_FONT_MIDDLE_LINE:  name = "font_middle_line"; break;\
            case MDT_CONTENT_FONT_BOTTOM_LINE:  name = "font_bottom_line"; break;\
            case MDT_CONTENT_LINK:              name = "link"; break;\
            case MDT_CONTENT_LINK_TEXT:         name = "link_text"; break;\
            case MDT_CONTENT_LINK_IMAGE:        name = "link_image"; break;\
            case MDT_CONTENT_TABLE_HEADER:      name = "table_header"; break;\
            case MDT_CONTENT_TABLE_ROW:         name = "table_row"; break;\
            case MDT_CONTENT_TABLE_DEFINE:      name = "table_define"; break;\
                                                \
            case MDT_TEMPLATE_HTML:             name = "html";break;\
            case MDT_TEMPLATE_HEADER:           name = "header";break;\
            case MDT_TEMPLATE_BODY:             name = "body";break;\
            case MDT_TEMPLATE_BODY_CONTAINER:   name = "container";break;\
            case MDT_TEMPLATE_TABLE_SUFFIX:     name = "table_suffix";break;\
            default :                           name = "UNKNOWN";break;\
        }\
        (name);\
     })

#define MD_FILE_LINE_BUF_INIT_SIZE 1024
#define MD_FILE_LINE_INIT_CONTENTS_COUNT 1
#define MD_TABLE_CELLS_INIT_COUNT 1
#define FILE_PATH_MAX 256
#define MD_TEMPLATE_FILE_EXT ".temp"
#define MD_HTML_BUF_INIT_SIZE 1024

typedef struct{
    const char *data;
    const char *filePath;
    unsigned int dataSize;
    unsigned int bufSize;
    const char *prefixPos;
    unsigned int prefixLen;
    const char *suffixPos;
    unsigned int suffixLen;
}md_template_t;

typedef struct{
    const char *dirTemplate;
    const char *templateVarName;
    md_template_t templates[MDT_MAX];
    int isInitedTemplates;
}md_t;

typedef enum{
    MDTCT_LEFT,
    MDTCT_MIDDLE,
    MDTCT_RIGHT
}md_table_cell_type_e;

#define MD_TABLE_CELL_NAME(t)\
    ({\
        const char *name = NULL;\
        switch(t){\
            case  MDTCT_LEFT: name = "LEFT"; break;\
            case  MDTCT_MIDDLE: name = "MIDDLE"; break;\
            case  MDTCT_RIGHT: name = "RIGHT"; break;\
            default : name = "UNKNOWN"; break;\
        }\
        (name);\
     })

typedef struct{
    md_table_cell_type_e type;
    const char *pos;
    unsigned int len;
}md_table_cell_t;

typedef struct{
    md_type_e type;
    const char *prefixPos;
    unsigned int prefixLen;
    const char *contentPos;
    unsigned int contentLen;
    const char *suffixPos;
    unsigned int suffixLen;

    /* link image */
    const char *linkDescriptionPos;
    unsigned int linkDescriptionLen;
    const char *linkUrlPos;
    unsigned int linkUrlLen;
    const char *linkTitlePos;
    unsigned int linkTitleLen;

    /* table  */
    unsigned int cellsUsed;
    arr_t *cells; // md_table_cell_t 's pointer array
}md_result_content_t;

typedef struct md_result_s md_result_t;
typedef struct{
    md_type_e type;
    unsigned int lineNo;
    
    const char *data;
    unsigned int dataSize;
    unsigned int bufSize;

    const char *prefixPos; // line tag prefixPos
    unsigned int prefixLen;// line tag prefix Size
    
    const char *lineCodeNamePos;
    unsigned int isAutoCodeLine;

    unsigned int contentsUsed;
    arr_t *contents;
    md_result_t *result;
}md_result_line_t;

struct md_result_s{
    arr_t *lines;
    int isMatchingTable;
    int matchingIndex;
    md_result_line_t *lastLineTableDefine;
    int isMatchingCodeTag;
    unsigned int matchingCodeTagPrefixLen;
    unsigned int matchingCodeTagSpacePrefixLen;
    int isMatchingCodeAuto;
};

int md_result_init(md_result_t **result);
int md_result_free(md_result_t **r);
int md_result_line_init(md_result_line_t **rl);
int md_result_line_free(md_result_line_t **rl);

int md_parse_file(md_t *md,md_result_t *result,const char *filePath);
// this function should call FREE_TN(dist,str_t,*distSize)
int md_parse_file_2_mem_html(md_t *md,const char *filePath,char **dist,unsigned int *distSize,unsigned int *distDataSize);
int md_parse_file_2_file_html(md_t *md,const char *from,const char *to);
int md_print_templates(md_t *md);

int md_init(md_t **md);
int md_load_templates(md_t *md,const char *dirTemplate,const char *templateVarName);
int md_clear_templates(md_t *md);
int md_free(md_t **md);

#endif
