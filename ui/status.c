/* Copyright 2023 Dual Tachyon
 * https://github.com/DualTachyon
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *     Unless required by applicable law or agreed to in writing, software
 *     distributed under the License is distributed on an "AS IS" BASIS,
 *     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *     See the License for the specific language governing permissions and
 *     limitations under the License.
 */

#include "battery.h"
#include <string.h>
#if defined(ENABLE_FMRADIO)
#include "app/fm.h"
#endif
#ifdef ENABLE_MESSENGER
	#include "app/messenger.h"
#endif
#include "../bitmaps.h"
#include "../driver/keyboard.h"
#include "../driver/st7565.h"
#include "../external/printf/printf.h"
#include "../functions.h"
#include "../helper/battery.h"
#include "../misc.h"
#include "../settings.h"
#include "../ui/helper.h"
#include "status.h"


void UI_DisplayStatus(void) {

  // memset(gStatusLine, 64, sizeof(gStatusLine));
  memset(gStatusLine, 0, sizeof(gStatusLine));

  if (gBatteryDisplayLevel < 2) {
    if (gLowBatteryBlink == 1) {
      UI_DisplayBattery(1);
    }
  } else {
    UI_DisplayBattery(gBatteryDisplayLevel);
  }

#ifdef ENABLE_MESSENGER
	#ifdef ENABLE_STATUS_BATTERY_PERC
		const uint8_t MSG_ICO_POS = 85;
	#else
		const uint8_t MSG_ICO_POS = 105;
	#endif
	if (hasNewMessage) { // New Message indicator	
    gStatusLine[MSG_ICO_POS + 0] |= 0b00000000;
    gStatusLine[MSG_ICO_POS + 1] |= 0b00111111;
    gStatusLine[MSG_ICO_POS + 2] |= 0b00100011;
    gStatusLine[MSG_ICO_POS + 3] |= 0b00100101;
    gStatusLine[MSG_ICO_POS + 4] |= 0b00101001;
    gStatusLine[MSG_ICO_POS + 5] |= 0b00100101;
    gStatusLine[MSG_ICO_POS + 6] |= 0b00100011;
    gStatusLine[MSG_ICO_POS + 7] |= 0b00111111;
	}
#endif

  bool isPowerSave = gCurrentFunction == FUNCTION_POWER_SAVE;
  bool isKeyLock = gEeprom.KEY_LOCK;
  bool isFPressed = gWasFKeyPressed;
  bool isVox = gEeprom.VOX_SWITCH;
  bool isWx = gEeprom.CROSS_BAND_RX_TX != CROSS_BAND_OFF;
  bool isDw = gEeprom.DUAL_WATCH != DUAL_WATCH_OFF;
#if defined(ENABLE_FMRADIO)
  bool isFm = gFmRadioMode;
#else
  bool isFm = false;
#endif

 char String[32];
#ifdef ENABLE_STATUS_BATTERY_PERC
 char Perc[4];
 char Format[32];
 sprintf(Perc, "%u%%", BATTERY_VoltsToPercent(gBatteryVoltageAverage));
  if (strlen(Perc) == 3) { 
    strcpy(Format, "%s %s %s %s %s %s %s       %u%%"); //7s +1 "_00%"
  } else if (strlen(Perc) == 2) { 
    strcpy(Format, "%s %s %s %s %s %s %s        %u%%"); //8s +2 "__0%"
  } else {
    strcpy(Format, "%s %s %s %s %s %s %s      %u%%"); //6s "000%"
  }
    sprintf(String, Format,
#else 
  sprintf(String, "%s %s %s %s %s %s %s",
#endif
          isPowerSave ? "S" : " ", //
          isKeyLock ? "L" : " ",   //
          isFPressed ? "F" : " ",  //
          isVox ? "VOX" : "   ",   //
          isWx ? "WX" : "  ",      //
          isDw ? "DW" : "  ",      //
#ifdef ENABLE_STATUS_BATTERY_PERC
          isFm ? "FM" : "  ",       //
          BATTERY_VoltsToPercent(gBatteryVoltageAverage)
#else
          isFm ? "FM" : "  "       //
#endif		  
  );
  UI_PrintStringSmallest(String, 0, 0, true, true);

  ST7565_BlitStatusLine();
}