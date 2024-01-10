#ifndef UI_APPMENU_H
#define UI_APPMENU_H

#include "../driver/keyboard.h"
#include "../driver/st7565.h"
#include "../external/printf/printf.h"
#include "../ui/helper.h"
#include "../misc.h"
#include "helper.h"
#include <string.h>

uint16_t extern APPMENU_cursor;
void UI_DisplayAppMenu();
void APPMENU_move(bool down);
void APPMENU_update(void);
void APPMENU_render(void);
void APPMENU_close(void);
void APPMENU_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld);


#endif /* end of include guard: UI_APPMENU_H */
