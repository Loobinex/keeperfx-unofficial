/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_keybrd.c
 *     Keyboard related routines - reading keyboard.
 * @par Purpose:
 *     Wrapper for keyboard support.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     10 Feb 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_keybrd.h"

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "globals.h"
#include "bflib_mouse.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/*
unsigned char lbKeyOn[256];
unsigned char lbShift;
unsigned char lbIInkeyFlags;
unsigned char lbInkey;
unsigned char lbIInkey;
unsigned char lbInkeyFlags;
unsigned short flow_control_flags;
unsigned long text_buf_pos;
bool lbExtendedKeyPress=false;
*/

const char AsciiToInkey[] = {
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x39, 0x2, 0x3,0x2B, 0x5, 0x6, 0x8,0x28,
  0x0A,0x0B, 0x9,0x4E,0x33,0x0C,0x34,0x35,
  0x0B, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8,
   0x9,0x0A,0x27,0x27,0x33,0x0D,0x34,0x35,
  0x28,0x1E,0x30,0x2E,0x20,0x12,0x21,0x22,
  0x23,0x17,0x24,0x25,0x26,0x32,0x31,0x18,
  0x19,0x10,0x13,0x1F,0x14,0x16,0x2F,0x11,
  0x2D,0x15,0x2C,0x1A,0x56,0x1B, 0x7,0x0C,
  0x29,0x1E,0x30,0x2E,0x20,0x12,0x21,0x22,
  0x23,0x17,0x24,0x25,0x26,0x32,0x31,0x18,
  0x19,0x10,0x13,0x1F,0x14,0x16,0x2F,0x11,
  0x2D,0x15,0x2C,0x1A,0x56,0x1B,0x2B, 0x0,
};

char lbInkeyToAscii[] = {
   0x0, 0x0,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x30,0x2D,0x3D, 0x8, 0x9,
  0x71,0x77,0x65,0x72,0x74,0x79,0x75,0x69,0x6F,0x70,0x5B,0x5D, 0x0, 0x0,0x61,0x73,
  0x64,0x66,0x67,0x68,0x6A,0x6B,0x6C,0x3B,0x27,0x60, 0x0,0x23,0x7A,0x78,0x63,0x76,
  0x62,0x6E,0x6D,0x2C,0x2E,0x2F, 0x0,0x2A, 0x0,0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,0x2D, 0x0, 0x0, 0x0,0x2B, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0,0x5C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x2F, 0x0, 0x0,0x28,0x29,0x2F,0x2A, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0,0x2E, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

char lbInkeyToAsciiShift[] = {
   0x0, 0x0, 0x21,0x22,0x9C,0x24,0x25,0x5E,0x26,0x2A,0x28,0x29,0x5F,0x2B, 0x8, 0x9,
  0x51,0x57,0x45,0x52,0x54,0x59,0x55,0x49,0x4F,0x50,0x7B,0x7D, 0x0, 0x0,0x41,0x53,
  0x44,0x46,0x47,0x48,0x4A,0x4B,0x4C,0x3A,0x40,0x7E, 0x0,0x7E,0x5A,0x58,0x43,0x56,
  0x42,0x4E,0x4D,0x3C,0x3E,0x3F, 0x0,0x2A, 0x0,0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,0x2D, 0x0, 0x0, 0x0,0x2B, 0x0,
   0x0, 0x0, 0x0, 0x0, 0x0, 0x0,0x7C, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x2F, 0x0, 0x0,0x28,0x29,0x2F,0x2A, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
   0x0,0x2E, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

/******************************************************************************/
DLLIMPORT long __stdcall _DK_KeyboardProc(int a1, unsigned int a2, long a3);
/******************************************************************************/
short LbIKeyboardClose(void)
{
  return 1;
}

short LbIKeyboardOpen(void)
{
  return 1;
}

void LbKeyboardSetLanguage(int lngnum)
{
  _DK_lbKeyboardLang = lngnum;
}

short LbKeyCodeValid(TbKeyCode key)
{
  if (key <= KC_UNASSIGNED)
    return false;
  if (key > KC_WAKE) // last key in enumeration - update if enumeration is changed
    return false;
  return true;
}

long __stdcall KeyboardProc(int a1, unsigned int a2, long a3)
{
  return _DK_KeyboardProc(a1, a2, a3);
}
/******************************************************************************/
#ifdef __cplusplus
}
#endif
