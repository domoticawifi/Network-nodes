# Network-nodes

Questo progetto vede la realizzazione, tramite Arduino, di una rete a nodi (domestica o aziendale).
Lo scopo principale è quello di far comunicare il terminale (PC) con i nodi della rete, passando o trasmettendo informazioni.

## Struttura
Abbiamo presvisto un terminale pc, un nodo master e due nodi, schematizzati a questa maniera:
  * Terminale (Invia e riceve le informazioni di tutta la rete)
  * Router (Instrada le richieste fatte tra terminale e nodo master sulla rete domestica e anche quella esterna andando ad aggiornare un database)
  * Nodo Master (Nodo formato da un modulo [ESP8266 ESP-12E](https://acrobotic.com/acr-00018) per il collegamento WI-FI con il terminale e l'invio delle informazioni sul database)
  * Nodo1 (Nodo formato da un arduino [Arduino NANO](https://www.arduino.cc/en/Main/arduinoBoardNano) che gestisce un attuatore/relè/led e un sensore
  * Nodo2 (Stessa cosa del nodo precedente)
  
  N.B: Lo storage delle informazioni su un database, ci serve per cotrollare lo stato della rete, anche da smartphone o tablet, in remoto. 
  
## Comunicazioni  
Le comunicazioni fra i nodi e il terminale avviene a questa maniera:
  * Il terminale comunica con il solo Nodo Master tramite la rete WI-FI, e riceve ed invia a questo nodo le richieste da fare agli altri due nodi della rete.
  * Il nodo master comunica con il terminale, via WI-FI, e, con il nodo1, attraverso un collegamento ottico (LED IR e Sensor IR), e la stessa comunicazione avviene tra il nodo1 e nodo2
  * Ogni nodo può comunicare solamente con il suo nodo adiacente formando un ponte di nodi
  * Se il terminale deve contattare il nodo2, la sua richiesta passerà prima a tutti i nodi
  
## Schema Network
<img src="https://i.imgbox.com/dVumVRec.png"/>

Come possiamo vedere dall'immagine, abbiamo un PC, che funziona da terminale, collegato alla rete WI-FI, invia e riceve informazioni attraverso il Nodo Master.
Il nodo master, collegato alla rete, instrada le richieste e le informazioni, mandate dal terminale e dagli altri nodi, nella giusta direzione.

## Occorrente Hardware
  * 1 terminale (PC, Tablet, Smartphone)
  * 1 Router WI-FI
  * 1 ESP8266 ESP-12E oppure una qualsiasi shield WI-FI
  * 2 Arduino NANO/UNO/MEGA/Mino PRO
  * 4 TX Diodo LED infrarossi
  * 4 RX Sensori IR
  * 3 Breadboard (830 fori)
  * Cavetti e jumper per i collegamenti
  * 3 Display LCD per verificare lo stato di ogni nodo
  
## Occorrente Software
  * [Arduino IDE 1.8.0](https://www.arduino.cc/en/main/software) o superiori
  * [Firebase](https://firebase.google.com/)
  * [Core ESP8266](https://github.com/domoticawifi/Network-nodes/blob/master/GestioneShieldESP8266.md)
  * [Librearia gestione ESP8266](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
  * [Libreria gestione segnale IR](https://www.pjrc.com/teensy/arduino_libraries/IRremote.zip)
  * [Libreria gestione Display LCD](https://www.dropbox.com/s/62x4w48kwf5biko/LiquidCrystalI2C.zip?dl=0)
  
Per la gestione delle librerie clicca [QUI](https://github.com/domoticawifi/Network-nodes/blob/master/GestioneLibraryArduino.md)


  
  
  
  
  

