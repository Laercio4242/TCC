#include <Arduino.h>
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include <LiquidCrystal_I2C.h> //INCLUSÃO DA BIBLIOTECA

LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"};

// ===================== Mapeamento de Hardware ======================//
#define TRIG_START 13 //Pino 13 do Arduino será a saída de TRIG_STARTger
#define ECHO_START 12 //Pino 12 do Arduino será a entrada de echo
#define TRIG_STOP 10 //Pino 10 do Arduino será a saída de trigger
#define ECHO_STOP 11 //Pino 11 do Arduino será a entrada de echo
// ===================== Mapeamento de Hardware ======================//

// ===================== METODOS E FUNÇÕES ======================//
int pulsoInicio(); //Função que gera o pulso de trigger
int pulsoStop(); //Função que gera o pulso de trigger
bool ativaTime();
bool pararTime();
// ===================== METODOS E FUNÇÕES ======================//

// ===================== VARIAVEIS E CONSTANTES ======================//
float pulse; //Variável que armazena o tempo de duração do echo
float dist_cm; //Variável que armazena o valor da distância em centímetros
const int DISTANCIA = 50;

int segundo = 0;
int minuto = 0;
// ===================== VARIAVEIS E CONSTANTES ======================//

void setup() {
    lcd.init();
    // ===================== PINMODE ======================//
    pinMode(TRIG_START, OUTPUT); //Pino de TRIG_STARTger será saída digital
    pinMode(ECHO_START, INPUT); //Pino de echo será entrada digital
    pinMode(TRIG_STOP, OUTPUT); //Pino de TRIG_STARTger será saída digital
    pinMode(ECHO_STOP, INPUT); //Pino de echo será entrada digital
    digitalWrite(TRIG_START, LOW); //Saída TRIG_STARTger inicia em nível baixo
    digitalWrite(TRIG_STOP, LOW); //Saída TRIG_STARTger inicia em nível baixo
    // ===================== PINMODE ======================//

    if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
        Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
        while(1); //SEMPRE ENTRE NO LOOP
    }
    if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
        Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
        //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
        //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
        rtc.adjust(DateTime(2019, 12, 9, 00, 00, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
    }
    Serial.begin(9600); //Inicia comunicação serial
}

void loop() {
    DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO

    // Print a message to the LCD.
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("INICIO: "); //IMPRIME O TEXTO NA SERIAL
    lcd.print(now.minute()); //IMPRIME NO MONITOR SERIAL A HORA
    lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    lcd.print(now.second()); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    // lcd.print("Data: "); //IMPRIME O TEXTO NO DISPLAY
    // lcd.print(now.day(), DEC); //IMPRIME NO DISPLAY O DIA
    // lcd.print('/'); //IMPRIME O CARACTERE NO DISPLAY
    // lcd.print(now.month(), DEC); //IMPRIME NO DISPLAY O MÊS
    // lcd.print('/'); //IMPRIME O CARACTERE NO DISPLAY
    // lcd.print(now.year(), DEC); //IMPRIME NO DISPLAY O ANO
    // lcd.print(' '); //IMPRIME O CARACTERE NO DISPLAY
    // // lcd.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO DISPLAY O DIA
    // lcd.setCursor(0,1);
    // lcd.print("Hora: "); //IMPRIME O TEXTO NA SERIAL
    // lcd.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    // lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    // lcd.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    // lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    // lcd.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS

    minuto = (now.minute());
    segundo = (now.second());

    if (pararTime())
    {
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(1,0);
        lcd.print("FIM: "); //IMPRIME O TEXTO NA SERIAL
        lcd.print(minuto); //IMPRIME NO MONITOR SERIAL A HORA
        lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
        lcd.print(segundo); //IMPRIME NO MONITOR SERIAL OS MINUTOS
        delay(7000);
    }

    Serial.print("START: ");
    Serial.print(pulsoInicio());
    Serial.print(" | ");
    Serial.print("STOP: ");
    Serial.println(pulsoStop());
    delay(1000); //Taxa de atualização
}

// =====================================  ULTRASSONICOS ===============================//
int pulsoInicio() {
    digitalWrite(TRIG_START, HIGH); //Pulso de TRIG_STARTger em nível alto
    delayMicroseconds(10); //duração de 10 micro segundos
    digitalWrite(TRIG_START, LOW); //Pulso de TRIG_STARTge em nível baixo
    pulse = pulseIn(ECHO_START, HIGH, 200000); //Mede o tempo em que o pino de echo fica em nível alto
    return dist_cm = pulse / 58.82; //Valor da distância em centímetros
}

bool ativarTime(){
    return pulsoInicio() < DISTANCIA;
}

int pulsoStop() {
    digitalWrite(TRIG_STOP, HIGH); //Pulso de TRIG_STARTger em nível alto
    delayMicroseconds(10); //duração de 10 micro segundos
    digitalWrite(TRIG_STOP, LOW); //Pulso de trigge em nível baixo
    pulse = pulseIn(ECHO_STOP, HIGH, 200000); //Mede o tempo em que o pino de echo fica em nível alto
    return dist_cm = pulse / 58.82; //Valor da distância em centímetros
}

bool pararTime(){
    return pulsoStop() < DISTANCIA;
}
// =====================================  ULTRASSONICOS ===============================//