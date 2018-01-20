/*
  Repeating Web client

 This sketch connects to a a web server and makes a request
 using a Wiznet Ethernet shield. You can use the Arduino Ethernet shield, or
 the Adafruit Ethernet shield, either one will work, as long as it's got
 a Wiznet Ethernet module on board.

 This example uses DNS, by assigning the Ethernet client with a MAC address,
 IP address, and DNS address.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 19 Apr 2012
 by Tom Igoe
 modified 21 Jan 2014
 by Federico Vanzati

 http://www.arduino.cc/en/Tutorial/WebClientRepeating
 This code is in the public domain.

 */

#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = {
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x028
};
// fill in an available IP address on your network here,
// for manual configuration:
IPAddress ip(192, 168, 0, 28);

// fill in your Domain Name Server address here:
IPAddress myDns(1, 1, 1, 1);

// initialize the library instance:
EthernetClient client;

char server[] = "192.168.0.4";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10L * 1000L; // delay between updates, in milliseconds
// the "L" is needed to use long type numbers
int s,w,d,p,l,pingPin=4,inPin=2,power=7;
long duration,inches,cm,microseconds;
String data="";
void setup() {
  // start serial port:
  Serial.begin(9600);
  pinMode(power,INPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac, ip, myDns);
  // print the Ethernet board/shield's IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // if there's incoming data from the net connection.
  // send it out the serial port.  This is for debugging
  // purposes only:
  if (client.available()) {
    char c = client.read();
    data+=c;
   //Serial.write(c);
   //if(data.endsWith("Content-Type: text/html; charset=UTF-8")){
  //data="";}
  }
  //Serial.println(data);
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();
  if(data.indexOf('&')!=-1)
  {
  data=data.substring(data.indexOf('&')+1);
  int smax=data.substring(0,data.indexOf('&')).toInt();
  data=data.substring(data.indexOf('&')+1);
  int wmin=data.substring(0,data.indexOf('&')).toInt();
  data=data.substring(data.indexOf('&')+1);
  int wmax=data.substring(0,data.indexOf('&')).toInt();
  data=data.substring(data.indexOf('&')+1);
  int dr=data.substring(0,data.indexOf('&')).toInt(); 

  Serial.println(smax);
  Serial.println(wmin);
  Serial.println(wmax);
  Serial.println(dr);
  }
  
  s=analogRead(1);
  w=analogRead(2);
  d=ultrasonic();
  if(digitalRead(power)==HIGH){
    p=1;
  }
  else{
    p=0;
  }
  l=analogRead(0);
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connected");
    // send the HTTP GET request:
    client.print( "GET /IoT/add.php?");
    client.print("s=");
    client.print( s );
    client.print("&w=");
    client.print( w );
    client.print("&d=");
    client.print( d );
    client.print("&p=");
    client.print( p );
    client.print("&l=");
    client.print( l );
    client.println( " HTTP/1.1");
    client.print( "Host: " );
    client.println("192.168.0.4");
    client.println( "Connection: close" );
    client.println();
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
  data="";
}
int ultrasonic(){
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
digitalWrite(pingPin, HIGH);
delayMicroseconds(10);
digitalWrite(pingPin, LOW);
delayMicroseconds(2);
pinMode(pingPin, OUTPUT);
pinMode(inPin, INPUT); 
duration = pulseIn(inPin, HIGH);
cm = microsecondsToCentimeters(duration);
return cm;
}
long microsecondsToCentimeters(long microseconds) { 
  return microseconds / 29 / 2;
}
