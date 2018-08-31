/*
*********************************************************************************************************
*
*	模块名称 : 汉字点阵字库。有效显示区 高16x宽15, 最右一列留白
*	文件名称 : hz16.c
*	版    本 : V1.0
*	说    明 : 只包含本程序用到汉字字库
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2011-09-08 armfly  ST固件库V3.5.0版本。
*
*	Copyright (C), 2010-2011, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "fonts.h"

/*
	FLASH中内嵌小字库，只包括本程序用到的汉字点阵
	每行点阵数据，头2字节是汉子的内码，后面32字节是16点阵汉子的字模数据。
*/

#ifdef USE_SMALL_FONT
/*
	FLASH中内嵌小字库，只包括本程序用到的汉字点阵
	每行点阵数据，头2字节是汉子的内码，后面32字节是16点阵汉子的字模数据。
*/
unsigned char const g_Hz16[] = {
0xA3,0xA8, 0x00,0x00,0x00,0x08,0x00,0x10,0x00,0x60,0x00,0x40,0x00,0xC0,0x00,0x80,0x01,0x80,// （ //
           0x01,0x80,0x00,0x80,0x00,0xC0,0x00,0x40,0x00,0x60,0x00,0x10,0x00,0x08,0x00,0x00,

0xA3,0xA9, 0x00,0x00,0x10,0x00,0x08,0x00,0x06,0x00,0x02,0x00,0x03,0x00,0x01,0x00,0x01,0x80,// ） //
           0x01,0x80,0x01,0x00,0x03,0x00,0x02,0x00,0x06,0x00,0x08,0x00,0x10,0x00,0x00,0x00,

0xB0,0xE5, 0x10,0x08,0x10,0x1C,0x11,0xE0,0x11,0x00,0xFD,0x00,0x11,0xFC,0x31,0x04,0x39,0x48,// 板 //
           0x55,0x48,0x51,0x50,0x91,0x30,0x12,0x20,0x12,0x50,0x14,0x8E,0x19,0x04,0x10,0x00,

0xB2,0xA9, 0x20,0x50,0x20,0x48,0x2F,0xFE,0x20,0x40,0x27,0xFC,0xFC,0x44,0x27,0xFC,0x24,0x44,// 博 //
           0x27,0xFC,0x24,0x44,0x20,0x10,0x2F,0xFE,0x22,0x10,0x21,0x10,0x20,0x50,0x20,0x20,

0xB4,0xB4, 0x08,0x04,0x08,0x04,0x14,0x04,0x12,0x24,0x21,0xA4,0x20,0xA4,0x7E,0x24,0xA2,0x24,// 创 //
           0x22,0x24,0x2A,0x24,0x24,0x24,0x20,0x04,0x21,0x04,0x21,0x04,0x1F,0x14,0x00,0x08,

0xB7,0xA2, 0x01,0x00,0x11,0x20,0x11,0x10,0x21,0x04,0x7F,0xFE,0x02,0x00,0x02,0x00,0x07,0xF0,// 发 //
           0x06,0x10,0x0A,0x20,0x09,0x40,0x10,0x80,0x21,0x40,0x42,0x30,0x8C,0x0E,0x30,0x04,

0xB7,0xA8, 0x00,0x40,0x40,0x40,0x30,0x40,0x10,0x48,0x87,0xFC,0x60,0x40,0x20,0x40,0x08,0x44,// 法 //
           0x17,0xFE,0x20,0x40,0xE0,0x40,0x20,0x80,0x21,0x10,0x22,0x08,0x27,0xFC,0x20,0x04,

0xBE,0xBA, 0x02,0x00,0x01,0x10,0x3F,0xF8,0x08,0x20,0x04,0x44,0xFF,0xFE,0x00,0x10,0x1F,0xF8,// 竞 //
           0x10,0x10,0x10,0x10,0x1F,0xF0,0x14,0x50,0x04,0x40,0x04,0x42,0x08,0x42,0x70,0x3E,

0xBF,0xAA, 0x00,0x08,0x7F,0xFC,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x24,0xFF,0xFE,// 开 //
           0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x10,0x20,0x20,0x20,0x40,0x20,

0xC1,0xAA, 0x01,0x04,0xFE,0xCC,0x24,0x50,0x24,0x00,0x3D,0xFC,0x24,0x20,0x24,0x20,0x24,0x24,// 联 //
           0x3F,0xFE,0x24,0x20,0x24,0x20,0x24,0x50,0x3E,0x50,0xE4,0x88,0x05,0x0E,0x06,0x04,

0xC4,0xA7, 0x00,0x84,0x3F,0xFE,0x22,0x20,0x3F,0xFC,0x26,0x30,0x2B,0x6E,0x32,0xA4,0x2F,0xF8,// 魔 //
           0x28,0x88,0x2F,0xF8,0x28,0x88,0x2F,0xF8,0x49,0x40,0x42,0x52,0x8C,0x42,0x30,0x3E,

0xC6,0xBD, 0x00,0x08,0x7F,0xFC,0x01,0x00,0x11,0x10,0x11,0x10,0x09,0x20,0x09,0x20,0x01,0x04,// 平 //
           0xFF,0xFE,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,

0xC8,0xFC, 0x01,0x00,0x7F,0xFE,0x44,0x42,0x9F,0xF4,0x04,0x40,0x1F,0xF0,0x04,0x40,0x7F,0xFC,// 赛 //
           0x08,0x20,0x1F,0xF0,0x28,0x2E,0xC9,0x24,0x09,0x20,0x01,0x00,0x06,0xC0,0x18,0x30,

0xCA,0xA6, 0x08,0x04,0x0B,0xFE,0x48,0x20,0x48,0x24,0x49,0xFE,0x49,0x24,0x49,0x24,0x49,0x24,// 师 //
           0x49,0x24,0x49,0x24,0x49,0x24,0x49,0x24,0x11,0x34,0x11,0x28,0x20,0x20,0x40,0x20,

0xCC,0xA8, 0x02,0x00,0x02,0x00,0x04,0x00,0x08,0x20,0x10,0x10,0x20,0x08,0x7F,0xFC,0x00,0x04,// 台 //
           0x00,0x10,0x1F,0xF8,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x1F,0xF0,0x10,0x10,

0xD2,0xE2, 0x01,0x10,0x3F,0xF8,0x08,0x20,0x04,0x44,0xFF,0xFE,0x00,0x10,0x1F,0xF8,0x10,0x10,// 意 //
           0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x02,0x00,0x51,0x84,0x50,0x92,0x90,0x12,0x0F,0xF0,

0xD6,0xC7, 0x20,0x00,0x22,0x04,0x3F,0x7E,0x48,0x44,0x08,0x44,0xFF,0xC4,0x10,0x44,0x14,0x7C,// 智 //
           0x22,0x44,0x42,0x10,0x9F,0xF8,0x10,0x10,0x1F,0xF0,0x10,0x10,0x1F,0xF0,0x10,0x10,


/* 最后一行必须用0xFF,0xFF结束，这是字库数组结束标志 */
0xFF,0xFF

};

#else
	unsigned char const g_Hz16[] = {0xFF, 0xFF};
#endif

