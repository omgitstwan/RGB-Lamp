// RGB_Led_via_Bluetoot en_app_op_Android_Smartphone
// https://www.instructables.com/id/Android-Controlled-RGB-LED-Using-Arduino/
// App controller: https://play.google.com/store/apps/details?id=appinventor.ai_yuanryan_chen.BT_LED

#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial BT(7, 6); // RX is pin 6, connect to TX of HC-05 BT device, TX is pin 7, connect to RX of HC-05 BT device)

int RedLed = 5;
int GreenLed = 3;
int BlueLed = 9;

int R = 254;
int G = 1;
int B = 127;

int rDir = -1;
int gDir = 1;
int bDir = -1;

int Control_LED = 13;
String RGB = ""; //store RGB code from BT
String RGB_Previous = "255.255.255)"; //preserve previous RGB color for LED switch on/off, default White
String ON = "ON"; //Check if ON command is received
String OFF = "OFF"; //Check if OFF command is received
String FADE = "FADE";
String FIRE = "FIRE";
boolean RGB_Completed = false;

void setup() {
  Serial.begin(9600); //Arduino serial port baud rate：9600
  BT.begin(9600);     //HC-05 module default baud rate is 9600
  RGB.reserve(30); // Maakt een ruimte van 30 characters in de string RGB
  pinMode(Control_LED, OUTPUT); //Set pin13 as output for LED,
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(BlueLed, OUTPUT);
}

void loop() {
  //Read each character from Serial Port(Bluetooth)
  while (BT.available()) {
    char ReadChar = (char)BT.read();
    if (ReadChar == ')') RGB_Completed = true;  // Right parentheses ) indicates string is complete
    else RGB += ReadChar;
  }

  while (RGB == FADE) {
    analogWrite(RedLed,R);
    analogWrite(GreenLed, G);
    analogWrite(BlueLed, B);

  // change the values of the LEDs
    R = R + rDir;
    G = G + gDir;
    B = B + bDir;

  // for each color, change direction if
  // you reached 0 or 255
    if (R >= 255 || R <= 0) {
    rDir = rDir * -1;
  }

    if (G >= 255 || G <= 0) {
    gDir = gDir * -1;
  }

    if (B >= 255 || B <= 0) {
    bDir = bDir * -1;
  }

  // slight delay so it doesn't rotate color too quicky
  delay(33);

  while (BT.available()) {
    char ReadChar = (char)BT.read();
    if (ReadChar == ')') RGB_Completed = true;  // Right parentheses ) indicates string is complete
    else RGB += ReadChar;
  }
  }
  while (RGB == FIRE) {
    analogWrite(RedLed,R);
    analogWrite(GreenLed, G);
    analogWrite(BlueLed, B);

  // change the values of the LEDs
    R = random(200,255);
    G = random(50,100);
    B = 0;
  delay(150);

  while (BT.available()) {
    char ReadChar = (char)BT.read();
    if (ReadChar == ')') RGB_Completed = true;  // Right parentheses ) indicates string is complete
    else RGB += ReadChar;
  }
  }

  

  //When a command code is received completely with ')' ending character
  if (RGB_Completed) {
    //Print out debug info at Serial output window
    Serial.print("RGB received:");
    Serial.print(RGB);
    Serial.print("     Previous RGB:");
    Serial.println(RGB_Previous);

    if (RGB == ON) {
      digitalWrite(13, HIGH);
      RGB = RGB_Previous; //We only receive 'ON', so get previous RGB color back to turn LED on
      Light_RGB_LED();

    } else if (RGB == OFF) {
      digitalWrite(13, LOW);
      RGB = "0.0.0)"; //Send OFF string to turn light off
      Light_RGB_LED();
    } else {
      //Turn the color according the color code from Bluetooth Serial Port
      Light_RGB_LED();
      RGB_Previous = RGB;
    }
    //Reset RGB String
    RGB = "";
    RGB_Completed = false;
  } //end if of check if RGB completed

} // end of loop


void Light_RGB_LED() {
  int SP1 = RGB.indexOf('.');
  int SP2 = RGB.indexOf('.', SP1 + 1);
  int SP3 = RGB.indexOf('.', SP2 + 1);
  int SP4 = RGB.indexOf('.', SP3 + 1); //Helderheid
  String R = RGB.substring(0, SP1);
  String G = RGB.substring(SP1 + 1, SP2);
  String B = RGB.substring(SP2 + 1, SP3);
  String H = RGB.substring(SP3 + 1, SP4); //Helderheid
  
  //Print out debug info at Serial output window
  Serial.print("R=");
  Serial.println( constrain(R.toInt(), 0, 255));
  Serial.print("G=");
  Serial.println(constrain(G.toInt(), 0, 255));
  Serial.print("B=");
  Serial.println( constrain(B.toInt(), 0, 255));
  Serial.print("Brightnes=");
  Serial.println( constrain(H.toInt(), 0, 100));
  analogWrite(RedLed,  (R.toInt()*H.toInt()/100));
  analogWrite(GreenLed, (G.toInt()*H.toInt()/100));
  analogWrite(BlueLed,  (B.toInt()*H.toInt()/100));
}
