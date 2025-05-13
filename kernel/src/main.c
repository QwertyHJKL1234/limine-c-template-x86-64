#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <io.h>

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

__attribute__((used, section(".limine_requests")))
static volatile LIMINE_BASE_REVISION(3);

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .c file, as seen fit.

__attribute__((used, section(".limine_requests_start")))
static volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile LIMINE_REQUESTS_END_MARKER;


// Halt and catch fire function.
static void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}
static void panic(void)
{
    hcf();
}

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
void kmain(void) {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }
    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
    println(framebuffer, "Framebuffer fetched.", 0xffffff);
    startTests(framebuffer);
    // We're done, just hang...
    hcf();
}
void startTests(struct limine_framebuffer *framebuffer)
{
    //printBreakLine("TESTING", 0xffffff);
    println(framebuffer, "----------------------------------------------- BEGIN FONT TEST SCREEN -----------------------------------------------", 0xffffff); // White text
    bool red = false;
    for (int i = 0; i < sizeof(font) / sizeof(font[0]); ++i) {
        if (red)
        {
            printchar(framebuffer, i, 0xff0000);
        }
        else printchar(framebuffer, i, 0xffffff);
        red = !red;
    }
    printEmptyln(framebuffer);
    println(framebuffer, "------------------------------------------------ END FONT TEST SCREEN ------------------------------------------------", 0xffffff); // White text
    printEmptyln(framebuffer);
    println(framebuffer, "----------------------------------------------- BEGIN COLOR TEST SCREEN -----------------------------------------------", 0xffffff); // White text
    printrect(framebuffer, 8, 8, 0xffffff);
    printrect(framebuffer, 8, 8, 0xff0000);
    printrect(framebuffer, 8, 8, 0xffA500);
    printrect(framebuffer, 8, 8, 0xffff00);
    printrect(framebuffer, 8, 8, 0x008000);
    printrect(framebuffer, 8, 8, 0x0000ff);
    printrect(framebuffer, 8, 8, 0xee82ee);
    printEmptyln(framebuffer);
    println(framebuffer, "------------------------------------------------ END COLOR TEST SCREEN ------------------------------------------------", 0xffffff); // White text
    printEmptyln(framebuffer);
    println(framebuffer, "---------------------------------------------- BEGIN SCROLL TEST SCREEN ----------------------------------------------", 0xffffff);
    for (int i = 0; i < 1000; i++)
    {
        println(framebuffer, "test", 0xffffff);
    }
    //clear_screen(framebuffer, 0xffffff);
    println(framebuffer, "----------------------------------------------- END SCROLL TEST SCREEN -----------------------------------------------", 0xffffff);
}
