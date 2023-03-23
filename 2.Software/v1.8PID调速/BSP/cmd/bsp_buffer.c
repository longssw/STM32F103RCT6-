/*
 * buffer.c:  ���黺����
 *
 * Description:  �����黺�����Ժ���������֮ǰ�洢����������
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bsp_buffer.h"

struct buffer *buffer_create(u32 const len)
{
    struct buffer *this = (struct buffer *)malloc(sizeof(struct buffer));
    if (NULL == this)
        return NULL;

    this->buf = (u8 *)malloc((sizeof(u8) * len));
    if (NULL == this->buf)
        return NULL;

    this->len = len;
    this->in = 0;
    this->out = 0;

    memset(this->buf, 0, this->len);
    return this;
}

void buffer_destroy(struct buffer *const this)
{
    free(this->buf);
    free(this);
}

void buffer_input(struct buffer *this, const u8 data)
{
    this->buf[this->in++] = data;
    if (this->in == this->len)
    {
        this->in = 0;
    }
}

u8 buffer_output(struct buffer *this, u8 *data)
{
    if (this->in != this->out)
    {
        *data = this->buf[this->out++];
        if (this->out == this->len)
        {
            this->out = 0;
        }
        return 0;
    }
    return 1;
}

void buffer_staic_create(struct buffer *this,u8 buf[],const u16 buf_size)
{
    this->in = 0;
    this->out = 0;
    this->buf = buf;
    this->len = buf_size;
}
