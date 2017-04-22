//---------------------------------------------------------------NETWORK-NODES-----------------------------------------------

//-------------------------------------------------------PIETRO RIGNANESE & ANDREA POLENTA---------------------------------

/*
 * NODO 2
 * 
 * Comunicazione ottica
 * 
 * Link Progetto: https://github.com/domoticawifi/Network-nodes
 * 
 * 
 * DA IMPORTARE IN ARDUINO UNO/NANO
 * 
 * Strumenti > Scheda: > Arduino UNO/NANO
 * Strumenti > Porta > COM(3,5,7)
 * 
 * DATA ULTIMA MODIFICA: SABATO 22 APRILE 2017 ORE 15:50
 * 
 * 
 */

//Librerie per gestione display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Libreria IR
#include <IRremote.h>
int RECV_IR = 8;
IRrecv irrecv(RECV_IR);   //Ricezione del segnale IR sul pin 8
decode_results results;

IRsend irsend;

//Mappatura pin Arduino 
int relay = 2;
int attuatore = 4;

LiquidCrystal_I2C lcd(0x27,20,4);

void setup() 
{
  //Display
  lcd.init();                     
  lcd.backlight();
  lcd.home();
  lcd.print("Nodo 2");
  
  Serial.begin(115200);
  Serial.println("Nodo 2");
  Serial.println("");

  pinMode(relay, OUTPUT);
  pinMode(attuatore, OUTPUT);

  digitalWrite(relay,HIGH);
  digitalWrite(attuatore,HIGH);
  delay(2000);
  digitalWrite(relay,LOW);
  digitalWrite(attuatore,LOW);

  irrecv.enableIRIn();
}

void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("Nodo 2          ");
  lcd.print("                ");
  
  //Controlla se è stato ricevuto un dato sul sensore IR
  if (irrecv.decode(&results)) 
  {
    int cont = 0;
    lcd.setCursor(0,0);
    lcd.print("Comunicazione:");
    lcd.setCursor(0,1);
    Serial.println(results.value);
    
     //Accensione Relay nodo 2
    if(results.value == 69632)
    {
      Serial.println("Accensione Realy");
      digitalWrite(relay,HIGH);
      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato = 4369
        Serial.println("Invio OK");
        delay(20);
      }while(cont<80);
    }

    //Accensione Attuatore nodo 2
    if(results.value == 17895424)
    {
      Serial.println("Accensione Attuatore");
      digitalWrite(attuatore, HIGH);
      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato = 4369
        Serial.println("Invio OK");
        delay(20);
      }while(cont<80);
    }

    //Spegnimento Relay nodo 2
    if(results.value == 65536)
    {
      Serial.println("Spegnimento Realy");
      digitalWrite(relay, LOW);
      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato = 4369
        delay(20);
      }while(cont<80);
    }

    //Spegnimento Attuatore nodo 2
    if(results.value == 286261248)
    {
      Serial.println("Spegnimento Attuatore");
      digitalWrite(attuatore, LOW);
      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato = 4369
        delay(20);
      }while(cont<80);
    }
    irrecv.enableIRIn();
    irrecv.decode(&results);
    irrecv.resume();
  }
 
}
