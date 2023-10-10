//  - - - libraries
//#include <sounds.h>


//  - - -  Constants
#define SPEAKER_PIN 25


//  - - -  Variables  - - -
byte last_Volume = 0;
byte min_ampl=9;        // Max is 8 bits - 1 Byte
byte max_ampl=245;      // Max is 8 bits - 1 Byte
bool player_stop = false;
byte play_status = 0;
/*
    0 -> IDLE
    1 -> Play finish
    2 -> Play Busy
    3 -> DAC finish
    4 -> DAC Busy
*/

//  - - -  Functions  - - -
void _noTone(uint8_t pin, uint8_t channel = 0)
{
    ledcDetachPin(pin);
    ledcWrite(channel, 0);
}

void _Tone(uint8_t pin, unsigned int frequency, unsigned long duration, uint8_t channel = 0)
{
    if (ledcRead(channel)) {
        telnet_println("Tone channel " + String(channel) + " is already in use");
        delay(10);
        return;
    }
    ledcAttachPin(pin, channel);
    ledcWriteTone(channel, frequency);
    if (duration) {
        delay(duration);
        _noTone(pin, channel);
    }    
}


void player_tone (float noteFrequency, long noteDuration, int silentDuration = 0){

    // tone(10,261,500);
    // delay(500);

  play_status = 4;
      _Tone(PBUZZER, noteFrequency, noteDuration);
      delay(silentDuration);
  play_status = 3;
}

void player_bendTones (float initFrequency, float finalFrequency, float prop, long noteDuration, int silentDuration){

  //Examples:
  //  player_bendTones (880, 2093, 1.02, 18, 1);
  //  player_bendTones (NOTE_A5, NOTE_C7, 1.02, 18, 0);
  if(initFrequency < finalFrequency)
  {
      for (int freq=initFrequency; freq<finalFrequency; freq=freq*prop) {
          player_tone(freq, noteDuration, silentDuration);
      }

  } else{

      for (int freq=initFrequency; freq>finalFrequency; freq=freq/prop) {
          player_tone(freq, noteDuration, silentDuration);
      }
  }
}

void player_play(byte SongName = S_buttonPushed){
    telnet_println("playing: " + sounds[SongName]);
    play_status = 2;
    switch(SongName) {
      
        case S_connection:              // 0
          player_tone(NOTE_E5,50,30);
          player_tone(NOTE_E6,55,25);
          player_tone(NOTE_A6,60,10);
        break;

        case S_disconnection:           // 1
          player_tone(NOTE_E5,50,30);
          player_tone(NOTE_A6,55,25);
          player_tone(NOTE_E6,50,10);
        break;

        case S_buttonPushed:            // 2
          player_bendTones (NOTE_E6, NOTE_G6, 1.03, 20, 2);
          delay(30);
          player_bendTones (NOTE_E6, NOTE_D7, 1.04, 10, 2);
        break;

        case S_mode1:                   // 3
          player_bendTones (NOTE_E6, NOTE_A6, 1.02, 30, 10);  //1318.51 to 1760
        break;

        case S_mode2:                   // 4
          player_bendTones (NOTE_G6, NOTE_D7, 1.03, 30, 10);  //1567.98 to 2349.32
        break;

        case S_mode3:                   // 5
          player_tone(NOTE_E6,50,100); //D6
          player_tone(NOTE_G6,50,80);  //E6
          player_tone(NOTE_D7,300,0);  //G6
        break;

        case S_surprise:                // 6
          player_bendTones (800, 2150, 1.02, 10, 1);
          player_bendTones (2149, 800, 1.03, 7, 1);
        break;

        case S_OhOoh:                   // 7
          player_bendTones (880, 2000, 1.04, 8, 3); //A5 = 880
          delay(200);

          for (int i=880; i<2000; i=i*1.04) {
               player_tone(NOTE_B5,5,10);
          }
        break;

        case S_OhOoh2:                  // 8
          player_bendTones (1880, 3000, 1.03, 8, 3);
          delay(200);

          for (int i=1880; i<3000; i=i*1.03) {
              player_tone(NOTE_C6,10,10);
          }
        break;

        case S_cuddly:                  // 9
          player_bendTones (700, 900, 1.03, 16, 4);
          player_bendTones (899, 650, 1.01, 18, 7);
        break;

        case S_sleeping:                // 10
          player_bendTones (100, 600, 1.04, 10, 10);
          delay(500);
          player_bendTones (500, 100, 1.04, 10, 2);
        break;

        case S_happy:                   // 11
          player_bendTones (1500, 2500, 1.05, 20, 8);
          player_bendTones (2499, 1500, 1.05, 25, 8);
        break;

        case S_superHappy:              // 12
          player_bendTones (2000, 6000, 1.05, 8, 3);
          delay(50);
          player_bendTones (5999, 2000, 1.05, 13, 2);
        break;

        case S_happy_short:             // 13
          player_bendTones (1500, 2000, 1.05, 15, 8);
          delay(100);
          player_bendTones (1900, 2500, 1.05, 10, 8);
        break;

        case S_sad:                     // 14
          player_bendTones (880, 669, 1.02, 20, 200);
        break;

        case S_confused:                // 15
          player_bendTones (1000, 1700, 1.03, 8, 2);
          player_bendTones (1699, 500, 1.04, 8, 3);
          player_bendTones (1000, 1700, 1.05, 9, 10);
        break;

        case S_fart1:                   // 16
          player_bendTones (1600, 3000, 1.02, 2, 15);
        break;

        case S_fart2:                   // 17
          player_bendTones (2000, 6000, 1.02, 2, 20);
        break;

        case S_fart3:                   // 18
          player_bendTones (1600, 4000, 1.02, 2, 20);
          player_bendTones (4000, 3000, 1.02, 2, 20);
        break;

        case S_beep:                    // 19
          player_tone(NOTE_E5,40,40);
          player_tone(NOTE_E5,60,20);
        break;

        case S_beepup:                  // 20
          player_tone(NOTE_E5,40,40);
          player_tone(NOTE_E6,60,20);
        break;

        case S_beepdn:                  // 21
          player_tone(NOTE_E6,60,40);
          player_tone(NOTE_E5,20,40);
        break;

        case bird_song:                 // 22
        player_tone(NOTE_C5,90,10);
        player_tone(NOTE_D5,90,10);
        player_tone(NOTE_C5,90,10);
        player_tone(NOTE_E5,90,10);
        player_tone(NOTE_F5,190,10);
        player_tone(NOTE_G5,90,10);
        player_tone(NOTE_F5,90,10);
        player_tone(NOTE_E5,190,10);
        player_tone(NOTE_C5,90,10);
        player_tone(NOTE_E5,90,10);
        player_tone(NOTE_F5,90,10);
        player_tone(NOTE_G5,90,10);
        player_tone(NOTE_A5,90,10);
        player_tone(NOTE_G5,90,10);
        player_tone(NOTE_F5,90,10);
        player_tone(NOTE_E5,90,10);
        player_tone(NOTE_D5,90,10);
        player_tone(NOTE_C5,300,0);
    };
    play_status = 1;
 }



void Touch_to_Stop() {
  if (TL_COUNT>=1 && TL_STATUS == false && millis() - Last_TL > Touch_Interval) {      // Was Touch Button pressed?
    //TL_COUNT = 0;
    //TL_STATUS = false;
    player_stop = true;
    TL_COUNT = 0;
    telnet_println("Touched to stop player");
  }
}

void player_setup () {
    ledcSetup(0, 1000, 14);   // channel = 0, Freq = 1KHz,  Bits = 14 
    //player_sing();
}

void player_loop() {
    Touch_to_Stop();
}
