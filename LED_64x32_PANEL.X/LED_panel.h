/* 
 * File:   LED_32x32_panel.h
 * Author: Jovan
 *
 * Created on June 3, 2017, 5:00 PM
 */

#ifndef LED_PANEL_H
#define	LED_PANEL_H

#include <xc.h>			// Required for SFR defs
#include <sys/attribs.h>
#include "my_delays.h"
#include "colors.h"
#include <plib.h>
#include <math.h>

/********************************************
 --------------------------------------------
 ********************************************/

#define NUM_OF_SELECTION_BITS 3 // =3 (for panels 1,2,3); =4 (for panel 4)

/********************************************
 --------------------------------------------
 ********************************************/

// configure pins
#define R1  LATDbits.LATD1
#define G1  LATDbits.LATD2
#define B1  LATDbits.LATD10

#define R2  LATDbits.LATD9
#define G2  LATDbits.LATD3
#define B2  LATDbits.LATD8

#define CLK LATBbits.LATB14
#define LAT LATDbits.LATD0
#define OE LATBbits.LATB15
	
#define A LATFbits.LATF1
#define B LATBbits.LATB7
#define C LATFbits.LATF0
#define D LATBbits.LATB6  // needed for panel 4
/*
atlernative pins 
#define R1  LATEbits.LATE5
#define G1  LATEbits.LATE6
#define B1  LATEbits.LATE4

#define R2  LATFbits.LATF4
#define G2  LATEbits.LATE7
#define B2  LATFbits.LATF5
*/

// configure panel
#define NUM_OF_PANELS 2  // change for diferent num of panels
#define WIDTH 32
#define HEIGHT 32 
#define SUBPANELS 1
//------------------------
#define NUM_PIXELS_PER_PANEL (WIDTH*HEIGHT) //1024
#define NUM_OF_ROWS (SUBPANELS<<NUM_OF_SELECTION_BITS) //8/16
#define NUM_OF_COLUMNS WIDTH //32
#define NUM_OF_SEGMENTS ((HEIGHT>>SUBPANELS)>>NUM_OF_SELECTION_BITS) //2/1
//------------------------
#define NEXT_PANEL 10
#define NEXT_ROW 5
#define NEXT_COLUMN 5
#define SECOND_SUBPANEL ((WIDTH*HEIGHT)>>SUBPANELS) //512
//------------------------
#define PANEL1 0
#define PANEL2 1
//------------------------
#define DOWN 1
#define UP 2
#define RIGHT 3
#define LEFT 4

#define TRUE 1
#define FALSE 0

//functions
void clock(void); // OK
void latch(void); // OK
void selectRow(unsigned char x); // OK
void SetBitsPerChannel(unsigned char bits);
void SetTopColor(unsigned long color, unsigned char i); // OK
void SetBottomColor(unsigned long color, unsigned char i); // OK
void InitLEDPanel(void); // OK
void Refresh(void); // OK
void RefreshDelay(unsigned int delay); // OK
void NoRefreshDelay(unsigned int delay); // OK
void SetBrightness(unsigned int value);
void FillPanel(unsigned long color, unsigned int panel); // OK
void FillScreen(unsigned long color);
void InvertPanel(unsigned int panel); // OK
void InvertScreen(void); // OK
void GammaCorrectPanel(unsigned int panel); // OK
void GammaCorrect(); //
void ScrollLeftRight(unsigned char dir); // OK
void ScrollUpDown(unsigned char dir); // OK
void Scroll(unsigned char dir, unsigned int num_of_px, unsigned int speed); // OK
void SetPixel(unsigned char x, unsigned char y, unsigned long color); // OK
void LoadImage(const unsigned long *img, unsigned char correct_gamma); // OK
void GetPanelImage(unsigned int panel, unsigned long *img);
void PanelPutChar (char c, unsigned long color); // OK
void SetCursorPosition (unsigned char x, unsigned char y); // OK
void NewLine(void); // OK
void NextRow(void); // OK
void PanelPrint(char *text, unsigned long color);  // OK
void ClearScreen(void); // OK
void DrawRectangle (unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, unsigned long line_color, unsigned long fill_color);// OK
void DrawCircle (unsigned char x, unsigned char y, unsigned char r, unsigned long line_color, unsigned long fill_color); // OK
void SineFunction(unsigned char amplitude, unsigned char period, unsigned long color); // OK
void NoiseFunction(void); // OK
unsigned long GammaCorrected(unsigned long color); // OK
void SetScreenPixel(unsigned int i, unsigned long color); // OK

#endif	/* LED_PANEL_H */

