//  - - -  Variables  - - -
//bool AMB_State = false;                   // Defined on config structure ZEN state (true -> Ring / False -> Not Ring)
float Last_Temperature = 0.0;               // Variable
float Last_Humidity = 0.0;                  // Variable
float Last_Tempe_MAX = 0.0;                 // Variable
float Last_Tempe_MIN = 0.0;                 // Variable
bool Last_AMB_State = false;                // Last ZEN state
bool AMB_Set = false;                       // set ZEN time menu
byte AMB_Set_index = 9;                     // ZEN item index to set (hour, min, wday, alarm, sound,...)

//  - - -  ALARM related FUNCTIONS  - - -
void set_ambient() {
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
        switch (AMB_Set_index) {
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
                    AMB_Set_index ++;
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
                    AMB_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawmin(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 2:       // edit ZEN Enabled
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
                    AMB_Set_index ++;
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
                    AMB_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawwday(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            case 4:       // edit Light
                if (delta !=0) {
                    //display_drawZEN(config.AlarmDateTime, BGColor);
                    //config.AlarmDateTime.sound = (config.AlarmDateTime.sound + delta)%(sizeof(ZEN)/sizeof(*ZEN));;
                    //display_drawZEN(config.AlarmDateTime, EditColor);
                    delta = 0;
                    break;
                }
                if (Menu_Next) {
                    //display_drawZEN(config.AlarmDateTime, SetColor);
                    Menu_Next = false;
                    Menu_1stRun = true;
                    AMB_Set_index ++;
                    break;
                }
                if (Menu_1stRun) {
                    //display_drawZEN(config.AlarmDateTime, EditColor);
                    Menu_1stRun = false;
                }
                break;
            default :
                player_play(S_beep);
                Menu_Next = false;
                Menu_1stRun = true;
                AMB_Set_index = 0;
        }
}


void refresh_ambient() {
        display_drawambient(Last_Temperature, Last_Humidity, Last_Tempe_MIN, Last_Tempe_MAX, BGColor);
        if (Temperature < Tempe_MIN) {
            Tempe_MIN = Temperature;
            telnet_println("Temperatura MIN: " + String(Tempe_MIN) + " C");
            mqtt_publish(mqtt_pathtele, "Tempe_MIN", String(Tempe_MIN));
        }
        if (Temperature > Tempe_MAX) {
            Tempe_MAX = Temperature;
            telnet_println("Temperatura MAX: " + String(Tempe_MAX) + " C");
            mqtt_publish(mqtt_pathtele, "Tempe_MAX", String(Tempe_MAX));
        }
        display_drawambient(Temperature, Humidity, Tempe_MIN, Tempe_MAX, MainColor);
        Last_Temperature = Temperature;                 // Variable
        Last_Humidity = Humidity;                       // Variable
        Last_Tempe_MIN = Tempe_MIN;
        Last_Tempe_MAX = Tempe_MAX;
}

void loop_ambient() {
    if(B_COUNT == 1 && B_STATUS && (millis() - Last_B > Butt_Interval)) {
        ambient_data();
        B_COUNT = 0;
    }
    if (AMB_Set) {
        //set_ZEN();
    }
    else{
        if(A_COUNT == 1 && !A_STATUS && (millis() - Last_A > Butt_Interval)) {
            MENU = (MENU + 1)%(sizeof(menu_main)/sizeof(*menu_main));
            telnet_println("Menu: " + menu_main[MENU]);
            A_COUNT = 0;
        }
        if(C_COUNT == 1 && !C_STATUS && (millis() - Last_C > Butt_Interval)) {
            Tempe_MAX = 0.0;                      // Variable
            Tempe_MIN = 100.0;                    // Variable
            refresh_ambient();
            C_COUNT = 0;
        }
    }
    if (Last_Temperature != Temperature || Last_Humidity != Humidity) refresh_ambient();
}
