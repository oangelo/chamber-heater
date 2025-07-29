#include <DHT.h>
#include <LiquidCrystal.h>
#include <PID_v1_bc.h>
#include <RotaryEncoder.h>
#include <Pushbutton.h>

// Definições de pinos e constantes
#define FAN_PIN 9        // Ventoinha
#define bed_INPUT_PIN 14 // Termistor mesa
#define bed_OUTPUT_PIN 8 // Mesa
#define RT0 100000       // Ω
#define B 3950           // K
#define VCC 5            // Supply voltage
#define R 4700           // R=10KΩ
#define DHT_PIN 4        // Sensor temperatura
#define DHT_TYPE DHT22   // Sensor temperatura
#define LCD_COLS 20      // Colunas do LCD
#define LCD_ROWS 4       // Linhas do LCD
#define BTN_EN1 31       // Encoder 1
#define BTN_EN2 33       // Encoder 2
#define BTN_ENC 35       // Botão do encoder
#define BUZZER_PIN 37    // Buzzer
#define LCD_RS 16        // LCD RS Pin
#define LCD_EN 17        // LCD EN Pin
#define LCD_D4 23        // LCD D4 Pin
#define LCD_D5 25        // LCD D5 Pin
#define LCD_D6 27        // LCD D6 Pin
#define LCD_D7 29        // LCD D7 Pin

// Definições de variáveis
int row[4] = {0, 1, 2, 3}; // Linhas do LCD

float internal_temp;                         // Temperatura interna (DHT22)
float internal_hum;                          // Umidade interna (DHT22)
float bed_temp;                              // Temperatura da mesa (bed_INPUT_PIN - Termistor)
double bed_set_point, bed_INPUT, bed_OUTPUT; // Variáveis do PID da mesa
double fan_set_point, fan_INPUT, fan_OUTPUT;
const int Kp = 15, Ki = 1, Kd = 15; // Constantes do PID da mesa

RotaryEncoder encoder(BTN_EN1, BTN_EN2, RotaryEncoder::LatchMode::TWO03); // Encoder
Pushbutton button(BTN_ENC);                                               // Botão do encoder
DHT dht(DHT_PIN, DHT_TYPE);                                               // Sensor de temperatura e umidade
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);        // LCD
PID bedPID(&bed_INPUT, &bed_OUTPUT, &bed_set_point, Kp, Ki, Kd, DIRECT);  // PID da mesa
PID fanPID(&fan_INPUT, &fan_OUTPUT, &fan_set_point, Kp, Ki, Kd, REVERSE); // PID da ventoinha

// Retorna a temperatura da mesa
float readBedTemp()
{
  float RT, VR, ln, TX, VRT;
  const float T0 = 20 + 273.15; // K -> °C

  VRT = analogRead(bed_INPUT_PIN); // Lê o valor analógico do pino do termistor

  VRT = (5.00 / 1023.00) * VRT; // Converte o valor lido para tensão

  VR = VCC - VRT; // Calcula a tensão restante (VCC - VRT)

  RT = VRT / (VR / R); // Calcula a resistência do termistor (RT)

  ln = log(RT / RT0); // Calcula o logaritmo natural da razão entre RT e RT0

  TX = (1 / ((ln / B) + (1 / T0))); // Calcula a temperatura em Kelvin usando a equação de Steinhart-Hart

  TX = TX - 273.15; // Converte a temperatura de Kelvin para Celsius
  return TX;        // Retorna a temperatura calculada
}

// Calcula o PID da mesa
void calcBedPID()
{
  bed_INPUT = readBedTemp();               // Lê a temperatura da mesa
  bedPID.Compute();                        // Calcula o PID
  analogWrite(bed_OUTPUT_PIN, bed_OUTPUT); // Ajusta a potência da mesa
}

// Calcula o PID da ventoinha
void calcFanPID()
{
  fan_INPUT = dht.readHumidity();   // Lê a umidade do DHT22
  fanPID.Compute();                 // Calcula o PID
  analogWrite(FAN_PIN, fan_OUTPUT); // Ajusta a potência da ventoinha
}


// Função setup
void setup()
{
  Serial.begin(9600);
  lcd.begin(LCD_COLS, LCD_ROWS); // Inicializa o LCD

  dht.begin(); // Inicializa o sensor DHT

  pinMode(BTN_ENC, bed_INPUT);
  pinMode(bed_OUTPUT_PIN, bed_OUTPUT);
  pinMode(FAN_PIN, bed_OUTPUT);
  pinMode(BUZZER_PIN, bed_OUTPUT);

  bed_INPUT = readBedTemp(); // Lê a temperatura inicial da mesa
  bed_set_point = 130;       // Define a temperatura desejada (bed_set_point)

  bedPID.SetMode(AUTOMATIC); // Ativa o modo automático do PID

  float Umi = dht.readHumidity();
  fan_INPUT = Umi;
  fan_set_point = 10;

  fanPID.SetMode(AUTOMATIC);
  fanPID.SetOutputLimits(0, 125);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.setCursor(0, 0);
  lcd.print("Testando PID");
}

// Função loop
void loop()
{
  calcBedPID();
  // so ativa dps de 50°c
    calcFanPID();
  printInfo();
}
