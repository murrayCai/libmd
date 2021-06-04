#ifndef __MD_RULE_H__
#define __MD_RULE_H__

#define CHAR_ZERO 0
#define CHAR_CR 13
#define CHAR_NL 10
#define CHAR_SLASH 92

#define IS_LINE_TITLE_1(str,len) ((len) > 1 && '#' == (str)[0] && isspace((str)[1]))
#define IS_LINE_TITLE_2(str,len) ((len) > 2 && '#' == (str)[0] && '#' == (str)[1] && isspace((str)[2]) )
#define IS_LINE_TITLE_3(str,len) ((len) > 3 && '#' == (str)[0] && '#' == (str)[1] && '#' == (str)[2] && isspace((str)[3]))
#define IS_LINE_TITLE_4(str,len) ((len) > 4 && '#' == (str)[0] && '#' == (str)[1] && '#' == (str)[2] && '#' == (str)[3] && isspace((str)[4]) )
#define IS_LINE_TITLE_5(str,len) ((len) > 5 && '#' == (str)[0] && '#' == (str)[1] && '#' == (str)[2] && '#' == (str)[3]  && '#' == (str)[4] && isspace((str)[5])) 
#define IS_LINE_TITLE_6(str,len) ((len) > 6 && '#' == (str)[0] && '#' == (str)[1] && '#' == (str)[2] && '#' == (str)[3]  && '#' == (str)[4] && '#' == (str)[5] && isspace((str)[6]) )

#define IS_LINE_CODE_TAG(str,len) ((len) > 3 && '`' == (str)[0] && '`' == (str)[1] && '`' == (str)[2])

#define IS_LINE_CODE_PREFIX(str,len) \
    ({\
        int _it = 0,_is = 1,_isContinuedPrefixChar = 1;\
        if( IS_LINE_CODE_TAG( (str), (len) ) ){\
           for( _it = 3; _it < (len); _it++ ){\
                if( '`' == (str)[_it] ){\
                    if(_isContinuedPrefixChar){\
                    }else{\
                        _is = 0;\
                        break;\
                    }\
                }else{\
                    _isContinuedPrefixChar = 0;\
                }\
           }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

#define IS_LINE_CODE_SUFFIX(str,len) \
    ({\
     int _it = 0,_is = 1;\
        if( IS_LINE_CODE_TAG( (str), (len) ) ){\
           for( _it = 3; _it < (len); _it++ ){\
                if( !(CHAR_NL == (str)[_it] || CHAR_CR == (str)[_it] || '`' == (str)[_it] || isspace((str)[_it])) ){\
                    _is = 0;\
                    break;\
                }\
           }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })


#define IS_LINE_SPLIT(str,len) \
    ({\
        int _i = 0,_is = 1,_count = 0;\
        char _tag = (str)[0];\
        for(;_i < (len); _i++){\
            if(!( isspace((str)[_i]) || _tag == (str)[_i] || CHAR_CR == (str)[_i] || CHAR_NL == (str)[_i] )){\
                _is = 0;\
                break;\
            }else{\
                if(_tag == (str)[_i]){\
                    _count++;\
                }\
            }\
        }\
        if(_is){\
            _is = _count >= 3 ? 1 : 0;\
        }\
        (_is);\
    })

#define GET_LINE_REF_LV(str,len,prefixSize) \
    ({\
        int _ir = 0,_lv = 0; \
        for(; _ir < (len); _ir++){\
            if('>' == (str)[_ir]){_lv++; (*(prefixSize))++; } \
            else if(isspace((str)[_ir])){ (*(prefixSize))++; }\
            else{ break;}\
        }\
        (_lv);\
     })

#define IS_LINE_UN_SORT_LIST(str,size) ((size) > 2 && ( '*' == (str)[0] || '-' == (str)[0] || '+' == (str)[0] ) &&  isspace((str)[1]) )

#define IS_NUM(_c) ('0' <= (_c) && '9' >= (_c) )
#define IS_LINE_SORTED_LIST(str,size,prefixSize) \
    ({\
        int _il = 0,_is = 1;\
        for(;_il < (size); _il++){\
            if(IS_NUM((str)[_il])){\
                (*(prefixSize))++;\
            }else{\
                if('.' == (str)[_il] && (_il + 1) < (size) && isspace((str)[_il + 1])){\
                    (*(prefixSize)) += 2;\
                    break;\
                }else{\
                    (*(prefixSize)) = 0;\
                    _is = 0;\
                }\
            }\
        }\
        (_is);\
     })

#define IS_LINE_TABLE_DEFINE(str,size) \
    ({\
        int _ir = 0,_is = 1,_isColStart = 1,_colCount = 0;\
        for(; _ir < (size); _ir++){\
            if(!('|' == (str)[_ir] || '-' == (str)[_ir] || ':' == (str)[_ir] || CHAR_NL == (str)[_ir])){ \
                _is = 0; \
                break;\
            }else{\
                if(_isColStart){\
                    if(!(':' == (str)[_ir] || '-' == (str)[_ir])){\
                        _is = 0;\
                        break;\
                    }else{\
                        _isColStart = 0;\
                    }\
                }else{\
                    if( '|' == (str)[_ir] ){\
                        _colCount++;\
                        _isColStart = 1;\
                    }else{\
                        _isColStart = 0;\
                    }\
                }\
            }\
        }\
        if(_is){\
            if(_colCount == 0) _is = 0;\
        }\
        (_is);\
     })

#define IS_FONT_BOLD_ITALIC_TAG(str,size) (2 < (size) && ('*' == (str)[0] || '_' == (str)[0] ) && (str)[0] == (str)[1] && (str)[0] == (str)[2])
#define IS_FONT_BOLD_TAG(str,size) (1 < (size) && ('*' == (str)[0] || '_' == (str)[0] ) && (str)[0] == (str)[1])

#define IS_FONT_BOLD_ITALIC(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0],_prefix = CHAR_ZERO;\
        if(IS_FONT_BOLD_ITALIC_TAG( (str),(size) ) && !isspace( (str)[3] ) && _tag != (str)[3] ){\
            _prefix = _tag;\
            for(_it = 4;_it < (size); _it++){\
                if(_tag == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        if(IS_FONT_BOLD_ITALIC_TAG((str) + _it, (size) - _it) && _tag != (str)[_it - 1] && !isspace( (str)[_it - 1] )){\
                            _is = 1;\
                            (*(_pContentSize)) = _it + 3;\
                        }else{\
                            _is = 0;\
                        }\
                        break;\
                    }\
                }else{\
                    _prefix = (str)[_it];\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

#define IS_FONT_BOLD(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0],_prefix = CHAR_ZERO;\
        if(IS_FONT_BOLD_TAG(str,size) && !isspace( (str)[2] ) && _tag != (str)[2] ){\
            _prefix = _tag;\
            for(_it = 3;_it < (size); _it++){\
                if(_tag == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        if(IS_FONT_BOLD_TAG((str) + _it, (size) - _it) && _tag != (str)[_it - 1] && !isspace( (str)[_it - 1] )){\
                            _is = 1;\
                            (*(_pContentSize)) = _it + 2;\
                        }else{\
                            _is = 0;\
                        }\
                        break;\
                    }\
                }else{\
                    _prefix = (str)[_it];\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

#define IS_FONT_ITALIC(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0], _prefix = (str)[0];\
        if(!isspace( (str)[1]) && _tag != (str)[1] ){\
            for(_it = 1;_it < (size); _it++){\
                if(_tag == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        if( !isspace( (str)[_it - 1] ) && _tag != (str)[_it - 1] ){\
                            _is = 1;\
                            (*(_pContentSize)) = _it + 1;\
                        }else{\
                            _is = 0;\
                        }\
                        break;\
                    }\
                }\
            }\
        }\
        (_is);\
     })

#define IS_FONT_MIDDLE_LINE_TAG(str,size) (1 < (size) && '~' == (str)[0] && '~' == (str)[1])
#define IS_FONT_MIDDLE_LINE(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0],_prefix = CHAR_ZERO;\
        if(IS_FONT_MIDDLE_LINE_TAG( (str) , (size) )){\
            _prefix = _tag;\
            for(_it = 2;_it < (size); _it++){\
                if(_tag == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        if(IS_FONT_MIDDLE_LINE_TAG( (str) + _it, (size) - _it )){\
                            _is = 1;\
                            (*(_pContentSize)) = _it + 2;\
                        }else{\
                            _is = 0;\
                        }\
                    }\
                }else{\
                    _prefix = (str)[_it];\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

#define IS_FONT_BOTTOM_LINE_PREFIX_TAG(str,size) ( 2 < (size) && '<' == (str)[0] && ( 'u' == (str)[1] || 'U' == (str)[1] ) && '>' == (str)[2] )
#define IS_FONT_BOTTOM_LINE_SUFFIX_TAG(str,size) ( 3 < (size) && '<' == (str)[0] && '/' == (str)[1] && ('u' == (str)[2]  || 'U' == (str)[2] )   && '>' == (str)[3])

#define IS_FONT_BOTTOM_LINE(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0],_prefix = CHAR_ZERO;\
        if(IS_FONT_BOTTOM_LINE_PREFIX_TAG( (str) , (size) )){\
            _prefix = _tag;\
            for(_it = 3;_it < (size); _it++){\
                if(_tag == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        if(IS_FONT_BOTTOM_LINE_SUFFIX_TAG( (str) + _it, (size) - _it )){\
                            _is = 1;\
                            (*(_pContentSize)) = _it + 4;\
                        }else{\
                            _is = 0;\
                        }\
                    }\
                }else{\
                    _prefix = (str)[_it];\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })

#define IS_LINK_PROTO_TAG(str,size) ( 3 < (size) && ':' == (str)[0] && '/' == (str)[1] && '/' == (str)[2] ) 

#define IS_LINK_HTTP_TAG(str,size) ( 6 < (size) && ( 'h' == (str)[0] || 'H' == (str)[0] ) && ( 't' == (str)[1] || 'T' == (str)[1] ) && ( 't' == (str)[2] || 'T' == (str)[2]  ) && ( 'p' == (str)[3] || 'P' == (str)[3] ) && IS_LINK_PROTO_TAG( (str) + 4, (size) - 4 ))

#define IS_LINK_HTTPS_TAG(str,size) ( 7 < (size) && ( 'h' == (str)[0] || 'H' == (str)[0] ) && ( 't' == (str)[1] || 'T' == (str)[1] ) && ( 't' == (str)[2] || 'T' == (str)[2]  ) && ( 'p' == (str)[3] || 'P' == (str)[3] ) && ( 's' == (str)[4] || 'S' == (str)[4] ) && IS_LINK_PROTO_TAG( (str) + 5, (size) - 5 ) )

#define IS_LINK_FTP_TAG(str,size) ( 5 < (size) && ( 'f' == (str)[0] || 'F' == (str)[0] ) && ( 't' == (str)[1] || 'T' == (str)[1] ) && ( 'p' == (str)[2] || 'P' == (str)[2] )  && IS_LINK_PROTO_TAG( (str) + 3, (size) - 3 ) )

#define IS_URL_CHAR(c) ( ( '0' <= (c) && '9' >= (c)) ||  ( 'a' <= (c) && 'z' >= (c) ) ||  ( 'a' <= (c) && 'z' >= (c) ) || ':' == (c) || '/' == (c) || '.' == (c) || '?' == (c) || '%' == (c) || '=' == (c) || '%' == (c) || '#' == (c) || '&' == (c) || '+' == (c) || '@' == (c) || '-' == (c) || '_' == (c) || '~' == (c) || '!' == (c) || '(' == (c) || ')' == (c) || ';' == (c) || '$' == (c) || ',' == (c) || '[' == (c) || ']' == (c) || "'"[0] == (c) )

#define IS_LINK(str,size,_pContentSize) \
    ({\
        int _it = 0,_is = 0;\
        char _tag = (str)[0],_prefix = CHAR_ZERO;\
        if( IS_LINK_HTTP_TAG( (str)+1 , (size)-1 ) || IS_LINK_HTTPS_TAG( (str)+1, (size)-1 ) || IS_LINK_FTP_TAG( (str)+1, (size)-1 ) ){\
            _prefix = _tag;\
            for(_it = 1; _it < (size); _it++){\
                if(! ( IS_URL_CHAR( (str)[_it]) || '>' == (str)[_it] || '<' == (str)[_it] )  ) break;\
                if('>' == (str)[_it]){\
                    if(CHAR_SLASH == _prefix){\
                        _prefix = (str)[_it];\
                    }else{\
                        _is = 1;\
                        (*(_pContentSize)) = _it + 1;\
                    }\
                }else{\
                    _prefix = (str)[_it];\
                }\
            }\
        }else{\
            _is = 0;\
        }\
        (_is);\
     })


#define IS_LINK_IMAGE_PREFIX_TAG(str,size) ( 1 < (size) && '!' == (str)[0] && '[' == (str)[1] )
#define IS_LINK_IMAGE_DESCRIPTION_SUFFIX_TAG(str,size) ( 1 < (size) && ']' == (str)[0] && '(' == (str)[1] )
#define IS_LINK_IMAGE_URL_SUFFIX_TAG(str,size) ( 1 < (size) && ( isspace( (str)[0] ) || ')' == (str)[0] ) )
#define IS_LINK_IMAGE_TITLE_PREFIX_TAG(str,size) ( 1 < (size) &&  )


int get_link_image(const char *str,unsigned int size,unsigned int *contentSize,
        const char **descriptionPos,unsigned int *descriptionLen,
        const char **urlPos,unsigned int *urlLen,
        const char **titlePos,unsigned int *titleLen,
        unsigned int *isSuccess);

int get_link_description(const char *str,unsigned int size,unsigned int *contentSize,
        const char **descriptionPos,unsigned int *descriptionLen,
        const char **urlPos,unsigned int *urlLen,
        const char **titlePos,unsigned int *titleLen,
        unsigned int *isSuccess);

#endif

