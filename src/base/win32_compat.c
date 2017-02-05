/***************************************************************************/
/*                                                                         */
/*  ftstream.h                                                             */
/*                                                                         */
/*    Stream handling (specification).                                     */
/*                                                                         */
/*  Copyright 2017 by                                                      */
/*  P�r Bj�rklund                                                          */
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
wchar_t* ft_to_utf16(const char* str, size_t length)
{
  if (length == 0)
    length = strlen(str);

  int result = MultiByteToWideChar(CP_UTF8, 0, str, length, NULL, 0);
  if (result == 0)
    return NULL;

  length = result + 1;
  wchar_t* dirPath = malloc(length * 2);
  result = MultiByteToWideChar(CP_UTF8, 0, str, result, dirPath, length);

  if (result == 0)
  {
    free(dirPath);
    return NULL;
  }

  if (dirPath[length - 1] != '\0')
    dirPath[length - 1] = '\0';

  return dirPath;
}

char* ft_to_utf8(const wchar_t* str, size_t length)
{
  if (length == 0)
    length = wcslen(str);

  int result = WideCharToMultiByte(CP_UTF8, 0, str, length, NULL, 0, NULL, NULL);
  if (result == 0)
    return NULL;

  length = result + 1;
  char *newStr = malloc(length);
  result = WideCharToMultiByte(CP_UTF8, 0, str, result, newStr, length, NULL, NULL);
  if (result == 0)
  {
    free(newStr);
    return NULL;
  }

  if (newStr[length - 1] != '\0')
    newStr[length - 1] = '\0';

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
  wchar_t* filename = ft_to_utf16(_FileName, 0);
  wchar_t* mode = ft_to_utf16(_Mode, 0);

  if (filename && mode)
    result = _wfopen_s(&f, filename, mode);

  free(filename);
  free(mode);
  if (result == 0)
    return f;

  return NULL;
}