// #include <ArduinoJson.h>
#include <EEPROM.h>
#include <Stepper.h>

const int stepsPerRevolution = 20;

#define IN1_MOTOR1 9
#define IN2_MOTOR1 8
#define IN3_MOTOR1 7
#define IN4_MOTOR1 6

#define IN1_MOTOR2 5
#define IN2_MOTOR2 4
#define IN3_MOTOR2 3
#define IN4_MOTOR2 2

Stepper stepperMotor1(stepsPerRevolution, IN1_MOTOR1, IN3_MOTOR1, IN2_MOTOR1, IN4_MOTOR1);
Stepper stepperMotor2(stepsPerRevolution, IN1_MOTOR2, IN3_MOTOR2, IN2_MOTOR2, IN4_MOTOR2);

int motor1 = 1;
int motor2 = 2;

struct Configuracoes
{
	int passo_horizontal;
	int passo_vertical;
	int tamanho_horizontal;
	int tamanho_vertical;
};

Configuracoes configObj;

void setup()
{
	stepperMotor1.setSpeed(200);
	stepperMotor2.setSpeed(200);
	Serial.begin(9600);

	configObj = lerConfiguracoes();
	prints();
}

void loop()
{
	if (Serial.available())
	{
		String comando = Serial.readStringUntil('\n');
		comando.trim(); // Remove espaços e quebras de linha desnecessárias

		if (comando == "configurar")
		{

			configurarPassos();
		}
		else if (comando == "testar")
		{
			testarPassos();
		}
		else if (comando == "capturar")
		{
			realizarCaptura();
			prints();
		}
	}
}

void prints()
{
	Serial.println("");
	Serial.println("menu");
	Serial.println("Selecione as opções abaixo via Serial:");
	Serial.println("Digite 'configurar' para configurar os passos");
	Serial.println("Digite 'testar' para testar os passos");
	Serial.println("Digite 'capturar' para começar a captura");
}

void configurarPassos()
{
	Serial.println("Configurando Passo Horizontal (->)");
	configObj.passo_horizontal = config(motor1);
	Serial.println("Configurando Passo Vertical (|)");
	configObj.passo_vertical = config(motor2);
	Serial.println("Configurando Tamanho Horizontal (<-)");
	configObj.tamanho_horizontal = config(motor1);
	Serial.println("Configurando Tamanho Vertical (|)");
	configObj.tamanho_vertical = config(motor2);

	stepperMotor1.step(-configObj.passo_horizontal + configObj.tamanho_horizontal);
	stepperMotor2.step(-configObj.passo_vertical + configObj.tamanho_vertical);

	salvarConfiguracoes(configObj);
	prints();
}

void testarPassos()
{
	stepperMotor1.step(configObj.passo_horizontal);
	stepperMotor2.step(configObj.passo_vertical);
	delay(500);

	stepperMotor1.step(-configObj.tamanho_horizontal);
	stepperMotor2.step(-configObj.tamanho_vertical);
	delay(500);

	stepperMotor1.step(-(configObj.passo_horizontal + configObj.tamanho_horizontal));
	stepperMotor2.step(-(configObj.passo_vertical + configObj.tamanho_vertical));
	prints();
}

int config(int currentMotor)
{
	int passo = 0;
	delay(1000);

	while (true)
	{
		if (Serial.available())
		{
			String comando = Serial.readStringUntil('\n');
			comando.trim();

			if (comando == "positivo")
			{
				Serial.println("Passo positivo");
				currentMotor == motor1 ? stepperMotor1.step(stepsPerRevolution) : stepperMotor2.step(stepsPerRevolution);
				passo += stepsPerRevolution;
			}
			else if (comando == "negativo")
			{
				Serial.println("Passo negativo");
				currentMotor == motor1 ? stepperMotor1.step(-stepsPerRevolution) : stepperMotor2.step(-stepsPerRevolution);
				passo -= stepsPerRevolution;
			}
			else if (comando == "ok")
			{
				Serial.println("Ok");
				delay(2000);
				return passo;
			}
		}
	}
}

void salvarConfiguracoes(Configuracoes configObj)
{
	int addr = 0; // endereço inicial na EEPROM

	// Escrever as configurações na EEPROM usando put()
	EEPROM.put(addr, configObj);

	Serial.println("Configurações salvas com sucesso na EEPROM.");
}

Configuracoes lerConfiguracoes()
{
	int addr = 0; // endereço inicial na EEPROM
	Configuracoes configObj;

	// Ler as configurações da EEPROM usando get()
	EEPROM.get(addr, configObj);

	Serial.print("Passo Horizontal: ");
	Serial.println(configObj.passo_horizontal);
	Serial.print("Passo Vertical: ");
	Serial.println(configObj.passo_vertical);
	Serial.print("Tamanho Horizontal: ");
	Serial.println(configObj.tamanho_horizontal);
	Serial.print("Tamanho Vertical: ");
	Serial.println(configObj.tamanho_vertical);

	return configObj;
}

void passoHorizontalCrescente()
{
	for (int passo_horizontal = 0;; passo_horizontal += configObj.tamanho_horizontal)
	{

		stepperMotor1.step(configObj.tamanho_horizontal);

		Serial.print("passo horizontal1: ");
		Serial.println(configObj.tamanho_horizontal);

		captura();

		if (passo_horizontal + configObj.tamanho_horizontal >= configObj.passo_horizontal)
		{
			int passo_final = configObj.passo_horizontal - passo_horizontal;
			stepperMotor1.step(passo_final);

			Serial.print("passo horizontal2: ");
			Serial.println(configObj.tamanho_horizontal);

			captura();
			break;
		}
	}
}

void passoHorizontalDecrescente()
{
	for (int passo_horizontal = configObj.passo_horizontal;; passo_horizontal -= configObj.tamanho_horizontal)
	{

		stepperMotor1.step(-configObj.tamanho_horizontal);

		Serial.print("passo horizontal3: ");
		Serial.println(-configObj.tamanho_horizontal);

		captura();

		if (passo_horizontal - configObj.tamanho_horizontal <= 0)
		{
			stepperMotor1.step(-passo_horizontal);

			Serial.print("passo horizontal4: ");
			Serial.println(-configObj.tamanho_horizontal);

			captura();
			break;
		}
	}
}

void realizarCaptura()
{

	for (int passo_vertical = 0;; passo_vertical += configObj.tamanho_vertical)
	{
		captura();
		passoHorizontalCrescente();
		// passoHorizontalDecrescente();

		passo_vertical += configObj.tamanho_vertical;
		stepperMotor2.step(configObj.tamanho_vertical);

		Serial.print("passo vertical1: ");
		Serial.println(configObj.tamanho_vertical);

		if (passo_vertical + configObj.tamanho_vertical >= configObj.passo_vertical)
		{
			captura();
			passoHorizontalDecrescente();
			// passoHorizontalCrescente();
			int passo_final = configObj.passo_vertical - passo_vertical;
			stepperMotor2.step(passo_final);

			Serial.print("passo vertical2: ");
			Serial.println(configObj.tamanho_vertical);
			captura();
			passoHorizontalDecrescente();

			break;
		}

		captura();
		passoHorizontalDecrescente();
		// passoHorizontalCrescente();

		if (passo_vertical + configObj.tamanho_vertical >= configObj.passo_vertical)
		{
			captura();
			passoHorizontalCrescente();
			// passoHorizontalDecrescente();
			int passo_final = configObj.passo_vertical - passo_vertical;
			stepperMotor2.step(passo_final);

			Serial.print("passo vertical3: ");
			Serial.println(configObj.tamanho_vertical);
			captura();
			passoHorizontalCrescente();
			// passoHorizontalDecrescente();
			break;
		}
		stepperMotor2.step(configObj.tamanho_vertical);

		Serial.print("passo vertical4: ");
		Serial.println(configObj.tamanho_vertical);
	}
};

void captura()
{
	delay(2000);
	Serial.println('0');
	while (!Serial.available())
		;
	String value_read = Serial.readString();

	if (value_read.indexOf('1') != -1)
	{
		Serial.println("2");
	}
}
