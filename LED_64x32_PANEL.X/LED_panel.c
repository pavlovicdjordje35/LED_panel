

#include "LED_panel.h"
#include <p32xxxx.h>
#include "gamma.h"
#include "fonts.h"

// frame_buffer size
unsigned long frame_buffer [NUM_OF_PANELS*HEIGHT*WIDTH];
unsigned char positionX=0;
unsigned char positionY=0;
unsigned char panel=PANEL1;
unsigned int brightness=20;

extern const unsigned char gamma_curve[256];        
extern const char font[1024]; 

unsigned char BITS_PER_CHANNEL=8;


void clock(void)
{
    CLK=1;
    CLK=0;
}
void latch(void)
{
    LAT=1;
    LAT=0; 
}
void selectRow(unsigned char x)
{
    A=(x&0x01);
    B=(x&0x02)>>1;
    C=(x&0x04)>>2;
    D=(x&0x08)>>3;

}
void SetBitsPerChannel(unsigned char bits)
{
    if(bits>8)BITS_PER_CHANNEL=8;
    else BITS_PER_CHANNEL=bits;
}

void SetTopColor(unsigned long color, unsigned char i)
{ 
    R1=(((color&0xFF0000)>>16)>>i)&0x0001;
    G1=(((color&0x00FF00)>>8)>>i)&0x0001;
    B1= ((color&0x0000FF)>>i)&0x0001;
}
void SetBottomColor(unsigned long color, unsigned char i)
{
    R2=(((color&0xFF0000)>>16)>>i)&0x0001;
    G2=(((color&0x00FF00)>>8)>>i)&0x0001;
    B2= ((color&0x0000FF)>>i)&0x0001;
}

 void InitLEDPanel(void)
{
    unsigned int i;
    TRISEbits.TRISE5=0;  //R1-2
    TRISEbits.TRISE6=0;  //G1-2
    TRISEbits.TRISE4=0;  //B1-2
    //----------------
    TRISEbits.TRISE4=0;  //R2-2
    TRISEbits.TRISE7=0;  //G2-2
    TRISFbits.TRISF5=0;  //B2-2
   //---------------- 
    TRISDbits.TRISD1=0;   //R1-1
    TRISDbits.TRISD2=0;   //G1-1
    TRISDbits.TRISD10=0;  //B1-1
    //----------------
    TRISDbits.TRISD9=0;  //R2-1
    TRISDbits.TRISD3=0;  //G2-1
    TRISDbits.TRISD8=0;  //B2-1
    //----------------
    TRISBbits.TRISB14=0; //CLK
    TRISDbits.TRISD0=0;  //LAT
    TRISBbits.TRISB15=0;  //OE
    //----------------
    TRISFbits.TRISF1=0;  //A
    TRISBbits.TRISB7=0;  //B
    TRISFbits.TRISF0=0;  //C
    //----------------
    TRISBbits.TRISB6=0;  //D
    
    

    for(i=0;i<(NUM_OF_PANELS*WIDTH*HEIGHT);i++)
    {
        frame_buffer[i]=0;
    }   
}

void Refresh(void) 
{
    unsigned int row, column, segment, panel;
    unsigned int index;
    unsigned int i=0, cBit=0;
    
    for( i=0; i<BITS_PER_CHANNEL;i++){
        cBit=7-i;
    for(row=0; row < NUM_OF_ROWS; row++){
        selectRow(row);
        
        index = (row<<5);
        for (panel = 0;  panel< NUM_OF_PANELS; panel++) {
            for ( segment= 1;  segment<= NUM_OF_SEGMENTS;  segment++) {
                
                index+=((NUM_OF_SEGMENTS-segment)<<8);
                 
                for( column = 0; column< NUM_OF_COLUMNS; column++){
                SetTopColor(frame_buffer[index+column],cBit);
                SetBottomColor(frame_buffer[index+column+1024],cBit);
                clock(); 
                
                }
                
            }
            index += 1024;
        }
        latch();
        OE = 0;
        Delay_us(1<<cBit);
        OE = 1;
        
    }
    
}
}

void RefreshDelay(unsigned int delay)
{
    unsigned int i;
    for(i=0;i<delay;i++)
    {
        Refresh();
    }
}

void NoRefreshDelay(unsigned int delay)
{
    unsigned int i;
    for(i=0;i<delay;i++)
    {
        Delay_us(5000); 
    }
    
}

void SetBrightness(unsigned int value)
{
    brightness=value;
}
void FillPanel(unsigned long color, unsigned int panel)
{
    unsigned int i;
	for(i=(panel<<NEXT_PANEL);i<((panel<<NEXT_PANEL)+WIDTH*HEIGHT);i++)
    {
        /*if(frame_buffer[i])*/frame_buffer[i]=color;
    }
}
void InvertPanel(unsigned int panel)
{
    unsigned int i;
	for(i=(panel<<NEXT_PANEL);i<((panel<<NEXT_PANEL)+WIDTH*HEIGHT);i++)
    {
        frame_buffer[i]=0xFFFFFF-frame_buffer[i];
    }
}

void GammaCorrectPanel(unsigned int panel)
{
    unsigned int i;
	for(i=(panel<<NEXT_PANEL);i<((panel<<NEXT_PANEL)+WIDTH*HEIGHT);i++)
    {
        frame_buffer[i]=GammaCorrected(frame_buffer[i]);
    }
}
void GammaCorrect()
{
    GammaCorrectPanel(0);
    GammaCorrectPanel(1);
}
void FillScreen(unsigned long color)
{
    FillPanel(color, 0);
    FillPanel(color, 1);
}

void InvertScreen(void)
{
    InvertPanel(0);
    InvertPanel(1);
}
void ScrollLeftRight(unsigned char dir)
{

    unsigned int  i, j, panel;
    unsigned long temp[NUM_OF_PANELS*WIDTH]={0};
    
    dir=dir>>2;
    //store first column to temp
    for(i=0;i<32;i++)
    {
        for(panel=0; panel<NUM_OF_PANELS; panel++)
        {
            
            if(dir)temp[i+panel*32]=frame_buffer[panel*1024+i*32];
            else temp[i+panel*32]=frame_buffer[(NUM_OF_PANELS-panel-1)*1024+31+i*32];
        }
    }
    // move data from fist to last column, column by column, panel by panel
    for(i=0;i<32;i++)
    {
        for(j=0;j<31;j++)
        {
            for(panel=0; panel<NUM_OF_PANELS; panel++)
            {
                if(dir)frame_buffer[panel*1024+j+i*32]=frame_buffer[panel*1024+(j+1)+i*32];
                else frame_buffer[panel*1024+(31-j)+i*32]=frame_buffer[panel*1024+(30-j)+i*32];
            }
        }    
    }
    // store data from temp to last column 
    for(i=0;i<32;i++)
    {
        for(panel=0; panel<NUM_OF_PANELS; panel++)
        {
            if(dir)frame_buffer[(NUM_OF_PANELS-panel-1)*1024+31+i*32]=temp[i+panel*32];
            else frame_buffer[panel*1024+i*32]=temp[i+panel*32];
        }        
    }
 
}

void ScrollUpDown(unsigned char dir)
{
 
    unsigned int  i, j, panel;
    unsigned long temp[NUM_OF_PANELS*WIDTH]={0};  
    
    dir=dir>>1;
    //store first row to temp
    for(i=0;i<32;i++)
    {
        for(panel=0; panel<NUM_OF_PANELS; panel++)
        {
            if(dir) temp[i+panel*32]=frame_buffer[i+panel*1024];
            else temp[i+panel*32]=frame_buffer[i+31*32+panel*1024];
        }
    }
    // move data from fist to last row, row by row, panel by panel
    for(i=0;i<32;i++)
    {
        for(j=0;j<31;j++)
        {
            for(panel=0; panel<NUM_OF_PANELS; panel++)
            {
                if(dir) frame_buffer[i+j*32+panel*1024]=frame_buffer[i+(j+1)*32+panel*1024];
                else frame_buffer[i+(31-j)*32+panel*1024]=frame_buffer[i+(30-j)*32+panel*1024];
            }
        }
    }
    // store data from temp to last row 
    for(i=0;i<32;i++)
    {
        for(panel=0; panel<NUM_OF_PANELS; panel++)
        {
            if(dir) frame_buffer[i+31*32+panel*1024]=temp[i+panel*32];
            else frame_buffer[i+panel*1024]=temp[i+panel*32];
        }
    }  
}

void Scroll(unsigned char dir, unsigned int num_of_px, unsigned int speed)
{
    unsigned int i;
    
    switch(dir)
    {
        
        case LEFT:
                for(i=0;i<num_of_px;i++)
                {
                    ScrollLeftRight(LEFT);
                    RefreshDelay(speed);
                }
                break;
        case RIGHT:
                for(i=0;i<num_of_px;i++)
                {
                    ScrollLeftRight(RIGHT);
                    RefreshDelay(speed);
                }
                break;
        case UP:        
                for(i=0;i<num_of_px;i++)
                {
                    ScrollUpDown(UP);
                    RefreshDelay(speed);
                }
                break;
        case DOWN:
                for(i=0;i<num_of_px;i++)
                {
                    ScrollUpDown(DOWN);
                    RefreshDelay(speed);
                }
                break;
    }
}


/*void LoadImage(const unsigned long *img, unsigned char img_width, unsigned char img_height, unsigned int panel, unsigned char correct_gamma)
{
    unsigned int i, j, endX, endY;
    
    endX=img_width+positionX;
    endY=img_height+positionY;
    if(endX>32)endX=32;
    if(endY>32)endY=32;
    
    for(j=positionY;j<(endY);j++)
    {
        for(i=positionX;i<(endX);i++)
        {
            if(correct_gamma)frame_buffer[i+j*32+(panel<<NEXT_PANEL)]=GammaCorrected(img[i-positionX+(j-positionY)*(img_width)]);
            else frame_buffer[i+j*32+(panel<<NEXT_PANEL)]=(img[i-positionX+(j-positionY)*(img_width)]);
        }
    }
}*/


void LoadImage(const unsigned long *img, unsigned char correct_gamma)
{
    unsigned int i, j, endX, endY, p, k, img_width, img_height;
    
    img_width=img[0];
    img_height=img[1];
    endX=img_width+positionX+panel*32;
    endY=img_height+positionY;
    if(endX>64)endX=64;
    if(endY>32)endY=32;
    
    for(j=positionY;j<(endY);j++)
    {
        for(i=(positionX+panel*32);i<(endX);i++)
        {
            p=i/32;
            k=i%32;
            if(correct_gamma)frame_buffer[k+j*32+(p<<NEXT_PANEL)]=GammaCorrected(img[2+i-(positionX+panel*32)+(j-positionY)*(img_width)]);
            else frame_buffer[k+j*32+(p<<NEXT_PANEL)]=img[2+i-(positionX+panel*32)+(j-positionY)*(img_width)];
        }
    }
}
void GetPanelImage(unsigned int panel, unsigned long *img)
{
    unsigned int i;

    for(i=(panel<<NEXT_PANEL);i<((panel<<NEXT_PANEL)+WIDTH*HEIGHT);i++)
    {
       img[i]=frame_buffer[i+(panel<<NEXT_PANEL)];
    }
}
void SetPixel(unsigned char x, unsigned char y, unsigned long color)
{
    panel=x/32;
    if(panel>=NUM_OF_PANELS)panel=0;
    positionX=x%32;
    positionY=y%32;
    frame_buffer[(positionY<<NEXT_ROW)+positionX+(panel<<NEXT_PANEL)]=color; 
}

void SetCursorPosition (unsigned char x, unsigned char y)
{
    panel=x/32;
    if(panel>=NUM_OF_PANELS)panel=0;
    positionX=x%32;
    positionY=y%32;
}
void NewLine(void)
{
    panel=0;
    positionX=0;
    
    if((positionY+FONT_HEIGHT)>=32)
    {
        positionY=0;
        ClearScreen();
    }
    else positionY+=FONT_HEIGHT;
}
void NextRow(void)
{
    positionX++;
    if(positionX>=32)
    {
        panel++;
        positionX=0;
        if(panel>=NUM_OF_PANELS) NewLine();
    }    
}

void PanelPutChar (char c, unsigned long color)
{
    unsigned char data, x, y, y_pom;

    
    if(((positionX+FONT_LENGHT)>=32)&&(panel>=(NUM_OF_PANELS-1)))NewLine();
    y_pom=positionY;
    
    if(c==' '){NextRow();NextRow();} 
    else
    {
        for (x=0;x<FONT_LENGHT;x++)
        {

            {
                for(y=0; y<FONT_HEIGHT; y++)
                {
                    data=font[((c-32)*FONT_LENGHT)+x];
                    if((data>>y)&0x01) SetPixel(positionX+panel*32, y_pom+y, color);
                //    else SetPixel(positionX+panel*32, y_pom+y, BLACK);
                   // positionY++;
                }
                positionY=y_pom;
                if(data!=0) NextRow(); 
            }    
        }
    }
    
    NextRow();
}

void PanelPrint(char *text, unsigned long color) 
{
	while(*text != 0)
	{
		PanelPutChar(*text, color);
		text++;
	}
}

void ClearScreen(void)
{
    unsigned int i;
    for(i=0; i<(NUM_OF_PANELS<<NEXT_PANEL);i++) frame_buffer[i]=0;
}


void DrawRectangle (unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2, unsigned long line_color, unsigned long fill_color)
{
  	unsigned char pixel_x=0, pixel_y=0;

	/* Draw the two horizontal lines */
  	for (pixel_x = 0; pixel_x<(x2-x1+1); pixel_x++) 
  	{
		SetPixel((x1+pixel_x), y1, line_color);
		SetPixel((x1+pixel_x), y2, line_color);
	}
  	
  	/* draw the two vertical lines */
  	for (pixel_y = 0; pixel_y<(y2-y1+1); pixel_y++) 
  	{
		SetPixel(x1, y1+pixel_y, line_color);
		SetPixel(x2, y1+pixel_y, line_color);
	}
    
    for (pixel_x = 1; pixel_x<(x2-x1); pixel_x++) 
  	{
        for (pixel_y = 1; pixel_y<(y2-y1); pixel_y++) 
        {
            SetPixel(x1+pixel_x, y1+pixel_y, fill_color);
         //   SetPixel(x2+pixel_x, y1+pixel_y, fill_color);
        }
	}
  	
  	/* draw the two vertical lines */

}
void DrawCircle (unsigned char x, unsigned char y, unsigned char r, unsigned long line_color, unsigned long fill_color)
{
	int sw=0, intY=0, intX=0, i;
	unsigned char d;

	d = y - x;
	intY = r;
	sw =3-2*r;
	while(intX<=intY) 
	{
        for(i=(y-(unsigned char)intY)+1; i<(y+(unsigned char)intY);i++)
        {
            SetPixel(((unsigned char)(x-intX)), i, fill_color);
            SetPixel(((unsigned char)(x+intX)), i, fill_color);
        }
            
		SetPixel(((unsigned char)(x+intX)), y+(unsigned char)intY, line_color);
		SetPixel(((unsigned char)(x+intX)), y-(unsigned char)intY, line_color);
		SetPixel(((unsigned char)(x-intX)), y+(unsigned char)intY, line_color);
		SetPixel(((unsigned char)(x-intX)), y-(unsigned char)intY, line_color);


        for(i=(y-(unsigned char)intX); i<(y+(unsigned char)intX)+1;i++)
        {   
            SetPixel(((unsigned char)(y+intY-d))-1, i, fill_color);    
            SetPixel(((unsigned char)(y-intY-d))+1, i, fill_color);
        }
		SetPixel(((unsigned char)(y+intY-d)), y+(unsigned char)intX, line_color);
		SetPixel(((unsigned char)(y+intY-d)), y-(unsigned char)intX, line_color);
		SetPixel(((unsigned char)(y-intY-d)), y+(unsigned char)intX, line_color); 
		SetPixel(((unsigned char)(y-intY-d)), y-(unsigned char)intX, line_color);

        
		if(sw<0) sw+=(4*intX+6);
		else 
		{
			sw+=(4*(intX-intY)+10);
			intY--;
		}
		intX++;

	}
   
}

void RandCir(unsigned long line_color){

    int x =rand()%64;
    int y =rand()%32;
    int r =rand()%6;
    int i;
    for (i = 0; i < 3; i++) {
        DrawCircle(x, y, r, RED, 0x000);
        DrawCircle(x, y, r-1, 0x000, 0x000);
        r+=1;
    }
}

void SineFunction(unsigned char amplitude, unsigned char period, unsigned long color)
{
    double x;
    unsigned char i, y;
    
    for(i=0; i<(32*NUM_OF_PANELS); i++)
    {
        x=i;
        x=(sin(x/(period)*2*3.14159)*amplitude)+amplitude;
        SetPixel(i, x+((32-2*amplitude)>>1),color);
    }
}  

void NoiseFunction(void)
{
    unsigned int i,p;
    unsigned int long r;
    
    //for(i=(panel<<NEXT_PANEL);i<((panel<<NEXT_PANEL)+WIDTH*HEIGHT);i++)
    for(i=0; i<1024;i++)    
    {
        r=rand()*0xFFFFFF;
        for(p=0; p<NUM_OF_PANELS;p++) 
        {   
            frame_buffer[i+(p<<NEXT_PANEL)]=r;
        }
    }
    
}

unsigned long GammaCorrected(unsigned long color)
{
	unsigned long r,g,b;
	
	r=gamma_curve[((color&0xFF0000)>>16)];
	g=gamma_curve[((color&0x00FF00)>>8)];
	b=gamma_curve[((color&0x0000FF))];
	
	return ((r<<16)|(g<<8)|b);
}

void SetScreenPixel(unsigned int i, unsigned long color)
{
    frame_buffer[i]=color;
}