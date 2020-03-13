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


[1]:https://deskthority.net/viewtopic.php?f=7&t=2510&start=

[2]:https://github.com/tmk/tmk_keyboard/tree/master/converter/ibm4704_usb

El teclado que yo tengo es un modelo de teclado español, con letra Ñ y otras teclas cambiadas de sitio respecto del teclado estándar, al que además le encontré dos *Buckling springs*, los martillos de IBM rotos. Con estas características no resutla fácil la aplicación directa de un software estandar tipo *[Soarer][1]*, para aprovechar todas las capacidades de este teclado. 

![BucklingSpring](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/155px-Bucklingspring-animation-300ms.gif "Sistema IBM Buckling Spring")

Por otra parte es posible conseguir hardware adicional para reemplazar la placa controladora original por una más moderna, con puerto USB, pero los precios no son precisamente económicos. Hablamos de unos 41$ más gastos de envío. 

Por ese motivo me propuse buscar alternativas utilizando la información que *[Soarer][1]* ha hecho pública. La primera alternativa fue conectar un adaptador entre el bus del teclado y PC, con salida USB, basada en *Arduino Nano*. En segundo lugar en reemplazar el controlador Intel 8048, instalando un zócalo de 40 pines, al igual que hizo *[Soarer][1]*, y reprogramando todo desde cero. Esta segunda alternativa era la que ofrecía más probabilidades desde mi punto de vista, ya que tenía el presentimiento de que el Intel 8048 no funcionaba.

Más tarde encontré, en el Github de *[iflowfor8hours][3]*, como hacer un conversor USB utilizando una placa Pro Micor, una placa similar a la placa Tennsy, la que utiliza Soarer, aunque con una configuración más limitada. Ambas placas vienen a ser un Arduino Leonardo reformado, en miniatura, con un microcontrolador _*ATmega32U4*_. Este controlador lleva incorporado un puerto USB nativo, lo que facilita enormemente la simulación de dispositivos *HID* en el ordenador.

[3]:https://github.com/iflowfor8hours/6112884-pro-micro-controller

![ProMicro](

El resultado no es inmediato, siguiendo las instrucciones de Soarer conseguí, despues de pelearme un poco con la información, salir del modo comando en el que se inicia el teclado y leer las teclas. Esto me permitió confirmar que el procesador IBM 8048 y el resto de la placa funcionan perfectamente despues de más de 35 años sin conectar.

Previamente a todo esto, desmonté el teclado, pues la suciedad acumulada en 35 años era considerable. Durante varios días estuve limpiando la placa, las cubiertas de acero, los soportes de las teclas y las teclas. Lo que más trabajo me dió fueron los soportes de las teclas ya que la cubierta de espuma que cubría las teclas, entre estas y la placa de acero superior, estaba desintegrada y pegada como con cola a los soportes de las teclas.

Lo que no fue fácil fue el envío de comandos del PC al teclado. Algunos comandos si los recibía sin problema, por ejemplo el comando RESET o SOFT_RESET, pero el resto de comandos como poner los Break Codes o los Repit Codes, parecía que los aceptaba y sin embarno no los ejecutaba. El problema estaba en que el teclado funciona siempre como maestro, manejando el reloj CLK, tanto en la lectura de códigos de escaneo como cuando se le envían datos de comandos, y yo intentaba enviarle los comandos controlando el reloj desde el PC. Una vez descubierto esto ya me puse a configurar un programa para que el IBM 4704 tenga un uso más funcional posible como teclado en español.


### Autor:
**Ignacio Otero**

### Agradecimientos:

Muy agradecido tanto a _*Soarer*_, como _*iflowfor8hours*_, así como a _*http://kishy.ca*_, quienes me permitieron acceder a información esencial para conocer el funcionamiento de este teclado.

### Licencia:
Todos estos productos están liberados mediante Creative Commons Attribution-ShareAlike 4.0 International License.




