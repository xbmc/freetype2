/***************************************************************************/
/*                                                                         */
/*  ftstream.h                                                             */
/*                                                                         */
/*    Stream handling (specification).                                     */
/*                                                                         */
/*  Copyright 2017 by                                                      */
/*  Pär Björklund                                                          */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#if defined(WIN32) || defined(_MSC_VER)
wchar_t* to_utf16(const char* str, size_t length)
{
  if (length == 0)
    length = strlen(str);
  int result = MultiByteToWideChar(CP_UTF8, 0, str, length, NULL, 0);
  if (result == 0)
  {
    return NULL;
  }

  wchar_t* dirPath = malloc(result * 2);
  result = MultiByteToWideChar(CP_UTF8, 0, str, length, dirPath, result);

  if (result == 0)
  {
    free(dirPath);
    return NULL;
  }

  return dirPath;
}

char* to_utf8(const wchar_t* str, size_t length)
{
  if (length == 0)
    length = wcslen(str);

  int result = WideCharToMultiByte(CP_UTF8, 0, str, length, NULL, 0, NULL, NULL);
  if (result == 0)
    return NULL;

  char *newStr = malloc(result);
  result = WideCharToMultiByte(CP_UTF8, 0, str, length, newStr, result, NULL, NULL);
  if (result == 0)
  {
    free(newStr);
    return NULL;
  }

  return newStr;
}
#endif

FILE* utf8_fopen(
  char const* _FileName,
  char const* _Mode
)
{
  FILE* f = NULL;
  errno_t result = 0;
  wchar_t* filename = to_utf16(_FileName, 0);
  wchar_t* mode = to_utf16(_Mode, 0);

  if (filename && mode)
    result = _wfopen_s(&f, filename, mode);

  free(filename);
  free(mode);
  if (result == 0)
    return f;

  return NULL;
}