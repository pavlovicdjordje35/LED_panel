#include "display.h"
#include "LED_panel.h"
#include "colors.h"
#include "random.h"

// ovde ukljuciti fajlove sa slikama
// npr. #include img.h

/* FUNKCIJE */


// ClearScreen(); // brise sadrzaj sa ekrana
// FillScreen(COLOR); // postavlja boju ekrana u boju COLOR
// InvertScreen(); // invertuje boje piskela na ekranu 
// GammaCorrect(); // vrsi gamma korekciju boja na ekranu
// SetPixel(x,y, COLOR); // postavlja vrednost piksela na poziciji (x,y) na ekranu u zadatu boju COLOR (RED, GREEN, BLUE, YELLOW...)
// SetCursorPosition(x,y); // postavlja kursor na poziciju (x,y) na ekranu (prilikom ispisa teksta ili slike odredjuje pocetni piksel)
// DrawRectangle(x1,y1,x2,y2, LINE_COLOR, FILL_COLOR); // iscrtava pravougaonik na ekranu od x1 do x2 u horizontalnom pravcu, odnosno od y1 do y2 u vertikalnom pravcu zadate boje LINE_COLOR, ispunjen bojom FILL_COLOR
// DrawCircle(x,y,r, LINE_COLOR, FILL_COLOR); // iscrtava krug sa centrom u tacki (x,y), poluprecnika r, zadate boje LINE_COLOR, ispunjen bojom FILL_COLOR
// PanelPutChar(c, COLOR); // ispisuje jedan karakter c na ekranu boje COLOR. Pre poziva ove funkcije pozvati funkciju SetCursorPosition za odredjivanje mesta ispisa karaktera
// PanelPrint(t, COLOR); // ispisuje tekstualni sadrzaj t na ekranu boje COLOR. Pre poziva ove funkcije pozvati funkciju SetCursorPosition za odredjivanje mesta ispisa teksta
// Scroll(dir, n, speed); // pomera sadrzaj na ekranu u smeru dir (UP, DOWN, LEFT, RIGHT) za n piksela brzinom speed (1-10).
// LoadImage(img, gamma_corrected); // ucitava sliku na ekran. Slika moze biti sa ili bez gama korekcije gamma_corrected (TRUE, FALSE). Pre poziva ove funkcije pozvati funkciju SetCursorPosition za odredjivanje mesta ispisa slike
// predefinisane boje nalaze se u fajlu colors.h
// Refresh(); osvezava ekrana. S obzirom da je u pitanju displej sa pasivnom matricom da bi se prikazivao sadrzaj na ekranu neophodno je osvezavati ekran
// RefreshDelay(t); // zaustavlja program za vreme t uz osvezavanje ekrana. Za t=1 program se zaustavlja na oko 5ms.
// NoRefreshDelay(t); // zaustavlja sarzaj za vreme t bez osvezavanja ekrana. 


/*************/

// setup - funkcije pozvane u delu setup izvrsavaju se samo jednom
// loop - funkcije pozvane u delu loop izvrsavaju se ciklicno


void setup(void)
{
    SetBitsPerChannel(8);
    ClearScreen();
    SetCursorPosition(0,0);
    
    // kod napisan na ovom mestu izvrsava se samo jednom na pocetku programa
}
void loop(void)
{
    int i;
    for (i = 0; i < 9; i++) {
        RandCir(RED);
        RefreshDelay(200);
    }
    
    
    ClearScreen();
    
    RefreshDelay(200);
    SetCursorPosition(5,10);
    PanelPutChar('P',RED);
    RefreshDelay(50);
    SetCursorPosition(11,11);
    PanelPutChar('E',RED);
    RefreshDelay(50);
    SetCursorPosition(17,12);
    PanelPutChar('C',RED);
    RefreshDelay(50);
    SetCursorPosition(23,13);
    PanelPutChar('E',RED);
    RefreshDelay(50);
    SetCursorPosition(39,12);
    PanelPutChar('N',RED);
    RefreshDelay(50);
    SetCursorPosition(45,11);
    PanelPutChar('J',RED);
    RefreshDelay(50);
    SetCursorPosition(51,10);
    PanelPutChar('E',RED);
    RefreshDelay(3000);
    Scroll(DOWN,30,10);
    ClearScreen();
    
    SetCursorPosition(0,0);
    LoadImage(random,FALSE);
    
    SetCursorPosition(32,0);
    LoadImage(random,FALSE);
    
    RefreshDelay(5000);
    
    

    // kod napisan na ovom mestu izvrsava se stalno (periodicno) 
}