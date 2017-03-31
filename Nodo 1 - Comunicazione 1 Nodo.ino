//----------------------CALCOLATORI ELETTRONICI E RETE DI CALCOLATORI-------------------------------------------//

//---------------PIETRO RIGNANESE & ANDREA POLENTA   2017------------------------------------------------------//

//Librerie per gestione display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Libreria per comunicazione IR
#include <IRremote.h>
int RECV_IR = 8;
IRrecv irrecv(RECV_IR);   //Ricezione del segnale IR sul pin 8
decode_results results;
IRsend irsend;

LiquidCrystal_I2C lcd(0x27,20,4);

int cont = 0; //Contatore per il conteggio dei secondi una volta ricevuto il dato dal Nodo Master
int num_ricezioni = 1; //Conteggio delle ricezioni

//Mappatura pin Arduino
int relay = 2;
int attuatore = 4;

boolean dato_corrotto = true;  //Flag per il controllo del dato corrotto

void setup() 
{ 
  //Display
  lcd.init();                     
  lcd.backlight();
  lcd.home();
  lcd.print("Benvenuto!");

  Serial.println("Benvenuto");

  //Inizializzazione pin arduino
  pinMode(relay,OUTPUT);
  pinMode(attuatore, OUTPUT);
  
  Serial.begin(115200);
  
  irrecv.enableIRIn();        // Comincia la trasmissione del segnale  
}

void loop() 
{
  
  //Controlla se è stato ricevuto un dato sul sensore IR
  if (irrecv.decode(&results)) 
  {
    dato_corrotto = true;
    Serial.println("");
    Serial.println("");
    Serial.println("Fase ");
    Serial.print(num_ricezioni);
    Serial.println("");
    Serial.println("Dato in arrivo...");
    Serial.println("");

    num_ricezioni++;
    
    lcd.setCursor(0,0);
    lcd.print("Comunicazione:");
    lcd.setCursor(0,1);
    
    Serial.println(results.value); //Stampa su terminale il valore del segnale ricevuto

    //-----------------Controllo del dato---------------------

    //Accensione Relay
    if(results.value == 1)
    {
      Serial.println("Accensione Relay");
      
      int cont = 0;
      lcd.print("R.ON ");
      digitalWrite(relay,HIGH);
      delay(20);
      do
      {
        cont++;
        irsend.sendNEC(0x0000001111, 32);// invio del dato per la conferma della purezza del dato = 4369
        delay(20);
      }while(cont<80);
      
      //Reinstanziamento della ricezione
      irrecv.enableIRIn();
      irrecv.decode(&results);

      dato_corrotto = false;  //Pongo dato corrotto a false in modo da non richiedere l'invio del dato al Nodo Master
    }

    //Spegnimento Relay
    if(results.value == 257)
    {
      Serial.println("Segnimento Relay");
      
      lcd.print("R.OFF ");
      digitalWrite(relay,LOW);
      cont = 0;

      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato
        delay(20);
      }while(cont<80);
      
      //Reinstanziamento della ricezione
      irrecv.enableIRIn();
      irrecv.decode(&results);

      

      dato_corrotto = false;  //Pongo dato corrotto a false in modo da non richiedere l'invio del dato al Nodo Master
    }

    //Accensione Attuatore
    if(results.value == 17)
    {
      Serial.println("Accensione Attuatore");
      
      lcd.setCursor(5,1);
      lcd.print("A.ON ");
      digitalWrite(attuatore,HIGH);
      cont = 0;

      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato
        delay(20);
      }while(cont<80);
      
      //Reinstanziamento della ricezione
      irrecv.enableIRIn();
      irrecv.decode(&results);

      dato_corrotto = false;  //Pongo dato corrotto a false in modo da non richiedere l'invio del dato al Nodo Master
    }

    //Spegnimento Attuatore
    if(results.value == 16)
    {
      Serial.println("Spegnimento Attuatore");
      
      lcd.setCursor(5,1);
      lcd.print("A.OFF");
      digitalWrite(attuatore,LOW);
      cont = 0;

      do
      {
        cont++;
        irsend.sendNEC(0x1000001111, 32);// invio del dato per la conferma della purezza del dato
        delay(20);
      }while(cont<80);
      
      //Reinstanziamento della ricezione
      irrecv.enableIRIn();
      irrecv.decode(&results);

      dato_corrotto = false;  //Pongo dato corrotto a false in modo da non richiedere l'invio del dato al Nodo Master
    }

    //Richiedo l'invio del dato al Nodo Master, perchè il dato è corrotto!
    if(dato_corrotto)
    {
      irsend.sendNEC(0x0000000111, 32);
      Serial.println("Messaggio corrotto!"); 

      //Reinstanzio la ricezione
      irrecv.enableIRIn();
      irrecv.decode(&results);
    }
 
    irrecv.resume(); // Ricezione del prossimo valore
 }
  

}
