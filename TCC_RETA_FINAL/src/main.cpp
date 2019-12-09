#include <Arduino.h>
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA
#include <LiquidCrystal_I2C.h> //INCLUSÃO DA BIBLIOTECA

LiquidCrystal_I2C lcd(0x27,20,4); // set the LCD address to 0x27 for a 16 chars and 2 line display

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

// --- Máscaras ---
#define     mask_pin     B10000000                   //cria uma máscara para inverter o digital 7

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
bool ativarTime();
bool pararTime();
// ===================== METODOS E FUNÇÕES ======================//

// ===================== VARIAVEIS E CONSTANTES ======================//
float pulse; //Variável que armazena o tempo de duração do echo
float dist_cm; //Variável que armazena o valor da distância em centímetros
bool horaDoShow = false;
const int DISTANCIA = 50;

// --- Protótipo das Funções ---
void rtc_system();                                   //Função de controle dos parâmetros do RTC
void rtc_disp();                                     //Função para imprimir valores do RTC

int seg = 0;
int min = 0;

// --- Variáveis Globais ---
int    T2_aux = 0x00;                                //contador auxiliar para o Timer2 (base de tempo de 1 seg)


// variáveis do RTC
short  hora          =   00, 
       minuto        =   00,
       segundo       =   00,  
       dia           =   9,
       mes           =   12;
int    ano           =   2019;
// ===================== VARIAVEIS E CONSTANTES ======================//

// --- Interrupção ---
ISR(TIMER2_OVF_vect)
{
    TCNT2   = 56;                                    //Reinicializa o registrador do Timer2
    T2_aux +=  1;                                    //incrementa o contador auxiliar


    if(T2_aux == 2500)                               //T2_aux chegou em 2500?  (2500 x 400µs = 1 seg.)
    {                                                //Sim
       T2_aux = 0x00;                                //Reinicia T2_aux


       // -- BASE DE TEMPO DE 1 SEGUNDO --
       PORTD ^= mask_pin;                            //PORTD = PORTD XOR mask_pin
                                                     //inverte estado do pino digital 7 (PD7 do atmega328p)
       
       rtc_disp();                                   //imprime parâmetros do RTC no monitor serial
       rtc_system();                                 //chama função de controle do RTC
       
       
   
    } //end if T2_aux
    
} //end ISR

void setup() {
    DDRD   = 0x80;                                  //configura digital 7 (PD7) como saída digital
    PORTD  = 0x00;                                  //inicializa o digital 7 (PD7) em LOW

    TCCR2A = 0x00;                                  //Timer2 operando em modo normal
    TCCR2B = 0x03;                                  //Prescaler 1:32
    TCNT2  = 56;                                    //Inicializa TCNT2 em 56 (contará até 200)
    TIMSK2 = 0x01;   

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
        //rtc.adjust(DateTime(2019, 12, 9, 00, 00, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
    }
    Serial.begin(9600); //Inicia comunicação serial
}

void loop() {
    // DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO

    // Print a message to the LCD.
    lcd.clear();
    lcd.backlight();
    // lcd.setCursor(1,0);
    // lcd.print("INICIO: "); //IMPRIME O TEXTO NA SERIAL
    // lcd.print(now.minute()); //IMPRIME NO MONITOR SERIAL A HORA
    // lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    // lcd.print(now.second()); //IMPRIME NO MONITOR SERIAL OS MINUTOS

    lcd.setCursor(3,1);
    if(dia < 10) lcd.print(" ");
    lcd.print(dia);
    lcd.print("/");
    if(mes < 10) lcd.print(" ");
    lcd.print(mes);
    lcd.print("/");
    lcd.print(ano);

    // min = (now.minute());
    // seg = (now.second());

    if (ativarTime())
    {
        horaDoShow = true;
    }

    if (horaDoShow)
    {
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(4,0);
        if(hora < 10) lcd.print(" ");
        lcd.print(hora);
        lcd.print(":");
        if(minuto < 10) lcd.print(" ");
        lcd.print(minuto);
        lcd.print(":");
        if(segundo < 10) lcd.print(" ");
        lcd.print(segundo);
        lcd.setCursor(3,1);
        if(dia < 10) lcd.print(" ");
        lcd.print(dia);
        lcd.print("/");
        if(mes < 10) lcd.print(" ");
        lcd.print(mes);
        lcd.print("/");
        lcd.print(ano);
    } 

    if (pararTime())
    {
        lcd.clear();
        lcd.backlight();
        lcd.setCursor(0,0);
        lcd.print("TEMPO FINAL: "); //IMPRIME O TEXTO NA SERIAL
        lcd.print(minuto); //IMPRIME NO MONITOR SERIAL A HORA
        lcd.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
        lcd.print(segundo); //IMPRIME NO MONITOR SERIAL OS MINUTOS
        delay(7000);
        horaDoShow = false;
        minuto = 00;
        segundo = 00;
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


// ===================================== Desenvolvimento das Funções ===================================== //

void rtc_system()                                    //Função de controle dos parâmetros do RTC
{
  short n_dias;                                      //armazena o número de dias de cada mês

  switch(mes)                                        //testa o mês
  {
    case  2:                                         //Fevereiro?
            if((ano%4 == 0 && ano%100 != 0) || ano%400 == 0)
              n_dias = 29; //se bissexto, terá 29 dias
            else
              n_dias = 28; //se não bissexto, terá 28 dias

              break;
    case  4:               //meses de 30 dias...
    case  6:
    case  9:
    case 11: n_dias = 30; break;
    default: n_dias = 31;  //meses de 31 dias...
    
  } //end switch

// ================================================== 
  segundo += 1;                                      //incrementa segundos

  if(segundo > 59)                                   //segundos maior que 59?
  {                                                  //sim
    segundo = 0;                                     //reinicia
    minuto += 1;                                     //incrementa minutos

     if(minuto > 59)                                 //minutos maior que 59?
     {                                               //sim
        minuto = 0;                                  //reinicia
        hora += 1;                                   //incrementa horas
       
         if(hora > 23)                               //horas maior que 23?
         {                                           //sim
            hora = 0;                                //reinicia
            dia += 1;                                //incrementa dias

             if(dia > n_dias)                        //dias maior que n_dias?
             {                                       //sim
                dia = 1;                             //reinicia
                mes += 1;                            //incrementa meses

                 if(mes > 12)                        //meses maior que 12?
                 {                                   //sim
                    mes = 1;                         //reinicia
                    ano += 1;                        //incrementa anos
                  
                 } //end if mes
              
             } //end if dia
          
         } //end if hora
      
     } //end if minuto
    
  } //end if segundo


} //end rtc_system


void rtc_disp()                                      //Função para imprimir valores do RTC
{

// Serial Monitor
   if(hora < 10) Serial.print(" ");
   Serial.print(hora);
   Serial.print(" : ");
   if(minuto < 10) Serial.print(" ");
   Serial.print(minuto);
   Serial.print(" : ");
   if(segundo < 10) Serial.print(" ");
   Serial.print(segundo);
   Serial.print(" | ");
   if(dia < 10) Serial.print(" ");
   Serial.print(dia);
   Serial.print(" / ");
   if(mes < 10) Serial.print(" ");
   Serial.print(mes);
   Serial.print(" / ");
   Serial.println(ano);

  
} //end rtc_disp
