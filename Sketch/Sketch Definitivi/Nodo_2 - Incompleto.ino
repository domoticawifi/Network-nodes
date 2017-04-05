//---------------------------------------------------------------NETWORK-NODES-----------------------------------------------

//-------------------------------------------------------PIETRO RIGNANESE & ANDREA POLENTA---------------------------------

//Libreria IR
#include <IRremote.h>
int RECV_IR = 8;
IRrecv irrecv(RECV_IR);   //Ricezione del segnale IR sul pin 8
decode_results results;

//Mappatura pin Arduino 
int relay = 2;
int attuatore = 4;

void setup() 
{
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
  
  //Controlla se è stato ricevuto un dato sul sensore IR
  if (irrecv.decode(&results)) 
  {
    Serial.println(results.value);
     //Accensione Relay nodo 2
    if(results.value == 69632)
    {
      Serial.println("Accensione Realy");
      digitalWrite(relay,HIGH);
    }

    //Accensione Attuatore nodo 2
    if(results.value == 17895424)
    {
      Serial.println("Accensione Attuatore");
      digitalWrite(attuatore, HIGH);
    }

    //Spegnimento Relay nodo 2
    if(results.value == 65536)
    {
      Serial.println("Spegnimento Realy");
      digitalWrite(relay, LOW);
    }

    //Spegnimento Attuatore nodo 2
    if(results.value == 286261248)
    {
      Serial.println("Spegnimento Attuatore");
      digitalWrite(attuatore, LOW);
    }
    irrecv.resume();
  }
  
  

}
