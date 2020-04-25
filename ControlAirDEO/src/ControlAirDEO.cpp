#include <Arduino.h>

/// FOR DEBUGGING ///
#define ToneOnBuzzer  false
#define ReadWrite     true
#define TestMemoria   false
/// ------------- ///

// --- Bibliotecas Auxiliares ---
#include <EEPROM.h>
#include <LiquidCrystal.h>                              //Biblioteca para o display LCD
//#include <Wire.h>
//#include <OneWire.h>
// #include "RTClib.h"

// Modelo da Placa PCB*****
#include "PCB_V1.h"

// Funções Auxiliares
#include "FuncoesAuxiliares.h"

// Configuração do LCD*****
#include "LCD20X4.h"

//RTC_DS1307 rtc;

// --- Protótipo das Funções Auxiliares ---
void ChangeMenu();                                      //Função para modificar o menu atual
void DispMenu();                                        //Função para mostrar o menu atual
void MenuPrincial();                                    //Função do menu1, principal
void temperatura();                                     //Função do menu2, temperatura
void lights();                                          //Função do menu3, acionamento de lampadas  

void GetTemperatura();
void GetDataHora();

void AjustaDataHoraSerial();
//void printDateTime(DateTime t);
void printI00(int val, char delim);

void ControlaCompressor();
void MenuSetup();
void MenuAuto();
void MostraTemperatura();

// --- Variáveis Globais ---
byte menu = 1;                                      //Variável para selecionar o menu
byte submenu = 0;                                   //Variavel para selecionar o submenu

// Variaveis Globais para o mapeamento do endereço de memoria 
const int ender_men_set_point = 10;  //--Setpoint da temperatura 10 até 12
const int ender_men_qtde_rampas = 13; //--Automação-> Qtde. de rampas até 5 rampas
// const int ender_men_ = 14; //--Automação-> Qtde. de rampas

boolean Conv_start   = false;  //Conversão iniciada
byte data[9]            ; // Sensor Temp with crc version
byte Busy            = 0;

float temperatura_atual = 0;
float temperatura_anterior = 0;
float temp_set_point = 0.0f;
int temp_anterior1 = 0;
int temp_anterior2 = 0;
int temp_anterior3 = 0;
int temp_fracao_anterior = 0;
//DateTime data_hora_atual; //Variável para armazernar a data e hora atual;  

// Parâmetros do termistor
const double beta = 4400.0;
const double r0 = 100.0;
const double t0 = 273.0 + 25.0;
const double rx = r0 * exp(-beta/t0);
 
// Parâmetros do circuito
const double vcc = 5.0;
const double R = 10000.0;
 
// Numero de amostras na leitura
const int nAmostras = 5;

// --- Configurações Iniciais ---
void setup() {
  // put your setup code here, to run once:
  IniciaDisplay();
  
  // Wire.begin();                                          // Inicando o OneWire
  // rtc.begin();                                           // Iniciando o relogio RTC

  Serial.begin(9600);
  // if (!rtc.isrunning()) {
  //   Serial.println("RTC parado, vou ajustar com a hora da compilacao...");
  //   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // }
  // se o byte 44 da memoria permanente do modulo contiver 
  // o valor 65 (colocado la anteriormente por este mesmo programa), 
  // sabemos que podemos exibir o dia e hora do ultimo reset.
  byte InfoUltimoReset[7];
  
  for (int nCount = 0; nCount < 6; nCount++){
    InfoUltimoReset[nCount] = GetDataMen(nCount);
  }
    
  if (InfoUltimoReset[0]==65) {
    Serial.print("O ultimo reset deste sistema foi no dia ");
    Serial.print(printnn(InfoUltimoReset[1]) + "/");
    Serial.print(printnn(InfoUltimoReset[2]) + "/");
    Serial.print(printnn(InfoUltimoReset[3]));
    Serial.print(" `as " + printnn(InfoUltimoReset[4]));
    Serial.print(":" + printnn(InfoUltimoReset[5]));
    Serial.print(":" + printnn(InfoUltimoReset[5]));
//    Serial.print(" - " + rtc.readnvram(50)); 
    Serial.println(".");
  }
  
  //--Verifico se foi passado o comando para alterar a data e hora via serial
  AjustaDataHoraSerial();
  
  // grava dia/hora desta inicializacao na memoria permanente do Tiny RTC
  //DateTime tstamp = rtc.now();
//  SetDataMen(0, 65); //Controle
//  SetDataMen(1, tstamp.day());
//  SetDataMen(2, tstamp.month());
//  SetDataMen(3, (tstamp.year() - 2000));
//  SetDataMen(4, tstamp.hour());
//  SetDataMen(5, tstamp.minute());
//  SetDataMen(6, tstamp.second());

  temp_set_point = GetDataMenFloat(ender_men_set_point);
  
  if((temp_set_point < -20) || (temp_set_point > 99))
    temp_set_point = 18;
  
  for(char i=9; i<13; i++) 
    pinMode(i, INPUT_PULLUP);     //Entrada para os botões (digitais 9 a 12) com pull-ups internos
  
  pinMode(Compressor, OUTPUT);                                //Configura saída para o compressor 1
  pinMode(Buzz, OUTPUT);
  
  digitalWrite(Compressor, HIGH);                        //Compressor 1 inicia desligado 
} //end setup

// --- Loop Infinito ---
void loop() {
  // put your main code here, to run repeatedly:
  // Obtem a data e hora correntes e armazena em tstamp
  //GetDataHora();
  
  GetTemperatura();
  
  ControlaCompressor();
  
  ChangeMenu();
    
  DispMenu();
} //end loop

// --- Desenvolvimento das Funções Auxiliares ---
void ChangeMenu()                                       //Modifica o menu atual
{
  if(BtnPresionado(Button_R, 50))                      //Botão "Direita" Start(AUTO)
  {           
    disp.clear();                                     //Limpa display
    
    if (menu == 1)// Se estiver no menu principal
    {
      menu++;                                           //Incrementa menu
      submenu=1;
      
      if(menu > 2) menu = 2;                      
    }
    else if(menu == 0)//Se estiver no menu SETUP
    {
      submenu++;
      if (submenu > 3)
        submenu = 3; 
    }
    else if(menu == 2)//Se estiver no menu AUTOMACAO
    {
      submenu++;
      if (submenu > 1)
        submenu = 1; 
    }
   
  } //end butUp
   
  if(BtnPresionado(Button_L, 50))                      //Botão "esquerda" Enter(SETUP)
  {
    disp.clear();                                     //Limpa display
    if (menu == 1)// Se estiver no menu principal
    {
      menu--;                                           //Decrementa menu
      submenu=1;
        
      if(menu < 0) menu = 0;                      //Se menu menor que 1, volta a ser 3
    }
    else if((menu == 0) || (menu == 2))//Se estiver no menu SETUP ou AUTOMACAO
    {
      submenu--;
      if (submenu == 0)
        menu = 1; //Volto para o menu principal 
    }
  } //end butDown   

} //end changeMenu

void DispMenu()                                         //Mostra o menu atual
{
    switch(menu)                                        //Controle da variável menu
    {
        case 0:                                         //Caso 0
             MenuSetup();                               //Chama a função do Setup        
             
             break;                                     //break
        case 1:                                         //Caso 1
             MenuPrincial();                            //Chama a menu principal
             
             break;                                     //break
       case 2:                                          //Caso 2
             MenuAuto();                                //Chama menu Automação  
            
             break;                                     //break
    
    } //end switch menu

} //end dispMenu

//--Busca a temperatura do sensor
void GetTemperatura(){   
  int soma = 0;
  for (int i = 0; i < nAmostras; i++) {
    soma += analogRead(Termistor);
    delay (10);
  }
 
  // Determina a resistência do termistor
  double v = (vcc*soma)/(nAmostras*1024.0);
  double rt = (vcc*R)/v - R;
 
  // Calcula a temperatura
  double t = beta / log(rt/rx);
  Serial.println (t-273.0);

  // Dá um tempo entre leituras
  delay (1000);
}

//--Busca a Data/Hora atual
void GetDataHora(){
  // data_hora_atual = rtc.now(); 
}

void AjustaDataHoraSerial(){
  //Verifica se foi setado um novo horario
  //Formato: ano, mês, dia, hora, minuto, segundo
  if (Serial.available() >= 12) {
//    DateTime tm;
    int y = Serial.parseInt();
    if (y >= 100 && y < 1000)
      Serial.print("Erro: Ano deve ter dois ou quatro digitos!");
    else {
     /* if (y >= 1000)
        tm.Year = CalendarYrToTm(y);
      else    //(y < 100)
        tm.Year = y2kYearToTm(y);
        
      tm.Month = Serial.parseInt();
      tm.Day = Serial.parseInt();
      tm.Hour = Serial.parseInt();
      tm.Minute = Serial.parseInt();
      tm.Second = Serial.parseInt();
      
      data_hora_atual = makeTime(tm);
      rtc.set(data_hora_atual);
      setTimedata_hora_atual;
      Serial.print("Horario modificado para: ");
      printDateTime(data_hora_atual);
      
      while (Serial.available() > 0) Serial.read();*/
    }
  } 
}

//Mostra data e hora na serial
/*void printDateTime(DateTime t)
{
  printI00(day(t), 0);
  Serial << monthShortStr(month(t)) << _DEC(year(t));
  Serial << ' ';
  printI00(hour(t), ':');
  printI00(minute(t), ':');
  printI00(second(t), ' ');
}*/

//Correcao para imprimir "00" ao inves de "0" caso
//o valor seja menor do que 10
void printI00(int val, char delim)
{
//  if (val < 10) Serial << '0';
//  Serial << _DEC(val);
//  if (delim > 0) Serial << delim;
//  return;
}
//--Faço os
void ControlaCompressor(){
 
/* Serial.print("Input: ");
 Serial.print(pid_input);
 Serial.print(" SetPoint: ");
 Serial.print(pid_set_point);
 Serial.print(" Output: ");
 Serial.println(pid_output);*/
   
 if (temperatura_atual > temp_set_point)
   digitalWrite(Compressor, LOW);  
 else
   digitalWrite(Compressor, HIGH); 

}

void MenuPrincial()                                     //Menu principal
{
  //--Função gera a barra superior com data/hora estado do compressor e wifi
  //MostraBarraSuperiorDataHora(data_hora_atual, true, true);   
   
  if(BtnPresionado(Button_up, 20))                       //Botão +
  {                                                      //Sim...      
    if(temp_set_point < 99){
      temp_set_point = (temp_set_point + 0.5);            //Incrementa Temp   
      SalvaSetPointTemp(ender_men_set_point, temp_set_point);
    }
   } //end butP
   
   if(BtnPresionado(Button_dn, 20))                      //Botão -
   {                                                    //Sim...      
      if(temp_set_point > -20){
        temp_set_point = (temp_set_point - 0.5);          //Decrementa temp             
        SalvaSetPointTemp(ender_men_set_point, temp_set_point);
      }
   
   } //end butM  
  
  MostraTemperatura();
  
  MostraSetPointTemp(temp_set_point);
  
  MostraBarraInferiorMenuPrincipal();  
} //end menu principal

void MostraTemperatura()
{
  int numero;
  float temperatura = temperatura_atual;

  /*O %10 é usado para calcular o "módulo" de uma divisão, ou seja, o resto de um número dividido pelo outro. 
    No programa, eu uso o %10 para separar os números da hora, por exemplo. 
    Vamos supor que eu tenha o valor 17, e quero separar o 1 e o 7. Para calcular isso, eu faço :

    primeiro_digito_hora = 17 / 10 = 1 (estou trabalhando apenas com números inteiros)
    segundo_digito_hora =17 % 10 = 7 */
      
  if(temperatura != temperatura_anterior)
  {
    //--Limpo o Sinal "-"   
    if(temperatura_anterior < 0) 
    {
      if(temperatura_anterior <= -10)
        DisplayLCD(0, 1, " ", 0);
      else
        DisplayLCD(4, 1, " ", 0);   
    }
   
    if(temperatura < 0)
    {
      if(temperatura <= -10)
        SinalNegativoCustom(0);   
      else
        SinalNegativoCustom(4);   
    }
    
    temperatura_anterior = temperatura; 
  }
  
  //--Verificação do 1º digito 
  if (temperatura >= 0)
    numero = (temperatura / 10);
  else      
    numero = ((temperatura * -1) / 10); //--Divido por 10 para achar o primeiro digito Ex.: 18 graus / 10 = 1,8
  
  //Condicional para evitar sobreposição de caracteres
  if (temp_anterior1 != numero) 
  {
    DisplayLCD(1, 1, "   ", 0);
    DisplayLCD(1, 2, "   ", 0);
    temp_anterior1 = numero;
  }
  //Chama rotina que mostra o numero customizado na tela
  if(numero > 0)
    MostraNumero(numero, 1); 

  if (temperatura >= 0)
    numero = (int(temperatura) % 10);
  else 
    numero = ((int(temperatura) * -1) % 10);
    
  if (temp_anterior2 != numero)
  {
    DisplayLCD(5, 1, "   ", 0);
    DisplayLCD(5, 2, "   ", 0);
    temp_anterior2 = numero;
  }
  MostraNumero(numero, 5); 
  
  DisplayLCD(8, 2, ".", 0);
  
  //--Mostro a fração
  if (temperatura >= 0)
    numero = (temperatura * 10);
  else 
    numero = ((temperatura * -1) * 10);
    
  numero = (numero % 10);
  
  if (temp_fracao_anterior != numero)
  {
    DisplayLCD(9, 1, "   ", 0);
    DisplayLCD(9, 2, "   ", 0);
    temp_fracao_anterior = numero;
  }
  MostraNumero(numero, 9); 
  
  DisplayLCD(12, 1, "c", 0);
}

void MenuSetup()                                     //Setup
{
  MostraBarraSuperiorSubMenu_SETUP();
  
  disp.setCursor(0,1);
  disp.print("Menu: ");
  disp.print(menu);
  
  disp.setCursor(0,2);
  disp.print("SubMenu: ");
  disp.print(submenu);
  
  MostraBarraInferiorSubMenu(submenu);
    
} //end temperatura()
 
void MenuAuto()                                      //Menu automatizado
{  
  disp.setCursor(0,0);                              //Posiciona cursor na coluna 1, linha 1
  disp.print("AUTOMATIZACAO");                      //Imprime mensagem
   
  disp.setCursor(0,1);
  disp.print("Menu: ");
  disp.print(menu);
  
  disp.setCursor(0,2);
  disp.print("SubMenu: ");
  disp.print(submenu);
 
  MostraBarraInferiorSubMenu(4);
} //end lights