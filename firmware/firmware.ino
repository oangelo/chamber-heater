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

// Simbolos Customizados
byte bitmapCelsius[8] = {0b11000, 0b11000, 0b00000, 0b00110, 0b01001, 0b01000, 0b01001, 0b00110};
byte bitmapSeta1[8] = {0b00000, 0b00000, 0b00000, 0b01111, 0b01111, 0b00000, 0b00000, 0b00000};
byte bitmapSeta2[8] = {0b10000, 0b11000, 0b11100, 0b11110, 0b11110, 0b11100, 0b11000, 0b10000};

double bed_set_point, bed_INPUT, bed_OUTPUT;// Variáveis do PID da mesa
double fan_set_point, fan_INPUT, fan_OUTPUT;
const int Kp = 15, Ki = 1, Kd = 15; // Constantes do PID da mesa

RotaryEncoder encoder(BTN_EN1, BTN_EN2, RotaryEncoder::LatchMode::TWO03); // Encoder
Pushbutton button(BTN_ENC);                                               // Botão do encoder
DHT dht(DHT_PIN, DHT_TYPE);                                               // Sensor de temperatura e umidade
LiquidCrystal lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);        // LCD
PID bedPID(&bed_INPUT, &bed_OUTPUT, &bed_set_point, Kp, Ki, Kd, DIRECT);  // PID da mesa
PID fanPID(&fan_INPUT, &fan_OUTPUT, &fan_set_point, Kp, Ki, Kd, REVERSE); // PID da ventoinha


float last_temp = -999.0;
float last_hum = -999.0;



// --- Variáveis de tempo e menu ---
unsigned long previousMillis = 0;
const long interval = 2000;
const char *opcoes[] = {"SET UMIDADE", "SET TEMPERATURA", "Voltar"};
const int totalOpcoes = 3;


// --- Variáveis de Leitura e Estado ---
double current_ambient_temp = 0;
double current_humidity = 0;
double current_bed_temp = 0;
double last_bed_temp = -999.0;
int fan_speed = 0;

// Retorna a temperatura da mesa
float readBedTemp()
{
  float RT, VR, ln, TX, VRT;
  const float T0 = 20 + 273.15;     // K -> °C
  VRT = analogRead(bed_INPUT_PIN);  // Lê o valor analógico do pino do termistor
  VRT = (5.00 / 1023.00) * VRT;     // Converte o valor lido para tensão
  VR = VCC - VRT;                   // Calcula a tensão restante (VCC - VRT)
  RT = VRT / (VR / R);              // Calcula a resistência do termistor (RT)
  ln = log(RT / RT0);               // Calcula o logaritmo natural da razão entre RT e RT0
  TX = (1 / ((ln / B) + (1 / T0))); // Calcula a temperatura em Kelvin usando a equação de Steinhart-Hart
  TX = TX - 273.15;                 // Converte a temperatura de Kelvin para Celsius
  return TX;                        // Retorna a temperatura calculada
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

// Menu lcd
void menu()
{
  int indiceMenu = 0;
  int ultimaPos = encoder.getPosition();
  mostrarMenu(indiceMenu); // Mostra o menu a primeira vez
  while (true)
  { // Este loop agora pode ser quebrado
    encoder.tick();
    int novaPos = encoder.getPosition();
    if (novaPos != ultimaPos)
    {
      int delta = novaPos - ultimaPos;
      ultimaPos = novaPos;
      indiceMenu += delta;
      if (indiceMenu < 0)
        indiceMenu = totalOpcoes - 1;
      if (indiceMenu >= totalOpcoes)
        indiceMenu = 0;
      mostrarMenu(indiceMenu);
    }

    if (button.getSingleDebouncedPress())
    {
      // Se a opção selecionada for "Voltar" (última opção)
      if (indiceMenu == totalOpcoes - 1)
      {
        return; // Sai da função menu() e volta para o loop() principal
      }

      lcd.clear();
      switch (indiceMenu)
      {
        case 0:
          ajustarUmidade();
          delay(1000);
          break;
        case 1:
          ajustarTemperatura();
          delay(1000);
          break;
      }

      mostrarMenu(indiceMenu);
    }
  }
}

void mostrarMenu(int indice)
{
  lcd.clear(); // Limpa a tela antes de redesenhar o menu
  for (int i = 0; i < totalOpcoes; i++)
  {
    lcd.setCursor(0, i);
    if (i == indice)
    {
      lcd.write(byte(1)); // Desenha a primeira metade da seta
      lcd.write(byte(2)); // Desenha a segunda metade da seta
    }
    else
    {
      lcd.print("  ");
    }
    lcd.print(" ");
    lcd.print(opcoes[i]);
  }
}

// informacoes

void setupInfoScreen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp Int: ");
  lcd.setCursor(0, 1);
  lcd.print("Umid Int:  ");
  lcd.setCursor(0, 2);
  lcd.print("Temp Mesa: ");
  lcd.setCursor(0, 3);
  lcd.print("Pressione para Menu");
}

// atualiza os valores de temperatura e umidade no LCD
void updateInfoValues()
{

  // Lê a temperatura e umidade do sensor DHT22
  float current_hum = dht.readHumidity();
  float current_temp = dht.readTemperature();
  float current_bed_temp = readBedTemp();
  // Verifica se a leitura é válida (sensores DHT às vezes falham)
  if (isnan(current_hum) || isnan(current_temp))
  {
    return; // Se a leitura falhou, não faz nada
  }
  // --- Atualiza a Temperatura ---
  // Compara o valor atual com o último valor mostrado
  if (current_temp != last_temp)
  {
    lcd.setCursor(11, 0); // Posição exata para escrever o valor da temperatura
    lcd.print("      ");  // Limpa a área com espaços (para casos como 100.0 -> 25.5)
    lcd.setCursor(11, 0); // Volta o cursor
    lcd.print(current_temp, 1);
    lcd.write((byte)0);       // Simbolo °C
    last_temp = current_temp; // Atualiza o último valor mostrado
  }
  // --- Atualiza a Umidade ---

  if (current_hum != last_hum)
  {

    lcd.setCursor(11, 1); // Posição para o valor da umidade
    lcd.print("      ");  // Limpa a área
    lcd.setCursor(11, 1); // Volta o cursor
    lcd.print(current_hum, 1);
    lcd.print(" %");
    last_hum = current_hum; // Atualiza o último valor
  }

  // --- temperatura da mesa
  if (current_bed_temp != last_bed_temp)
  {
    lcd.setCursor(11, 2);
    lcd.print("      ");  // Limpa a área
    lcd.setCursor(11, 2); // Volta o cursor
    lcd.print(current_bed_temp, 2);
    lcd.write((byte)0);
    last_bed_temp = current_bed_temp; // Atualiza o último valor
  }
}

// Função para ajustar a umidade
void ajustarUmidade()
{
  lcd.clear();
  int ultimaPos = encoder.getPosition();
  bool telaAtualizada = false; // Flag para desenhar a tela só uma vez

  while (true)
  {
    encoder.tick();

    // --- Lógica do Encoder: Ajusta o setpoint ---
    int novaPos = encoder.getPosition();
    if (novaPos != ultimaPos)
    {
      // Cada "click" do encoder muda o setpoint em 0.5%
      fan_set_point += (novaPos - ultimaPos) * 0.5;
      ultimaPos = novaPos;
      telaAtualizada = false; // Força a atualização da tela
    }

    // --- Lógica do Display (desenha apenas quando necessário) ---
    if (!telaAtualizada)
    {
      float umidadeAtual = dht.readHumidity();

      lcd.setCursor(0, 0);
      lcd.print("Ajustar Umidade");

      lcd.setCursor(0, 1);
      lcd.print("Atual: ");
      lcd.print(umidadeAtual, 1);
      lcd.print("%   "); // Espaços para limpar a linha

      lcd.setCursor(0, 2);
      lcd.print("Meta-> ");
      lcd.print(fan_set_point, 1);
      lcd.print("%   "); // Espaços para limpar a linha

      lcd.setCursor(0, 3);
      lcd.print("Pressione p/ Salvar");

      telaAtualizada = true; // Marca que a tela foi desenhada
    }

    // --- Lógica de Saída ---
    if (button.getSingleDebouncedPress())
    {
      // Opcional: Tocar um beep para confirmar
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);
      digitalWrite(BUZZER_PIN, LOW);

      return; // Sai da função e volta para o menu principal
    }
  }
}

// Função para ajustar a temperatura

void ajustarTemperatura()
{
  lcd.clear();
  int ultimaPos = encoder.getPosition();
  bool telaAtualizada = false; // Flag para desenhar a tela só quando necessário

  // Loop do submenu: prende o programa aqui até o usuário sair
  while (true)
  {
    encoder.tick();

    // --- Lógica do Encoder: Ajusta o setpoint de temperatura ---
    int novaPos = encoder.getPosition();
    if (novaPos != ultimaPos)
    {
      // Cada "click" do encoder muda o setpoint em 0.5°C
      bed_set_point += (novaPos - ultimaPos) * 0.5;
      ultimaPos = novaPos;
      telaAtualizada = false; // Força a atualização da tela
    }

    // --- Lógica do Display (desenha apenas quando necessário para evitar flicker) ---
    if (!telaAtualizada)
    {
      float temperaturaAtual = readBedTemp();

      lcd.setCursor(0, 0);
      lcd.print("Ajustar Temperatura");

      lcd.setCursor(0, 1);
      lcd.print("Atual: ");
      lcd.print(temperaturaAtual, 1);
      lcd.write((byte)0);
      lcd.print("   ");

      lcd.setCursor(0, 2);
      lcd.print("Meta-> ");
      lcd.print(bed_set_point, 1);
      lcd.write((byte)0);
      lcd.print("   ");

      lcd.setCursor(0, 3);
      lcd.print("Pressione p/ Salvar");

      telaAtualizada = true;
    }

    if (button.getSingleDebouncedPress())
    {

      digitalWrite(BUZZER_PIN, HIGH);
      delay(50);
      digitalWrite(BUZZER_PIN, LOW);

      return;
    }
  }
}

// Função setup
void setup()
{
  Serial.begin(9600);
  lcd.begin(LCD_COLS, LCD_ROWS); // Inicializa o LCD

  lcd.createChar(0, bitmapCelsius);
  lcd.createChar(1, bitmapSeta1);
  lcd.createChar(2, bitmapSeta2);

  dht.begin(); // Inicializa o sensor DHT

  pinMode(BTN_ENC, bed_INPUT);
  pinMode(bed_OUTPUT_PIN, bed_OUTPUT);
  pinMode(FAN_PIN, bed_OUTPUT);
  pinMode(BUZZER_PIN, bed_OUTPUT);

  // --- Configuração da mesa ---
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

  setupInfoScreen();
}

// Função loop
void loop()
{

  calcBedPID();
  
  if (dht.readTemperature() >= 50) {
    calcFanPID();
  }
  else {
    analogWrite(FAN_PIN, 0);
  }
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    updateInfoValues();

  }

  if (button.getSingleDebouncedPress())
  {

    menu(); // Chama a função do menu

    setupInfoScreen();

    last_temp = -999.0;

    last_hum = -999.0;
  }
}
