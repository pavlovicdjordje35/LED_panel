/* 
 * File:   config.h
 * Author: Jovan
 *
 * Created on June 4, 2017, 3:34 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H


#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF   //bio fplldiv 2
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_8
#define SYS_FREQ (80000000L)
#define GetSystemClock() 80000000L



#endif	/* CONFIG_H */

