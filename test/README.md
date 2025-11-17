# Testes - Chamber Heater

## 📋 Plano de Testes

### Testes Unitários

- [x] Leitura do termistor NTC
- [x] Controle PID do ventilador (`fan_pid/`)
- [ ] Leitura DHT22 isolada
- [ ] Encoder e botão isolados
- [ ] Display LCD isolado

### Testes de Integração

- [x] Sistema completo operacional
- [ ] Sensor + Transmissão serial
- [ ] Menu + Controle PID
- [ ] Múltiplos ciclos de aquecimento/resfriamento

### Testes de Campo

| Data | Teste | Resultado | Observações |
|------|-------|-----------|-------------|
| - | - | - | Adicionar logs de testes aqui |

## 🧪 Testes Disponíveis

### test/fan_pid/

**Descrição**: Teste isolado do controlador PID do ventilador

**Como executar**:
1. Abrir `test/fan_pid/fan_pid.ino` no Arduino IDE
2. Fazer upload para o Arduino
3. Monitorar via Serial (115200 baud)

**O que testa**:
- Controle PID básico
- Resposta do ventilador a diferentes setpoints
- Ajuste de parâmetros Kp, Ki, Kd

**Resultados esperados**:
- Ventilador responde ao setpoint
- PID converge para valor estável
- Sem oscilações excessivas

## 📝 Como Adicionar Novos Testes

### 1. Criar diretório do teste

```bash
mkdir test/nome_do_teste
cd test/nome_do_teste
```

### 2. Criar arquivo .ino

```cpp
// nome_do_teste.ino
void setup() {
  Serial.begin(115200);
  // Inicialização
}

void loop() {
  // Teste aqui
  delay(1000);
}
```

### 3. Documentar no README

Adicionar entrada neste arquivo com:
- Nome do teste
- O que testa
- Como executar
- Resultados esperados

## 🔬 Testes Recomendados

### Teste de Precisão do Termistor

**Objetivo**: Verificar precisão da leitura de temperatura

**Procedimento**:
1. Colocar termistor em banho térmico controlado
2. Comparar leitura com termômetro calibrado
3. Registrar diferenças em várias temperaturas (20°C, 40°C, 60°C, 80°C)

**Critério de aceitação**: Erro < ±2°C

### Teste de Resposta do PID

**Objetivo**: Avaliar tempo de resposta do controle

**Procedimento**:
1. Configurar setpoint para 60°C
2. Iniciar com temperatura ambiente
3. Registrar temperatura a cada 10 segundos
4. Medir tempo até estabilizar (±1°C do setpoint)
5. Medir overshoot (quanto ultrapassa)

**Critério de aceitação**: 
- Tempo de estabilização < 15 minutos
- Overshoot < 5°C

### Teste de Estabilidade Longa Duração

**Objetivo**: Verificar estabilidade térmica prolongada

**Procedimento**:
1. Configurar setpoint para 50°C
2. Deixar operando por 24 horas
3. Registrar temperatura a cada minuto
4. Analisar variação e deriva

**Critério de aceitação**:
- Variação < ±1°C
- Sem deriva ao longo do tempo

### Teste de DHT22 - Ciclo Térmico

**Objetivo**: Verificar confiabilidade do DHT22 em temperatura elevada

**Procedimento**:
1. Operar sistema com mesa a 80°C
2. Verificar se DHT22 continua lendo corretamente
3. Registrar falhas de leitura (NaN)

**Critério de aceitação**: < 1% de leituras falhas

## 📊 Análise de Dados

### Script Python para Análise

```python
import pandas as pd
import matplotlib.pyplot as plt

# Ler dados do serial
df = pd.read_csv('test_log.csv', 
                 names=['bed_temp', 'amb_temp', 'humidity', 'fan_speed'])

# Plot
fig, ax = plt.subplots(2, 1, figsize=(12, 8))

ax[0].plot(df['bed_temp'], label='Temperatura Mesa')
ax[0].axhline(y=60, color='r', linestyle='--', label='Setpoint')
ax[0].set_ylabel('Temperatura (°C)')
ax[0].legend()
ax[0].grid(True)

ax[1].plot(df['humidity'], label='Umidade')
ax[1].set_ylabel('Umidade (%)')
ax[1].set_xlabel('Tempo (amostras)')
ax[1].legend()
ax[1].grid(True)

plt.tight_layout()
plt.savefig('test_results.png')
plt.show()
```

### Logging de Dados

Para salvar dados dos testes:

```python
import serial
import csv
from datetime import datetime

ser = serial.Serial('/dev/ttyUSB0', 115200)

with open('test_log.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['timestamp', 'bed_temp', 'amb_temp', 'humidity', 'fan_speed'])
    
    try:
        while True:
            line = ser.readline().decode('utf-8').strip()
            data = line.split(',')
            if len(data) == 4:
                row = [datetime.now().isoformat()] + data
                writer.writerow(row)
                print(row)
    except KeyboardInterrupt:
        print("Logging encerrado")
```

## ✅ Checklist de Validação

Antes de considerar o sistema pronto para uso:

### Hardware
- [ ] Todas as conexões verificadas
- [ ] Tensões corretas medidas
- [ ] Sem aquecimento excessivo de componentes
- [ ] MOSFETs operando corretamente
- [ ] Ventilador girando suavemente

### Software
- [ ] Código compila sem warnings
- [ ] Upload bem sucedido
- [ ] Serial output correto
- [ ] Menu funcional
- [ ] Encoder responsivo

### Funcional
- [ ] Temperatura da mesa estabiliza no setpoint
- [ ] Umidade é controlada pelo ventilador
- [ ] Display mostra informações corretas
- [ ] Sem travamentos ou resets
- [ ] Sistema opera por > 1 hora sem problemas

### Segurança
- [ ] Mesa não ultrapassa 100°C
- [ ] Shutdown automático em caso de falha (implementar)
- [ ] Buzzer alerta em condições anormais
- [ ] Ventilação adequada

## 🆘 Reportando Problemas

Se encontrar bugs durante os testes:

1. Descrever o problema detalhadamente
2. Incluir dados seriais relevantes
3. Fotografar montagem se relevante
4. Mencionar condições do teste
5. Abrir issue no GitHub

## 📁 Organização de Resultados

```
test/
├── fan_pid/              # Teste do PID do ventilador
├── logs/                 # Logs de testes
│   ├── test_001.csv
│   ├── test_002.csv
│   └── ...
├── results/              # Resultados e análises
│   ├── graficos/
│   └── relatorios/
└── README.md             # Este arquivo
```

**Nota**: Arquivos `.csv` e logs extensos não devem ser commitados (ver `.gitignore`)

