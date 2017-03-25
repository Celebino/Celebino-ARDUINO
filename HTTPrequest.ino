
#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server{192,168,1,9};   

IPAddress ip(192, 168, 1, 177);

EthernetClient client;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    Ethernet.begin(mac, ip);
  }
  delay(1000);
  Serial.println("connecting...");

  if (client.connect(server, 8080)) {
    Serial.println("connected");
    String PostData = sensor();
    client.println("POST /primeiro/primeiro HTTP/1.1");
    client.println("Host: 192.168.1.9");
    client.println("User-Agent: Arduino"); 
    client.println("Content-Type: application/json"); 
    client.println("Connection: Close");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);
    client.println();
    client.stop();                           
    Serial.println("Client has closed");        
  } else {
    Serial.println("connection failed");
  }
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    while (true);
  }
}

String sensor(){
  int umidade = analogRead(A0);
  umidade = map(umidade,0,1023,100,0); 
  String json = "{\"umidade\":\"" + String(umidade) + "\"}";
  return json;     
}


