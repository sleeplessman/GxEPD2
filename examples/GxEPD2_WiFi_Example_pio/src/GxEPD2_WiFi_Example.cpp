#include <Arduino.h>
// GxEPD2_WiFi_Example : Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: https://www.good-display.com/companyfile/32/
//
// BMP handling code extracts taken from: https://github.com/prenticedavid/MCUFRIEND_kbv/tree/master/examples/showBMP_kbv_Uno
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2
//
// note that BMP bitmaps are drawn at physical position in physical orientation of the screen

// Supporting Arduino Forum Topics (closed, read only):
// Good Display ePaper for Arduino: https://forum.arduino.cc/t/good-display-epaper-for-arduino/419657
// Waveshare e-paper displays with SPI: https://forum.arduino.cc/t/waveshare-e-paper-displays-with-spi/467865
//
// Add new topics in https://forum.arduino.cc/c/using-arduino/displays/23 for new questions and issues

// see GxEPD2_wiring_examples.h for wiring suggestions and examples

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
// Note: if you use this with ENABLE_GxEPD2_GFX 1:
//       uncomment it in GxEPD2_GFX.h too, or add #include <GFX.h> before any #include <GxEPD2_GFX.h>

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_7C.h>

// NOTE: you may need to adapt or select for your wiring in the processor specific conditional compile sections below

// select the display class (only one), matching the kind of display panel
//#define GxEPD2_DISPLAY_CLASS GxEPD2_BW
//#define GxEPD2_DISPLAY_CLASS GxEPD2_3C
//#define GxEPD2_DISPLAY_CLASS GxEPD2_4C
#define GxEPD2_DISPLAY_CLASS GxEPD2_7C

// select the display driver class (only one) for your  panel
//#define GxEPD2_DRIVER_CLASS GxEPD2_102     // GDEW0102T4   80x128, UC8175, (WFT0102CZA2)
//#define GxEPD2_DRIVER_CLASS GxEPD2_150_BN  // DEPG0150BN 200x200, SSD1681, (FPC8101), TTGO T5 V2.4.1
//#define GxEPD2_DRIVER_CLASS GxEPD2_154     // GDEP015OC1  200x200, IL3829, (WFC0000CZ07), no longer available
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_D67 // GDEH0154D67 200x200, SSD1681, (HINK-E154A07-A1)
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_T8  // GDEW0154T8  152x152, UC8151 (IL0373), (WFT0154CZ17)
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_M09 // GDEW0154M09 200x200, JD79653A, (WFT0154CZB3)
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_M10 // GDEW0154M10 152x152, UC8151D, (WFT0154CZ17)
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_GDEY0154D67 // GDEY0154D67 200x200, SSD1681, (FPC-B001 20.05.21)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213     // GDE0213B1   122x250, IL3895, (HINK-E0213-G01), phased out
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_B72 // GDEH0213B72 122x250, SSD1675A (IL3897), (HINK-E0213A22-A0 SLH1852)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_B73 // GDEH0213B73 122x250, SSD1675B, (HINK-E0213A22-A0 SLH1914)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_B74 // GDEM0213B74 122x250, SSD1680, FPC-7528B)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_flex // GDEW0213I5F 104x212, UC8151 (IL0373), (WFT0213CZ16)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_M21 // GDEW0213M21 104x212, UC8151 (IL0373), (WFT0213CZ16)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_T5D // GDEW0213T5D 104x212, UC8151D, (WFT0213CZ16)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_BN // DEPG0213BN  122x250, SSD1680, (FPC-7528B), TTGO T5 V2.4.1, V2.3.1
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_GDEY0213B74 // GDEY0213B74 122x250, SSD1680, (FPC-A002 20.04.08)
//#define GxEPD2_DRIVER_CLASS GxEPD2_260     // GDEW026T0   152x296, UC8151 (IL0373), (WFT0154CZ17)
//#define GxEPD2_DRIVER_CLASS GxEPD2_260_M01 // GDEW026M01  152x296, UC8151 (IL0373), (WFT0260CZB2)
//#define GxEPD2_DRIVER_CLASS GxEPD2_266_BN // DEPG0266BN   152x296, SSD1680, (FPC7510), TTGO T5 V2.66, TTGO T5 V2.4.1
//#define GxEPD2_DRIVER_CLASS GxEPD2_266_GDEY0266T90 // GDEY0266T90 152x296, SSD1680, (FPC-A003 HB)
//#define GxEPD2_DRIVER_CLASS GxEPD2_270     // GDEW027W3   176x264, EK79652 (IL91874), (WFI0190CZ22)
//#define GxEPD2_DRIVER_CLASS GxEPD2_270_GDEY027T91 // GDEY027T91 176x264, SSD1680, (FB)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290     // GDEH029A1   128x296, SSD1608 (IL3820), (E029A01-FPC-A1 SYX1553)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_T5  // GDEW029T5   128x296, UC8151 (IL0373), (WFT0290CZ10)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_T5D // GDEW029T5D  128x296, UC8151D, (WFT0290CZ10)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_I6FD // GDEW029I6FD  128x296, UC8151D, (WFT0290CZ10)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_T94 // GDEM029T94  128x296, SSD1680, (FPC-7519 rev.b)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_T94_V2 // GDEM029T94  128x296, SSD1680, (FPC-7519 rev.b), Waveshare 2.9" V2 variant
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_BS // DEPG0290BS  128x296, SSD1680, (FPC-7519 rev.b)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_M06 // GDEW029M06  128x296, UC8151D, (WFT0290CZ10)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_GDEY029T94 // GDEY029T94 128x296, SSD1680, (FPC-A005 20.06.15)
//#define GxEPD2_DRIVER_CLASS GxEPD2_310_GDEQ031T10 // GDEQ031T10 240x320, UC8253, (no inking, backside mark KEGMO 3100)
//#define GxEPD2_DRIVER_CLASS GxEPD2_371     // GDEW0371W7  240x416, UC8171 (IL0324), (missing)
//#define GxEPD2_DRIVER_CLASS GxEPD2_370_TC1 // ED037TC1  280x480, SSD1677, (ICA-FU-20 ichia 2029), Waveshare 3.7"
//#define GxEPD2_DRIVER_CLASS GxEPD2_420     // GDEW042T2   400x300, UC8176 (IL0398), (WFT042CZ15)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420_M01 // GDEW042M01  400x300, UC8176 (IL0398), (WFT042CZ15)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420_GDEY042T81 // GDEY042T81 400x300, SSD1683 (no inking)
//#define GxEPD2_DRIVER_CLASS GxEPD2_426_GDEQ0426T82 // GDEQ0426T82 480x800, SSD1677 (P426010-MF1-A)
//#define GxEPD2_DRIVER_CLASS GxEPD2_583     // GDEW0583T7  600x448, UC8159c (IL0371), (missing)
//#define GxEPD2_DRIVER_CLASS GxEPD2_583_T8  // GDEW0583T8  648x480, EK79655 (GD7965), (WFT0583CZ61)
//#define GxEPD2_DRIVER_CLASS GxEPD2_583_GDEQ0583T31 // GDEQ0583T31  648x480, UC8179, (P583010-MF1-B)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750     // GDEW075T8   640x384, UC8159c (IL0371), (WF0583CZ09)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750_T7  // GDEW075T7   800x480, EK79655 (GD7965), (WFT0583CZ61)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750_YT7  // GDEY075T7  800x480, UC8179 (GD7965), (FPC-C001 20.8.20)
//#define GxEPD2_DRIVER_CLASS GxEPD2_1160_T91 // GDEH116T91 960x640, SSD1677, (none or hidden)
//#define GxEPD2_DRIVER_CLASS GxEPD2_1248     // GDEW1248T3 1304x984, UC8179, (WFT1248BZ23,WFT1248BZ24)
// 3-color e-papers
//#define GxEPD2_DRIVER_CLASS GxEPD2_154c     // GDEW0154Z04 200x200, IL0376F, (WFT0000CZ04), no longer available
//#define GxEPD2_DRIVER_CLASS GxEPD2_154_Z90c // GDEH0154Z90 200x200, SSD1681, (HINK-E154A07-A1)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213c     // GDEW0213Z16 104x212, UC8151 (IL0373), (WFT0213CZ16)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_Z19c // GDEH0213Z19 104x212, UC8151D, (HINK-E0213A20-A2 2020-11-19)
//#define GxEPD2_DRIVER_CLASS GxEPD2_213_Z98c // GDEY0213Z98 122x250, SSD1680, (FPC-A002 20.04.08)
//#define GxEPD2_DRIVER_CLASS GxEPD2_266c     // GDEY0266Z90 152x296, SSD1680, (FPC-7510)
//#define GxEPD2_DRIVER_CLASS GxEPD2_270c     // GDEW027C44  176x264, IL91874, (WFI0190CZ22)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290c     // GDEW029Z10  128x296, UC8151 (IL0373), (WFT0290CZ10)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_Z13c // GDEH029Z13  128x296, UC8151D, (HINK-E029A10-A3 20160809)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290_C90c // GDEM029C90  128x296, SSD1680, (FPC-7519 rev.b)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420c     // GDEW042Z15  400x300, UC8176 (IL0398), (WFT0420CZ15)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420c_Z21 // GDEQ042Z21  400x300, UC8276, (hidden)
//#define GxEPD2_DRIVER_CLASS GxEPD2_583c     // GDEW0583Z21 600x448, UC8159c (IL0371), (missing)
//#define GxEPD2_DRIVER_CLASS GxEPD2_583c_Z83 // GDEW0583Z83 648x480, EK79655 (GD7965), (WFT0583CZ61)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750c     // GDEW075Z09  640x384, UC8159c (IL0371), (WF0583CZ09)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750c_Z08 // GDEW075Z08  800x480, EK79655 (GD7965), (WFT0583CZ61)
//#define GxEPD2_DRIVER_CLASS GxEPD2_750c_Z90 // GDEH075Z90  880x528, SSD1677, (HINK-E075A07-A0)
//#define GxEPD2_DRIVER_CLASS GxEPD2_1248c    // GDEY1248Z51 1304x984, UC8179, (WFT1248BZ23,WFT1248BZ24)
// 4-color e-paper
//#define GxEPD2_DRIVER_CLASS GxEPD2_266c_GDEY0266F51H // GDEY0266F51H 184x360, JD79667 (FPC-H006 22.04.02)
//#define GxEPD2_DRIVER_CLASS GxEPD2_290c_GDEY029F51H // GDEY029F51H 168x384, JD79667 (FPC-H004 22.03.24)
//#define GxEPD2_DRIVER_CLASS GxEPD2_300c     // Waveshare 3.00" 4-color
//#define GxEPD2_DRIVER_CLASS GxEPD2_420c_GDEY0420F51 // GDEY0420F51 400x300, HX8717 (no inking)
//#define GxEPD2_DRIVER_CLASS GxEPD2_437c     // Waveshare 4.37" 4-color
// 7-color e-paper
//#define GxEPD2_DRIVER_CLASS GxEPD2_565c // Waveshare 5.65" 7-color
#define GxEPD2_DRIVER_CLASS GxEPD2_730c_GDEY073D46 // GDEY073D46 800x480 7-color, (N-FPC-001 2021.11.26)
// grey levels parallel IF e-papers on Waveshare e-Paper IT8951 Driver HAT
//#define GxEPD2_DRIVER_CLASS GxEPD2_it60           // ED060SCT 800x600
//#define GxEPD2_DRIVER_CLASS GxEPD2_it60_1448x1072 // ED060KC1 1448x1072
//#define GxEPD2_DRIVER_CLASS GxEPD2_it78_1872x1404 // ED078KC2 1872x1404
//#define GxEPD2_DRIVER_CLASS GxEPD2_it103_1872x1404 // ES103TC1 1872x1404

// SS is usually used for CS. define here for easy change
#ifndef EPD_CS
#define EPD_CS SS
#endif

// somehow there should be an easier way to do this
#define GxEPD2_BW_IS_GxEPD2_BW true
#define GxEPD2_3C_IS_GxEPD2_3C true
#define GxEPD2_4C_IS_GxEPD2_4C true
#define GxEPD2_7C_IS_GxEPD2_7C true
#define GxEPD2_1248_IS_GxEPD2_1248 true
#define GxEPD2_1248c_IS_GxEPD2_1248c true
#define IS_GxEPD(c, x) (c##x)
#define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)
#define IS_GxEPD2_3C(x) IS_GxEPD(GxEPD2_3C_IS_, x)
#define IS_GxEPD2_4C(x) IS_GxEPD(GxEPD2_4C_IS_, x)
#define IS_GxEPD2_7C(x) IS_GxEPD(GxEPD2_7C_IS_, x)
#define IS_GxEPD2_1248(x) IS_GxEPD(GxEPD2_1248_IS_, x)
#define IS_GxEPD2_1248c(x) IS_GxEPD(GxEPD2_1248c_IS_, x)

#if defined (ESP8266)
#define MAX_DISPLAY_BUFFER_SIZE (81920ul-34000ul-36000ul) // ~34000 base use, WiFiClientSecure seems to need about 36k more to work (with certificates)
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS) || IS_GxEPD2_4C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif
// adapt the constructor parameters to your wiring
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=D8*/ EPD_CS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BUSY=D2*/ 4));
// mapping of Waveshare e-Paper ESP8266 Driver Board, new version
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=15*/ EPD_CS, /*DC=4*/ 4, /*RST=2*/ 2, /*BUSY=5*/ 5));
// mapping of Waveshare e-Paper ESP8266 Driver Board, old version
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=15*/ EPD_CS, /*DC=4*/ 4, /*RST=5*/ 5, /*BUSY=16*/ 16));
#undef MAX_DISPLAY_BUFFER_SIZE
#undef MAX_HEIGHT
#endif

#if defined(ESP32)
#define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS) || IS_GxEPD2_4C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif
// adapt the constructor parameters to your wiring
#if !IS_GxEPD2_1248(GxEPD2_DRIVER_CLASS) && !IS_GxEPD2_1248c(GxEPD2_DRIVER_CLASS)
#if defined(ARDUINO_LOLIN_D32_PRO)
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ EPD_CS, /*DC=*/ 0, /*RST=*/ 2, /*BUSY=*/ 15)); // my LOLIN_D32_PRO proto board
#else
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ EPD_CS, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // my suggested wiring and proto board
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ 5, /*DC=*/ 17, /*RST=*/ 16, /*BUSY=*/ 4)); // LILYGO_T5_V2.4.1
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ EPD_CS, /*DC=*/ 19, /*RST=*/ 4, /*BUSY=*/ 34)); // LILYGO® TTGO T5 2.66
//GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=5*/ EPD_CS, /*DC=*/ 2, /*RST=*/ 0, /*BUSY=*/ 4)); // e.g. TTGO T8 ESP32-WROVER
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=*/ 33, /*DC=*/ 27, /*RST=*/ 26, /*BUSY=*/ 25)); // Waveshare ESP32 Driver Board
#endif
#else // GxEPD2_1248 or GxEPD2_1248c
// Waveshare 12.48 b/w or b/w/r SPI display board and frame or Good Display 12.48 b/w panel GDEW1248T3 or b/w/r panel GDEY1248Z51
// general constructor for use with all parameters, e.g. for Waveshare ESP32 driver board mounted on connection board
GxEPD2_DISPLAY_CLASS < GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS) > display(GxEPD2_DRIVER_CLASS(/*sck=*/ 13, /*miso=*/ 12, /*mosi=*/ 14,
    /*cs_m1=*/ 23, /*cs_s1=*/ 22, /*cs_m2=*/ 16, /*cs_s2=*/ 19,
    /*dc1=*/ 25, /*dc2=*/ 17, /*rst1=*/ 33, /*rst2=*/ 5,
    /*busy_m1=*/ 32, /*busy_s1=*/ 26, /*busy_m2=*/ 18, /*busy_s2=*/ 4));
#endif
#undef MAX_DISPLAY_BUFFER_SIZE
#undef MAX_HEIGHT
#endif

#if defined(ARDUINO_UNOR4_WIFI)
#define MAX_DISPLAY_BUFFER_SIZE 16384ul // e.g. half of available RAM
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS) || IS_GxEPD2_4C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif
// adapt the constructor parameters to your wiring
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=*/ EPD_CS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
#endif

#if defined(ARDUINO_ARCH_RP2040)
#define MAX_DISPLAY_BUFFER_SIZE 131072ul // e.g. half of available ram
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#elif IS_GxEPD2_3C(GxEPD2_DISPLAY_CLASS) || IS_GxEPD2_4C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
#elif IS_GxEPD2_7C(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 2))
#endif
#if defined(ARDUINO_NANO_RP2040_CONNECT)
// adapt the constructor parameters to your wiring
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=*/ EPD_CS, /*DC=*/ 8, /*RST=*/ 9, /*BUSY=*/ 7));
#endif
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
GxEPD2_DISPLAY_CLASS<GxEPD2_DRIVER_CLASS, MAX_HEIGHT(GxEPD2_DRIVER_CLASS)> display(GxEPD2_DRIVER_CLASS(/*CS=*/ 9, /*DC=*/ 8, /*RST=*/ 12, /*BUSY=*/ 13)); // Waveshare Pico-ePaper-2.9
#endif
#undef MAX_DISPLAY_BUFFER_SIZE
#undef MAX_HEIGHT
#endif

#if defined (ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

const char* ssid     = "Xiaomi_JOE";
const char* password = "tcinterlib";
const int httpPort  = 80;
const int httpsPort = 443;

// note: the certificates have been moved to a separate header file, as R"CERT( destroys IDE Auto Format capability

#include "GxEPD2_github_raw_certs.h"

const char* certificate_rawcontent = cert_DigiCert_TLS_RSA_SHA256_2020_CA1; // ok, should work until 2031-04-13 23:59:59
//const char* certificate_rawcontent = github_io_chain_pem_first;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_second;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT
//const char* certificate_rawcontent = github_io_chain_pem_third;  // ok, should work until Tue, 21 Mar 2023 23:59:59 GMT

const char* host_rawcontent   = "raw.githubusercontent.com";
const char* path_rawcontent   = "/ZinggJM/GxEPD2/master/extras/bitmaps/";
const char* path_prenticedavid   = "/prenticedavid/MCUFRIEND_kbv/master/extras/bitmaps/";
const char* path_waveshare_c  = "/waveshare/e-Paper/master/RaspberryPi_JetsonNano/c/pic/";
const char* path_waveshare_py = "/waveshare/e-Paper/master/RaspberryPi_JetsonNano/python/pic/";
const char* fp_rawcontent     = "8F 0E 79 24 71 C5 A7 D2 A7 46 76 30 C1 3C B7 2A 13 B0 01 B2"; // as of 29.7.2022

// note that BMP bitmaps are drawn at physical position in physical orientation of the screen
void showBitmapFrom_HTTP(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color = true);
void showBitmapFrom_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, bool with_color = true,
                          const char* certificate = certificate_rawcontent);

// draws BMP bitmap according to set orientation
void showBitmapFrom_HTTP_Buffered(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color = true);
void showBitmapFrom_HTTPS_Buffered(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, bool with_color = true,
                                   const char* certificate = certificate_rawcontent);

void setClock();
void drawBitmapsBuffered_7C();
void drawBitmapsBuffered_200x200();
void drawBitmapsBuffered_other();
void drawBitmaps_200x200();
void drawBitmaps_other();
void drawBitmaps_test();
void drawBitmapsBuffered_test();
uint32_t skip(WiFiClient& client, int32_t bytes);
void readBytes(WiFiClient& client, uint8_t* buffer, int32_t bytes);

void drawBitmapFrom_HTTP_ToBuffer(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color);
void drawBitmapFrom_HTTPS_ToBuffer(const char* host, const char* path, const char* filename, const char* fingerprint, 
  int16_t x, int16_t y, bool with_color, const char* certificate);
uint16_t read16(WiFiClient& client);
uint32_t read32(WiFiClient& client);
uint32_t read8n(WiFiClient& client, uint8_t* buffer, int32_t bytes);

#if defined(ESP32)
// uncomment next line to use HSPI for EPD (and VSPI for SD), e.g. with Waveshare ESP32 Driver Board
#define USE_HSPI_FOR_EPD
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
// SPIClassRP2040(spi_inst_t *spi, pin_size_t rx, pin_size_t cs, pin_size_t sck, pin_size_t tx);
// uncomment next line for use with Waveshare Pico-ePaper-2.9.
SPIClassRP2040 SPIn(spi1, 12, 13, 10, 11); // need be valid pins for same SPI channel, else fails blinking 4 long 4 short
#endif

#if defined(ESP32) && defined(USE_HSPI_FOR_EPD)
SPIClass hspi(HSPI);
#endif

void setup()
{
#if defined(ESP32) && defined(USE_HSPI_FOR_EPD)
  hspi.begin(13, 12, 14, 15); // remap hspi for EPD (swap pins)
  display.epd2.selectSPI(hspi, SPISettings(4000000, MSBFIRST, SPI_MODE0));
#endif
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
  delay(2000); // USB serial available
  display.epd2.selectSPI(SPIn, SPISettings(4000000, MSBFIRST, SPI_MODE0));
#endif

  Serial.begin(115200);
  Serial.println();
  Serial.println("GxEPD2_WiFi_Example");

  //display.init(115200); // default 10ms reset pulse, e.g. for bare panels with DESPI-C02
  display.init(115200, true, 2, false); // USE THIS for Waveshare boards with "clever" reset circuit, 2ms reset pulse

#if defined (ESP8266) || defined (ESP32)
#ifdef RE_INIT_NEEDED
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.disconnect();
#endif

  if (!WiFi.getAutoConnect() || ( WiFi.getMode() != WIFI_STA) || ((WiFi.SSID() != ssid) && String(ssid) != "........"))
  {
    Serial.println();
    Serial.print("WiFi.getAutoConnect() = ");
    Serial.println(WiFi.getAutoConnect());
    Serial.print("WiFi.SSID() = ");
    Serial.println(WiFi.SSID());
    WiFi.mode(WIFI_STA); // switch off AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }
#else
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
#endif
  int ConnectTimeout = 60; // 30 seconds
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
    if (--ConnectTimeout <= 0)
    {
      Serial.println();
      Serial.println("WiFi connect timeout");
      return;
    }
  }
  Serial.println();
  Serial.println("WiFi connected");

  // Print the IP address
  Serial.println(WiFi.localIP());

  setClock();

  if ((display.epd2.panel == GxEPD2::GDEW0154Z04) || (display.epd2.panel == GxEPD2::ACeP565) || (display.epd2.panel == GxEPD2::GDEY073D46) || false)
  {
    //drawBitmapsBuffered_200x200();
    //drawBitmapsBuffered_other();
    drawBitmapsBuffered_7C();
  }
  else
  {
    drawBitmaps_200x200();
    drawBitmaps_other();
  }

  //drawBitmaps_test();
  //drawBitmapsBuffered_test();

  Serial.println("GxEPD2_WiFi_Example done");
}

void loop(void)
{
}

void drawBitmaps_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
}

void drawBitmaps_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  //showBitmapFrom_HTTP("www.packescape.com", "/img/assets/", "IniciMenusTV2.bmp", w2 - 200, h2 - 150, false);
  //delay(2000);
  showBitmapFrom_HTTP("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, w2 - 100, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, w2 - 120, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, w2 - 60, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, w2 - 160, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, w2 - 89, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, w2 - 100, h2 - 100);
  delay(2000);
}

void drawBitmaps_test()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTPS(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "output5.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "output6.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_1.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_4.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_8.bmp", fp_rawcontent, 0, 0); // format 1: BI_RLE8 is not supported
  //delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_11.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_44.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "tractor_88.bmp", fp_rawcontent, 0, 0);
  delay(2000);
  //showBitmapFrom_HTTPS(host_rawcontent, path_rawcontent, "liberato640x384.bmp", fp_rawcontent, 0, 0);
  //delay(2000);
}

void drawBitmapsBuffered_200x200()
{
  int16_t x = (display.width() - 200) / 2;
  int16_t y = (display.height() - 200) / 2;
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "logo200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "first200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "second200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "third200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "fourth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "fifth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "sixth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "seventh200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "eighth200x200.bmp", fp_rawcontent, x, y);
  delay(2000);
}

void drawBitmapsBuffered_other()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTP_Buffered("www.squix.org", "/blog/wunderground/", "chanceflurries.bmp", w2 - 50, h2 - 50, false);
  delay(2000);
  //showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "betty_1.bmp", fp_rawcontent, w2 - 100, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "marilyn_240x240x8.bmp", fp_rawcontent, w2 - 120, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "miniwoof.bmp", fp_rawcontent, w2 - 60, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "test.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "tiger.bmp", fp_rawcontent, w2 - 160, h2 - 120);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "tiger_178x160x4.bmp", fp_rawcontent, w2 - 89, h2 - 80);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "tiger_240x317x4.bmp", fp_rawcontent, w2 - 120, h2 - 160);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "tiger_320x200x24.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "tiger16T.bmp", fp_rawcontent, w2 - 160, h2 - 100);
  delay(2000);
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "woof.bmp", fp_rawcontent, w2 - 100, h2 - 100);
  delay(2000);
}

void drawBitmapsBuffered_7C()
{
  if ((display.epd2.panel == GxEPD2::ACeP565) || (display.epd2.panel == GxEPD2::GDEY073D46))
  {
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_c, "5in65f.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_c, "5in65f2.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_c, "5in65f3.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_c, "5in65f4.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_py, "5in65f4.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_waveshare_py, "N-Color1.bmp", fp_rawcontent, 0, 0);
    delay(2000);

    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "displayed_bmp_small_but_padded.bmp", fp_rawcontent, 0, 0);
    delay(2000);
    showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_rawcontent, "displayed_bmp_large.bmp", fp_rawcontent, 0, 0);
    delay(2000);
  }
}

void drawBitmapsBuffered_test()
{
  int16_t w2 = display.width() / 2;
  int16_t h2 = display.height() / 2;
  showBitmapFrom_HTTPS_Buffered(host_rawcontent, path_prenticedavid, "betty_4.bmp", fp_rawcontent, w2 - 102, h2 - 126);
  delay(2000);
}

static const uint16_t input_buffer_pixels = 800; // may affect performance

static const uint16_t max_row_width = 1872; // for up to 7.8" display 1872x1404
static const uint16_t max_palette_pixels = 256; // for depth <= 8

uint8_t input_buffer[3 * input_buffer_pixels]; // up to depth 24
uint8_t output_row_mono_buffer[max_row_width / 8]; // buffer for at least one row of b/w bits
uint8_t output_row_color_buffer[max_row_width / 8]; // buffer for at least one row of color bits
uint8_t mono_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w
uint16_t rgb_palette_buffer[max_palette_pixels]; // palette buffer for depth <= 8 for buffered graphics, needed for 7-color display

void showBitmapFrom_HTTP(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color)
{
  WiFiClient client;
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd2.WIDTH) || (y >= display.epd2.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  if (read16(client) == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.epd2.WIDTH)  w = display.epd2.WIDTH  - x;
      if ((y + h - 1) >= display.epd2.HEIGHT) h = display.epd2.HEIGHT - y;
      if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish = false;
        bool colored = false;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint8_t out_color_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = read8n(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                uint32_t gotmore = read8n(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
            }
            switch (depth)
            {
              case 32:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                in_idx++; // skip alpha
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 2:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                }
                break;
            }
            if (whitish)
            {
              // keep white
            }
            else if (colored && with_color)
            {
              out_color_byte &= ~(0x80 >> col % 8); // colored
            }
            else
            {
              out_byte &= ~(0x80 >> col % 8); // black
            }
            if ((7 == col % 8) || (col == w - 1)) // write that last byte! (for w%8!=0 border)
            {
              output_row_color_buffer[out_idx] = out_color_byte;
              output_row_mono_buffer[out_idx++] = out_byte;
              out_byte = 0xFF; // white (for w%8!=0 border)
              out_color_byte = 0xFF; // white (for w%8!=0 border)
            }
          } // end pixel
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, output_row_color_buffer, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in ");
        Serial.print(millis() - startTime);
        Serial.println(" ms");
        display.refresh();
      }
      Serial.print("bytes read "); Serial.println(bytes_read);
    }
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void drawBitmapFrom_HTTP_ToBuffer(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color)
{
  WiFiClient client;
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  bool has_multicolors = (display.epd2.panel == GxEPD2::ACeP565) || (display.epd2.panel == GxEPD2::GDEY073D46);
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) return;
  display.fillScreen(GxEPD_WHITE);
  Serial.print("connecting to "); Serial.println(host);
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("http://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  if (read16(client) == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.width())  w = display.width()  - x;
      if ((y + h - 1) >= display.height()) h = display.height() - y;
      //if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish = false;
        bool colored = false;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
            //Serial.print("0x00"); Serial.print(red, HEX); Serial.print(green, HEX); Serial.print(blue, HEX);
            //Serial.print(" : "); Serial.print(whitish); Serial.print(", "); Serial.println(colored);
            rgb_palette_buffer[pn] = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
          }
        }
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint16_t color = GxEPD_WHITE;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = read8n(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                uint32_t gotmore = read8n(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
            }
            switch (depth)
            {
              case 32:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                in_idx++; // skip alpha
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                break;
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                    color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                    color = (msb << 8) | lsb;
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 2:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                  color = rgb_palette_buffer[pn];
                }
                break;
            }
            if (with_color && has_multicolors)
            {
              // keep color
            }
            else if (whitish)
            {
              color = GxEPD_WHITE;
            }
            else if (colored && with_color)
            {
              color = GxEPD_COLORED;
            }
            else
            {
              color = GxEPD_BLACK;
            }
            uint16_t yrow = y + (flip ? h - row - 1 : row);
            display.drawPixel(x + col, yrow, color);
          } // end pixel
        } // end line
      }
      Serial.print("bytes read "); Serial.println(bytes_read);
    }
  }
  Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void showBitmapFrom_HTTP_Buffered(const char* host, const char* path, const char* filename, int16_t x, int16_t y, bool with_color)
{
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  display.setFullWindow();
  display.firstPage();
  do
  {
    drawBitmapFrom_HTTP_ToBuffer(host, path, filename, x, y, with_color);
  }
  while (display.nextPage());
}

void showBitmapFrom_HTTPS(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, bool with_color, const char* certificate)
{
  // Use WiFiClientSecure class to create TLS connection
#if defined (ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List cert(certificate ? certificate : certificate_rawcontent);
#else
  WiFiClientSecure client;
#endif
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  uint32_t startTime = millis();
  if ((x >= display.epd2.WIDTH) || (y >= display.epd2.HEIGHT)) return;
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  Serial.print("connecting to "); Serial.println(host);
#if defined (ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
  client.setBufferSizes(4096, 4096); // required
  //client.setBufferSizes(8192, 4096); // may help for some sites
  if (certificate) client.setTrustAnchors(&cert);
  else if (fingerprint) client.setFingerprint(fingerprint);
  else client.setInsecure();
#elif defined (ESP32)
  if (certificate) client.setCACert(certificate);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  //if (read16(client) == 0x4D42) // BMP signature
  uint16_t signature = 0;
  for (int16_t i = 0; i < 50; i++)
  {
    if (!client.available()) delay(100);
    else signature = read16(client);
    //Serial.print("signature: 0x"); Serial.println(signature, HEX);
    if (signature == 0x4D42) break;
  }
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.epd2.WIDTH)  w = display.epd2.WIDTH  - x;
      if ((y + h - 1) >= display.epd2.HEIGHT) h = display.epd2.HEIGHT - y;
      if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish = false;
        bool colored = false;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
          }
        }
        display.clearScreen();
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint8_t out_byte = 0xFF; // white (for w%8!=0 border)
          uint8_t out_color_byte = 0xFF; // white (for w%8!=0 border)
          uint32_t out_idx = 0;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = read8n(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                //if ((get - got) > client.available()) delay(200); // does improve? yes, if >= 200
                uint32_t gotmore = read8n(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
            }
            switch (depth)
            {
              case 32:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                in_idx++; // skip alpha
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 2:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                }
                break;
            }
            if (whitish)
            {
              // keep white
            }
            else if (colored && with_color)
            {
              out_color_byte &= ~(0x80 >> col % 8); // colored
            }
            else
            {
              out_byte &= ~(0x80 >> col % 8); // black
            }
            if ((7 == col % 8) || (col == w - 1)) // write that last byte! (for w%8!=0 border)
            {
              output_row_color_buffer[out_idx] = out_color_byte;
              output_row_mono_buffer[out_idx++] = out_byte;
              out_byte = 0xFF; // white (for w%8!=0 border)
              out_color_byte = 0xFF; // white (for w%8!=0 border)
            }
          } // end pixel
          int16_t yrow = y + (flip ? h - row - 1 : row);
          display.writeImage(output_row_mono_buffer, output_row_color_buffer, x, yrow, w, 1);
        } // end line
        Serial.print("downloaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
        Serial.print("bytes read "); Serial.println(bytes_read);
        display.refresh();
      }
    }
  }
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void drawBitmapFrom_HTTPS_ToBuffer(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, bool with_color, const char* certificate)
{
  // Use WiFiClientSecure class to create TLS connection
#if defined (ESP8266)
  BearSSL::WiFiClientSecure client;
  BearSSL::X509List cert(certificate ? certificate : certificate_rawcontent);
#else
  WiFiClientSecure client;
#endif
  bool connection_ok = false;
  bool valid = false; // valid format to be handled
  bool flip = true; // bitmap is stored bottom-to-top
  bool has_multicolors = (display.epd2.panel == GxEPD2::ACeP565) || (display.epd2.panel == GxEPD2::GDEY073D46);
  uint32_t startTime = millis();
  if ((x >= display.width()) || (y >= display.height())) return;
  display.fillScreen(GxEPD_WHITE);
  Serial.print("connecting to "); Serial.println(host);
#if defined (ESP8266)
  client.setBufferSizes(4096, 4096); // required
  //client.setBufferSizes(8192, 4096); // may help for some sites
  if (certificate) client.setTrustAnchors(&cert);
  else if (fingerprint) client.setFingerprint(fingerprint);
  else client.setInsecure();
#elif defined (ESP32)
  if (certificate) client.setCACert(certificate);
#endif
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.print("requesting URL: ");
  Serial.println(String("https://") + host + path + filename);
  client.print(String("GET ") + path + filename + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: GxEPD2_WiFi_Example\r\n" +
               "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (!connection_ok)
    {
      connection_ok = line.startsWith("HTTP/1.1 200 OK");
      if (connection_ok) Serial.println(line);
      //if (!connection_ok) Serial.println(line);
    }
    if (!connection_ok) Serial.println(line);
    //Serial.println(line);
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  if (!connection_ok) return;
  // Parse BMP header
  //if (read16(client) == 0x4D42) // BMP signature
  uint16_t signature = 0;
  for (int16_t i = 0; i < 50; i++)
  {
    if (!client.available()) delay(100);
    else signature = read16(client);
    if (signature == 0x4D42)
    {
      //Serial.print("signature wait loops: "); Serial.println(i);
      break;
    }
  }
  if (signature == 0x4D42) // BMP signature
  {
    uint32_t fileSize = read32(client);
    uint32_t creatorBytes = read32(client); (void)creatorBytes; //unused
    uint32_t imageOffset = read32(client); // Start of image data
    uint32_t headerSize = read32(client);
    uint32_t width  = read32(client);
    int32_t height = (int32_t) read32(client);
    uint16_t planes = read16(client);
    uint16_t depth = read16(client); // bits per pixel
    uint32_t format = read32(client);
    uint32_t bytes_read = 7 * 4 + 3 * 2; // read so far
    if ((planes == 1) && ((format == 0) || (format == 3))) // uncompressed is handled, 565 also
    {
      Serial.print("File size: "); Serial.println(fileSize);
      Serial.print("Image Offset: "); Serial.println(imageOffset);
      Serial.print("Header size: "); Serial.println(headerSize);
      Serial.print("Bit Depth: "); Serial.println(depth);
      Serial.print("Image size: ");
      Serial.print(width);
      Serial.print('x');
      Serial.println(height);
      // BMP rows are padded (if needed) to 4-byte boundary
      uint32_t rowSize = (width * depth / 8 + 3) & ~3;
      if (depth < 8) rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
      if (height < 0)
      {
        height = -height;
        flip = false;
      }
      uint16_t w = width;
      uint16_t h = height;
      if ((x + w - 1) >= display.width())  w = display.width()  - x;
      if ((y + h - 1) >= display.height()) h = display.height() - y;
      //if (w <= max_row_width) // handle with direct drawing
      {
        valid = true;
        uint8_t bitmask = 0xFF;
        uint8_t bitshift = 8 - depth;
        uint16_t red, green, blue;
        bool whitish = false;
        bool colored = false;
        if (depth == 1) with_color = false;
        if (depth <= 8)
        {
          if (depth < 8) bitmask >>= depth;
          //bytes_read += skip(client, 54 - bytes_read); //palette is always @ 54
          bytes_read += skip(client, imageOffset - (4 << depth) - bytes_read); // 54 for regular, diff for colorsimportant
          for (uint16_t pn = 0; pn < (1 << depth); pn++)
          {
            blue  = client.read();
            green = client.read();
            red   = client.read();
            client.read();
            bytes_read += 4;
            whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
            colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
            if (0 == pn % 8) mono_palette_buffer[pn / 8] = 0;
            mono_palette_buffer[pn / 8] |= whitish << pn % 8;
            if (0 == pn % 8) color_palette_buffer[pn / 8] = 0;
            color_palette_buffer[pn / 8] |= colored << pn % 8;
            //Serial.print("0x00"); Serial.print(red, HEX); Serial.print(green, HEX); Serial.print(blue, HEX);
            //Serial.print(" : "); Serial.print(whitish); Serial.print(", "); Serial.println(colored);
            rgb_palette_buffer[pn] = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
          }
        }
        uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
        //Serial.print("skip "); Serial.println(rowPosition - bytes_read);
        bytes_read += skip(client, rowPosition - bytes_read);
        for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) // for each line
        {
          if (!connection_ok || !(client.connected() || client.available())) break;
          delay(1); // yield() to avoid WDT
          uint32_t in_remain = rowSize;
          uint32_t in_idx = 0;
          uint32_t in_bytes = 0;
          uint8_t in_byte = 0; // for depth <= 8
          uint8_t in_bits = 0; // for depth <= 8
          uint16_t color = GxEPD_WHITE;
          for (uint16_t col = 0; col < w; col++) // for each pixel
          {
            yield();
            if (!connection_ok || !(client.connected() || client.available())) break;
            // Time to read more pixel data?
            if (in_idx >= in_bytes) // ok, exact match for 24bit also (size IS multiple of 3)
            {
              uint32_t get = in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain;
              uint32_t got = read8n(client, input_buffer, get);
              while ((got < get) && connection_ok)
              {
                //Serial.print("got "); Serial.print(got); Serial.print(" < "); Serial.print(get); Serial.print(" @ "); Serial.println(bytes_read);
                uint32_t gotmore = read8n(client, input_buffer + got, get - got);
                got += gotmore;
                connection_ok = gotmore > 0;
              }
              in_bytes = got;
              in_remain -= got;
              bytes_read += got;
            }
            if (!connection_ok)
            {
              Serial.print("Error: got no more after "); Serial.print(bytes_read); Serial.println(" bytes read!");
              break;
            }
            switch (depth)
            {
              case 32:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                in_idx++; // skip alpha
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                break;
              case 24:
                blue = input_buffer[in_idx++];
                green = input_buffer[in_idx++];
                red = input_buffer[in_idx++];
                whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                break;
              case 16:
                {
                  uint8_t lsb = input_buffer[in_idx++];
                  uint8_t msb = input_buffer[in_idx++];
                  if (format == 0) // 555
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                    red   = (msb & 0x7C) << 1;
                    color = ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
                  }
                  else // 565
                  {
                    blue  = (lsb & 0x1F) << 3;
                    green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                    red   = (msb & 0xF8);
                    color = (msb << 8) | lsb;
                  }
                  whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                  colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0)); // reddish or yellowish?
                }
                break;
              case 1:
              case 2:
              case 4:
              case 8:
                {
                  if (0 == in_bits)
                  {
                    in_byte = input_buffer[in_idx++];
                    in_bits = 8;
                  }
                  uint16_t pn = (in_byte >> bitshift) & bitmask;
                  whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                  in_byte <<= depth;
                  in_bits -= depth;
                  color = rgb_palette_buffer[pn];
                }
                break;
            }
            if (with_color && has_multicolors)
            {
              // keep color
            }
            else if (whitish)
            {
              color = GxEPD_WHITE;
            }
            else if (colored && with_color)
            {
              color = GxEPD_COLORED;
            }
            else
            {
              color = GxEPD_BLACK;
            }
            uint16_t yrow = y + (flip ? h - row - 1 : row);
            display.drawPixel(x + col, yrow, color);
          } // end pixel
        } // end line
      }
      Serial.print("bytes read "); Serial.println(bytes_read);
    }
  }
  Serial.print("loaded in "); Serial.print(millis() - startTime); Serial.println(" ms");
  client.stop();
  if (!valid)
  {
    Serial.println("bitmap format not handled.");
  }
}

void showBitmapFrom_HTTPS_Buffered(const char* host, const char* path, const char* filename, const char* fingerprint, int16_t x, int16_t y, bool with_color, const char* certificate)
{
  Serial.println(); Serial.print("downloading file \""); Serial.print(filename);  Serial.println("\"");
  display.setFullWindow();
  display.firstPage();
  do
  {
    drawBitmapFrom_HTTPS_ToBuffer(host, path, filename, fingerprint, x, y, with_color, certificate);
  }
  while (display.nextPage());
}

uint16_t read16(WiFiClient& client)
{
  // BMP data is stored little-endian, same as Arduino.
  uint16_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read(); // MSB
  return result;
}

uint32_t read32(WiFiClient& client)
{
  // BMP data is stored little-endian, same as Arduino.
  uint32_t result;
  ((uint8_t *)&result)[0] = client.read(); // LSB
  ((uint8_t *)&result)[1] = client.read();
  ((uint8_t *)&result)[2] = client.read();
  ((uint8_t *)&result)[3] = client.read(); // MSB
  return result;
}

#if USE_BearSSL

uint32_t skip(BearSSL::WiFiClientSecure& client, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      client.read();
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

uint32_t read8n(BearSSL::WiFiClientSecure& client, uint8_t* buffer, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      int16_t v = client.read();
      *buffer++ = uint8_t(v);
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

#endif

uint32_t skip(WiFiClient& client, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      client.read();
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

uint32_t read8n(WiFiClient& client, uint8_t* buffer, int32_t bytes)
{
  int32_t remain = bytes;
  uint32_t start = millis();
  while ((client.connected() || client.available()) && (remain > 0))
  {
    if (client.available())
    {
      int16_t v = client.read();
      *buffer++ = uint8_t(v);
      remain--;
    }
    else delay(1);
    if (millis() - start > 2000) break; // don't hang forever
  }
  return bytes - remain;
}

// Set time via NTP, as required for x.509 validation
void setClock()
{
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}
