#include "inc.h"
#ifdef MODULE_CURR
#undef MODULE_CURR
#define MODULE_CURR MODULE_MD_RULE
#endif

int get_link_image(const char *str,unsigned int size,unsigned int *contentSize,
        const char **descriptionPos,unsigned int *descriptionLen,
        const char **urlPos,unsigned int *urlLen,
        const char **titlePos,unsigned int *titleLen,
        unsigned int *isSuccess){
    int ret = 0;
    R(NULL == str);
    R(NULL == contentSize);
    R(NULL == descriptionPos);
    R(NULL != (*descriptionPos) );
    R(NULL == descriptionLen);
    R(NULL == urlPos);
    R(NULL != (*urlPos) );
    R(NULL == urlLen);
    R(NULL == titlePos);
    R(NULL != (*titlePos) );
    R(NULL == titleLen);
    R(NULL == isSuccess);

    int _it = 0;
    char prefixC = '\0';

    if( !IS_LINK_IMAGE_PREFIX_TAG(str,size) ) {
        *isSuccess = 0;
    }else{
        R(get_link_description(str + 1,size - 1,contentSize,descriptionPos,descriptionLen,
                    urlPos,urlLen,titlePos,titleLen,isSuccess));
        if(*isSuccess){
            (*contentSize) += 1;
        }else{
            (*contentSize) = 0;
        }
    }
    return ret;
}

int get_link_description(const char *str,unsigned int size,unsigned int *contentSize,
        const char **descriptionPos,unsigned int *descriptionLen,
        const char **urlPos,unsigned int *urlLen,
        const char **titlePos,unsigned int *titleLen,
        unsigned int *isSuccess){
    int ret = 0;
    R(NULL == str);
    R(NULL == contentSize);
    R(NULL == descriptionPos);
    R(NULL != (*descriptionPos) );
    R(NULL == descriptionLen);
    R(NULL == urlPos);
    R(NULL != (*urlPos) );
    R(NULL == urlLen);
    R(NULL != (*titlePos) );
    R(NULL == titleLen);
    R(NULL == isSuccess);

    int isMatchingUrlPrefixSpace = 1;
    int isMatchingUrlSuffixSpace = 0;
    const char *urlEndPos = NULL;
    const char *titleEndPos = NULL;

    int _it = 0;
    char prefixC = '\0';

    if( '[' != str[0] ) {
        *isSuccess = 0;
    }else{
        _it += 1;
        goto match_description;
    }
end:
    (*contentSize) = _it;
    return ret;

match_description: // when matched "[", then matching "]("
    (*descriptionPos) = str + _it;
    for(; _it < size; _it++ ){
        if(CHAR_SLASH == prefixC){
            prefixC = str[_it];
        }else{
            if( ']' == str[_it] ){
                if( IS_LINK_IMAGE_DESCRIPTION_SUFFIX_TAG( str + _it,( size - _it) ) ){
                    (*descriptionLen) = str + _it - (*descriptionPos);
                    _it += 2;
                    goto match_url;
                }else{
                    break;
                }
            }else{
                prefixC = str[_it];
            }
        }
    }
    _it = 0;
    *isSuccess = 0;
    goto end;

match_url: // when matched "](", then matching " \"" or " )"
    for(; _it < size; _it++){
        if(CHAR_SLASH == prefixC){
            prefixC = str[_it];
        }else{
            if(isMatchingUrlPrefixSpace){
                if( isspace(str[_it]) ){
                    continue;
                }else{
                    isMatchingUrlPrefixSpace = 0;
                    (*urlPos) = str + _it;
                    goto matching_url_char;
                }
            }

matching_url_suffix_space:
            if(isMatchingUrlSuffixSpace){
                if( isspace(str[_it]) ){
                    continue;
                }else{
                    goto matching_url_end;
                }
            }else{
                if( isspace(str[_it]) ){
                    urlEndPos = str + _it;
                    isMatchingUrlSuffixSpace = 1;
                    continue;
                }else{
                    goto matching_url_char;
                }
            }

matching_url_char:
            if( isspace(str[_it]) ){
                if(isMatchingUrlSuffixSpace){
                }else{
                    urlEndPos = str + _it;
                    isMatchingUrlSuffixSpace = 1;
                }
                continue;
            }else{
                if('"' == str[_it] || ')' == str[_it]){
                    if(NULL == urlEndPos){
                        urlEndPos = str + _it;
                    }else{
                        // printf("===========[%d]failed at[%d](%s):%s",__LINE__,_it,str,str + _it);
                    }
                    goto matching_url_end;
                }else{
                    if( IS_URL_CHAR(str[_it]) ){
                        continue;
                    }else{
                        // printf("===========[%d]failed at[%d](%s):%s",__LINE__,_it,str,str + _it);
                        break;
                    }
                }
            }

matching_url_end:
            if('"' == str[_it]){
                (*urlLen) = urlEndPos - (*urlPos);
                _it++;
                goto match_title;
            }else if(')' == str[_it]){
                (*urlLen) = urlEndPos - (*urlPos);
                _it++;
                *isSuccess = 1;
                goto end;
            }else{ // error
                // printf("===========[%d]failed at[%d](%s):%s",__LINE__,_it,str,str + _it);
                break;
            }
        }
    }
    _it = 0;
    *isSuccess = 0;
    goto end;

match_title: // when matched " \"", then matching "\" )"
    (*titlePos) = str + _it;
    for(; _it < size; _it++){
        if(CHAR_SLASH == prefixC){
            prefixC = str[_it];
        }else{
            if('"' == str[_it]){
                titleEndPos = str + _it;
            }else if(')' == str[_it]){
                if(NULL == titleEndPos){
                    // just title's ) char
                }else{
                    (*titleLen) = titleEndPos - (*titlePos);
                    _it++;
                    *isSuccess = 1;
                    goto end;
                }
            }else{
                if( NULL == titleEndPos ){
                    // regular title chars is ok
                }else{
                    // only space char is ok
                    if( isspace(str[_it]) ){
                        continue;
                    }else{
                        break;
                    }
                }
            }
        }
    }
    _it = 0;
    *isSuccess = 0;
    goto end;
}
