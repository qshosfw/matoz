#include "appmenu.h"
#include "../helper/measurements.h"
#include "../misc.h"
#include "../ui/ui.h"
#include <stdio.h>

uint16_t APPMENU_cursor;

void APPMENU_move(bool down) {
  if (down) {
      if (APPMENU_cursor == 3) {
        APPMENU_cursor = 0;
      } else {
        APPMENU_cursor++;
      }
  } else {
      if (APPMENU_cursor == 0) {
        APPMENU_cursor = 3;
      } else {
        APPMENU_cursor--;
      }
  }
    gUpdateDisplay = true;
}

void UI_DisplayAppMenu() {
  char String[32];
  char itemName[16];
  memset(gFrameBuffer, 0, sizeof(gFrameBuffer));

  const uint8_t count = 200;
  const uint8_t perScreen = 7;
  const uint8_t offset = Clamp(APPMENU_cursor - 2, 0, count - perScreen);
  for (uint8_t i = 0; i < perScreen; ++i) {
    uint8_t itemIndex = i + offset;
    uint8_t y = i * 8 + 1;
    uint8_t *pLine = gFrameBuffer[i];

    bool isCurrent = APPMENU_cursor == itemIndex;

    if (isCurrent) {
      memset(pLine, 127, LCD_WIDTH);
    }

    switch (itemIndex) {
      case 0:
        sprintf(itemName, "SETTINGS");
        break;
      case 1:
        sprintf(itemName, "Edit Scanlist");
        break;
      case 2:
        sprintf(itemName, "Messenger");
        break;
      case 3:
        sprintf(itemName, "Info");
        break;
      default:
        sprintf(itemName, "?");
        break;
    }


    sprintf(String, "%s", itemName);

    UI_PrintStringSmallest(String, 1, y, false, !isCurrent);
  }

  ST7565_BlitFullScreen();
  gUpdateDisplay = true;

}

