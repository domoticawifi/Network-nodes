# Network-nodes

Questo progetto vede la realizzazione, tramite Arduino, di una rete a nodi (domestica o aziendale).
Lo scopo principale è quello di far comunicare il terminale (PC) con i nodi della rete, passando o trasmettendo informazioni.

## Struttura
Abbiamo presvisto un terminale pc, un nodo master e due nodi, schematizzati a questa maniera:
  1. Terminale (Invia e riceve le informazioni di tutta la rete)
  2. Router (Instrada le richieste fatte al nodo master e del terminale sulla rete internet andando ad aggiornare un database)
  3. Nodo Master (Nodo formato da un modulo [ESP8266 ESP-12E](https://acrobotic.com/acr-00018) per il collegamento WI-FI con il terminale e l'invio delle informazioni sul database)
  4. Nodo1 (Nodo formato da un arduino [Arduino NANO](https://www.arduino.cc/en/Main/arduinoBoardNano) che gestisce un attuatore/relè/led e un sensore
  5. Nodo2 (Stessa cosa del nodo precedente)
  
## Comunicazioni  
Le comunicazioni fra i nodi e il terminale avviene a questa maniera:
  1. Il terminale comunica con il solo Nodo Master con la rete WI-FI, e riceve ed invia a questo nodo le richieste da fare agli altri due nodi della rete.
  2. Il nodo master comunica con il solo nodo1 attraverso un collegamento ottico (LED IR e Sensor IR) e la stessa comunicazione avviene tra il nodo1 e nodo2
  3. Ogni nodo può comunicare solamente con il suo nodo adiacente formando un ponte di nodi
  4. Se il terminale deve contattare il nodo2, la sua richiesta passerà prima a tutti i nodi
  
## Schema Network
<img src="https://i.imgbox.com/dVumVRec.png"/>

Come possiamo vedere dall'immagine, abbiamo un PC, che funziona da terminale, collegato alla rete WI-FI, invia e riceve informazioni attraverso il Nodo Master.
Il nodo master, collegato alla rete, instrada le richieste e le informazioni, mandate dal terminale e dagli altri nodi, nella giusta direzione.
  
  
  

