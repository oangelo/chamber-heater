#!/usr/bin/env python3
"""
Log Analyzer - Chamber Heater

Analisa arquivos CSV gerados pelo serial_logger.py e gera gráficos e estatísticas.

Uso:
    python analyze_log.py data.csv --output report.png
"""

import pandas as pd
import matplotlib.pyplot as plt
import argparse
import sys
from datetime import datetime


def load_data(filename):
    """Carrega dados do CSV"""
    try:
        df = pd.read_csv(filename)
        df['timestamp'] = pd.to_datetime(df['timestamp'])
        return df
    except FileNotFoundError:
        print(f"❌ Arquivo não encontrado: {filename}")
        sys.exit(1)
    except Exception as e:
        print(f"❌ Erro ao carregar arquivo: {e}")
        sys.exit(1)


def print_statistics(df):
    """Imprime estatísticas dos dados"""
    print("\n" + "="*60)
    print("📊 ESTATÍSTICAS DO LOG")
    print("="*60)
    
    print(f"\n⏱️  Período:")
    print(f"   Início: {df['timestamp'].iloc[0]}")
    print(f"   Fim:    {df['timestamp'].iloc[-1]}")
    duration = df['timestamp'].iloc[-1] - df['timestamp'].iloc[0]
    print(f"   Duração: {duration}")
    print(f"   Amostras: {len(df)}")
    
    print(f"\n🌡️  Temperatura da Mesa (°C):")
    print(f"   Média:   {df['bed_temp'].mean():.2f}")
    print(f"   Mínima:  {df['bed_temp'].min():.2f}")
    print(f"   Máxima:  {df['bed_temp'].max():.2f}")
    print(f"   Desvio:  {df['bed_temp'].std():.2f}")
    
    print(f"\n🌡️  Temperatura Ambiente (°C):")
    print(f"   Média:   {df['amb_temp'].mean():.2f}")
    print(f"   Mínima:  {df['amb_temp'].min():.2f}")
    print(f"   Máxima:  {df['amb_temp'].max():.2f}")
    print(f"   Desvio:  {df['amb_temp'].std():.2f}")
    
    print(f"\n💧 Umidade (%):")
    print(f"   Média:   {df['humidity'].mean():.2f}")
    print(f"   Mínima:  {df['humidity'].min():.2f}")
    print(f"   Máxima:  {df['humidity'].max():.2f}")
    print(f"   Desvio:  {df['humidity'].std():.2f}")
    
    print(f"\n💨 Ventilador (PWM):")
    print(f"   Média:   {df['fan_speed'].mean():.1f}")
    print(f"   Mínima:  {df['fan_speed'].min():.0f}")
    print(f"   Máxima:  {df['fan_speed'].max():.0f}")
    
    print("\n" + "="*60 + "\n")


def plot_data(df, output_file):
    """Gera gráficos dos dados"""
    fig, axes = plt.subplots(4, 1, figsize=(14, 12))
    
    # Criar eixo X relativo (minutos desde o início)
    time_minutes = (df['timestamp'] - df['timestamp'].iloc[0]).dt.total_seconds() / 60
    
    # Gráfico 1: Temperaturas
    axes[0].plot(time_minutes, df['bed_temp'], label='Mesa', color='red', linewidth=1.5)
    axes[0].plot(time_minutes, df['amb_temp'], label='Ambiente', color='blue', linewidth=1.5)
    axes[0].set_ylabel('Temperatura (°C)', fontsize=11)
    axes[0].set_title('Temperaturas ao Longo do Tempo', fontsize=13, fontweight='bold')
    axes[0].legend(loc='best')
    axes[0].grid(True, alpha=0.3)
    
    # Gráfico 2: Umidade
    axes[1].plot(time_minutes, df['humidity'], label='Umidade', color='green', linewidth=1.5)
    axes[1].set_ylabel('Umidade (%)', fontsize=11)
    axes[1].set_title('Umidade ao Longo do Tempo', fontsize=13, fontweight='bold')
    axes[1].legend(loc='best')
    axes[1].grid(True, alpha=0.3)
    
    # Gráfico 3: Velocidade do Ventilador
    axes[2].plot(time_minutes, df['fan_speed'], label='Fan Speed', color='orange', linewidth=1.5)
    axes[2].set_ylabel('PWM (0-255)', fontsize=11)
    axes[2].set_title('Velocidade do Ventilador', fontsize=13, fontweight='bold')
    axes[2].legend(loc='best')
    axes[2].grid(True, alpha=0.3)
    
    # Gráfico 4: Correlação Umidade vs Fan Speed
    axes[3].scatter(df['humidity'], df['fan_speed'], alpha=0.5, s=10, color='purple')
    axes[3].set_xlabel('Umidade (%)', fontsize=11)
    axes[3].set_ylabel('Fan Speed (PWM)', fontsize=11)
    axes[3].set_title('Correlação: Umidade vs Velocidade do Ventilador', fontsize=13, fontweight='bold')
    axes[3].grid(True, alpha=0.3)
    
    # Ajustar layout
    plt.tight_layout()
    
    # Salvar figura
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    print(f"✅ Gráfico salvo em: {output_file}")
    
    # Mostrar (opcional)
    # plt.show()


def main():
    parser = argparse.ArgumentParser(description='Analisador de logs do Chamber Heater')
    parser.add_argument('input', help='Arquivo CSV de entrada')
    parser.add_argument('--output', '-o', default='analysis.png',
                        help='Arquivo de saída para gráfico (padrão: analysis.png)')
    parser.add_argument('--no-plot', action='store_true',
                        help='Não gerar gráfico, apenas estatísticas')
    
    args = parser.parse_args()
    
    # Carregar dados
    print(f"📂 Carregando dados de: {args.input}")
    df = load_data(args.input)
    
    # Mostrar estatísticas
    print_statistics(df)
    
    # Gerar gráficos
    if not args.no_plot:
        print(f"📈 Gerando gráficos...")
        plot_data(df, args.output)


if __name__ == '__main__':
    main()

