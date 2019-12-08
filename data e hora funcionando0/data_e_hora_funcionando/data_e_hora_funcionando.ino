#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};

void setup(){
  Serial.begin(9600); //INICIALIZA A SERIAL
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }

  lcd.begin (16,2);                               //Inicializa LCD I2C
  Serial.begin(115200);                           //Inicializa serial em 115200 baud rate

  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
}

void loop () {
    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
    Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    Serial.println(); //QUEBRA DE LINHA NA SERIAL

    lcd.setCursor(0,0);
    lcd.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    lcd.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    lcd.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    lcd.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    lcd.print(" / Dia: "); //IMPRIME O TEXTO NA SERIAL
    lcd.setCursor(3,1);
    lcd.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
    lcd.print(" / Horas: "); //IMPRIME O TEXTO NA SERIAL
    lcd.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    lcd.println(); //QUEBRA DE LINHA NA SERIAL


    delay(1000); //INTERVALO DE 1 SEGUNDO
}
