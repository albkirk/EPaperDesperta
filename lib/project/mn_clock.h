//  - - -  Variables  - - -


//  - - -  CLOCK related FUNCTIONS  - - -
void display_updatetime(uint16_t color=MainColor, uint16_t bgcolor=BGColor) {
  //DateandTime();
  if (LastDateTime.hour != DateTime.hour) {
      display_drawhour(LastDateTime, bgcolor);
      display_drawhour(DateTime, color);
  }
  if (LastDateTime.minute != DateTime.minute) {
      display_drawmin(LastDateTime, bgcolor);
      display_drawmin(DateTime, color);
  }
  /*
  if (LastDateTime.second != DateTime.second) {
      if (DateTime.second%2 == 0 ) display_Hcollon(color);
      else display_Hcollon(bgcolor);;
      display_drawsec(LastDateTime, bgcolor);
      display_drawsec(DateTime, color);    
  }
  */
  //LastDateTime = DateTime;
}

void display_updatedate(uint16_t color=MainColor, uint16_t bgcolor=BGColor) {
      //DateandTime();  //Skipped as display_drawclock() function should run much more often than this one.
    if (LastDateTime.wday != DateTime.wday) {
        display_drawwday(LastDateTime, bgcolor);
        display_drawwday(DateTime, color);
    }
    if (LastDateTime.day != DateTime.day) {
        display_drawday(LastDateTime, bgcolor);
        display_drawday(DateTime, color);
    }
    if (LastDateTime.month != DateTime.month) {
        display_drawmonth(LastDateTime, bgcolor);
        display_drawmonth(DateTime, color);
    }
    if (LastDateTime.year != DateTime.year) {
        display_drawyear(LastDateTime, bgcolor);
        display_drawyear(DateTime, color);
    }
    //LastDateTime = DateTime;
}


void display_updateclock() {
        DateandTime();
        display_updatetime();
        display_updatedate();
        LastDateTime = DateTime;
}


void loop_clock() {
    if(A_COUNT == 1 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
        MENU = (MENU + 1)%(sizeof(menu_main)/sizeof(*menu_main));
        telnet_println("Menu: " + menu_main[MENU]);
        A_COUNT = 0;
    }
    if ( ( curUnixTime() % Display_Refresh ) < 10 ) {       // 1000 for TFT display, 60000 for ePaper
        display_updateclock();
        if (config.Alarm_State) alarm_ring();
    };
}
