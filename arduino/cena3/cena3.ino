#include <SPI.h>     // Comunicação SPI
#include <MFRC522.h> // Leitor RFId MFRC522
#include <Wire.h>    // Protocolo I2C

/* -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#define RST_PIN 9 // Arduino UNO: RST 9 / SDA(SS) 10 / MOSI 11 / MISO 12 / SCK 13
#define SS_PIN 10 // Arduino MEGA: RST 5 / SDA(SS) 53 / MOSI 51 / MISO 50 / SCK 52
#define BAU1 A0
#define BAU2 A1
#define LVD A2
#define LVM A3

const int slaveAddress = 0x08;
int x = 0;

String payload = "nivel2";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  pinMode(BAU1, OUTPUT);
  pinMode(BAU2, OUTPUT);
  pinMode(LVD, OUTPUT);
  pinMode(LVM, OUTPUT);

  while (!Serial)
    ;                                // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();                       // Init SPI bus
  mfrc522.PCD_Init();                // Init MFRC522
  delay(1000);                       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  digitalWrite(BAU1, HIGH);
  digitalWrite(BAU2, HIGH);
  digitalWrite(LVD, LOW);
  digitalWrite(LVM, HIGH);
}

bool trava = false;
String strID = "";
// the loop function runs over and over again forever
void loop()
{
  while (trava == false)
  {
    Serial.println("Entrou no Loop da Trava");
    delay(1000);
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

    // if (strID.indexOf("1D:E3:06:30") >= 0){
    if (strID.indexOf("FE:E3:ED:06") >= 0)
    {
      digitalWrite(LVM, LOW);
      digitalWrite(BAU1, LOW); // Desabilitar Trava do Bau
      digitalWrite(BAU2, LOW); // Desabilitar Trava do Bau
      trava = true;
    }

    mfrc522.PICC_HaltA();      // PARADA DA LEITURA DO CARTÃO
    mfrc522.PCD_StopCrypto1(); // PARADA DA CRIPTOGRAFIA NO PCD

    delay(1000);
    if (trava == true)
    {
      Wire.beginTransmission(slaveAddress);
      String payload = "nivel2";
      Wire.write(payload.c_str());          // envia a mensagem
      Wire.endTransmission();               // para de transmitir
      x++;                                  // incremento da variável
      delay(500);                           // pausa de 500 milissegundos
      Serial.println("FIM LOOP DA TRAVA!"); // IMPRIME O TEXTO NA SERIAL
      Serial.println("enviado nivel2");     // IMPRIME O TEXTO NA SERIAL
      break;
    }
  }
  digitalWrite(LVD, HIGH);
  delay(500);
  digitalWrite(LVD, LOW);
  delay(500);
}
