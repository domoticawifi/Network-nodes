//---------------------------------------------------------------NETWORK-NODES-----------------------------------------------

//-------------------------------------------------------PIETRO RIGNANESE & ANDREA POLENTA---------------------------------

//Librerie per gestione display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Libreria IR
#include <IRremote.h>
int RECV_IR = 8;
IRrecv irrecv(RECV_IR);   //Ricezione del segnale IR sul pin 8
decode_results results;

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
    lcd.setCursor(0,0);
    lcd.print("Comunicazione:");
    lcd.setCursor(0,1);
    Serial.println(results.value);
     //Accensione Relay nodo 2
    if(results.value == 69632)
    {
      lcd.print("Relay ON  ");
      Serial.println("Accensione Realy");
      digitalWrite(relay,HIGH);
    }

    //Accensione Attuatore nodo 2
    if(results.value == 17895424)
    {
      lcd.print("Attua. ON ");
      Serial.println("Accensione Attuatore");
      digitalWrite(attuatore, HIGH);
    }

    //Spegnimento Relay nodo 2
    if(results.value == 65536)
    {
      lcd.print("Relay OFF ");
      Serial.println("Spegnimento Realy");
      digitalWrite(relay, LOW);
    }

    //Spegnimento Attuatore nodo 2
    if(results.value == 286261248)
    {
      lcd.print("Attua. OFF");
      Serial.println("Spegnimento Attuatore");
      digitalWrite(attuatore, LOW);
    }
    irrecv.resume();
  }
  
  

}
