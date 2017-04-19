//---------------------------------------------------------CALCOLATORI ELETTRONICI E RETE DI CALCOLATORI---------------------------------------------------------

//-------------------------------------------------------------PIETRO RIGNANESE & ANDREA POLENTA  2017----------------------------------------------------------- 

// Inclusione dell'apposita libreria per la Shield adoperata
#include <ESP8266WiFi.h>
// Libreria di gestione infrarossi per la shield esp8266
#include <IRremoteESP8266.h>

//Libreria per database esterno (Firebase)
#include <FirebaseArduino.h>
#define FIREBASE_HOST "*****.firebaseio.com"
#define FIREBASE_AUTH "*********"

//Librerie per gestione display
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Definizione delle credenziali per la connessione per accedere al Router
const char* ssid = "*******";
const char* password = "******";

String IP = "";

// Definiamo la mappatura della Shield ESP8266(Datasheet)
#define D0 16 // Trasmettitore IR
#define D1 5  // Display
#define D2 4  // LCD
#define D3 0  // Ricevitore IR 
#define D4 2  // Relè
#define D5 14 
#define D6 12 
#define D7 13
#define D8 15
#define D9 3  // RX
#define D10 1 // TX

//Istanziamo le variabii di Controllo Invio/Ricezione
boolean controllo_invio = true;
boolean controllo_ricezione = true;

//Istanziamo un flag per verificare lo stato della connessione
boolean stato_conn = true;

// Istanziamo 'server(80)' come oggetto della classe WiFiServer.
WiFiServer server(80);

//Istanziamento display
LiquidCrystal_I2C lcd(0x27,20,4); 

IRsend irsend(D0); //Instanziamo un oggetto per la trasmissione dell'informazione tramite IR al pin D0
IRrecv irrecv(D3); //Instanziamo oggetto per ricezione segnale infrarosso
decode_results results;

void setup() 
{
  //Display
  lcd.init();                     
  lcd.backlight();

  // Instanziamo la comunicazione infrarossi
  irsend.begin();
  irrecv.enableIRIn();
  
  // Connessione seriale a 115200.
  Serial.begin(115200);
  delay(10);

  // Inizializziamo i dispositivi connessi al nodo allo stato di LOW
  pinMode(D4, OUTPUT);
  digitalWrite(D4, LOW);

  lcd.home();
  lcd.print("Benvenuto!");
  delay(2500);
  lcd.setCursor(0, 0);
  lcd.print("Inizializzazione");
  lcd.setCursor(0,1);

  //Connessione alla rete WiFi
  // Mostriamo l'IP della scheda connessa alla rete
  Serial.print("\n\nInizializzazione rete WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.print(".");
  }
  lcd.setCursor(0,0);
  Serial.println("\nConnesso.");
  lcd.print("Connesso        ");
  Serial.print("IP del dispositivo: ");
  Serial.print(WiFi.localIP());
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  // Inizializziamo il WebServer.
  server.begin();
  Serial.println("\nWebServer inizializzato.\n");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //Inizializza firebase
}
int i = 0;

//Funzione per la comunicazione tra Nodo Master e Nodo 1 durante l'invio del dato
void comunicazione()
{  
  Serial.println("Comunicazione");
  do
  {
    if(irrecv.decode(&results))
      {
        Serial.println("Dato ricevuto:");
        Serial.println(results.value);

        //Dato ricevuto correttamente
        if(results.value == 4369)
        {
          lcd.setCursor(0,0);
          lcd.print("      ACK       ");
          lcd.setCursor(0,1);
          lcd.print("      O K       ");
          controllo_ricezione = false;
          controllo_invio = false;
          stato_conn = true;
          Serial.println("Dato valido");
          delay(10000);        
        }
        //Dato non ricevuto correttamente
        else
        {
          controllo_ricezione = false;
          stato_conn = false;
          Serial.println("Dato non valido");
        }
      }
     delay(1);
     Serial.print(i++);
     controllo_ricezione = false;

      //Attesa di 10000 milli secondi = 10 secondi, dopo di che chiude la connessione
     if(i == 10000)
     {
       Serial.println("Connessione scaduta");
       controllo_ricezione = false;
       controllo_invio = false;
       stato_conn = false;
     }
     irrecv.resume(); //Ricevi un altro valore
     
  }while(controllo_ricezione);
  Serial.println("Sono uscito... ");
  
}

  int RM = LOW;   // Relè nodo master
  int RN1 = LOW;  // Relè Nodo 1
  int AN1 = LOW;  // Attuatore Nodo 1
  int RN2 = LOW;  // Relè Nodo 2
  int AN2 = LOW;  // Attuatore Nodo 2

//Variabili di controllo per il cambiamento di stato effettuato\ Quando le variabili sono a TRUE il cambiamento è avvenuto mentre a FALSE il cambiamento potrebbe non essere stato effettuato
  boolean CRN1 = true;
  boolean CAN1 = true;
  boolean CRN2 = true;
  boolean CAN2 = true;

  WiFiClient cliente;

  String percorso = "NetworkNodes/"; //Stringa che conterrà il percorso del database firebase

void firebase_upload(String nodo, String dispositivo, String stato)
{
  percorso = "NetworkNodes/";
  // rimozione valore precedente
  percorso += nodo;
  percorso += dispositivo;
  Firebase.remove(percorso);

  // settaggio valore attuale
  Firebase.setString(percorso, stato);
  // Errore connessione
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
}

boolean js = false; //Variabile per far partire o meno la modifica della pagina di errore

void pag_html()
{
   // Pagina HTML 
  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type: text/html");
  cliente.println(""); //Separatore
  cliente.println("<!DOCTYPE html>");
  cliente.println("<html>");
  cliente.println("<head>");
  cliente.println("<style type='text/css'>");
  cliente.println("body");
  //---------------------------------------CSS----------------------------------------------
  cliente.println("                                              {");
  cliente.println("                                                  background:  #DCDCDC;");
  cliente.println("                                                  font-family:  Rockwell;");
  cliente.println("                                              }");                                         
  cliente.println("                                              h1");
  cliente.println("                                              {");
  cliente.println("                                                  color: #000000;");
  cliente.println("                                                  text-align: center;");
  cliente.println("                                              }");
  cliente.println("                                              h2");
  cliente.println("                                              {");
  cliente.println("                                                  color: #000000;");
  cliente.println("                                                  text-align: center;");
  cliente.println("                                              }");
                      
  cliente.println("                                              table");
  cliente.println("                                              {");
  cliente.println("                                                  border-collapse: collapse;");
  cliente.println("                                                  color: #000000;");
  cliente.println("                                              }");
                                                
  cliente.println("                                              .tinto");
  cliente.println("                                              {");
  cliente.println("                                                  border-collapse: collapse;");
  cliente.println("                                                  border: 1px solid #DCDCDC;");
  cliente.println("                                              }");
                                            
  cliente.println("                                              .tinto tr");
  cliente.println("                                              {");
  cliente.println("                                                  border: 1px solid #DCDCDC;");
  cliente.println("                                              }");
  cliente.println("                                              .tinto td");
  cliente.println("                                              {");
  cliente.println("                                                  font-size: small;");
  cliente.println("                                                  background: #DCDCDC;");
  cliente.println("                                                  border: 1px solid #000000;");
  cliente.println("                                              }");                                             
  cliente.println("                                              .cinto");
  cliente.println("                                              {");
  cliente.println("                                                  padding: 0px 0px 0px 0px;");
  cliente.println("                                              }");
    
  cliente.println("                                              table, td");
  cliente.println("                                              {");
  cliente.println("                                                  border: 3px solid #000000;");
  cliente.println("                                                  background:#FAFAD2;");
  cliente.println("                                              }");                                            
  cliente.println("                                              table, tr");
  cliente.println("                                              {");
  cliente.println("                                                  background: #ffffff;");
  cliente.println("                                                  border: 3px solid #000000;");
  cliente.println("                                                  text-align: center;");
  cliente.println("                                              }");
  cliente.println("      </style>");
  //-------------------------------------------------------------------------------------------
  //---------------------------------------HTML------------------------------------------------
  cliente.println("      <meta charset='utf-8'>");
  cliente.println("      <title>The Sentinel</title>");
  cliente.println("  </head>");
  cliente.println("  <body>");
  cliente.println("      <h1 id='titolo'>The Sentinel</h1>");
  cliente.println("      <h2>Controllo Nodi</h2>");
  cliente.println("      <form>");
  cliente.println("          <table align='center'>");
  cliente.println("          <thead>");
  cliente.println("         <tr>");
  cliente.println("             <td>Nodo</td>");
  cliente.println("               <td>Stato</td>");
  cliente.println("          </tr>");
  cliente.println("          </thead>");
  cliente.println("           <tr>");
  cliente.println("               <td>Stazione Master</td>");
  cliente.println("               <td class='cinto'>");
  cliente.println("                  <table align='center' class='tinto'>");
  cliente.println("                      <tr>");
  cliente.println("                          <td>Relè</td>");
  cliente.println("                          <td>Attuatore</td>");
  cliente.println("                          <td>Sensore n°1</td>");
  cliente.println("                          <td>Sensore n°2</td>");
  cliente.println("                      </tr>");
  cliente.println("                      </tr>");
  
  //Controllo stato del relè e attuatore al nodo master
  if(RM==HIGH)
  {
     cliente.println("                          <td>ON</td>");
     firebase_upload("NodoMaster/","Relay","ON");
  }
  else
  {
     cliente.println("                          <td>OFF</td>");
     firebase_upload("NodoMaster/","Relay","OFF");
  }
  cliente.println("                          <td>/</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      <tr>");
  cliente.println("                      <tr>");
  //Stato del radio button
  if(RM==HIGH)
  {
    cliente.println("                          <td><input type='radio' value='ON' name='RelayMaster' checked='checked'>ON</input><input type='radio' value='OFF' name='RelayMaster'>OFF</input></td>");
  }
  else
  {
    cliente.println("                          <td><input type='radio' value='ON' name='RelayMaster'>ON</input><input type='radio' value='OFF' name='RelayMaster' checked='checked'>OFF</input></td>");
  }
  cliente.println("                          <td><input type='radio' checked='checked'>NO DEVICE</input></td>");
  cliente.println("                          <td>/</td>");
  cliente.println("                          <td>/</td>");
  cliente.println("                      </tr>");
  cliente.println("                  </table>");
  cliente.println("               </td>");
  cliente.println("           </tr>");
  cliente.println("          <tr>");
  cliente.println("              <td>Stazione n°1</td>");
  cliente.println("              <td class='cinto'>");
  cliente.println("                  <table align='center' class='tinto'>");
  cliente.println("                      <tr>");
  if(CRN1)
  {
    cliente.println("                          <td>Relè</td>");
  }
  else
  {
    cliente.println("                          <td bgcolor='#FF0000'><font color='#FF0000'>Relè</font></td>");
  }
  if(CAN1)
  {
    cliente.println("                          <td>Attuatore</td>");
  }
  else
  {
    cliente.println("                          <td bgcolor='#FF0000'><font color='#FF0000'>Attuatore</font></td>");
  }
  cliente.println("                          <td>Sensore n°1</td>");
  cliente.println("                          <td>Sensore n°2</td>");
  cliente.println("                      </tr>");
  cliente.println("                      <tr>");

  //Controllo stato Relè e Attuatore nodo 1
  if(RN1==HIGH)
  {
    cliente.println("                          <td>ON</td>");
    firebase_upload("Nodo1/","Relay","ON");
  }
  else
  {
    cliente.println("                          <td>OFF</td>");
    firebase_upload("Nodo1/","Relay","OFF");
  }
  if(AN1 ==HIGH)
  {
    cliente.println("                          <td>ON</td>");
    firebase_upload("Nodo1/","Attuatore1","ON");
  }
  else
  {
    cliente.println("                          <td>OFF</td>");
    firebase_upload("Nodo1/","Attuatore1","OFF");
  }  
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      </tr>");
  cliente.println("                      <tr>");
  //Stato radio button Nodo 1
  if(RN1==HIGH)
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Relay1' checked='checked'>ON</input><input type='radio' value='OFF' name='Relay1'>OFF</input></td>");
  }
  else
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Relay1' >ON</input><input type='radio' value='OFF' name='Relay1' checked='checked'>OFF</input></td>");
  }
  if(AN1==HIGH)
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Attuatore1' checked='checked'>ON</input><input type='radio' value='OFF' name='Attuatore1'>OFF</input></td>");
  }
  else
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Attuatore1' >ON</input><input type='radio' value='OFF' name='Attuatore1' checked='checked'>OFF</input></td>");
  }
  cliente.println("                          <td>/</td>");
  cliente.println("                          <td>/</td>");
  cliente.println("                      </tr>");
  cliente.println("                  </table>");
  cliente.println("              </td>");
  cliente.println("          </tr>");
  cliente.println("          <tr>");
  cliente.println("              <td>Stazione n°2</td>");
  cliente.println("              <td class='cinto'>");
  cliente.println("                  <table align='center' class='tinto'>");
  cliente.println("                      <tr>");
  cliente.println("                          <td>Relè</td>");
  cliente.println("                          <td>Attuatore</td>");
  cliente.println("                          <td>Sensore n°1</td>");
  cliente.println("                          <td>Sensore n°2</td>");
  cliente.println("                      </tr>");
  
  //Controllo stato Relè e Attuatore nodo 2
  if(RN2==HIGH)
  {
    cliente.println("                          <td>ON</td>");
    firebase_upload("Nodo2/","Relay","ON");
  }
  else
  {
    cliente.println("                          <td>OFF</td>");
    firebase_upload("Nodo2/","Relay","OFF");
  }
  if(AN2 ==HIGH)
  {
    cliente.println("                          <td>ON</td>");
    firebase_upload("Nodo2/","Attuatore1","ON");
  }
  else
  {
    cliente.println("                          <td>OFF</td>");
    firebase_upload("Nodo2/","Attuatore1","OFF");
  }  
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      <tr>");
  cliente.println("                      <tr>");
  //Stato radio button Nodo 1
  if(RN2==HIGH)
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Relay2' checked='checked'>ON</input><input type='radio' value='OFF' name='Relay2'>OFF</input></td>");
  }
  else
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Relay2' >ON</input><input type='radio' value='OFF' name='Relay2' checked='checked'>OFF</input></td>");
  }
  if(AN2==HIGH)
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Attuatore2' checked='checked'>ON</input><input type='radio' value='OFF' name='Attuatore2'>OFF</input></td>");
  }
  else
  {
    cliente.println("                          <td><input type='radio' value='ON' name='Attuatore2' >ON</input><input type='radio' value='OFF' name='Attuatore2' checked='checked'>OFF</input></td>");
  }
  cliente.println("                          <td>/</td>");
  cliente.println("                          <td>/</td>");
  cliente.println("                      </tr>");
  cliente.println("                      </tr>");
  cliente.println("                  </table>");
  cliente.println("              </td>");
  cliente.println("          </tr>");
  cliente.println("       </table>");
  cliente.println("          <p align='center'>");
  cliente.println("              <input type='submit' value='Invio' align='center'/>");
  cliente.println("              <input type='reset' value='Reset' align='center'/>");
  cliente.println("          </p>");
  cliente.println("      </form>");
  String button = "   <div align='center'><a href='";
  button += IP;
  button += "'><button>Aggiorna</button></a></div>";
  cliente.println(button); 
  if(js)
  {
    //Parte la pagina di errore
    cliente.println("<h1 align='center'>Attenzione!!!! La connessione non è avvenuta nel modo corretto!</h1>");
    cliente.println("<h2 align='center'>Potrebbe essersi verificato:</h2>");
    cliente.println("<h3> 1. Ostacolo tra due nodi</h3>");
    cliente.println("<h3> 2. Componente di comunicazione difettato di un nodo</h3>");
  }
  cliente.println("   </body>");
  cliente.println("</html>");
}

boolean analogic_input = false; //Input analogico dato all'attuatore o relè dei vari nodi
  
void loop() 
{  
  i=0;  //Settaggio a zero della variabile che conta il tempo di attesa
    
  // Vediamo se esiste una connessione con un client. Se non esiste ritorniamo al loop.
  cliente = server.available();
  if (!cliente) 
  {
    if(irrecv.decode(&results))
    {
      Serial.println(results.value);
      if(results.value==4112)
      {
        RN1=HIGH;
        analogic_input = true;
      }
      if(results.value==257)
      {
        RN1=LOW;
        analogic_input = true;
      }
      if(results.value==1118481)
      {
        AN1=HIGH;
        analogic_input = true;
      }
      if(results.value==1052688)
      {
        AN1=LOW;
        analogic_input = true;
      }
      irrecv.resume();
      
    }
    return;
  }

  // Nela caso ci sia una connessione con un client, mostriamo un messaggio di avvenuta connessione
  // mantenendo aperta la connessione.
  Serial.println("Client connesso: ");
  while (!cliente.available()) 
  {
    delay(1);
  }

  String form ="";
  
  if(analogic_input)
  {
    form = "http://";
    form += IP;
  }
  else
  {
    //Realizziamo la lettura della form.
    form = cliente.readStringUntil('\r');
  }

  analogic_input = false;
  
  
  Serial.println(form);
  cliente.flush();

// Controllo della from restituita dall'aggiornamento della pagina HTML
  if(form.indexOf("RelayMaster=ON")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay ON        ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      digitalWrite(D4, HIGH); // Accensione Relè nodo master
      RM = HIGH;
    }
  if(form.indexOf("RelayMaster=OFF")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay OFF       ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      digitalWrite(D4, LOW);  // Spegnimento relè nodo master
      RM = LOW;
    }
  if(form.indexOf("Relay1=ON")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay Nodo1 ON  ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      Serial.println("NEC");
      do
      {
        //Controllo tempo di attesa
        if(i<1000)
        {
          irsend.sendNEC(0x0000000001, 32); //Invio dato al nodo 1 per l'accensione del relè nodo 1
          comunicazione();
          i++;  //Aumenta di +1 il tempo di attesa
        }
        else
        {
          controllo_invio = false;
        }
       }while(controllo_invio);
      
      Serial.println("Anch'io...");
      //Controllo stato connessione per modificare o meno la pagina HTML
      if(stato_conn)
      {
        RN1 = HIGH;
        CRN1 = true;
      }
      else
      {
        RN1 = LOW;
        js = true;
        CRN1 = false;
      }
      //Resetto le variabili di controllo per una prossima comunicazione
      stato_conn = true;
      controllo_invio=true;
      controllo_ricezione=true;
      i=0;      
    }
    
  if(form.indexOf("Relay1=OFF")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay Nodo1 OFF ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      Serial.println("NEC");
      do
      {
        if(i<1000)
        {
          irsend.sendNEC(0x0000000101, 32); // Invio dato al nodo 1 per lo spegnimento del relè nodo 1
          comunicazione();
          i++;
        }
        else
        {
          controllo_invio = false;
        }
       }while(controllo_invio);
      
      Serial.println("Anch'io...");
      //Controllo stato connessione per modificare o meno la pagina HTML
      if(stato_conn)
      {
        RN1 = LOW;
        CRN1 = true;
      }
      else
      {
        RN1 = HIGH;
        js = true;
        CRN1 = false;
      }
      //Resetto le variabili di controllo per una prossima comunicazione
      stato_conn = true;
      controllo_invio=true;
      controllo_ricezione=true;
      i=0;
    }
  if(form.indexOf("Attuatore1=ON")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Attuat. Nodo1 ON");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      Serial.println("NEC");
      do
      {
        if(i<1000)
        {
          irsend.sendNEC(0x0000000011, 32); //Invio dato al nodo 1 per l'accensione dell'attuatore nodo 1
          comunicazione();
          i++;
        }
        else
        {
          controllo_invio = false;
        }
       }while(controllo_invio);
      
      Serial.println("Anch'io...");
      //Controllo stato connessione per modificare o meno la pagina HTML
      if(stato_conn)
      {
        AN1 = HIGH;
        CAN1 = true;
      }
      else
      {
        AN1 = LOW;
        js = true;
        CAN1 = false;
      }
      //Resetto le variabili di controllo per una prossima comunicazione
      stato_conn = true;
      controllo_invio=true;
      controllo_ricezione=true;
      i=0;
    }
  if(form.indexOf("Attuatore1=OFF")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Attua. Nodo1 OFF");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      Serial.println("NEC");
      do
      {
        if(i<1000)
        {
          irsend.sendNEC(0x0000000010, 32); // Invio dato al nodo 1 per lo spegnimento dell'attuatore nodo 1
          comunicazione();
          i++;
        }
        else
        {
          controllo_invio = false;
        }
       }while(controllo_invio);
      
      Serial.println("Anch'io...");
      //Controllo stato connessione per modificare o meno la pagina HTML
      if(stato_conn)
      {
        AN1 = LOW;
        CAN1 = true;
      }
      else
      {
        AN1 = HIGH;
        js = true;
        CAN1 = false;
      }
      //Resetto le variabili di controllo per una prossima comunicazione
      stato_conn = true;
      controllo_invio=true;
      controllo_ricezione=true;
      i=0;
    }

    if(form.indexOf("Relay2=ON")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay Nodo2 ON  ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      int invio = 0;
      do
      {
        irsend.sendNEC(0x0000011000, 32);
        invio++;
      }while(invio<10);
      RN2 = HIGH;
      
    }

    if(form.indexOf("Relay2=OFF")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Relay Nodo2 OFF ");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      int invio = 0;
      do
      {
        irsend.sendNEC(0x0000010000, 32);
        invio++;
      }while(invio<10);
      RN2 = LOW;
      
    }

    if(form.indexOf("Attuatore2=ON")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Attuat. Nodo2 ON");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      int invio = 0;
      do
      {
        irsend.sendNEC(0x0001111000, 32);
        invio++;
      }while(invio<10);
      AN2 = HIGH;
      
    }

    if(form.indexOf("Attuatore2=OFF")!=-1)
    {
      lcd.setCursor(0,0);
      lcd.print("Attua. Nodo2 OFF");
      lcd.setCursor(0,1);
      lcd.print("Attendere...    ");
      delay(100);
      int invio = 0;
      do
      {
        irsend.sendNEC(0x0011100000, 32);
        invio++;
      }while(invio<10);
      AN2 = LOW;
      
    }

  pag_html(); //Pagina HTML
  js = false;

  lcd.setCursor(0,0);
  lcd.print("IP device:      ");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());

  //
  delay(1);
  Serial.println("Client Disconnesso.\n");
}
