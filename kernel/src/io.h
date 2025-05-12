#ifndef IO_H
#define IO_H
#endif
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
void print_char(struct limine_framebuffer *fb, char c, uint32_t color);
void println(struct limine_framebuffer *fb, const char *str, uint32_t color);
void print(struct limine_framebuffer *fb, const char *str, uint32_t color);
void putpixel(struct limine_framebuffer *fb, size_t x, size_t y, uint32_t color);
void printEmptyln(struct limine_framebuffer *fb);
void printEmpty(struct limine_framebuffer *fb);
static const unsigned char font[128][8];