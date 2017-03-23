// Inclusione dell'apposita libreria per la Shield adoperata
#include <ESP8266WiFi.h>

//Definizione delle credenziali per la connessione per accedere al Router
const char* ssid = "HUAWEI P8";
const char* password = "pierino123";

// Definiamo la mappatura della Shield ESP8266(Datasheet)
#define D0 16
#define D1 5 // Emettitore a infrarossi
#define D2 4 // Ricevitore a infrarossi
#define D3 0
#define D4 2 
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3 // RX0
#define D10 1 // TX0

// Istanziamo 'server(80)' come oggetto della classe WiFiServer.
WiFiServer server(80);

void setup() {

  // Connessione seriale a 115200.
  Serial.begin(115200);
  delay(10);

  // Inicializamos los GPIO D1, D2, D3, D4 como 'output' y estado 'low'.
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);

  //Connessione alla rete WiFi
  // Mostriamo l'IP della scheda connessa alla rete
  Serial.print("\n\nInizializzazione rete WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnesso.");
  Serial.print("IP del dispositivo: ");
  Serial.print(WiFi.localIP());

  // Inizializziamo il WebServer.
  server.begin();
  Serial.println("\nWebServer inizializzato.\n");
}

void loop() {

  // Vediamo se esiste una connessione con un client. Se non esiste ritorniamo al loop.
  WiFiClient cliente = server.available();
  if (!cliente) 
  {
    return;
  }

  // Nela caso ci sia una connessione con un client, mostriamo un messaggio di avvenuta connessione
  // mantenendo aperta la connessione.
  Serial.println("Client connesso: ");
  while (!cliente.available()) 
  {
    delay(1);
  }

  //Realizziamo la lettura della form.
  String form = cliente.readStringUntil('\r');
  Serial.println(form);
  cliente.flush();

  int RM = LOW; //Variabile per controllo stato Relè
  int AM = LOW; //Variabile per controllo stato Attuatore

  //Controlliamo il risultato della form e vediamo se è presente un cambiamento di stato dell'attuatore e relè
  if(form.indexOf("RelayMaster=ON")!=-1)
  {
    digitalWrite(D1, HIGH);
    RM = HIGH;
  }
  if(form.indexOf("RelayMaster=OFF")!=-1)
  {
    digitalWrite(D1, LOW);
    RM = LOW;
  }
  if(form.indexOf("AttuatoreMaster=ON")!=-1)
  {
    digitalWrite(D2, HIGH);
    AM = HIGH;
  }
  if(form.indexOf("AttuatoreMaster=OFF")!=-1)
  {
    digitalWrite(D2, LOW);
    AM = LOW;
  }

  // Pagina WEB da mandare al Client 
  cliente.println("HTTP/1.1 200 OK");
  cliente.println("Content-Type: text/html");
  cliente.println(""); //  Separación
  cliente.println("<!DOCTYPE html>");
  cliente.println("<html>");
  cliente.println("<head>");
  cliente.println("<style type='text/css'>");
  cliente.println("body");
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
  if(RM==HIGH)
  {
     cliente.println("                          <td>ON</td>");
  }
  else
  {
     cliente.println("                          <td>OFF</td>");
  }
  if(AM==HIGH)
  {
     cliente.println("                          <td>ON</td>");
  }
  else
  {
     cliente.println("                          <td>OFF</td>");
  }
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      <tr>");
  cliente.println("                      <tr>");
  cliente.println("                          <td><input type='radio' value='ON' name='RelayMaster'>ON</input><input type='radio' value='OFF' name='RelayMaster' checked='checked'>OFF</input></td>");
  cliente.println("                          <td><input type='radio' value='ON' name='AttuatoreMaster'>ON</input><input type='radio' value='OFF' name='AttuatoreMaster' checked='checked'>OFF</input></td>");
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
  cliente.println("                          <td>Relè</td>");
  cliente.println("                          <td>Attuatore</td>");
  cliente.println("                          <td>Sensore n°1</td>");
  cliente.println("                          <td>Sensore n°2</td>");
  cliente.println("                      </tr>");
  cliente.println("                      <tr>");
  cliente.println("                          <td>OFF</td>");
  cliente.println("                          <td>OFF</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      </tr>");
  cliente.println("                      <tr>");
  cliente.println("                          <td><input type='radio' value='ON' name='Relay1'>ON</input><input type='radio' value='OFF' name='Relay1' checked='checked'>OFF</input></td>");
  cliente.println("                          <td><input type='radio' value='ON' name='Attuatore1'>ON</input><input type='radio' value='OFF' name='Attuatore1' checked='checked'>OFF</input></td>");
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
  cliente.println("                          <td>OFF</td>");
  cliente.println("                          <td>OFF</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                          <td>0.00</td>");
  cliente.println("                      <tr>");
  cliente.println("                      <tr>");
  cliente.println("                          <td><input type='radio' value='ON' name='Relay2'>ON</input><input type='radio' value='OFF' name='Relay2' checked='checked'>OFF</input></td>");
  cliente.println("                          <td><input type='radio' value='ON' name='Attuatore2'>ON</input><input type='radio' value='OFF' name='Attuatore2' checked='checked'>OFF</input></td>");
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
  cliente.println("   </body>");
  cliente.println("</html>");

  // Client disconnesso
  delay(1);
  Serial.println("Cliente Desconectado.\n");
}
