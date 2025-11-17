# Data Analysis - Chamber Heater

Scripts Python para análise de dados e monitoramento do Chamber Heater.

## 📦 Instalação

### Dependências

```bash
pip install -r requirements.txt
```

Ou instale manualmente:

```bash
pip install pyserial pandas matplotlib
```

## 🔧 Scripts Disponíveis

### 1. serial_logger.py - Logger de Dados

Captura dados da porta serial e salva em arquivo CSV.

**Uso:**

```bash
# Básico
python serial_logger.py --port /dev/ttyUSB0 --output data.csv

# Com modo verboso
python serial_logger.py -p /dev/ttyUSB0 -o test.csv --verbose

# Windows
python serial_logger.py -p COM3 -o data.csv
```

**Opções:**
- `-p, --port`: Porta serial (padrão: `/dev/ttyUSB0`)
- `-b, --baud`: Baudrate (padrão: `115200`)
- `-o, --output`: Arquivo de saída (padrão: `chamber_log.csv`)
- `-v, --verbose`: Modo verboso (mostra todas as leituras)

**Formato do CSV:**
```csv
timestamp,bed_temp,amb_temp,humidity,fan_speed
2024-11-14T10:30:15.123456,45.3,23.5,65.2,128
2024-11-14T10:30:17.234567,46.1,23.6,64.8,130
...
```

### 2. plot_realtime.py - Visualização em Tempo Real

Plota gráficos em tempo real dos dados recebidos pela serial.

**Uso:**

```bash
# Básico
python plot_realtime.py --port /dev/ttyUSB0

# Com mais pontos no histórico
python plot_realtime.py -p /dev/ttyUSB0 --points 200
```

**Opções:**
- `-p, --port`: Porta serial (padrão: `/dev/ttyUSB0`)
- `-b, --baud`: Baudrate (padrão: `115200`)
- `-n, --points`: Número máximo de pontos (padrão: `100`)

**Gráficos:**
1. Temperaturas (Mesa e Ambiente)
2. Umidade
3. Velocidade do Ventilador

### 3. analyze_log.py - Análise de Logs

Analisa arquivos CSV gerados pelo `serial_logger.py` e gera estatísticas e gráficos.

**Uso:**

```bash
# Gerar análise completa
python analyze_log.py data.csv --output report.png

# Apenas estatísticas (sem gráficos)
python analyze_log.py data.csv --no-plot
```

**Opções:**
- `input`: Arquivo CSV de entrada
- `-o, --output`: Arquivo PNG de saída (padrão: `analysis.png`)
- `--no-plot`: Não gerar gráfico, apenas estatísticas

**Estatísticas geradas:**
- Período de coleta
- Temperatura média/min/max/desvio (mesa e ambiente)
- Umidade média/min/max/desvio
- Velocidade média do ventilador

**Gráficos gerados:**
1. Temperaturas ao longo do tempo
2. Umidade ao longo do tempo
3. Velocidade do ventilador
4. Correlação umidade vs velocidade

## 📊 Exemplos de Uso

### Workflow Completo

#### 1. Capturar dados durante teste

```bash
python serial_logger.py -p /dev/ttyUSB0 -o test_2024-11-14.csv --verbose
```

Deixe rodando durante o teste (30 min, 1 hora, etc). Pressione Ctrl+C para parar.

#### 2. Analisar dados capturados

```bash
python analyze_log.py test_2024-11-14.csv --output test_2024-11-14_analysis.png
```

#### 3. Visualizar em tempo real (opcional)

Para monitoramento durante o teste:

```bash
python plot_realtime.py -p /dev/ttyUSB0
```

### Teste de Estabilidade Térmica

```bash
# 1. Inicie o logging
python serial_logger.py -o stability_test.csv

# 2. Configure setpoint no hardware (via LCD)
# 3. Aguarde 24 horas
# 4. Pressione Ctrl+C no logger

# 5. Analise resultados
python analyze_log.py stability_test.csv -o stability_report.png
```

### Teste de Resposta PID

```bash
# 1. Inicie logging
python serial_logger.py -o pid_test.csv --verbose

# 2. Configure setpoint alto (70°C)
# 3. Observe temperatura subindo
# 4. Após estabilizar, pare o logging (Ctrl+C)

# 5. Analise curva de resposta
python analyze_log.py pid_test.csv -o pid_response.png
```

## 🔍 Troubleshooting

### Erro: "Permission denied" (Linux)

```bash
# Adicionar usuário ao grupo dialout
sudo usermod -a -G dialout $USER

# Logout e login novamente
# Ou temporariamente:
sudo chmod 666 /dev/ttyUSB0
```

### Erro: "Port already in use"

Feche outras aplicações usando a porta (Arduino IDE Serial Monitor, minicom, etc).

### Erro: "Module not found"

```bash
# Instalar dependências faltantes
pip install pyserial pandas matplotlib
```

### Gráficos não aparecem (SSH/remoto)

Para servidores remotos, salve apenas o PNG:

```bash
python analyze_log.py data.csv -o report.png
```

Se precisar de plot remoto via SSH:

```bash
# No servidor, com X11 forwarding
ssh -X usuario@servidor
python plot_realtime.py
```

## 📁 Organização de Dados

Sugestão de estrutura para seus logs:

```
data/
├── 2024-11-14/
│   ├── test_01.csv
│   ├── test_01_analysis.png
│   ├── test_02.csv
│   └── test_02_analysis.png
├── 2024-11-15/
│   └── ...
└── README.md  # Notas sobre os testes
```

## 📝 Formato de Dados

### Entrada (Serial)

Formato CSV bruto da porta serial:

```
45.3,23.5,65.2,128
46.1,23.6,64.8,130
```

### Saída (CSV com timestamp)

```csv
timestamp,bed_temp,amb_temp,humidity,fan_speed
2024-11-14T10:30:15.123456,45.3,23.5,65.2,128
```

### Campos

- `timestamp`: ISO 8601 format
- `bed_temp`: Temperatura da mesa (°C)
- `amb_temp`: Temperatura ambiente (°C)
- `humidity`: Umidade relativa (%)
- `fan_speed`: PWM do ventilador (0-255)

## 🚀 Extensões Futuras

### Scripts Planejados

- `pid_tuner.py`: Auto-tuning de parâmetros PID
- `send_commands.py`: Envio de comandos via serial (quando implementado no firmware)
- `live_dashboard.py`: Dashboard web em tempo real (Flask/Dash)
- `export_report.py`: Gerador de relatórios PDF

### Integrações

- **InfluxDB**: Para séries temporais
- **Grafana**: Dashboard profissional
- **Home Assistant**: Integração domótica
- **Telegram Bot**: Notificações remotas

## 📚 Documentação Relacionada

- [API e Protocolos](../../docs/API.md)
- [Guia de Testes](../../test/README.md)
- [Hardware](../../hardware/README.md)

## 🤝 Contribuindo

Ao adicionar novos scripts:

1. Documente uso e opções
2. Adicione exemplo neste README
3. Inclua tratamento de erros
4. Siga PEP 8 (use `black` para formatação)
5. Adicione shebang `#!/usr/bin/env python3`

## 📄 Licença

Mesmo que o projeto principal (ver LICENSE na raiz).

