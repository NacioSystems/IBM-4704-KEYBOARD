# CONVERSOR IBM 4704 A USB:

## de NacioSystems:

Versión 1.0
Fecha 28/2/2020

Con este proyecto se explica la construcción de un interfaz para el teclado español 4704 de IBM de 62 teclas, para su utilización por USB en cualquier otro ordenador. 

---

### Introducción:


Paseado por el rastro, en 1985, encontré un puesto que tenia varios teclados y terminales en venta, tenía la pinta de una sucursal de banco que había cambiado de sistema.

Se trataba de un teclado de terminal IBM 4704, que tenía la apariencia de estar estropeado, pues este en concreto era más barato y no disponía de caja externa.

Me gustó mucho su tacto y su sonido y decidí comprarlo. Después supe que estaba considerado uno de los mejores teclados, el IMB Modelo F, que por ser capacitivo superaba en prestaciones al más conocido Model M, tambien de IBM.

En 2020 decidí ponerlo en marcha y encontré mucha información en Internet. En concreto el proyecto de *[Soarer][1]*, quien destripó el hardware y el software del microprocesador Intel 8048, lo que a su vez permitió el desarrollo de otros proyectos de reconversión de este teclado en modelo USB, como es el caso del proyecto *[TMK][2]*.

![Kishsaver](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/Teclado%20Completo.jpg "Pruebas conexión")

El teclado es una matriz de 62 teclas, 8x8, en la que uno de los puntos de la matriz, el 63, es un código fijo que identifica el teclado, en mi caso 0xA3h. Para la lectura de la matriz se utiliza un controlador Intel 8048, aunque el teclado que yo tengo está marcado con un código diferente, básicamente es un 8048, que se encarga de alimentar sucesivamenta cada una de las 8 filas, seleccionar cada una de las 8 columnas, mientras lee el valor 1 / 0 del pin 1, denominado TO. El 8048 dispone de un microprocesador Intel de 8 bits, memoria RAM de 64 bytes y una ROM de 1kB, así como 3 puertos configurables de entrada salida tambien de 8 bits. La seleccion de las columnas es a través de un chip específico con entrada decimal (tres bits 0-7) y una salida por cada columna (8 bits). Las entradas y salidas de Data, Clock y Buzzer se realizan a traves de puertas adicionales en el controlador, puestas en Pull-Up.

![Intel8048](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/8048_replace_kishsaver2.png "Intel 8048")

[1]:https://deskthority.net/viewtopic.php?f=7&t=2510&start=

[2]:https://github.com/tmk/tmk_keyboard/tree/master/converter/ibm4704_usb

El teclado que yo tengo es un modelo de teclado español, con letra Ñ y otras teclas cambiadas de sitio respecto del teclado estándar, al que además le encontré dos *Buckling springs*, los martillos de IBM rotos. Con estas características no resutla fácil la aplicación directa de un software estandar tipo *[Soarer][1]*, para aprovechar todas las capacidades de este teclado. 

![BucklingSpring](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/155px-Bucklingspring-animation-300ms.gif "Sistema IBM Buckling Spring")

Por otra parte es posible conseguir hardware adicional para reemplazar la placa controladora original por una más moderna, con puerto USB, pero los precios no son precisamente económicos. Hablamos de unos 41$ más gastos de envío. 

Por ese motivo me propuse buscar alternativas utilizando la información que *[Soarer][1]* ha hecho pública. La primera alternativa fue conectar un adaptador entre el bus del teclado y PC, con salida USB, basada en *Arduino Nano*. En segundo lugar en reemplazar el controlador Intel 8048, instalando un zócalo de 40 pines, al igual que hizo *[Soarer][1]*, y reprogramando todo desde cero. Esta segunda alternativa era la que ofrecía más probabilidades desde mi punto de vista, ya que tenía el presentimiento de que el Intel 8048 no funcionaba.

Más tarde encontré, en el Github de *[iflowfor8hours][3]*, como hacer un conversor USB utilizando una placa Pro Micor, una placa similar a la placa Tennsy, la que utiliza Soarer, aunque con una configuración más limitada. Ambas placas vienen a ser un Arduino Leonardo reformado, en miniatura, con un microcontrolador _*ATmega32U4*_. Este controlador lleva incorporado un puerto USB nativo, lo que facilita enormemente la simulación de dispositivos *HID* en el ordenador.

[3]:https://github.com/iflowfor8hours/6112884-pro-micro-controller

![ProMicro](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/ProMicro.jpg "Pro Micro")

El resultado no es inmediato, siguiendo las instrucciones de Soarer conseguí, despues de pelearme un poco con la información, salir del modo comando en el que se inicia el teclado y leer las teclas. Esto me permitió confirmar que el procesador IBM 8048 y el resto de la placa funcionan perfectamente despues de más de 35 años sin conectar.

Previamente a todo esto, desmonté el teclado, pues la suciedad acumulada en 35 años era considerable. Durante varios días estuve limpiando la placa, las cubiertas de acero, los soportes de las teclas y las teclas. Lo que más trabajo me dió fueron los soportes de las teclas ya que la cubierta de espuma que cubría las teclas, entre estas y la placa de acero superior, estaba desintegrada y pegada como con cola a los soportes de las teclas.

Lo que no fue fácil fue el envío de comandos del PC al teclado. Algunos comandos si los recibía sin problema, por ejemplo el comando RESET o SOFT_RESET, pero el resto de comandos como poner los Break Codes o los Repit Codes, parecía que los aceptaba y sin embarno no los ejecutaba. El problema estaba en que el teclado funciona siempre como maestro, manejando el reloj CLK, tanto en la lectura de códigos de escaneo como cuando se le envían datos de comandos, y yo intentaba enviarle los comandos controlando el reloj desde el PC. Una vez descubierto esto ya me puse a configurar un programa para que el IBM 4704 tenga un uso más funcional posible como teclado en español.

### Materiales:
* Teclado IBM 4704 62 teclas, Kishsaver, modelo F
* Sparkfun Pro Micro


### Programa:

El programa está realizado con el IDE Arduino, para su programación directa a través de cable USB. En la carpeta software se puede ver el programa comentado. El pineado está explicado en los comentarios, sólo es necesario montar el cableado para la comunicación con el IMB 4704 y el puerto USB. La conexión con el PC abre un puerto HID Keyboard standar, para el envío de la información tecleada, pero por otra parte abre simultáneamente un puerto COM, en el que permite visualizar los códigos de escaneo del telcado, los códigos de conversión de teclas y los comandos enviados.

![IBM4704](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/IBM4704.jpg "IBM 4704 Kishsaver")

### Construcción:

La construcción es en base es la plataforma **_Sparkfun Pro Micro_**, a la que se le han soldado conectores *duppond* hembra en los pines Vcc, GND, A3 y A4, los primeros para la alimentación de la placa con el procesador Intel 8048, los segundos para las fuciones de comunicación de IBM, CLK y DATA. 

Por otra parte se diseñó una caja funcional, similar a la original de IBM, impresa en PLA, con una impresora 3D.


### Piezas impresas:

En la carpeta de piezas impresas, están los modelos Stl de la caja para el Kishsaver, imitando la caja original, modelo sacado a partir de la fotos ofrecidas por *[Kishy.ca][8]*.


### Referencias:

* *[IBM 4700 Kishy.ca][5]*

* *[IBM 4704 to USB keyboard converter][6]*

* *[XT / AT / PS2 / Terminal to USB Converter with NKRO] [7]

* *[Sparkfun Pro Micro][4]*




### Autor:
**Ignacio Otero**

### Agradecimientos:

Muy agradecido tanto a *[Soarer][1]*, como *[iflowfor8hours][3]*, así como a *[http://kishy.ca][8], quienes me permitieron acceder a información esencial para conocer el funcionamiento de este teclado.

### Licencia:
Todos estos productos están liberados mediante Creative Commons Attribution-ShareAlike 4.0 International License.


[4]:https://www.sparkfun.com/products/12640

[5]:http://kishy.ca/?p=648

[6]:https://github.com/tmk/tmk_keyboard/tree/master/converter/ibm4704_usb

[7]:https://geekhack.org/index.php?topic=17458.0

[8]:http://kishy.ca/?p=894
