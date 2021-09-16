
const int trigPin = 5;
const int echoPin = 3;
int EP = A5;
long duration;
int distance;
#include <SoftwareSerial.h>
SoftwareSerial serial_connection(0, 1); //RX=pin 10, TX=pin 11
#include <TinyGPS++.h>

SoftwareSerial mySerial(9, 10);
TinyGPSPlus gps;
//SoftwareSerial mySerial(0,1);
void setup()
{
  pinMode(EP, INPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");
  mySerial.begin(9600); // Setting the baud rate of GSM Module
  delay(5000);
}
void loop() {
  Serial.println("sonar reading");
  sonar();
  Serial.println("sonar alarm");

  Serial.println("vibration");
  vibration();
  Serial.println("vibration alarm");

  delay(200);
  //gps1();
  //delay(200);
}
void gps1() {
  while (serial_connection.available()) //While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if (gps.location.isUpdated()) //This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");
  }
}

long TP_init() {
  delay(10);
  long measurement = pulseIn (EP, HIGH); //wait for the pin to get HIGH and returns measurement
  return measurement;
}
void vibration()
{ long measurement = TP_init();
  delay(50);
  // Serial.print("measurment = ");
  Serial.println(measurement);
  if (measurement > 2000) {
    Serial.println("prithybi vaiga jaitese");
  }
  else {
    Serial.println("gari thik ase... chill");
  }
  vibrationalarm();
  delay(500);
}
void sonar() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  sonaralarm();
  delay(100);
}
void sonaralarm()
{
  if (distance < 12) {
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    // mySerial.println("AT+CMGS=\"+8801517056381\"\r"); // Replace x with mobile number
    mySerial.println("AT+CMGS=\"+8801933275535\"\r"); // Replace x with mobile number
    //mySerial.println("AT+CMGS=\"+8801940951519\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("Your car got crushed. Try to find it through the following location : 23.816796, 90.359583");// The SMS text you want to send
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(100000);
  }
}
void vibrationalarm()
{ long measurement = TP_init();
  delay(50);
  // Serial.print("measurment = ");
  Serial.println(measurement);
  if (measurement > 2000) {
    mySerial.println("AT+CMGF=1");
    Serial.println("prithybi vaiga jaitese");//Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    //mySerial.println("AT+CMGS=\"+8801517056381\"\r"); // Replace x with mobile number
    mySerial.println("AT+CMGS=\"+8801933275535\"\r"); // Replace x with mobile number
    //mySerial.println("AT+CMGS=\"+8801940951519\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("Your car got crushed. Try to find it through the following location :23.816796, 90.359583 ");// The SMS text you want to send
    //mySerial.println("location: 23.816796, 90.359583");
    Serial.println("Your car got crushed. Try to find it through the following location :");//Sets the GSM Module in Text Mode
    Serial.println("location: 23.816796, 90.359583");
    delay(100);
    mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(100000);
  }
  else {
    Serial.println("gari thik ase... chill");
  }
}
