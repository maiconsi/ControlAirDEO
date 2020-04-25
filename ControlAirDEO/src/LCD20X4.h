//Arrays para criação dos segmentos e customização dos números
byte LT[8]  = {B01111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};
byte UB[8]  = {B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B00000,  B00000};
byte RT[8]  = {B11110,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};
byte LL[8]  = {B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B01111};
byte LB[8]  = {B00000,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111};
byte LR[8]  = {B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11110};
byte UMB[8] = {B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B11111,  B11111};
byte LMB[8] = {B11111,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111};

byte SimboloDegC[8]          = {B01000, B10100, B01000, B00111, B01000, B01000, B01000, B00111};  // [8] Simbolo Graus Celsios
byte SimboloSetPoint[8]      = {B11100, B10000, B11100, B00111, B11101, B00111, B00100, B00100};  // [9] Simbolo do Set Point
byte SimboloCompressorON[8]  = {B11100, B10100, B10100, B11111, B00000, B10001, B10001, B11111};  // [10] Simbolo Compressor Ligado
byte SimboloCompressorOFF[8] = {B10001, B01010, B01110, B00100, B00100, B01110, B01010, B10001};  // [11] Simbolo Compressor desligado
byte SimboloWifiON[8]        = {B00100, B01010, B10001, B00100, B01010, B00000, B00100, B00000};  // [12] Simbolo Wifi ON

void IniciaDisplay()
{
  disp.begin(20,4);                                      //Inicializa LCD 20 x 4
  
  //Associa cada segmento criado, a um número
  disp.createChar(0,LT);
  disp.createChar(1,UB);
  disp.createChar(2,RT);
  disp.createChar(3,LL);
  disp.createChar(4,LB);
  disp.createChar(5,LR);
  disp.createChar(6,UMB);
  disp.createChar(7,LMB);
  
  //--Crio os simbolos no LCD
/*  disp.createChar(8, SimboloDegC);           // Simbolo Graus Celsius
  disp.createChar(9, SimboloSetPoint);       // SetPoint
  disp.createChar(10, SimboloCompressorON);  // Compressor ON
  disp.createChar(11, SimboloCompressorOFF); // Compressor OFF
  disp.createChar(12, SimboloWifiON);        // Wifi ON*/
}

void DisplayLCD (byte posX , byte posY ,const char* texto, int Pausa){
  disp.setCursor(posX, posY);
  disp.print(texto);
  delay(Pausa);
}

/*void MostraBarraSuperiorDataHora(DateTime pDataHoraAtual, boolean pCompressorON, boolean pWifiON)      //Função para gerar a barra superior do display
{
   disp.setCursor(0,0);                                     //Posiciona cursor na coluna 1, linha 1
   disp.print(printnn(pDataHoraAtual.day()) + "/");          //Mostrador
   disp.print(printnn(pDataHoraAtual.month()) + "/");
   disp.print(printnn(pDataHoraAtual.year() - 2000) + " ");

   disp.setCursor(9,0);
   disp.print(printnn(pDataHoraAtual.hour()) + ":");   
   disp.print(printnn(pDataHoraAtual.minute()) + " ");
   
   disp.setCursor(16,0);
   disp.print("*");
   
   disp.setCursor(19,0);
   disp.print("@");

} //end Função MostraBarraSuperior
*/
void MostraBarraInferiorMenuPrincipal()      //Função para gerar a barra inferior do display
{
  disp.setCursor(0,3);
  //disp.print("SETUP");
  disp.print("SETUP");
  
  disp.setCursor(7,3);
  disp.print("AUTO");
  
  disp.setCursor(14,3);
  disp.print("+    -");

} //end Função MostraBarraInferior

void MostraBarraSuperiorSubMenu_SETUP()
{
  disp.setCursor(0, 0);
  disp.print("       SETUP       ");
}

void MostraBarraInferiorSubMenu(byte pOpcao)
{
  disp.setCursor(0,3);                               
//  disp.print("VOLTAR PROXIMO   OK");  
  switch (pOpcao)    // 12345678901234567890
  {
    case 1: disp.print("ESC    --->       OK");
      break;
    case 2: disp.print("<---   --->       OK");
      break;
    case 3: disp.print("<---              OK");
      break;
    case 4: disp.print("ESC               OK");
      break;
  }
}

void SinalNegativoCustom(int pPosX)//Seleciona os segmentos para formar o numero 0
{ 
  disp.setCursor(pPosX, 1);
  disp.write(4);
}

void NumCustom0(int pPosX)//Seleciona os segmentos para formar o numero 0
{ 
  disp.setCursor(pPosX, 1); //Seleciona a linha superior
  disp.write((byte)0);  //Segmento 0 selecionado
  disp.write(1);  //Segmento 1 selecionado
  disp.write(2);
  disp.setCursor(pPosX, 2); //Seleciona a linha inferior
  disp.write(3);  
  disp.write(4);  
  disp.write(5);
}

void NumCustom1(int pPosX) //Seleciona os segmentos para formar o numero 1
{
  disp.setCursor(pPosX,1);
  disp.write(1);
  disp.write(2);
  disp.setCursor(pPosX+1,2);
  disp.write(5);
}

void NumCustom2(int pPosX) //Seleciona os segmentos para formar o numero 2
{
  disp.setCursor(pPosX,1);
  disp.write(6);
  disp.write(6);
  disp.write(2);
  disp.setCursor(pPosX, 2);
  disp.write(3);
  disp.write(7);
  disp.write(7);
}

void NumCustom3(int pPosX)  //Seleciona os segmentos para formar o numero 3
{
  disp.setCursor(pPosX,1);
  disp.write(6);
  disp.write(6);
  disp.write(2);
  disp.setCursor(pPosX, 2);
  disp.write(7);
  disp.write(7);
  disp.write(5); 
}

void NumCustom4(int pPosX)  //Seleciona os segmentos para formar o numero 4
{
  disp.setCursor(pPosX,1);
  disp.write(3);
  disp.write(4);
  disp.write(2);
  disp.setCursor(pPosX+2, 2);
  disp.write(5);
}

void NumCustom5(int pPosX)  //Seleciona os segmentos para formar o numero 5
{
  disp.setCursor(pPosX,1);
  disp.write((byte)0);
  disp.write(6);
  disp.write(6);
  disp.setCursor(pPosX, 2);
  disp.write(7);
  disp.write(7);
  disp.write(5);
}

void NumCustom6(int pPosX)  //Seleciona os segmentos para formar o numero 6
{
  disp.setCursor(pPosX,1);
  disp.write((byte)0);
  disp.write(6);
  disp.write(6);
  disp.setCursor(pPosX, 2);
  disp.write(3);
  disp.write(7);
  disp.write(5);
}

void NumCustom7(int pPosX) //Seleciona os segmentos para formar o numero 7
{
  disp.setCursor(pPosX,1);
  disp.write(1);
  disp.write(1);
  disp.write(2);
  disp.setCursor(pPosX+1, 2);
  disp.write((byte)0);
}

void NumCustom8(int pPosX)  //Seleciona os segmentos para formar o numero 8
{
  disp.setCursor(pPosX,1);
  disp.write((byte)0);
  disp.write((byte)6);
  disp.write(2);
  disp.setCursor(pPosX, 2);
  disp.write(3);
  disp.write(7);
  disp.write(5);
}

void NumCustom9(int pPosX)  //Seleciona os segmentos para formar o numero 9
{
  disp.setCursor(pPosX,1);
  disp.write((byte)0);
  disp.write((byte)6);
  disp.write((byte)2);
  disp.setCursor(pPosX+2, 2);
  disp.write((byte)5);
}

void MostraNumero(int pNumero, int pPosX) //Mostra o numero na posicao definida por "X"
{
  switch(pNumero)
    {
      case 0: NumCustom0(pPosX);
      break;
      case 1: NumCustom1(pPosX);
      break;
      case 2: NumCustom2(pPosX);
      break;
      case 3: NumCustom3(pPosX);
      break;
      case 4: NumCustom4(pPosX);
      break;
      case 5: NumCustom5(pPosX);
      break;
      case 6: NumCustom6(pPosX);
      break;
      case 7: NumCustom7(pPosX);
      break;
      case 8: NumCustom8(pPosX);
      break;
      case 9: NumCustom9(pPosX);
      break;
    }
}

//--Mostra o SetPoint da tela
void MostraSetPointTemp(double pSetPoint)
{
  disp.setCursor(13, 2);
  disp.print(AlinhaNumeroDireita(pSetPoint, -1));
  disp.print(pSetPoint, 1);
  disp.print("sp");
}
