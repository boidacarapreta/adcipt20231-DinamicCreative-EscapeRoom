#include <NewPing.h> // Sensor Ultrassônico HC-SR04
#include <SPI.h>     // Comunicação SPI
#include <MFRC522.h> // Leitor RFId

#define TRIGGER_PIN A0
#define ECHO_PIN A1
#define MAX_DISTANCE 200
#define L1 7
#define L2 6
#define L3 5
#define L4 4
#define L5 3
#define L6 2

#define RST_PIN 9 // Configurable, see typical pin layout above
#define SS_PIN 10 // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void ritual()
{
  for (int i = 0; i < 10; i++)
  {
    digitalWrite(L1, LOW);
    digitalWrite(L2, LOW);
    digitalWrite(L3, LOW);
    digitalWrite(L4, LOW);
    digitalWrite(L5, LOW);
    digitalWrite(L6, LOW);
    delay(150);
    digitalWrite(L1, HIGH);
    digitalWrite(L2, HIGH);
    digitalWrite(L3, HIGH);
    digitalWrite(L4, HIGH);
    digitalWrite(L5, HIGH);
    digitalWrite(L6, HIGH);
    delay(150);
  }
}

void setup()
{
  Serial.begin(9600);

  pinMode(L1, OUTPUT);
  pinMode(L2, OUTPUT);
  pinMode(L3, OUTPUT);
  pinMode(L4, OUTPUT);
  pinMode(L5, OUTPUT);
  pinMode(L6, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  while (!Serial)
    ;                                // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522
  delay(1000);                       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

bool controle = false;
bool rito1 = false;
bool rito2 = false;
bool rito3 = false;
bool rito4 = false;
bool rito5 = false;
bool rito6 = false;
float distancia = 200;
String strID = "";

// the loop function runs over and over again forever
void loop()
{
  while (controle == false)
  {
    Serial.println("Entrou no LOOP do ultrasonico");
    distancia = sonar.ping_cm(); // Realiza a captura da distancia em [cm].
    Serial.println(distancia);   // Imprime na serial o valor da variável distancia em [cm].
    delay(500);
    if (distancia < 31)
    {
      distancia = sonar.ping_cm(); // Realiza a captura da distancia em [cm].
      Serial.println(distancia);   // Imprime na serial o valor da variável distancia em [cm].
      delay(500);
    }
    else
    {
      Serial.println("nivel4");
      ritual();
      controle = true;
      break;
    }
  }

  while (rito1 == false)
  {
    Serial.println("Entrou no LOOP do 1o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("96:9B:EF:48") >= 0)
    {
      digitalWrite(L2, LOW); // Desabilitar Trava do Bau
      rito1 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito1 == true)
    {
      break;
    }
  }

  while (rito2 == false)
  {
    Serial.println("Entrou no LOOP do 2o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("06:78:AD:D5") >= 0)
    {
      digitalWrite(L1, LOW); // Desabilitar Trava do Bau
      rito2 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito2 == true)
    {
      break;
    }
  }

  while (rito3 == false)
  {
    Serial.println("Entrou no LOOP do 3o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("3B:E9:03:C5") >= 0)
    {
      digitalWrite(L3, LOW); // Desabilitar Trava do Bau
      rito3 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito3 == true)
    {
      break;
    }
  }

  while (rito4 == false)
  {
    Serial.println("Entrou no LOOP do 4o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("C6:A5:49:7B") >= 0)
    {
      digitalWrite(L6, LOW); // Desabilitar Trava do Bau
      rito4 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito4 == true)
    {
      break;
    }
  }

  while (rito5 == false)
  {
    Serial.println("Entrou no LOOP do 5o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("52:63:93:E5") >= 0)
    {
      digitalWrite(L5, LOW); // Desabilitar Trava do Bau
      rito5 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito5 == true)
    {
      break;
    }
  }

  while (rito6 == false)
  {
    Serial.println("Entrou no LOOP do 6o ritual");
    if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    {
      return; // RETORNA PARA LER NOVAMENTE
    }

    String strID = "";
    for (byte i = 0; i < 4; i++)
    {
      strID +=
          (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "") +
          String(mfrc522.uid.uidByte[i], HEX) +
          (i != 3 ? ":" : "");
    }
    strID.toUpperCase();

    Serial.print("Identificador (UID) da tag: "); // IMPRIME O TEXTO NA SERIAL
    Serial.println(strID);                        // IMPRIME NA SERIAL O UID DA TAG RFID

    if (strID.indexOf("B3:0A:9F:1B") >= 0)
    {
      digitalWrite(L4, LOW); // Desabilitar Trava do Bau
      rito6 = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (rito6 == true)
    {
      Serial.println("FIM - LOOP DO RITUAL"); // IMPRIME O TEXTO NA SERIAL
      Serial.println("nivel5");               // IMPRIME O TEXTO NA SERIAL
      break;
    }
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}
