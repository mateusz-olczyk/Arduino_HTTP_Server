/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi

 */

#include <SPI.h>
#include <Ethernet.h>
#include <String.h> 
#include "Interrupts.h"

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
int var = 0;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 176);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);
String HTTP_req; 

void setup() {
  // Init interrupts
  interruptsInit();
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // Check buffer
  while (!intBuffer.empty()) {
    intBuffer.get();
    var++;
    //var = (int)//intBuffer.get();
  }
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        HTTP_req+=c;
        if (c == '\n' && currentLineIsBlank) {
         
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: keep-alive");
          client.println();
//          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          if (HTTP_req.indexOf("BT") > -1) {
              // read switch state and send appropriate paragraph text
              GetSwitchState(client);
          }else{
          client.println("<!DOCTYPE HTML>");
          
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Miernik zużycia wody</title>");
          client.println("<script>");
          client.println("function getContent(elementId, headerId) {");
          client.println("header =\"&nocache\";");
          client.println("var request = new XMLHttpRequest();");
//             client.println("var counter_litres = document.getElementById(\"counter_litres_input\").value;");
          client.println("request.onreadystatechange = function (){");
          client.println("if (this.readyState == 4){");
          client.println("if (this.status == 200){");
          client.println("if (this.responseText != null){");
          client.println("document.getElementById(elementId).innerHTML = this.responseText;}}}}");

          client.println("request.open(\"GET\", headerId + header, true);");
          client.println("request.send(null);}");
          client.println("</script>");
          client.println("</head>");
          client.println("<body bgcolor=\"#2e5b85\">");
          client.println("<p id=\"counter_var\">Counter var=</p>");
          client.println("<p>Liczba litrow na pojedynczy sygnal: <input type=\"text\" id=\"counter_litres_input\" value=\"Test\"></p>");
          client.println("<button type=\"button\" onclick=\"getContent('counter_var', 'BT')\">Odswierz ilosc wody</button>");
          client.println("</body>");
          client.println("</html>");

          }
          Serial.print(HTTP_req);
          HTTP_req = "";            // finished with request, empty string
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

void GetSwitchState(EthernetClient cl)
{
    //var++;
    cl.print("Water counter = ");
    cl.println(var);

    
}
