/*****************************************************************************
 * copyleft (c) 2023 �� ˼ ά
 *
 * �� �� ��:   bsp_cmd.c
 * ��    ��:   ����̨�ַ�����������(���ڵ��ι���)
 * ��    ��:   ��˼ά
 * ��    ��:   2023-03-03 09:51:31
 * ��    ��:   v1.0.0
 * ---------------------------------------------------------------------------
 * �ļ��޸���ʷ:
 *    <time>    |  <version>  |  <author>  |  <description>
 *  2023-02-28  |   v1.0.0    |    ssw     |     �����ļ�
 *  2023-03-3   |   v1.0.1    |    ssw     |     �޸�BUG
 * ---------------------------------------------------------------------------
 ******************************************************************************/
/*================================ ͷ �� �� ==================================*/

#include "bsp_cmd.h"/* "stm32f10x.h" */

//#include <stdio.h>/* printf */
#include <stdlib.h>/* ��̬�ڴ� */
#include <string.h>/* �ַ������� */
/*============================== ˽ �� �� �� ================================*/
static char *skip(char *buf);
static void cmd_fun_run(struct cmd *const this, char *buf);
/*=========================== �� �� �� / �� �� �� ============================*/
/*=============================== �� �� �� =================================*/
/*============================== ȫ �� �� �� ================================*/
/*============================== �� �� �� �� ================================*/
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
