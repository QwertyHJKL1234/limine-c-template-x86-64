#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <memmgmt.h>
#include <io/io.h>
#include <io/font.h>

static volatile size_t cursor_x = 0;
static volatile size_t cursor_y = 0;
#define FONT_WIDTH 8
#define FONT_HEIGHT 8
//struct limine_framebuffer_response framebuffer_response;
void putpixel(struct limine_framebuffer *fb, size_t x, size_t y, uint32_t color) {
    if (x < fb->width && y < fb->height) {
        uint32_t *pixel_ptr = fb->address;
        pixel_ptr[y * (fb->pitch / 4) + x] = color;
    }
}
void printrect(struct limine_framebuffer *fb, size_t width, size_t height, uint32_t color) {
    for (size_t i = 0; i < height; i++) {
        for (size_t j = 0; j < width; j++) {
            putpixel(fb, cursor_x + j, cursor_y + i, color);
        }
    }
    cursor_x += width;
}
size_t getScreenWidth(struct limine_framebuffer *fb) {
    if (fb == NULL || fb == NULL) {
        // Handle the error, maybe return a default width or panic.
        return 80; // Return a default value (e.g., 80)
    }
    return fb->width;
}
void clear_screen(struct limine_framebuffer *fb, uint32_t color) {
    if (fb == NULL) {
        return;
    }
    uint32_t *pixel_ptr = fb->address;
    size_t num_pixels = fb->width * fb->height;
    for (size_t i = 0; i < num_pixels; i++) {
        pixel_ptr[i] = color;
    }
}

void printchar(struct limine_framebuffer *fb, char c, uint32_t color) {
    if (fb == NULL) {
        return;
    }

    if (c == '\n') {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
        return;
    }

    for (size_t y = 0; y < FONT_HEIGHT; y++) {
        unsigned char row = font[(int)c][y];
        for (size_t x = 0; x < FONT_WIDTH; x++) {
            if ((row >> x) & 1) {
                putpixel(fb, cursor_x + x, cursor_y + y, color);
            }
        }
    }

    cursor_x += FONT_WIDTH;
    if (cursor_x >= fb->width - FONT_WIDTH) {
        cursor_x = 0;
        cursor_y += FONT_HEIGHT;
    }
}
// Function to calculate string length (replacement for strlen)
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Function to print a formatted line with dashes
void printBreakLine(struct limine_framebuffer *fb, const char *str, uint32_t color) {
    size_t screen_width = getScreenWidth(fb);
    size_t str_len = strlen(str);
    size_t total_dashes = screen_width;
    size_t side_dashes;

    if (str_len + 2 + 2 <= screen_width) { //check if string with padding will fit on the screen
        side_dashes = (screen_width - str_len - 2 - 2) / 2; // Calculate dashes on each side
    }
    else
    {
        side_dashes = 0;
        total_dashes = str_len + 2 + 2;
    }

    // Allocate a buffer to hold the formatted string.  Use a stack buffer.
    char buffer[screen_width + 1]; //worst case all dashes + null terminator.

    memset(buffer, '-', total_dashes);  // Fill with dashes initially.
    size_t buffer_index = 0;

    // Add left dashes
    for (size_t i = 0; i < side_dashes; ++i) {
        buffer[buffer_index++] = '-';
    }
    // Add space
    buffer[buffer_index++] = ' ';

    // Copy the string into the buffer
    for (size_t i = 0; i < str_len; ++i) {
        buffer[buffer_index++] = str[i];
    }
     // Add space
    buffer[buffer_index++] = ' ';

    // Add right dashes
     for (size_t i = 0; i < side_dashes; ++i) {
        buffer[buffer_index++] = '-';
    }
    buffer[buffer_index] = '\0'; // Null-terminate the string.

    // Print the formatted string
    print(framebuffer_response.framebuffers[0], buffer, color);
}
void print(struct limine_framebuffer *fb, const char *str, uint32_t color) {
    while (*str) {
        printchar(fb, *str++, color);
    }
}
void println(struct limine_framebuffer *fb, const char *str, uint32_t color) {
    while (*str) {
        printchar(fb, *str++, color);
    }
    printchar(fb, '\n', color); // Add a newline character after the string
}
void printEmptyln(struct limine_framebuffer *fb)
{
    printchar(fb, '\n', 0xffffff);
}
void printEmpty(struct limine_framebuffer *fb)
{
    printchar(fb, '\0', 0xffffff);
}

