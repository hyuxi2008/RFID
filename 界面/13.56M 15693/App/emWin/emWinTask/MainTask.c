/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "BSP_ST16C554.h"
#include "BSP_Delay.h"
#include "MESSAGEBOX.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

uint8_t chuankou[20] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x08, 0x01, 0x31, 0x38};
uint8_t xunka[20] = {0xAA, 0xBB, 0x05, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10};
uint8_t duka[20] = {0xaa, 0xbb, 0x10, 0x00, 0x00, 0x00, 0x05, 0x10, 0x02};
uint8_t xieka[20] = {0xaa, 0xbb, 0x13, 0x00, 0x00, 0x00, 0x06, 0x10, 0x02};

volatile uint8_t drbuf[100]= {0x00};
volatile uint8_t rxflag = 0;
volatile uint8_t len = 0;
volatile uint8_t dataLen = 0;
volatile int recSta = 0x01;

int dizhi = 0;
int UID = 0;
uint8_t Kahao[10];
uint8_t Shuju[5];

int flag1 = 0;//15693
int flag2 = 0;//寻卡
int flag3 = 0;//读卡
int flag4 = 0;//写卡

int Min(int a, int b)
{
    return a > b ? b : a;
}

char tran(uint16_t n)
{
    char ans;
    if(n > 9)
        ans = n % 9 +'A' - 1;
    else
        ans = n + '0' - 0;
    return ans;
}

uint8_t trannum(char c)
{
    if(c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return c - '0';
}

void _15693()
{
    uint8_t i;
    for(i = 0; i < 10; i++)
    {
        BSP_ST16C554_CS1D_Write(chuankou[i]);
        BSP_Delay_ms(1);
    }
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00) flag1 = 1;
    else flag1 = 2;
    rxflag = 0;
}

void Xunka()
{
    uint8_t i;
    for(i = 0; i < 9; i++)//选卡
    {
        BSP_ST16C554_CS1D_Write(xunka[i]);
        BSP_Delay_ms(1);
    }
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)flag2 = 1;
    else flag2 = 2;
    rxflag = 0;
}

void Duka()
{
    uint8_t i, cnt;
    cnt = 0;
    for(i = 0; i < 9; i++)
    {
        BSP_ST16C554_CS1D_Write(duka[i]);
        BSP_Delay_ms(1);
        if(i >= 4 && i < 9) cnt ^= duka[i];
    }
    for(i = 0; i < 8; i++)
    {
        BSP_ST16C554_CS1D_Write(Kahao[i]);
        BSP_Delay_ms(1);
				if(Kahao[i] == 0xaa) 
				{
						BSP_ST16C554_CS1D_Write(0x00);
						BSP_Delay_ms(1);
				}
        cnt ^= Kahao[i];
    }
    BSP_ST16C554_CS1D_Write(dizhi);
    BSP_Delay_ms(1);
    cnt ^= dizhi;
    BSP_ST16C554_CS1D_Write(0x01);
    BSP_Delay_ms(1);
    cnt ^= 0x01;
    BSP_ST16C554_CS1D_Write(cnt);
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)flag3 = 1;
    else flag3 = 2;
    rxflag = 0;
}

void Xieka()
{
    uint8_t i, cnt;
    cnt = 0;
    for(i = 0; i < 9; i++)
    {
        BSP_ST16C554_CS1D_Write(xieka[i]);
        BSP_Delay_ms(1);
        if(i >= 4 && i < 9) cnt ^= xieka[i];
    }
    for(i = 0; i < 8; i++)
    {
        BSP_ST16C554_CS1D_Write(Kahao[i]);
        BSP_Delay_ms(1);
				if(Kahao[i] == 0xaa) 
				{
						BSP_ST16C554_CS1D_Write(0x00);
						BSP_Delay_ms(1);
				}
        cnt ^= Kahao[i];
    }
    BSP_ST16C554_CS1D_Write(dizhi);
    BSP_Delay_ms(1);
    cnt ^= dizhi;
    for(i = 0; i < 4; i++)
    {
        BSP_ST16C554_CS1D_Write(Shuju[i]);
        BSP_Delay_ms(1);
        if(Shuju[i] == 0xaa)
        {
            BSP_ST16C554_CS1D_Write(0x00);
            BSP_Delay_ms(1);
        }
        cnt ^= Shuju[i];
    }
    BSP_ST16C554_CS1D_Write(cnt);
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)flag4 = 1;
    else flag4 = 2;
    rxflag = 0;
}

static WM_HWIN _CreateMessageBox(const char * sMessage, const char * sCaption, int Flags, const GUI_FONT * pFont)
{
    WM_HWIN hWin;
    WM_HWIN hItem;
    GUI_RECT Rect;
    hWin = MESSAGEBOX_Create(sMessage, sCaption, Flags);
//
// Change font of message box window
//
    FRAMEWIN_SetFont(hWin, pFont);
//
// Adjust size
//
    WM_GetWindowRectEx(hWin, &Rect);
    WM_SetWindowPos(hWin, Rect.x0 - 15,
                    Rect.y0 - 40,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 30);
//
// Change font of button widget
//
    hItem = WM_GetDialogItem(hWin, GUI_ID_OK);
    BUTTON_SetFont(hItem, pFont);
//
// Adjust size of button widget
//
    WM_GetWindowRectEx(hItem, &Rect);
    WM_SetWindowPos(hItem, Rect.x0,
                    Rect.y0 + 10,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 5);
//
// Change font of text widget
//
    hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
    TEXT_SetFont(hItem, pFont);
//
// Adjust size text widget
//
    WM_GetWindowRectEx(hItem, &Rect);
    WM_SetWindowPos(hItem, Rect.x0,
                    Rect.y0,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 12);
    return hWin;
}


/*
************************************************************************************************
* 键盘对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
#define ID_WINDOW_04 (GUI_ID_USER + 0x53)
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePageKeyboard[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_04,                 260, 305, 530, 165, 0, 0x0, 0},
    { BUTTON_CreateIndirect,    "4",                 ID_BUTTON_4,                  2, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "7",                 ID_BUTTON_7,                  2, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "8",                 ID_BUTTON_8,                  93, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "9",                 ID_BUTTON_9,                  189, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "0",                 ID_BUTTON_0,                  274, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "5",                 ID_BUTTON_5,                  93, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "6",                 ID_BUTTON_6,                  189, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "Backspace",         ID_BUTTON_Backspace,          363, 104, 160, 40, 0, 0},
    { BUTTON_CreateIndirect,    "D",                 ID_BUTTON_D,                  274, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "E",                 ID_BUTTON_E,                  363, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "F",                 ID_BUTTON_F,                  453, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "1",                 ID_BUTTON_1,                  2, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "2",                 ID_BUTTON_2,                  93, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "3",                 ID_BUTTON_3,                  189, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "A",                 ID_BUTTON_A,                  274, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "B",                 ID_BUTTON_B,                  363, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "C",                 ID_BUTTON_C,                  453, 3, 70, 40, 0, 0},
};
/*********************************************************************
*
* Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
// USER END
/*********************************************************************
*
* _cbDialog
*/
static void _cbDialogPageKeyboard(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    int Pressed = 0;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hItem, 0x578b2e);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_4), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_4), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_7), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_7), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_8), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_8), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_9), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_9), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_0), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_0), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_5), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_5), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_6), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_6), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_Backspace), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_Backspace), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_D), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_D), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_E), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_E), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_F), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_F), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_1), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_1), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_2), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_2), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_3), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_3), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_A), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_A), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_B), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_B), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_C), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_C), 0);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(NCode) {
        case WM_NOTIFICATION_CLICKED:
            Pressed = 1;
        case WM_NOTIFICATION_RELEASED:
            if ((Id >= GUI_ID_USER + 10 && Id <= GUI_ID_USER + 18) || (Id >= GUI_ID_USER + 0 && Id <= GUI_ID_USER + 7)) {
                int Key;
                if (Id != GUI_ID_USER + 7) {
                    char acBuffer[10];
                    BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
                    Key = acBuffer[0];
                } else {
                    Key = GUI_KEY_BACKSPACE; /* Get the text from the array */
                }
                GUI_SendKeyMsg(Key, Pressed); /* Send a key message to the focussed window */
            }
            break;
// USER START (Optionally insert additional code for further Ids)
// USER END
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/*********************************************************************
*
* CreateWindowPage3
*/
WM_HWIN CreateWindowPageKeyboard(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePageKeyboard, GUI_COUNTOF(_aDialogCreatePageKeyboard), _cbDialogPageKeyboard,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*********************************************************************
*
*       static data
*
**********************************************************************
*/
/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    NULL,                0,                    0, 0, 800, 480, 0, 0, 0},
    { TEXT_CreateIndirect,      "标签内存显示区",    ID_TEXT_0,            8, 18, 120, 30, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_0,            330, 120, 180, 30, 0, 180},
    { TEXT_CreateIndirect,      "地址",              ID_TEXT_1,            540, 120, 50, 30, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_1,            330, 160, 180, 30, 0, 180},
    { TEXT_CreateIndirect,      "ISO15693协议",      ID_TEXT_2,            620, 18, 217, 30, 0, 0},
    { BUTTON_CreateIndirect,    "ISO15693",          ID_BUTTON_15693,      250, 80, 150, 30, 0, 0},
    { BUTTON_CreateIndirect,    "写卡",          		 ID_BUTTON_xieka,      670, 250, 110, 40, 0, 0},
    { BUTTON_CreateIndirect,    "寻卡",              ID_BUTTON_xunka,      330, 250, 180, 40, 0, 0},
    { BUTTON_CreateIndirect,    "读卡",          		 ID_BUTTON_duka,       540, 250, 110, 40, 0, 0},
    { TEXT_CreateIndirect,      "数据",              ID_TEXT_3,            540, 200, 50, 30, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_0,        600, 120, 180, 30, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_2,            330, 200, 180, 30, 0, 180},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_3,            600, 200, 180, 30, 0, 180},
    { RADIO_CreateIndirect,      NULL,               ID_RADIO_0,           270, 128, 40, 120, 0, 0x2803, 0},
    { LISTVIEW_CreateIndirect,  "Listview",             GUI_ID_LISTVIEW0,           8, 55, 230, 420, 0, 0x0, 0 },
};

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    uint8_t i;
    SCROLLBAR_Handle hScroll;
    char temp[5];
    WM_HWIN hWin = pMsg->hWin;

    GUI_UC_SetEncodeUTF8();
    //
    //WINDOW
    //
    WINDOW_SetBkColor(hWin,0x578b2e);
    //
    //ID_TEXT_0
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin,ID_TEXT_0),0x578b2e);
    TEXT_SetTextColor(WM_GetDialogItem(hWin,ID_TEXT_0),0xffffff);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,ID_TEXT_0),GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin,ID_TEXT_0),&GUI_FontFontSong);
    //
    //ID_EDIT_0
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_0),GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_0), &GUI_Font16B_ASCII);
    EDIT_SetFocussable(WM_GetDialogItem(hWin, ID_EDIT_0), 0);
    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_0),"");
    //
    //ID_TEXT_1
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin,ID_TEXT_1),0x578b2e);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,ID_TEXT_1),GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin,ID_TEXT_1),&GUI_FontFontSong);
    //
    //ID_EDIT_1
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_1),GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_1), &GUI_Font16B_ASCII);
    EDIT_SetFocussable(WM_GetDialogItem(hWin, ID_EDIT_1), 0);
    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_1),"");
    //
    //ID_TEXT_2
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin,ID_TEXT_2),0x578b2e);
    TEXT_SetTextColor(WM_GetDialogItem(hWin,ID_TEXT_2),0x8b0000);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,ID_TEXT_2),GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin,ID_TEXT_2),&GUI_FontFontSong);
    //
    //ID_TEXT_3
    //
    TEXT_SetTextAlign(WM_GetDialogItem(hWin,ID_TEXT_3),GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin,ID_TEXT_3),&GUI_FontFontSong);
//
    //
    //ID_DROPDOWN_0
    //
//        DROPDOWN_SetAutoScroll(WM_GetDialogItem(hWin,ID_DROPDOWN_0), 1);
//				DROPDOWN_SetScrollbarWidth(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 16);
    DROPDOWN_SetTextAlign(WM_GetDialogItem(hWin,ID_DROPDOWN_0), GUI_TA_HCENTER | GUI_TA_VCENTER);
    DROPDOWN_SetTextHeight(WM_GetDialogItem(hWin,ID_DROPDOWN_0), 34);
    DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 85);
    DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), GUI_FONT_16B_ASCII);
    DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "00");
    DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "01");
    DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "02");
    DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "03");
    DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "04");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "05");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "06");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "07");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "08");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "09");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0A");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0B");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0C");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0D");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0E");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0F");
//				DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "10");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "11");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "12");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "13");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "14");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "15");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "16");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "17");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "18");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "19");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1A");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1B");
    //
    //ID_EDIT_2
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_2),GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_2), &GUI_Font16B_ASCII);
    EDIT_SetFocussable(WM_GetDialogItem(hWin, ID_EDIT_2), 0);
    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_2),"");
    //
    //ID_EDIT_3
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_3),GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_3), &GUI_Font16B_ASCII);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, ID_EDIT_3), 8);
    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_3),"");
    //
    //BUTTON
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin,ID_BUTTON_15693),&GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin,ID_BUTTON_xieka),&GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin,ID_BUTTON_xunka),&GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin,ID_BUTTON_duka),&GUI_FontFontSong);

    RADIO_SetText(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0), "UID", 0);
    RADIO_SetText(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0), "UID", 1);
    RADIO_SetText(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0), "UID", 2);
    RADIO_SetFont(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0), &GUI_Font16B_ASCII);

    HEADER_Handle hHeader;

    WM_HWIN hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
    hHeader = LISTVIEW_GetHeader(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
    HEADER_SetFont(hHeader, &GUI_Font16B_ASCII);
    LISTVIEW_SetFont(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), &GUI_Font16B_ASCII);
    LISTVIEW_SetHeaderHeight(hItem, 40);
    LISTVIEW_SetRowHeight(hItem, 40);
    LISTVIEW_AddColumn(hItem, 40, "", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "00", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "01", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "02", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "03", GUI_TA_HCENTER | GUI_TA_VCENTER);
    for(i = 0; i < 28; i++)
        LISTVIEW_AddRow(hItem, NULL);
    for(i = 0; i < 28; i++)
    {
        temp[0] = tran(i/16);
        temp[1] = tran(i%16);
        temp[2] = '\0';
        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 0, i, temp);
        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 1, i, "00");
        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, i, "00");
        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 3, i, "00");
        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 4, i, "00");
    }
    LISTVIEW_SetGridVis(hItem, 1);
    //LISTVIEW_SetAutoScrollV(hItem, 1);
    hScroll = SCROLLBAR_CreateAttached(hItem, GUI_ID_VSCROLL);
    SCROLLBAR_SetWidth(hScroll, 15);

}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    uint8_t i, cnt;
    char temp[20], buf[40], tp[5];
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case ID_DROPDOWN_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                dizhi = DROPDOWN_GetSel(WM_GetDialogItem(hWin, ID_DROPDOWN_0));
                break;
            }
            break;
        case ID_RADIO_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                UID = RADIO_GetValue(WM_GetDialogItem(hWin, ID_RADIO_0));
                break;
            }
            break;
        case ID_BUTTON_15693://选择15693
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                _15693();
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xunka:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                Xunka();
                if(flag2 == 1)
                {
                    cnt = 0;
                    for(i = 10; i < 18; i++)//10+drbuf[2]-7
                    {
                        temp[cnt++] = tran(drbuf[i]/16);
                        temp[cnt++] = tran(drbuf[i]%16);
                    }
                    temp[cnt] = '\0';
                    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_0), GUI_TA_VCENTER | GUI_TA_LEFT);
                    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_0), &GUI_Font16B_ASCII);
                    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_0),temp);
                    cnt = 0;
                    for(i = 19; i < Min(27, 3+drbuf[2]); i++)//10+drbuf[2]-7
                    {
                        temp[cnt++] = tran(drbuf[i]/16);
                        temp[cnt++] = tran(drbuf[i]%16);
                    }
                    temp[cnt] = '\0';
                    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_1), GUI_TA_VCENTER | GUI_TA_LEFT);
                    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_1), &GUI_Font16B_ASCII);
                    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_1),temp);
                    cnt = 0;
                    for(i = 28; i < Min(36, 3+drbuf[2]); i++)//10+drbuf[2]-7
                    {
                        temp[cnt++] = tran(drbuf[i]/16);
                        temp[cnt++] = tran(drbuf[i]%16);
                    }
                    temp[cnt] = '\0';
                    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_2), GUI_TA_VCENTER | GUI_TA_LEFT);
                    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_2), &GUI_Font16B_ASCII);
                    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_2),temp);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_duka:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                EDIT_GetText(WM_GetDialogItem(hWin,GUI_ID_USER+34+UID), buf, sizeof(buf));
                for(i = 0; i < 8; i++)
                    Kahao[i] = trannum(buf[i*2]) * 16 + trannum(buf[i*2+1]);
                Duka();
                if(flag3 == 1)
                {
                    cnt = 0;
                    for(i = 9; i < 13; i++)
                    {
                        temp[cnt++] = tran(drbuf[i]/16);
                        temp[cnt++] = tran(drbuf[i]%16);
                    }
                    temp[cnt] = '\0';
                    for(i = 0; i < 4; i++)
                    {
                        tp[0] = temp[i*2];
                        tp[1] = temp[i*2+1];
                        tp[2] = '\0';
                        LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), i+1, dizhi, tp);
                    }
                    EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_3), GUI_TA_VCENTER | GUI_TA_LEFT);
                    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_3), &GUI_Font16B_ASCII);
                    EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_3),temp);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xieka:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                EDIT_GetText(WM_GetDialogItem(hWin, GUI_ID_USER+34+UID), buf, sizeof(buf));
                for(i = 0; i < 8; i++)
                    Kahao[i] = trannum(buf[i*2]) * 16 + trannum(buf[i*2+1]);
                EDIT_GetText(WM_GetDialogItem(hWin, ID_EDIT_3), buf, sizeof(buf));
                for(i = 0; i < 4; i++)
                    Shuju[i] = trannum(buf[i*2]) * 16 + trannum(buf[i*2+1]);
                Xieka();
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}


WM_HWIN CreateFramewin0(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
    return hWin;
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void MainTask(void)
{
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    CreateFramewin0();
    CreateWindowPageKeyboard();
    while(1)
    {
        //15693模式选择
        if(flag1 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("15693 OK!", "", 0, &GUI_Font16B_ASCII));
            flag1 = 0;
        }
        else if(flag1 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("15693 failed!", "", 0, &GUI_Font16B_ASCII));
            flag1 = 0;
        }

        //寻卡
        if(flag2 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag2 = 0;
        }
        else if(flag2 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag2 = 0;
        }

        //读卡
        if(flag3 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag3 = 0;
        }
        else if(flag3 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag3 = 0;
        }

        //写卡
        if(flag4 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag4 = 0;
        }
        else if(flag4 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag4 = 0;
        }

        BSP_Delay_ms(100);
        GUI_Delay(1);
    }
}

