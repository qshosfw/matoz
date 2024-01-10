#include "appmenu.h"
#include "helper/measurements.h"
#include "misc.h"
#include "ui.h"
#include "welcome.h"
#include <stdio.h>

#ifdef ENABLE_MESSENGER
	#include "../apps/messenger.h"
  #include "frequencies.h"
  #include "external/printf/printf.h"
  #include "settings.h"
#endif

#if defined(ENABLE_SPECTRUM)
#include "main/spectrum.h"
#include "functions.h"
#endif

#include "ui/scanner.h"

uint16_t APPMENU_cursor;

void APPMENU_move(bool down) {
  if (down) {
      if (APPMENU_cursor == 7) {
        APPMENU_cursor = 0;
      } else {
        APPMENU_cursor++;
      }
  } else {
      if (APPMENU_cursor == 0) {
        APPMENU_cursor = 7;
      } else {
        APPMENU_cursor--;
      }
  }
    gUpdateDisplay = true;
}

void APPMENU_close() {
      gRequestDisplayScreen = DISPLAY_MAIN;
}

void UI_DisplayAppMenu() {
  char String[32];
  char itemName[16];
  memset(gFrameBuffer, 0, sizeof(gFrameBuffer));

  const uint8_t count = 8;
  const uint8_t perScreen = 7;;
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
        sprintf(itemName, "Settings");
        break;
      case 1:
        sprintf(itemName, "Edit Scanlist");
        break;
      case 2:
        sprintf(itemName, "Messenger");
        break;
      case 3:
        sprintf(itemName, "Spectrum");
        break;
      case 4:
        sprintf(itemName, "FM Radio");
        break;
      case 5:
        sprintf(itemName, "Air Copy");
        break;
      case 6:
        sprintf(itemName, "Scanner");
        break;
      case 7:
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


void APPMENU_ProcessKeys(KEY_Code_t Key, bool bKeyPressed, bool bKeyHeld) {
  if (!bKeyPressed) return;
  switch (Key) {
  case KEY_EXIT:
    APPMENU_close();
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
       gRequestDisplayScreen = DISPLAY_MENU;
       break;
      case 1:
       gAppToDisplay = APP_SCANLIST;
       gRequestDisplayScreen = DISPLAY_MAIN;
       break;
#ifdef ENABLE_MESSENGER
#if defined (ENABLE_MESSENGER_SHOW_RX_FREQ) && !defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ)
      case 2:
    hasNewMessage = false;
    uint32_t frequency = gEeprom.VfoInfo[gEeprom.TX_CHANNEL].pTX->Frequency;
    if ( IsTXAllowed(gEeprom.VfoInfo[gEeprom.TX_CHANNEL].pTX->Frequency) ) {
      frequency = GetScreenF(frequency);
      sprintf(msgFreqInfo, "%u.%05u Mhz", (unsigned int) frequency / 100000, (unsigned int) frequency % 100000);
    } else {
      sprintf(msgFreqInfo, "TX DISABLE");
    }
    gUpdateStatus = true;
    gAppToDisplay = APP_MESSENGER;
	  gRequestDisplayScreen = DISPLAY_MAIN;
    break;
#elif defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ) && !defined (ENABLE_MESSENGER_SHOW_RX_FREQ) || defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ) && defined (ENABLE_MESSENGER_SHOW_RX_FREQ)
      case 2:
    hasNewMessage = false;
    uint32_t txFrequency = gEeprom.VfoInfo[gEeprom.TX_CHANNEL].pTX->Frequency;
    uint32_t rxFrequency = gEeprom.VfoInfo[gEeprom.RX_CHANNEL].pRX->Frequency;
    
    if (IsTXAllowed(txFrequency)) {
      txFrequency = GetScreenF(txFrequency);
      rxFrequency = GetScreenF(rxFrequency);
      sprintf(msgFreqInfo, "R:%u.%05u T:%u.%05u Mhz", (unsigned int) rxFrequency / 100000, (unsigned int) rxFrequency % 100000, (unsigned int) xFrequency / 100000, (unsigned int) txFrequency % 100000);
    } else {
      sprintf(msgFreqInfo, "TX DISABLE");
    }

    gUpdateStatus = true;
    gAppToDisplay = APP_MESSENGER;
    gRequestDisplayScreen = DISPLAY_MAIN;
    break;
#else
  case 2:
    hasNewMessage = false;    
    gUpdateStatus = true;
    gAppToDisplay = APP_MESSENGER;
    gRequestDisplayScreen = DISPLAY_MAIN;    
    break;
#endif	
#endif
      case 3:
        gCurrentFunction = 0;
        APP_RunSpectrum();
      break;
      case 4:
        #if defined(ENABLE_FMRADIO)
		    UI_DisplayFM();
        #endif
      break;
      case 5:
        #if defined(ENABLE_AIRCOPY)
		    UI_DisplayAircopy();
        #endif
      break;
      case 6:
       UI_DisplayScanner();
       break;
      case 7:
      	UI_DisplayWelcome();
        break;
      default:
        break;
    }
	  break;

  default:
    break;
  }
}

