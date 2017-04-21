# Network-Nodes

Questo progetto vede la realizzazione, tramite Arduino, di una piccola rete [LAN](https://it.wikipedia.org/wiki/Local_Area_Network) (domestica o aziendale) che permette di far comunicare, attraverso un terminale PC (o qualsiasi altro device connesso alla rete), i nodi della rete; quest'ultimi collegati tra loro attraverso un collegamento ottico(infrarossi).

_____________________________________

# <a name="ancora-indice"></a> Indice
* <a href="#ancora-struttura">Struttura</a>
* <a href="#ancora-comunicazioni">Comunicazioni</a>
  * <a href="#ancora-es-comunicazione">Esempio comunicazione</a>
* <a href="#ancora-schemaNet">Schema Network</a>
* <a href="#ancora-occorrente">Occorrente</a>
  * <a href="#ancora-occ-hard">Hardware</a>
  * <a href="#ancora-occ-soft">Software</a>
* <a href="#ancora-faseop">Fase Operativa</a>
  * <a href="#ancora-faseuno">1° Fase</a>
  * <a href="#ancora-fasedue">2° Fase</a>
    * <a href="#ancora-faseduemaster">Nodo Master</a>
    * <a href="#ancora-faseduenoduno">Nodo 1</a>
  * <a href="#ancora-fasetre">3° Fase</a>
    * <a href="#ancora-protuno">Primo Prototipo: Simple Connection</a>
      * <a href="#ancora-protuno-video">Video Dimostrativo Prototipo Uno</a>
    * <a href="#ancora-protdue">Secondo Prototipo: Connected Oriented</a>
        * <a href="#ancora-protdue-video">Video Dimostrativo Prototipo Due</a>
* <a href="#ancora-db">Database</a>
* <a href="#ancora-app">Applicazione Android</a>
* <a href="#ancora-problemi">Problemi Rilevati</a>
* <a href="#ancora-conclcost">Conclusione e Costi</a>

_____________________________________

## <a name="ancora-struttura"></a> Struttura
Abbiamo presvisto un terminale PC, un nodo master e due nodi, schematizzati in questa maniera (classifichiamo questa rete come P2P):
  * Terminale (Invia e riceve le informazioni dei nodi attraverso la rete accedendo all'[indirizzo IP](https://it.wikipedia.org/wiki/Indirizzo_IP) del Nodo Master)
  * [Router](https://it.wikipedia.org/wiki/Router) (Instrada le richieste fatte tra terminale e Nodo Master sulla rete domestica e anche quella esterna andando ad aggiornare un [DB](https://it.wikipedia.org/wiki/Base_di_dati))
  * Nodo Master (Nodo formato da un modulo [ESP8266 ESP-12E](https://acrobotic.com/acr-00018) per il collegamento WI-FI con il terminale, l'invio delle informazioni sul DB e la trasmissione di informazioni tra i vari nodi presenti nella rete locale).
  * Nodo1 (Nodo formato da [Arduino NANO](https://www.arduino.cc/en/Main/arduinoBoardNano) che gestisce un attuatore/relè/led e, in futuro, un eventuale sensore)
  * Nodo2(Stessa cosa del nodo precedente)
  
  *N.B: Lo storage delle informazioni su un DB, ci serve per cotrollare lo stato della rete, anche da smartphone o tablet, in         remoto (Internetworking: il router fungerà da gateway).
  Il Nodo Master può essere, a sua volta, provvisto di attuatori e sensori.
  Il problema della scheda ESP-12E è la scarsità di PIN disponibili: nel nostro caso è sato possibile collegare un solo attuatore/relè a tale nodo a differenza dei due collegati sugli altri nodi.*
  
____________________________________________________________________________________________  
# <a name="ancora-comunicazioni"></a>Comunicazioni  
La comunicazione fra i nodi e il terminale avviene in questa maniera:
  * Il terminale comunica con il solo Nodo Master tramite la rete WI-FI, e riceve ed invia a questo nodo le richieste da fare agli altri due nodi della rete.
  * Il Nodo Master comunica con il terminale, via WI-FI, e, con il Nodo1, attraverso un collegamento ottico (LED IR e Sensor IR), e la stessa comunicazione avviene tra il Nodo1 e Nodo2
  * Ogni nodo può comunicare solamente con il suo nodo adiacente formando un ponte di nodi
  * Se il terminale deve contattare il Nodo2, la sua richiesta passerà prima a tutti i nodi
  
## <a name="ancora-es-comunicazione"></a>Esempio di comunicazione: Connected Oriented

## Terminale > Nodo Master > Nodo Interessato
* Attraverso una pagina HTML, appositamente caricata in locale attraverso la scheda Wi-Fi, avremo la possibilità di inviare informazioni alla shield da qualsiasi terminale noi vogliamo (es. PC, smartphone, tablet); inoltre dalla stessa pagina sarà possibile reperire lo stato di ogni nodo(attuatori ON/OFF e sensori)
* Se volessimo inviare una certa informazione, come ad esempio l'accensione o lo spegnimento di un LED(Attuatore e/o Relè), non dovremmo far altro che selezionare l'apposito 'Radio Button' per poi determinare l'invio della Form attraverso il Bottone 'Invio'
* In questa maniera verrà mandata una richiesta tramite rete WI-FI al Nodo Master gestito dalla scheda ESP8266
* La scheda Wi-Fi riceverà la richiesta, la inoltrerà, tramite segnale infrarossi, al nodo adiacente(Nodo 1)
* Il nodo adiacente(Nodo 1) riceve questo segnale e lo elabora per capire se è lui il destinatario
* Se così non fosse lo manda all'altro nodo adiacente(Nodo 2) con il collegamento ottico
* Quando il messaggio è arrivato a destinazione, la scheda arduino presente su tale nodo spegnerà/accenderà l'attuatore o relè
* Verrà mandato, in ritorno, l'avvenuta/o accensione/spegnimento
* Il messaggio di ritorno arriverà fino al Nodo Master che darà in risposta al PC un'altra pagina HTML aggiornata
* Il Nodo Master, successivamente, manderà questo dato sul database per poterlo consultare in remoto tramite smartphone
* Il PC riceverà la pagina HTML aggiornata e vedrà lo stato attuale di tutti i nodi per una prossima operazione  
## Nodo Master > Nodo Interessato
* Il Nodo Master avrà un proprio indirizzo IP nella rete (es: 192.168.1.100)
* Il Nodo Master riceve la richiesta dal PC da instradare al giusto nodo
* Il Nodo Master comunicherà con il Nodo1, essendo il suo adiacente
* Verrà mandato un segnale ottico al Nodo1 con il dato da elaborare
* Il Nodo1 riceve questo dato e lo elabora; una volta elaborato restituisce un ACK al Nodo Master
* Il Nodo Master chiude la comunicazione
* Il Nodo1 se dovesse ricevere un dato corrotto richiede il rinvio del dato
* Il Nodo1 elabora il dato ricevuto e capisce se appartiene a lui altrimenti lo manda, alla stessa maniera della prima comunicazione, al nodo adiacente (Nodo2)
* Così come il Nodo1 ha elaborato il dato all'andata, il Nodo Master lo farà per il ritorno
* Una volta chiusa la comunicazione il Nodo Master aggiorna la pagina HTML e comunica il cambiamento al database

*N.B: Se non dovesse esserci instradamento in uscita(connessione internet) sulla rete LAN, la connessione tra i nodi non ne risentirebbe e continuerebbe ad operare. Il database verrà aggiornato appena sarà presente una connessione in uscita.
Per verificare lo stato di ogni nodo, si è pensato di collegare un Display LCD o OLED. Su questo display compariranno le azioni che sta eseguendo tale nodo. Esempio: Nodo1 - Display (Sto comunicando...) -> Nodo2 - Display (Sto ricevendo...)*

___________________________________________________________________________________
# <a name="ancora-schemaNet"></a>Schema Network
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Progetto_Schema.png"/>
Come possiamo vedere dall'immagine, abbiamo un PC che funziona da terminale:collegato alla rete WI-FI, invia e riceve informazioni attraverso il Nodo Master.
Il Nodo Master, collegato alla rete, instrada le richieste e le informazioni, mandate dal terminale e dagli altri nodi, nella giusta direzione.

<a name="ancora-occorrente"></a>

## <a name="ancora-occ-hard"></a>Occorrente Hardware
  * 1 terminale (PC, Tablet, Smartphone)
  * 1 Router WI-FI
  * 1 ESP8266 ESP-12E oppure una qualsiasi shield WI-FI
  * 2 Arduino NANO/UNO/MEGA/Mino PRO
  * 4 TX Diodo LED infrarossi
  * 4 RX Sensori IR
  * 3 Breadboard (830 fori)
  * Cavetti e jumper per i collegamenti
  * 3 Display LCD per verificare lo stato di ogni nodo
  * 4 pulsanti
  
## <a name="ancora-occ-soft"></a>Occorrente Software
  * [Arduino IDE 1.8.0](https://www.arduino.cc/en/main/software) o superiori
  * [Firebase](https://firebase.google.com/)
  * [Core ESP8266](https://github.com/domoticawifi/Network-nodes/blob/master/GestioneShieldESP8266.md)
  * [Librearia gestione ESP8266](https://github.com/googlesamples/firebase-arduino/archive/master.zip)
  * [Libreria gestione segnale IR](https://www.pjrc.com/teensy/arduino_libraries/IRremote.zip)
  * [Libreria gestione Display LCD](https://www.dropbox.com/s/62x4w48kwf5biko/LiquidCrystalI2C.zip?dl=0)
  
Per l'installazione e la gestione delle librerie clicca [QUI](https://github.com/domoticawifi/Network-nodes/blob/master/Library/GestioneLibraryArduino.md)


________________________________________________________________________________________  
  
# <a name="ancora-faseop"></a>Fase Operativa

 ## <a name="ancora-faseuno"></a>Prima fase: Collegamento Scheda WI-FI ad una rete LAN
 Potete vedere la connessione Wi-Fi della shield cliccando <a href="https://github.com/domoticawifi/Network-nodes/blob/master/GestioneShieldESP8266.md#ancora-collegamento-wifi">QUI</a>.
 
 * Una volta testata la connessione di questa shield, si è implementata un'interfaccia grafica in HTML per comunicare i cambiamenti di stati di alcuni attuatori e relè presenti sul Nodo Master (attuatori e relè sono sostituiti da semplici led per facilitare i collegamenti, ma il concetto non cambia).
 * La pagina HTML viene mandata al terminale dalla scheda ESP8266 e aggiornata ogni qual volta avviene un cambiamento di stato dettato dal terminale.
 * Il cambiamento di stato avviene attraverso una semplice FORM HTML che invia i dati cambiati all'URL della pagina.
 Ecco la pagina HTML con la relativa Form:
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/Pagina%20HTML/Pagina%20HTML.png"/>
 
*N.B:La pagina HTML ha tutte le funzionalità! Ma di queste solo l'accensione del relè e dell'attuatore sul Nodo Master funziona! Il resto verrà implementato man mano...*
 
 * Il terminale, per accedere a tale pagina HTML, dovrà allocarsi, tramite browser(Chrome, Firefox, Edge, ....) all'indirizzo IP associato alla shield Wi-Fi.
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
 
 
 ## <a name="ancora-fasedue"></a>Seconda fase: Collegamento Ottico Nodo Master -> Nodo 1
 Per questa fase, avremo la comunicazione fra due nodi, quindi dividiamola, per semplicità, in due sotto-fasi.
 
 ### <a name="ancora-faseduemaster"></a>*Nodo Master: Collegamento Ottico*
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
  
 Per connettere la scheda al display LCD, vi rimando a [questa pagina](https://www.losant.com/blog/how-to-connect-lcd-esp8266-nodemcu).
 
 *N.B: Possiamo anche non utilizzare i resistori, perchè la tensione di uscita sui pin non supera i 3volt. Se dovessimo usare resistenze alte la luce risulterebbe più fievole, comportando un possibile ostacolo alla comunicazione*
 
 *Durante i collegamneti e i test abbiamo riscontrato un problema! Non sappiamo a cosa sia dovuto, ma se dovessimo alimentare il sensore IR con i 5v della shield Wi-Fi, i dati risulterebbero molto corrotti! Utilizzando l'alimentazione di Arduino i risultati sono migliori... comunque abbiamo dei piccoli problemi, dovuti a disturbi e alla scarsa schermatura, nel rilevare il valore dal sensore. Spiegheremo questo più avanti...*
 
 
 Da aggiungere all'occorrente software, per la realizzazione del Nodo Master, è la libreria per la gestione dei segnali infrarossi per la shield ESP8266 (purtroppo non può essere utilizzata la stessa libreria di arduino!).
 Possiamo scaricarla da [qui](https://github.com/markszabo/IRremoteESP8266/), dove è presente una piccola guida per l'installazione.
 
 
 ### <a name="ancora-faseduenoduno"></a>*Nodo 1: Collegamento Ottico*
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
  <td>PIN A4 </td>
  <td>Display SDA</td>
 </tr>
 <tr>
  <td>PIN A5 </td>
  <td>Display SCL</td>
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
 
 
 Per le prime prove, si è installato, sui due nodi, un emettitore(sul Nodo Master) e un sensore infrarossi(sul Nodo 1), per un semplice test di comunicazione.
 Si è inviato un piccolo dato, sottoforma di codifica NEC, dal Nodo Master al Nodo 1. Si hanno a disposizione tanti tipi di codifica: NEC, Sony, Sharp e tante altre...
 
 Nel nostro caso, ogni nodo avrà almeno una coppia led-sensore per l'invio e ricezione infrarossi, quindi il procedimento è più complesso.
 Il punto che va a favore è che si conosce il dato che potrebbe arrivare, questo perchè possono verificarsi solo queste situazioni:
 * Attivazione Relè Nodo 1;
 * Attivazione Attuatore Nodo 1;
 * Attivazione Relè Nodo 2;
 * Attivazione Attuatore Nodo 2.
 
Quindi i segnali che possono essere inoltrati dall'emettitore IR(sul Nodo Master) sono essenzialmente 4; a questi c'è da aggiungere un solo segnale di ritorno(Nodo 1, Nodo 2) che verifica la corretta ricezione del dato.

Procediamo per gradi...

* Il Nodo 1 riceve un dato dal Nodo Master attraverso la comunicazione infrarossi;
* Il Nodo Master, una volta inviato il dato, rimane in ascolto per l'ok(ACK);
* Il Nodo 1 elabora tale dato e verifica se può usarlo;
* Se può usarlo, lo elabora ed esegue un comando. Fatto questo manda l'ok al Nodo Master;
* Se non può usarlo manda al Nodo Master un dato per il rinvio del dato(il dato potrebbe essere corrotto);
* Il Nodo Master, in base alla risposta del Nodo 1, ritrasmette il dato oppure chiude la comunicazione.

Tutto questo dovrà essere implementato con un elegante e bell'algoritmo. 

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Flow-Chart/Flowchart%20-%20Nodo%20Master.png"/>

Questo è l'algoritmo che dovrà eseguire il Nodo Master una volta inviato il dato.
Il Nodo 1 dovrè elaborare il dato inviato e mandargli la conferma.

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Flow-Chart/Flowchart%20-%20Nodo%201.png"/>

*N.B: Se dovesse passare un certo lasso di tempo(es. 10 secondi) la comunicazione si interrompe!*


Una volta eseguiti tutti i collegamenti, possiamo eseguire gli sketch dei due nodi:
* [Programma Nodo Master](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo%20Master(Comunicazione%201%20Nodo).ino)
* [Programma Nodo 1](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo%201%20-%20Comunicazione%201%20Nodo.ino)

*N.B: Questi due programmi sono per la comunicazione fra Nodo Master e Nodo 1. Se dovessimo collegare un altro nodo(come faremo più in la) i due programmi cambieranno(cambierà solo il programma del Nodo 1).*

Con il lancio di questi due programmi otteniamo il risultato voluto:
* Il client si connette all'indirizzo IP della scheda wi-fi;
* Decide di cambiare lo stato di un attuatore e/o relè sul Nodo 1; 
* Il Nodo Master riceve il dato inviato e lo elabora inviandolo al Nodo 1;
* Il Nodo 1 elabora il dato ricevuto; in questa fase il Nodo Master rimane in ascolto per 10 secondi;
* Il Nodo 1, se accetta il dato, manda un ACK al Nodo Master;
* Il Nodo Master riceve l'ok è chiude la comunicazione aggiornando la pagina HTML che visualizzerà il client.

Il compito del Nodo Master è quello di un server web e di un "Ponte di comunicazione" tra il terminale e gli altri nodi.

*N.B: Potrebbe capitare che:*
* *La comunicazione sia interrotta da un ostacolo:in questo caso il Nodo Master invia per 10 secondi lo stesso dato fino a che non riceverà conferma. Se non dovesse ricevere conferma chiude comunicazione e la pagina HTML non viene aggiornata*
* *La comunicazione viene interrotta solo dopo che il Nodo 1 ha ricevuto il dato corretto(in questo caso non potrà comunicare l'ok):*
*Potremmo risolvere questo problema mandando in ritorno una pagina con un WARNING di comunicazione attraverso un javascript.
Purtroppo non possiamo prevenire ostacoli durante la comunicazione, ma su 100 comunicazione provate, solamente un 10%, non ha ricevuto l'ACK dal Nodo 1!
Mandando il WARNING il client sa che qualcosa non è andato nel modo giusto, perchè potrebbe risultare anche un guasto a qualche componente di comunicazione.
Questo problema potrebbe essere in parte gestito...
Si è pensato di mandare, attraverso il Nodo Master, lo stato precedente del dato che non è stato ricevuto; così facendo si ha la sicurezza, in parte, di far tornare il Nodo 1 allo stato precedente. Questo è possibile se e solo se la comunicazione è interrotta solo nella ricezione della risposta e non nell'invio del dato!*

*PS:
Durante la fase di ascolto del Nodo Master, il Nodo 1 manda un ACK per circa 10 secondi; nei dieci secondi di ascolto, da parte del Nodo Master, vengono prelevati valori random non inviati dal Nodo 1. 
Fortunatamente, nell'arco dei 10 secondi di ascolto, il dato, inviato dal Nodo 1, viene letto dal Nodo Master ricevendo l'ok di avvenuta ricezione.*

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Foto/Nodo%20Master%20%2B%20Nodo%201(Completo).jpg"/></a>

Come dicevamo in precedenza: il terzo arduino serve solo per alimentare il ricevitore del Nodo Master e in secondo a prelevare i valori del sensore, solamente per essere testati.

Il ricevitore IR, del Nodo Master, ci ha dato parecchi problemi:
* I valori rilevati erano falsati(riceveva valori anche quando non venivano inviati segnali infrarossi)
* L'alimentazione della shield Wi-Fi dava parecchi problemi al sensore durante la lettura

##### *Come si è risolto tutto ciò?*
Si è utilizzata, prima di tutto, un'alimentazione esterna, presa da un altro Arduino UNO (l'alimentazione può essere presa anche da una pila esterna), risolvendo in parte i problemi di lettura del sensore; in questo modo parte del primo problema è stata risolta, ma tuttavia continua, in modo random, a ricevere valori casuali.
Non si è venuto a capo di questo!!!
Si è cercato di risolvere la cosa nel migliore dei modi: Potrebbe essere un problema della scheda, un problema del sensore, un problema di alimentazione... Potrebbe essere tutto o niente... 


*N.B: potrebbe capitare che il dato inviato dal Nodo 1 non venga letto del sensore del Nodo Master, ma su 100 prove non è mai successo! Il dato prima o poi verrà letto!*

 
 ## <a name="ancora-fasetre"></a>Terza fase: Collegamento Ottico Nodo Master -> Nodo 1 -> Nodo 2
 Per questa terza fase implementiamo due prototipi:il primo avrà una semplice comunicazione senza risposta del Nodo 2(cioè senza verificare la ricezione del dato), mentre il secondo sarà quello più completo e strutturato in cui anche il Nodo 2 interagisce attraverso l'architettura "Connected Oriented" sfruttata già dagli altri due nodi.
 
 ### <a name="ancora-protuno"></a>*Primo prototipo: Simple Connection*
 * Lo schema per il Nodo Master rimane lo stesso, senza nessuna variazione neanche nel codice(essendo già tutta implementata)
 * Lo schema del Nodo 1 subische piccolissime variazioni(un'aggiunta di un solo LED infrarossi e due pulsanti per l'accensione dei dispositivi):
 <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Nodo%201%20-%20Comunicazione%20Nodo%20Master%20-%20Nodo%201(Incompleta).png"/>
 
Si è semplicemente aggiunto un altro diodo led infrarossi per inviare il segnale al Nodo 2.
In questo caso abbiamo il LED IR aggiuntivo collegato allo stesso piedino del primo LED IR(in modo da sfruttare la libreria per l'invio del segnale IR, questo perchè, come dicevamo, la libreria IRremote utilizza, per default, il PIN3 di arduino).
Il codice sorgente è leggermente diverso rispetto al primo.
Lo sketch lo trovate [qui](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo_1%20-%20Comunicazione%20Nodo%20Master%20e%20Nodo%201.ino).

Si sono aggiunti due pulsanti per controllare, in modo analogico, lo stato dei sispositivi collegati al Nodo 1.
Una volta avvenuto il cambiamento viene comunicato lo stato al Nodo Master.

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
 <tr>
 <td>PIN A4</td>
 <td>Display SDA</td> 
 </tr>
 <tr>
 <td>PIN A5</td>
 <td>Display SCL</td> 
 </tr>
</table>

Lo sketch da far eseguire a questa scheda è il [seguente](https://github.com/domoticawifi/Network-nodes/blob/master/Sketch/Sketch%20Definitivi/Nodo_2%20-%20Incompleto.ino)

La disposizione dei tre nodi è il seguente:
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Foto/IMG_20170404_150244.jpg"/>

Da destra verso sinistra: Nodo Master(con modulo ESP8266), Nodo 1(con Arduino UNO), Nodo 2(con Arduino UNO).

*N.B: In questo caso la comunicazione, per attivare attuatori e relè sul Nodo 2 è del tipo semplice... cioè senza ack. Non si è ancora implementata una Connected Oriented tra Nodo 1 e Nodo 2.*

#### <a name="ancora-protuno-video"></a>*Video*

<table align='center'>
  <tr align='center'>
   <td>Video </td>
   <td>Link</td>
  </tr>
  <tr align='center'>
   <td>Promo</td>
   <td>https://youtu.be/ombGCsYmXGk</td>
   </tr>
   <tr align='center'>
   <td>Completo</td>
   <td>https://youtu.be/g6Z9CRAygbg</td>
   </tr>
 </table>
 
________________________________________________________ 


### <a name="ancora-protdue"></a>*Secondo prototipo: Connected Oriented*

Per il secondo prototipo si devono apportare delle piccole modifiche:
* Modificare lo sketch del Nodo Master;
* Modificare lo schema e lo sketch del Nodo 1;
* Modificare lo schema e lo sketch del Nodo 2.

Per il primo prototipo non era richiesta la risposta dell'avvenuta ricezione del dato da parte del Nodo 2, mentre in questo prototipo vogliamo instaurare un "dialogo" tra Nodo 1 e Nodo 2, così come quello che avviene tra gli altri due nodi.
Per far ciò dobbiamo collegare un altro sensore infrarosso, questa volta rivolto verso il Nodo 2, in modo da captare le frequenze infrarosse di quest'ultimo.
Questo purtroppo non è possibile! Arduino non riesce a gestire due sensori IR perchè nella parte di programmazione non è possibile creare due oggetti(o istanze) di due diversi sensori con la stessa classe!
Questo descritto è un problema in ambito di programmazione ma si è pensato a delle possibili soluzioni:
* Un sensore IR sferico(360°) in modo da poter coprire, interamente, entrambi i nodi; risulterebbero dei problemi nella ricezione, perchè riceverà da entrambi i versi, ma logicamente potrebbe funzionare;
* Una serie di specchi che converge il fascio del segnale infrarossi del Nodo 2 verso il sensore del Nodo 1; potrebbe sembrare fantascienza ma potremmo usare lo stesso principio dei telescopi;
* Per ultimo, ma non meno importante, la "triangolazione".

*Cos'è la "triangolazione"?*

*Con triangolazione intendiamo la disposizione dei nodi a formare un triangolo.
Come in questa figura:*

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/Esempio%20Triangolazione.png"/>

*In questo modo i nodi sono disposti in modo tale da poter inviare al Nodo 1 i segnali infrarossi.*

Logicamente è un modo corretto da realizzare, ma facendo alcune prove e test si è verificato che questa comunicazione non può avvenire!
Il sensore del Nodo 1 è soggetto a troppe informazione e non riesce a captare nel modo corretto il segnale di ritorno del Nodo 2.
Si è pensato ad un altro tipo di soluzione, da aggiungere alle altre elencate poco fa:
* Gestire il cambio di sensore attraverso un circuito switch a transistor: due transistor per alimentare due diversi circuiti, in modo alternato, in base alla ricezione del dato(*soluzione hardware*).

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Schemi%20%26%20Circuiti/schema%20circuito%20nodo%201%20per%20sensore%20IR.png"/>

*N.B: Questo potrebbe essere realizzato anche con un semplice relè! Basta collegare uno dei due sensori all'ingresso "Normalemente chiuso" e lìaltro al "Normalmente aperto" del relè... In modo che solo uno dei due sensori è in funzione!*

Si potrebbe progettare un "Secondo prototipo" con tutti i metodi risolutivi elencati e constatare quanti e quali di questi metodi funzionino...
In questo caso realizzeremo l'ultimo metodo risolutivo:

### *Risoluzione Switch a transistor:*
<h2>Per quanto questo metodo possa essere logicamente corretto purtroppo non può essere realizzato!
Perchè non è possibile collegare, allo stesso pin di Arduino, i due pin dei sensori che trasportano il segnale... Quando uno dei due sensori è interdetto l'altro sensore non percepisce nessun dato... Come se il circuito fosse aperto!</h2>

### *Risoluzione Sensore Sferico:*




#### <a name="ancora-protdue-video"></a>*Video*

_____________________________________
# <a name="ancora-db"></a>Database (Firebase)
Adesso non ci resta che implementare un database dove salvare i nostri dati, che saranno visibili in remoto attraverso un'app Android.
Il database che si è utilizzato per questo progetto è [Firebase](https://firebase.google.com/) (database NoSQL).
Firebase è semplice ed intuitivo da utilizzare.
La struttura che utilizza non è una struttura tabellare ma ad albero.

Una volta implementato il DB dobbiamo effettuare la connessione a Firebase grazie ad una librearia per l'ESP8266 che è presente nell'occorrente software elencato a inizio pagina.
Abbiamo bisogno, per il collegamento, dell'HOSTNAME e dell'autorizzazione:
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
* Il Nodo Master riceve questa modifica attraverso la form e manda i dati, attraverso la connessione ottica, al Nodo 1
* Il Nodo 1 elabora i dati e li esegue o li invia al Nodo 2
* Il Nodo 1, una volta eseguito tutto, manda una risposta al Nodo Master, confermando l'avvenuta ricezione
* Il Nodo Master aggiorna la pagina HTML da mandare al client e contemporaneamente aggiorna il DB
* La pagina HTML viene aggiornata

In remoto sarà presente uno smartphone o tablet android con un'app per il controllo dello stato.
Dalla semplice app, realizzata con [AppInventor](http://appinventor.mit.edu/explore/), sarà possibile monitorare lo stato della rete e di tutti i nodi.
In futuro potrebbe essere implementato anche un controllo, in remoto, dell'intera rete.

## <a name="ancora-app"></a>*App Android*
<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Screenshot/Screenshot_2017-04-06-09-32-16.png"/>

<img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Screenshot/Screenshot_2017-04-06-09-32-34.png"/>

Come dicevamo, l'app è stata realizzata attraverso un software open source online della Google: App Inventor.
Tale software è molto semplice ed intuitivo.
in futuro verrà fatta una guida su come utilizzarlo.

L'app presenta una schermata principale in cui ci sono due grossi bottoni:
* Un occhio = rappresenta il controllo dello stato della rete
* Un joypad = rappresenta il controllo in remoto delle modifiche da portare ai nodi

Per il momento il controllo della modifica dei nodi non può essere effettuato (verrà implementato in un secondo momento)
Cliccando sull'occhio abbiamo l'apertura della pagina "Controllo stato", in cui sarà presente lo stato di ogni nodo.
Tale stato sarà prelevato da Firebase.

Potete scaricare il file APK da [QUI](https://github.com/domoticawifi/Network-nodes/blob/master/Android%20APP/NetworkNodes_App_Android.apk) oppure accedere alla cartella "Android APP" sulla pagina principale della reposity di GitHub.
_____________________________________

# <a name="ancora-problemi"></a>Problemi rilevati

In questa parte verranno riportati tutti i problemi rilevati durante la realizzazione del progetto; questi problemi sono già riportati nella relazione, questa sezione è solo un resoconto di tutti i problemi e le loro risoluzioni.

## *Problemi*
* Alimentazione instabile shield ESP8266
* Lettura instabile sensore IR Nodo Master
* Interruzione comunicazione ottica da parte di un oggetto o altro
* Comunicazione "Connected Oriented" Nodo 1 - Nodo 2

## *Soluzioni*
* L'alimentazione instabile (5v) della shield, non ci ha permesso di collegare il sensore IR con i 5v della scheda; si è risolto tale problema alimentando il sensore con i 5v di un'altra scheda Arduino.
* La lettura instabile del sensore IR sul Nodo Master non è stato risolto! Purtroppo la scheda rileva letture random di valori non conformi a quelli inviati dal nodo adiacente. Questo problema è dato solo dal Nodo Master, mentre la comunicazione tra il Nodo 1 e Nodo 2 avviene in modo impeccabile. Questo problema è comunque in parte risolto grazie all'invio continuo dell'ACK da parte del Nodo 1 al Nodo Master, in modo da fargli leggere, prima o poi, l'ok di avvenuta ricezione. Purtroppo la sicuezza assoluta non c'è!
* L'interruzione della comunicazione può avvenire in due modi: 
   * Oggetto che ostacola la comunicazione ottica;
   * Malfunzionamento di uno dei componenti di comunicazione.
   
  Per entrambi i casi si è previsto un javascript con un WARNING.
  
* Per la comunicazione "Connected Oriented" tra Nodo 1 e Nodo 2 si è pensato a una serie di soluzioni che potrebbero risolvere il problema:
  * Sensore sferico <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/green-ok-icon-2.png"/>
  * Triangolazione del segnale <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/no-image-icon-9.png"/>
  * Serie di specchi per dirigere il segnale <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/green-ok-icon-2.png"/>
  * Schema di switch per doppio sensore <img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/no-image-icon-9.png"/>
  
 
   
_____________________________________
# <a name="ancora-conclcost"></a>Conclusioni e costi complessivi

### *Costi:*
I costi dei vari moduli, schede, cavi, breadboard, led e altro si aggira intorno a questo range: 30€ - 70€.
il tutto dipende da dove compriate il materiale.
Il consiglio che vi diamo è quello di comprare tutto su e-Bay... è un sito molto sicuro e trovate tutto quello di cui avete bisogno.
Vi allego il link del venditore dove abbiamo reperito tutto il materiale:
http://www.ebay.it/usr/nouteclab?_trksid=p2047675.l2559

### *Conclusioni:*

 __________________________________________________________________
 
 ## <a href="#ancora-indice">Torna all'indice </a>
 
 <a href="http://html.com/"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/HTML5_Logo_256.png"/></a> <a href="https://www.arduino.cc/"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/arduino-logo-.png"/></a> <a href="http://www.esp8266.com/wiki/doku.php?id=start"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/esp8266.png"/></a> <a href="http://appinventor.mit.edu/explore/"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/google-app-inventor-icon.jpg"/></a> <a href="https://firebase.google.com/"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/Firebase_16-logo.png"/></a>
 
 
 
 ### Network-Nodes: Pietro Rignanese & Andrea Polenta Aprile 2017
 
 <a href="https://www.facebook.com/pietro.rignanese.7"><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/face.png"/></a> <a href=""><img src="https://github.com/domoticawifi/Network-nodes/blob/master/img/Loghi/face2.png"/></a>
 
  
  

