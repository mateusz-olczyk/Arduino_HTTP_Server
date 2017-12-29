#include <UIPEthernet.h>
#include "Interrupts.h"
#include "DataBuffer.h"
DataBuffer dataBuffer;
double INCREMENT_VALUE = 1.0;
double currentValue = 0.0;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(10, 10, 10, 1);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

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
  // Store button events
  while (!intBuffer.empty()) {
    double seconds = (double) intBuffer.get() / 1000;
    currentValue += INCREMENT_VALUE;
    dataBuffer.add(DataObject(seconds, currentValue));
  }

  // listen for incoming clients
  EthernetClient client = server.available();
  handle_client(client);
}

