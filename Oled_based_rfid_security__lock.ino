#include <MFRC522.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>
#include <SPI.h>


#define OLED_RESET 4
//Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SH1106 display(OLED_RESET);

#define SS_PIN 10
#define RST_PIN 9
#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
 
MFRC522 rfid(SS_PIN, RST_PIN); 

MFRC522::MIFARE_Key key; 

int code[] = {131,35,56,187};  //Enter your UID 
int codeRead = 0;
String uidString;
void setup() {
  
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  
//  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE); // or BLACK);
  display.setTextSize(2);
  display.setCursor(10,0); 
  display.print("SCAN RFID");
  display.display();
  
}

void loop() {
  if(  rfid.PICC_IsNewCardPresent())
  {
      readRFID();
  }
  delay(100);

}

void readRFID()
{
  
  rfid.PICC_ReadCardSerial();
  Serial.print(F("\nPICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

    clearUID();
   
    Serial.println("Scanned PICC's UID:");
    printDec(rfid.uid.uidByte, rfid.uid.size);

    uidString = String(rfid.uid.uidByte[0])+" "+String(rfid.uid.uidByte[1])+" "+String(rfid.uid.uidByte[2])+ " "+String(rfid.uid.uidByte[3]);
    
    printUID();

    int i = 0;
    boolean match = true;
    while(i<rfid.uid.size)
    {
      if(!(rfid.uid.uidByte[i] == code[i]))
      {
           match = false;
      }
      i++;
    }

    if(match)
    {
      Serial.println("\nI know this card!");
      printUnlockMessage();
    }else
    {
      Serial.println("\nUnknown Card");
    }


    // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

  void clearUID()
  {
    display.setTextColor(BLACK); // or BLACK);
    display.setTextSize(1);
    display.setCursor(30,20); 
    display.print(uidString);
    display.display();
  }

  void printUID()
  {
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(1);
    display.setCursor(0,20); 
    display.print("UID: ");
    display.setCursor(30,20); 
    display.print(uidString);
    display.display();
  }

  void printUnlockMessage()
  {
    display.display();
    display.setTextColor(BLACK); // or BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("SCAN RFID");
    display.display();
    
    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("UnLocked");
    display.display();
    
    delay(2000);
    
    display.setTextColor(BLACK); // or BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("UnLocked");

    display.setTextColor(WHITE); // or BLACK);
    display.setTextSize(2);
    display.setCursor(10,0); 
    display.print("SCAN RFID");
    display.display();
  }
