//  - - -  Display  - - -
#define TFTRotate 1
#define EPaperRotate 1

//  - - -  NeoPixels  - - -
#define NEOPixelsPIN  18 //21                   // pin on the ESP32 to connected to the NeoPixels
#define NEOPixelsNUM  8                         // Number of NeoPixels LEDs

//bool Alarm_State = false;                     // Defined on config structure Alarm state (true -> Ring / False -> Not Ring)
bool Last_Alarm_State = !config.Alarm_State;    // Last Alarm state


// **** Project code definition here ...
#include <buttons.h>
#include <touch.h>
//#include <SDReader.h>
#include <ambient.h>
#include <sounds.h>                             // lib used by the epapaer
#include <images.h>
//#include <color.h>

#ifdef TFT
    #include <dacplayer.h>
    #include <tft.h>
#endif
#ifdef EPAPER
    #include <buzzplayer.h>
    #include <epaper.h>
#endif

#include <menu.h>



// **** Project code functions here ...
void project_hw() {
 // Output GPIOs
    


 // Input GPIOs
    //if (ESPWakeUpReason() != "Deep-Sleep Wake" ) wifi_setup();
}

void project_sleep() {

    if (config.DEEPSLEEP && millis() > ONTime_Offset + (ulong(config.ONTime) + Extend_time)*1000) {
        uint64_t calculate_sleeptime, calculated_utctime;
        // Store counter to the Preferences
        keep_IP_address();
        unsigned long currUTime = curUTCTime();
        calculate_sleeptime = uint64_t( 60000UL - ((currUTime * 1000UL)%60000UL) - 5000);   // 5 seconds before time
        Serial.printf("calculate_sleeptime :%llu\t", calculate_sleeptime);
        calculated_utctime = currUTime + calculate_sleeptime*1000; // The expected UTC time at the moment the ESP will wake-up 
        Serial.printf("calculated_utctime :%llu\n", calculated_utctime);
        // Store calculated UTC time in Preferences
        //preferences.putULong("UTCTime", calculated_utctime);
        //preferences.end();

    
        if (calculate_sleeptime > 1000) {       // Sleeping if only for more than a sec.
            esp_sleep_enable_timer_wakeup(calculate_sleeptime*1000);  // converting sleeptim from milisec to microseconds
            esp_deep_sleep_start();
        }
    }

}

void project_setup() {
  // Start Push Buttons call precedures
    buttons_setup();
    touch_setup();

  // Start SD Card reader
    //SDReader_setup();

  // Start Ambient devices
    //ambient_setup();
    //ambient_get_data();

  // Start TFT device
    //display_setup();

  // Start Player device
    player_setup();

  // Color Managament Service
    //color_setup();

  // Start MENU
    menu_setup();
    //display_drawtime();
    //display_powerOff();

}

void project_loop() {
    //project_sleep();
  // Ambient handing
    //if (TIMER >0) if ((millis() - 3500) % (TIMER * 60000) < 5) ambient_data();      // TIMER bigger than zero on div or dog bites!!

  // Player handling
    player_loop();

  // Color handling
    //color_loop();

  // MENU handling
    touch_loop();
    menu_loop();
    if (NTP_Sync) {
        if (WIFI_state != WL_RADIO_OFF && millis() - NTP_LastTime > (NTP_Retry * 1000)) {
            telnet_println("Disabling MQTT & WiFi ...");
            display_powerOff();
            mqtt_disconnect();
            telnet_stop();
            wifi_disconnect();
            project_sleep();
        };
    }
    else {
        if(WIFI_state == WL_RADIO_OFF) {
            telnet_println("Enabling WiFi & MQTT ...");
            wifi_setup();
            telnet_setup();
            ntp_setup();
            mqtt_connect();
          // Start TFT device
            display_setup();
            loop_icons();
            display_drawtime();
            display_drawdate();
            batt_icon_update();
        };
    }
}
