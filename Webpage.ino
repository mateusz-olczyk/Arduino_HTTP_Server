const PROGMEM char prgstrHTTPHeader[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nRefresh: 150\n\n";
const int maxIncrementValueSize = 10;
const char newIncValueRequest[] = "NEW_LITRES_VALUE";
const int newIncValueRequestSize = 16;
const int maxHeaderSize = 6 + newIncValueRequestSize + maxIncrementValueSize; 
const PROGMEM char prgstrPageBegin[] = "<!doctype html>\n<html>\n<head>\n<title>Panel urzadzenia</title>\n<script src=\"http://www.chartjs.org/dist/2.7.1/Chart.bundle.js\"></script>\n<style>\ncanvas{\n-moz-user-select: none;\n-webkit-user-select: none;\n-ms-user-select: none;\n}\ndiv{\nwidth: 50%;\nmargin: 0 auto;\n}\n</style>\n<script>\nfunction sendLitresAmount() {\nvar request = new XMLHttpRequest();\nvar counter_litres = document.getElementById(\"counter_litres_input\").value;\nrequest.open(\"POST\", \"NEW_LITRES_VALUE\" + counter_litres, true);\nrequest.send(counter_litres);}\ndocument.getElementById(\"counter_litres_input\").value=\"\";\n</script>\n</head>\n<body>\n<div style=\"width:75%;\">\n<canvas id=\"canvas\"></canvas>\n<div>\n<p>Liczba litrow na pojedynczy sygnal: <input type=\"text\" id=\"counter_litres_input\"><button type=\"button\" onclick=\"sendLitresAmount()\">Wyslij</button></p>\n<p>Sredni pobor wody: <b>";
const PROGMEM char prgstrPageMiddle[] = "</b> litrow / sekunde.</p>\n</div>\n</div>\n<script>\nvar config = {\ntype: \"scatter\",\ndata: {\ndatasets: [{\nbackgroundColor: \"#1b5df6\",\nborderColor: \"#1b5df6\",\nshowLine: true,\ndata: [";
const PROGMEM char prgstrPageEnd[] = "],\nfill: false\n}]\n},\noptions: {\nresponsive: true,\nlegend: {\ndisplay: false\n},\nanimation: {\nduration: 0\n},\nresponsiveAnimationDuration: 0,\ntitle:{\ndisplay:true,\ntext:'Wykres przebiegu czasowego'\n},\nscales: {\nxAxes: [{\nticks: {\nreverse: true\n},\nscaleLabel: {\ndisplay: true,\nlabelString: 'czas (sekundy)'\n}\n}],\nyAxes: [{\nscaleLabel: {\ndisplay: true,\nlabelString: 'wartosc (litry)'\n}\n}]\n}\n}\n};\nwindow.onload = function() {\nvar ctx = document.getElementById(\"canvas\").getContext(\"2d\");\nwindow.myLine = new Chart(ctx, config);\n};\n</script>\n</body>\n</html>";

extern DataBuffer dataBuffer;
extern double INCREMENT_VALUE;

char newIncrementValue[maxIncrementValueSize] = {"0"};

void printProgmemString(EthernetClient & client, const char string[]) {
  for (int i = 0; i < strlen_P(string); i++)
    client.write(pgm_read_byte_near(string + i));
}

void show_webpage(EthernetClient & client) {
  printProgmemString(client, prgstrHTTPHeader);
  printProgmemString(client, prgstrPageBegin);
  client.print(dataBuffer.linearRegression());
  printProgmemString(client, prgstrPageMiddle);
  dataBuffer.print(client);
  printProgmemString(client, prgstrPageEnd);
}

void handle_client(EthernetClient & client) {
  if (client) {
    Serial.println("new client");
    int currentIndex=-1;
    boolean isIncValuePostRequest = false;
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        currentIndex++;
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (currentIndex <maxHeaderSize){
           if(c == newIncValueRequest[0]){
              parseHeader(client, currentIndex);
           }
        }
        
        if (c == '\n' && currentLineIsBlank) {
          show_webpage(client);
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

double getValue(EthernetClient & client, int & currentIndex){
  for (int i=0; i<maxIncrementValueSize; i++){
    newIncrementValue[i]='\0';  
  }
  char c;
  int i=0;
  while(true){
    c = client.read();
    Serial.write(c);
    currentIndex++;
    if(i < maxIncrementValueSize-1  && ((c>='0' && c<='9') || c == '.' )){
      newIncrementValue[i]=c;
    }else{
      if(i==0){
        return 1.0;
      }
      break;
    }
    i++;
  }
  return atof(newIncrementValue);
}

void setNewIncValueNumber(EthernetClient & client, int & currentIndex){
  INCREMENT_VALUE = getValue(client,currentIndex);
  Serial.print("New INCREMENT_VALUE = ");Serial.println(INCREMENT_VALUE);
}

void parseHeader(EthernetClient & client, int & currentIndex){
  int patternIndex = 1;
  char c;
  while (patternIndex < newIncValueRequestSize){
    c = client.read();
    Serial.write(c);
    currentIndex++;
    if(c != newIncValueRequest[patternIndex]){
      return;
    }
    patternIndex++;
  }

  setNewIncValueNumber(client, currentIndex);
}

