const PROGMEM char prgstrHTTPHeader[] = "HTTP/1.1 200 OK\nContent-Type: text/html\nConnection: close\nRefresh: 5\n\n";
const PROGMEM char prgstrPageBegin[] = "<!doctype html>\n<html>\n<head>\n<title>Scatter Chart</title>\n<script src=\"http://www.chartjs.org/dist/2.7.1/Chart.bundle.js\"></script>\n<style>\ncanvas{\n-moz-user-select: none;\n-webkit-user-select: none;\n-ms-user-select: none;\n}\ndiv{\nwidth: 50%;\nmargin: 0 auto;\n}\n</style>\n</head>\n<body>\n<div style=\"width:75%;\">\n<canvas id=\"canvas\"></canvas>\n</div>\n<script>\nvar config = {\ntype: \"scatter\",\ndata: {\ndatasets: [{\nbackgroundColor: \"#1b5df6\",\nborderColor: \"#1b5df6\",\nshowLine: true,\ndata: [";
const PROGMEM char prgstrPageEnd[] = "],\nfill: false\n}]\n},\noptions: {\nresponsive: true,\nlegend: {\ndisplay: false\n},\nanimation: {\nduration: 0\n},\nresponsiveAnimationDuration: 0,\ntitle:{\ndisplay:true,\ntext:'Chart.js Scatter Chart with lines'\n},\nscales: {\nxAxes: [{\nscaleLabel: {\ndisplay: true,\nlabelString: 'seconds'\n}\n}],\nyAxes: [{\nscaleLabel: {\ndisplay: true,\nlabelString: 'value'\n}\n}]\n}\n}\n};\nwindow.onload = function() {\nvar ctx = document.getElementById(\"canvas\").getContext(\"2d\");\nwindow.myLine = new Chart(ctx, config);\n};\n</script>\n</body>\n</html>";

//const PROGMEM char progmem_string[] = "<!doctype html>\n<html>\n<head>\n<title>Scatter Chart</title>\n<script src=\"http://www.chartjs.org/dist/2.7.1/Chart.bundle.js\"></script>\n<style>\ncanvas{\n-moz-user-select: none;\n-webkit-user-select: none;\n-ms-user-select: none;\n}\ndiv{\nwidth: 50%;\nmargin: 0 auto;\n}\n</style>\n</head>\n<body>\n<div style=\"width:75%;\">\n<canvas id=\"canvas\"></canvas>\n</div>\n<script>\nvar config = {\ntype: \"scatter\",\ndata: {\ndatasets: [{\nbackgroundColor: \"#1b5df6\",\nborderColor: \"#1b5df6\",\nshowLine: true,\ndata: [{\nx: 0.7,\ny: 5.6\n}, {\nx: 5.7,\ny: 10.7\n}, {\nx: 8.7,\ny: 5.7\n}, {\nx: 15.7,\ny: 0.7\n}],\nfill: false\n}]\n},\noptions: {\nresponsive: true,\nlegend: {\ndisplay: false\n},\nanimation: {\nduration: 0\n},\nresponsiveAnimationDuration: 0,\ntitle:{\ndisplay:true,\ntext:'Chart.js Scatter Chart with lines'\n},\nscales: {\nxAxes: [{\nscaleLabel: {\ndisplay: true,\nlabelString: 'x axis'\n}\n}],\nyAxes: [{\nscaleLabel: {\ndisplay: true,\nlabelString: 'y axis'\n}\n}]\n}\n}\n};\nwindow.onload = function() {\nvar ctx = document.getElementById(\"canvas\").getContext(\"2d\");\nwindow.myLine = new Chart(ctx, config);\n};\n</script>\n</body>\n</html>";
extern DataBuffer dataBuffer;

void printProgmemString(EthernetClient & client, const PROGMEM char string[]) {
  for (int i = 0; i < strlen_P(string); i++)
    client.write(pgm_read_byte_near(string + i));
}

void show_webpage(EthernetClient & client) {
  printProgmemString(client, prgstrHTTPHeader);
  printProgmemString(client, prgstrPageBegin);
  dataBuffer.print(client);
  printProgmemString(client, prgstrPageEnd);
}

void handle_client(EthernetClient & client) {
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
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

