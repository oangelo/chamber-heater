# Hardware - Chamber Heater

## 📦 Lista de Componentes (BOM)

| Componente | Quantidade | Especificação | Observações |
|------------|------------|---------------|-------------|
| Arduino Mega 2560 | 1 | ou MKS GEN v1.4 | Microcontrolador principal |
| DHT22 | 1 | Sensor temperatura/umidade | Faixa: -40°C a 80°C, 0-100% UR |
| Termistor NTC | 1 | 100kΩ @ 25°C, B=3950K | Para mesa aquecida |
| Resistor | 1 | 4.7kΩ | Pull-up do termistor |
| Display LCD | 1 | 20x4 caracteres, HD44780 | Interface paralela 4-bit |
| Encoder Rotativo | 1 | Com botão integrado | Para interface de usuário |
| Ventilador | 1 | 12V DC, compatível com PWM | Para controle de temperatura |
| Buzzer | 1 | 5V ativo | Notificações sonoras |
| Mesa Aquecida | 1 | 12V (potência adequada) | Elemento de aquecimento |
| MOSFET/Relé SSR | 1 | Para controle da mesa | Adequado à corrente da mesa |
| Fonte de Alimentação | 1 | 12V, corrente adequada | Para mesa e ventilador |

## 🔌 Pinagem do Arduino Mega

### Sensores

| Pino Arduino | Conexão | Descrição |
|--------------|---------|-----------|
| 4 | DHT22 Data | Sensor de temperatura e umidade |
| A0 (14) | Termistor | Leitura analógica da temperatura da mesa |
| 5V | DHT22 VCC | Alimentação sensor |
| GND | DHT22 GND | Terra sensor |

### Atuadores

| Pino Arduino | Conexão | Descrição |
|--------------|---------|-----------|
| 8 | Mesa Aquecida | Controle via MOSFET/SSR |
| 9 | Ventilador PWM | Controle de velocidade do ventilador |
| 37 | Buzzer | Notificações sonoras |

### Display LCD 20x4

| Pino Arduino | Pino LCD | Descrição |
|--------------|----------|-----------|
| 16 | RS | Register Select |
| 17 | EN | Enable |
| 23 | D4 | Data bit 4 |
| 25 | D5 | Data bit 5 |
| 27 | D6 | Data bit 6 |
| 29 | D7 | Data bit 7 |
| 5V | VCC | Alimentação |
| GND | GND | Terra |
| - | V0 | Contraste (potenciômetro 10kΩ) |
| - | A | Backlight + (via resistor) |
| - | K | Backlight - (GND) |

### Interface do Usuário (Encoder)

| Pino Arduino | Conexão | Descrição |
|--------------|---------|-----------|
| 31 | Encoder A | Sinal A do encoder |
| 33 | Encoder B | Sinal B do encoder |
| 35 | Botão | Botão integrado ao encoder |
| GND | Encoder GND | Terra comum |

## ⚡ Consumo de Energia

- **Arduino Mega**: ~100mA @ 5V (operação normal)
- **Display LCD + Backlight**: ~150mA @ 5V
- **DHT22**: ~2.5mA @ 5V (medição)
- **Ventilador**: Até 500mA @ 12V (dependendo do modelo)
- **Mesa Aquecida**: Varia conforme potência (verificar especificações)
- **Total 5V**: ~250mA
- **Total 12V**: Depende da mesa e ventilador

**⚠️ Importante**: Dimensionar fonte adequadamente para a potência da mesa aquecida!

## 🔧 Esquema de Conexão do Termistor

```
VCC (5V) ---- [Resistor 4.7kΩ] ---- A0 (pino analógico)
                                      |
                                      |---- [Termistor NTC 100kΩ] ---- GND
```

- **RT0**: 100000Ω (resistência a 25°C)
- **B**: 3950K (constante do termistor)
- **R**: 4700Ω (resistor pull-up)

## 🔌 Conexão da Mesa Aquecida

```
Arduino Pino 8 → Gate do MOSFET
                 Drain do MOSFET → Mesa Aquecida (-)
                 Source do MOSFET → GND
                 
12V+ → Mesa Aquecida (+)
```

**⚠️ Atenção**: Use MOSFET adequado para a corrente da mesa (recomendado: IRLZ44N ou similar)

## 📸 Fotos da Montagem

*Adicionar fotos da montagem real em `hardware/images/`*

## 📄 Arquivos de Documentação

- [Esquemático MKS GEN v1.4](../docs/hardware/MKS%20GEN%20V1.4_004%20SCH.pdf)
- [Pinagem MKS GEN v1.4](../docs/hardware/MKS%20GEN%20V1.4_004%20PIN.pdf)
- [BOM MKS GEN v1.4](../docs/hardware/MKS%20GEN%20V1.4_004%20BOM.pdf)
- [Layout PCB TOP](../docs/hardware/MKS%20GEN%20V1.4_004%20TOP.pdf)
- [Layout PCB BOTTOM](../docs/hardware/MKS%20GEN%20V1.4_004%20BOTTOM.pdf)

## ⚠️ Considerações de Segurança

1. **Mesa Aquecida**: Pode atingir altas temperaturas. Usar em superfície resistente ao calor.
2. **Ventilação**: Garantir ventilação adequada para dissipar calor.
3. **Corrente**: Verificar capacidade dos fios para a corrente da mesa aquecida.
4. **Isolamento**: Garantir isolamento adequado das conexões de alta corrente.
5. **Proteção**: Considerar fusível de proteção na alimentação da mesa.

## 🔄 Versões de Hardware

### v1.0 (Atual)
- Baseado em Arduino Mega ou MKS GEN v1.4
- Display LCD 20x4
- Controle via encoder rotativo
- PID implementado em software

### Melhorias Futuras
- [ ] Adicionar cartão SD para logging
- [ ] Interface Ethernet/WiFi para monitoramento remoto
- [ ] Sensor adicional de temperatura ambiente
- [ ] Display touch screen
- [ ] PCB customizado integrado

