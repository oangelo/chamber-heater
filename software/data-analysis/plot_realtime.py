#!/usr/bin/env python3
"""
Real-time Plotter - Chamber Heater

Plota dados de temperatura e umidade em tempo real da porta serial.

Uso:
    python plot_realtime.py --port /dev/ttyUSB0

Dependências:
    pip install pyserial matplotlib
"""

import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from collections import deque
import argparse
import sys


class RealtimePlotter:
    def __init__(self, port, baudrate=115200, max_points=100):
        self.port = port
        self.baudrate = baudrate
        self.max_points = max_points
        
        # Buffers de dados
        self.times = deque(maxlen=max_points)
        self.bed_temps = deque(maxlen=max_points)
        self.amb_temps = deque(maxlen=max_points)
        self.humidities = deque(maxlen=max_points)
        self.fan_speeds = deque(maxlen=max_points)
        
        self.frame_count = 0
        
        # Conectar serial
        try:
            self.ser = serial.Serial(port, baudrate, timeout=1)
            print(f"✅ Conectado a {port}")
        except serial.SerialException as e:
            print(f"❌ Erro ao abrir porta: {e}")
            sys.exit(1)
        
        # Configurar plots
        self.fig, (self.ax1, self.ax2, self.ax3) = plt.subplots(3, 1, figsize=(12, 10))
        self.fig.suptitle('Chamber Heater - Monitoramento em Tempo Real', fontsize=16)
        
    def update(self, frame):
        """Atualiza os gráficos com novos dados"""
        try:
            line = self.ser.readline().decode('utf-8', errors='ignore').strip()
            
            if line:
                data = line.split(',')
                
                if len(data) == 4:
                    bed_temp = float(data[0])
                    amb_temp = float(data[1])
                    humidity = float(data[2])
                    fan_speed = float(data[3])
                    
                    self.times.append(self.frame_count)
                    self.bed_temps.append(bed_temp)
                    self.amb_temps.append(amb_temp)
                    self.humidities.append(humidity)
                    self.fan_speeds.append(fan_speed)
                    
                    self.frame_count += 1
                    
                    # Atualizar gráfico de temperaturas
                    self.ax1.clear()
                    self.ax1.plot(list(self.times), list(self.bed_temps), 
                                 label='Mesa', color='red', linewidth=2)
                    self.ax1.plot(list(self.times), list(self.amb_temps), 
                                 label='Ambiente', color='blue', linewidth=2)
                    self.ax1.set_ylabel('Temperatura (°C)', fontsize=12)
                    self.ax1.legend(loc='upper left')
                    self.ax1.grid(True, alpha=0.3)
                    self.ax1.set_title(f'Atual: Mesa={bed_temp:.1f}°C | Ambiente={amb_temp:.1f}°C')
                    
                    # Atualizar gráfico de umidade
                    self.ax2.clear()
                    self.ax2.plot(list(self.times), list(self.humidities), 
                                 label='Umidade', color='green', linewidth=2)
                    self.ax2.set_ylabel('Umidade (%)', fontsize=12)
                    self.ax2.legend(loc='upper left')
                    self.ax2.grid(True, alpha=0.3)
                    self.ax2.set_title(f'Atual: {humidity:.1f}%')
                    
                    # Atualizar gráfico de velocidade do ventilador
                    self.ax3.clear()
                    self.ax3.plot(list(self.times), list(self.fan_speeds), 
                                 label='Velocidade Fan', color='orange', linewidth=2)
                    self.ax3.set_ylabel('PWM (0-255)', fontsize=12)
                    self.ax3.set_xlabel('Tempo (amostras)', fontsize=12)
                    self.ax3.legend(loc='upper left')
                    self.ax3.grid(True, alpha=0.3)
                    self.ax3.set_title(f'Atual: {int(fan_speed)} PWM')
                    
        except ValueError:
            pass  # Ignora valores inválidos
        except Exception as e:
            print(f"⚠️  Erro: {e}")
    
    def run(self):
        """Inicia a animação"""
        ani = animation.FuncAnimation(
            self.fig, 
            self.update, 
            interval=2000,  # 2 segundos
            cache_frame_data=False
        )
        
        plt.tight_layout()
        plt.show()
        
        self.ser.close()


def main():
    parser = argparse.ArgumentParser(description='Plot em tempo real do Chamber Heater')
    parser.add_argument('--port', '-p', default='/dev/ttyUSB0',
                        help='Porta serial (padrão: /dev/ttyUSB0)')
    parser.add_argument('--baud', '-b', type=int, default=115200,
                        help='Baudrate (padrão: 115200)')
    parser.add_argument('--points', '-n', type=int, default=100,
                        help='Número máximo de pontos no gráfico (padrão: 100)')
    
    args = parser.parse_args()
    
    plotter = RealtimePlotter(args.port, args.baud, args.points)
    plotter.run()


if __name__ == '__main__':
    main()

