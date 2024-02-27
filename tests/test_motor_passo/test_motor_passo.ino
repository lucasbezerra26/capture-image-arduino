#include <Stepper.h>

// Constantes
const int stepsPerRevolution = 20;

// Pinos do Driver ULN2003 para Motor 1
#define IN1_MOTOR1 21
#define IN2_MOTOR1 19
#define IN3_MOTOR1 18
#define IN4_MOTOR1 5

// Pinos do Driver ULN2003 para Motor 2
#define IN1_MOTOR2 23
#define IN2_MOTOR2 22
#define IN3_MOTOR2 32
#define IN4_MOTOR2 15

// Pinos dos Botões
#define BUTTON_PIN_VERMELHO 4
#define BUTTON_PIN_AZUL 2
#define BUTTON_PIN_VERDE_OK 34

// Inicialização dos motores
Stepper stepperMotor1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper stepperMotor2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

// Variáveis
int motor1 = 1;
int motor2 = 2;
int currentMotor = motor2;

void setup()
{
  stepperMotor1.setSpeed(100);
  stepperMotor2.setSpeed(100);
  Serial.begin(9600);

  pinMode(BUTTON_PIN_VERMELHO, INPUT);
  pinMode(BUTTON_PIN_AZUL, INPUT);
  pinMode(BUTTON_PIN_VERDE_OK, INPUT);
}

void loop()
{
  if (digitalRead(BUTTON_PIN_VERDE_OK) == HIGH)
  {
    Serial.println("Trocar motor");
    currentMotor = (currentMotor == motor1) ? motor2 : motor1;
    
  }

  if (digitalRead(BUTTON_PIN_AZUL) == HIGH)
  {
    Serial.println("Passo para frente");
    currentMotor == motor1 ? stepperMotor1.step(stepsPerRevolution) : stepperMotor2.step(stepsPerRevolution);
  }
  else if (digitalRead(BUTTON_PIN_VERMELHO) == HIGH)
  {
    Serial.println("Passo para trás");
    currentMotor == motor1 ? stepperMotor1.step(-stepsPerRevolution) : stepperMotor2.step(-stepsPerRevolution);
  }
}
