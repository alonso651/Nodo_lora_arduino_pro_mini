/*Nodo LoRa ABP Arduino Pro Mini 3v3 Atmega328p 8MHz

  Banda de frecuencia US915 MHz
  Sub Banda 1 (903.9 - 905.3) ---> Configuración del Gateway Single Channel ESP
  Canal 0 (de la sub banda 1) ---> Configuración del Gateway Single Channel ESP
  Canal en el que envia 903.9 MHz (canal 8 de 72)

  Radio HopeRF RFM95w (SX1276) 915MHz 

  Libreria: LMiC v1.5 *Modificada*
    Modificación: lmic.cpp LMIC_disableChannel(u1_t channel) 
      //LMIC.channelMap[channel/4] &= ~(1<<(channel&0xF)); 
      LMIC.channelMap[channel/16] &= ~(1<<(channel&0xF));

  Conexión de pines (Arduino Pro Mini):
   _____________________________
  |   RFM95w   |Arduino Pro Mini|
  |____________|________________|
  |GND         |GND             |
  |3.3v        |VCC (3.3v)      |
  |MISO        |12              |
  |MOSI        |11              |
  |SCK         |13              |
  |NSS         |10              |
  |DIO0        |4               |
  |DIO1        |5               |
  |DIO2        |7               |
  |____________|________________|

  Conexión de pines (TTGO Pro Mini_LORA - T-DEER):

    Puente entre los pines D5 y IO1 (Necesario para que envie mas de un paquete)
*/


//#define WAIT_SECS 120   //60 Segundos en Arduino Pro Mini
//#define WAIT_SECS 60  //60 Segundos en TTGO Pro Mini_LORA - T-DEER
#define WAIT_SECS 15  //15 Segundos en TTGO Pro Mini_LORA - T-DEER


#if defined(__AVR__)
#include <avr/pgmspace.h>
#include <Arduino.h>
#elif defined(ARDUINO_ARCH_ESP8266)
#include <ESP.h>
#elif defined(__MKL26Z64__)
#include <Arduino.h>
#else
#error Unknown architecture in aes.cpp
#endif

#include "lmic.h"
#include "hal/hal.h"
#include <SPI.h>

//---------------------------------------------------------
// Declaracion de variables
//---------------------------------------------------------

// Contador de tramas
int count=0;

// LoRaWAN Application identifier (AppEUI)
// No usado
static const u1_t APPEUI[8] PROGMEM = { 0x02, 0x00, 0x00, 0x00, 0x00, 0xEE, 0xFF, 0xC0 };

// LoRaWAN DevEUI
// No usado
static const u1_t DEVEUI[8] PROGMEM  = { 0x42, 0x42, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };

// LoRaWAN NwkSKey, Network Session Key 
// Network Session Key The Things Network
unsigned char NwkSkey[16] =     { 0xE9, 0x01, 0xE2, 0x79, 0x10, 0x18, 0xF1, 0x40, 0xB7, 0xF7, 0xBA, 0x6D, 0x5C, 0x04, 0x01, 0xB7 };

// LoRaWAN AppSKey, Application Session Key
// Application Session Key para desencriptación de mensajes en The Things Network
unsigned char AppSkey[16] =   { 0x0E, 0xBE, 0x00, 0xE7, 0x66, 0x10, 0xE5, 0x3A, 0x19, 0x0B, 0xC6, 0x8C, 0x71, 0x1C, 0xAA, 0x22 };


#define msbf4_read(p)   (u4_t)((u4_t)(p)[0]<<24 | (u4_t)(p)[1]<<16 | (p)[2]<<8 | (p)[3])

// LoRaWAN end-device address (DevAddr)
unsigned char DevAddr[4] = { 0x26, 0x06, 0x2C, 0xE1 };


// ----------------------------------------------------------------------------
// APPLICATION CALLBACKS
// ----------------------------------------------------------------------------

// Desabilitar todos los canales, excepto el canal definido en el if de ForceTxSingleChannelDr()
// Setear el SF en LMIC_setDrTxpow() al final de esta función.
// Esto solo afecta a los mensajes enviados (uplinks). 

void forceTxSingleChannelDr() {
    for(int j=0; j<71; j++) { // US i<71 canales
        if(j != 8) {
            LMIC_disableChannel(j);
        }
    }
    // Set data rate (SF) and transmit power for uplink
    LMIC_setDrTxpow(DR_SF10, 14);
}

// provide application router ID (8 bytes, LSBF)
void os_getArtEui (u1_t* buf) {
    memcpy(buf, APPEUI, 8);
}

// provide device ID (8 bytes, LSBF)
void os_getDevEui (u1_t* buf) {
    memcpy(buf, DEVEUI, 8);
}

// provide device key (16 bytes)
void os_getDevKey (u1_t* buf) {
    memcpy(buf, NwkSkey, 16);
}

int debug=1;
uint8_t mydata[64];
static osjob_t sendjob;

// ----------------------------------------------------------------------------
// Pin mapping (Arduino Pro Mini)
// ----------------------------------------------------------------------------

// lmic_pinmap pins = {
//   .nss = 10,          // Conectado a D10 (Arduino Pro Mini)
//   .rxtx = 0,          // No conectar al RFM92/RFM95
//   .rst = 0,           // No conectar al RFM92/RFM95
//   .dio = {4, 5, 7},   // Especificación de pines DIO0, DIO1, DIO2 (RFM95)
//                       // Conectado a D4, D5, D7 (Arduino Pro Mini)
// };

// ----------------------------------------------------------------------------
// Pin mapping (TTGO Pro Mini_LORA 20180926 - T-DEER) 
// ----------------------------------------------------------------------------

lmic_pinmap pins = {
  .nss = 10,          // Conectado a D10 (TTGO Pro Mini_LORA)
  .rxtx = 0,          // No conectar al RFM92/RFM95
  .rst = 9,           // Conectado a D9 (TTGO Pro Mini_LORA)
  .dio = {2, 5, 0},   // Especificación de pines DIO0, DIO1, DIO2 (RFM95)
                      // Conectado a D2, D5, No Conectar (TTGO Pro Mini_LORA)
};

// ----------------------------------------------------------------------------
// Declaración de funciones
// ----------------------------------------------------------------------------

void onEvent (ev_t ev) {

    switch(ev) {
      // scheduled data sent (optionally data received)
      // note: this includes the receive window!
      case EV_TXCOMPLETE:
          // use this event to keep track of actual transmissions
          Serial.print("EV_TXCOMPLETE, tiempo: ");
          //Serial.print((millis() / 1000)/2); //Pro Mini
          Serial.print((millis() / 1000));
          Serial.println(" Segundos");
          if(LMIC.dataLen) { // Datos recibidos en la ventana de recepción (RX slot) luego del envio de datos (TX)
              Serial.println("Data Received");
          }
          break;
       default:
          break;
    }
}

void do_send(osjob_t* j){
    delay(1);                         // delay agregado para la comunicación serial
      Serial.print("Tiempo: ");
      //Serial.print((millis() / 1000)/2); //Pro Mini
      Serial.print((millis() / 1000));
      Serial.println(" Segundos");
      // Muestra el canal TX 
      Serial.print("Enviado. Canal: ");
      Serial.println(LMIC.txChnl);
      Serial.print("Opmode check: ");
      // Check if there is not a current TX/RX job running
    if (LMIC.opmode & (1 << 7)) {
      Serial.println("OP_TXRXPEND, not sending");
    } 
  else {
  
    strcpy((char *) mydata,"Arduino Pro Mini"); //Mensaje que se envia a TTN
    LMIC_setTxData2(1, mydata, strlen((char *)mydata), 0);
    }
    // Schedule a timed job to run at the given timestamp (absolute system time)
    os_setTimedCallback(j, os_getTime()+sec2osticks(WAIT_SECS), do_send);
         
}

// ----------------------------------------------------------------------------
// SETUP
// ----------------------------------------------------------------------------
void setup() {
  Serial.begin(115200);

  // Inicializacion de LMiC
  os_init();
  // Definición de la banda de frecuencia a utilizar (No se si cumple alguna función esto aquí)
  #if defined(CFG_us915)
  LMIC.freq = 903900000;
  #endif
  // Resetea el estado de la MAC. Sesión o datos pendientes de envio serán descartados
  LMIC_reset();
  // Establece la sesión con los parametros suministrados en la sección "Declaración de Variables"
  LMIC_setSession (0x1, msbf4_read(DevAddr), (uint8_t*)NwkSkey, (uint8_t*)AppSkey);
  // Desabilita la adaptación de SF (configuración para nodos móviles)
  LMIC_setAdrMode(0);
  // Disabilita link check validation
  LMIC_setLinkCheckMode(0);
  // Desabilita beacon tracking
  LMIC_disableTracking ();
  // Desabilita la recepcion periodica de datos (configuración para dispositivos LoRaWAN Clase A)
  LMIC_stopPingable();
  // Establece el canal, SF y potencia del transmisor para el envio de datos
  forceTxSingleChannelDr(); //reemplazar valores en la definición de la función forceTxSingleChannelDr() en la sección "Declaración de funciones"
  
#if defined(__AVR__)
  Serial.println("AVR arch");
#elif defined(ARDUINO_ARCH_ESP8266)
  Serial.println("ESP arch");
#elif defined(__MKL26Z64__)
  Serial.println("Teensy arch");
#else
  Serial.println("WARNING. Unknown Arch");
#endif

}

// ----------------------------------------------------------------------------
// LOOP
// ----------------------------------------------------------------------------
void loop() {

  do_send(&sendjob);
  while(1) {
    os_runloop_once(); 
    delay(100);
  }
}
