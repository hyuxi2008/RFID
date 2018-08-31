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

#define ID_WINDOW_01 (GUI_ID_USER + 0x41)
#define ID_WINDOW_02 (GUI_ID_USER + 0x43)
#define ID_WINDOW_03 (GUI_ID_USER + 0x42)

WM_HWIN hWin0, hWin1, hWin2, hWin3;

static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage1[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_01,          8, 300, 780, 165, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "防碰撞识别",             ID_TEXT_3,             45, 50, 80, 40, 0, 0},
    { TEXT_CreateIndirect,      "防碰撞识别Q值：",           ID_TEXT_4,             200, 50, 120, 40, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_0,          330, 50, 120, 35, 0, 0},
    { BUTTON_CreateIndirect,    "内存窗口",         		 ID_BUTTON_neicunchuangkou,    660, 20, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "清除信息",          		 ID_BUTTON_qingchuxinxi,      540, 20, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "识别标签",          	 ID_BUTTON_shibiebiaoqian,        540, 75, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "单步识别",          	 ID_BUTTON_danbushibie,        540, 115, 100, 35, 0, 0},
    { CHECKBOX_CreateIndirect,  "Checkbox",            ID_CHECKBOX_0,    20, 61, 20, 20, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage2[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_02,          8, 300, 780, 165, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "标签ID：",             ID_TEXT_3,            15, 15, 70, 40, 0, 0},
    { TEXT_CreateIndirect,      "不指定UII",           ID_TEXT_4,            420, 15, 70, 40, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_1,         85, 75, 150, 40, DROPDOWN_CF_UP | DROPDOWN_CF_AUTOSCROLLBAR, 0},
    { TEXT_CreateIndirect,       "数据块：",               ID_TEXT_7,            15, 75, 70, 40, 0, 0},
    { TEXT_CreateIndirect,      "数据：",               ID_TEXT_8,             290, 75, 50, 40, 0, 0},
    { TEXT_CreateIndirect,      "地址：",               ID_TEXT_9,             15, 115, 50, 40, 0, 0},
    { TEXT_CreateIndirect,      "Dec",               ID_TEXT_10,            240, 115, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "长度：",               ID_TEXT_5,             290, 115, 50, 40, 0, 0},
    { TEXT_CreateIndirect,      "Dec",               ID_TEXT_6,            505, 115, 30, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_8,             85, 15, 280, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_9,             350, 75, 150, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_10,            85, 115, 150, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_11,            350, 115, 150, 40, 0, 0},
    { BUTTON_CreateIndirect,    "寻卡窗口",          		 ID_BUTTON_xunkachuangkou,     660, 20, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "清除信息",          		 ID_BUTTON_qingchuxinxi,      540, 20, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "读取数据",          		 ID_BUTTON_duqushuju,      540, 75, 100, 35, 0, 0},
    { BUTTON_CreateIndirect,    "写入数据",          		 ID_BUTTON_xierushuju,      540, 115, 100, 35, 0, 0},
    { CHECKBOX_CreateIndirect,  "Checkbox",            ID_CHECKBOX_1,    395, 26, 20, 20, 0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage3[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_03,          548, 375, 100, 35, 0, 0x0, 0},
    { BUTTON_CreateIndirect,    "停止",          		 ID_BUTTON_tingzhi,     0, 0, 100, 35, 0, 0},
};

static void _cbDialogPage1(WM_MESSAGE * pMsg);
static void _cbDialogPage2(WM_MESSAGE * pMsg);
static void _cbDialogPage3(WM_MESSAGE * pMsg);
WM_HWIN CreateWindowPage1();
WM_HWIN CreateWindowPage2();
WM_HWIN CreateWindowPage3();

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

uint8_t xunka[20] = {0xAA, 0x02, 0x18, 0x55};
uint8_t fangpengzhuang[20] = {0xaa, 0x03, 0x11};//防碰撞值 结束位

volatile uint8_t drbuf[105]= {0x00};
volatile uint8_t rxflag = 0;
volatile uint8_t len = 0;
volatile uint8_t dataLen = 0;
volatile int recSta = 0x01;
int flag = 0;
int flagxunka = 1;
int flagfangpengzhuang = 0;
int tingzhi = 0;
int shujukuai = 0;
int dizhi = 0;
int changdu = 0;
int flagbuzhidingUII = 0;
uint8_t Q = 0;
int ct = 0;

typedef struct node {
    char kahao[100];
    int num;
} Ka;

Ka ka[100];

char tran(uint16_t n)
{
    char ans;
    if(n > 9)
    {
        ans = n % 9 +'A' - 1;
    }
    else
    {
        ans = n + '0'-0;
    }
    return ans;
}

char prtnum[50];

void num_to_str(int n)
{
    int wei = 0;
    int temp = n;
    while(temp)
    {
        temp /= 10;
        wei++;
    }
    prtnum[wei] = '\0';
    while(n)
    {
        prtnum[--wei] = tran(n % 10);
        n /= 10;
    }
}

uint8_t char_to_num(char c)
{
    uint8_t ans;
    if(c >= 'A' && c <= 'F') ans = 10 + c - 'A';
    else ans = c - '0';
    return ans;
}

uint8_t Same(char *a, char *b, uint8_t LEN)
{
    uint8_t i;
    for(i = 0; i < LEN; i++)
        if(a[i] != b[i])
            return 0;
    return 1;
}


void printXunka()
{
    uint8_t i, LEN, cnt, flg;
    char temp[50];
    if(rxflag == 1 && drbuf[3] == 0x00)
    {
        LEN = drbuf[1] - 3;
        cnt = 0;
        flg = 0;
        for(i = 4; i < 4 + LEN; i++)
        {
            temp[cnt++] = tran(drbuf[i]/16);
            temp[cnt++] = tran(drbuf[i]%16);
        }
        temp[cnt] = '\0';
        for(i = 0; i < ct; i++)
        {
            if(Same(temp, ka[i].kahao, LEN))
            {
                ka[i].num++;
                num_to_str(ka[i].num);
                LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, i, prtnum);
                flg = 1;
            }
        }
        if(!flg)
        {
            strcpy(ka[ct].kahao, temp);
            ka[ct].num++;
            num_to_str(ct+1);
            LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 0, ct, prtnum);
            LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 1, ct, "识别");
            LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, ct, ka[ct].kahao);
            num_to_str(ka[ct].num);
            LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, ct, prtnum);
            ct++;
        }
    }
    rxflag = 0;
    for(i = 0; i < 100; i++) drbuf[i] = 0x00;
}

/*
************************************************************************************************
* 添加到多页控件第一个页面窗口的对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/

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


static void _cbDialogPage1(WM_MESSAGE * pMsg) {
    uint8_t i;
    WM_HWIN hItem;
    char pt[40];
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:

        WINDOW_SetBkColor(hItem, 0x804000);

        //
        //ID_TEXT_3
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_3), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_3), &GUI_FontFontSong);
        //
        //ID_TEXT_4
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_4), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_4), &GUI_FontFontSong);

        //ID_DROPDOWN_0
        //
//				DROPDOWN_SetAutoScroll(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 1);
//				DROPDOWN_SetScrollbarWidth(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 16);
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_0), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 70);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "3");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "4");
//				DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "5");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "6");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "7");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "8");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "9");
//				DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "10");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "11");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "12");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "13");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "14");
//				DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "15");

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_neicunchuangkou),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_qingchuxinxi),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_shibiebiaoqian),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_danbushibie),&GUI_FontFontSong);

        //
        //ID_TEXT_5
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_5),GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_5),&GUI_FontFontSong);
        //
        //ID_TEXT_6
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_6),GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_6),&GUI_FontFontSong);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_DROPDOWN_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                Q =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_0));
                break;
            }
            break;
        case ID_CHECKBOX_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                flagfangpengzhuang ^= 1;
                break;
            }
            break;
        case ID_BUTTON_neicunchuangkou:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                GUI_EndDialog(pMsg->hWin, 0);
                hWin2 = CreateWindowPage2();
                flagxunka = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_qingchuxinxi:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 30; i++)
                {
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 0, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 1, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 4, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 5, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 6, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 7, i, "");
                    ka[i].num = 0;
                }
                ct = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_shibiebiaoqian:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                hWin3 = CreateWindowPage3();
                WM_DisableWindow(WM_GetDialogItem(hItem,ID_BUTTON_qingchuxinxi));
                WM_DisableWindow(WM_GetDialogItem(hItem,ID_BUTTON_danbushibie));
                WM_DisableWindow(WM_GetDialogItem(hItem,ID_BUTTON_neicunchuangkou));
                tingzhi = 1;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_danbushibie:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(flagfangpengzhuang)
                {
                    for(i = 0; i < 3; i++)
                    {
                        BSP_ST16C554_CS2A_Write(fangpengzhuang[i]);
                        BSP_Delay_ms(1);
                    }
                    BSP_ST16C554_CS2A_Write(Q);
                    BSP_Delay_ms(1);
                    BSP_ST16C554_CS2A_Write(0x55);
                }
                else
                {
                    for(i = 0; i < 4; i++)
                    {
                        BSP_ST16C554_CS2A_Write(xunka[i]);
                        BSP_Delay_ms(1);
                    }
                }
                BSP_Delay_ms(5);
                printXunka();
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
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
* CreateWindowPage1
*/
WM_HWIN CreateWindowPage1(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage1, GUI_COUNTOF(_aDialogCreatePage1), _cbDialogPage1,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*
************************************************************************************************
* 添加到多页控件第二个页面窗口的对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/

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
static void _cbDialogPage2(WM_MESSAGE * pMsg) {
    uint8_t i, cnt, res;
    int Len;
    char buf[40];
    WM_HWIN hItem;
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:

        WINDOW_SetBkColor(hItem, 0x804000);

        //
        //ID_TEXT_3
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_3), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_3), &GUI_FontFontSong);
        //
        //ID_TEXT_4
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_4), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_4), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_7), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_7), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_8), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_8), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_9), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_9), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_10), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_10), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_5), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_5), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_6), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_6), &GUI_FontFontSong);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_8), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_8), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_8), 36);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_8), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_8), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_9), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_9), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_9), 4);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_9), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_9), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_10), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_10), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_10), 2);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_10), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_10), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_11), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_11), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_11), 2);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_11), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_11), "");

        //
        //ID_DROPDOWN_1
        //
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_1), GUI_TA_LEFT | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_1),30);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), 75);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "00:Reserved");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "01:UII");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "10:TID");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "11:User");

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_xunkachuangkou),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_qingchuxinxi),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_duqushuju),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_xierushuju),&GUI_FontFontSong);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_DROPDOWN_1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                shujukuai =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_1));
                break;
            }
            break;
        case ID_CHECKBOX_1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                flagbuzhidingUII ^= 1;
                break;
            }
            break;
        case ID_BUTTON_xunkachuangkou:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                GUI_EndDialog(pMsg->hWin, 0);
                hWin1 = CreateWindowPage1();
                flagxunka = 1;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            }
            break;
        case ID_BUTTON_qingchuxinxi:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 30; i++)
                {
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 0, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 1, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 4, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 5, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 6, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 7, i, "");
                    ka[i].num = 0;
                }
                ct = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_duqushuju:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!flagbuzhidingUII)
                {
                    EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_8), buf, sizeof(buf));
                    Len = 0;
                    while(buf[Len] != '\0') {
                        Len++;
                    }
                }
                BSP_ST16C554_CS2A_Write(0xaa);
                BSP_Delay_ms(1);
                if(!flagbuzhidingUII)
                    BSP_ST16C554_CS2A_Write(9+Len/2);
                else
                    BSP_ST16C554_CS2A_Write(9);
                BSP_Delay_ms(1);
                if(!flagbuzhidingUII)
                    BSP_ST16C554_CS2A_Write(0x13);
                else
                    BSP_ST16C554_CS2A_Write(0x20);
                BSP_Delay_ms(1);
                for(i = 0; i < 4; i++)
                {
                    BSP_ST16C554_CS2A_Write(0x00);
                    BSP_Delay_ms(1);
                }
                BSP_ST16C554_CS2A_Write(shujukuai);
                BSP_Delay_ms(1);
                EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_10), buf, sizeof(buf));
                sscanf(buf, "%d", &dizhi);
                BSP_ST16C554_CS2A_Write(dizhi);
                BSP_Delay_ms(1);
                EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_11), buf, sizeof(buf));
                sscanf(buf, "%d", &changdu);
                BSP_ST16C554_CS2A_Write(changdu);
                BSP_Delay_ms(1);
                if(!flagbuzhidingUII)
                {
                    //发卡号
                    EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_8), buf, sizeof(buf));
                    for(i = 0; i < Len/2; i++)
                    {
                        res = char_to_num(buf[i*2])*16+char_to_num(buf[i*2+1]);
                        if(res == 0xaa || res == 0x55 || res == 0xff)
                        {
                            BSP_ST16C554_CS2A_Write(0xff);
                            BSP_Delay_ms(1);
                        }
                        BSP_ST16C554_CS2A_Write(res);
                        BSP_Delay_ms(1);
                    }
                }
                BSP_ST16C554_CS2A_Write(0x55);
                BSP_Delay_ms(500);
                Len = drbuf[1];
                if(rxflag && drbuf[3] == 0x00)
                {
                    if(!flagbuzhidingUII)
                        EDIT_SetText(WM_GetDialogItem(hWin0,ID_EDIT_0), buf);
                    else
                        EDIT_SetText(WM_GetDialogItem(hWin0,ID_EDIT_0), "");
                    strcpy(ka[ct].kahao, buf);
                    num_to_str(ct+1);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 0, ct, prtnum);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 1, ct, "读取");
                    if(!flagbuzhidingUII)
                        LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, ct, ka[ct].kahao);
                    else
                        LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, ct, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, ct, "1");
                    num_to_str(shujukuai);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 4, ct, prtnum);
                    num_to_str(dizhi);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 5, ct, prtnum);
                    num_to_str(changdu);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 6, ct, prtnum);
                    cnt = 0;
                    for(i = 4; i < Len + 1; i++)
                    {
                        prtnum[cnt++] = tran(drbuf[i]/16);
                        prtnum[cnt++] = tran(drbuf[i]%16);
                    }
                    prtnum[cnt] = '\0';
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 7, ct, prtnum);
                    ct++;
                    rxflag = 0;
                    for(i = 0; i < 100; i++) drbuf[i] = 0x00;
                    BSP_Delay_ms(100);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            }
            break;
        case ID_BUTTON_xierushuju:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!flagbuzhidingUII)
                {
                    EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_8), buf, sizeof(buf));
                    Len = 0;
                    while(buf[Len] != '\0') {
                        Len++;
                    }
                }
                BSP_ST16C554_CS2A_Write(0xaa);
                BSP_Delay_ms(1);
                if(!flagbuzhidingUII)
                    BSP_ST16C554_CS2A_Write(0x0b+Len/2);
                else
                    BSP_ST16C554_CS2A_Write(0x0b);
                BSP_Delay_ms(1);
                if(!flagbuzhidingUII)
                    BSP_ST16C554_CS2A_Write(0x14);
                else
                    BSP_ST16C554_CS2A_Write(0x21);
                BSP_Delay_ms(1);
                for(i = 0; i < 4; i++)
                {
                    BSP_ST16C554_CS2A_Write(0x00);
                    BSP_Delay_ms(1);
                }
                BSP_ST16C554_CS2A_Write(shujukuai);
                BSP_Delay_ms(1);
                EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_10), buf, sizeof(buf));
                sscanf(buf, "%d", &dizhi);
                BSP_ST16C554_CS2A_Write(dizhi);
                BSP_Delay_ms(1);
                BSP_ST16C554_CS2A_Write(0x01);
                BSP_Delay_ms(1);
                EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_9), buf, sizeof(buf));
                for(i = 0; i < 2; i++)
                {
                    res = char_to_num(buf[i*2]*16) + char_to_num(buf[i*2+1]);
                    if(res == 0xaa || res == 0x55 || res == 0xff)
                    {
                        BSP_ST16C554_CS2A_Write(0xff);
                        BSP_Delay_ms(1);
                    }
                    BSP_ST16C554_CS2A_Write(res);
                    BSP_Delay_ms(1);
                }
                if(!flagbuzhidingUII)
                {
                    //发卡号
                    EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_8), buf, sizeof(buf));
                    for(i = 0; i < Len/2; i++)
                    {
                        res = char_to_num(buf[i*2])*16+char_to_num(buf[i*2+1]);
                        if(res == 0xaa || res == 0x55 || res == 0xff)
                        {
                            BSP_ST16C554_CS2A_Write(0xff);
                            BSP_Delay_ms(1);
                        }
                        BSP_ST16C554_CS2A_Write(res);
                        BSP_Delay_ms(1);
                    }
                }
                BSP_ST16C554_CS2A_Write(0x55);
                BSP_Delay_ms(500);
                Len = drbuf[1];
                if(rxflag && drbuf[3] == 0x00)
                {
                    if(!flagbuzhidingUII)
                        EDIT_SetText(WM_GetDialogItem(hWin0,ID_EDIT_0), buf);
                    else
                        EDIT_SetText(WM_GetDialogItem(hWin0,ID_EDIT_0), "");
                    strcpy(ka[ct].kahao, buf);
                    num_to_str(ct+1);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 0, ct, prtnum);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 1, ct, "写入");
                    if(!flagbuzhidingUII)
                        LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, ct, ka[ct].kahao);
                    else
                        LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 2, ct, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 3, ct, "1");
                    num_to_str(shujukuai);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 4, ct, prtnum);
                    num_to_str(dizhi);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 5, ct, prtnum);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 6, ct, "1");
                    EDIT_GetText(WM_GetDialogItem(hItem,ID_EDIT_9), buf, sizeof(buf));
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin0,GUI_ID_LISTVIEW0), 7, ct, buf);
                    ct++;
                    rxflag = 0;
                    for(i = 0; i < 100; i++) drbuf[i] = 0x00;
                    BSP_Delay_ms(100);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
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
* CreateWindowPage2
*/
WM_HWIN CreateWindowPage2(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage2, GUI_COUNTOF(_aDialogCreatePage2), _cbDialogPage2,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*
************************************************************************************************
* 停止按钮
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/

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
static void _cbDialogPage3(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:

        //WINDOW_SetBkColor(hItem, 0x804000);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_tingzhi),&GUI_FontFontSong);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_BUTTON_tingzhi:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                GUI_EndDialog(hItem, 0);
                WM_EnableWindow(WM_GetDialogItem(hWin1,ID_BUTTON_qingchuxinxi));
                WM_EnableWindow(WM_GetDialogItem(hWin1,ID_BUTTON_danbushibie));
                WM_EnableWindow(WM_GetDialogItem(hWin1,ID_BUTTON_neicunchuangkou));
                tingzhi = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                break;
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
* CreateWindowPage2
*/
WM_HWIN CreateWindowPage3(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage3, GUI_COUNTOF(_aDialogCreatePage3), _cbDialogPage3,
                               WM_HBKWIN, 0, 0);
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
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_04,                 550, 0, 780, 300, 0, 0x0, 0},
    { BUTTON_CreateIndirect,    "4",                 ID_BUTTON_4,                  2, 57, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "7",                 ID_BUTTON_7,                  2, 107, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "8",                 ID_BUTTON_8,                  83, 107, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "9",                 ID_BUTTON_9,                  169, 107, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "0",                 ID_BUTTON_0,                  2, 157, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "5",                 ID_BUTTON_5,                  83, 57, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "6",                 ID_BUTTON_6,                  169, 57, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "Backspace",         ID_BUTTON_Backspace,          83, 257, 155, 40, 0, 0},
    { BUTTON_CreateIndirect,    "D",                 ID_BUTTON_D,                  83, 207, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "E",                 ID_BUTTON_E,                  169, 207, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "F",                 ID_BUTTON_F,                  2, 257, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "1",                 ID_BUTTON_1,                  2, 8, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "2",                 ID_BUTTON_2,                  83, 8, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "3",                 ID_BUTTON_3,                  169, 8, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "A",                 ID_BUTTON_A,                  83, 157, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "B",                 ID_BUTTON_B,                  169, 157, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "C",                 ID_BUTTON_C,                  2, 207, 70, 40, 0, 0},
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

static const GUI_WIDGET_CREATE_INFO _aDialogCreate0[] = {
    { WINDOW_CreateIndirect,    NULL,                0,                      0, 0, 800, 480, 0, 0, 0},
    { EDIT_CreateIndirect,      NULL,                ID_EDIT_0,              150, 265, 390, 30, 0, 390},
    { TEXT_CreateIndirect,      "##寻卡操作##",      ID_TEXT_0,              0, 265, 150, 30, 0, 0},
    { LISTVIEW_CreateIndirect,  "Listview",          GUI_ID_LISTVIEW0,       8, 8, 535, 255, 0, 0x0, 0 },
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
    SCROLLBAR_Handle hScroll1, hScroll2;
    WM_HWIN hItem = pMsg->hWin;

    GUI_UC_SetEncodeUTF8();

    //
    //WINDOW
    //
    WINDOW_SetBkColor(hItem,0x578b2e);

    EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_0),GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_0),&GUI_Font16B_ASCII);
    EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_0), 0);
    EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_0),"");

    TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_0), GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_0), &GUI_FontFontSong);

    HEADER_Handle hHeader;

    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
    hHeader = LISTVIEW_GetHeader(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
    HEADER_SetFont(hHeader, &GUI_FontFontSong);
    LISTVIEW_SetHeaderHeight(hItem, 30);
    LISTVIEW_SetRowHeight(hItem, 30);
    LISTVIEW_AddColumn(hItem, 30, "", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "状态", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 300, "标签ID号", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "次数", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 70, "数据块", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "地址", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 50, "长度", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 300, "数据", GUI_TA_LEFT | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 1, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 2, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 3, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 4, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 5, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 6, GUI_TA_HCENTER | GUI_TA_VCENTER);
    HEADER_SetTextAlign(hHeader, 7, GUI_TA_HCENTER | GUI_TA_VCENTER);
    for(i = 0; i < 20; i++)
        LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    //LISTVIEW_SetAutoScrollV(hItem, 1);
    //LISTVIEW_SetAutoScrollH(hItem, 1);
    hScroll1 = SCROLLBAR_CreateAttached(hItem, 0);
    hScroll2 = SCROLLBAR_CreateAttached(hItem, GUI_ID_VSCROLL);
    SCROLLBAR_SetWidth(hScroll1, 15);
    SCROLLBAR_SetWidth(hScroll2, 15);
    LISTVIEW_SetFont(hItem, &GUI_FontFontSong);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbDialog0(WM_MESSAGE * pMsg)
{
    uint8_t i;
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

        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}


WM_HWIN CreateFramewin0(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate0, GUI_COUNTOF(_aDialogCreate0), _cbDialog0, WM_HBKWIN, 0, 0);
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
    uint8_t i;
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    hWin0 =CreateFramewin0();
    hWin1 = CreateWindowPage1();
    CreateWindowPageKeyboard();
    for(i = 0; i < 100; i++)
        ka[i].num = 0;
    while(1)
    {
        if(flagxunka)
        {
            TEXT_SetTextAlign(WM_GetDialogItem(hWin0,ID_TEXT_0), GUI_TA_VCENTER|GUI_TA_CENTER);
            TEXT_SetFont(WM_GetDialogItem(hWin0,ID_TEXT_0), &GUI_FontFontSong);
            TEXT_SetText(WM_GetDialogItem(hWin0,ID_TEXT_0), "##寻卡操作##");
        }
        else
        {
            TEXT_SetTextAlign(WM_GetDialogItem(hWin0,ID_TEXT_0), GUI_TA_VCENTER|GUI_TA_CENTER);
            TEXT_SetFont(WM_GetDialogItem(hWin0,ID_TEXT_0), &GUI_FontFontSong);
            TEXT_SetText(WM_GetDialogItem(hWin0,ID_TEXT_0), "##内存操作##");
        }

        if(tingzhi)
        {
            if(flagfangpengzhuang)
            {
                while(1)
                {
                    if(!tingzhi) break;
                    for(i = 0; i < 3; i++)
                    {
                        BSP_ST16C554_CS2A_Write(fangpengzhuang[i]);
                        BSP_Delay_ms(1);
                    }
                    BSP_ST16C554_CS2A_Write(Q);
                    BSP_Delay_ms(1);
                    BSP_ST16C554_CS2A_Write(0x55);
                    BSP_Delay_ms(5);
                    printXunka();
                    BSP_Delay_ms(100);
                    GUI_Delay(1);
                }
            }
            else
            {
                while(1)
                {
                    if(!tingzhi) break;
                    for(i = 0; i < 4; i++)
                    {
                        BSP_ST16C554_CS2A_Write(xunka[i]);
                        BSP_Delay_ms(1);
                    }
                    BSP_Delay_ms(5);
                    printXunka();
                    BSP_Delay_ms(100);
                    GUI_Delay(1);
                }
            }
        }

        BSP_Delay_ms(100);
        GUI_Delay(1);
    }
}

