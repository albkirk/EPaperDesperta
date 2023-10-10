/* ****************************************************/
//#include <GFX.h>
#include <GxEPD2_BW.h>

GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(CS_PIN, DC_PIN, RST_PIN, BUSY_PIN)); // GDEH0213B73
uint16_t MainColor=GxEPD_BLACK;
uint16_t BGColor=GxEPD_WHITE;
uint16_t SetColor=GxEPD_BLACK;      //GxEPD_DARKGREY;
uint16_t EditColor=GxEPD_BLACK;     //GxEPD_LIGHTGREY;
uint16_t WarnColor=GxEPD_BLACK;     //GxEPD_WHITE;
uint16_t ECOColor=GxEPD_BLACK;      //GxEPD_WHITE;
uint16_t DebugColor=GxEPD_BLACK;    //GxEPD_WHITE;

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique18pt7b.h>
#include <Fonts/FreeSerif24pt7b.h>
#include <Fonts/FreeSans24pt7b.h>

// sammple code from \\Despertador-ZEN\.pio\libdeps\esp32dev\GxEPD2\examples\GxEPD2_NotPagedExample
// void showBox(
// void drawCornerTest()



#define Display_Width      display.width()                          // X   TFT=128  Epaper=212
#define Display_Height     display.height()                         // Y   TFT=128  Epaper=104
int Time_x = 40;
int Time_y = 56;
int T_NN = 72;
int T_Sep = 36;
int Date_x = 7;
int Date_y = 105;
int D_Comm = 36;
int D_Coll = 10;
bool Display_ON = false;


void display_powerOff() {
    display.powerOff();
    Display_ON = false;
}
    
void display_clear(uint16_t color=BGColor) {
    //display.fillScreen(BGColor);
    display.display();
    display.firstPage();
    do {
        display.fillScreen(color);
    } while(display.nextPage());
}

void display_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (Display_ON) {
        display.setFullWindow();
        display.drawRect(x, y, w, h, color);
        display.displayWindow(x, y, w, h);
        telnet_println("display_DrawRect");
    }
}

void display_FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if (Display_ON) {
        display.setFullWindow();
        display.fillRect(x, y, w, h, color);
        display.displayWindow(x, y, w, h);
        telnet_println("display_FillRect");
    }
}

void display_Image(const unsigned char *image , uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, byte invert = true, byte rotation = EPaperRotate) {
    uint16_t rot_x; 
    uint16_t rot_y;
    uint16_t rot_w = w; 
    uint16_t rot_h = h;
    if (Display_ON) {
       if(color == BGColor) { display_FillRect(x, y, w, h, color); }
       else {
           switch (rotation) {
               case 0:
                   rot_x = x; 
                   rot_y = y;
                   break;
               case 1:
                   rot_x = Display_Height - y - h;
                   rot_y = x;
                   break;
               case 2:
                   rot_x = Display_Width - x - w;      // to be confirmed 
                   rot_y = Display_Height - y - h;     // to be confirmed
                   break;
               case 3:
                   rot_x = y;
                   rot_y = Display_Width - x - w; 
                   break;
           };
           if (invert) display.writeImage(image, rot_x, rot_y, rot_w, rot_h, !color);
           else display.writeImage(image, rot_x, rot_y, rot_w, rot_h, color);
       };
    }
}

void display_text(String text, uint16_t color=MainColor, uint8_t s=1, int x=0, int y=0, const GFXfont *font = NULL, bool wraptext=false, uint16_t bgcolor=BGColor) {
    if (Display_ON) {
        display.setCursor(x, y);
        display.setTextColor(color, bgcolor);
        if(font != NULL) display.setFont(font);
        else display.setFont();
        display.setTextSize(s);
        int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
        display.getTextBounds(text, x, y, &tbx, &tby, &tbw, &tbh);
        display.setTextWrap(wraptext);
        display.setPartialWindow(x, y, tbw, tbh);
        display.firstPage();
        do {
            display.print(text);
        } while (display.nextPage());
    }
}

void display_drawhour(strDateTime strDtTm, uint16_t color, int x=Time_x, int y=Time_y) {
      String text="";
      if (strDtTm.hour <=9) text = String("0") + String(strDtTm.hour);
      else text = String(strDtTm.hour);
      display_text(text,        color, 6, x,      y);
      //display_text(text,        color, 1, x,      y, &FreeSerif24pt7b);
}

void display_Hcollon(uint16_t color, int x=Time_x, int y=Time_y) {
      String text="";
      display_text(String(':'), color, 6, x + T_NN, y);
}

void display_drawmin(strDateTime strDtTm, uint16_t color, int x=Time_x, int y=Time_y) {
      String text="";
      if (strDtTm.minute <=9) text = String("0") + String(strDtTm.minute);
      else text = String(strDtTm.minute);
      display_text(text,      color, 6, x + T_NN + T_Sep, y);
      //display_text(text,      color, 1, x + 55, y, &FreeSerif24pt7b);
}

void display_drawsec(strDateTime strDtTm, uint16_t color, int x=Time_x, int y=Time_y) {
      String text="";
      if (strDtTm.second <=9) text = String("0") + String(strDtTm.second);
      else text = String(strDtTm.second);
      display_text(text,      color, 2, x + (T_NN * 2 + T_Sep), y);
}


void display_drawyear(strDateTime strDtTm, uint16_t color, int x=Date_x, int y=Date_y) {
      String text="";
      if (strDtTm.year <=9) text = String("0") + String(strDtTm.year);
      else text = String(strDtTm.year);
      display_text(text,        color, 2, x + 127, y);
}

void display_drawmonth(strDateTime strDtTm, uint16_t color, int x=Date_x, int y=Date_y) {
      String text="";
      if (strDtTm.month <=9) text = String("0") + String(strDtTm.month);
      else text = String(strDtTm.month);
      display_text(text,        color, 2, x + 88, y);
}

void display_drawday(strDateTime strDtTm, uint16_t color, int x=Date_x, int y=Date_y) {
      String text="";
      if (strDtTm.day <=9) text = String("0") + String(strDtTm.day);
      else text = String(strDtTm.day);
      display_text(text,        color, 2, x + 50, y);
}

void display_drawwday(strDateTime strDtTm, uint16_t color, int x=Date_x, int y=Date_y) {
      String text="";
      text = WeekDays[strDtTm.wday];
      display_text(text,        color, 2, x,      y);
}

void display_drawsound(byte strDtTm_sound = 0, uint16_t color=MainColor, int x=7, int y=34) {
      String text="";
      text = sounds[strDtTm_sound];
      display_text(text,        color, 2, x,      y);
}

void display_drawvolume(byte volbar = config.Volume, uint16_t color=MainColor, int x=12, int y=96) {
      display_DrawRect(x, y, 104, 10, color);
      display_FillRect(x+2, y+2, volbar, 6, color);
}

void display_drawshades(byte level = 50, uint16_t color=MainColor, uint16_t bgcolor=BGColor, int x=24, int y=40) {
    display_DrawRect(x - 2, y - 2, 84, 54, color);
    display_FillRect(x, y, 80, level/2, color);
    display_FillRect(x, y + level/2, 80, 50 - level/2, bgcolor);
}

void display_drawEFX(byte EFXid = 0, uint16_t color=MainColor, int x=7, int y=34) {
      String text="";
      //text = EFXName[EFXid];
      display_text(text,        color, 2, x,      y);
}

void display_drawambient(float display_temp, float display_humi, float display_TMin, float display_TMax, uint16_t color=MainColor, int x=7, int y=40) {
      String text="";
      text = String("T: " + String(display_temp, 1) + "C");
      display_text(text,        color, 2, x,      y);
      text = String("H: " + String(display_humi, 0) + "%");
      display_text(text,        color, 2, x, y + 20);
      text = String("Min/Max T: " + String(display_TMin, 1) + "/" + String(display_TMax, 1));
      display_text(text,        color, 1, x, y + 40);

}

void display_drawprevious(uint16_t color=MainColor, int x=Time_x, int y=Time_y) {
    display_Image(previous_icon, x, y, 32, 32, color);
}

void display_drawplay(byte pps = 0, uint16_t color=MainColor, int x=Time_x, int y=Time_y) {
    switch (pps) {
        case 0:
            display_Image(play_icon, x + 38, y, 32, 32, color);
            break;
        case 1:
            display_Image(pause_icon, x + 38, y, 32, 32, color);
            break;
        case 2:
            display_Image(stop_icon, x + 38, y, 32, 32, color);
            break;
    }
}

void display_drawnext(uint16_t color=MainColor, int x=Time_x, int y=Time_y) {
    display_Image(next_icon, x + 76, y, 32, 32, color);
}

void display_drawbell(strDateTime strDtTm=DateTime, uint16_t color=MainColor, int x=(Time_x + T_NN * 2 + T_Sep + 4), int y=40) {
      display_Image(bell_icon, x, y, 16, 16, color);
      if (!strDtTm.alarm) {
          display.drawLine(x, y, x + 16, y + 16, color);
          display.drawLine(x, y + 16, x + 16, y, color);
      };
}

void display_drawalarm(strDateTime strDtTm=DateTime, uint16_t color=MainColor) {
    display_drawhour(strDtTm, color);
    display_Hcollon(color);
    display_drawmin(strDtTm, color);
    display_drawwday(strDtTm, color);
    display_drawbell(strDtTm, color);
    display_drawsound(strDtTm.sound, color);
}

void display_drawtime(strDateTime strDtTm=DateTime, uint16_t color=MainColor) {
    display_drawhour(strDtTm, color);
    display_Hcollon(color);
    display_drawmin(strDtTm, color);
    //display_drawsec(strDtTm, color);
}

void display_drawdate(strDateTime strDtTm=DateTime, uint16_t color=MainColor, int x=Date_x, int y=Date_y) {
    display_drawwday(strDtTm, color);
    display_text(String(','), color, 2, x + 35, y);
    display_drawday(strDtTm, color);
    display_text(String('/'), color, 2, x + 74, y);
    display_drawmonth(strDtTm, color);
    display_text(String('/'), color, 2, x + 113, y);
    display_drawyear(strDtTm, color);
}

void batt_icon_update() {
    //display_text((String((int)Batt_Level)+"%"),BGColor, 1, 104, 16);
    //display_text((String(voltage, 2)+"v"),BGColor, 1, 98, 26);
    float Batt_Level = getBattLevel();
    //display_text((String((int)Batt_Level)+"%"),MainColor, 1, Display_Width - 20, 16);
    //display_text((String(voltage, 2)+"v"),MainColor, 1, 98, 26);
    if (Batt_Level > 100)  {
        BattPowered = false;
        //display_Image(battery_icon_1_4, Display_Width - 20, 8, 16, 8, ECOColor);
        display_DrawRect(Display_Width - 20, 8, 16, 8, ECOColor);
        display_FillRect(Display_Width - 20 +  3, 10, 2, 6, ECOColor);
        display_FillRect(Display_Width - 20 +  7, 10, 2, 6, ECOColor);
        display_FillRect(Display_Width - 20 + 11, 10, 2, 6, ECOColor);
    }
    if (Batt_Level > 75 && Batt_Level <= 100)  {
        BattPowered = true;
        //display_Image(battery_icon_1_4, Display_Width - 20, 8, 16, 8, MainColor);
        display_DrawRect(Display_Width - 20, 8, 16, 8, MainColor);
        display_FillRect(Display_Width - 20 + 3, 10, 2, 6, MainColor);
        display_FillRect(Display_Width - 20 + 7, 10, 2, 6, MainColor);
        display_FillRect(Display_Width - 20 + 11, 10, 2, 6, MainColor);
    }
    else if (Batt_Level > 50 && Batt_Level <= 75) {
        BattPowered = true;
        //display_Image(battery_icon_1_4, Display_Width - 20, 8, 16, 8, MainColor);
        display_DrawRect(Display_Width - 20, 8, 16, 8, MainColor);
        display_FillRect(Display_Width - 20 + 3, 10, 2, 6, MainColor);
        display_FillRect(Display_Width - 20 + 7, 10, 2, 6, MainColor);
        display_FillRect(Display_Width - 20 + 11, 10, 2, 6, BGColor);
    }
    else if (Batt_Level > 25 && Batt_Level <= 50) {
        BattPowered = true;
        //display_Image(battery_icon_1_4, Display_Width - 20, 8, 16, 8, MainColor);
        display_DrawRect(Display_Width - 20, 8, 16, 8, MainColor);
        display_FillRect(Display_Width - 20 + 3, 10, 2, 6, MainColor);
        display_FillRect(Display_Width - 20 + 7, 10, 2, 6, BGColor);
        display_FillRect(Display_Width - 20 + 11, 10, 2, 6, BGColor);
    }
    else if (Batt_Level <= 25)  {
        BattPowered = true;
        //display_Image(battery_icon_1_4, Display_Width - 20, 8, 16, 8, WarnColor);
        display_DrawRect(Display_Width - 20, 8, 16, 8, WarnColor);
        display_FillRect(Display_Width - 20 + 3, 10, 2, 6, BGColor);
        display_FillRect(Display_Width - 20 + 7, 10, 2, 6, BGColor);
        display_FillRect(Display_Width - 20 + 11, 10, 2, 6, BGColor);
        player_play(S_beepdn);
    }
    //LOW_Batt_check();
}

void loop_icons() {
  // -- ALARM Icon --
    if ( config.Alarm_State != Last_Alarm_State ) {
        if (config.Alarm_State) display_Image(bell_icon, 0, 0, 16, 16, MainColor);
        else display_Image(bell_icon, 0, 0, 16, 16, BGColor);
        Last_Alarm_State = config.Alarm_State;
    };


  //  -- WiFI Icon --
    if ( WIFI_state != Last_WIFI_state ) {
        if ( WIFI_state == WL_CONNECTED ) display_Image(wifi_icon, 20, 0, 16, 16, MainColor);
        else display_Image(wifi_icon, 20, 0, 16, 16, BGColor);
        Last_WIFI_state = WIFI_state;
    };
    //display_Image(wifi_OFF_icon, 20, 0, 16, 16, ST7735_GREEN);


  //  -- Battery Icon --
    if (((millis() - ONTime_Offset)%300000) < 20) batt_icon_update();

  //  -- Touch Button --
    if (Last_TL_STATUS != TL_STATUS) {
        if (TL_STATUS == true) display_text(("Touched!"),DebugColor, 1, 40, 2);
        if (TL_STATUS == false) display_text(("Touched!"),BGColor, 1, 40, 2);
        telnet_println("Display Touched: " + String(TL_STATUS));
        Last_TL_STATUS = TL_STATUS;
        //TL_COUNT = 0;

    }
}



void display_setup() {

    display.init(115200);
    Display_ON = true;
    Serial.println("EPaper Initialized");
    if (display.epd2.hasFastPartialUpdate) Serial.println("FAST!! partial mode");
    else if (display.epd2.hasPartialUpdate) Serial.println("Slow partial mode");
    else Serial.println("NO partial mode");
 
    display.setRotation(EPaperRotate);

    //long deltatime = millis();
    display.fillScreen(BGColor);
    //display_clear();
    //deltatime = millis() - deltatime;

    /*
    display_text("Date / Time: " + DateandTime());
    display.println("F Rate: " + String(1000/deltatime));
    delay(5000);
    display.powerOff();
    */
}
