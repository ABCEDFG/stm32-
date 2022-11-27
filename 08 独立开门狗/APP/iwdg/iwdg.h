#ifndef _iwdg_H
#define _iwdg_H

#include <system.h>

void Iwdg_Init(u8 pre,u16 rlr);
void Iwdg_FeedDog(void);

#endif
