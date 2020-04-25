// --- Mapeamento de Hardware ---
#define Button_L      12                                     //Botão para selecionar tela acima no digital 12
#define Button_R      11                                     //Botão para selecionar tela abaixo no digital 11
#define Button_up     10                                     //Botão de ajuste mais no digital 10
#define Button_dn      9                                     //Botão de ajuste menos no digital 9
#define Compressor    A0                                     //Saída para o compressor no A0 (será usado como digital)
#define Termistor     A1                                     //Saída para o Termistor NTC TTC-104
#define Buzz          A2                                     //Saida para o beep  

// --Sensor de temperatura
//OneWire ds(A1);

// --- Hardware do LCD ---
LiquidCrystal disp(7,  //RS no digital 7
                   6,  //EN no digital 6
                   5,  //D4 no digital 5
                   4,  //D5 no digital 4
                   3,  //D6 no digital 3
                   2); //D7 no digital 2
