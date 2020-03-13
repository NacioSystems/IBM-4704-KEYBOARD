// Intento de lectura de teclado IBM modelo F
// Referencias en Github...
// Utilizamos A3 para lectura del reloj
//          y A4 para lectura y escritura de datos

#define CLK  A3
#define DATA A2
#define Tup   60 // son microsegundos de reloj en up
#define Tdown 30 // son 30 microsegundos de rejol down
#define TStart 300 // en el inicio de 300 a 350 microsegundos
#define error 0xFF // Devolveremos un error en caso de que falle la lecura

// COMANDOS KB
#define RESET 0xFA
#define SOFT_RESET 0xFF
#define BUZZER 0xFD
#define SKF 0xFC        // Soft Key Flag
#define RESEND 0xFE

#include "KbspIBM4073.h" // Es una librería adaptada de Keyboard.h para el 4703 
//#include "Keyboard.h";

// Variables generales
volatile unsigned long Ttotal, Tabajo, Talto;
volatile boolean HayDato=false;
volatile char Dato=0x00;
//uint16_t Clk[250];
//uint16_t Data[250];
uint8_t ValorTecla=0;
int contador=0;

boolean SHIFT = false;
boolean ALT=false;
boolean CONTROL=false;
boolean BREAK_CODE=false;
boolean FUNCION_E=false;


char CodTeclado[0x48] = {
    // 00         01    02    03    04         05    06    07         
       KEY_ESC,   'i',  'o',  'p',  KEY_TAB,   'q',  'w',  ' ',
       
    // 08    09    0A    0B    0C               0D     0E               0F 
       '7',  '8',  '9',  ' '  ,KEY_BACKSPACE,   0x27,  KEY_BACKSPACE,   '0',
       
    // 10    11    12    13    14    15    16    17
       '4',  '5',  '6',  'e',  'r',  't',  'y',  'u',
       
    // 18    19    1A    1B`    1C+    1D            1E    1F'
       '1',  '2',  '3',  0x5B,  0x5D,  KEY_DELETE,   ' ',  0x2D,  // KEY_ACENTO2 1B

    // 20               21a    22    23    24    25    26    27
       KEY_CAPS_LOCK,   0X61,  's',  'd',  'f',  'g',  'h',  'j',

    // 28    29    2Añ    2B'    2Cç    2D            2E    2F
       'k',  'l',  0x3B,  0x27,  0xBF,  KEY_RETURN,   ' ',  KEY_RIGHT_ALT, // KEY_CEDILLA 2C

    // 30                31               32    33    34    35    36b    37
       KEY_LEFT_SHIFT,   KEY_LEFT_CTRL,   'z',  'x',  'c',  'v',  'b',  'n',

    // 38    39    3A    3B/    3C                3D                 3E<    3F
       'm',  ','  ,'.',  0X2F,  KEY_RIGHT_SHIFT,  KEY_RIGHT_SHIFT,   0xB7  ,  KEY_LEFT_ALT,

    // 40    41               42              43    44    45    46    47
       ' ',  KEY_RIGHT_GUI,   KEY_LEFT_GUI,   ' ',  ' ',  ' ',  ' ',  ' '    
};


void setup() {
  Keyboard.begin();
  Serial.begin(57600); // Utilizaremos el puerto serie para salida  
  delay(100);
   
  // Enviamos un comando FC, antes un comando Set Key Flag
  // para cada tecla hay que enviar el bit 7 a 1
  // Por algún motivo, el teclado envía normalmente el release de las teclas ALT
  // Y la función SetKeyFlag no hace lo que debería
  
  // Esperamos que arranque el teclado y envíe un 0xA3h, que no leemos
  delay(3000); // 1 segudo será suficiente
  Serial.println("Programado por Ignacio Otero (2020)\n");
  Serial.println("Reseteamos el teclado y leemos el codigo del teclado: ");
  EnviarComando(RESET);
  delayMicroseconds(2000);
  
  ValorTecla=LeerTeclado();
  Serial.print("Teclado Kishsaver IBM Modelo F200 detectado: ");
  Serial.println(ValorTecla,HEX);

  Serial.println("\nPonemos en modo break las teclas SHIFT, CTRL y ALT");
  // Estamos en modo comando SFK
  // Ponemos la tecla shift en modo break
  delay(10);
  EnviarComando(SKF); // Por si las moscas pasamos a SFK
  delayMicroseconds(280);

  // ALT YA ESTÁ POR DEFECTO

  // SHIFT
  EnviarComando(0xB0); // shift izquierda es 0x30 + 0x80 = 0xB0
  delayMicroseconds(280);
  EnviarComando(0xBD); // Derecha
  delayMicroseconds(280);

  // CONTROL
  EnviarComando(0xB1); //Izquierdo
  delayMicroseconds(280);
  EnviarComando(0xAF); //Derecho
  delayMicroseconds(280);

  // TECLA MENÚ INICIO PARA FUNCIONES ESPECIALES
  EnviarComando(0xC1); // Tecla menú Izquierdo
  delayMicroseconds(280);

  // BARRA ESPACIADORA EN MODO REPETICIÓN
  EnviarComando(0x40); // Espacio es 0x40
  delayMicroseconds(280);

  // TECLA BORRAR, EN MODO REPETICIÓN
  EnviarComando(0x0E); // Back es 0x0E
  delayMicroseconds(280);

  // SUPRIMIR, EN MODO REPETICIÓN
  EnviarComando(0x1D);
  delayMicroseconds(280);

  // TECLAS CURSORES EN MODO REPETICIÓN
  EnviarComando(0x35); // V <-
  EnviarComando(0x36); // B \/
  EnviarComando(0x37); // N ->
  EnviarComando(0x25); // G /\

  //Serial.println("Ponemos break codes en marcha");  
  //SetKeyFlag();
  
  // Salimos del modo comando
  delay(1); 
  EnviarComando(SOFT_RESET); //Comando FC = 0xFF Soft Reset
  Serial.println("Listos para leer teclado.");
  delay(1000);
}

void loop() {

// leemos teclado
ValorTecla=LeerTeclado();
if (ValorTecla!=0xFF) {
      // Hay valor, imprimirmos el valor
      Serial.print(" Codigo tecla:");      //
      Serial.print(ValorTecla, HEX);
      Serial.print(" Codigo enviado: ");
      Serial.println((uint8_t) CodTeclado[ValorTecla],HEX);
      //Serial.print(" Tecla: ");

      // Si es ESCAPE levantamos y borramos los modificadores y teclas pulsadas
      if(ValorTecla==0x00) {
           SHIFT = false;
           ALT=false;
           CONTROL=false;
           BREAK_CODE=false;
           FUNCION_E=false;
           Keyboard.releaseAll();
           }
      // TECLAS RARAS QUE NO COINCIDEN EN EL TECLADO NORMAL           
      // Otros valores, teclas modificadoras y otras teclas cambiadas:
      // Paréntesis y corchetes {[]}, @
      // MODIFICADORES:
      switch (ValorTecla) {
          // EN FUNCIÓN DE LAS TECLAS HACEMOS...
          // TECLAS MODIFICADORES Y TECLAS RARAS
          //          
          case 0x30: // BLOQUEO MAYUSCULAS
              SHIFT=false; 
              Keyboard.release(CodTeclado[ValorTecla]);
              break;
          case 0xB0: // SHIFT
              SHIFT=true;
              Keyboard.press(CodTeclado[0x30]);
              break;
          case 0x3F: // ALT. ALT tiene el mismo código en los dos lados
              ALT=false;
              Keyboard.release(CodTeclado[ValorTecla]);
              break;
          case 0xBF: // ALT
              ALT=true;
              Keyboard.press(CodTeclado[0x3F]);
              break;
          case 0x31: // CONTROL
              CONTROL=false;
              Keyboard.release(CodTeclado[ValorTecla]);
              break;
          case 0xB1: // CONTROL
              CONTROL=true;
              Keyboard.press(CodTeclado[0x31]);
              break;
         case 0xC1: // FUNCIONES ESPECIALES, MENÚ INICIO
              FUNCION_E=true;
              //Keyboard.press(CodTeclado[0x41]);
              break; 
         case 0x41: // FUNCIONES ESPECIALES
              FUNCION_E =false;
              //Keyboard.release(CodTeclado[0x41]);
              break;
         // TECLAS RARAS     
         case 0x1A: // La @ está en el 3 no en el 2
              if(SHIFT) {
                   Keyboard.press(KEY_LEFT_CTRL);
                   Keyboard.press(KEY_LEFT_ALT);
                   Keyboard.press(0x32); // Tecla 2 que es arroba" 
                   }
              else Keyboard.press(CodTeclado[ValorTecla]); // Tecla 3
              delay(50);
              Keyboard.releaseAll();
              //Keyboard.press(KEY_LEFT_SHIFT); // Estábamos con Shift lo dejamos arriba
              break;
         case 0x0D: // { Control + alt + tecla acento
              if(!SHIFT) { // Abre corchete {
                      Keyboard.press(KEY_LEFT_CTRL);
                      Keyboard.press(KEY_LEFT_ALT);
                      Keyboard.press(0x27);
                      }
              else { // [
                      Keyboard.press(KEY_LEFT_CTRL);
                      Keyboard.press(KEY_LEFT_ALT);
                      Keyboard.press(0x5B);
              }
              delay(50);
              Keyboard.releaseAll();
              break;
          case 0x2C: // } Control + alt + tecla CEDILLLA
              if(FUNCION_E) { // Cierra corchete }
                     Keyboard.press(0x5C);
                     Keyboard.release(0x5C);
              }
              else
              if(!SHIFT) { // Cierra corchete 1 ]
                      Keyboard.press(KEY_LEFT_CTRL);
                      Keyboard.press(KEY_LEFT_ALT);
                      Keyboard.press(0xBA);
                      Keyboard.releaseAll();
                      }
              else { // ]
                      Keyboard.press(KEY_LEFT_CTRL);
                      Keyboard.press(KEY_LEFT_ALT);
                      Keyboard.press(0x5D);
                      Keyboard.releaseAll();
              }
              break;
              
          default:
              if(!FUNCION_E) {  // TELCAS NORMALES
                  Keyboard.press(CodTeclado[ValorTecla]);
                  delay(50);
                  Keyboard.release(CodTeclado[ValorTecla]);
              }
              else { // TECLAS ESPECIALES
                //MOVIMIENTO CURSOR
                        switch (ValorTecla) {
                           case 0x35: // V Izquierda
                              Keyboard.press(KEY_LEFT_ARROW);
                              break;

                           case 0x36: // B Abajo
                              Keyboard.press(KEY_DOWN_ARROW);
                              break;

                           case 0x37: // N Derecha
                              Keyboard.press(KEY_RIGHT_ARROW);
                              break;

                           case 0x25:  // G Arriba
                              Keyboard.press(KEY_UP_ARROW);
                              break;
                              
                // TECLAS INCICIO, AVANCE PÁG Y RET. PÁG.
                           case 0x01:  // I Incio 'i'
                              Keyboard.press(KEY_HOME);
                              break;
                           case 0x24:  // F Fin 'f'
                              Keyboard.press(KEY_END);
                              break;
                           case 0x21:  // A Av Pag 'a'
                              Keyboard.press(KEY_PAGE_DOWN);
                              break;
                           case 0x14:  // R Re Pag 'r'
                              Keyboard.press(KEY_PAGE_UP);
                              break;
                              
                // IMPRIMIR PANTALLA
                           case 0x03:  // p Imprimir Pantalla
                              //Keyboard.press(0x2C); // Desconozco el codigo
                              break;
                        
                // TECLAS FUNCIÓN
                           case 0x18: // F1 o ¡
                              if(SHIFT) {
                                 Keyboard.release(KEY_LEFT_SHIFT); // Quitemos el SHIFT
                                 Keyboard.press(61);// abre admiración ¡
                                 //Keyboard.press(KEY_LEFT_SHIFT); // Volvemos a dejar SHIFT
                              }
                              else Keyboard.press(KEY_F1); 
                              break;
                           case 0x19: // F2
                              Keyboard.press(KEY_F2);
                              break;
                           case 0x1A: // F3
                              Keyboard.press(KEY_F3);
                              break;
                           case 0x10: // F4
                              Keyboard.press(KEY_F4);
                              break;
                           case 0x11: // F5
                              Keyboard.press(KEY_F5);
                              break;
                           case 0x12: // F6
                              Keyboard.press(KEY_F6);
                              break;
                           case 0x08: // F7
                              Keyboard.press(KEY_F7);
                              break;
                           case 0x09: // F8
                              Keyboard.press(KEY_F8);
                              break;
                           case 0x0A: // F9
                              Keyboard.press(KEY_F9);
                              break;
                           case 0x0F: // F10
                              Keyboard.press(KEY_F10);
                              break;
                           case 0x1F: // F11 o ¿
                              if(SHIFT) {
                                Keyboard.press(KEY_LEFT_SHIFT);
                                Keyboard.press(61);// abre pregunta
                                //Keyboard.press(KEY_LEFT_SHIFT);
                                }
                              else Keyboard.press(KEY_F11);
                              break;
                           case 0x0D: // F12
                              Keyboard.press(KEY_F12);
                              break;

                           // Otras teclas raras
                           case 0x02: // º
                              if(SHIFT) {
                                Keyboard.press(126);   //ª
                                }
                              else Keyboard.press(0x60);       //º
                              break;
                          }
               
               Keyboard.releaseAll(); // Soltamos todas las teclas
               // Si tenemos pulsado el SHIFT lo volvemos a poner
               if(SHIFT) Keyboard.press(KEY_LEFT_SHIFT);
               delay(50); 
              }
          }

       
      }
}

// ENVIAR COMANDO
void EnviarComando(uint8_t Comando) {
/*Host to Keyboard
----------------
No lo dice en ninguna parte, pero el Keyboard hace siempre de Master, controlando el CLK.
El PC sólo lee el CLK del teclado y lo pone a LOW para herle saber que le va a envar datos;
liberando después el CLK para que lo controle el KB.

Data bits are LSB first and Pairty is odd. Clock has around 60us high and 30us low part.

            ____        __   __   __   __   __   __   __   __   __   ________
    Clock       \______/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/
                ^   ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ___
    Data    ____|__/    X____X____X____X____X____X____X____X____X____X   \___
                |  Start   0    1    2    3    4    5    6    7    P   Stop
                Request by host

Start bit:  can be long as 300-350us during start up and upto 2500us while key scanning
Request:    Host pulls Clock line down to request to send a command.
Timing:     After Request keyboard pull up Data and down Clock line to low for start bit.
            After request host release Clock line once Data line becomes hi.
            Host wirtes a bit while Clock is hi and Keyboard reads while low.
Stop bit:   Host releases or pulls up Data line to hi after 9th clock and waits for keybaord pull down the line to lo.

Keyboard command
----------------
Keyboard accepts these commands from host.

        Description                             Entry point
    -----------------------------------------------------------
    FF  Soft Reset                              0008h
    FE  Resend                                  00e8h
    FD  Buzzer(emits a short beep)              00edh
    FC  Set Key Flag                            00f6h
    FB  Soft Reset                              0008h
    FA  Reset                                   0000h
*/

  uint8_t bit=0x01; // Ponemos un bit a 1
  uint8_t PARIDAD=1; // Par 0, impar 1
  // Por algún motivo no funciona empezando en 0
  
  // Comprobamos que estamos en condiciones de enviar datos
  pinMode(DATA,INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);

  while((digitalRead(CLK)!=HIGH)&(digitalRead(DATA)!=LOW)) {
    //Esperamos
  }
  // Podemos enviar el comando
  Serial.print("Comando: ");
  switch(Comando) {
      case 0xFA:
          Serial.print("RESET ");
          break;
      case 0xFB:
          Serial.print("SOFT RESET ");
          break;
      case 0xFC:
          Serial.print("SET KEY FLAG ");
          break;
      case 0xFE:
          Serial.print("RESEND ");
          break;
      case 0xFD:
          Serial.print("BUZZER ");
          break;
      default:
          if(Comando&0x80) Serial.print("BREAK CODE ");
          else Serial.print("ENABLE REPEAT ");
      }
  Serial.println(Comando, HEX);
  
  // Empezamos el start bit poniendo CLK a 0 durante 300 usegudos
  habilitaKB();
  clock_lo();

  // Esperamos por el Start bit, KB responda poniendo el DATA en alto
  while(digitalRead(DATA)==LOW) {
    //Esperamos
    }
  // Empieza el Start bit
  // Liberamos el CLK
   pinMode(CLK, INPUT_PULLUP);
   pinMode(DATA,OUTPUT);
   while(digitalRead(CLK)==LOW) {
    // Esperamos el reloj para escribir
   }

   
   for(int i=0;i<8;i++) {
            // Escribimos con el CLK arriba
            if(Comando&bit) {
                digitalWrite(DATA, HIGH);
                PARIDAD++; // Vamos contanto la paridad
                }
            else {
                digitalWrite(DATA, LOW);
                }
            Comando=Comando>>1; // Corremos un bit a la derecha, de LSB a MSB
            while(digitalRead(CLK)==HIGH) {
              //Esperamos que baje
            }
            
            //Ciclo reloj abajo 30 us
            // Aquí lee el KB
            while(digitalRead(CLK)==LOW) {
              // Esperamos
            }
  }
    //Ponemos paridad
    //Ciclo reloj arriba 60 us
    // Data se escribe en alto
    if(PARIDAD&bit) {
        digitalWrite(DATA, HIGH); // suponiendo paridad impar ?
        } 
    else {
         digitalWrite(DATA, LOW);
         }
    while(digitalRead(CLK)==HIGH) {
       // Esperamos
       }
    //Ciclo reloj abajo 30 us
    // El teclado lee en el ciclo bajo de CLK
    while(digitalRead(CLK)==LOW) {
       // Esperamos
       }

  //Bit de STOP, dejamos el reloj arriba
  //dejamos el data arriba y eperamos que el teclado lo baje
  digitalWrite(DATA, HIGH);
  // Ponemos como entradas para escuchar
  pinMode(DATA, INPUT_PULLUP);
  while(digitalRead(DATA)==HIGH) {
    // Esperamos que el KB baje el DATA
    }
 habilitaKB(); // Habilitamos KB
  

  delayMicroseconds(30);
  // Deberíamos esperar a que el teclado baje el DATA LOW
  //Serial.print(" Paridad: ");
  //if(PARIDAD&0x01) Serial.println("odd"); else Serial.println("even");
}


// LEER TECLADO
uint8_t LeerTeclado(void) {
/*
Keyboard to Host
----------------
Data bits are LSB first and Pairty is odd. Clock has around 60us high and 30us low part.

            ____       __   __   __   __   __   __   __   __   __   _______
    Clock       \_____/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/  \_/
                 ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
    Data    ____/    X____X____X____X____X____X____X____X____X____X________
                Start   0    1    2    3    4    5    6    7    P  Stop

Start bit:  can be long as 300-350us.
Inhibit:    Pull Data line down to inhibit keyboard to send.
Timing:     Host reads bit while Clock is hi.(rising edge)
Stop bit:   Keyboard pulls down Data line to lo after 9th clock.

Keyboard response
-----------------
Keyobard sends these bytes to host.

        Description                             Entry point
    -----------------------------------------------------------
    FE  Overflow(key event/receive data)        00c5h, 0346h
        Memory test error                       0224h
    FD  Command out of bound                    00d8h
        Key out of bound
    7E  Read/Parity error in receive from host  00bch
    80-FB? scan code(make)
    00-7B? scan code(break)
    note: Alps model spits scan code 7B(FB) at least.

 */
  
  uint8_t LECTURA_BITS=0x01; // Es el bit menos significativo por la LSB
  uint8_t KB_LECTURA=0;   // Guardamos los tatos aquí
  uint8_t PARIDAD=1; // Par 0, impar 1
  
  // Ponemos como entradas para escuchar
  pinMode(DATA, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  //Esperamos que el CLK baje para comprobar que DATA sube
  if((digitalRead(CLK)==LOW)&&(digitalRead(DATA)==HIGH)) {
            // Habrá mensaje y lo leemos
            // Esperamos que el reloj suba
            while(digitalRead(CLK)==LOW) {
              // Esperamos a que suba pueden ser 300 us
              }
            // El reloj está arriba. Leemos 8 veces los datos
            for(int i=0;i<8;i++) {
                  delayMicroseconds(5); // Esperamos un poco para leer
                  // CLK está HIGH, tomamos el dato en la subida
                  if(digitalRead(DATA)) {
                          KB_LECTURA|=LECTURA_BITS;
                          PARIDAD++; // Vamos contanto la paridad
                          }
                  LECTURA_BITS=LECTURA_BITS<<1; // Desplazamos el bit a la izquierda
                  // el pulso alto dura 60us y el bajo 30
                  while(digitalRead(CLK)==HIGH) {
                     // Esperamos que baje el reloj
                     }
                  // ahora esperamos otra vez que el reloj suba
                  while(digitalRead(CLK)==LOW) {
                        // 60+30 -5 -70 pueden ser unos us
                    }     
            }
            // Ahora el dato leído está en KB_LECTURA
            // Falta la paridad
            delayMicroseconds(5);
            // CLK está HIGH, tomamos el dato en la subida
            LECTURA_BITS=digitalRead(DATA);
            // el pulso alto dura 60us y el bajo 30
            // Esperamos 70 ux
            delayMicroseconds(70);
            // ahora esperamos otra vez que el reloj suba
            while(digitalRead(CLK)==LOW) {
                  // 60+30 -5 -70 pueden ser unos 15 us
            }    
            // bit de parada
            // Ahora KB_LECTURA tiene el valor y LECTURA_BITS la paridad
            if(KB_LECTURA==0xFE) Serial.println("\nOverflow key event/recibe data. ");
            if(KB_LECTURA==0xFD) Serial.println("\nOut of bound. ");
            if(KB_LECTURA==0x7E) Serial.println("\nParity error. ");
            
            return KB_LECTURA; 
            }

  else return 0xFF; // No hay mensajes
}

//SET KEY FLAG MODO COMANDO TODAS TECLAS BREAK
// Esa función pone todas las teclas en modo Set Flag
// Se envía el carácter de la tecla y el mismo carácter con el código OR 0b10000000
void SetKeyFlag(void) {
/*
Set Key Flag command(FC)
------------------------
After 'Power on Reset' firmware enters this command mode and waits for data from host,
so that you don't need to send 'FC' and it doesn't send any scancode until you send 'FF' to exit this mode.
With Alps models you need to send 'FC' command to enter this mode.

Data sent from host:

    bit: 7   6 ... 0
         en  |     |
         |   `-----`--- scan code
         `------------- enable bit(0: enable repeat, 1: enable break)

    00-7B   Enable repeat
    80-FB   Enable break
    FE      Resend(011ah) no need to use
    FF      End(0114h) exits FC command mode.

Response from keyboard:
    FD      Out of bound - Invalid scancode
    --      OK - No response means that command is accepted.

Examples:
    To enable break code of all keys you have to send following commands.

    FC 80 81 ... FB FF

 */  
  uint8_t Inicio=0x00;
  uint8_t Final=0x42;
  // Entramos en modo comando FC
  EnviarComando(0xFC);
  for(uint8_t i=Inicio;i<=Final;i++) {
    EnviarComando(i|0x80);
    delayMicroseconds(280);
    // Esperar 280us no importa mucho. Si el teclado nos quiere decir algo
    // tendrá que lanzar un Start bit de 300us, por lo que no pasa nada.
    //Leemos respuesta si hay
    ValorTecla=LeerTeclado();
    if (ValorTecla!=0xFF) {
          // Hay valor, imprimirmos el valor
          Serial.print("Respuesta: ");
          Serial.println(ValorTecla, HEX);
          }
    else Serial.println("- OK");
  }
  // Salimos de modo comando
  EnviarComando(0xFF);
  delayMicroseconds(280);
}


// HABILITA KEYBOARD
// Esta deja que el teclado envíe datos
void habilitaKB(void) {
  clock_hi();
  data_hi();
}

//INHABILITA KEYBOARD
// Esta función inhabilita que el teclado envíe datos
void inhabilitaKB(void) {
  clock_hi();
  data_lo();
}


// CLOCK LOW
void clock_lo() {
  pinMode(CLK, OUTPUT);
  digitalWrite(CLK, LOW);
}

// CLOCK HIGH
void clock_hi() {
  pinMode(CLK, OUTPUT);
  digitalWrite(CLK, HIGH);
}

// DATA LOW
void data_lo() {
  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, LOW);
}

// DATA HIGH
void data_hi() {
  pinMode(DATA, OUTPUT);
  digitalWrite(DATA, HIGH);
}