/*
 * Este archivo de configuración permite cambiar las condiciones en las que está progarmado el Hardware
 * 
 * Esta basado en :
 *       - Un Arduino Pro Micro
 *       - Un teclado IMB 4704, de 62 teclas español, cuyo hardware funciona
 *       - No se disponde de la caja original ni del cable de salida del teclado para conexión a la CPU
 *       - Las salidas del teclado están conectadas directamene a la placa base. 
 *             Visto de frente, teclas hacia arriba, el conector tiene dos líneas de 5 pines:
 *                      Vcc       GND  Data CLK
 *                       x    x    x    x    x
 *                       x    x    x    x    x
 *                       
 *       - En el Arduino los puertos son:
 *       
 *                      Vcc  - Vcc (5V)
 *                      GND  - GND (0V)
 *                      CLK  - A3
 *                      Data - A2
 *                      
 *   No se han establecido tiempos máximos de espera para las comunicaciones, aunque después de muchos días de uso no
 *   se han tinido problemas en las comunicaciones Arduino - IBM 4704. Lo recomendable es establecer unos tiempos máximos
 *   de espera, ya que en caso de que el teclado se quede colgado, los tiempos del reloj no cumplan el protocolo, el Arduino 
 *   lo sepa, pueda abortar el proceso en el que esté y si es necesario hacer un reset al teclado.
 *   
 *   
 *   REFERENCIAS DE SOARER:
Scancodes
---------
Keyboard doesn't send Break code for all keys except for Alt by default.

    6019284 62-key:
    ,-----------------------------------------------------------.
    |  `|  1|  2|  3|  4|  5|  6|  7|  8|  9|  0|  -|  =|**1|BS |
    |-----------------------------------------------------------|
    |Tab  |  Q|  W|  E|  R|  T|  Y|  U|  I|  O|  P| ￠|  \|  PD2|
    |-----------------------------------------------------------|
    |Ctrl  |  A|  S|  D|  F|  G|  H|  J|  K|  L|  ;|  '| {}| PD3|
    |-----------------------------------------------------------|
    |Shif| <>|  Z|  X|  C|  V|  B|  N|  M|  ,|  ,|  /|**2|Shift |
    |-----------------------------------------------------------|
    |Reset|blk|Alt  |         Space             |Alt  |blk|Enter|
    `-----------------------------------------------------------'
    +----------+---------------------+----------+----------+
    |`       00|PD1     04|Caps    20|LShift  30|Reset   31|
    |1       18|q       05|a       21|<>      3E|Rblank  41|
    |2       19|w       06|s       22|z       32|Alt     3F|
    |3       1A|e       13|d       23|x       33|Space   40|
    |4       10|r       14|f       24|c       34|Alt     3F|
    |5       11|t       15|g       25|v       35|Lblank  42|
    |6       12|y       16|h       26|b       36|Enter   2F|
    |7       08|u       17|j       27|n       37|          |
    |8       09|i       01|k       28|m       38|          |
    |9       0A|o       02|l       29|,       39|          |
    |0       0F|p       03|;       2A|.       3A|          |
    |-       1F|￠      1B|'       2B|/       3B|          |
    |=       0D|\       1C|{}      2C|**2     3C|          |
    |**1     0C|PD2     1D|PD3     2D|RShift  3D|          |
    |BS      0E|          |          |          |          |
    +----------+---------------------+----------+----------+
    Bit7 is 'press flag' which set 1 on press and 0 on release when break code is enabled.

NOTE: When break code is enabled the key sends scancode with setting 7th bit on press
and without it on release. That is, '`' sends 80h on press and 00h on release.
 *   
 *   
 *   TECLAS MODIFICADORAS:
 *   Para mantener la referencia con los teclados actuales, las teclas modificadoras son las siguientes
 *      CRTL Izquierdo: Es la tecla Reset
 *      MENÚ Izquierdo: Es la tecla sin marcar entre RESET Y ALT
 *      ALT Izquierdo: Es la tecla ALT
 *      ALT Derecho: Es la tecla ALT derecha. Esta tecla tiene el mismo cödigo de escaneo que la tecla izquierda
 *      MENÚ Derecho: Es la tecla sin marcar entre ALT e INTRO. Esta tecla se dejó sin funcionar, martilo roto
 *      CRTL Derecho: Es la tecla Intro. Esta tecla se dejó sin funcionar, es el otro martilo roto.
 *      ESC: La tecla "escape" es la tecla superior izquirda sin marcar. Tamibien hace las funcione de \ en combinació con CRTL y ALT
 *   
 *   
 *   TECLAS ESPECIALES PARA FUNCINAMIENTO DE UN TECLADO NORMAL:
 *   ==========================================================
 *   Las teclas especiales se activan pulsando la tecla de "menú" izquieda, entre (RESET) y (ALT).
 *   
 *   Teclas de Función:
 *        Se pulsa la tecla de "menú" y el número correspondiente, F! a F10, F11 con ' y F12 con {
 *        
 *   Reset:
 *        Para resetear Ardunio Leonardo y teclado IBM, pulsar "menú" y suprimir PD2<-
 *        
 *   Teclas de Cursor:
 *        La mejor opción que he considerado es utilizar las tres teclas marcadas de la fila inferior,
 *        estas son V (Prba), B (Cursr) y N (Parpa), que hacen las funciones de cursor <, \/, >
 *        La tecla de cursor arriba /\ será la G.
 *        Se activan pulsando la tecla "menú" y la correspndiene del cursor
 *        
 *   Teclas Inicio y Fin:
 *        De igual manera, se activan pulsando la tecla "menú" y I para Inicio (Home) y F para Fin (End)
 *        
 *   Teclas Avance página y Retroceso página:
 *        En este caso se utilizan las teclas "menú" enconbinación con A (AvPág) y R (RePág)
 *        
 *   Tecla Suprimir:    
 *        Pasa a ser la tecla PD2 |<-- directamente
 *        
 *   Tecla Enter:
 *        Es la tecla PD3 <-|
 *        
 *   Tecla Tab:
 *        Es la tecla PD1 -->|
 *        
 *        
 *   Teclas que no aparecen en el "layout":     
 *   Se ha intentado respetar el marcado de las teclas, para que reproduzcan el carácter que figura,pero
 *   hay varias teclas que no aparecen marcadas en el teclado IBM, pero que son habituales en un teclado estándar.
 *   A estas teclas se accede pulsando CRTL + ALT, si son teclas gráficas, con su corresondiene tecla habitual estandar,
 *   por ejemplo CRTL+ALT+2 = '@', CRTL+ALT+E = '€', CRTL+ALT+3 = '#'. Si el gráfico aparece en el teclado como mayúscula, tambien se accede
 *   a él de forma normal, SHIFT+Tecla, por ejemplo SIHT+3 = '@', SHIFT+{ = '['.
 *   
 *   En este teclado viene el acento francés '`' marcado, pero no viene el acento español '´'. Como hay una tecla en la que la mayúscula 
 *   es la tecla '¨', se utilizó esta tecla en la posicion de la tecla del acento español, aunque el carácter gráfico es '¬'.
 *   
 *   Pero hay otras teclas que no aparecen en el marcado del teclado, ni hay tecla similar. Por ejemplo las teclas 'º', 'ª', 'ç'.
 *   Para acceder a estas teclas se utiliza la tecla función "menú" de la siguiente manera:
 *              'º' Pulsando "menú" y 'o'
 *              'ª' Pulsando "menú"+SHIFT y 'o'
 *              'ç' Pulsando "menú" y '}', ya que 'ç' suele estar en esta posición
 *              '\' Pulsando "menú" y '`', ya que aparece rotulada en esa posicion.
 *              '\' También se accede pulsando CRTL+ALT+ESC
 *              '|', '@', '#' son accesibles desde su posición habitual, CRTL+ALT+ el número correspondiete (1, 2, 3)
 *   
 *        
 */
 


#define CLK  A3
#define DATA A2

// Estos tiempos los utilizaba cuando quería controlar el CLK desde Pro Micro
#define Tup   60 // son microsegundos de reloj en up
#define Tdown 30 // son 30 microsegundos de rejol down
#define TStart 300 // en el inicio de 300 a 350 microsegundos


// COMANDOS KB
#define RESET 0xFA
#define SOFT_RESET 0xFF // Tambien es 0xFB
#define BUZZER 0xFD
#define SKF 0xFC        // Soft Key Flag
#define RESEND 0xFE
#define BREAKCODE 0x80  // Hay una variable booleana BREAK_CODE que en la practica no utilizo

// RESPUESTAS KB
#define OVERFLOW 0xFE
#define OUTOFBOUND 0xFD
#define PARITYERROR 0x7E
#define KISHSAVER 0xA3

// ERROR O FALTA DE DATOS
#define ERROR 0xFF // Devolveremos un error en caso de que falle la lecura
#define T_SCAN  2500 // T start en caso máximo del teclado en modo escaneo
#define T_START  350 // En este caso T start para lectura está entre 300 y 350us
#define T_S_BIT  100 // Start bit, un ciclo de 60+30 =90, más 10us de regalo
#define T_CLOCK   95 // Pulso de reloj de 60+30 =90, más 5us de regalo
#define T_C_H     65 // El reloj está en alto 60 us, más 5us de regalo
#define T_C_L     35 // El reloj está en bajo 30 us
// Estos tiempos son mayores de los especificados para el teclado 4703, de las diferentes
// fases del rejoj, 30, 60, 90, 300 o 2500 us. No importa que el intervalo sea mayor,
// lo que importa es saber que nos hemos pasado de tiempo, y generar el error.
