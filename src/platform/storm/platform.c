// Platform-dependent functions

#include "platform.h"
#include "type.h"
#include "devman.h"
#include "genstd.h"
#include "stacks.h"
#include <reent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>



#include "common.h"
#include "platform_conf.h"
#include "interface.h"
// ****************************************************************************
// Platform initialization

int platform_init()
{
  printf("Platform init called\n");
  cmn_platform_init();

  return PLATFORM_OK;
} 

// ****************************************************************************
// PIO functions


pio_type platform_pio_op( unsigned port, pio_type pinmask, int op )
{
/*
  pio_type retval = 1;
  
  switch( op )
  {
    case PLATFORM_IO_PORT_SET_VALUE:
      *pio_m0_regs[ port ] = pinmask;        
      break;
      
    case PLATFORM_IO_PIN_SET:
      *pio_m0s_regs[ port ] = pinmask;    
      break;
      
    case PLATFORM_IO_PIN_CLEAR:
      *pio_m0c_regs[ port ] = pinmask;
      break;
      
    case PLATFORM_IO_PORT_DIR_OUTPUT:
      *pio_m1_regs[ port ] = 0xFFFFFFFF;
      break;
      
    case PLATFORM_IO_PIN_DIR_OUTPUT:
      *pio_m1s_regs[ port ] = pinmask;
      break;
      
    case PLATFORM_IO_PORT_DIR_INPUT:
      *pio_m1_regs[ port ] = 0;
      *pio_m0_regs[ port ] = 0;
      break;
      
    case PLATFORM_IO_PIN_DIR_INPUT:
      *pio_m1c_regs[ port ] = pinmask;
      *pio_m0c_regs[ port ] = pinmask;
      break;
            
    case PLATFORM_IO_PORT_GET_VALUE:
      retval = *pio_pin_regs[ port ];
      break;
      
    case PLATFORM_IO_PIN_GET:
      retval = *pio_pin_regs[ port ] & pinmask ? 1 : 0;
      break;
      
    default:
      retval = 0;
      break;
  }*/
  return 0;
}

// ****************************************************************************
// UART

u32 platform_uart_setup( unsigned id, u32 baud, int databits, int parity, int stopbits )
{
  return 0;
}

void platform_s_uart_send( unsigned id, u8 data )
{
    k_write(1, &data, 1);
}

int platform_s_uart_recv( unsigned id, timer_data_type timeout )
{
    unsigned char rv;
    unsigned int rrv;
    while (1)
    {
        printf("!",rrv);
        rrv = k_read(0, &rv, 1);
        if (rrv != 0) {
            printf("(%d)",rrv);
            return rv;
        }
        if (timeout != PLATFORM_TIMER_INF_TIMEOUT)
        {
            return -1;
        }
    }
}

int platform_s_uart_set_flow_control( unsigned id, int type )
{
  return PLATFORM_ERR;
}

timer_data_type platform_s_timer_op( unsigned id, int op, timer_data_type data )
{
  u32 res = 0;

  return res;
}

int platform_cpu_set_global_interrupts( int status )
{
  return 0;
}

int platform_cpu_get_global_interrupts( void )
{
  return 0;
}
