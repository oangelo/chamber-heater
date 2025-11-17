# Troubleshooting - Chamber Heater

## 🔍 Problemas Comuns e Soluções

### 🖥️ Display LCD

#### LCD não liga / sem backlight

**Sintomas**: Display completamente apagado

**Causas possíveis**:
- Falta de alimentação
- Conexões soltas

**Soluções**:
1. ✅ Verificar alimentação 5V e GND
2. ✅ Verificar conexão do backlight (pinos A e K)
3. ✅ Verificar resistor limitador do backlight (220Ω)
4. ✅ Medir tensão nos pinos VDD e VSS do LCD

#### LCD liga mas não mostra caracteres

**Sintomas**: Backlight aceso, tela branca ou escura, sem texto

**Causas possíveis**:
- Contraste mal ajustado
- Conexões de dados incorretas

**Soluções**:
1. ✅ **Ajustar potenciômetro de contraste** (primeiro teste!)
   - Girar completamente em ambas direções
   - Procurar posição onde aparecem caracteres
2. ✅ Verificar conexões dos pinos RS, E, D4-D7
3. ✅ Verificar se está em modo 4-bit (RW no GND)
4. ✅ Upload do código novamente

#### LCD mostra caracteres estranhos/aleatórios

**Sintomas**: Símbolos errados, lixo na tela

**Causas possíveis**:
- Conexões ruins
- Interferência eletromagnética
- Velocidade de comunicação

**Soluções**:
1. ✅ Reaperte todas as conexões do LCD
2. ✅ Use cabos mais curtos (< 15cm idealmente)
3. ✅ Afaste o LCD de fontes de ruído (motores, fontes chaveadas)
4. ✅ Adicione capacitor 100nF entre VDD e GND do LCD
5. ✅ Reset o Arduino (botão ou desligar/ligar)

### 🎛️ Encoder Rotativo

#### Encoder não responde

**Sintomas**: Girar o encoder não muda nada

**Causas possíveis**:
- Pinos trocados
- Encoder com defeito
- Falta de pull-up

**Soluções**:
1. ✅ Verificar conexões nos pinos 31 e 33
2. ✅ Testar continuidade com multímetro
3. ✅ Trocar pinos A e B (inverter 31 ↔ 33)
4. ✅ Adicionar resistores pull-up 10kΩ (se necessário)
5. ✅ Teste com código simples:
   ```cpp
   void loop() {
     encoder.tick();
     Serial.println(encoder.getPosition());
     delay(100);
   }
   ```

#### Encoder conta errado (pula valores)

**Sintomas**: Ao girar 1 clique, conta 2 ou 0

**Causas possíveis**:
- Modo de latch incorreto
- Bouncing excessivo

**Soluções**:
1. ✅ Mudar modo de latch no código:
   ```cpp
   // Testar diferentes modos
   RotaryEncoder encoder(BTN_EN1, BTN_EN2, RotaryEncoder::LatchMode::TWO03);
   // ou
   RotaryEncoder encoder(BTN_EN1, BTN_EN2, RotaryEncoder::LatchMode::FOUR3);
   ```
2. ✅ Adicionar capacitores 100nF nos pinos do encoder

#### Botão do encoder não funciona

**Sintomas**: Pressionar botão não abre menu

**Causas possíveis**:
- Conexão solta no pin 35
- Botão com defeito

**Soluções**:
1. ✅ Verificar conexão no pin 35
2. ✅ Testar botão com multímetro (continuidade ao pressionar)
3. ✅ Verificar se pin 35 está correto no código
4. ✅ Teste simples:
   ```cpp
   void loop() {
     if (button.getSingleDebouncedPress()) {
       Serial.println("Botão pressionado!");
     }
   }
   ```

### 🌡️ Sensor DHT22

#### Leitura retorna NaN

**Sintomas**: Display mostra "nan" ou valores inválidos

**Causas possíveis**:
- Sensor não inicializado
- Conexão ruim
- Tempo insuficiente de estabilização

**Soluções**:
1. ✅ **Aguardar 2 segundos** após ligar antes da primeira leitura
2. ✅ Verificar conexão no pin 4
3. ✅ Verificar alimentação 5V do DHT22
4. ✅ Adicionar resistor pull-up 10kΩ (DATA → VCC)
5. ✅ Aumentar intervalo de leitura:
   ```cpp
   const long interval = 3000; // ao invés de 2000
   ```
6. ✅ Testar com código simples:
   ```cpp
   void loop() {
     float t = dht.readTemperature();
     float h = dht.readHumidity();
     Serial.print("Temp: "); Serial.print(t);
     Serial.print(" Umid: "); Serial.println(h);
     delay(2000);
   }
   ```

#### Leituras congeladas/não mudam

**Sintomas**: Valores não atualizam

**Causas possíveis**:
- Sensor travado
- Código não está lendo

**Soluções**:
1. ✅ Desligar e ligar o sistema
2. ✅ Verificar se `dht.begin()` está no `setup()`
3. ✅ Trocar o sensor DHT22

#### Valores muito errados

**Sintomas**: Temperatura ou umidade absurdas

**Causas possíveis**:
- Sensor com defeito
- Tipo de sensor errado no código

**Soluções**:
1. ✅ Verificar se `#define DHT_TYPE DHT22` está correto
2. ✅ Se tiver DHT11, mudar para `#define DHT_TYPE DHT11`
3. ✅ Testar sensor em outro projeto
4. ✅ Trocar o sensor

### 🔥 Termistor (Mesa)

#### Temperatura da mesa sempre em 0°C ou -273°C

**Sintomas**: Leitura inválida, muito baixa

**Causas possíveis**:
- Termistor não conectado
- Resistor pull-up faltando
- Pino analógico errado

**Soluções**:
1. ✅ Verificar conexão do termistor
2. ✅ Verificar resistor 4.7kΩ entre 5V e A0
3. ✅ Verificar pino A0 (pin 14) no código
4. ✅ Medir tensão em A0 (deve estar entre 0.5V e 4.5V)

#### Temperatura absurdamente alta (>200°C)

**Sintomas**: Leitura muito alta sem aquecimento

**Causas possíveis**:
- Curto-circuito no termistor
- Resistor pull-up errado

**Soluções**:
1. ✅ Verificar valor do resistor (deve ser 4.7kΩ)
2. ✅ Verificar constantes no código:
   ```cpp
   #define RT0 100000  // 100kΩ
   #define B 3950      // Conferir datasheet
   #define R 4700      // Valor do resistor
   ```
3. ✅ Testar termistor com multímetro (deve ser ~100kΩ a 25°C)

#### Temperatura oscila muito

**Sintomas**: Valores pulam +/- 5°C rapidamente

**Causas possíveis**:
- Ruído no ADC
- Cabos longos

**Soluções**:
1. ✅ Adicionar capacitor 100nF em paralelo com termistor
2. ✅ Usar cabos blindados ou mais curtos
3. ✅ Implementar filtro de média móvel:
   ```cpp
   float media = 0;
   for(int i = 0; i < 10; i++) {
     media += readBedTemp();
     delay(10);
   }
   media = media / 10;
   ```

### 💨 Ventilador

#### Ventilador não gira

**Sintomas**: Sem rotação, mesmo com setpoint ajustado

**Causas possíveis**:
- MOSFET não conduzindo
- Fonte 12V desconectada
- PWM muito baixo

**Soluções**:
1. ✅ Verificar alimentação 12V do ventilador
2. ✅ Verificar MOSFET (gate, source, drain)
3. ✅ Testar PWM diretamente:
   ```cpp
   void loop() {
     analogWrite(FAN_PIN, 255); // Máximo
     delay(2000);
     analogWrite(FAN_PIN, 0);   // Desliga
     delay(2000);
   }
   ```
4. ✅ Verificar se gate do MOSFET está conectado ao pin 9
5. ✅ Adicionar resistor 1kΩ entre pin 9 e gate
6. ✅ Verificar se Source está no GND

#### Ventilador sempre na máxima velocidade

**Sintomas**: Não varia velocidade, sempre 100%

**Causas possíveis**:
- PID não está funcionando
- Setpoint muito distante

**Soluções**:
1. ✅ Verificar setpoint de umidade (deve ser razoável)
2. ✅ Verificar se PID está habilitado:
   ```cpp
   void setup() {
     fanPID.SetMode(AUTOMATIC);
   }
   ```
3. ✅ Ajustar limites do PID:
   ```cpp
   fanPID.SetOutputLimits(0, 255);
   ```

#### Ventilador gira mas não controla umidade

**Sintomas**: Umidade não muda com ventilador ligado

**Causas possíveis**:
- Ambiente muito seco/úmido
- Ventilador posicionado errado
- PID mal ajustado

**Soluções**:
1. ✅ Verificar posicionamento do ventilador
2. ✅ Ajustar constantes PID (aumentar Kp)
3. ✅ Verificar se há fonte de umidade/secagem no ambiente
4. ✅ Considerar adicionar umidificador ou desumidificador

### 🔥 Mesa Aquecida

⚠️ **PERIGO**: Mesa pode causar queimaduras! Cuidado!

#### Mesa não aquece

**Sintomas**: Temperatura não sobe mesmo com setpoint alto

**Causas possíveis**:
- MOSFET não conduzindo
- Mesa desconectada
- Fonte inadequada

**Soluções**:
1. ✅ Verificar alimentação 12V da mesa
2. ✅ Verificar MOSFET (gate no pin 8)
3. ✅ Medir tensão no gate (deve ser ~5V quando ativo)
4. ✅ Verificar corrente da fonte (deve suportar a mesa)
5. ✅ Teste direto (CUIDADO!):
   ```cpp
   void loop() {
     digitalWrite(bed_OUTPUT_PIN, HIGH);
     delay(5000);
     digitalWrite(bed_OUTPUT_PIN, LOW);
     delay(5000);
   }
   ```

#### Mesa aquece demais / temperatura instável

**Sintomas**: Ultrapassa setpoint, oscila muito

**Causas possíveis**:
- PID mal ajustado
- Inércia térmica alta
- Termistor mal posicionado

**Soluções**:
1. ✅ Reduzir Kp do PID
2. ✅ Verificar se termistor está bem acoplado à mesa
3. ✅ Aumentar intervalo do loop (mais tempo para estabilizar)
4. ✅ Implementar limite de segurança:
   ```cpp
   void calcBedPID() {
     bed_INPUT = readBedTemp();
     if (bed_INPUT > 100) { // Limite de segurança
       bed_OUTPUT = 0;
       digitalWrite(BUZZER_PIN, HIGH); // Alarme
     } else {
       bedPID.Compute();
       analogWrite(bed_OUTPUT_PIN, bed_OUTPUT);
     }
   }
   ```

### 💻 Compilação e Upload

#### Erro: "library not found"

**Sintomas**: Compilação falha com erro de biblioteca

**Soluções**:
1. ✅ Instalar bibliotecas faltantes via Library Manager
2. ✅ Verificar se todas as libs estão instaladas (ver [INSTALACAO.md](./INSTALACAO.md))
3. ✅ Reiniciar Arduino IDE

#### Erro: "stk500v2_ReceiveMessage(): timeout"

**Sintomas**: Upload falha com timeout

**Soluções**:
1. ✅ Verificar porta serial correta
2. ✅ Verificar se outra aplicação está usando a porta (fechar Serial Monitor)
3. ✅ Trocar cabo USB
4. ✅ Pressionar reset no Arduino antes do upload
5. ✅ Verificar driver USB (CH340 ou FTDI)

#### Código compila mas Arduino não funciona

**Sintomas**: Upload OK mas nada acontece

**Soluções**:
1. ✅ Abrir Serial Monitor e verificar erros
2. ✅ Verificar se bootloader está OK (testar com Blink)
3. ✅ Verificar alimentação adequada
4. ✅ Upload novamente

### 📡 Serial Monitor

#### Nada aparece no Serial Monitor

**Sintomas**: Monitor serial vazio

**Soluções**:
1. ✅ Verificar baud rate (deve ser 115200)
2. ✅ Verificar porta serial correta
3. ✅ Aguardar inicialização (2-3 segundos)
4. ✅ Pressionar reset no Arduino

#### Caracteres estranhos no Serial

**Sintomas**: Lixo, símbolos aleatórios

**Soluções**:
1. ✅ Ajustar baud rate para 115200
2. ✅ Verificar se não há conflito com pinos TX/RX

## 🆘 Diagnóstico Geral

### Checklist de Debug

1. **Alimentação**
   - [ ] 5V presente nos componentes
   - [ ] 12V presente em ventilador/mesa
   - [ ] GND comum conectado

2. **Conexões**
   - [ ] Todos os cabos firmes
   - [ ] Sem cabos trocados
   - [ ] Sem curtos-circuitos

3. **Software**
   - [ ] Bibliotecas instaladas
   - [ ] Código compilado sem erros
   - [ ] Upload bem sucedido

4. **Funcional**
   - [ ] Display mostra algo
   - [ ] Encoder responde
   - [ ] Sensores leem valores
   - [ ] Atuadores funcionam

### Teste Isolado de Componentes

Se tudo falhar, teste cada componente separadamente:

**Display LCD**:
```cpp
#include <LiquidCrystal.h>
LiquidCrystal lcd(16, 17, 23, 25, 27, 29);
void setup() { lcd.begin(20, 4); lcd.print("Teste"); }
void loop() {}
```

**DHT22**:
```cpp
#include <DHT.h>
DHT dht(4, DHT22);
void setup() { Serial.begin(115200); dht.begin(); }
void loop() { Serial.println(dht.readTemperature()); delay(2000); }
```

## 📞 Quando Pedir Ajuda

Se após todas as verificações o problema persistir:

1. 📸 Tire fotos da montagem
2. 📋 Descreva o problema detalhadamente
3. 📝 Inclua mensagens de erro
4. 🔍 Mencione o que já tentou
5. 💬 Abra uma issue no GitHub

## 🔗 Links Úteis

- [Datasheet DHT22](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf)
- [Datasheet LCD HD44780](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)
- [Tutorial PID Arduino](https://playground.arduino.cc/Code/PIDLibrary/)
- [Forum Arduino](https://forum.arduino.cc/)

