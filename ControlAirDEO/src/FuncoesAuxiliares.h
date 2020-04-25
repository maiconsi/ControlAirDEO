void Buzzer(byte NumBeep, int Period){
  for (byte i = 0; i < NumBeep; i++){
    #if ToneOnBuzzer == false
      digitalWrite (Buzz, HIGH);
      delay (Period);
      digitalWrite(Buzz, LOW);
      delay(75);
    #elif ToneOnBuzzer == true
      tone(Buzz, 600, Period);
      delay(Period + 75);
      noTone(Buzz);
    #endif
  }
}

// Verifica se o botão está precionado
byte BtnPresionado(byte Button_press, int msTime){
  if (digitalRead(Button_press)==0){
    delay (msTime);
    if (digitalRead(Button_press)==0){
      while(digitalRead(Button_press)==0){
      }
      return 1;
    }
  }
  return 0;
}

// repeat button press
byte BtnRepetir(byte Button_press){
  if (digitalRead(Button_press)==0){
    delay(250);
    return 1;
  }
  return 0;
}

String LCDSpace(int Num){
  if(Num < 0)
    Num = 0;
  
  String Espaco = "";
    
  for(byte i = 0; i < Num; i++){
    Espaco = Espaco + " ";
  }
  
  return Espaco;
}

//Função para imprimir um numero com 2 digitos
String printnn(int n) {
  // imprime um numero com 2 digitos
  // acrescenta zero `a esquerda se necessario
  String digitos= String(n);
  if (digitos.length()==1) {
    digitos="0" + digitos;
  }
  
 return digitos; 
}  

String AlinhaNumeroDireita(float Numero, int Offset) {
  byte Espaco = 0;
  if (Numero <= -100.0)                    Espaco = 0;
  if (Numero <=  -10.0 && Numero > -100.0) Espaco = 1;
  if (Numero <     0.0 && Numero >  -10.0) Espaco = 2;
  if (Numero <    10.0 && Numero >=   0.0) Espaco = 3;
  if (Numero <   100.0 && Numero >=  10.0) Espaco = 2;
  if (Numero <  1000.0 && Numero >= 100.0) Espaco = 1;
  if (Numero >= 1000.0)                    Espaco = 0;
  
  return LCDSpace(Espaco + Offset);
}

#if TestMemoria == true
int freeRam() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
#endif


byte GetDataMen(int addr){
  #if ReadWrite   == true  
    Serial.print (F("R-> "));
    Serial.print (addr);
    Serial.print (F(" byte: "));
    Serial.println (EEPROM.read(addr)); 
  #endif

  return EEPROM.read(addr);
}

void SetDataMen(int addr, byte data){
  #if ReadWrite   == true
    Serial.print (F("W-> "));
    Serial.print (addr);
    Serial.print (F(" byte: "));
    Serial.println (data);
  #endif
  
  EEPROM.write(addr,data);
}

void SetDataMenFloat(int addr, float pValor){
  if(pValor < 0){ //Verifico se o valor é negativo
    SetDataMen(addr, 1);
    pValor = pValor * -1;
  }
  else
    SetDataMen(addr, 0);
    
  SetDataMen(addr + 1, int(pValor)); 
  SetDataMen(addr + 2, int((pValor - int(pValor))*100));
}

float GetDataMenFloat(int addr){ 
  byte vNegativo = byte(EEPROM.read(addr)); // Operações se 1 = valor é negativo
  float vParte1 = float(EEPROM.read(addr + 1)); // Parte inteira do numero
  float vParte2 = float(EEPROM.read(addr + 2)); // valor decimal do numero
  float vValor = vParte1 + (vParte2 /100);
  if (vNegativo == 1)
    vValor = vValor * -1;
    
  #if ReadWrite    == true
    Serial.print (F("R-> "));
    Serial.print (addr);
    Serial.print (F(" float: "));
    Serial.println(vValor); 
  #endif
  
  return vValor;
}

void SetDataMenWord(int addr, int data){
  EEPROM.write(addr, highByte(data));
  EEPROM.write((addr+1),lowByte(data));
  
  #if ReadWrite   == true
    Serial.print (F("W-> "));
    Serial.print (addr);
    Serial.print (F(" Word: "));
    Serial.println (data); 
  #endif
}  

void SalvaSetPointTemp(int addr, float pSetPoint){
  SetDataMenFloat(addr, pSetPoint);
}

