
#ifdef ENABLE_MESSENGER

#include <string.h>

#include <string.h>
#include "apps/messenger.h"
#include "driver/st7565.h"
#include "external/printf/printf.h"
#include "misc.h"
#include "settings.h"
#include "ui/messenger.h"
#include "ui/helper.h"
#include "ui/inputbox.h"
#include "ui/ui.h"

void UI_DisplayMSG(void) {
	
	static char String[37];

	memset(gFrameBuffer, 0, sizeof(gFrameBuffer));
	memset(String, 0, sizeof(String));
#ifdef ENABLE_MESSENGER_MORE_ONE_LINE
	//UI_PrintStringSmallBold("MESSENGER", 0, 127, 0);
	//UI_PrintStringSmall("Messenger", 1, 127, 0);

	//UI_DrawDottedLineBuffer(gFrameBuffer, 2, 3, 26, 3, true, 2);
	//UI_DrawDottedLineBuffer(gFrameBuffer, 100, 3, 126, 3, true, 2);
#else
	UI_PrintStringSmallBold("MESSENGER", 0, 127, 0);
	//UI_PrintStringSmall("Messenger", 1, 127, 0);

	UI_DrawDottedLineBuffer(gFrameBuffer, 2, 3, 26, 3, true, 2);
	UI_DrawDottedLineBuffer(gFrameBuffer, 100, 3, 126, 3, true, 2);	
#endif	
	/*if ( msgStatus == SENDING ) {
		GUI_DisplaySmallest("SENDING", 100, 6, false, true);
	} else if ( msgStatus == RECEIVING ) {
		GUI_DisplaySmallest("RECEIVING", 100, 6, false, true);
	} else {
		GUI_DisplaySmallest("READY", 100, 6, false, true);
	}*/

	// RX Screen

	//GUI_DisplaySmallest("RX", 4, 34, false, true);

	memset(String, 0, sizeof(String));
#ifdef ENABLE_MESSENGER_MORE_ONE_LINE
	uint8_t mPos = 1;//8;
	const uint8_t mLine = 7;
	for (int i = 0; i < 5; ++i) {
#else
uint8_t mPos = 8;
	const uint8_t mLine = 7;
	for (int i = 0; i < 4; ++i) {
#endif		
		sprintf(String, "%s", rxMessage[i]);
		GUI_DisplaySmallest(String, 2, mPos, false, true);
		mPos += mLine;
    }

	// TX Screen
#if defined (ENABLE_MESSENGER_SHOW_RX_FREQ) && ! defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ)
	UI_DrawDottedLineBuffer(gFrameBuffer, 80, 40, 126, 40, true, 4);
#elif defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ) && ! defined (ENABLE_MESSENGER_SHOW_RX_FREQ) || defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ) && defined (ENABLE_MESSENGER_SHOW_RX_FREQ)
//	UI_DrawDottedLineBuffer(gFrameBuffer, 80, 40, 126, 40, true, 4);
#else
	UI_DrawDottedLineBuffer(gFrameBuffer, 14, 40, 126, 40, true, 4);
#endif
	memset(String, 0, sizeof(String));
	if ( keyboardType == NUMERIC ) {
		strcpy(String, "2");
	} else if ( keyboardType == UPPERCASE ) {		
		strcpy(String, "B");
	} else {		
		strcpy(String, "b");
	}

	UI_DrawRectangleBuffer(gFrameBuffer, 2, 36, 10, 44, true);
	GUI_DisplaySmallest(String, 5, 38, false, true);
#if defined (ENABLE_MESSENGER_SHOW_RX_FREQ) || defined (ENABLE_MESSENGER_SHOW_RX_TX_FREQ)
	GUI_DisplaySmallest(msgFreqInfo, 20, 38, false, true);	
#endif
	memset(String, 0, sizeof(String));
	sprintf(String, "%s_", cMessage);
	//UI_PrintStringSmall(String, 3, 0, 6);
	GUI_DisplaySmallest(String, 5, 48, false, true);

	// debug msg
	/*memset(String, 0, sizeof(String));
	sprintf(String, "S:%u", gErrorsDuringMSG);
	GUI_DisplaySmallest(String, 4, 12, false, true);

	memset(String, 0, sizeof(String));
	for (uint8_t i = 0; i < 19; i++) {
		sprintf(&String[i*2], "%02X", rxMessage[i]);
	}
	
	GUI_DisplaySmallest(String, 20, 34, false, true);*/

	ST7565_BlitFullScreen();
}

#endif