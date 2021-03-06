
// (Based on Ethernet's WebClient Example)

    /*about communication*/
#include "WiFly.h"
#include "Credentials.h"
#include <Wire.h>
byte server[] = { 66, 249, 89, 104 }; // Google

//Client client(server, 80);
//ip del ordenador
WiFlyClient client("10.164.3.182", 80);

byte servidor[]={192, 168, 1, 132};



    /*about toy*/
//#include <Wire.h>
#include "Adafruit_Trellis.h"

#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_Trellis matrix1 = Adafruit_Trellis();

Adafruit_TrellisSet trellis = Adafruit_TrellisSet (&matrix0, &matrix1);

#define NUMTRELLIS 2
#define numKeys (NUMTRELLIS * 16)

#define INTPIN A8


    /* declaración de variables sonidos */
int spk=47;                          // altavoz a GND y pin 13
int c[5]={131,262,523,1046,2093};    // frecuencias 4 octavas de Do
int cs[5]={139,277,554,1108,2217};   // Do#
int d[5]={147,294,587,1175,2349};    // Re
int ds[5]={156,311,622,1244,2489};   // Re#
int e[5]={165,330,659,1319,2637};    // Mi
int f[5]={175,349,698,1397,2794};    // Fa
int fs[5]={185,370,740,1480,2960};   // Fa#
int g[5]={196,392,784,1568,3136};    // Sol
int gs[5]={208,415,831,1661,3322};   // Sol#
int a[5]={220,440,880,1760,3520};    // La
int as[5]={233,466,932,1866,3729};   // La#
int b[5]={247,494,988,1976,3951};    // Si


    /*definicion de variables globales del codigo*/

//determina si el juego se a terminado, pulsacion de un boton
boolean terminar=false;
boolean sinLuz=false;

int matriz1[]={30, 29, 28};
int matriz2[]={26, 25, 24};
int matriz3[]={22, 21, 20};
int matriz4[]={18, 17, 16};
int matriz5[]={14, 13, 12};
int matriz6[]={10, 9, 8};
int matriz7[]={6, 5, 4};
int matriz8[]={2, 1, 0};

//vaiables de subida
int bloque = 1;
int nivel= 1;
int fallos= 0;
int lecturas= 0;
int bucle= 0;
int iluminacion= 0;

//array donde se guardan los bucles
int arrayBucle[]={-1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,
                  -1,-1,-1,-1,};
                   
int estadoPul []={0,0,0,0,
                  0,0,0,0,
                  0,0,0,0, 
                  0,0,0,0,
                  0,0,0,0,
                  0,0,0,0,
                  0,0,0,0, 
                  0,0,0,0};

int secuencia1 []={0,0,0,0,
                   0,0,0,0,
                   0,0,0,0, 
                   0,0,0,0,
                   0,0,0,0,
                   0,0,0,0,
                   0,0,0,0, 
                   0,0,0,0};

void nota(int a, int b); // declaracion de la funcion auxiliar. Recibe dos numeros enteros.

int low;
int mid;
int high;


void setup() {
  //device configuration
  // put your setup code here, to run once:
  Serial.begin(9600);

  // INT pin requires a pullup
  pinMode(INTPIN, INPUT);
  digitalWrite(INTPIN, HIGH);
  
  //2 matrices de leds
  trellis.begin(0x70, 0x71);
  Serial.println("va a entrar.");
  
    WiFly.begin(); 
 if (!WiFly.join(ssid, passphrase)) {
   Serial.println("Association failed.");
 
  }

  
  //etapa inicial ao carregar o projeto no arduino: todos os leds acendem e apagam em seguida
  // light up all the LEDs in order - (pt: acende todos os leds)
  for (int i = numKeys; i >= 0; i--) {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  
  // then turn them off - (pt: apaga todos os leds)
  for (int i = numKeys; i >= 0; i--) {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  
  delay(100);
}


void loop() {
  //bloque00(3);
  //Inicio del juego main seleccion de nivel
  juegoMain();
  Serial.println("juegomain loop");
}


//seleeccion del nivel del juego que se quiere jugar
void juegoMain(){
  Serial.println("entra en juegoForMain");
  terminar = false; //o jogo não terminou 
  //los botones para la seleccion son el 3 y el 7 en binario

  while(!terminar){ //enquanto não terminar fazer essa sequência
    delay(30); // 30ms delay is required, dont remove me!
    if (MODE == LATCHING) { //se o mode é latching
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button 
        for (uint8_t i=0; i<numKeys; i++) {
          // if it was pressed... - (pt: verifica se o botão de iniciar sequência foi pressionado
          //caso contrário o jogo fica esperando)
          if (trellis.justPressed(i)) {
            Serial.print("v"); Serial.println(i);
            if (i==3){
              Serial.println("TERMINADO PULSADORES");
              trellis.setLED(3);
              trellis.writeDisplay();
      
              delay(100);
              trellis.clrLED(3);
              trellis.writeDisplay();
              terminar=true;
              seleccionNivel();
            }
            // Alternate the LED
            if (trellis.isLED(i))
              trellis.clrLED(i);
            else
              trellis.setLED(i);
          } 
        }
        // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }   
  }
  delay(100);
}


void seleccionNivel(){

  //leds de seleção de nível; confere se eles foram pulsado
  int once = trellis.isLED(11); //confere se o led7 foi selecionado 
  int siete= trellis.isLED(7); //confere se o led11 foi selecionado

  //apagar los leds pulsados
  trellis.clrLED(3);
  trellis.clrLED(7);
  trellis.clrLED(11);
  trellis.writeDisplay();
   
  //nivel es 4
  if(once && siete){ //ambos os leds foram selecionado: 11
    Serial.println("bloque11");
    bloque11(1);
    //bloque11;
  }

  //nivel 2
  if(once){  //apenas o led11 foi selecionado : 10
    Serial.println("bloque10");
    bloque10(1);
    Serial.println("bloque01");
    bloque01(1);
  }
  
  //nivel 3
  if(siete){ //apenas o led7 foi selecionado : 01
    Serial.println("bloque01");
    bloque01(1);
  }
  
  //nivel 1
  else{ //nenhum led foi selecionado: 00 
    Serial.println("bloque00");
    bloque00(1);
  }
}


void bloquesNiveles(int nivel){
  Serial.print("el boque es: ");
  Serial.println(bloque);
  Serial.print("nivel: ");
  Serial.println(bloque);
  
  if(bloque == 1){
    Serial.print("entra en bloques niveles para ver en que nivel esta");
    bloque00(nivel);
  }
  
  if(bloque==2){
    Serial.println("entra por if bloque 2");
    bloque01(nivel);
  }

  if(bloque==3){
    bloque10(nivel);
  }
  
  if(bloque ==4){
    bloque11(nivel);
  }
}

void bloque00(int nivel){
  bloque = 1;
  Serial.print("el bloque es: ");
  Serial.println(bloque);
  Serial.print("nivel: ");
  Serial.println(bloque);
  delay(1000);

  //vaiables de subida
  low=131;
  mid=523;
  high=2093;

  Serial.println("entra en bloque 00");
  switch (nivel) {
    case 1:{
      Serial.print("entra en case 1 \n ");
      boolean sinLuz=false; //é com luz
      iluminacion = 1;
      //sequência
      int secuencia1 []={0,0,1,0,
                         0,1,0,0,
                         1,0,0,0, 
                         0,1,0,0,
                         0,0,1,0,
                         0,1,0,0,
                         1,0,0,0, 
                         0,0,1,0};

      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 2: {
      Serial.println("entra en case 2");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                         1,0,0,0,
                         0,0,1,0, 
                         1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,1,0,0, 
                         1,0,0,0};
        
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 3:{
      Serial.println("entra en case 3 que entras o k");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                         1,0,0,0,
                         0,1,0,0, 
                         1,0,1,0,
                         0,1,0,0,
                         1,0,0,0,
                         0,0,1,0, 
                         0,1,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 4:{
      Serial.println("entra en case 4");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                         1,0,0,0,
                         0,1,0,0, 
                         0,0,1,0,
                         0,1,0,0,
                         1,0,0,0,
                         0,0,1,0, 
                         0,1,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 5:{
      Serial.println("entra en case 5");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,0,1,0,
                        0,1,0,0,
                        1,0,0,0, 
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 6:{
      Serial.println("entra en case 6");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,0,1,0,
                        0,1,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 7:{
      Serial.println("entra en case 7");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 8:{
      Serial.println("entra en case 8");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        1,0,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 9:{
      Serial.println("entra en case 9");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,1,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    } 
    case 10:{
      Serial.println("entra en case 10");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,0,1,0,
                        0,1,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    
    }
    default: {
      Serial.print("entra en default del bloque00");
      Serial.print(nivel);
      bloque01(1);
      break;
    }
  }
}

      
void bloque01(int nivel){
  delay(1000);
  bloque =2;
  low=131;
  mid=523;
  high=2093;

  Serial.println("entra en bloque 01");
  switch (nivel) {
    case 1:{
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1 []={1,0,0,0,
                         1,0,0,0,
                         1,0,0,0, 
                         0,1,0,0,
                         1,0,0,0,
                         1,0,0,0,
                         1,0,0,0, 
                         0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 2:{
      Serial.println("entra en case 2");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                        0,1,0,0,
                        1,0,0,0, 
                        1,0,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        1,0,0,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 3:{
      Serial.println("entra en case 3");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,0,1,0,
                        1,0,0,0,
                        1,0,0,0, 
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 4:{
      Serial.println("entra en case 4");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={1,0,0,0,
                        0,1,0,0,
                        0,0,1,0, 
                        0,1,0,0,
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0, 
                        0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 5:{
      Serial.println("entra en case 5");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,0,1,0,
                        0,0,1,0,
                        1,0,0,0, 
                        1,0,0,0,
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 6:{
      Serial.println("entra en case 6");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,1,0,0,
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 7:{
      Serial.println("entra en case 7");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        0,0,1,0,
                        0,0,1,0, 
                        1,0,0,0,
                        1,0,0,0,
                        0,0,1,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 8:{
      Serial.println("entra en case 8");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,0,1,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 9:{
      Serial.println("entra en case 9");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,1,0,0,
                        0,1,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 10:{
      Serial.println("entra en case 10");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,1,0,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0,
                        0,1,0,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    default: {
      bloque10(1);
      break;
    }
  }
}


//bloque tres
void bloque10(int nivel){
  delay(1000);
  bloque = 3;
  low=131;
  mid=523;
  high=2093;

  Serial.println("entra en bloque10");
  switch (nivel) {
    case 1:{
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1 []={1,0,0,0,
                         1,0,0,0,
                         1,0,0,0, 
                         1,0,0,0,
                         1,0,0,0,
                         1,0,0,0,
                         1,0,0,0, 
                         1,0,0,0};
      juegoRepeticiones(secuencia1, sinLuz, nivel);
      break;
    }
    case 2:{
      Serial.println("entra en case 2");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={1,0,0,0,
                        0,1,0,0,
                        1,0,0,0, 
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0,
                        0,0,1,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 3:{
      Serial.println("entra en case 3");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={1,0,0,0,
                        0,1,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 4:{
      Serial.println("entra en case 4");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                        1,0,0,0,
                        0,1,0,0, 
                        1,0,0,0,
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 5:{
      Serial.println("entra en case 5");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={0,0,1,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0,
                        1,0,0,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 6:{
      Serial.println("entra en case 6");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,0,1,0,
                        0,1,0,0,
                        0,0,1,0, 
                        0,1,0,0,
                        0,0,1,0,
                        0,1,0,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 7:{
      Serial.println("entra en case 7");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        1,0,0,0,
                        1,0,0,0, 
                        1,0,0,0,
                        0,1,0,0,
                        0,1,0,0,
                        0,1,0,0, 
                        0,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 8:{
      Serial.println("entra en case 8");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        1,0,0,0,
                        1,0,0,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 9:{
      Serial.println("entra en case 9");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        0,0,1,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,0,1,0,
                        0,0,1,0,
                        1,0,0,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 10:{
      Serial.println("entra en case 10");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={0,0,1,0,
                        0,1,0,0,
                        0,0,1,0, 
                        1,0,0,0,
                        0,1,0,0,
                        0,0,1,0,
                        0,0,1,0, 
                        0,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    default: {
      bloque11(1);
      break;
    }
  }
}


//bloque cuatro
void bloque11(int nivel){
  delay(1000);
  bloque = 4;
  low=131;
  mid=523;
  high=2093;

  Serial.println("entra en bloque11");
  switch (nivel) {
    case 1:{
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1 []={1,1,0,0,
                         1,0,0,0,
                         1,1,1,0, 
                         0,1,1,0,
                         1,0,0,0,
                         0,1,0,0,
                         0,1,1,0, 
                         0,1,0,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 2:{
      Serial.println("entra en case 2");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={1,1,1,0,
                        0,1,1,0,
                        1,0,0,0, 
                        0,1,1,0,
                        0,0,1,0,
                        1,0,1,0,
                        0,0,1,0, 
                        0,1,1,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 3:{
      Serial.println("entra en case 3");
      boolean sinLuz=false;
      iluminacion = 0;
      int secuencia1[]={1,0,0,0,
                        0,1,0,0,
                        0,1,1,0, 
                        1,1,0,0,
                        0,1,0,0,
                        1,0,1,0,
                        0,0,1,0, 
                        1,0,1,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 4:{
      Serial.println("entra en case 4");
      boolean sinLuz=false;
      iluminacion = 0;
      int secuencia1[]={1,1,0,0,
                        1,0,0,0,
                        1,1,1,0, 
                        1,0,0,0,
                        0,0,1,0,
                        0,1,1,0,
                        1,0,1,0, 
                        0,1,1,0};
      juegoSinRepeticiones(secuencia1, sinLuz);
      break;
    }
    case 5:{
      Serial.println("entra en case 5");
      boolean sinLuz=false;
      iluminacion = 1;
      int secuencia1[]={1,0,0,0,
                        0,0,1,0,
                        1,0,1,0, 
                        0,1,1,0,
                        1,0,0,0,
                        0,0,1,0,
                        1,0,1,0, 
                        0,1,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 6:{
      Serial.println("entra en case 6");
      boolean sinLuz=true;
      iluminacion = 1;
      int secuencia1[]={0,1,0,0,
                        1,1,1,0,
                        0,0,1,0, 
                        1,0,1,0,
                        0,1,0,0,
                        1,1,1,0,
                        0,0,1,0, 
                        1,0,1,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    }
    case 7:{
      Serial.println("entra en case 7");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,1,0,0,
                        1,0,1,0,
                        1,0,1,0, 
                        1,1,0,0,
                        1,1,0,0,
                        1,0,1,0,
                        1,0,1,0, 
                        1,1,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 8:{
      Serial.println("entra en case 8");
      boolean sinLuz=true;
      iluminacion = 0;
      int secuencia1[]={1,1,1,0,
                        1,0,0,0,
                        0,1,1,0, 
                        1,0,0,0,
                        1,1,1,0,
                        1,0,0,0,
                        0,1,1,0, 
                        1,0,0,0};
      juegoBucleCompleto(secuencia1, sinLuz, nivel);
      break;
    } 
    case 9:{
      Serial.println("entra en case 9");
      boolean sinLuz=true;
      iluminacion = 1;
      int secuencia1[]={1,0,0,0,
                        0,1,1,0,
                        0,1,1,0, 
                        1,0,0,0,
                        1,0,1,0,
                        1,0,1,0,
                        1,0,1,0, 
                        0,1,1,0};
      juegoRepeticiones(secuencia1, sinLuz, nivel);
      break;
    }
    case 10:{
      Serial.println("entra en case 10");
      boolean sinLuz=true;
      iluminacion = 1;
      int secuencia1[]={1,1,0,0,
                        1,1,0,0,
                        0,1,1,0, 
                        1,1,0,0,
                        0,1,1,0,
                        0,1,1,0,
                        1,0,1,0, 
                        1,0,1,0};
      juegoRepeticiones(secuencia1, sinLuz, nivel);
      break;
    }
    case 11:{
      boolean sinLuz=false;
      iluminacion = 0;
      int secuencia1 []={1,0,1,0,
                         1,0,1,0,
                         0,1,1,0, 
                         1,1,0,0,
                         1,1,0,0,
                         0,1,1,0,
                         0,1,1,0, 
                         0,1,1,0};
      juegoRepeticiones(secuencia1, sinLuz, nivel);
      break;
    }
    case 12:{
      Serial.println("entra en case 2");
      boolean sinLuz=false;
      iluminacion = 0;
      int secuencia1[]={1,1,0,0,
                        1,1,1,0,
                        1,1,1,0, 
                        1,1,1,0,
                        1,0,1,0,
                        1,0,1,0,
                        0,0,1,0, 
                        1,0,0,0};
      juegoRepeticiones(secuencia1, sinLuz, nivel);
      break;
    }
    default: {
      bloque00(1);
      break;
    }
  }
}


void juegoSinRepeticiones(int secuencia1[],boolean sinLuz){
  Serial.println("juego sin repeticiones");
  terminar = false;
  int correctoNivel= nivel;
  leerSecuencia(secuencia1, sinLuz);

  //mientras no se pulse terminar
  while(!terminar){
    delay(30); // 30ms delay is required, dont remove me!
    if (MODE == LATCHING) {
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button
        for (uint8_t i=0; i<numKeys; i++) {
          // if it was pressed...
          if (trellis.justPressed(i)) {
            if(i==31){ //se apertou o botão para repetir a sequência
              Serial.println("leer secuencia otra vez");
              lecturas= lecturas + 1;
              trellis.setLED(31);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(31);
              trellis.writeDisplay();
              leerSecuencia(secuencia1, sinLuz); //repetição da sequência
            }
            if (i==15) { //se apertou o botão para ir para o próximo bloco
              terminar = true;
              nivel = 1 + nivel;
              //correctoNivel=1 + nivel;
              //correctoNivel=1 + correctoNivel;
              Serial.println("el nivel correcto es....");
              Serial.println(nivel);
              bloquesNiveles(nivel);
              
            }
            if (i==19){ //se apertou botão de que terminou a sequência
              Serial.println("TERMINADO PULSADORES");
              trellis.setLED(19);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(19);
              trellis.writeDisplay();
              terminar=true;
              guardarSecuencia();
        //se reproducen las dos secuencias lo pulsado y lo correcto con parpadeo
        // leerSecuencia(estadoPul, true);
        // reproducirPulsadores(secuencia1);
              //correctoNivel = comprobarSecuencia(secuencia1, nivel);
              //if (correctoNivel==0){
              if (comprobarSecuencia(secuencia1, nivel) == false){  
                terminar= false;
                correctoNivel=nivel;
              } else{
      //mirar secuencia y terminar repeticiones igual a repetir secuencia
      //  correctoNivel=correctoNivel + nivel;
                correctoNivel=1 + nivel;
                Serial.println("correcto nivel: ");
                Serial.println(correctoNivel);
                Serial.println(" nivel: ");
                Serial.println(nivel);
                bloquesNiveles(correctoNivel);
      // bloque00(correctoNivel);
                delay(1000);
              }
            }
      // Alternate the LED
            if (trellis.isLED(i))
              trellis.clrLED(i);
            else
              trellis.setLED(i);
          } 
        }
      // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }  
  }
}


//se repite la mitad de la secuencia
void juegoBucleCompleto(int secuencia1[],boolean sinLuz, int nivel){
  Serial.println("juego CON repeticiones");
  reinicioArrayBucle();
  boolean usoBucle= false;
  terminar = false;
  int posicion = 0;
  int correctoNivel;
  leerSecuencia(secuencia1, sinLuz);

  //mientras no se pulse terminar
  while(!terminar){
    delay(30); // 30ms delay is required, dont remove me!
    if (MODE == LATCHING) {
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button
        for (uint8_t i=0; i<numKeys; i++) {
          // if it was pressed...
          if (trellis.justPressed(i)) {
            Serial.print(i);
            //leer secuencia otra vez
            if(i==31){
              Serial.println("leer secuencia otra vez");
              //se suma una lectura
              lecturas= lecturas + 1;
              //se inicia el arrayBucle
              reinicioArrayBucle();
              trellis.setLED(31);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(31);
              trellis.writeDisplay();
              leerSecuencia(secuencia1, sinLuz);
            }
            
            //si no es boton de control controles, guardar en array
            if(i!=3 || i!=7 || i!=11 || i!=15|| i!=19|| i!=31 ){
              Serial.print("entra en no botones de control");
              
              //se a usado el boton inicio bucle
              if(i==27){
                bucle =1;
                usoBucle= true;
              }
              posicion= posicion + 1;
              arrayBucle[posicion]= i;
            } 
            if (i==19){
              //si se a pulsado el boton 27 es que a realizado la secuencia mediane los botones de bucles
              if(usoBucle){
                guardarArrayBucles();
              } else {
                guardarSecuencia();
              }
              Serial.print(arrayBucle[1]);
              Serial.print(arrayBucle[31]);
              Serial.println("TERMINADO PULSADORES");
              trellis.setLED(19);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(19);
              trellis.writeDisplay();
              terminar=true;
              //se reproducen las dos secuencias lo pulsado y lo correcto con parpadeo
              // leerSecuencia(estadoPul, true);
              // reproducirPulsadores(secuencia1);
//              correctoNivel = comprobarSecuencia(secuencia1, nivel);
              reinicioArrayBucle();
              Serial.println("sale al principal");
 //             if (correctoNivel==0){
               if (comprobarSecuencia(secuencia1, nivel) == false){  

                correctoNivel= nivel;
                terminar= false;
              } else {
                //mirar secuencia y terminar repeticiones igual a repetir secuencia
                correctoNivel=correctoNivel + nivel;
              }
              Serial.println(correctoNivel);
            } 
            if (i==15) {
              Serial.print("entra en siguiente");
              reinicioArrayBucle();
              terminar = true;
              correctoNivel=1 + nivel;
              bloquesNiveles(correctoNivel);
            }
            // Alternate the LED
            if (trellis.isLED(i))
              trellis.clrLED(i);
            else
              trellis.setLED(i);
          } 
        }
        // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }   
  }
  bloquesNiveles(correctoNivel);
  // bloque00(correctoNivel);
  delay(1000);
}


//se repiten partes de la secuencia
void juegoRepeticiones(int secuencia1[],boolean sinLuz, int nivel){
  Serial.println("juego CON repeticiones");
  reinicioArrayBucle();
  boolean usoBucle= false;
  terminar = false;
  int posicion = 0;
  int correctoNivel;
  leerSecuencia(secuencia1, sinLuz);

  //se encienden los leds del ultimo bloque para que el usuario tenga que realizan los bucles obligatoriamente
  trellis.setLED(0);
  trellis.setLED(1);
  trellis.setLED(2);
  trellis.writeDisplay();

  //mientras no se pulse terminar
  while(!terminar){
    delay(30); // 30ms delay is required, dont remove me!

    if (MODE == LATCHING) {
      // If a button was just pressed or released...
      if (trellis.readSwitches()) {
        // go through every button
        for (uint8_t i=0; i<numKeys; i++) {
          // if it was pressed...
          if (trellis.justPressed(i)) {
            Serial.print(i);
            //leer secuencia otra vez
            if(i==31){
              Serial.println("leer secuencia otra vez");
              //se suma una lectura
              lecturas= lecturas + 1;
              //se inicia el arrayBucle
              reinicioArrayBucle();
              trellis.setLED(31);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(31);
              trellis.writeDisplay();
              leerSecuencia(secuencia1, sinLuz);
            }
            //si no es boton de control controles, guardar en array
            if(i!=3 || i!=7 || i!=11 || i!=15|| i!=19|| i!=31 ){
              Serial.print("entra en no botones de control");
              //se a usado el boton inicio bucle
              if(i==27){
                bucle =1;
                usoBucle= true;
              }
              posicion= posicion + 1;
              arrayBucle[posicion]= i;
            } 
            if (i==19){
              //si se a pulsado el boton 27 es que a realizado la secuencia mediane los botones de bucles
              Serial.print(arrayBucle[1]);
              Serial.print(arrayBucle[31]);
              Serial.println("TERMINADO PULSADORES");
              trellis.setLED(19);
              trellis.writeDisplay();
              delay(100);
              trellis.clrLED(19);
              trellis.writeDisplay();
              guardarBuclesVarios();
              terminar=true;
              // se reproducen las dos secuencias lo pulsado y lo correcto con parpadeo
              // leerSecuencia(estadoPul, true);
              // reproducirPulsadores(secuencia1);
          //    correctoNivel = comprobarSecuencia(secuencia1, nivel);
              reinicioArrayBucle();
              Serial.println("sale al principal");
            //  if (correctoNivel==0){
                          if (comprobarSecuencia(secuencia1, nivel) == false){  

                terminar= false;
              } else {
                //mirar secuencia y terminar repeticiones igual a repetir secuencia
                correctoNivel=correctoNivel + nivel;
              }
              Serial.println(correctoNivel);
            }
            if (i==15) {
              Serial.print("entra en siguiente");
              reinicioArrayBucle();
              terminar = true;
              correctoNivel=1 + nivel;
              bloquesNiveles(correctoNivel);
            }
            // Alternate the LED
            if (trellis.isLED(i))
              trellis.clrLED(i);
            else
              trellis.setLED(i);
          } 
        }
        // tell the trellis to set the LEDs we requested
        trellis.writeDisplay();
      }
    }   
  }
  bloquesNiveles(correctoNivel);
  // bloque00(correctoNivel);
  delay(1000);
}


void leerSecuencia(int secuencia1[], boolean sinLuz){
  Serial.println("leer secuencia");
  Serial.println("sin luz, porque valor é: ");
  Serial.println(sinLuz);

  // apagar luces que esten encendidas
  for (int i = numKeys; i >= 0; i--) {
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  
  if(sinLuz==1){
    for (int i = numKeys; i >= 0; i--) {
      Serial.println(secuencia1[i]);
      //si es 1 es que tiene que sonar
      if(secuencia1[i]==1){
        if(i==0 || i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28){
          if(secuencia1[i]==1){
            nota(low, 500);noTone(spk);delay(50);
          }
        }
        if(i==1 || i==5 || i==9 || i==13 || i==17 || i==21 || i==25 || i==29){
          if(secuencia1[i]==1){
            nota(mid, 500);noTone(spk);delay(50);
          }
        }
        if(i==2 || i==6 || i==10 || i==14 || i==18 || i==22 || i==26 || i==30){
          if(secuencia1[i]==1){
            nota(high, 500);noTone(spk);delay(50);
          }
        }
      }
      delay(100);
    }
  } else  {
    Serial.println("sin luz false");
    for (int i = numKeys; i >= 0; i--) {
      Serial.println(secuencia1[i]);
      //si es 1 es que tiene que sonar
      if(secuencia1[i]==1){
        if(i==0 || i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28){
          if(secuencia1[i]==1){
            trellis.setLED(i);
            trellis.writeDisplay();
            nota(low, 500);noTone(spk);delay(50);
          }
        }
        if(i==1 || i==5 || i==9 || i==13 || i==17 || i==21 || i==25 || i==29){
          if(secuencia1[i]==1){
            trellis.setLED(i);
            trellis.writeDisplay();
            nota(mid, 500);noTone(spk);delay(50);
          }
        }
        if(i==2 || i==6 || i==10 || i==14 || i==18 || i==22 || i==26 || i==30){
          if(secuencia1[i]==1){
            trellis.setLED(i);
            trellis.writeDisplay();
            nota(high, 500);noTone(spk);delay(50);
          }
        }
      }
      //si a pasado de fila un delay
      if(i==3 || i==7 || i==11 || i==15|| i==19|| i==23|| i==27|| i==31 ){
        delay(500);
      } 
      //  delay(200);
      trellis.clrLED(i);
      trellis.writeDisplay();
    }
  }
}
      
  
//reproducir la secuencia pulsada
void reproducirPulsadores(int secuancia1[]){
  int y = 0;
  int ledMatrices []={0,0, 0};
   
  //reproucidr pulsadores fila1
  for(int i=2;i<=0;i--){
    if(estadoPul[i]==1){
  
      //se copia en la matriz los led encendidos por la fila de tiempo
      ledMatrices[y]=i;
      y = y + 1;
      //trellis.setLED(i);
      //trellis.writeDisplay();
    } else {
      ledMatrices[y]=-1;
    }
    y = y + 1;
  }

  for(int i=2;i<=0;i--){   
    if(i==0 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==1 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==2 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }

  delay(50);

  for(int i=2;i<=0;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
  //reproucidr pulsadores fila2
  for(int i=6;i<=4;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }
  for(int i=6;i<=4;i--){   
    if(i==4 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);
    }
    if(i==5 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);
    }
    if(i==6 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);
    }
  }

  delay(50);
  for(int i=6;i<=4;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
  //reproducir fila 3
  for(int i=10;i<=8;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }

  for(int i=10;i<=8;i--){   
    if(i==8 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==9 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==10 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }

  delay(50);
  for(int i=10;i<=8;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);

  //reproducir fila 4
  for(int i=14;i<=12;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }
  for(int i=14;i<=12;i--){   
    if(i==12 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==13 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==14 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }
  delay(50);
  for(int i=14;i<=12;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  //reproducir fila 5
  for(int i=18;i<=16;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }
  for(int i=18;i<=16;i--){   
    if(i==16 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==17 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==18 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }
  delay(50);
  for(int i=18;i<=16;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
  //reproducir fila 6
  for(int i=22;i<=20;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }
  for(int  i=22;i<=20;i--){   
    if(i==20 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==21 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==22 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }
  delay(50);

  for(int  i=22;i<=20;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
  //reproducir fila 7
  for(int i=26;i<=24;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }

  for(int i=26;i<=24;i--){   
    if(i==24 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==25 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==26 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }
  delay(50);

  for(int i=26;i<=24;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
  //reproducir fila 8
  for(int i=30;i<=28;i--){
    if(estadoPul[i]==1){
      trellis.setLED(i);
      trellis.writeDisplay();
    }
  }

  for(int i=30;i<=28;i--){   
    if(i==28 && secuencia1[i]==1){
      nota(low, 500);noTone(spk);delay(100);
    }
    if(i==29 && secuencia1[i]==1){
      nota(mid, 500);noTone(spk);delay(100);
    }
    if(i==30 && secuencia1[i]==1){
      nota(high, 500);noTone(spk);delay(100);
    }
  }

  delay(50);

  for(int i=30;i<=28;i--){
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
  delay(50);
}


void  guardarArrayBucles(){
  Serial.println("guardar array bucle");
  //obtencion de inicio bucle
  int inicioBucle=0;
  boolean encontrado = false;
  //obtencion del nimero de repeticiones
  int posicionRepeticiones= 0;
  int numVeces= 0;
  boolean encontradoFin= false;
  //suma de incremento
  int incremento = 0;
  int finBucle= 0;
  for (int i = 1;  i < numKeys-1 ; i++){
    Serial.print("numero encontrado en el array");
    Serial. println (arrayBucle [i]);
    if (arrayBucle [i]== 19){
      break;
    }
    if (arrayBucle [i]== 27){
      Serial.print("se a encontrado el inicio bucle=");
      Serial.println (i);
      //comienzo del bucle a copiar
      inicioBucle = i+1;
      //buscar los 23, fin de bucle
      for (int j = inicioBucle ; j < numKeys; j++){
        if (arrayBucle [j]==23 ){
          Serial.print("el numero que va mierando en todo el bucle");
          Serial.println(arrayBucle[j]);
          if(23 != arrayBucle[j-1]){
            Serial.println(arrayBucle[j-1]);
            //se termina el bucle
            Serial.print ("se a encontrado el fin del bucle =");
            finBucle = j-1;
            Serial.println(finBucle);
            encontradoFin = true;
          }
        }
        if (arrayBucle [j]==23 && arrayBucle [j-1]==23){
          //suma si tiene una por delanre porque la primera siempre se hace
          numVeces = numVeces + 1;
        }
        if (encontrado && arrayBucle [j + 1] !=23){
          break;
        }
      }
      Serial.print("numveces =");
      Serial.println(numVeces);
      //bucle a copiar n veces
      for (int k = 0; k <= numVeces; k++){
        Serial.println("numero de veces que entra en el for de copiar");
        Serial.println(inicioBucle);
        Serial.println(finBucle);
        for (int h =inicioBucle ; h<= finBucle ; h++ ){
          Serial.println("lo que se copia es por bucle");
          Serial.print(arrayBucle [h]- incremento);
          estadoPul[arrayBucle [h]-incremento]=1;
        }
        //  if(arrayBucle[finBucle]==16){
        incremento = 16;
        //   }
        // incremento = incremento + 4;
        //Serial.print("el incremento es =");
        //Serial.println(incremento);
      }
      i = finBucle + numVeces;
      numVeces = 0;
    }
  }
  if(inicioBucle != 0){
    bucle = 1;
  }
  for (int i = numKeys; i >= 0; i--) {
    Serial.println(i);
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
}


void guardarBuclesVarios(){
  //obtencion de inicio bucle
  int inicioBucle=0;
  boolean encontrado = false;
  //obtencion del nimero de repeticiones
  int posicionRepeticiones= 0;
  int numVeces= 0;
  boolean encontradoFin= false;
  //suma de incremento
  int incremento = 0;
  int finBucle= 0;
  for (int i = 1;  i < numKeys-1 ; i++){
    Serial.print("numero encontrado en el array");
    Serial. println (arrayBucle [i]);
    if (arrayBucle [i]== 19){
      break;
    }
    if (arrayBucle [i]== 27){
      Serial.print("se a encontrado el inicio bucle=");
      Serial.println (i);
      //comienzo del bucle a copiar
      inicioBucle = i+1;
      //buscar los 23, fin de bucle
      for (int j = inicioBucle ; j < numKeys; j++){
        if (arrayBucle [j]==23 ){
          Serial.print("el numero que va mierando en todo el bucle");
          Serial.println(arrayBucle[j]);
          if(23 != arrayBucle[j-1]){
            Serial.println(arrayBucle[j-1]);
            //se termina el bucle
            Serial.print ("se a encontrado el fin del bucle =");
            finBucle = j-1;
            Serial.println(finBucle);
            encontrado = true;
          }
        }
        if (arrayBucle [j]==23 && arrayBucle [j-1]==23){
          //suma si tiene una por delanre porque la primera siempre se hace
          numVeces = numVeces + 1;
        }
        if (encontrado && arrayBucle [j + 1] !=23){
          j=numKeys;
        }
      }
      Serial.print("numveces =");
      Serial.println(numVeces);
      //bucle a copiar n veces
      for (int k = 0; k <= numVeces; k++){
        Serial.println("inicio del bucle");
        Serial.print(inicioBucle);
        Serial.println("fin del bucle");
        Serial.println(finBucle);
        for (int h =inicioBucle ; h<= finBucle ; h++ ){
          Serial.println("lo que se copia con un uno");
          Serial.print(arrayBucle [h]- incremento);
          estadoPul[arrayBucle [h]-incremento]=1;
        }
        incremento = incremento + 4;
        //Serial.print("el incremento es =");
        //Serial.println(incremento);
      }
      i = finBucle + numVeces;
      numVeces = 0;
    } else {
      estadoPul[arrayBucle [i]- incremento]=1;
    }
  }
  for (int i = numKeys; i >= 0; i--) {
    Serial.println(i);
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
}


void reinicioArrayBucle(){
  Serial.println("entra en resetear arraybucle");
  for (int i = 31;  i >= 0 ; i--){
    arrayBucle [i]= -1;
    estadoPul[i]= 0;
  }
}

    
//comprobar for tipo b
boolean comprobarSecuencia(int secuencia1[], int nivelJuego){
  Serial.print("Entra en comprobar");
  comprobarDosSecuencias(estadoPul, secuencia1);
  boolean siguiente;
  //comprobar
  Serial.print("mira sin son iguales");
  if (array_cmp(secuencia1, estadoPul, 15, 15) == true ){
    // do this if they are equal
    Serial.println("SON IGUALESS");
    correcto();
    //mandar datos
    subidaDatos(nivel, bloque, fallos, secuencia1, estadoPul, lecturas, bucle);
    siguiente = true;
    nivel = nivel+1;
    fallos= 0;
    lecturas = 0;
    bucle = 0;
  } else {
    // do this if they are different
    Serial.println(" NOOO SON IGUALESS");
    incorrecto();
    //mandar datos   
    fallos = fallos + 1;
    subidaDatos(nivel, bloque, fallos, secuencia1, estadoPul, lecturas, bucle);
    siguiente= false;
    bucle = 0;
  }
  delay(100);
  return siguiente;
}


//guardar los led que estan con luz
void guardarSecuencia(){
  Serial.print("Entra en guardarSecuencia");
  //guardar secuencia los leds pulsados 
  for (int i=0;i<numKeys;i++){
    if(i==3 || i==7 || i==11 || i==15|| i==19|| i==23|| i==27|| i==31 ){
      trellis.isLED(i);
      trellis.writeDisplay();
      estadoPul[i]=0;
    }
    estadoPul[i]=trellis.isLED(i);
    Serial.print(i); Serial.print(": ");
    Serial.println(estadoPul[i]);
    trellis.clrLED(i);
    trellis.writeDisplay();
  }
}


//guardar fragmento secuencia bucle
void guardarSecuenciaBucle(){
}


boolean array_cmp(int *a, int *b, int len_a, int len_b){
  int n;
  Serial.print("Entra en array_cmp primero");
  // if their lengths are different, return false
  if (len_a != len_b) return false;
  Serial.print("Entra en array_cmp segundo");
  // test each element to be the same. if not, return false
  for (n=0;n<len_a;n++) {
    if (a[n]!=b[n] && n!=11 && n!=31 ) {
      Serial.print(n);
      return false;
    }
  }
     
  
  //ok, if we have not returned yet, they are equal :)
  return true;
}


void comprobarDosSecuencias(int secuencia1[], int solucionCorrecta[]){
  Serial.print("entra en comprobar secuencia dos");
  for (int i = numKeys; i >= 0; i--) {
    Serial.println(secuencia1[i]);
    //si es 1 es que tiene que sonar
    if(secuencia1[i]==1){
      if(i==0 || i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28){
        if(secuencia1[i]==1){
          trellis.setLED(i);
          trellis.writeDisplay();
          nota(low, 500);noTone(spk);delay(50);
        }
      }
      if(i==1 || i==5 || i==9 || i==13 || i==17 || i==21 || i==25 || i==29){
        if(secuencia1[i]==1){
          trellis.setLED(i);
          trellis.writeDisplay();
          nota(mid, 500);noTone(spk);delay(50);
        }
      }
      if(i==2 || i==6 || i==10 || i==14 || i==18 || i==22 || i==26 || i==30){
        if(secuencia1[i]==1){
          trellis.setLED(i);
          trellis.writeDisplay();
          nota(high, 500);noTone(spk);delay(50);
        }
      }
    }
    //si a pasado de fila un delay
    if(i==3 || i==7 || i==11 || i==15|| i==19|| i==23|| i==27|| i==31 ){
      delay(100);
    } 
    delay(100);
  }
  for (int i = numKeys; i >= 0; i--) {
    Serial.println(solucionCorrecta[i]);
    //si es 1 es que tiene que sonar
    if(solucionCorrecta[i]==1){
      if(i==0 || i==4 || i==8 || i==12 || i==16 || i==20 || i==24 || i==28){
        if(solucionCorrecta[i]==1){
          nota(low, 500);noTone(spk);delay(100);
        }
      }
      if(i==1 || i==5 || i==9 || i==13 || i==17 || i==21 || i==25 || i==29){
        if(solucionCorrecta[i]==1){
          nota(mid, 500);noTone(spk);delay(100);
        }
      }
      if(i==2 || i==6 || i==10 || i==14 || i==18 || i==22 || i==26 || i==30){
        if(solucionCorrecta[i]==1){
          nota(high, 500);noTone(spk);delay(100);
        }
      }
      if(trellis.isLED(i)){
        trellis.clrLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.setLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.clrLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.setLED(i);
        trellis.writeDisplay();
      } else {
        trellis.setLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.clrLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.setLED(i);
        trellis.writeDisplay();
        delay(300);
        trellis.clrLED(i);
        trellis.writeDisplay();
      }
    }
    delay(100);
  }
  Serial.print("sale de la funcion");
  // poner todas las luces apagadas a la vez
  borrarLedJuntos();
}


//subida de los datos a base de datos
void subidaDatos(int nivel,int bloque,int fallos,int secuencia1 [],int estadoPul [],int lecturas,int bucle){
    String resultado= "";
  String respuesta= "";
  for (int i = 0 ; i<32; i++){
    resultado = resultado + secuencia1[i];
    respuesta= respuesta + estadoPul[i];
    resultado = resultado ;
    respuesta = respuesta ;
  }

      WiFly.begin(); 
  if (!WiFly.join(ssid, passphrase)) {
    Serial.println("Association failed.");
  }

  Serial.println("entra en conexion");
  Serial.print("nivel ");  
  Serial.println(nivel);
  Serial.print("bloque "); 
  Serial.println(bloque);
  Serial.print("fallos "); 
  Serial.println(fallos);
  Serial.print("iluminacion "); 
  Serial.println(iluminacion);
  Serial.print("respuesta ");  
  Serial.println(respuesta);
  Serial.print("resultado ");
  Serial.println(resultado);
  Serial.print("lecturas ");
  Serial.println(lecturas);
  Serial.print("uso de bucle ");
  Serial.println(bucle);
  Serial.println("conecting");

  if (client.connect()) {
    Serial.println("connected");
    client.print("GET /arduino/salvardatos.php?");
    client.print("nivel=");
    client.print(nivel);
    client.print("&bloque=");
    client.print(bloque);
    client.print("&fallos=");
    client.print(fallos);
    client.print("&iluminacion=");
    client.print(iluminacion);
    client.print("&respuesta=");
    client.print(respuesta);
    client.print("&resultado=");
    client.print(resultado);
    client.print("&lecturas=");
    client.print(lecturas);
    client.print("&bucle=");
    client.println(bucle);
    client.println();
  } else {
    Serial.println("connection failed");
  }
   
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    for(;;);
  }  

}
  

void   borrarLedJuntos(){
  trellis.clrLED(0);
  trellis.clrLED(1);
  trellis.clrLED(2);
  trellis.clrLED(3);
  trellis.clrLED(4);
  trellis.clrLED(5);
  trellis.clrLED(6);
  trellis.clrLED(7);
  trellis.clrLED(8);
  trellis.clrLED(9);
  trellis.clrLED(10);
  trellis.clrLED(11);
  trellis.clrLED(12);
  trellis.clrLED(13);
  trellis.clrLED(14);
  trellis.clrLED(15);
  trellis.clrLED(16);
  trellis.clrLED(17);
  trellis.clrLED(18);
  trellis.clrLED(19);
  trellis.clrLED(20);
  trellis.clrLED(21);
  trellis.clrLED(22);
  trellis.clrLED(23);
  trellis.clrLED(24);
  trellis.clrLED(25);
  trellis.clrLED(26);
  trellis.clrLED(27);
  trellis.clrLED(28);
  trellis.clrLED(29);
  trellis.clrLED(30);
  trellis.clrLED(31);
  trellis.clrLED(32);
  trellis.writeDisplay();
}

      
//VOID NOTA
void nota(int frec, int t){
  tone(spk,frec);      // suena la nota frec recibida
  delay(t);                // para despues de un tiempo t
}


//// sonido correcto
void correcto(){
  nota(d[1],150);noTone(spk);delay(50);
  nota(d[1],150);noTone(spk);delay(50);
  nota(d[1],150);noTone(spk);delay(50);
  nota(g[1],900);noTone(spk);delay(150);
  //nota(d[2],900);noTone(spk);delay(50);
  //nota(c[2],150);noTone(spk);delay(50);
  //nota(b[1],150);noTone(spk);delay(50);
}


//// sonido incorrecto
void incorrecto(){
  nota(c[2],250);noTone(spk);delay(50);
  nota(c[1],250);noTone(spk);delay(50);
  nota(c[0],400);noTone(spk);delay(50);
  //nota(g[1],900);noTone(spk);delay(150);
  //nota(d[2],900);noTone(spk);delay(50);
  //nota(c[2],150);noTone(spk);delay(50);
  //nota(b[1],150);noTone(spk);delay(50);
}
