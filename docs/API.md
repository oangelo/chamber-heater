# API e Protocolos - Chamber Heater

## 📡 Comunicação Serial

### Configuração

- **Baudrate**: 115200 bps
- **Bits de dados**: 8
- **Paridade**: Nenhuma
- **Bits de parada**: 1
- **Controle de fluxo**: Nenhum

### Porta Serial

- **Linux**: `/dev/ttyUSB0` ou `/dev/ttyACM0`
- **Windows**: `COM3`, `COM4`, etc.
- **macOS**: `/dev/cu.usbserial-*`

## 📤 Formato de Saída (Output)

### Dados de Telemetria

O sistema transmite dados continuamente a cada 2 segundos no formato CSV:

```
temperatura_mesa,temperatura_ambiente,umidade,velocidade_ventilador
```

#### Campos

| Campo | Tipo | Unidade | Faixa | Descrição |
|-------|------|---------|-------|-----------|
| `temperatura_mesa` | float | °C | 0-100 | Temperatura da mesa aquecida medida pelo termistor NTC |
| `temperatura_ambiente` | float | °C | -40-80 | Temperatura ambiente medida pelo DHT22 |
| `umidade` | float | % | 0-100 | Umidade relativa medida pelo DHT22 |
| `velocidade_ventilador` | int | PWM | 0-255 | Valor PWM aplicado ao ventilador |

#### Exemplo

```csv
45.3,23.5,65.2,128
46.1,23.6,64.8,130
47.2,23.7,64.5,132
```

#### Valores Especiais

- `nan`: Leitura inválida (erro no sensor DHT22)
- `0.0` ou valores muito baixos no termistor: Sensor desconectado
- `255`: Ventilador na velocidade máxima

### Formato Futuro (Planejado)

Em versões futuras, o formato poderá ser expandido para JSON:

```json
{
  "timestamp": 1699900000,
  "bed": {
    "temp": 45.3,
    "setpoint": 60.0,
    "output": 180
  },
  "ambient": {
    "temp": 23.5,
    "humidity": 65.2
  },
  "fan": {
    "speed": 128,
    "setpoint": 50.0
  },
  "status": "OK"
}
```

## 📥 Comandos de Entrada (Input)

### Versão Atual

⚠️ **Nota**: A versão atual (1.0.0) **NÃO** aceita comandos via serial. 
Toda configuração é feita via interface física (encoder + LCD).

### Comandos Planejados (Futuro)

Comandos seguirão o formato NMEA-like com checksum:

```
$COMANDO,PARAMETRO1,PARAMETRO2*CHECKSUM\r\n
```

#### Comandos Propostos

| Comando | Parâmetros | Descrição | Exemplo |
|---------|-----------|-----------|---------|
| `ARM` | 0/1 | Arma/desarma o sistema | `$ARM,1*4D` |
| `SETTEMP` | temp_°C | Define setpoint de temperatura | `$SETTEMP,60.0*3A` |
| `SETHUM` | humidity_% | Define setpoint de umidade | `$SETHUM,50.0*2B` |
| `GETCONF` | - | Retorna configuração atual | `$GETCONF*1C` |
| `RESET` | - | Reseta o sistema | `$RESET*2D` |
| `PID` | Kp,Ki,Kd | Ajusta parâmetros PID | `$PID,15.0,1.0,15.0*4E` |

#### Respostas

```
$ACK,COMANDO*CHECKSUM\r\n      # Comando aceito
$NAK,COMANDO,ERRO*CHECKSUM\r\n # Comando rejeitado
```

## 🔧 Interface de Usuário (Atual)

### Menu via Encoder

#### Navegação

1. **Girar encoder**: Move cursor entre opções
2. **Pressionar botão**: Seleciona opção atual
3. **Pressionar novamente**: Confirma valor ou volta

#### Estrutura do Menu

```
MENU PRINCIPAL
├── SET UMIDADE     → Ajusta setpoint de umidade (0-100%)
├── SET TEMPERATURA → Ajusta setpoint de temperatura (0-100°C)
└── Voltar          → Retorna à tela de operação
```

#### Tela de Operação

```
┌────────────────────┐
│MESA:XX.X°C→YY.Y°C │  Linha 0: Temperatura mesa (atual → setpoint)
│TEMP:XX.X°C        │  Linha 1: Temperatura ambiente
│UMID:XX.X%→YY.Y%   │  Linha 2: Umidade (atual → setpoint)
│FAN:XXX            │  Linha 3: Velocidade ventilador (PWM)
└────────────────────┘
```

## 🔔 Notificações (Buzzer)

### Sons Atuais

| Evento | Padrão | Descrição |
|--------|--------|-----------|
| Menu aberto | Beep curto (100ms) | Ao pressionar botão do encoder |

### Sons Planejados

| Evento | Padrão | Descrição |
|--------|--------|-----------|
| Temperatura crítica | 3 beeps rápidos (repetir) | Mesa > 100°C |
| Erro de sensor | 2 beeps longos | DHT22 retorna NaN por 5 leituras |
| Setpoint atingido | 1 beep médio | Temperatura estabilizou |

## 📊 Integração com Software Externo

### Python - Leitura de Dados

```python
import serial
import csv
from datetime import datetime

# Configurar porta serial
ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

# Ler dados continuamente
try:
    while True:
        line = ser.readline().decode('utf-8').strip()
        
        if line:
            # Parse CSV
            data = line.split(',')
            if len(data) == 4:
                bed_temp, amb_temp, humidity, fan_speed = data
                
                print(f"Mesa: {bed_temp}°C | Ambiente: {amb_temp}°C | "
                      f"Umidade: {humidity}% | Fan: {fan_speed}")
                
except KeyboardInterrupt:
    print("\nEncerrando...")
finally:
    ser.close()
```

### Python - Logging em Arquivo

```python
import serial
import csv
from datetime import datetime

ser = serial.Serial('/dev/ttyUSB0', 115200)

with open('chamber_log.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['timestamp', 'bed_temp', 'amb_temp', 'humidity', 'fan_speed'])
    
    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            if line:
                data = line.split(',')
                if len(data) == 4:
                    row = [datetime.now().isoformat()] + data
                    writer.writerow(row)
                    f.flush()  # Garante gravação imediata
                    
    except KeyboardInterrupt:
        print("Log encerrado")
    finally:
        ser.close()
```

### Python - Análise em Tempo Real

```python
import serial
import matplotlib.pyplot as plt
from collections import deque
import matplotlib.animation as animation

# Buffers para dados
max_points = 100
times = deque(maxlen=max_points)
bed_temps = deque(maxlen=max_points)
amb_temps = deque(maxlen=max_points)
humidities = deque(maxlen=max_points)

ser = serial.Serial('/dev/ttyUSB0', 115200)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

def update(frame):
    try:
        line = ser.readline().decode('utf-8').strip()
        if line:
            data = line.split(',')
            if len(data) == 4:
                times.append(frame)
                bed_temps.append(float(data[0]))
                amb_temps.append(float(data[1]))
                humidities.append(float(data[2]))
                
                # Plot temperaturas
                ax1.clear()
                ax1.plot(list(times), list(bed_temps), label='Mesa', color='red')
                ax1.plot(list(times), list(amb_temps), label='Ambiente', color='blue')
                ax1.set_ylabel('Temperatura (°C)')
                ax1.legend()
                ax1.grid(True)
                
                # Plot umidade
                ax2.clear()
                ax2.plot(list(times), list(humidities), label='Umidade', color='green')
                ax2.set_ylabel('Umidade (%)')
                ax2.set_xlabel('Tempo')
                ax2.legend()
                ax2.grid(True)
    except:
        pass

ani = animation.FuncAnimation(fig, update, interval=2000)
plt.tight_layout()
plt.show()

ser.close()
```

### Node.js - Exemplo

```javascript
const SerialPort = require('serialport');
const Readline = require('@serialport/parser-readline');

const port = new SerialPort('/dev/ttyUSB0', { baudRate: 115200 });
const parser = port.pipe(new Readline({ delimiter: '\n' }));

parser.on('data', line => {
  const [bedTemp, ambTemp, humidity, fanSpeed] = line.split(',');
  
  const data = {
    timestamp: new Date().toISOString(),
    bed_temp: parseFloat(bedTemp),
    amb_temp: parseFloat(ambTemp),
    humidity: parseFloat(humidity),
    fan_speed: parseInt(fanSpeed)
  };
  
  console.log(JSON.stringify(data));
});
```

## 🌐 Integração Web (Planejada)

### REST API (Futuro)

Com módulo WiFi (ESP32 ou shield Ethernet):

```http
GET /api/status
Response: {"bed_temp": 45.3, "amb_temp": 23.5, ...}

POST /api/setpoint
Body: {"temp": 60.0, "humidity": 50.0}

GET /api/config
Response: {"pid": {"Kp": 15, "Ki": 1, "Kd": 15}, ...}
```

### WebSocket (Futuro)

```javascript
const ws = new WebSocket('ws://chamber-heater.local:8080');

ws.onmessage = (event) => {
  const data = JSON.parse(event.data);
  console.log(`Mesa: ${data.bed_temp}°C`);
};
```

## 🔒 Segurança

### Versão Atual

- **Sem autenticação**: Serial aberta
- **Sem criptografia**: Dados em texto plano

### Recomendações

- Não expor porta serial à rede
- Usar cabo USB isolado
- Monitorar acessos físicos

### Versão Futura (com WiFi)

- Autenticação básica (usuário/senha)
- HTTPS para API REST
- Token JWT para sessões
- Rate limiting para evitar spam

## 📝 Changelog da API

### Versão 1.0.0 (Atual)

- ✅ Saída serial CSV (4 campos)
- ✅ Formato estável e documentado
- ❌ Sem comandos de entrada
- ❌ Sem autenticação

### Versão 1.1.0 (Planejada)

- ⏳ Comandos NMEA-like
- ⏳ Checksum de validação
- ⏳ Respostas ACK/NAK

### Versão 2.0.0 (Futura)

- ⏳ API REST via WiFi/Ethernet
- ⏳ WebSocket para streaming
- ⏳ Autenticação
- ⏳ Formato JSON opcional

## 🐛 Troubleshooting de Comunicação

### Problema: Nada aparece no serial

**Solução**:
1. Verificar baudrate (115200)
2. Verificar porta correta
3. Aguardar 2-3 segundos após conexão
4. Pressionar reset no Arduino

### Problema: Caracteres estranhos

**Solução**:
1. Ajustar baudrate para 115200
2. Verificar configuração 8-N-1
3. Fechar outros programas usando a porta

### Problema: Valores NaN frequentes

**Solução**:
1. Verificar sensor DHT22
2. Aumentar intervalo de leitura
3. Ver [TROUBLESHOOTING.md](./TROUBLESHOOTING.md)

## 📞 Suporte

Para dúvidas sobre a API:
- Abra uma issue no GitHub
- Consulte [TROUBLESHOOTING.md](./TROUBLESHOOTING.md)
- Veja exemplos em `software/`

---

**Última atualização**: 2024-11-14  
**Versão da API**: 1.0.0

