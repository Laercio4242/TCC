#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include <LiquidCrystal_I2C.h> //INCLUSÃO DA BIBLIOTECA

LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

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
    rtc.adjust(DateTime(2019, 12, 09, 00, 00, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
  lcd.init(); // initialize the lcd 
  lcd.init();
}

void loop () {
    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO

    // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Data: "); //IMPRIME O TEXTO NO DISPLAY
    lcd.print(now.day(), DEC); //IMPRIME NO DISPLAY O DIA
    lcd.print('/'); //IMPRIME O CARACTERE NO DISPLAY
    lcd.print(now.month(), DEC); //IMPRIME NO DISPLAY O MÊS
    lcd.print('/'); //IMPRIME O CARACTERE NO DISPLAY
    lcd.print(now.year(), DEC); //IMPRIME NO DISPLAY O ANO
    lcd.setCursor(0,1);
    lcd.print("Hora: "); //IMPRIME O TEXTO NA SERIAL
    lcd.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS

    delay(1000); //INTERVALO DE 1 SEGUNDO
}
