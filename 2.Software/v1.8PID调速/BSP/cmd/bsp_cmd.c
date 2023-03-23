/*****************************************************************************
 * copyleft (c) 2023 善 思 维
 *
 * 文 件 名:   bsp_cmd.c
 * 功    能:   控制台字符串流解析器(串口调参工具)
 * 作    者:   善思维
 * 日    期:   2023-03-03 09:51:31
 * 版    本:   v1.0.0
 * ---------------------------------------------------------------------------
 * 文件修改历史:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-28  |   v1.0.0    |    ssw     |     创建文件
 *  2023-03-3   |   v1.0.1    |    ssw     |     修改BUG
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ 头 文 件 ==================================*/

#include "bsp_cmd.h"/* "stm32f10x.h" */

//#include <stdio.h>/* printf */
#include <stdlib.h>/* 动态内存 */
#include <string.h>/* 字符串处理 */
/*============================== 私 有 函 数 ================================*/
static char *skip(char *buf);
static void cmd_fun_run(struct cmd *const this, char *buf);
/*=========================== 结 构 体 / 联 合 体 ============================*/
/*=============================== 宏 定 义 =================================*/
/*============================== 全 局 变 量 ================================*/
/*============================== 接 口 函 数 ================================*/
struct cmd *cmd_create(struct cmd_table *const table, const u16 table_len,
                       const u16 len) {
  struct cmd *this = (struct cmd *)malloc(sizeof(struct cmd));
  if (NULL == this) return NULL;

  this->buf = (u8 *)malloc(sizeof(u8) * len);
  if (NULL == this->buf) return NULL;

  this->count = 0;
  this->len = len;
  this->table_len = table_len;
  this->table = table;
  memset(this->buf, 0, this->len);
  return this;
}

struct cmd cmd_staic_create(struct cmd_table *const table, const u16 table_len,
                            u8 *buf, const u16 buf_len) {
  struct cmd this;

  this.table = table;
  this.table_len = table_len;
  this.buf = buf;
  this.len = buf_len;

  return this;
}

void cmd_destroy(struct cmd *const this) { free(this); }

void cmd_proc(struct cmd *const this, const char ch) {
  if (ch > 0x7e) {
    return;
  }
  if (ch < 0x20) {
    if (this->len) {
      this->count = 0;
      cmd_fun_run(this, (char *)this->buf);
      memset(this->buf, 0, this->len);
    }
  } else {
    this->buf[this->count] = ch;
    this->count++;
  }
  if (this->count >= this->len) {
    this->count = 0;
    memset(this->buf, 0, this->len);
  }
}

static char *skip(char *buf) {
  while (*buf == ' ') {
    buf++;
  }
  return buf;
}

static void cmd_fun_run(struct cmd *const this, char *buf) {
  u16 i, len;
  char *s;
  for (i = 0; i < this->table_len; i++) {
    len = strlen(this->table[i].name);
    if (strncmp(buf, this->table[i].name, len) == 0) {
      s = skip(buf + len);
      if (this->table[i].fun != NULL) {
        this->table[i].fun(s);
      }
    }
  }
}
