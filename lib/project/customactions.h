// Function to insert customized MQTT Configuration actions
void custom_mqtt(String command, String cmd_value) {
  if ( command == "Info" && bool(cmd_value.toInt()) ) {
    //status_report();
    if (BattPowered) mqtt_publish(mqtt_pathtele, "Battery", String(getBattLevel(),0));
    mqtt_publish(mqtt_pathtele, "DateTime", String(DateandTime()));
    mqtt_publish(mqtt_pathtele, "NTP_Sync", String(NTP_Sync));
    if (BattPowered) { telnet_print("Power: BATT", true); Serial.printf("  -  Level: %.0f", getBattLevel()); }
    else { telnet_print("Power: MAINS", true); }
    telnet_print("  -  Uptime: " + String(millis()/1000), true);
    telnet_print("  -  Current Date/Time: " + DateandTime(), true);
    telnet_println("  -  NTP Sync: " + String(NTP_Sync), true);
    #ifdef ESP8266
        telnet_println("Flash: " + Flash_Size() + "  -  CPU Clock: " + String(CPU_Clock()) + " MHz  -  WiFi State: " + WIFI_state_Name[WIFI_state] + " - Phy Mode: " + WIFI_PHY_Mode_Name[WiFi.getPhyMode()] + "  -  MQTT State: " + MQTT_state_string(), true);
    #else
        telnet_println("Flash: " + Flash_Size() + "  -  CPU Clock: " + String(CPU_Clock()) + " MHz  -  WiFi State: " + WIFI_state_Name[WIFI_state] + "  -  MQTT State: " + MQTT_state_string(), true);
    #endif
    #ifdef Modem_WEB_TELNET
        telnet_println("Modem State: " + Modem_state_Name[Modem_state] + "\t REG State: " + RegStatus_string(modem.getRegistrationStatus()));
    #endif
    hassio_attributes();
    //ambient_data();
  }
  if ( command == "Config" && bool(cmd_value.toInt()) ) {
    mqtt_publish(mqtt_pathtele, "OTA", String(config.OTA));
    mqtt_publish(mqtt_pathtele, "TELNET", String(config.TELNET));
    mqtt_publish(mqtt_pathtele, "WEB", String(config.WEB));
    mqtt_publish(mqtt_pathtele, "Interval", String(config.SLEEPTime));   
    mqtt_publish(mqtt_pathtele, "Temp_Corr", String(config.Temp_Corr));
    mqtt_publish(mqtt_pathtele, "LDO_Corr", String(config.LDO_Corr));
    config_backup();
    storage_print();
    telnet_print("Alamr = {" + String(config.AlarmDateTime.hour) + ", " + String(config.AlarmDateTime.minute) + ", " + String(config.AlarmDateTime.second) + ", ");
    telnet_println(String(config.AlarmDateTime.wday) + ", " + String(config.AlarmDateTime.alarm) + ", " + String(config.AlarmDateTime.sound) + "}");
  }
  
  if ( command == "CPU_Boost" ) { CPU_Boost(bool(cmd_value.toInt())); delay(10); telnet_println("CPU Clock: " + String(CPU_Clock()) + " MHz"); }
  if ( command == "CPU_Clock" && bool(cmd_value.toInt()) ) telnet_println("CPU Clock: " + String(CPU_Clock()) + " MHz");
  #ifdef ESP8266
      if ( command == "PHY_Mode" ) {WiFi.setPhyMode((WiFiPhyMode_t)cmd_value.toInt()); wifi_connect(); }
      // WIFI_PHY_MODE_11B = 1, WIFI_PHY_MODE_11G = 2, WIFI_PHY_MODE_11N = 3
  #endif
  //if ( command == "send_Telemetry" && bool(cmd_value.toInt())) { gps_update(); print_gps_data(); send_Telemetry(); }

  if ( command == "BattPowered" ) BattPowered = bool(cmd_value.toInt());
  
  if ( command == "BckpRstr" ) {
      if (cmd_value == "") telnet_println("Restore data is empty");
      else {
          DeserializationError error = deserializeJson(config_doc, cmd_value); //Deserialize string to JSON data
          if (error) {telnet_print("JSON DeSerialization failure: "); telnet_println(error.f_str());}
          else {

                strcpy(config.Location, config_doc["Location"]);
/*
                config.LOWER_LEVEL =    config_doc["LOWER_Pos"];
                config.UPPER_LEVEL =    config_doc["UPPER_Pos"];
*/
                storage_write();
                hassio_attributes();
                bckup_rstr_flag = false;
          }
      }
  }

  if ( command == "White" && bool(cmd_value.toInt()) ) {display_clear(); display_powerOff();}
  if ( command == "Black" && bool(cmd_value.toInt()) ) {display_clear(MainColor); display_powerOff();}
  if ( command == "Play" ) player_play(byte(cmd_value.toInt()));
  if ( command == "Alarm" ) {
    // Alarm = {17_00_00_00_00_00_08_1_22}
    // byte hour;    byte minute;    byte second;    int year;    byte month;    byte day;    byte wday;    bool alarm;    byte sound;
        config.AlarmDateTime.hour  = byte(cmd_value.substring(1,3).toInt());
        config.AlarmDateTime.minute = byte(cmd_value.substring(4,6).toInt());
        //config.AlarmDateTime.second
        //config.AlarmDateTime.year
        //config.AlarmDateTime.month
        //config.AlarmDateTime.day
        config.AlarmDateTime.wday = byte(cmd_value.substring(19,21).toInt());
        config.AlarmDateTime.alarm = bool(cmd_value.substring(22,23).toInt());
        config.AlarmDateTime.sound = byte(cmd_value.substring(24,26).toInt());

        storage_write();
  };

/*  To BE updated
  if ( reqparam == "Level") {SHADES = byte(long(data["value"]));}
  if ( reqparam == "Color") {
      strcpy(validData, data["value"]);
      if (validData[0] == '#') strcpy(Color, data["value"]);
  };
  if ( reqparam == "InitColor") {
      strcpy(config.InitColor, data["value"]);
      strcpy(Color, data["value"]);
      storage_write();
  };
  if ( reqparam == "Switch") {
      if (SWITCH == true) strcpy(Color, config.InitColor);
      else strcpy(Color, "#00000000");              // Color BLACK but, can't use the name as color is declared after mqtt
  }
  if ( reqparam == "Tempe_MIN") { Tempe_MIN = float(data["value"]); }
  if ( reqparam == "Tempe_MAX") { Tempe_MAX = float(data["value"]); }
*/

  
}

void custom_update(){
    yield();
    //mqtt_dump_data(mqtt_pathtele, "Telemetry");
}



// TELNET related actions


void parse_at_command(String msg) {
    #ifdef Modem_WEB_TELNET
        msg.remove(0, 2);       // to remove "AT"
        modem.sendAT(msg);
        String res = "";
        unsigned long timeout = millis();
        while ( (millis() - timeout < 1000) && modem.stream.available() < 0 ) yield();
        timeout = millis();
        while ((millis() - timeout < 1000)) {
            if (modem.stream.available() > 0) {
                res = modem.stream.readStringUntil('\n');
                telnet_println(res);
                timeout = millis();
            }
        }
    #endif
    yield();
}
