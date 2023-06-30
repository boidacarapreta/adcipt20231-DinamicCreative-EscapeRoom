// BIBLIOTECAS
#include <LiquidCrystal.h> // Display LCD
#include <Keypad.h>        // Teclado Matricial
#include <Password.h>      // Validação de Senha
#include <NewPing.h>       // Sensor Ultrassônico HC-SR04
#include <Ethernet.h>
#include <PubSubClient.h>

// DEFINIÇÕES SENSOR HC-SR04: >> VCC (+5V) / GND (0V) / ECHO (digital) / TRIG (digital)
#define TRIGGER_PIN A8
#define ECHO_PIN A9
#define MAX_DISTANCE 200
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// DEFINIÇÕES DE REDE/MQTT
byte mac[] = {0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xEF};
IPAddress broker(191, 36, 8, 44);
EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("arduinoClient_porta"))
    {
      Serial.println("connected");
      // client.subscribe("adcipt20231/escape-room");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// DEFINIÇÕES PONTE H / MOTOR DC
int IN3 = 36;
int IN4 = 38;
int vel = 34;

// DEFINIÇÕES DO DISPLAY: >> EN, RS, DB4, DB5, DB6, DB7 >> PSB (HIGH) >> R/W (LOW)
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

// CONFIGURAÇÃO DO TECLADO MATRICIAL [4 x 3]
const byte LINHAS = 4;
const byte COLUNAS = 3;
const char TECLAS_MATRIZ[LINHAS][COLUNAS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
const byte PINOS_LINHAS[LINHAS] = {14, 15, 16, 17}; // Pinos de Conexão (linhas)
const byte PINOS_COLUNAS[COLUNAS] = {18, 19, 20};   // Pinos de Conexão (colunas)

Keypad teclado = Keypad(makeKeymap(TECLAS_MATRIZ), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS);

// DEFINIÇÃO DA SENHA DE ACESSO
Password senha = Password("160623"); // Senha utilizada para liberacao

// MÉTODO PARA IMPRIMIR LINHA NO DISPLAY
void LCD_Linha(int a, int b, String texto)
{
  lcd.setCursor(a, b); // Posiciona o cursor na Coluna a x Linha b >> L1(0,0), L2(0,2), L3(8,0), L4(8,2)
  lcd.print(texto);    // Imprime o texto na posição (a,b) do LCD >> Tamanho máximo: 16 caracteres/linha
}

// MONTAGEM DAS TELAS
void LCD_Espera()
{
  lcd.clear();
  lcd.setCursor(0, 0); // Posiciona o cursor na Coluna a x Linha b >> L1(0,0), L2(0,2), L3(8,0), L4(8,2)
  char x = 0x08;
  for (int i = 0; i < 32; i++)
  {
    lcd.write(x); // Imprime o texto na posição (a,b) do LCD >> Tamanho máximo: 16 caracteres/linha
  }
  lcd.setCursor(0, 2); // Posiciona o cursor na Coluna a x Linha b >> L1(0,0), L2(0,2), L3(8,0), L4(8,2)
  for (int i = 0; i < 32; i++)
  {
    lcd.write(x); // Imprime o texto na posição (a,b) do LCD >> Tamanho máximo: 16 caracteres/linha
  }
}

void LCD_Inicial()
{
  lcd.clear();
  LCD_Linha(0, 0, "> PORTA FECHADA!");
  LCD_Linha(0, 2, "Digite o codigo");
  LCD_Linha(8, 0, "6 digitos + '#'");
  LCD_Linha(8, 2, "Codigo: ");
  delay(100);
  lcd.blink();
}

void LCD_Validacao()
{
  lcd.noBlink();
  lcd.clear();
  LCD_Linha(0, 0, "Codigo Validado!");
  LCD_Linha(0, 2, "Liberando acesso");
}

void LCD_Invalido()
{
  lcd.noBlink();
  lcd.clear();
  LCD_Linha(0, 0, "****************");
  LCD_Linha(0, 2, "CODIGO INVALIDO!");
  LCD_Linha(8, 0, "Tente novamente!");
  LCD_Linha(8, 2, "****************");
}

void LCD_Final()
{
  lcd.clear();
  LCD_Linha(0, 0, "================");
  LCD_Linha(0, 2, "=Porta Liberada=");
  LCD_Linha(8, 0, "= Bem-Vindo(a) =");
  LCD_Linha(8, 2, "================");
}

void ALERTAS()
{
  int BUZZER = 1;
  int LED = 21;
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
  digitalWrite(BUZZER, LOW);
}

void setup()
{
  Serial.begin(9600);
  client.setServer(broker, 1883);
  Ethernet.begin(mac);
  Serial.println(Ethernet.localIP());

  // Alertas de Acionamento de Tecla

  int RED = 8;
  int BUZZER = 1;
  int LED = 21;
  pinMode(BUZZER, OUTPUT); // Alerta sonoro
  pinMode(LED, OUTPUT);    // Alerta visual
  pinMode(RED, OUTPUT);    // Alerta visual
  // Inicialização do Display
  lcd.begin(16, 4); // Configura o tamanho do display
  LCD_Espera();     // Tela inicial solicitando código
}

String codigo = "Codigo: ";

int LED = 21;
int RED = 8;
bool controle = false;
void loop()
{
  if (!client.connected())
  {
    Serial.println("reconnecting...");
    reconnect();
  }
  else
  {
    while (controle == false)
    {
      digitalWrite(LED, HIGH);
      digitalWrite(RED, HIGH);
      float distancia = sonar.ping_cm(); // Realiza a captura da distancia em [cm].
      Serial.println(distancia);         // Imprime na serial o valor da variável distancia em [cm].
      delay(500);
      if (distancia < 60)
      {
        controle = true;
        digitalWrite(LED, LOW);
        Serial.println("STOP");
        client.publish((const char *)"adcipt20231/escape-room", (const char *)"nivel0");
        LCD_Inicial(); // Tela inicial solicitando código
        break;
      }
    }

    char leitura = teclado.getKey(); // Atribui a variavel a leitura do teclado
    if (leitura)
    { // Se alguma tecla foi pressionada
      ALERTAS();
      digitalWrite(RED, HIGH);
      if (leitura == '#')
      {
        if (senha.evaluate())
        {
          LCD_Validacao();
          // client.publish((const char*)"nivel2",porta_aberta_str);
          for (int i = 5; i > 0; i--)
          {
            String texto = "em: " + String(i) + " segundo(s)";
            LCD_Linha(8, 0, texto);
            delay(1000);
          }
          for (int i = 0; i < 20; i++)
          {
            lcd.clear();
            delay(100);
            LCD_Linha(8, 0, "ACESSO LIBERADO!");
            delay(100);
          }
          digitalWrite(IN3, HIGH);
          digitalWrite(IN4, LOW);
          analogWrite(vel, 220);
          delay(1250);
          Serial.println("PORTA ABERTA");
          client.publish((const char *)"adcipt20231/escape-room", (const char *)"nivel1");
          digitalWrite(IN3, LOW);
          digitalWrite(IN4, LOW);
          delay(3000);
          LCD_Final();
        }
        else
        {
          LCD_Invalido();
          senha.reset();
          codigo = String("Codigo: ");
          delay(1000);
          LCD_Inicial();
        }
      }
      else
      {
        if (leitura != '*')
        {
          senha.append(leitura);
          codigo = codigo + '*';
          LCD_Linha(8, 2, codigo);
        }
      }
      if (leitura == '*')
      {
        lcd.noBlink();
        LCD_Linha(8, 2, "Codigo:         ");
        LCD_Linha(8, 2, "Codigo: ");
        lcd.blink(); // Cursor piscando no final do texto
        senha.reset();
        codigo = String("Codigo: ");
      }
    }
    client.loop();
    Ethernet.maintain();
  }
}
