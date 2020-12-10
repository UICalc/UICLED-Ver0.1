/* ---------------- UNITED INTERGALACTIC ---------------- */
/* ------------------------------------------------------ * 
 *                                                        * 
 *            _   _ _____  ___ _    ____  ____            * 
 *           | | | |_   _|/ __| |  | __ /|  _ \           * 
 *           | | | |_| |_| |__| |__| __ || |_| |          * 
 *            \___/|_____|\___|____|____\|____/           * 
 *                                                        * 
 *         THE INTELLEGENT LED ADJUSTMENT SOLUTION        * 
 *                 BY UNITED INTERGALACTIC                * 
 *                                                        * 
 *                      ALPHA VERSION                     * 
 *                 0.0.1.20201210_alpha_a20               * 
 *                                                        * 
 * ------------------------------------------------------ * 
 *                        THANKS TO                       * 
 *                           WSY                          * 
 *                           H B                          * 
 *  ----------------------------------------------------- */
#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>

/* EEPROM DRIVER START */
const int index_OC1_addr = 0;
const int index_OC2_addr = 1;
int OC1_addr;
int OC2_addr;
bool is_OC1_modified = false;
bool is_OC2_modified = false;
const int change_period = 50;
int change_counter = 0;
int OC1_change = 0;
int OC2_change = 0;
const int max_change = 20;
/* EEPROM DRIVER END */

/* Event DEFINITION START */
#define buttonPress 0x0001
#define buttonRelease 0x0002
#define buttonShortPress 0x0003
#define RRotate 0x0004
#define LRotate 0x0005
/* Event DEFINITION END */

/* OLED DRIVER START */
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const int protect_period = 100;
int protect_counter = 0;
bool isinverse = true;
const int shutdown_timeout = 600;
int shutdown_counter = 0;
bool isshutdown = false;
bool isblackscreen = false;
static const unsigned char PROGMEM gImage_ui[] = { 0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X30,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X38,0X01,0X80,0X81,0X60,0XAF,0XFB,0XFB,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X3C,0X03,0X00,0X81,0X70,0XA0,0X82,0X02,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X0C,0X06,0X40,0X81,0X58,0XA0,0X82,0X02,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X06,0X0C,0XC8,0X81,0X4C,0XA0,0X82,0XF2,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0X19,0X98,0X81,0X46,0XA0,0X82,0X02,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X01,0X93,0X30,0X81,0X43,0XA0,0X82,0X02,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X50,0X60,0X81,0X41,0XA0,0X82,0X02,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X10,0XC0,0XFF,0X40,0XA0,0X83,0XFA,0XFC,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0XC7,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X03,0XC7,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X06,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X0C,0X12,0X00,0XB0,0X5F,0XF7,0XF7,0XC7,0XF8,0X61,0X00,0X18,0X7F,0XBF,0XEB,0XFC,
0X19,0X90,0X00,0XB8,0X41,0X04,0X04,0X24,0X08,0X61,0X00,0X18,0X40,0X82,0X0A,0X04,
0X33,0X30,0XC0,0XAC,0X41,0X04,0X04,0X24,0X08,0X91,0X00,0X24,0X40,0X82,0X0A,0X04,
0X26,0X60,0XC0,0XA6,0X41,0X05,0XE4,0X24,0X00,0X91,0X00,0X24,0X40,0X02,0X0A,0X00,
0X04,0XC0,0X40,0XA3,0X41,0X04,0X07,0XC4,0X79,0X99,0X00,0X66,0X40,0X02,0X0A,0X00,
0X01,0X80,0X18,0XA1,0XC1,0X04,0X04,0X44,0X09,0X09,0X00,0X42,0X40,0X82,0X0A,0X04,
0X03,0X00,0X1C,0XA0,0XC1,0X04,0X04,0X24,0X0B,0X05,0X00,0XC1,0X40,0X82,0X0A,0X04,
0X02,0X00,0X18,0XA0,0X41,0X07,0XF4,0X37,0XFA,0X7D,0XFE,0X9F,0X7F,0X82,0X0B,0XFC,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};
/* OLED DRIVER END */

/* UI DEFINITION START */
int UI_STAT = 0;
#define UI_Shutdown 0xFFFE
#define UI_Startup 0x0000
#define UI_Main 0x0001
#define UI_Switch 0x0002
#define UI_OCSet 0x0003
  #define OCSet_OC1 0x0004
  #define OCSet_OC2 0x0005
#define UI_SIV 0x0006
bool is_UIMain_Drawed = false;
int UI_CUR_PAGE = UI_Startup;
int UI_TARG_PAGE = UI_Main;
bool UI_NEED_UPDATE = true;
/* UI DEFINITION END */

/* Main_SIV VARs START */
bool isFull = false;
const int MainSIV_fresh_period = 5;
int MainSIV_fresh_counter = MainSIV_fresh_period - 1;
int OC1_Rec[50], OC2_Rec[50], OCArr = 0, OC1_AVG, OC2_AVG;
const int R1_x = 8, R1_y = 17, R2_x = 68, R2_y = 17;
const int MainSIV_x = 50, MainSIV_y = 12;
/* Main_SIV VARs END */

/* BUTTON CONFIG START */
bool ledstate = true, buttonstate;
int count = 0;
unsigned long long t1, t2;
/* BUTTON CONFIG END */

/* PWM CONFIG START */
int OC1, OC2;
int input;
const int OC_Ratio = 4;
/* PWM CONFIG END */
void init_EEPROM() {
  OC1_addr = EEPROM.read(index_OC1_addr);
  OC2_addr = EEPROM.read(index_OC2_addr);
  if(OC1_addr == 0 || OC1_addr == 255) {
    OC1_addr = 2;
    EEPROM.update(index_OC1_addr, OC1_addr);
  }
  if(OC2_addr == 0 || OC2_addr == 255) {
    OC2_addr = 3;
    EEPROM.update(index_OC2_addr, OC2_addr);
  }
  OC1 = EEPROM.read(OC1_addr);
  OC2 = EEPROM.read(OC2_addr);
  if(OC1 > 100) OC1 = 100;
  if(OC2 > 100) OC2 = 100;
}
void save_oc() {
  if(is_OC1_modified) {
    Serial.println("OC1 ADDR UPDATE");
    OC1_change++;
    if(OC1_change >= max_change) {
      OC1_change = 0;
      bool ok = false;
      do {
        OC1_addr++;
        if(OC1_addr > 200) OC1_addr = 2;
        ok = (OC1_addr != OC2_addr) && (OC1_addr <= 1000) && (OC1_addr >= 2);
      } while(!ok);
      EEPROM.update(index_OC1_addr, OC1_addr);
      Serial.print("OC1 INDEX UPDATE TO ");
      Serial.print(OC1_addr);
    }
    is_OC1_modified = false;
    EEPROM.update(OC1_addr, OC1);
  }
  if(is_OC2_modified) {
    Serial.println("OC2 ADDR UPDATE");
    OC2_change++;
    if(OC2_change >= max_change) {
      OC2_change = 0;
      bool ok = false;
      do {
        OC2_addr++;
        if(OC2_addr > 200) OC2_addr = 2;
        ok = (OC1_addr != OC2_addr) && (OC2_addr <= 1000) && (OC2_addr >= 2);
      } while(!ok);
      EEPROM.update(index_OC2_addr, OC2_addr);
      Serial.print("OC2 INDEX UPDATE TO ");
      Serial.print(OC2_addr);
    }
    is_OC2_modified = false;
    EEPROM.update(OC2_addr, OC2);
  }
}

void change_oc(int OCNum, bool isIncrease) {
  change_counter = 0;
  if(OCNum == 1) {
    if(OC1 <= 25 || OC1 >= 75) {
      if(isIncrease) OC1++;
      else OC1--;
    }
    else {
      if(isIncrease) OC1+= 3;
      else OC1 -= 3;
    }
    if(OC1 > 100) OC1=100;
    if(OC1 < 0) OC1 = 0;
    is_OC1_modified = true;
    UI_NEED_UPDATE = true;
  }
  else {
    if(OC2 <= 25 || OC2 >= 75) {
      if(isIncrease) OC2++;
      else OC2--;
    }
    else {
      if(isIncrease) OC2+= 3;
      else OC2 -= 3;
    }
    if(OC2 > 100) OC2=100;
    if(OC2 < 0) OC2 = 0;
    is_OC2_modified = true;
    UI_NEED_UPDATE = true;
  }
}

void draw_UIMain() {
  display.clearDisplay();
  display.setCursor(1,1);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.print("UICLED");
  display.setCursor(75,1);
  display.setTextSize(1);
  display.print("OC1");
  display.setCursor(75,9);
  display.print("OC2");
  display.fillRect(R1_x, R1_y, MainSIV_x + 2, MainSIV_y + 2, 1);
  display.fillRect(R1_x + 1, R1_y + 1, MainSIV_x, MainSIV_y, 0);
  display.fillRect(R2_x, R2_y, MainSIV_x + 2, MainSIV_y + 2, 1);
  display.fillRect(R2_x + 1, R2_y + 1, MainSIV_x, MainSIV_y, 0);
  display.display();
  is_UIMain_Drawed = true;
}
void fresh_UIMain_SIV() {
  MainSIV_fresh_counter++;
  if(MainSIV_fresh_counter >= MainSIV_fresh_period) {
    display.fillRect(R1_x + 1, R1_y + 1, MainSIV_x, MainSIV_y, 0);
    display.fillRect(R2_x + 1, R2_y + 1, MainSIV_x, MainSIV_y, 0);
    MainSIV_fresh_counter = 0;
    if(isFull) {
      OC1_AVG = 0;
      OC2_AVG = 0;
      for(int i = 0; i < MainSIV_x - 1; i++) {
        OC1_Rec[i] = OC1_Rec[i+1];
        OC2_Rec[i] = OC2_Rec[i+1];
        OC1_AVG += OC1_Rec[i];
        OC2_AVG += OC2_Rec[i];
      }
      OC1_AVG /= MainSIV_x;
      OC2_AVG /= MainSIV_x;
      OC1_Rec[MainSIV_x - 1] = (int)(OC1 / 100.0 * MainSIV_y);
      OC2_Rec[MainSIV_x - 1] = (int)(OC2 / 100.0 * MainSIV_y);
    }
    else {
      OC1_Rec[OCArr] = (int)(OC1 / 100.0 * MainSIV_y);
      OC2_Rec[OCArr] = (int)(OC2 / 100.0 * MainSIV_y);
      OCArr++;
      for(int i = 0; i < OCArr; i++) {
        OC1_AVG += OC1_Rec[i];
        OC2_AVG += OC2_Rec[i];
      }
      OC1_AVG /= OCArr;
      OC2_AVG /= OCArr;
      for(int i = 0; i < MainSIV_y; i++) {
        if(i % 2) {
          display.fillRect(R1_x + OCArr, R1_y + MainSIV_y - i + 1, 1, 1, 1);
        }
        if(i % 2) {
          display.fillRect(R2_x + OCArr, R2_y + MainSIV_y - i + 1, 1, 1, 1);
        }
      }
      if(OCArr >= MainSIV_x) isFull = true;
    }
    for(int i = 0; i < MainSIV_x; i++) {
      display.fillRect(R1_x + 1 + i, R1_y + MainSIV_y - OC1_Rec[i] + 1, 1, OC1_Rec[i], 1);
      display.fillRect(R2_x + 1 + i, R2_y + MainSIV_y - OC2_Rec[i] + 1, 1, OC2_Rec[i], 1);
      if(i % 2) {
        display.fillRect(R1_x + 1 + i, R1_y + MainSIV_y - OC1_AVG + 1, 1, 1, 1);
        display.fillRect(R2_x + 1 + i, R2_y + MainSIV_y - OC2_AVG + 1, 1, 1, 1);
      }
    }
  }
  display.display();
}
void fresh_UIMain() {
  display.fillRect(93, 1, 54, 16, 0);
  display.setCursor(95,1);
  display.setTextSize(1);
  display.print(OC1);
  display.print("%");
  display.setCursor(95,9);
  display.print(OC2);
  display.print("%");
  display.display();
}

//OLED LOGIC CONTROL CENTER
void OLEDCC() {
  if(UI_CUR_PAGE != UI_TARG_PAGE) {
    UI_NEED_UPDATE = true;
    switch(UI_TARG_PAGE) {
      case UI_Main:
        draw_UIMain();
        UI_CUR_PAGE = UI_TARG_PAGE;
        break;
      case UI_Startup:
        display.clearDisplay();
        display.drawBitmap(0, 0, gImage_ui, 128, 32, 1);
        display.display();
        UI_CUR_PAGE = UI_TARG_PAGE;
        break;
      case UI_Shutdown:
        display.fillRect(0, 0, 128, 32, 0);
        display.invertDisplay(false);
        UI_CUR_PAGE = UI_TARG_PAGE;
        display.display();
        break;
     }
  }
  if(UI_NEED_UPDATE) {
    switch(UI_CUR_PAGE) {
      case UI_Main:
        fresh_UIMain();
        UI_NEED_UPDATE = false;
        break;
    }
  }
  switch(UI_CUR_PAGE) {
    case UI_Main:
      fresh_UIMain_SIV();
      break;
  }
}

//EVENT LOGIC CONTROL CENTER
void EventCC(int EventID) {
  switch (EventID) {
    case buttonPress:
      if(isshutdown) {
        isshutdown = false;
        UI_NEED_UPDATE = true;
        UI_TARG_PAGE = UI_Main;
      }
      else {
        shutdown_counter = 0;
      }
      buttonstate = 1;
      break;
    case buttonRelease:
      buttonstate = 0;
      break;
    case buttonShortPress:
      if(isshutdown) {
        isshutdown = false;
        UI_NEED_UPDATE = true;
        UI_TARG_PAGE = UI_Main;
        return;
      }
      else {
        shutdown_counter = 0;
      }
      switch (UI_TARG_PAGE) {
        case UI_Main:
          UI_TARG_PAGE = UI_Startup;
          break;
        case UI_Startup:
          UI_TARG_PAGE = UI_Main;
          break;
        default:
          break;
      }
      break;
    case LRotate:
      if(buttonstate) change_oc(2, 0);
      else change_oc(1, 0);
      break;
    case RRotate:
      if(buttonstate) change_oc(2, 1);
      else change_oc(1, 1);
      break;
    default:
      break;
  }
}

/* PWM FUNC START */
void setupPWM(uint16_t max_count) {
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                    /* no prescaling */
    ICR1 = max_count;                      /* TOP counter value */
}
void analogWrite16(uint8_t pin, uint16_t val)
{
    switch (pin) {
    case  9: OCR1A = val; break;
    case 10: OCR1B = val; break;
    }
}
/* PWM FUNC END */

/* MUSIC START */
//#define NOTE_D0 -1
//#define NOTE_D1 294
//#define NOTE_D2 330
//#define NOTE_D3 350
//#define NOTE_D4 393
//#define NOTE_D5 441
//#define NOTE_D6 495
//#define NOTE_D7 556
//
//#define NOTE_DL1 147
//#define NOTE_DL2 165
//#define NOTE_DL3 175
//#define NOTE_DL4 196
//#define NOTE_DL5 221
//#define NOTE_DL6 248
//#define NOTE_DL7 278
//
//#define NOTE_DH1 589
//#define NOTE_DH2 661
//#define NOTE_DH3 700
//#define NOTE_DH4 786
//#define NOTE_DH5 882
//#define NOTE_DH6 990
//#define NOTE_DH7 112
//
//#define WHOLE 1
//#define HALF 0.5
//#define QUARTER 0.25
//#define EIGHTH 0.25
//#define SIXTEENTH 0.625
//
////整首曲子的音符部分
//static const PROGMEM float tune_HLW[] =
//{
//  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0,
//  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0, NOTE_D6,
//  NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D6,
//  NOTE_DH1, NOTE_D6, NOTE_D5, NOTE_DH1, NOTE_D6, NOTE_D0,
//
//  NOTE_D1, NOTE_D1, NOTE_D3,
//  NOTE_D1, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,
//  NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_DH1, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D6,
//  NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,
//  NOTE_D7, NOTE_D7, NOTE_D5, NOTE_D3,
//  NOTE_D5,
//  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D3, NOTE_D0,
//  NOTE_DH1, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D0, NOTE_D5, NOTE_D1, NOTE_D2, NOTE_D0,
//  NOTE_D3, NOTE_D3, NOTE_D1, NOTE_DL6,
//  NOTE_D1,
//  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_D3, NOTE_D5, NOTE_D6, NOTE_D6,
//  NOTE_DH1, NOTE_D0, NOTE_D7, NOTE_D5,
//  NOTE_D6,
//};
//
////曲子的节拍，即音符持续时间
//static const PROGMEM float duration_HLW[] =
//{
//  1, 1, 0.5, 0.5, 1,
//  0.5, 0.5, 0.5, 0.5, 1, 0.5, 0.5,
//  0.5, 1, 0.5, 1, 0.5, 0.5,
//  0.5, 0.5, 0.5, 0.5, 1, 1,
//
//  1, 1, 1 + 1,
//  0.5, 1, 1 + 0.5, 1,
//  1, 1, 0.5, 0.5, 1,
//  0.5, 1, 1 + 0.5, 1,
//  0.5, 0.5, 0.5, 0.5, 1 + 1,
//  0.5, 1, 1 + 0.5, 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1,
//  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,
//  0.5, 1, 0.5, 1, 1,
//  0.5, 0.5, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25, 0.5 + 0.25, 0.25,
//  0.5, 1, 0.5, 1, 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1,
//  0.5, 1, 0.5, 1 + 1,
//  0.5, 1, 0.5, 1 + 1,
//  1 + 1, 0.5, 0.5, 1,
//  1 + 1 + 1 + 1
//};

#define NTD0 0
#define NTD1 441
#define NTD2 495
#define NTD3 556
#define NTD4 589
#define NTD5 661
#define NTD6 742
#define NTD7 833
#define NTDL1 221
#define NTDL2 248
#define NTDL3 278
#define NTDL4 294
#define NTDL5 330
#define NTDL6 371
#define NTDL7 416
#define NTDH1 882
#define NTDH2 990
#define NTDH3 1112
#define NTDH4 1178
#define NTDH5 1322
#define NTDH6 1484
#define NTDH7 1665
//列出全部D调的频率
#define WHOLE 1
#define HALF 0.5
#define QUARTER 0.25
#define EIGHTH 0.25
#define SIXTEENTH 0.625
//列出所有节拍
static const PROGMEM float tune_Astronomia[] =                 //根据简谱列出各频率
{
  NTD6,NTD0,NTD6,NTD0,NTD6,NTD0,NTD6,NTDH1,NTDH2,NTDH3,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTD5,NTD5,NTD6,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTD0,NTD6,NTDH1,NTDH2,NTDH3,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTDH4,NTDH4,NTDH3,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTD0,NTD6,NTDH1,NTDH2,NTDH3,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTD5,NTD5,NTD6,
 NTD6,NTD0,NTD6,NTD0,NTD6,NTD0,NTD6,NTDH1,NTDH2,NTDH3,
 NTD6,NTD5,NTD0,NTD5,NTD6,NTD0,NTD6,
 NTD0,NTD0,NTD6,NTD6,NTD5,NTD5,NTD5,NTD6,
 NTD5,NTD3,NTD3,NTD5,NTD3,
 NTD0,NTD0,NTD6,NTD6,NTD5,NTD6,NTD7,
 NTDH1,NTD7,NTD6,NTD7,NTD6,NTD5,
 NTD0,NTD0,NTD6,NTD6,NTD5,NTD5,NTD6,NTD5,
 NTD5,NTD3,NTD3,NTD5,NTD3,NTD3,NTD0,NTD5,
 NTD5,NTD6,NTD0,NTD5,NTD5,NTD6,NTD0,NTD6,NTD7,
 NTDH1,NTDH2,NTD6,NTD3,NTD3,NTD5,
 NTD5,NTD6,NTD0,NTDH3,NTDH2,NTD0,NTD6,
 NTD0,NTD6,NTD6,NTDH3,NTDH2,NTD0,
 NTD0,NTDH2,NTDH2,NTDH1,NTDH2,NTDH1,NTD6,NTD6,NTD5,
 NTD5,NTD5,NTD5,NTD6,NTD6,NTD3,NTD3,NTD5,
 NTD5,NTD6,NTD0,NTD3,NTDH2,NTD0,NTD6,
 NTD0,NTD6,NTD6,NTDH3,NTDH2,NTD0,
 NTD0,NTDH2,NTDH2,NTDH1,NTDH2,NTDH1,NTD6,NTD6,NTD3,NTD5,
 NTD5,NTD6,NTD6,NTD5,NTD6,NTD3,NTD0,NTD3,NTD5,
 NTD5,NTD6,NTDH3,NTDH3,NTD0,NTD7,NTDH1,
 
 NTDH2,NTDH1,NTDH2,NTDH2,NTDH3,NTD6,NTD0,NTD5,NTD5,
 NTD6,NTDH3,NTDH3,NTDH2,NTDH1,NTD6,NTD5,
 NTD6,NTD6,NTD6,NTDH1,NTDH2,NTD0,
 NTD6,NTD6,NTD6,NTD6,NTD5,NTD6,NTDH3,NTD0,NTDH1,
 NTDH2,NTDH1,NTDH2,NTDH2,NTDH3,NTDH3,NTD0,NTD5,
 NTD6,NTDH1,NTD0,NTD5,NTD6,NTDH3,NTD0,NTDH3,NTDH2,
  NTDH1, NTD7, NTD6, NTD5, NTD5, NTD6, NTD0,
   NTD3, NTD3, NTD5,
  NTD5,NTD6,NTD0,NTDH3,NTDH2,NTD0,NTD6,
  NTD0,NTD6,NTD6,NTDH3,NTDH2,NTD0,
  NTD0, NTDH2, NTDH2, NTDH1, NTDH2, NTDH1, NTD6, NTD6, NTD3, NTD5,
  NTD5, NTD6, NTD6, NTD5, NTD6, NTD3, NTD3, NTD5,
  NTD5, NTD6, NTD6, NTDH3, NTDH3, NTD0, NTD7, NTDH1,
 NTDH2,NTDH1,NTDH2,NTDH2,NTDH3,NTD6,NTD0,NTD5,NTD5,
 NTD6,NTDH3,NTDH3,NTDH2,NTDH1,NTD6,NTD5,
 NTD6,NTD6,NTD6,NTDH1,NTDH2,NTD0,
 NTD6,NTD6,NTD6,NTD6,NTD5,NTD6,NTDH3,NTD0,NTDH1,
 NTDH2,NTDH1,NTDH2,NTDH2,NTDH3,NTDH3,NTD0,NTD5,
 NTD6,NTDH1,NTD0,NTD5,NTD6,NTDH3,NTD0,NTDH3,NTDH2,
 NTDH1,NTD7,NTD6,NTD5,NTD5,NTD6,NTD0,NTD5,
 NTD6,NTDH1,NTD0,NTD5,NTD6,NTDH5,NTD0,NTDH3,NTDH2,
 NTDH2,NTDH2,NTDH3,NTDH5,NTDH5,NTDH6,
};
static const PROGMEM float duration_Astronomia[] =                   //根据简谱列出各节拍
{
 
 1,0.05,1,0.05,1,0.05,0.25,0.25,0.25,0.25,
 1,0.05,1,0.05,0.5,0.5,0.5,0.5,
 1,0.05,1,0.05,1,0.05,0.25,0.25,0.25,0.25,
 1,0.05,1,0.05,0.5,0.5,0.5,0.5,
 1,0.05,1,0.05,1,0.05,0.25,0.25,0.25,0.25,
 1,0.05,1,0.05,0.5,0.5,0.5,0.5,
 1,0.05,1,0.05,1,0.05,0.25,0.25,0.25,0.25,
1.5,0.5,0.05,0.5,0.5,0.05,1,
1,0.5,0.5,0.5,0.25,0.25,0.5,0.5,
0.5,0.5,1,0.25,2,
1,0.5,0.5,0.5,0.5,0.5,0.5,
1,1,0.5,0.25,0.25,1,
1,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
0.5,0.5,0.5,0.25,0.25,1,0.5,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
1,1,0.5,0.5,0.5,0.5,
0.5,0.5,0.5,0.5,1,0.5,0.5,
0.5,0.5,0.5,0.5,1,1,
0.5,0.5,0.5,0.5,0.5,0.25,0.25,0.5,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
0.5,0.5,0.5,0.5,1,0.5,0.5,
0.5,0.5,0.5,0.5,1,1,
0.5,0.5,0.5,0.5,0.5,0.25,0.25,0.25,0.25,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
0.5,1,0.5,1,0.5,0.25,0.25,
0.5,0.25,0.25,0.5,0.5,1,0.5,0.25,0.25
,1,1,0.25,0.5,0.5,0.5,0.5,
1,0.25,0.5,0.5,1,1
,0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,
0.5,0.25,0.25,0.5,0.5,1,0.5,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
0.5,0.5,0.5,0.5,0.5,1,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,1,0.5,0.5,0.5,0.5,0.5,0.5,1,1,
0.5,0.5,0.5,0.5,0.5,0.25,0.25,0.5,0.25,0.25,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
0.5,0.5,0.5,0.5,1,0.5,0.25,0.25,
0.5,0.25,0.25,0.5,0.5,1,0.5,0.25,0.25
,1,1,0.25,0.5,0.5,0.5,0.5,
1,0.25,0.5,0.5,1,1
,0.5,0.25,0.25,0.5,0.5,0.5,0.5,0.5,0.5,
0.5,0.25,0.25,0.5,0.5,1,0.5,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5,
0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.25,0.25,
0.5,0.5,0.5,0.5,1,1
};

int length;//定义一个变量用来表示共有多少个音符

void playMusic() {
//  pinMode(9, OUTPUT);
  interrupts();
  uint16_t fsd;
  length = sizeof(tune_Astronomia) / sizeof(tune_Astronomia[0]); //这里用了一个sizeof函数，查出数组里有多少个音符
  for (int x = 0; x < length; x++) //循环音符的次数
  {
//    //tone(9, tune[x]); //依次播放tune数组元素，即每个音符
    fsd = 8000000 / pgm_read_float_near(&tune_Astronomia[x]);
    Serial.println(fsd);
    setupPWM(fsd);
    analogWrite16(9, fsd / 100 * OC1);
    delay(400 * pgm_read_float_near(&duration_Astronomia[x])); //每个音符持续的时间，即节拍duration，400是调整时间的越大，曲子速度越慢，越小曲子速度越快
  }
  setupPWM(0x0190);
}
/* MUSIC END */

/* BUTTON FUNC START */
void pressbutton() {
  bool first, last;
  first = digitalRead(2);
  for(long long int i=1;i<=10;i++) {
    delayMicroseconds(1000);
  }
  last = digitalRead(2);
  if(first == last) {
    if(first) {
      Serial.println("Released.");
      t2 = millis();
      if(t2 - t1 < 300) {
        Serial.println("Short Press");
        EventCC(buttonShortPress);
      }
      if(t2 - t1 < 50) {
        Serial.println("Very Short Press, Trigger Music");
        playMusic();
        Serial.println("OK");
      }
      EventCC(buttonRelease);
    }
    else {
      Serial.println("Pressed.");
      EventCC(buttonPress);
      t1 = millis();
      ledstate = !ledstate;
    }
  }
  digitalWrite(13, ledstate);
}
/* BUTTON FUNC END */

/* ROTARY ENCODER FUNC START */
void rotate() {
  if(isshutdown) {
    isshutdown = false;
    UI_NEED_UPDATE = true;
    UI_TARG_PAGE = UI_Main;
  }
  else {
    shutdown_counter = 0;
  }
  bool first_3, first_4, issame = 1;
  first_3 = digitalRead(3);
  first_4 = digitalRead(4);
  delayMicroseconds(1000);
  if(first_3 != digitalRead(3)) issame = 0;
  if(first_4 != digitalRead(4)) issame = 0;
  if(issame){
    if(first_3){
      if(buttonstate) Serial.print("Button Pressed with ");
      if(first_4){
        EventCC(LRotate);
        count--;
        Serial.print("L Rotate. Current count: ");
        Serial.println(count);
        ledstate = !ledstate;
      }
      else {
        EventCC(RRotate);
        if(OC1 > 100) OC1=100;
        if(OC2 > 100) OC2=100;
        count++;
        Serial.print("R Rotate. Current count: ");
        Serial.println(count);
        ledstate = !ledstate;
      }
    }
  }
  digitalWrite(13, ledstate);
}
/* ROTARY ENCODER FUNC END */
void soft_setup() {
  int o1, o2, co1 = 0, co2 = 0;
  o1 = OC1 * OC_Ratio;
  o2 = OC2 * OC_Ratio;
  for(int t = 0; t < 500; t++) {
    co1 = (int)((t / 500.0) * o1);
    co2 = (int)((t / 500.0) * o2);
    analogWrite16(9, co1);
    analogWrite16(10, co2);
    delay(2);
  }
}
void setup() {
  //INITIALIZE PINS
  pinMode(13, OUTPUT);//STAT LED
  pinMode(2, INPUT);//ROTARY ENCODER BUTTON
  pinMode(3, INPUT);//ROTARY ENCODER A
  pinMode(4, INPUT);//ROTARY ENCODER B
  Serial.begin(9600);//Start Serial
  attachInterrupt(digitalPinToInterrupt(2), pressbutton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), rotate, CHANGE);
  setupPWM(0x0190);

  //INITIALIZE DISPLAY
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, gImage_ui, 128, 32, 1);
  display.display();
  //read EEPROM
  init_EEPROM();
  soft_setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  OLEDCC();
  analogWrite16(9, OC1*OC_Ratio);
  analogWrite16(10, OC2*OC_Ratio);
  delay(100);
  change_counter++;
  if(change_counter >= change_period) {
    change_counter = 0;
    save_oc();
    Serial.println("OCSave");
  }
  protect_counter++;
  if(protect_counter >= protect_period) {
    protect_counter = 0;
    if(!isshutdown) {
      display.invertDisplay(isinverse);
      isinverse = !isinverse;
    }
    else display.invertDisplay(false);
  }
  if(!isshutdown) {
    shutdown_counter++;
    if(shutdown_counter >= shutdown_timeout) {
      shutdown_counter = 0;
      isshutdown = true;
      UI_TARG_PAGE = UI_Shutdown;
    }
  }
}
