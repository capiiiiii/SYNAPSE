/* SYNAPSE SO - PIT Timer */
/* Licensed under GPLv3 */

#include <kernel/timer.h>
#include <kernel/io.h>
#include <kernel/vga.h>

#define PIT_FREQUENCY_HZ 1193180
#define PIT_COMMAND_PORT 0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_COMMAND_MODE3 0x36

static volatile uint32_t timer_ticks;

void timer_init(uint32_t frequency_hz) {
    if (frequency_hz == 0) {
        return;
    }

    uint32_t divisor = PIT_FREQUENCY_HZ / frequency_hz;
    if (divisor == 0) {
        divisor = 1;
    }

    if (divisor > 0xFFFF) {
        divisor = 0xFFFF;
    }

    timer_ticks = 0;

    outb(PIT_COMMAND_PORT, PIT_COMMAND_MODE3);
    outb(PIT_CHANNEL0_PORT, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0_PORT, (uint8_t)((divisor >> 8) & 0xFF));

    vga_print("[+] PIT timer initialized (Hz: ");
    vga_print_dec(frequency_hz);
    vga_print(")\n");
}

void timer_increment_tick(void) {
    timer_ticks++;
}

uint32_t timer_get_ticks(void) {
    return timer_ticks;
}
