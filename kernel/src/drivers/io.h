#ifndef IO_H
#define IO_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
extern struct limine_framebuffer_response framebuffer_response;
inline void outb(unsigned short port, unsigned char val);
void printchar(struct limine_framebuffer *fb, char c, uint32_t color);
void println(struct limine_framebuffer *fb, const char *str, uint32_t color);
void print(struct limine_framebuffer *fb, const char *str, uint32_t color);
void putpixel(struct limine_framebuffer *fb, size_t x, size_t y, uint32_t color);
void printEmptyln(struct limine_framebuffer *fb);
void printEmpty(struct limine_framebuffer *fb);
void clear_screen(struct limine_framebuffer *fb, uint32_t color);
size_t getScreenWidth(struct limine_framebuffer *fb);
void printBreakLine(struct limine_framebuffer *fb, const char *str, uint32_t color);
#endif