// Inclusione dell'apposita libreria per la Shield adoperata
#include <ESP8266WiFi.h>

//Definizione delle credenziali per la connessione per accedere al Router
const char* ssid = "****";
const char* password = "****";

// Istanziamo 'server(80)' come oggetto della classe WiFiServer.
WiFiServer server(80);

void setup() {

  // Connessione seriale a 115200.
  Serial.begin(115200);
  delay(10);

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

  Serial.println("\n \n Client Connesso!");
}
