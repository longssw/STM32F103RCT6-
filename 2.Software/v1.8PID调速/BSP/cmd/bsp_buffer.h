#ifndef __BSP_BUFFER_H
#define __BSP_BUFFER_H

#include "stm32f10x.h"

struct buffer
{
    u16 in;
    u16 out;
    u8 *buf;
    u32 len;
};

typedef struct buffer buffer_t;

buffer_t *buffer_create(u32 const buf_size);
void buffer_staic_create(struct buffer *this,u8 buf[],const u16 buf_size);
void buffer_destroy(buffer_t *const pthis);
void buffer_input(buffer_t *pthis, const u8 data);
u8 buffer_output(buffer_t *pthis, u8 *data);

#endif /* __BSP_BUFFER_H */
