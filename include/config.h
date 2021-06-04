#ifndef __MD_CONFIG_H__
#define __MD_CONFIG_H__

/* line title */
#define MD_TITLE_1_PREFIX "# "
#define MD_TITLE_1_SUFFIX "#"

#define MD_TITLE_2_PREFIX "## "
#define MD_TITLE_2_SUFFIX "##"

#define MD_TITLE_3_PREFIX "### "
#define MD_TITLE_3_SUFFIX "###"

#define MD_TITLE_4_PREFIX "#### "
#define MD_TITLE_4_SUFFIX "####"

#define MD_TITLE_5_PREFIX "##### "
#define MD_TITLE_5_SUFFIX "#####"

#define MD_TITLE_6_PREFIX "###### "
#define MD_TITLE_6_SUFFIX "######"

/* line split */
#define MD_LINE_PREFIX "---|***|___"
#define MD_LINE_SUFFIX "\n"

/* line code */
#define MD_CODE_PREFIX "```"
#define MD_CODE_SUFFIX "```"

/* line ref */
#define MD_REF_PREFIX ">"
#define MD_REF_SUFFIX ""

/* line list */
#define MD_LIST_UN_SORT_PREFIX "* |- |+ "
#define MD_LIST_UN_SORT_SUFFIX ""

#define MD_LIST_SORTED_PREFIX ". "
#define MD_LIST_SORTED_SUFFIX ""

/* line table */
#define MD_TABLE_COLUMN_SPLIT "|"
#define MD_TABLE_DEF_PREFIX ""
#define MD_TABLE_DEF_SUFFIX "\n"
#define MD_TABLE_DEF_COLUMN_ALIGIN ":"


/* font */
#define MD_FONT_ITALIC_PREFIX "*|_"
#define MD_FONT_ITALIC_SUFFIX "*|_"

#define MD_FONT_BOLD_PREFIX "**|__"
#define MD_FONT_BOLD_SUFFIX "**|__"

#define MD_FONT_BOLD_ITALIC_PREFIX "***|___"
#define MD_FONT_BOLD_ITALIC_SUFFIX "***|___"

#define MD_FONT_MIDDLE_LINE_PREFIX "~~"
#define MD_FONT_MIDDLE_LINE_SUFFIX "~~"

#define MD_FONT_BOTTOM_LINE_PREFIX "<u>"
#define MD_FONT_BOTTOM_LINE_SUFFIX "</u>"

#define MD_FOOT_DESCRIPTION_PREFIX "[^"
#define MD_FOOT_DESCRIPTION_SUFFIX "]:"


/* link */
#define MD_LINK_PREFIX "<"
#define MD_LINK_SUFFIX ">"

/* link text */
#define MD_LINK_TEXT_PREFIX "["
#define MD_LINK_TEXT_SUFFIX "]"

#define MD_LINK_TEXT_CONTENT_PREFIX "("
#define MD_LINK_TEXT_CONTENT_SUFFIX ")"
#define MD_LINK_TEXT_CONTENT_URL_PREFIX ""
#define MD_LINK_TEXT_CONTENT_URL_SUFFIX ""
#define MD_LINK_TEXT_CONTENT_TITLE_PREFIX " \""
#define MD_LINK_TEXT_CONTENT_TITLE_SUFFIX "\""


/* link img */
#define MD_LINK_IMG_PREFIX "!"
#define MD_LINK_IMG_SUFFIX ""

#define MD_LINK_IMG_DESCRIPTION_PREFIX "["
#define MD_LINK_IMG_DESCRIPTION_SUFFIX "]"
#define MD_LINK_IMG_CONTENT_PREFIX "("
#define MD_LINK_IMG_CONTENT_SUFFIX ")"

#define MD_LINK_IMG_CONTENT_URL_PREFIX ""
#define MD_LINK_IMG_CONTENT_URL_SUFFIX ""

#define MD_LINK_IMG_CONTENT_TITLE_PREFIX " \""
#define MD_LINK_IMG_CONTENT_TITLE_SUFFIX "\""




#endif
