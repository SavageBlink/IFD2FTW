#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          53         // Configurable, see typical pin layout above

#define OPEN_PIN         2          // Configurable, see typical pin layout above
#define CLOSE_PIN        3


#define ESP_SERIAL_TIMEOUT 100
#define SERIAL_SPEED 9600

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

int ID = -1;

void SmbgotIn(){
  String SQLQ = "[INSERT INTO Logs (User_ID) VALUES ((SELECT ID from Users WHERE hexa =\"";
  Serial3.print(SQLQ);  
}

void OpenDoor(String DBr){
  if (strlen(DBr.c_str()) > 2){
    digitalWrite(OPEN_PIN,HIGH);
    delay(200);
    digitalWrite(OPEN_PIN,LOW);
    delay(200);
    digitalWrite(CLOSE_PIN,HIGH);
    delay(200);
    digitalWrite(OPEN_PIN,LOW);
    SmbgotIn();
    String temp="\";]";
    String buffer;
    char* parser = strcpy(parser,DBr.c_str());
    int i=0;
    while (parser[i]!=':'){
      i++;
      Serial.print(parser[i]);
    }
    i++;
    while(parser[i]!='}'){
      buffer += parser[i];
      Serial.print(parser[i]);
      i++; 
    }
    Serial.print(buffer);
    Serial3.print(buffer);
    Serial3.print(temp);
  }
}





void SendSQLRFID(){
  String SQLQ="[SELECT ID FROM Users WHERE Hexa=\"";
  Serial3.print(SQLQ);
}

String process_serial3_events() {
  String buffer;
  unsigned long begin = millis();
  while (Serial3.available() && millis() - begin < ESP_SERIAL_TIMEOUT) {
    char c = Serial3.read();
    // Serial.print(c);
    buffer += c;
    if (c == ']') {
      Serial.print(buffer);
      Serial.print("\n");
      return buffer;
      // handle command
      buffer = "";
      Serial.print("\n");
    }
  }
}


void printHex(byte *buffer, byte bufferSize) {
  SendSQLRFID();
  String END="\";]";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX); //Serial3.print for esp
    Serial3.print(buffer[i], HEX);
  }Serial3.print(END);
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}


void setup() { 
  Serial.begin(SERIAL_SPEED);
  Serial3.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  pinMode(OPEN_PIN, OUTPUT);
  pinMode(CLOSE_PIN, OUTPUT);
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  Serial.print(F("Using the following key:\n"));
}
 
void loop() {
  String Dbr = process_serial3_events();
  OpenDoor(Dbr);
  //int I = getDataSQLfromESP();
  //Serial.print(I);
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  
  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    Serial.println(F("A new card has been detected."));

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
   
    Serial.println(F("The NUID tag is:"));
    Serial.print(F("In hex: "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    Serial.print(F("In dec: "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
  }
  else Serial.println(F("Card read previously."));

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}




