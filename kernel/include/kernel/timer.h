/* SYNAPSE SO - PIT Timer */
/* Licensed under GPLv3 */

#ifndef KERNEL_TIMER_H
#define KERNEL_TIMER_H

#include <stdint.h>

void timer_init(uint32_t frequency_hz);
void timer_increment_tick(void);
uint32_t timer_get_ticks(void);

#endif /* KERNEL_TIMER_H */
