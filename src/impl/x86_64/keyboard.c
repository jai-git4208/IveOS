#include <stddef.h>
#include <stdint.h>
#include "bool.h"
#include "keyboard.h"
#include "x86_64/idt.h"
#include "x86_64/ps2.h"

#define KEY_EXT 0xE0
#define SC_SHIFT_LEFT  0x2A
#define SC_SHIFT_RIGHT 0x36
#define SC_CAPSLOCK    0x3A
#define SC_ENTER       0x1C
#define SC_BACKSPACE   0x0E

static bool shift = false;
static bool caps  = false;
static bool key_down[256] = {0};

void (*keyboard_handler_user)(struct KeyboardEvent event);

static const char keymap_normal[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',   0, '\\',
    'z','x','c','v','b','n','m',',','.','/',   0, '*',   0, ' '
};

static const char keymap_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~',   0, '|',
    'Z','X','C','V','B','N','M','<','>','?',   0, '*',   0, ' '
};


char keyboard_translate(uint16_t scancode) {
    if (scancode > 127) return 0;

    if (scancode == SC_BACKSPACE) return '\b';
    if (scancode == SC_ENTER) return '\n';

    if (shift ^ caps)
        return keymap_shift[scancode];
    else
        return keymap_normal[scancode];
}

bool keyboard_is_pressed(uint16_t scancode) {
    return key_down[scancode];
}

void keyboard_handler() {
    static bool extended = false;
    uint8_t sc = ps2_read_scan_code();

    if (sc == KEY_EXT) { 
        extended = true; 
        return; 
    }

    uint16_t code = sc & 0x7F;
    bool released = sc & 0x80;

    key_down[code] = !released;

    // handle modifiers
    if (!released) {
        if (code == SC_SHIFT_LEFT || code == SC_SHIFT_RIGHT) shift = true;
        if (code == SC_CAPSLOCK) caps = !caps;
    } else {
        if (code == SC_SHIFT_LEFT || code == SC_SHIFT_RIGHT) shift = false;
    }

    if (keyboard_handler_user) {
        struct KeyboardEvent e;
        e.type = released ? KEYBOARD_EVENT_TYPE_BREAK : KEYBOARD_EVENT_TYPE_MAKE;
        e.code = extended ? (0xE000 | code) : code;
        extended = false;

        keyboard_handler_user(e);
    }
}

void keyboard_init() {
    idt_init();
    idt_set_handler_keyboard(keyboard_handler);
}

void keyboard_set_handler(void (*handler)(struct KeyboardEvent event)) {
    keyboard_handler_user = handler;
}
