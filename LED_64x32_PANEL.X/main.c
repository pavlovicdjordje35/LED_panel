/* 
 * File:   main.c
 * Author: Jovan
 */
#include <p32xxxx.h>
#include <sys/attribs.h>
#include <plib.h>
#include <xc.h>
#include "config.h"
#include "my_delays.h"
#include "display.h"

#define DATA        LATDbits.LATD6

void Init(void)
{   
    // disable JTAG port
    DDPCONbits.JTAGEN = 0;
    AD1PCFG = 0xFFFF;
    TRISDbits.TRISD6=0;
  
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
}



void main(void) 
{
    Init();
    delay_ms(50);
    InitLEDPanel();
    
    setup();
  
    while(1)
    { 
        loop(); 
        
    }
}

