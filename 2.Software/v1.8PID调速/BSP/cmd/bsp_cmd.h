#ifndef __BSP_CMD_H
#define __BSP_CMD_H
#include "stm32f10x.h"

struct cmd_table
{
    char *name;
    void (*fun)(char *);
};

struct cmd
{
    struct cmd_table *table;
    u16 table_len;

    u8 *buf;
    u16 len;
    u16 count;
};

typedef struct cmd cmd_t;
void cmd_proc(struct cmd *const this, const char ch);
struct cmd *cmd_create(struct cmd_table *const table, const u16 table_len, const u16 len);
void cmd_destroy(struct cmd *const this);

struct cmd cmd_staic_create(struct cmd_table *const table, const u16 table_len, u8 *buf, const u16 buf_len);

#endif /* __MODULE02_H */
