#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <wchar.h>

#define LOWORD(a) ((short)(a))
#define HIWORD(a) ((short)(((int)(a) >> 16) & 0xFFFF))
#define MAX_PRINTF_BUFFER 256

#ifdef __cplusplus
extern "C" {
#endif

int ttyWrite(int channel, const char* message, int length, int* written);

int vsnprintf(char* str, size_t size, const char* format, va_list args);
int vsprintf(char* buf, const char* fmt, va_list args);
int sprintf(char* buffer, const char* fmt, ...);
int snprintf(char* buffer, size_t len, const char* fmt, ...);
int printf(const char* fmt, ...);

size_t StringToWideCharacter(wchar_t* dest, const char* scr, size_t len);
size_t WideCharacterToString(char* dest, const wchar_t* scr, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* PRINTF_H */
