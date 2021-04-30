#include "DoorControls.h"

/*----------------------------- Globals ----------------------------

CC1101 radio;

unsigned long lastSend = 0;
unsigned int sendDelay = 10000;

unsigned long lastStatusDump = 0;
unsigned int statusDelay = 5000;

String rec_payload;

float RT, VR, ln, TXX, VRT;

int   counter = 0;
char  output[2] = {0}; //SAM was size 64
char * return_data;


*/
int gdo0 = 4;
byte transmitt_byte[1] = {72};
char *transmitt_char = "Hello World";
int relayPin = 26;

void setupDoorControl(){
  // enable radio module.
      Serial.println("Initialzing Transceiver... ");
      if (ELECHOUSE_cc1101.getCC1101()){         // Check the CC1101 Spi connection.
        Serial.println("Connection OK");
      }else{
        Serial.println("Connection Error");
      }
    
      ELECHOUSE_cc1101.Init();               // must be set to initialize the cc1101!
      ELECHOUSE_cc1101.setGDO(gdo0,0);      // set lib internal gdo pins (gdo0,gdo2). Gdo2 not use for this example.
      ELECHOUSE_cc1101.setCCMode(1);       // set config for internal transmission mode.
      ELECHOUSE_cc1101.setModulation(0);  // set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.
      ELECHOUSE_cc1101.setMHZ(315);   // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
      ELECHOUSE_cc1101.setSyncMode(2);  // Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync, carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold.
   // ELECHOUSE_cc1101.setPA(10);      // set TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max!
      ELECHOUSE_cc1101.setCrc(1);     // 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX.
      Serial.println("Tx Mode");




    //if(fixedCode > 255){ //relay startup
      Serial.println("Rolling code device, initializing relay controls... ");
      Serial.println("Do you have your door button wired to the SGDO?");
      pinMode(relayPin, OUTPUT);
      digitalWrite(relayPin, LOW);
   // } else {
      Serial.println("Fixed code detected, starting CC1101...");
      //radio.enableSerialDebug();

      // Start RADIO
     // while ( !radio.begin(CFREQ_315, KBPS_250, /* channel num */ 16, /* address */ THIS_DEVICE_ADDRESS, GDO0_INTERRUPT_PIN /* Interrupt */) ){ //channel 16! whitening enabled
     //   yield();
     // }

     // radio.printCConfigCheck();
      Serial.println(F("CC1101 radio initialized."));
      delay(1000);

      //rec_payload.reserve(100); //does anyone know what this does?
    //}

}

void changeDoorState(int fixedCode){
  //called on user request to open/close door

  if(fixedCode < 256){ //fixed code present, use wireless
   // memset(output, 0x00, sizeof(output));
   // radio.sendChars("----------------------------------", DESINATION_DEVICE_ADDRESS);
    transmitt_byte[0] = (byte)fixedCode;
    ELECHOUSE_cc1101.SendData(transmitt_byte, 1);
    delay(2000);




   
  }
  else{ //no fixed code, use relay
    digitalWrite(relayPin, HIGH);
    delay(5000);
    digitalWrite(relayPin, LOW);
  }
}

void pairDevice(){
     /*WIP, only example code in here rn*/
    /*unsigned long now = millis();

    if ( radio.dataAvailable() )
    {


        // Data format from CC1110:
        //  V|33|D|000907|000393|000138|002047|002047|002047|001421|0
        //  V|33|D|000902|000393|000138|002047|002047|002047|001423|0

       rec_payload  = String(radio.getChars()); // pointer to memory location of start of string
       //Serial.print("Payload size recieved: "); Serial.println(radio.getSize());
       Serial.println(rec_payload);

       byte *payload = radio.getBytes();

       float battery           =  rec_payload.substring(2,4).toInt();
       Serial.print("battery: " ); Serial.println(battery);

       int movement           =  rec_payload.substring(8,14).toInt();
       //Serial.print("movement: " ); Serial.println(movement);

       int thermopile_surface_ir = rec_payload.substring(15,21).toInt();
       //Serial.print("thermopile_surface_ir: "); Serial.println(thermopile_surface_ir);

       int thermistor_ambient_temp = rec_payload.substring(22).toInt();
       //Serial.print("thermistor_ambient_temp: "); Serial.println(thermistor_ambient_temp);

       VRT  = thermistor_ambient_temp;     // Acquisition analog value of VRT
       VRT  = (3.30 / 1023.00) * VRT;      // Conversion to voltage
       VR   = VCC - VRT;
       RT   = VRT / (VR / R);              // Resistance of RT

       ln = log(RT / RT0);
       TXX = (1 / ((ln / B) + (1 / T0o))); //Temperature from thermistor

       TXX = TXX - 273.15;                 //Conversion to Celsius

       Serial.print("Temperature:");
       Serial.print("\t");
       Serial.print(TXX);
       Serial.print("C\t\t");
       Serial.print(TXX + 273.15);        //Conversion to Kelvin
       Serial.print("K\t\t");
       Serial.print((TXX * 1.8) + 32);    //Conversion to Fahrenheit
       Serial.println("F");

       // Infrared camer / Thermopile attempt
       float ir_temp = thermopile_surface_ir*(battery/33); // normalise to 3.3 volts due to the ir value going up as battery deplets
       ir_temp /= (float)4.95; // guestimate

       Serial.print("IR temperature:");
       Serial.print("\t");
       Serial.println(ir_temp);
   }*/

}
