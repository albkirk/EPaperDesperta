//  - - -  Variables  - - -
//bool Light_State = false;                   // Defined on config structure Light state (true -> Ring / False -> Not Ring)
bool Last_Light_State = false;                // Last Light state
bool Light_Set = false;                       // set Light time menu
byte Light_Set_index = 9;                     // Light item index to set (hour, min, wday, alarm, sound,...)

//  - - -  ALARM related FUNCTIONS  - - -
void set_light() {
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
        switch (Light_Set_index) {
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
                    Light_Set_index ++;
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
                    Light_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawmin(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 2:       // edit Light Enabled
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
                    Light_Set_index ++;
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
                    Light_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawwday(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 4:       // edit Light
                if (delta !=0) {
                    //display_drawlight(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.sound = (config.AlarmDateTime.sound + delta)%(sizeof(lights)/sizeof(*lights));;
                    //display_drawlight(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawlight(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    Light_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawlight(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            default :
                player_beep();
                Menu_Next = false;
                Menu_1stRun = true;
                Light_Set_index = 0;
        }
}

void loop_lights() {
    if(B_COUNT == 1 && B_STATUS && (millis() - Last_B > Butt_Interval)) {
        if (!Light_Set) {
            Light_Set = true;
            //display_drawalarm(config.AlarmDateTime, SetColor);
            Menu_Next = false;
            Menu_1stRun = true;
            Light_Set_index = 0;
        }
        else {
            Light_Set = false;
            player_beep(1);
            //storage_write();
            //display_drawalarm(config.AlarmDateTime, MainColor);
        }
        B_COUNT = 0;
    }
    if (Light_Set) {
        //set_light();
    }
    else{
        if(A_COUNT == 1 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            MENU = (MENU + 1)%(sizeof(menu_main)/sizeof(*menu_main));
            telnet_println("Menu: " + menu_main[MENU]);
            A_COUNT = 0;
        }
        if(A_COUNT == 2 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            MENU = 0;
            A_COUNT = 0;
        }

        if(C_COUNT == 1 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
            display_drawEFX(EFX, BGColor);
            EFX = (EFX + 1) % (sizeof(EFXName)/sizeof(*EFXName));
            display_drawEFX(EFX, MainColor);
            C_COUNT = 0;
        }
    }
}
