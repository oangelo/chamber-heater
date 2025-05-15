#include <PID_v1_bc.h> 
#include <DHT.h>

#define FAN_PIN 9    
#define DHT_PIN 4   
#define DHT_TYPE DHT22 

DHT dht(DHT_PIN, DHT_TYPE);

double Umi_set, Umidade, PWM;

const int Kp = 15, Ki = 1, Kd = 15;

PID myPID(&Umidade, &PWM, &Umi_set, Kp, Ki, Kd, REVERSE);

void setup()
{
  dht.begin();
  Serial.begin(9600);
  Serial.println("Iniciando");
  float Umi = dht.readHumidity();
  Umidade = Umi;
  Umi_set = 10;

  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0,255);
}

void loop()
{
  float Umi = dht.readHumidity();
  Umidade = Umi;
  myPID.Compute();
  analogWrite(FAN_PIN, PWM);
  Serial.print("Umidade: ");
  Serial.print(Umi);
  Serial.print(" | PID: ");
  Serial.print(map(PWM, 0, 255, 0, 100));
  Serial.print(" | Output: ");
  Serial.println(PWM);
}
