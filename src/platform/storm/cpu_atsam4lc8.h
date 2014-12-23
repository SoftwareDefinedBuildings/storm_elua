// LPC2888 CPU definition

#ifndef __CPU_ATSAM4LC8_H_
#define __CPU_ATSAM4LC8_H__

#include "stacks.h"

// Number of resources (0 if not available/not implemented)
#define NUM_PIO               0
#define NUM_SPI               0
#define NUM_UART              1
#define NUM_TIMER             0
#define NUM_PWM               0
#define NUM_ADC               0
#define NUM_CAN               0

// CPU frequency (needed by the CPU module and MMCFS code, 0 if not used)
// #define CPU_FREQUENCY         Fcclk

// PIO prefix ('0' for P0, P1, ... or 'A' for PA, PB, ...)
#define PIO_PREFIX            '0'
// Pins per port configuration:
// #define PIO_PINS_PER_PORT (n) if each port has the same number of pins, or
// #define PIO_PIN_ARRAY { n1, n2, ... } to define pins per port in an array
#define PIO_PINS_PER_PORT 0
// #define PIO_PIN_ARRAY         { 32, 20, 4, 6, 12, 6, 4, 1 }

// Allocator data: define your free memory zones here in two arrays
// (start address and end address)
#define SRAM_ORIGIN           0x20008000
#define SRAM_SIZE             0x8000
#define INTERNAL_RAM1_FIRST_FREE end
#define INTERNAL_RAM1_LAST_FREE  (0x2000e000 - 4)

#endif // #ifndef __CPU_LPC2888_H__

