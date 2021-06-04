#ifndef __MD_INC_H__
#define __MD_INC_H__

#include "md.h"
#include "rule.h"

int md_result_contents_init(arr_t **contents);
int md_result_contents_free(arr_t **contents);
int md_result_content_free(md_result_content_t **content);
int md_result_content_init(md_result_content_t **content);

int md_table_cells_get_unused(md_table_cell_t **dist,md_result_content_t *c);

int md_result_contents_get_unused(md_result_content_t **dist,md_result_line_t *rl);
#endif
