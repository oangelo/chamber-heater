# Guia de Instalação - Chamber Heater

## 📋 Pré-requisitos

### Hardware Necessário

- [ ] Arduino Mega 2560 (ou MKS GEN v1.4)
- [ ] Sensor DHT22
- [ ] Termistor NTC 100kΩ (B=3950K)
- [ ] Resistor 4.7kΩ (para termistor)
- [ ] Display LCD 20x4 (HD44780)
- [ ] Encoder rotativo com botão
- [ ] Ventilador 12V PWM
- [ ] Mesa aquecida 12V
- [ ] MOSFET (IRLZ44N ou similar)
- [ ] Buzzer 5V
- [ ] Fonte 12V (corrente adequada)
- [ ] Cabos jumper
- [ ] Protoboard ou PCB

### Software Necessário

- [ ] Arduino IDE 1.8.x ou superior (ou PlatformIO)
- [ ] Driver USB para Arduino Mega (CH340 ou FTDI)
- [ ] Bibliotecas Arduino (ver seção abaixo)

## 🔧 Instalação do Software

### Opção 1: Arduino IDE

#### 1. Instalar Arduino IDE

**Linux:**
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install arduino

# Arch/Manjaro
sudo pacman -S arduino

# Ou baixar do site oficial
wget https://downloads.arduino.cc/arduino-1.8.19-linux64.tar.xz
tar -xf arduino-1.8.19-linux64.tar.xz
cd arduino-1.8.19
sudo ./install.sh
```

**Windows:**
- Baixar de https://www.arduino.cc/en/software
- Executar o instalador

**macOS:**
- Baixar de https://www.arduino.cc/en/software
- Arrastar para Applications

#### 2. Instalar Bibliotecas

Abra Arduino IDE e vá em: `Sketch → Include Library → Manage Libraries...`

Instale as seguintes bibliotecas:

1. **DHT sensor library** by Adafruit
   - Buscar: "DHT sensor library"
   - Versão: 1.4.4 ou superior
   - Autor: Adafruit

2. **RotaryEncoder** by Matthias Hertel
   - Buscar: "RotaryEncoder"
   - Versão: 1.5.3 ou superior
   - Autor: Matthias Hertel

3. **Pushbutton** by Pololu
   - Buscar: "Pushbutton"
   - Versão: 1.0.0 ou superior
   - Autor: Pololu

4. **PID** by Brett Beauregard
   - Buscar: "PID"
   - Versão: 1.2.1 ou superior
   - Autor: Brett Beauregard (br3ttb)
   - **Nota**: O projeto usa `PID_v1_bc`, que é uma variante. Se não encontrar, baixe de https://github.com/br3ttb/Arduino-PID-Library

**LiquidCrystal** já vem incluso no Arduino IDE.

#### 3. Configurar Placa

1. Conecte o Arduino Mega ao computador via USB
2. `Tools → Board → Arduino AVR Boards → Arduino Mega or Mega 2560`
3. `Tools → Processor → ATmega2560`
4. `Tools → Port → /dev/ttyUSB0` (Linux) ou `COM3` (Windows)

### Opção 2: PlatformIO (Recomendado)

#### 1. Instalar PlatformIO

**Via VS Code:**
```
Extensions → Buscar "PlatformIO IDE" → Install
```

**Via CLI:**
```bash
pip install platformio
```

#### 2. Criar arquivo platformio.ini

Na raiz do projeto, crie ou edite `platformio.ini`:

```ini
[env:megaatmega2560]
platform = atmelavr
board = megaatmega2560
framework = arduino
monitor_speed = 115200

lib_deps = 
    adafruit/DHT sensor library@^1.4.4
    mathertel/RotaryEncoder@^1.5.3
    pololu/Pushbutton@^1.0.0
    br3ttb/PID@^1.2.1
```

#### 3. Compilar e Fazer Upload

```bash
cd chamber-heater
pio run           # Compilar
pio run -t upload # Upload para o Arduino
```

## 🔌 Montagem do Hardware

### Passo 1: Preparar o Arduino

1. Conecte o Arduino Mega à protoboard ou prepare a PCB
2. Identifique os pinos conforme [hardware/README.md](../hardware/README.md)

### Passo 2: Conectar o DHT22

```
DHT22 VCC  → Arduino 5V
DHT22 GND  → Arduino GND
DHT22 DATA → Arduino Pin 4
```

**Nota**: Alguns DHT22 requerem resistor pull-up de 10kΩ entre DATA e VCC

### Passo 3: Conectar o Termistor (Mesa)

```
Arduino 5V → Resistor 4.7kΩ → Arduino A0
                               |
                               └→ Termistor NTC → GND
```

### Passo 4: Conectar o Display LCD 20x4

| Pino LCD | Conexão Arduino | Observação |
|----------|-----------------|------------|
| VSS | GND | Terra |
| VDD | 5V | Alimentação |
| V0 | Potenciômetro 10kΩ | Contraste |
| RS | Pin 16 | Register Select |
| RW | GND | Read/Write (sempre Write) |
| E | Pin 17 | Enable |
| D0-D3 | - | Não usado (modo 4-bit) |
| D4 | Pin 23 | Data 4 |
| D5 | Pin 25 | Data 5 |
| D6 | Pin 27 | Data 6 |
| D7 | Pin 29 | Data 7 |
| A | 5V (via resistor 220Ω) | Backlight + |
| K | GND | Backlight - |

### Passo 5: Conectar o Encoder Rotativo

```
Encoder A   → Arduino Pin 31
Encoder B   → Arduino Pin 33
Encoder BTN → Arduino Pin 35
Encoder GND → Arduino GND
Encoder +   → Arduino 5V (se necessário)
```

### Passo 6: Conectar o Buzzer

```
Buzzer + → Arduino Pin 37
Buzzer - → Arduino GND
```

### Passo 7: Conectar o Ventilador (via MOSFET)

```
Arduino Pin 9 → Resistor 1kΩ → Gate do MOSFET
MOSFET Source → GND
MOSFET Drain → Ventilador (-)
12V + → Ventilador (+)
```

**MOSFET recomendado**: IRLZ44N, IRL540N

### Passo 8: Conectar a Mesa Aquecida (via MOSFET/SSR)

```
Arduino Pin 8 → Gate do MOSFET (via resistor 1kΩ)
MOSFET Source → GND
MOSFET Drain → Mesa (-)
12V + → Mesa (+)
```

**⚠️ IMPORTANTE**: 
- Use MOSFET adequado para a corrente da mesa
- Adicione dissipador de calor se necessário
- Considere usar SSR (Solid State Relay) para cargas maiores

### Passo 9: Alimentação

```
Fonte 12V + → Vin do Arduino (se Arduino alimentado por 12V)
            → Mesa aquecida
            → Ventilador
            
Fonte 12V - → GND comum
```

**Alternativa**: Alimentar Arduino via USB e usar fonte separada para mesa/ventilador (conectar GNDs juntos!)

## 📤 Upload do Firmware

### Via Arduino IDE

1. Abra `firmware/firmware.ino`
2. Verifique: `Sketch → Verify/Compile` (Ctrl+R)
3. Upload: `Sketch → Upload` (Ctrl+U)
4. Aguarde "Done uploading"

### Via PlatformIO

```bash
cd chamber-heater
pio run -t upload
```

### Verificação

Abra o Serial Monitor (115200 baud):
```
Arduino IDE: Tools → Serial Monitor
PlatformIO: pio device monitor
```

Você deve ver dados no formato:
```
45.3,23.5,65.2,128
46.1,23.6,64.8,130
...
```

## ✅ Testes Iniciais

### Teste 1: Display LCD

- [ ] LCD acende backlight
- [ ] LCD mostra caracteres
- [ ] Ajustar contraste com potenciômetro

**Problema**: LCD não mostra nada
- Verifique conexões
- Ajuste potenciômetro de contraste
- Verifique alimentação 5V

### Teste 2: Encoder

- [ ] Gire o encoder → Valores no Serial Monitor mudam
- [ ] Pressione botão → Menu aparece no LCD

**Problema**: Encoder não responde
- Verifique pinos 31, 33, 35
- Teste com multímetro se há sinal

### Teste 3: DHT22

- [ ] Temperatura ambiente é mostrada no LCD
- [ ] Umidade é mostrada no LCD

**Problema**: Leitura NaN ou inválida
- Aguarde 2 segundos após ligar
- Verifique conexões
- Verifique pin 4

### Teste 4: Termistor

- [ ] Temperatura da mesa é mostrada
- [ ] Ao aquecer com a mão, temperatura sobe

**Problema**: Leitura errada
- Verifique resistor pull-up (4.7kΩ)
- Verifique conexão ao A0

### Teste 5: Ventilador

- [ ] Ventilador gira ao ajustar setpoint de umidade
- [ ] Velocidade varia conforme PID

**Problema**: Ventilador não gira
- Verifique fonte 12V
- Verifique MOSFET
- Teste PWM com LED no pin 9

### Teste 6: Mesa Aquecida

⚠️ **ATENÇÃO**: Mesa pode aquecer muito!

- [ ] Ajuste setpoint de temperatura baixo (30°C)
- [ ] Verifique aquecimento gradual
- [ ] Verifique controle PID mantém temperatura

**Problema**: Mesa não aquece
- Verifique MOSFET
- Verifique fonte 12V
- Verifique conexão pin 8

## 🎯 Calibração

### Calibração do Termistor

Se a leitura da mesa estiver imprecisa:

1. Use termômetro de referência
2. Compare leituras
3. Ajuste constantes no código:
   ```cpp
   #define RT0 100000  // Ajustar se necessário
   #define B 3950      // Verificar no datasheet
   ```

### Ajuste do PID

Para otimizar o controle:

1. Ajuste inicial (atual):
   ```cpp
   const int Kp = 15, Ki = 1, Kd = 15;
   ```

2. Métodos de ajuste:
   - **Ziegler-Nichols**: Método clássico
   - **Manual**: Aumentar Kp até oscilar, depois ajustar Ki e Kd
   - **Auto-tune**: Implementar em versão futura

## 📝 Configurações Iniciais

### Setpoints Padrão

No primeiro uso, configure via menu:

1. Pressione o botão do encoder
2. Selecione "SET TEMPERATURA"
3. Ajuste para temperatura desejada (ex: 60°C)
4. Voltar
5. Selecione "SET UMIDADE"
6. Ajuste para umidade desejada (ex: 50%)
7. Voltar

### Monitoramento

Via Serial (Python):
```python
import serial
import csv
from datetime import datetime

ser = serial.Serial('/dev/ttyUSB0', 115200)

with open('log.csv', 'w') as f:
    writer = csv.writer(f)
    writer.writerow(['timestamp', 'bed_temp', 'amb_temp', 'humidity', 'fan_speed'])
    
    while True:
        data = ser.readline().decode('utf-8').strip().split(',')
        row = [datetime.now()] + data
        writer.writerow(row)
        print(row)
```

## 🔍 Troubleshooting

Ver [TROUBLESHOOTING.md](./TROUBLESHOOTING.md) para problemas comuns e soluções.

## 📞 Suporte

Para dúvidas ou problemas:
- Abra uma issue no GitHub
- Consulte a documentação do Serra Rocketry
- Verifique o [TROUBLESHOOTING.md](./TROUBLESHOOTING.md)

## ✅ Checklist Final

- [ ] Todas as bibliotecas instaladas
- [ ] Firmware compilado sem erros
- [ ] Upload realizado com sucesso
- [ ] Display LCD funcionando
- [ ] Encoder responde
- [ ] Sensores lendo valores
- [ ] Ventilador controlado
- [ ] Mesa aquecida controlada (com cuidado!)
- [ ] Dados no Serial Monitor
- [ ] Setpoints configurados

**Parabéns! Seu Chamber Heater está funcionando! 🎉**

