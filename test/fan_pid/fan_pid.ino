#include <PID_v1_bc.h> 
#include <DHT.h>

#define FAN_PIN 9    
#define DHT_PIN 4   
#define DHT_TYPE DHT22 

DHT dht(DHT_PIN, DHT_TYPE);

double fan_set_point, fan_INPUT, fan_OUTPUT;

const int Kp = 15, Ki = 1, Kd = 15;

PID fanPID(&fan_INPUT, &fan_OUTPUT, &fan_set_point, Kp, Ki, Kd, REVERSE);

void setup()
{
  dht.begin();
  Serial.begin(9600);
  Serial.println("Iniciando");
  float Umi = dht.readHumidity();
  fan_INPUT = Umi;
  fan_set_point = 10;

  fanPID.SetMode(AUTOMATIC);
  fanPID.SetOutputLimits(0,255);
}

void loop()
{
  float Umi = dht.readHumidity();
  fan_INPUT = Umi;
  fanPID.Compute();
  analogWrite(FAN_PIN, fan_OUTPUT);
  Serial.print("fan_INPUT: ");
  Serial.print(Umi);
  Serial.print(" | PID: ");
  Serial.print(map(fan_OUTPUT, 0, 255, 0, 100));
  Serial.print(" | Output: ");
  Serial.println(fan_OUTPUT);
}
