/**
 * config.example.h
 * 
 * Arquivo de configuração exemplo para o Chamber Heater
 * 
 * INSTRUÇÕES:
 * 1. Copie este arquivo para "config.h": cp config.example.h config.h
 * 2. Edite "config.h" com suas configurações específicas
 * 3. NUNCA commite o arquivo "config.h" (está no .gitignore)
 * 
 * NOTA: Esta funcionalidade será implementada em versão futura.
 * Por enquanto, ajuste os valores diretamente em firmware.ino
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// DEFINIÇÕES DE PINOS
// ============================================================================

// Sensores
#define DHT_PIN 4              // Pino do sensor DHT22
#define DHT_TYPE DHT22         // Tipo do sensor DHT
#define BED_INPUT_PIN 14       // Pino analógico do termistor (A0)

// Atuadores
#define FAN_PIN 9              // Pino PWM do ventilador
#define BED_OUTPUT_PIN 8       // Pino de controle da mesa aquecida
#define BUZZER_PIN 37          // Pino do buzzer

// Display LCD 20x4
#define LCD_RS 16
#define LCD_EN 17
#define LCD_D4 23
#define LCD_D5 25
#define LCD_D6 27
#define LCD_D7 29
#define LCD_COLS 20
#define LCD_ROWS 4

// Encoder Rotativo
#define BTN_EN1 31             // Encoder pino A
#define BTN_EN2 33             // Encoder pino B
#define BTN_ENC 35             // Botão do encoder

// ============================================================================
// CONFIGURAÇÕES DO TERMISTOR
// ============================================================================

#define RT0 100000             // Resistência nominal a 25°C (100kΩ)
#define B 3950                 // Constante B do termistor (K)
#define VCC 5                  // Tensão de alimentação (V)
#define R 4700                 // Resistor pull-up (Ω)

// ============================================================================
// CONFIGURAÇÕES DO PID - MESA AQUECIDA
// ============================================================================

#define BED_PID_KP 15.0        // Ganho proporcional
#define BED_PID_KI 1.0         // Ganho integral
#define BED_PID_KD 15.0        // Ganho derivativo
#define BED_PID_MIN 0          // Saída mínima do PID
#define BED_PID_MAX 255        // Saída máxima do PID

// Setpoint padrão da mesa (°C)
#define BED_SETPOINT_DEFAULT 60.0

// ============================================================================
// CONFIGURAÇÕES DO PID - VENTILADOR
// ============================================================================

#define FAN_PID_KP 15.0        // Ganho proporcional
#define FAN_PID_KI 1.0         // Ganho integral
#define FAN_PID_KD 15.0        // Ganho derivativo
#define FAN_PID_MIN 0          // Saída mínima do PID
#define FAN_PID_MAX 255        // Saída máxima do PID

// Setpoint padrão de umidade (%)
#define HUMIDITY_SETPOINT_DEFAULT 50.0

// ============================================================================
// CONFIGURAÇÕES DE TIMING
// ============================================================================

#define LOOP_INTERVAL 2000     // Intervalo do loop principal (ms)
#define DHT_READ_DELAY 2000    // Delay mínimo entre leituras DHT (ms)

// ============================================================================
// CONFIGURAÇÕES DE SEGURANÇA
// ============================================================================

#define BED_MAX_TEMP 100.0     // Temperatura máxima permitida (°C)
#define BED_EMERGENCY_SHUTOFF 110.0  // Temperatura de emergência (°C)

// ============================================================================
// CONFIGURAÇÕES DE SERIAL
// ============================================================================

#define SERIAL_BAUD 115200     // Velocidade da comunicação serial
#define SERIAL_OUTPUT_ENABLED true  // Habilitar saída serial

// ============================================================================
// CONFIGURAÇÕES DE DEBUG
// ============================================================================

#define DEBUG_ENABLED false    // Habilitar mensagens de debug
#define DEBUG_PID false        // Debug específico do PID

#endif // CONFIG_H

