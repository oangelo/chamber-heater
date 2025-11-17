#!/usr/bin/env python3
"""
Serial Logger - Chamber Heater

Captura dados da porta serial e salva em arquivo CSV com timestamp.

Uso:
    python serial_logger.py --port /dev/ttyUSB0 --output data.csv
"""

import serial
import csv
import argparse
from datetime import datetime
import sys
import time


def main():
    parser = argparse.ArgumentParser(description='Logger de dados do Chamber Heater')
    parser.add_argument('--port', '-p', default='/dev/ttyUSB0',
                        help='Porta serial (padrão: /dev/ttyUSB0)')
    parser.add_argument('--baud', '-b', type=int, default=115200,
                        help='Baudrate (padrão: 115200)')
    parser.add_argument('--output', '-o', default='chamber_log.csv',
                        help='Arquivo de saída (padrão: chamber_log.csv)')
    parser.add_argument('--verbose', '-v', action='store_true',
                        help='Modo verboso')
    
    args = parser.parse_args()
    
    print(f"🔌 Conectando à porta {args.port} ({args.baud} bps)...")
    
    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        time.sleep(2)  # Aguarda inicialização
        print("✅ Conectado com sucesso!")
        
    except serial.SerialException as e:
        print(f"❌ Erro ao abrir porta serial: {e}")
        sys.exit(1)
    
    print(f"📝 Salvando dados em: {args.output}")
    print("Press Ctrl+C para parar.\n")
    
    with open(args.output, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['timestamp', 'bed_temp', 'amb_temp', 'humidity', 'fan_speed'])
        
        sample_count = 0
        
        try:
            while True:
                line = ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line:
                    data = line.split(',')
                    
                    if len(data) == 4:
                        timestamp = datetime.now().isoformat()
                        row = [timestamp] + data
                        writer.writerow(row)
                        f.flush()  # Garante gravação imediata
                        
                        sample_count += 1
                        
                        if args.verbose or sample_count % 10 == 0:
                            bed_temp, amb_temp, humidity, fan_speed = data
                            print(f"[{sample_count:04d}] Mesa: {bed_temp}°C | "
                                  f"Ambiente: {amb_temp}°C | "
                                  f"Umidade: {humidity}% | "
                                  f"Fan: {fan_speed}")
                    else:
                        if args.verbose:
                            print(f"⚠️  Linha inválida: {line}")
                            
        except KeyboardInterrupt:
            print(f"\n\n✅ Logging encerrado. {sample_count} amostras salvas em {args.output}")
            
        finally:
            ser.close()


if __name__ == '__main__':
    main()

