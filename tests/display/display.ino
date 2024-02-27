// Programa: Display LCD 16x2
// Autor: MakerHero

// Carrega a biblioteca LiquidCrystal
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Define os pinos que serão utilizados para ligação ao display
LiquidCrystal lcd(13, A1, A5, A4, A3, A2);

void setup()
{
  // Define o número de colunas e linhas do LCD
  lcd.begin(16, 2);
  Serial.begin(9600);
}

void loop()
{
  Serial.println("Teste");
  // Limpa a tela
  lcd.clear();
  // Posiciona o cursor na coluna 3, linha 0;
  lcd.setCursor(3, 0);
  // Envia o texto entre aspas para o LCD
  lcd.print("MakerHero");
  lcd.setCursor(3, 1);
  lcd.print(" LCD 16x2");
  delay(5000);

  // Rolagem para a esquerda
  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }

  // Rolagem para a direita
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }
}
