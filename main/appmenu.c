#include "appmenu.h"
#include "../ui/welcome.h"
#ifdef ENABLE_MESSENGER
	#include "../apps/messenger.h"
  #include "frequencies.h"
  #include "external/printf/printf.h"
  #include "settings.h"
#endif
void APPMENU_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld) {
  switch (Key) {
  case KEY_EXIT:
    gRequestDisplayScreen = DISPLAY_MAIN;
    break;
  case KEY_UP:
    APPMENU_move(false);
    break;
  case KEY_DOWN:
    APPMENU_move(true);
    break;
  case KEY_PTT:
  case KEY_MENU:
    switch (APPMENU_cursor) {
      case 0:
      case 1:
      case 2:
      case 3:
      default:
        break;
    }
    gRequestDisplayScreen = DISPLAY_MENU;
	  break;

  default:
    break;
  }
}