# Network-nodes

Questo progetto vede la realizzazione, tramite Arduino, di una rete locale (domestica o aziendale).
Lo scopo principale è quello di far comunicare il terminale (PC) con i nodi della rete trasmettendo informazioni.

## Struttura
Abbiamo presvisto un terminale PC, un nodo master e due nodi, schematizzati a questa maniera (classifichiamo questa rete come P2P):
  * Terminale (Invia e riceve le informazioni di tutta la rete)
  * Router (Instrada le richieste fatte tra terminale e nodo master sulla rete domestica e anche quella esterna andando ad aggiornare un DB)
  * Nodo Master (Nodo formato da un modulo [ESP8266 ESP-12E](https://acrobotic.com/acr-00018) per il collegamento WI-FI con il terminale, l'invio delle informazioni sul DB e la trasmissione di informazioni tra i vari nodi presenti nella rete locale.
  * Nodo1 (Nodo formato da [Arduino NANO](https://www.arduino.cc/en/Main/arduinoBoardNano) che gestisce un attuatore/relè/led e un sensore
  * Nodo2 (Stessa cosa del nodo precedente)
  
  N.B: Lo storage delle informazioni su un DB, ci serve per cotrollare lo stato della rete, anche da smartphone o tablet, in         remoto (Internetworking: il router fungerà da gateway).
  
## Comunicazioni  
Le comunicazioni fra i nodi e il terminale avviene in questa maniera:
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

# Esempio di comunicazione: Connected Oriented
## Terminale > Nodo Master > Nodo Interessato
* Il PC avrà una pagina HTML, in locale, in cui sarà presente lo stato di ogni nodo
* Se vorrà azionare un attuatore o relè di un determinato nodo, cliccerà su l'apposito bottone o link della pagina HTML
* Verrà mandata una richiesta tramite rete WI-FI al nodo master, in cui sarà presente l'ESP8266
* La scheda Wi-Fi riceverà la richiesta e manderà un segnale ad infrarossi al nodo adiacente
* Il nodo adiacente riceve questo segnale e lo elabora per capire se è lui il destinatario
* Se così non fosse lo manda all'altro nodo adiacente con il collegamento ottico
* Quando il messaggio è arrivato a destinazione, l'arduino presente su tale nodo spegnerà/accenderà l'attuatore o relè
* Verrà mandato, in ritorno, l'avvenuta/o accensione/spegnimento
* Il messaggio di ritorno arriverà fino al nodo master che darà in risposta al PC un'altra pagina HTML aggiornata
* Il nodo master, successivamente, manderà questo dato sul database per poterlo consultare in remoto tramite smartphone
* Il pc riceverà la pagina HTML aggiornata e vedrà lo stato attuale di tutti i nodi per una prossima operazione
  
## Nodo Master > Nodo Interessato
* Il nodo master avrà un proprio indirizzo IP nella rete (es: 192.168.1.100)
* Il nodo master riceve la richiesta dal PC da instradare al giusto nodo
* Il nodo master comunicherà con il nodo1, essendo il suo adiacente
* Verrà mandato un segnale ottico al nodo1 per informarlo che sta per avvenire una trasmissione
* Il nodo1 accetta questa trasmissione
* Il nodo master invia il dato seguito dalla chiusura della comunicazione
* Il nodo1 riceve questo dato e la chiusura, in modo da capire che tutto è andato nel modo giusto, se così non fosse fa ristrasmettere il dato
* Il nodo1 elabora questo dato e capisce se appartiene a lui altrimenti lo manda, alla stessa maniera della prima comunicazione, al nodo adiacente (nodo2)
* Elaborato il dato ed eseguita la modifica su tale nodo, il nodo1 manda un messaggio ottico al nodo master per informarli che sta per trasmettere
* Il nodo master accetta e il nodo1 comincia a trasmettere il dato seguito dalla chiusura
* Così come il nodo1 ha elaborato il dato all'andata, il nodo master lo farà per il ritorno
* Una volta chiusa la comunicazione il nodo master aggiorna la pagina HTML e comunica il cambiamneto al database

N.B: Se non dovesse esserci rete internet nella rete domestica, comunque quest'ultima non ne risentirebbe e continuerebbe ad operare. Il database verrà aggiornato appena sarà presente una connessione in uscita.
Per verificare lo stato di ogni nodo, si è pensato di collegare un Display LCD o OLED. Su questo diaplay comparirà le azioni che sta eseguendo tale nodo. Esempio: Nodo1 - Display (Sto comunicando...) -> Nodo2 - Display (Sto ricevendo...)
  
  
# Fase Operativa

 ## Prima fase: Collegamento Scheda WI-FI ad una rete LAN
 Come prima fase operativa, si è collegata la scheda ESP8266 alla rete LAN, formata da un semplite PC e uno smartphone.
 Lo smartphone funge da router attraverso il collegamento hotspot Wi-Fi.
 La connessione Wi-Fi della scheda la trovate [qui](https://github.com/domoticawifi/Network-nodes/blob/master/GestioneShieldESP8266.md).
 Eseguita la semplice connessione abbiamo implementato un'interfaccia grafica in HTML per comunicare i cambiamenti di stati di alcuni attuatori e relè presenti sul nodo master (attuatori e relè sono sostituiti da semplici led per facilitare i collegamenti, ma il concetto non cambia).
 La pagina HTML viene mandata al terminale dalla scheda ESP8266 e aggiornata ogni qual volta avviene un cambiamento di stato dettato dal terminale.
 Il terminale, per accedere a tale pagina HTML, dovrà allocarsi, tramite browser(Chrome, Firefox, Edge, ....) all'indirizzo associato alla shield Wi-Fi.
 Tale indirizzo sarà visualizzato su terminale e, in un secondo momento, su un display posizionato sul nodo.
 Il programma di lancio lo trovate [qui](https://github.com/domoticawifi/Network-nodes/blob/master/sketch_SchedaWiFi_NodoMaster_Prova_AccensioneLED.ino).
 Ecco i collegamenti attraverso questo schema:
 <img src="https://i.imgbox.com/JAIu9wio.png"/>
 Su fritzing non è presente l'ESP-12E compreso di basetta per i collegamenti e USB incorporata, ma una semplice ESP-12E.
 Con un pò di fantasia immaginiamo che questa scheda sia quella che stiamo utilizzando noi(la scheda con 30 pin), per facilitarne la spiegazione.
 I due collegamenti che vanno ai led sono i pin D1 e D2.
 Fatto questo possiamo passare ad aggiungere una coppia di LED infrarossi-Ricevitore IR, per la comunicazione ottica con il nodo 1.
 Lo schema che andremo a realizzare sarà il seguente:
 <img src="https://i.imgbox.com/25MNds3H.png"/>
 Una volta realizzato il seguente collegamento ed effetuata la connessione ottica, con relativo programma di gestione, sarà possibile unire i due circuiti per realizzarne uno solo; così facendo otterremmo il nostro Nodo Master.
 
 
 ## Seconda fase: Collegamento Ottico Nodo Master -> Nodo 1
 
 ## Terza fase: Collegamneto Ottico Nodo Master -> Nodo 1 -> Nodo 2
 
  
  

