#include <DHT.h>
#include <LiquidCrystal.h>
#include <PID_v1.h>
#include <RotaryEncoder.h>
#include <Pushbutton.h>

// Constants
#define PIN_FAN 9
#define PIN_INPUT 14
#define PIN_OUTPUT 8
#define RT0 100000 // Ω
#define B 3950     // K
#define resist 8
#define VCC 5  // Supply voltage
#define R 4700 // R=10KΩ

// Constants LCD, Encoder and DHT22
#define DHT_PIN 4
#define DHT_TYPE DHT22
#define LCD_COLS 20
#define LCD_ROWS 4
#define BTN_EN1 31
#define BTN_EN2 33
#define BTN_ENC 35

RotaryEncoder encoder(BTN_EN1, BTN_EN2, RotaryEncoder::LatchMode::TWO03);
Pushbutton button(BTN_ENC);

// Initialize the DHT sensor
// instanciação de controle de temparatura e humidade
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize the LCD object
const int rs = 16, en = 17, d4 = 23, d5 = 25, d6 = 27, d7 = 29; // pinos relacionados com LCD
const int buzzer = 37;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int linha[4] = {0, 1, 2, 3};

unsigned long lastMillis = millis();

// bitmap encodado como
byte bitmapCelsius[8] =
{
  0b11000,
  0b11000,
  0b00000,
  0b00110,
  0b01001,
  0b01000,
  0b01001,
  0b00110
};
byte bitmapSeta1[8] =
{
  0b00000,
  0b00000,
  0b00000,
  0b01111,
  0b01111,
  0b00000,
  0b00000,
  0b00000
};
byte bitmapSeta2[8] =
{
  0b10000,
  0b11000,
  0b11100,
  0b11110,
  0b11110,
  0b11100,
  0b11000,
  0b10000
};

float temperaturaInterna;
float umidadeInterna;
float mesaTemperatura;
int FANTEMP = 35;
int FANMAX = 25;
int FANMIN = 20;

const float T0 = 20 + 273.15;
float RT, VR, ln, TX, VRT;

double Setpoint, Input, Output;

// Specify the links and initial tuning parameters
double Kp = 15, Ki = 1, Kd = 15;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

double readTemperature() // Temperatura da mesa
{
  VRT = analogRead(PIN_INPUT);  // Acquisition analog value of VRT
  VRT = (5.00 / 1023.00) * VRT; // Conversion to voltage
  VR = VCC - VRT;
  RT = VRT / (VR / R); // Resistance of RT

  ln = log(RT / RT0);
  TX = (1 / ((ln / B) + (1 / T0))); // Temperature from thermistor

  TX = TX - 273.15; // Conversion to Celsius
  return TX;
}

void lcdprint()
{
  if (lastMillis < millis() - 1000)
  {
    lcd.setCursor(0, 0);
    lcd.print("Temp. Geral: ");
    lcd.print(temperaturaInterna);
    lcd.print(" ");
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.setCursor(0, 1);
    lcd.print("Temp. Mesa: ");
    lcd.print(mesaTemperatura);
    lcd.print(" ");
    lcd.write(byte(0));
    lcd.print(" ");
    lcd.setCursor(0, 2);
    lcd.print("Umidade: ");
    lcd.print(umidadeInterna);
    lcd.print(" % ");
    lcd.setCursor(0, 3);
    lcd.print("PWM: ");
    lcd.print(map(Output, 0, 255, 0, 100));
    lcd.print(" % ");
    lastMillis = millis();
  }
}

void serialdata()
{
  temperaturaInterna = dht.readTemperature();
  umidadeInterna = dht.readHumidity();
  mesaTemperatura = readTemperature();
  Serial.print("Temperatura: ");
  Serial.print(temperaturaInterna);
  Serial.print(" °C   Umidade: ");
  Serial.print(umidadeInterna);
  Serial.print(" %   ");
  Serial.print("Temperatura mesa: ");
  Serial.print(mesaTemperatura);
  Serial.print(" °C");
  Serial.print("   Setpoint: ");
  Serial.print(Setpoint);
  Serial.print(" °C");
  Serial.print("   PWM: ");
  Serial.print(map(Output, 0, 255, 0, 100));
  Serial.println(" % ");
}

void setpointconfig()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp. Mesa: ");
  lcd.setCursor(0, 1);
  lcd.print(mesaTemperatura);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print("Objetivo: ");
  lcd.setCursor(0, 3);
  lcd.print(Setpoint);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(" ");
  while (true)
  {
    static int pos = 0;
    encoder.tick();
    int newPos = encoder.getPosition();
    if (pos != newPos)
    {
      Setpoint += (int)(encoder.getDirection());
      lcd.setCursor(0, 0);
      lcd.print("Temp. Mesa: ");
      lcd.setCursor(0, 1);
      lcd.print(mesaTemperatura);
      lcd.print(" ");
      lcd.write(byte(0));
      lcd.print(" ");
      lcd.setCursor(0, 2);
      lcd.print("Objetivo: ");
      lcd.setCursor(0, 3);
      lcd.print(Setpoint);
      lcd.print(" ");
      lcd.write(byte(0));
      lcd.print(" ");
      Serial.print("Temperatura mesa: ");
      Serial.print(mesaTemperatura);
      Serial.print(" °C || ");
      Serial.print("Setpoint: ");
      Serial.print(Setpoint);
      Serial.println(" °C");
      pos = newPos;
    }
    if (button.getSingleDebouncedPress())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(1));
      lcd.write(byte(2));
      lcd.setCursor(2, 0);
      lcd.print("Temp. Setpoint ");
      lcd.setCursor(2, 1);
      lcd.print("Temp. Ativ. FAN ");
      lcd.setCursor(2, 2);
      lcd.print("Umi. Ativ. FAN ");
      lcd.setCursor(2, 3);
      lcd.print("Voltar ");
      break;
    }
  }
}

void tempfanconfig()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ativa em: ");
  lcd.setCursor(0, 1);
  lcd.print(FANTEMP);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print("Mudar para: ");
  lcd.setCursor(0, 3);
  lcd.print(FANTEMP);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print(" ");
  while (true)
  {
    static int pos = 0;
    encoder.tick();
    int newPos = encoder.getPosition();
    if (pos != newPos)
    {
      FANTEMP += (int)(encoder.getDirection());
      lcd.setCursor(0, 2);
      lcd.print("Mudar para: ");
      lcd.setCursor(0, 3);
      lcd.print(FANTEMP);
      lcd.print(" ");
      lcd.write(byte(0));
      lcd.print(" ");
      Serial.print("Temperatura de ativação da FAN: ");
      Serial.print(FANTEMP);
      Serial.println(" °C");
      pos = newPos;
    }
    if (button.getSingleDebouncedPress())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(1));
      lcd.write(byte(2));
      lcd.setCursor(2, 0);
      lcd.print("Temp. Setpoint ");
      lcd.setCursor(2, 1);
      lcd.print("Temp. Ativ. FAN ");
      lcd.setCursor(2, 2);
      lcd.print("Umi. Ativ. FAN ");
      lcd.setCursor(2, 3);
      lcd.print("Voltar ");
      break;
    }
  }
}

void umimaxfanconfig()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ativa em: ");
  lcd.setCursor(0, 1);
  lcd.print(FANMAX);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print("% ");
  lcd.setCursor(0, 2);
  lcd.print("Mudar para: ");
  lcd.setCursor(0, 3);
  lcd.print(FANMAX);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print("% ");
  while (true)
  {
    static int pos = 0;
    encoder.tick();
    int newPos = encoder.getPosition();
    if (pos != newPos)
    {
      FANMAX += (int)(encoder.getDirection());
      lcd.setCursor(0, 2);
      lcd.print("Mudar para: ");
      lcd.setCursor(0, 3);
      lcd.print(FANMAX);
      lcd.print(" % ");
      Serial.print("Umidade de ativação da FAN: ");
      Serial.print(FANMAX);
      Serial.println(" % ");
      pos = newPos;
    }
    if (button.getSingleDebouncedPress())
    {
      minimalFanUmidityConfig();
      break;
    }
  }
}

void minimalFanUmidityConfig()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desativa em: ");
  lcd.setCursor(0, 1);
  lcd.print(FANMIN);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print("% ");
  lcd.setCursor(0, 2);
  lcd.print("Mudar para: ");
  lcd.setCursor(0, 3);
  lcd.print(FANMIN);
  lcd.print(" ");
  lcd.write(byte(0));
  lcd.print("% ");
  while (true)
  {
    static int pos = 0;
    encoder.tick();
    int newPos = encoder.getPosition();
    if (pos != newPos)
    {
      FANMIN += (int)(encoder.getDirection());
      lcd.setCursor(0, 2);
      lcd.print("Mudar para: ");
      lcd.setCursor(0, 3);
      lcd.print(FANMIN);
      lcd.print(" % ");
      Serial.print("Umidade de desativação da FAN: ");
      Serial.print(FANMIN);
      Serial.println(" % ");
      pos = newPos;
    }
    if (button.getSingleDebouncedPress())
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.write(byte(1));
      lcd.write(byte(2));
      lcd.setCursor(2, 0);
      lcd.print("Temp. Setpoint ");
      lcd.setCursor(2, 1);
      lcd.print("Temp. Ativ. FAN ");
      lcd.setCursor(2, 2);
      lcd.print("Umi. Ativ. FAN ");
      lcd.setCursor(2, 3);
      lcd.print("Voltar ");
      break;
    }
  }
}

void menu()
{
  int i = 0;
  lcd.clear();
  lcd.setCursor(0, linha[i]);
  lcd.write(byte(1));
  lcd.write(byte(2));
  lcd.setCursor(2, 0);
  lcd.print("Temp. Setpoint ");
  lcd.setCursor(2, 1);
  lcd.print("Temp. Ativ. FAN ");
  lcd.setCursor(2, 2);
  lcd.print("Umi. Ativ. FAN ");
  lcd.setCursor(2, 3);
  lcd.print("Voltar ");
  int s = 1;
  while (s == 1)
  {
    static int pos = 0;
    encoder.tick();
    int newPos = encoder.getPosition();
    if (pos != newPos)
    {
      lcd.clear();
      i += (int)(encoder.getDirection());
      if (i > 3)
      {
        i = 0;
      }
      if (i < 0)
      {
        i = 3;
      }
      lcd.setCursor(0, linha[i]);
      lcd.write(byte(1));
      lcd.write(byte(2));
      lcd.setCursor(2, 0);
      lcd.print("Temp. Setpoint ");
      lcd.setCursor(2, 1);
      lcd.print("Temp. Ativ. FAN ");
      lcd.setCursor(2, 2);
      lcd.print("Umi. Ativ. FAN ");
      lcd.setCursor(2, 3);
      lcd.print("Voltar ");
      pos = newPos;
    }
    if (button.getSingleDebouncedPress())
    {
      switch (linha[i])
      {
        case 0: // setpoint
          setpointconfig();
          break;
        case 1: // temperatura de ativação da fan
          tempfanconfig();
          break;
        case 2: // umidade de ativação da fan
          umimaxfanconfig();
          break;
        case 3: // voltar
          s = 0;
          break;
      }
    }
  }
}

void calcPID()
{
  Input = readTemperature();
  myPID.Compute();
  analogWrite(PIN_OUTPUT, Output);
}

void ativaFAN()
{
  if (temperaturaInterna > FANTEMP)
  {
    if (umidadeInterna > FANMAX)
    {
      analogWrite(PIN_FAN, 100);
    }
    else if (umidadeInterna <= FANMIN)
    {
      analogWrite(PIN_FAN, 0);
    }
  }
  else
  {
    analogWrite(PIN_FAN, 0);
  }
}

void setup()
{
  Serial.begin(9600);
  // Initialize the DHT sensor
  dht.begin();
  // Initialize the LCD
  lcd.begin(LCD_COLS, LCD_ROWS);
  lcd.createChar(0, bitmapCelsius);
  lcd.createChar(1, bitmapSeta1);
  lcd.createChar(2, bitmapSeta2);

  pinMode(BTN_ENC, INPUT);
  pinMode(PIN_OUTPUT, OUTPUT);
  pinMode(PIN_FAN, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Input = readTemperature();
  Setpoint = 130;

  // turn the PID on
  myPID.SetMode(AUTOMATIC);

  lcd.setCursor(0, 0);
  lcd.print("Inicializando");
  for (int i = 0; i <= 5; i++)
  {
    lcd.print(".");
    delay(200);
  }
  digitalWrite(buzzer, HIGH);
  delay(200);
  digitalWrite(buzzer, LOW);
  lcd.setCursor(0, 1);
  lcd.print("Pronto!");
  delay(800);
}

void loop()
{
  calcPID();
  serialdata();
  ativaFAN();
  if (button.getSingleDebouncedPress())
  {
    menu();
  }
  lcdprint();
}
