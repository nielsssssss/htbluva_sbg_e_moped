#include <Arduino_CAN.h>



/* 
##################

Globale Variablen

##################
*/


uint16_t rpm = 0; //0-6000
uint16_t speed = 65; //0-120
uint16_t bus_voltage = 0;//in mV
uint16_t bus_amps = 0; //in mA
uint16_t phase_amps = 0; //in mA
uint16_t power = 0; // in W
uint16_t power_relative = 0; // in %
uint16_t gear = 1; // 0=N 1=D 2=P 3=R
uint16_t drive_mode = 1; //0=eco 1=comfort 2=sport insane=3
uint16_t recop = 0; //0=no 1=light 2=middle 3=high
uint16_t motor_temp = 26; //0-120 C
uint16_t battery_temp = 20; //0-100 C
uint16_t scroll_selection = 0;// 0= tempomat 1=horn loudness 2=speaker loudness
uint16_t SOC = 100; //Battery in %
uint16_t tempomat_speed = 52; //Speed Tempomat if active
uint16_t tempomat = 0; //0 inactive 1 active


/*
########

CAN-ID's

########
*/
static uint32_t const CAN_ID1 = 0x10; //tx_pi_can snippet 1
static uint32_t const CAN_ID2 = 0x11; //tx_pi_can snippet 2
static uint32_t const CAN_ID3 = 0x12; //tx_pi_can snippet 3


/*
########

Arduino Pins

########
*/

//Bedienpult links
#define BUTTON1 2
#define BUTTON2 12
#define BUTTON3 6
#define BUTTON4 8

#define BUTTON5 7

/*
#######

states

#######
*/
bool state_button1 =0;
bool state_button2 =0;
bool state_button3 =0;
bool state_button4 =0;
bool state_button5 =0;

bool s1=0;
bool s2=0;
bool s3=0;
bool s4=0;
bool s5=0;




/*
#######

Handle Buttons

#######
*/
bool buttonState1 = false;       // Aktueller Zustand des Buttons
bool lastButtonState1 = false;   // Letzter Zustand des Buttons
unsigned long lastDebounceTime1 = 0; // Zeitstempel für Debounce
bool lbutton1 = 0;

bool buttonState2 = false;       // Aktueller Zustand des Buttons
bool lastButtonState2 = false;   // Letzter Zustand des Buttons
unsigned long lastDebounceTime2 = 0; // Zeitstempel für Debounce
bool lbutton2 = 0;

bool buttonState3 = false;       // Aktueller Zustand des Buttons
bool lastButtonState3 = false;   // Letzter Zustand des Buttons
unsigned long lastDebounceTime3 = 0; // Zeitstempel für Debounce
bool lbutton3 = 0;

bool buttonState4 = false;       // Aktueller Zustand des Buttons
bool lastButtonState4 = false;   // Letzter Zustand des Buttons
unsigned long lastDebounceTime4 = 0; // Zeitstempel für Debounce
bool lbutton4 = 0;

bool buttonState5 = false;       // Aktueller Zustand des Buttons
bool lastButtonState5 = false;   // Letzter Zustand des Buttons
unsigned long lastDebounceTime5 = 0; // Zeitstempel für Debounce
bool lbutton5 = 0;

const unsigned long debounceDelay = 50; // Verzögerung für Debounce in Millisekunden



void setup() {
  pinMode(BUTTON1, INPUT_PULLUP);
  pinMode(BUTTON2, INPUT_PULLUP);
  pinMode(BUTTON3, INPUT_PULLUP);
  pinMode(BUTTON4, INPUT_PULLUP);
  pinMode(BUTTON5, INPUT_PULLUP);
  //Serial.begin(115200);
  //while (!Serial) { }

  if (!CAN.begin(CanBitRate::BR_250k))
  {
    //Serial.println("CAN.begin(...) failed.");
    for (;;) {}
  }

}

void read_inputs(bool debug){
    if(debug){
    //Serial.print("Button ");
    //Serial.println(state_button5);
  }
  state_button1 = !digitalRead(BUTTON1);
  state_button2 = !digitalRead(BUTTON2);
  state_button3 = !digitalRead(BUTTON3);
  state_button4 = !digitalRead(BUTTON4);
  state_button5 = !digitalRead(BUTTON5);


}

void calc(bool debug) {
  if (debug) {
    //Serial.print("Switchstate vor handleButton: ");
    //Serial.print(s1);
    //Serial.print(s2);
    //Serial.print(s3);
    //Serial.println(s4);
  }
  //Serial.print(rpm);
  //Serial.print(" ");
  //Serial.println(speed);
  handleButton(state_button1, s1,buttonState1,lastButtonState1,lastDebounceTime1);
  handleButton(state_button2, s2,buttonState2,lastButtonState2,lastDebounceTime2);
  handleButton(state_button3, s3,buttonState3,lastButtonState3,lastDebounceTime3);
  handleButton(state_button4, s4,buttonState4,lastButtonState4,lastDebounceTime4);
  handleButton(state_button5, s5,buttonState5,lastButtonState5,lastDebounceTime5);

  count_inc_buttons(s1,gear,lbutton1,debug);
  count_inc_buttons(s2,drive_mode,lbutton2,debug);
  count_inc_buttons(s3,recop,lbutton3,debug);
  count_inc_buttons(s4,scroll_selection,lbutton4,debug);
  count_inc_buttons(s5,tempomat,lbutton5,debug);


  limit_var();
}


void count_inc_buttons(bool sbutton, uint16_t &var,bool &lbutton,bool debug) {
  if (sbutton != lbutton) { // Nur wenn ein tatsächlicher Zustandwechsel stattfindet
    var++;
    if(debug){
      //Serial.print("Counter: ");
      //Serial.println(var);
    }

  }
  lbutton = sbutton; // Aktualisiere den letzten Zustand
}

void limit_var(){
  if(gear>3) gear=0;
  if(drive_mode>3) drive_mode=0;
  if(recop>2) recop=0;
  if(scroll_selection>2) scroll_selection=0;
  if(tempomat>1) tempomat=0;
}

void handleButton(int reading, bool &switchState,bool &buttonState,bool &lastButtonState,unsigned long &lastDebounceTime) {
  // Überprüfen, ob der Buttonzustand sich verändert hat
  if (reading != lastButtonState) {
    // Debounce-Timer zurücksetzen
    lastDebounceTime = millis();
  }

  // Wenn der Zustand stabil bleibt und die Verzögerung überschritten ist
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Prüfen, ob der Buttonzustand sich verändert hat
    if (reading != buttonState) {
      buttonState = reading;

      // Schalterzustand nur umschalten, wenn der Button gedrückt wurde
      if (buttonState == HIGH) {
        switchState = !switchState;
      }
    }
  }

  // Aktuellen Buttonzustand speichern
  lastButtonState = reading;
}





void tx_pi_can(){
  // Nachrichtendaten für die erste Nachricht (4 x 16-Bit-Werte)
  uint8_t msg_data1[8];
  msg_data1[0] = (phase_amps >> 8) & 0xFF; // High byte Wert 1
  msg_data1[1] = phase_amps & 0xFF;        // Low byte Wert 1
  msg_data1[2] = (power_relative >> 8) & 0xFF; // High byte Wert 2
  msg_data1[3] = power_relative & 0xFF;        // Low byte Wert 2
  msg_data1[4] = (SOC >> 8) & 0xFF; // High byte Wert 3
  msg_data1[5] = SOC & 0xFF;        // Low byte Wert 3
  msg_data1[6] = (gear >> 8) & 0xFF; // High byte Wert 4
  msg_data1[7] = gear & 0xFF;        // Low byte Wert 4

  CanMsg const msg1(CanStandardId(CAN_ID1), sizeof(msg_data1), msg_data1);

  if (int const rc = CAN.write(msg1); rc < 0)
  {
    //Serial.print("CAN.write(...) failed with error code ");
    //Serial.println(rc);
    for (;;) { }
  }
  delay(2);

  // Nachrichtendaten für die zweite Nachricht (4 x 16-Bit-Werte)
  uint8_t msg_data2[8];
  msg_data2[0] = (drive_mode >> 8) & 0xFF; // High byte Wert 5
  msg_data2[1] = drive_mode & 0xFF;        // Low byte Wert 5
  msg_data2[2] = (battery_temp >> 8) & 0xFF; // High byte Wert 6
  msg_data2[3] = battery_temp & 0xFF;        // Low byte Wert 6
  msg_data2[4] = (motor_temp >> 8) & 0xFF; // High byte Wert 7
  msg_data2[5] = motor_temp & 0xFF;        // Low byte Wert 7
  msg_data2[6] = (rpm >> 8) & 0xFF; // High byte Wert 8
  msg_data2[7] = rpm & 0xFF;        // Low byte Wert 8
  //Serial.println(speed);

  CanMsg const msg2(CanStandardId(CAN_ID2), sizeof(msg_data2), msg_data2);

  if (int const rc = CAN.write(msg2); rc < 0)
  {
    //Serial.print("CAN.write(...) failed with error code ");
    //Serial.println(rc);
    for (;;) { }
  }
  delay(2);

    // Nachrichtendaten für die zweite Nachricht (4 x 16-Bit-Werte)
  uint8_t msg_data3[8];
  msg_data3[0] = (scroll_selection >> 8) & 0xFF; // High byte Wert 5
  msg_data3[1] = scroll_selection & 0xFF;        // Low byte Wert 5
  msg_data3[2] = (tempomat_speed >> 8) & 0xFF; // High byte Wert 6
  msg_data3[3] = tempomat_speed & 0xFF;        // Low byte Wert 6
  msg_data3[4] = (tempomat >> 8) & 0xFF; // High byte Wert 7
  msg_data3[5] = tempomat & 0xFF;        // Low byte Wert 7
  msg_data3[6] = (recop >> 8) & 0xFF; // High byte Wert 8
  msg_data3[7] = recop & 0xFF;        // Low byte Wert 8

  CanMsg const msg3(CanStandardId(CAN_ID3), sizeof(msg_data3), msg_data3);

  if (int const rc = CAN.write(msg3); rc < 0)
  {
    //Serial.print("CAN.write(...) failed with error code ");
    //Serial.println(rc);
    for (;;) { }
  }
  delay(2);
}



void read_motor(bool debug){
  if (CAN.available()) {
    CanMsg msg = CAN.read();

    // Process only messages with ID 0x980117EF (or the correct ID for bus voltage)
    if (msg.id == 0x980117EF) {
      // Process bytes 1 & 2
      uint16_t combinedValue1 = (msg.data[1] << 8) | msg.data[0];
      bus_voltage = combinedValue1; // Resolution is 0.1 V/bit
      //Serial.print("Voltage:");
      //Serial.println(bus_voltage);
  

      // Process bytes 3 & 4
      uint16_t combinedValue2 = (msg.data[3] << 8) | msg.data[2];
      bus_amps = (combinedValue2); // Resolution is 0.1 V/bit
      //Serial.print("Bus Current:");
      //Serial.println(bus_amps);

      // Process bytes 5 & 6
      uint16_t combinedValue3 = (msg.data[5] << 8) | msg.data[4];
      phase_amps = (combinedValue3); // Resolution is 0.1 V/bit
      //Serial.print("Phase current: ");
      //Serial.println(phase_amps);

      // Process bytes 7 & 8
      uint16_t combinedValue4 = (msg.data[7] << 8) | msg.data[6];
      rpm = combinedValue4; // Resolution is 0.1 V/bit
      //Serial.print("Speed:");
      //Serial.println(rpm);
    }
  }

}



void loop() {
  read_motor(0);
  read_inputs(0);
  calc(0);



  tx_pi_can();


}
