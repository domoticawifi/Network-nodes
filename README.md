# Network-Nodes

Questo progetto vede la realizzazione, tramite Arduino, di una piccola rete LAN (domestica o aziendale) che permette di comunicare, attraverso un terminale PC (o qualsiasi altro device connesso alla rete), i nodi della rete, quest'ultimi, a loro volta, saranno collegati tra loro attraverso un collegamento ottico(infrarossi).

_____________________________________

## Struttura
Abbiamo presvisto un terminale PC, un nodo master e due nodi, schematizzati a questa maniera (classifichiamo questa rete come P2P):
  * Terminale (Invia e riceve le informazioni di tutta la rete accedendo all'indirizzo IP del Nodo Master)
  * Router (Instrada le richieste fatte tra terminale e Nodo Master sulla rete domestica e anche quella esterna andando ad aggiornare un DB)
  * Nodo Master (Nodo formato da un modulo [ESP8266 ESP-12E](https://acrobotic.com/acr-00018) per il collegamento WI-FI con il terminale, l'invio delle informazioni sul DB e la trasmissione di informazioni tra i vari nodi presenti nella rete locale).
  * Nodo1 (Nodo formato da [Arduino NANO](https://www.arduino.cc/en/Main/arduinoBoardNano) che gestisce un attuatore/relè/led e, in futuro, un eventuale sensore
  * Nodo2 (Stessa cosa del nodo precedente)
  
  *N.B: Lo storage delle informazioni su un DB, ci serve per cotrollare lo stato della rete, anche da smartphone o tablet, in         remoto (Internetworking: il router fungerà da gateway).
  Il Nodo Master può essere, a sua volta, provvisto di attuatori e sensori.
  Il problema della scheda ESP-12E è la scarsità di PIN disponibili: nel nostro caso è sato possibile collegare un solo attuatore/relè a tale nodo a differenza dei due collegati sugli altri nodi.*
  
____________________________________________________________________________________________  
# Comunicazioni  
Le comunicazioni fra i nodi e il terminale avviene in questa maniera:
  * Il terminale comunica con il solo Nodo Master tramite la rete WI-FI, e riceve ed invia a questo nodo le richieste da fare agli altri due nodi della rete.
  * Il Nodo Master comunica con il terminale, via WI-FI, e, con il Nodo1, attraverso un collegamento ottico (LED IR e Sensor IR), e la stessa comunicazione avviene tra il Nodo1 e Nodo2
  * Ogni nodo può comunicare solamente con il suo nodo adiacente formando un ponte di nodi
  * Se il terminale deve contattare il Nodo2, la sua richiesta passerà prima a tutti i nodi
  
## Esempio di comunicazione: Connected Oriented

## Terminale > Nodo Master > Nodo Interessato
* Attraverso una pagina HTML, appositamente caricata in locale attraverso la scheda Wi-Fi, avremo la possibilità di inviare informazioni alla shield da qualsiasi terminale noi vogliamo (es. PC, smartphone, tablet). inoltre da questa sarà possibile reperire lo stato di ogni nodo(attuatori ON/OFF e sensori)
* Se volessimo inviare una certa informazione, come ad esempio l'accensione o lo spegnimento di un LED, non dovremmo far altro che selezionare l'apposito 'Radio Button' per poi determinare l'invio della Form attraverso il'Button' 'Invio'
* In questa maniera verrà mandata una richiesta tramite rete WI-FI al Nodo Master gestito dalla scheda ESP8266
* La scheda Wi-Fi riceverà la richiesta, la elaborerà e controllerà se il dato dovà essere mandato, tramite segnale infrarossi, al nodo adiacente
* Il nodo adiacente riceve questo segnale e lo elabora per capire se è lui il destinatario
* Se così non fosse lo manda all'altro nodo adiacente con il collegamento ottico
* Quando il messaggio è arrivato a destinazione, la scheda arduino presente su tale nodo spegnerà/accenderà l'attuatore o relè
* Verrà mandato, in ritorno, l'avvenuta/o accensione/spegnimento
* Il messaggio di ritorno arriverà fino al nodo master che darà in risposta al PC un'altra pagina HTML aggiornata
* Il Nodo Master, successivamente, manderà questo dato sul database per poterlo consultare in remoto tramite smartphone
* Il PC riceverà la pagina HTML aggiornata e vedrà lo stato attuale di tutti i nodi per una prossima operazione  
## Nodo Master > Nodo Interessato
* Il Nodo Master avrà un proprio indirizzo IP nella rete (es: 192.168.1.100)
* Il Nodo Master riceve la richiesta dal PC da instradare al giusto nodo
* Il Nodo Master comunicherà con il Nodo1, essendo il suo adiacente
* Verrà mandato un segnale ottico al Nodo1 per informarlo che sta per avvenire una trasmissione
* Il Nodo1 accetta questa trasmissione(ACK)
* Il Nodo Master invia il dato seguito dalla chiusura della comunicazione
* Il Nodo1 riceve questo dato e la chiusura, in modo da capire che tutto è andato a buon fine, se così non fosse fa ristrasmettere il dato
* Il Nodo1 elabora questo dato e capisce se appartiene a lui altrimenti lo manda, alla stessa maniera della prima comunicazione, al nodo adiacente (Nodo2)
* Elaborato il dato ed eseguita la modifica su tale nodo, il Nodo1 manda un messaggio ottico al Nodo Master per informarli che sta per trasmettere
* Il Nodo Master accetta(ACK) e il Nodo1 comincia a trasmettere il dato seguito dalla chiusura
* Così come il Nodo1 ha elaborato il dato all'andata, il Nodo Master lo farà per il ritorno
* Una volta chiusa la comunicazione il Nodo Master aggiorna la pagina HTML e comunica il cambiamento al database

*N.B: Se non dovesse esserci rete internet nella rete domestica, comunque quest'ultima non ne risentirebbe e continuerebbe ad operare. Il database verrà aggiornato appena sarà presente una connessione in uscita.
Per verificare lo stato di ogni nodo, si è pensato di collegare un Display LCD o OLED. Su questo display comparirà le azioni che sta eseguendo tale nodo. Esempio: Nodo1 - Display (Sto comunicando...) -> Nodo2 - Display (Sto ricevendo...)*

___________________________________________________________________________________
# Schema Network
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Progetto_Schema.png"/>
Come possiamo vedere dall'immagine, abbiamo un PC, che funziona da terminale, collegato alla rete WI-FI, invia e riceve informazioni attraverso il Nodo Master.
Il Nodo Master, collegato alla rete, instrada le richieste e le informazioni, mandate dal terminale e dagli altri nodi, nella giusta direzione.

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
  
Per l'installazione e la gestione delle librerie clicca [QUI](https://github.com/domoticawifi/Network-nodes/blob/master/Library/GestioneLibraryArduino.md)


________________________________________________________________________________________  
  
# Fase Operativa

 ## Prima fase: Collegamento Scheda WI-FI ad una rete LAN
 Potete vedere la connessione Wi-Fi della shield cliccando [QUI](https://github.com/domoticawifi/Network-nodes/blob/master/GestioneShieldESP8266.md).
 
 * Una volta testata la connessione di questa shield, si è implementato un'interfaccia grafica in HTML per comunicare i cambiamenti di stati di alcuni attuatori e relè presenti sul Nodo Master (attuatori e relè sono sostituiti da semplici led per facilitare i collegamenti, ma il concetto non cambia).
 * La pagina HTML viene mandata al terminale dalla scheda ESP8266 e aggiornata ogni qual volta avviene un cambiamento di stato dettato dal terminale.
 * Il cambiamento di stato avviene attraverso una semplice FORM HTML che invia i dati cambiati all'URL della pagina.
 Ecco la pagina HTML con la relativa Form:
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/Pagina%20HTML/Pagina%20HTML.png"/>
 
*N.B:La pagina HTML ha tutte le funzionalità! Ma di queste solo l'accensione del relè e dell'attuatore sul Nodo Master funziona! Il resto verrà implementato man mano...*
 
 * Il terminale, per accedere a tale pagina HTML, dovrà allocarsi, tramite browser(Chrome, Firefox, Edge, ....) all'indirizzo associato alla shield Wi-Fi.
 * Tale indirizzo sarà visualizzato su terminale, per le prime prove, e, in un secondo momento, su un display posizionato sul nodo.
 * Il programma di lancio lo trovate [Qui](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Di%20Prova/sketch_SchedaWiFi_NodoMaster_Prova_AccensioneLED.ino).
 
Ecco i collegamenti:
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%20Master%20-%20Prova%20accensione%20LED.png"/>
 Su Fritzing non è presente l'ESP-12E compreso di basetta per i collegamenti e USB incorporata, ma una semplice ESP-12E.
 Con un pò di fantasia immaginiamo che questa scheda sia quella che stiamo utilizzando noi(la scheda con 30 pin), per facilitarne la spiegazione.
 
 I due collegamenti che vanno ai led sono i pin D1 e D2.
 
 *N.B: Nella seconda fase lasceremo solo un led che rappresenterà un solo relè/attuatore, perchè abbiamo poca disponibilità di piedini nella ESP-12E. In questo caso il led sarà collegato al piedino D4; i piedini D1 e D2 saranno riservati al display lcd I2C.*
 
 Ecco una foto dei collegamenti su breadbord:
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Foto/Nodo%20Master(Incompleto).jpg"/>
 Purtroppo la shield Wi-Fi ha una larghezza che va ha coprire tutta la breadbord e per facilitare i collegamneti abbiamo utilizzato dei jumper.
 Dalla foto risulta acceso il led 2, questo perchè abbiamo attivato, attraverso la pagina HTML visualizzata dal PC, l'accensione del led 2(Che rappresenta un semplice relè).
 
 
 Fatto questo possiamo passare ad aggiungere una coppia di LED infrarossi-Ricevitore IR, per la comunicazione ottica con il Nodo 1.
  
 
 *N.B: Possiamo anche non utilizzare i resistori, perchè la tensione di uscita sui pin non supera i 3volt.*
 
 
 ## Seconda fase: Collegamento Ottico Nodo Master -> Nodo 1
 Per questa fase, avremo la comunicazione fra due nodi, quindi dividiamola, per semplicità, in due sotto-fasi.
 
 ### *Nodo Master: Collegamento Ottico*
 Lo schema che andremo a realizzare sarà il seguente:
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%20Master%20-%20Immagine.png"/>
 
 Tale schema dovrà essere incorporato con lo schema della prima fase; così facendo avremmo il nostro Nodo Master completo!
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%20Master%20-%20Completo.png"/>
 
 I collegamenti sono i seguenti:
 <table align='center'>
  <tr align='center'>
   <td> </td>
   <td>DISPLAY</td>
   <td>LED</td>
   <td>EMETTITORE IR</td>
   <td>RICEVITORE IR</td>
  </tr>
  <tr align='center'>
   <td>ESP8266</td>
   <td>D1, D2</td>
   <td>D4</td>
   <td>D0</td>
   <td>D3</td>
   </tr>
 </table>
 
 Così facendo occuperemo tutti i pin disponibili.
 Purtroppo ESP-12E non ha tanti pin da poter essere utilizzati, perchè gli altri sono tutti gestiti dalla scheda stessa.
  
 
 *N.B: Possiamo anche non utilizzare i resistori, perchè la tensione di uscita sui pin non supera i 3volt. Se dovessimo usare resistenze alte la luce risulterebbe più fievole, comportando un possibile ostacolo alla comunicazione*
 
 *Durante i collegamneti e i test abbiamo riscontrato un problema! Non sappiamo a cosa sia dovuto, ma se dovessimo alimentare il sensore IR ai 5v della shield Wi-Fi, i dati sono molto corrotti! Utilizzando l'alimentazione di Arduino i risultati sono migliori... comunque abbiamo dei piccoli problemi, dovuti a disturbi e alla scarsa schermatura, nel rilevare il valore dal sensore. Spiegheremo questo più avanti...*
 
 
 Da aggiungere all'occorrente software, per la realizzazione del Nodo Master, è la libreria per la gestione dei segnali infrarossi per la shield ESP8266 (purtroppo non può essere utilizzata la stessa libreria di arduino!).
 Possiamo scaricarla da [qui](https://github.com/markszabo/IRremoteESP8266/), dove è presente una piccola guida per l'installazione.
 
 
 ### *Nodo 1: Collegamento Ottico*
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%201%20-%20Incompleto.png"/>
 
 *N.B:Può essere usato, tranquillamente, Arduino UNO! Arduino Nano è esattamente uguale(tranne nella dimensione) ad Arduino UNO! Noi per le prove abbiamo utilizzato Arduino UNO per comodità...*
 
 Connessioni:
 
 <table>
 <tr align='center'>
  <td>Arduino UNO/NANO</td>
  <td> </td>
 </tr>
 <tr>
  <td>PIN 2</td>
  <td>Relè</td>
 </tr>
 <tr>
  <td>PIN 3 </td>
  <td>Emettitore LED IR</td>
 </tr>
 <tr>
  <td>PIN 4 </td>
  <td>Attuatore</td>
 </tr>
 <tr>
  <td>PIN 8 </td>
  <td>Ricevitore IR</td>
 </tr>
 <tr>
  <td>5v </td>
  <td>Breadboard</td>
 </tr>
 <tr>
  <td>GND </td>
  <td>Breadboard</td>
 </tr>
</table>

Il collegamento dell'emettitore IR è obbligatorio farlo sul PIN3! Perchè la libreria di Arduino sull'invio del segnare infrarosso utilizza quel piedino per default. Se volessimo cambiare piedino dovremmo andare ad agire sulla libreria.
 
 
 Per le prime prove, si è installato, sui due nodi, un emettitore(Nodo Master) e un sensore infrarossi(Nodo 1), per un semplice test di comunicazione.
 Si è inviato un piccolo dato, sottoforma di codifica NEC, dal Nodo Master al Nodo 1. Si hanno a disposizione tanti tipi di codifica: NEC, Sony, Sharp e tante altre...
 
 Nel nostro caso, ogni nodo avrà almeno una coppia led-sensore per l'invio e ricezione infrarossi, quindi il procedimento è più complesso.
 Il punto che va a favore è che si conosce il dato che potrebbe arrivare, questo perchè possono verificarsi solo queste situazioni:
 * Attivazione Relè Nodo 1
 * Attivazione Attuatore Nodo 1
 * Attivazione Relè Nodo 2
 * Attivazione Attuatore Nodo 2
 
Quindi i segnali che possono essere inoltrato dall'emmettitore IR(Nodo Master) sono essenzialmente 4; a questi ce da aggiungere un solo segnale di ritorno(Nodo 1, Nodo 2) che verifica la corretta ricezione del dato.

Procediamo per gradi...

* Il Nodo 1 riceve un dato dal Nodo Master attraverso la comunicazione infrarossi
* Il Nodo Master, una volta inviato il dato, rimane in ascolto per l'ok(ACK)
* Il Nodo 1 elabora tale dato e verifica se può usarlo
* Se può usarlo, lo elabora ed esegue un comando. Fatto questo manda l'ok al Nodo Master
* Se non può usarlo manda al Nodo Master un dato per il rinvio del dato(il dato potrebbe essere corrotto)
* Il Nodo Master, in base alla risposta del Nodo 1, ritrasmette il dato oppure chiude la comunicazione

Tutto questo dovrà essere implementato con un elegante e bell'algoritmo. 

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Flow-Chart/Flowchart%20-%20Nodo%20Master.png"/>

Questo è il flow chart che dovrà eseguire il Nodo Master una volta inviato il dato.
Il Nodo 1 dovrè elaborare il dato inviato e mandarli la conferma.

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Flow-Chart/Flowchart%20-%20Nodo%201.png"/>

*N.B: Se dovesse passare un certo lasso di tempo(comunicazione interrotta) la comunicazione si interrompe!*

________________________________________

Una volta eseguiti tutti i collegamenti, possiamo eseguire gli sketch dei due nodi:
* [Programma Nodo Master](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo%20Master(Comunicazione%201%20Nodo).ino)
* [Programma Nodo 1](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo%20Master(Comunicazione%201%20Nodo).ino)

*N.B: Questi due programmi sono per la comunicazione fra Nodo Master e Nodo 1. Se dovessimo collegare un altro nodo(come faremo più in la) i due programmi cambieranno(cambierà solo il programma del Nodo 1).*

Con il lancio di questi due programmi otteniamo il risultato voluto:
* Il client si connette all'indirizzo IP della scheda wi-fi
* Decide di cambiare lo stato di un attuatore e/o relè sul Nodo 1 
* Il Nodo Master riceve il dato inviato e lo elabora inviandolo al Nodo 1
* Il Nodo 1 elabora il dato ricevuto; in qusta fase il Nodo Master rimane in ascolto per 10 secondi
* Il Nodo 1, se accetta il dato, manda un ACK al Nodo Master
* Il Nodo Master riceve l'ok è chiude la comunicazione aggiornando la pagina HTML che visualizzerà il client

Il compito del Nodo Master è quello di un server web e di un "Ponte di comunicazione" tra il terminale e gli altri nodi.

*N.B: Potrebbe capitare che:*
* *La comunicazione sia interrotta da un ostacolo:in questo caso il Nodo Master invia per 10 secondi lo stesso dato fino a che non riceverà conferma. Se non dovesse ricevere conferma chiude comunicazione e la pagina HTML non viene aggiornata*
* *La comunicazione viene interrotta solo dopo che il Nodo 1 ha ricevuto il dato corretto(in questo caso non potrà comunicare l'ok):*
*Potremmo risolvere questo problema mandando in ritorno una pagina con un WARNING di comunicazione attraverso un javascript.
Purtroppo non possiamo prevenire ostacoli durante la comunicazione, ma su 100 comunicazione provate, solamente un 10%, non ha ricevuto l'ACK dal Nodo 1!
Mandando il WARNING il client sa che qualcosa non è andato nel modo giusto, perchè potrebbe risultare anche un guasto a qualche componente di comunicazione.
Questo problema potrebbe essere in parte gestito...
Si è pensato di mandare, attraverso il Nodo Master, lo stato precedente del dato che non è stato ricevuto; così facendo si ha la sicurezza, in parte, di far tornare il Nodo 1 allo stato precedente. Questo è possibile se e solo se la comunicazione è interrotta solo nella ricezione della risposta e non nell'invio del dato!*

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Foto/Nodo%20Master%20%2B%20Nodo%201(Completo).jpg"/></a>

Come dicevamo in precedenza: il terzo arduino serve solo per alimentare il ricevitore del Nodo Master e in secondo a prelevare i valori del sensore, solamente per essere testati.

Il ricevitore IR, del Nodo Master, ci ha dato parecchi problemi:
* I valori rilevati erano falsati(riceveva valori anche quando non venivano inviati segnali infrarossi)
* L'alimentazione della shield Wi-Fi dava parecchi problemi al sensore durante la lettura

##### *Come si è risolto tutto ciò?*
Si è utilizzata, prima di tutto, un'alimentazione esterna, presa da un altro Arduino UNO (l'alimentazione può essere presa anche da una pila esterna), risolvendo in parte i problemi di lettura del sensore.
Per il primo problema, una parte è stata risolta risolvendo il secondo problema, ma continuava, in modo random a ricevere valori casuali.
Non si è venuto a capo di questo!!!
Si è cercato di risolvere la cosa nel migliore dei modi: Potrebbe essere un problema della scheda, un problema del sensore, un problema di alimentazione... Potrebbe essere tutto o niente... 

Durante la fase di ascolto del Nodo Master, il Nodo 1 manda un ACK per circa 10 secondi; nei dieci secondi di ascolto, da parte del Nodo Master, vengono prelevati valori random non inviati dal Nodo 1. 
Fortunatamente, nell'arco dei 10 secondi di ascolto, il dato, inviato dal Nodo 1, viene letto dal Nodo Master ricevendo l'ok di avvenuta ricezione, quindi i 2 problemi riscontrati sono stati risolti.

*N.B: potrebbe capitare che il dato, inviato dal Nodo 1, non venga letto del sensore del Nodo Master, ma su 100 prove non è mai successo! Il dato prima o poi verrà letto!*

 
 ## Terza fase: Collegamento Ottico Nodo Master -> Nodo 1 -> Nodo 2
 Per questa terza fase implementiamo due prototipi:il primo avrà una semplice comunicazione senza risposta del Nodo 2(cioè senza verificare la ricezione del dato), mentre il secondo sarà quello più completo e strutturato in cui anche il Nodo 2 interagisce attraverso l'rchitettura "Connected Oriented" sfruttata già dagli altri due nodi.
 
 ### *Primo prototipo: Simple Connection*
 * Lo schema per il Nodo Master rimane lo stesso, senza nessuna variazione neanche nel codice(essendo già tutta implementata)
 * Lo schema del Nodo 1 subische piccolissime variazioni(un'aggiunta di un solo LED infrarossi):
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%201%20-%20Comunicazione%20Nodo%20Master%20-%20Nodo%201(Incompleta).png"/>
 
Si è semplicemente aggiunto un altro diodo led infrarossi per inviare il segnale al Nodo 2.
In questo caso abbiamo il LED IR aggiuntivo collegato allo stesso piedino del primo LED IR(in modo da sfruttare la libreria per l'invio del segnale IR, questo perchè, come dicevamo, la libreria IRremote utilizza, per default, il PIN3 di arduino).
Il codice sorgente è leggermente diverso rispetto al primo.
Lo sketch lo trovate [qui](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo_1%20-%20Comunicazione%20Nodo%20Master%20e%20Nodo%201.ino).

* Lo schema del Nodo 2 è il seguente:
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%202.png"/>

I collegamenti:
<table>
 <tr>
 <td>Arduino NANO</td>
 <td> </td>
 </tr>
 <tr>
 <td>PIN 2</td>
 <td>LED(Relay)</td>
 </tr>
 <tr>
 <td>PIN 4</td>
 <td>LED(Attuatore)</td>
 </tr>
 <tr>
 <td>PIN 8</td>
 <td>Ricevitore IR</td> 
 </tr>
</table>

Lo sketch da far eseguire a questa scheda è il [seguente](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo_2%20-%20Incompleto.ino)

La disposizione dei tre nodi è il seguente:
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Foto/IMG_20170404_150244.jpg"/>

Da destra verso sinistra: Nodo Master(con modulo ESP8266), Nodo 1(con Arduino UNO), Nodo 2(con Arduino UNO).

*N.B: In questo caso la comunicazione, per attivare attuatori e relè sul Nodo 2 è del tipo semplice... cioè senza ack. Non si è ancora implementata una Connected Oriented tra Nodo 1 e Nodo 2.*

### *Secondo prototipo: Connected Oriented*

_____________________________________
# Database (Firebase)
Adesso non ci resta che implementare un database dove salvare i nostri dati, che saranno visibili in remoto attraverso un'app Android.
Questo è lo sketch da mettere nel Nodo Master:[Sketch Nodo Master Completo](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo_Master_Database.ino). Questo sketch fa si che ogni volta che si aggiorni la pagina, venga aggiornato anche il database.
il database che si è utilizzato per questo progetto è [Firebase](https://firebase.google.com/) (database NoSQL).
Firebase è semplice ed intuitivo da utilizzare.
La struttura che utilizza non è una struttura tabellare ma ad albero.

Una volta implementato il DB dobbiamo effettuare la connessione a Firebase grazie ad una librearia per l'ESP8266 che è presente nell'occorrente software elencato a inizio pagina.
Abbiamo bisogno, per il collegamento dell'HOSTNAME e dell'autorizzazione:
* Accedete allo sketch del Nodo Master attraverso Arduino IDE
* Aprite Firebase
* Andate al vostro DB creato in precedenza
* Copiate l'hostname del DB e incollatelo nello sketch alla variabile FIREBASE_HOST senza https:// e lo / finale
* Andate su Firebase > Impostazioni > Database > Database Secret
* Cliccate su SHOW e copiate il codice di autorizzazione
* Incollate il codice nella variabile FIREBASE_AUTH presente nello sketch

*N.B:Nello sketch postato su GitHub, non è presente ne SSID, Password, FIREBASE_HOST, FIREBASE_AUTH, perchè sono dati sensibili!
Dovrete inserire al posto dei "****" le vostre credenziali.*

Elenchiamo quello che succede nello sketch da eseguire sul Nodo Master(gli altri sketch, delle altre schede poste sugli altri nodi, rimangono invariati):
* Il client accede all'indirizzo IP della pagina HTML e fa una richiesta di modifica dei nodi
* Il Nodo Master riceve questa modifica attraverso la forma e manda i dati, attraverso la connessione ottica, al Nodo 1
* Il Nodo 1 elabora i dati e li esegue o li invia al Nodo 2
* Il Nodo 1, una volta eseguito tutto, manda una risposta al Nodo Master, confermando l'avvenuta ricezione
* Il Nodo Master aggiorna la pagina HTML da mandare al client e contemporaneamente aggiorna il DB
* La pagina HTML viene aggiornata

In remoto sarà presente uno smartphone o tablet android con un'app per il controllo dello stato in remoto.
Dalla semplice app, realizzata con [AppInventor](http://appinventor.mit.edu/explore/), sarà possibile monitorare lo stato della rete e di tutti i noti.
In futuro potrebbe essere implementato anche un controllo, in remoto, dell'intera rete.

## *App Android*
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Screenshot/Screenshot_2017-04-06-09-32-16.png"/>

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Screenshot/Screenshot_2017-04-06-09-32-34.png"/>

Come dicevamo, l'app è stata realizzata attraverso un software open source online della Google: App Inventor.
Tale software è molto semplice ed intuitivo.
in futuro verrà fatta una guida su come utilizzarlo.

L'app presenta una schermata principale in cui ci sono due grassi bottoni:
* Un occhio = rappresenta il controllo dello stato della rete
* Un joypad = rappresenta il controllo in remoto delle modifiche da portare ai nodi

Per il momento il controllo della modifica dei nodi non può essere effettuato (verrà implementato in un secondo momento)
Cliccando sull'occhio abbiamo l'apertura della pagina "Controllo stato", in cui sarà presente lo stato di ogni nodo.
tale stati saranno prelevati da Firebase.
_____________________________________
# Conclusioni e costi complessivi

### *Costi:*
I costi dei vari moduli, schede, cavi, breadboard, led e altro si aggira intorno a questo range: 30€ - 70€.
il tutto dipende da dove compriate il materiale.
Il consiglio che vi diamo è quello di comprare tutto su e-Bay... è un sito molto sicuro e trovate tutto quello di cui avete bisogno.
Vi allego il link del venditore dove abbiamo reperito tutto il materiale:
http://www.ebay.it/usr/nouteclab?_trksid=p2047675.l2559

### *Conclusioni:*

 __________________________________________________________________
 
 ### Network-Nodes: Pietro Rignanese & Andrea Polenta Aprile 2017
 
  
  

