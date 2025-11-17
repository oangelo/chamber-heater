# Firmware - Chamber Heater

## 📐 Arquitetura

O firmware implementa um sistema de controle dual com dois controladores PID independentes:

### Sistema de Controle

```
┌─────────────────────────────────────────────────────────────┐
│                    Loop Principal (2s)                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐      ┌──────────────┐                    │
│  │   Termistor  │──────▶│   PID Mesa   │──────▶ PWM Mesa   │
│  │  (Analógico) │      │   (DIRECT)   │                    │
│  └──────────────┘      └──────────────┘                    │
│                                                              │
│  ┌──────────────┐      ┌──────────────┐                    │
│  │    DHT22     │──────▶│  PID Fan     │──────▶ PWM Fan    │
│  │ (Temp/Umid)  │      │  (REVERSE)   │                    │
│  └──────────────┘      └──────────────┘                    │
│                                                              │
│  ┌──────────────┐      ┌──────────────┐                    │
│  │   Encoder +  │──────▶│  Interface   │──────▶ LCD 20x4   │
│  │    Botão     │      │     Menu     │                    │
│  └──────────────┘      └──────────────┘                    │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Estados do Sistema

1. **OPERAÇÃO NORMAL**: 
   - Leitura de sensores a cada 2 segundos
   - Cálculo e ajuste dos PIDs
   - Atualização do display LCD
   - Saída de dados via Serial

2. **MENU**: 
   - Navegação via encoder rotativo
   - Ajuste de setpoints (umidade e temperatura)
   - Retorno ao modo operação

## ⚙️ Configuração

### Constantes do PID

As constantes PID estão definidas no código:

```cpp
const int Kp = 15;  // Ganho proporcional
const int Ki = 1;   // Ganho integral
const int Kd = 15;  // Ganho derivativo
```

**⚠️ Nota**: Em versões futuras, estas constantes serão movidas para `config.h`

### Parâmetros dos Sensores

#### Termistor NTC
```cpp
#define RT0 100000    // Resistência a 25°C (100kΩ)
#define B 3950        // Constante B do termistor (K)
#define VCC 5         // Tensão de alimentação (V)
#define R 4700        // Resistor pull-up (Ω)
```

#### DHT22
```cpp
#define DHT_PIN 4
#define DHT_TYPE DHT22
```

### Display LCD
```cpp
#define LCD_COLS 20   // Colunas do display
#define LCD_ROWS 4    // Linhas do display
```

## 🔄 Fluxo de Dados

### 1. Leitura dos Sensores

```
┌─────────────┐
│  setup()    │  Inicializa sensores, LCD, encoder e PIDs
└──────┬──────┘
       │
       ▼
┌─────────────┐
│   loop()    │  Loop principal (2s de intervalo)
└──────┬──────┘
       │
       ├──▶ readBedTemp()      → Lê termistor via ADC
       │                          (Equação Steinhart-Hart)
       │
       ├──▶ dht.readTemperature() → Lê DHT22 (temperatura)
       │
       └──▶ dht.readHumidity()    → Lê DHT22 (umidade)
```

### 2. Processamento PID

```
Entrada → PID.Compute() → Saída PWM
```

- **Mesa (DIRECT)**: Aumenta PWM quando temperatura < setpoint
- **Ventilador (REVERSE)**: Aumenta PWM quando umidade > setpoint

### 3. Atualização da Interface

```
┌──────────────────┐
│  Linha 0: MESA   │  "MESA: XX.X°C -> YY.Y°C"
│  Linha 1: TEMP   │  "TEMP: XX.X°C"
│  Linha 2: UMID   │  "UMID: XX.X% -> YY.Y%"
│  Linha 3: FAN    │  "FAN: XXX"
└──────────────────┘
```

### 4. Saída Serial

Formato: `temperatura_mesa,temperatura_ambiente,umidade,velocidade_ventilador`

Exemplo: `45.3,23.5,65.2,128`

## 🎮 Comandos e Interface

### Navegação no Menu

- **Girar encoder**: Navegar entre opções
- **Pressionar botão**: Selecionar opção

### Opções do Menu

1. **SET UMIDADE**: Ajusta setpoint de umidade (0-100%)
2. **SET TEMPERATURA**: Ajusta setpoint de temperatura da mesa (0-100°C)
3. **Voltar**: Retorna à tela principal

### Notificações

- **Buzzer**: Emite beep ao entrar no menu (função pode ser expandida)

## 📚 Bibliotecas Necessárias

| Biblioteca | Versão | Descrição | Link |
|------------|--------|-----------|------|
| DHT | 1.4.x | Leitura do DHT22 | [GitHub](https://github.com/adafruit/DHT-sensor-library) |
| LiquidCrystal | Builtin | Controle do LCD HD44780 | Arduino IDE |
| PID_v1_bc | 1.2.x | Controlador PID | [GitHub](https://github.com/br3ttb/Arduino-PID-Library) |
| RotaryEncoder | 1.5.x | Leitura do encoder | [GitHub](https://github.com/mathertel/RotaryEncoder) |
| Pushbutton | 1.0.x | Debounce do botão | [Pololu](https://github.com/pololu/pushbutton-arduino) |

### Instalação das Bibliotecas

#### Via Arduino IDE
```
Sketch → Include Library → Manage Libraries...
```
Buscar e instalar cada biblioteca acima.

#### Via PlatformIO
```ini
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
lib_deps = 
    adafruit/DHT sensor library@^1.4.4
    mathertel/RotaryEncoder@^1.5.3
    pololu/Pushbutton@^1.0.0
    br3ttb/PID@^1.2.1
```

## 🔧 Compilação e Upload

### Arduino IDE

1. Abra `firmware/firmware.ino`
2. Selecione a placa: **Tools → Board → Arduino Mega or Mega 2560**
3. Selecione a porta: **Tools → Port → (sua porta)**
4. Clique em **Upload**

### PlatformIO

```bash
cd chamber-heater
pio run --target upload
```

## 📊 Monitoramento Serial

### Arduino IDE
```
Tools → Serial Monitor (115200 baud)
```

### PlatformIO
```bash
pio device monitor
```

### Python Script (exemplo)
```python
import serial
import time

ser = serial.Serial('/dev/ttyUSB0', 115200)
while True:
    data = ser.readline().decode('utf-8').strip()
    print(f"Dados: {data}")
```

## 🐛 Debug

### Modo Verbose

Para adicionar debug, descomente ou adicione prints seriais:

```cpp
void loop() {
  Serial.print("DEBUG: Temp Mesa = ");
  Serial.println(current_bed_temp);
  // ...
}
```

### Problemas Comuns

Ver [docs/TROUBLESHOOTING.md](../docs/TROUBLESHOOTING.md)

## 🚀 Melhorias Futuras

- [ ] Mover configurações para `config.h`
- [ ] Implementar máquina de estados formal
- [ ] Adicionar logging em cartão SD
- [ ] Implementar comunicação via WiFi/Ethernet
- [ ] Adicionar proteção contra superaquecimento
- [ ] Implementar auto-tuning dos parâmetros PID
- [ ] Adicionar gráficos no display (se usar display gráfico)
- [ ] Implementar alarmes configuráveis

## 📝 Estrutura do Código

```
firmware.ino
├── Definições (pinos, constantes)
├── Inicialização de objetos
├── Funções auxiliares
│   ├── readBedTemp()      - Lê temperatura da mesa
│   ├── calcBedPID()       - Calcula PID da mesa
│   ├── calcFanPID()       - Calcula PID do ventilador
│   ├── menu()             - Sistema de menu
│   ├── mostrarMenu()      - Renderiza menu no LCD
│   ├── ajustarUmidade()   - UI para ajuste de umidade
│   ├── ajustarTemperatura() - UI para ajuste de temperatura
│   └── beep()             - Controle do buzzer
├── setup()                - Inicialização
└── loop()                 - Loop principal
```

## 🔐 Configuração Futura (config.h)

**Nota**: Em desenvolvimento. Versão futura incluirá:

```cpp
// config.h (exemplo)
#define BED_SETPOINT_DEFAULT 60.0
#define HUMIDITY_SETPOINT_DEFAULT 50.0
#define PID_BED_KP 15
#define PID_BED_KI 1
#define PID_BED_KD 15
// ... mais configurações
```

Por enquanto, edite diretamente no código `firmware.ino`.

