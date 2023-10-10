//  - - -  Variables  - - -

static const String location[] = {"Cozinha", "Escritorio", "Quarto", "SalaJantar","SalaEstar","Suite"};
byte SHADES = 80;                              // Position of SHADES
byte Last_SHADES = 80;                         // Last position of SHADES
bool Last_Shades_State = false;                // Last Shades state
bool Shades_Set = false;                       // set Shades time menu
byte Shades_Set_index = 9;                     // Shades item index to set (hour, min, wday, alarm, shades,...)

//  - - -  ALARM related FUNCTIONS  - - -
void set_shades() {
        if(A_COUNT == 1 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            A_COUNT = 0;
        }
        if(B_COUNT == 1 && !B_STATUS && (millis() - Last_B > Butt_Interval)) {
            Menu_Next = true;
            B_COUNT = 0;
            MENU_LastTime = millis();
        }
        if(C_COUNT == 1 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
            delta = 1;
            C_COUNT = 0;
        }
        if(C_COUNT == 2 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
            delta = 10;
            C_COUNT = 0;
        }
        switch (Shades_Set_index) {
            case 0:       // edit hour
                if (delta !=0) {
                    //display_drawhour(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.hour = (config.AlarmDateTime.hour + delta)%24;
                    //display_drawhour(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawhour(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Shades_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawhour(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 1:       // edit minute
                if (delta !=0) {
                    //display_drawmin(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.minute = (config.AlarmDateTime.minute + delta)%60;
                    //display_drawmin(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawmin(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Shades_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawmin(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 2:       // edit Shades Enabled
                if (delta !=0) {
                    //display_drawbell(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.alarm = !config.AlarmDateTime.alarm;
                    //display_drawbell(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawbell(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Shades_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawbell(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 3:       // edit week day
                if (delta !=0) {
                    //display_drawwday(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.wday = (config.AlarmDateTime.wday + delta)%10;
                    //display_drawwday(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawwday(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Shades_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawwday(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 4:       // edit Shades
                if (delta !=0) {
                    //display_drawshades(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.shades = (config.AlarmDateTime.shades + delta)%(sizeof(shades)/sizeof(*shades));;
                    //display_drawshades(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawshades(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Shades_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawshades(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            default :
                //player_beep();
                Menu_Next = false;
                Menu_1stRun = true;
                Shades_Set_index = 0;
        }
}

void loop_shades() {
    if(A_COUNT == 1 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            MENU = (MENU + 1)%(sizeof(menu_main)/sizeof(*menu_main));
            telnet_println("Menu: " + menu_main[MENU]);
            A_COUNT = 0;
    }
    if(A_COUNT == 2 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            MENU = 0;
            A_COUNT = 0;
    }
    if(B_COUNT == 1 && !B_STATUS && (millis() - Last_B > Butt_Interval)) {
            if (SHADES > 100) SHADES = 100;
            else SHADES = SHADES + 10;
            B_COUNT = 0;
    }
    if(C_COUNT == 1 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
            if (SHADES <= 10) SHADES = 0;
            else SHADES = SHADES - 10;
            C_COUNT = 0;
    }
    if(C_COUNT == 1 && C_STATUS && (millis() - Last_C > Butt_Interval)) {
            //player_play(S_Beep);
            //storage_write();
            C_COUNT = 0;
    }
    if(C_COUNT == 2 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
              C_COUNT = 0;
    }
    if (Last_SHADES != SHADES) {
        display_drawshades(SHADES, MainColor);
        Last_SHADES=SHADES;
    }
}
