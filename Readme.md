# CONVERSOR IBM 4704 A USB:

## de NacioSystems:

Versión 1.0
Fecha 28/2/2020

Con este proyecto se explica la construcción de un interfaz para el teclado español 4704 de IBM de 62 teclas, para su utilización por USB en cualquier otro ordenador. 

---

### Introducción:


Paseado por el Rastro, en 1985, encontré un puesto que tenia varios teclados y terminales en venta. Tenía la pinta de que en una sucursal de banco habían cambiado de sistema y se habían deshecho de pantallas, terminales, impresoras y teclados. Había varios teclados, creo recordar que unos completos con caja, teclados de funciones y varios expandidos. 

Me llamó la atención, era la época de los *Conmmodore 64* y los *Spectrum*, esos teclados no estaban al alcance de cualquiera. En el Rastro nada es de fiar y no quería gastar mucho. Pregunté el precio y había uno económico por unas 100 pesetas. Se trataba de un teclado de terminal IBM 4704, que tenía la apariencia de estar estropeado, pues este en concreto era más barato y no disponía de caja externa.

Me gustó mucho su tacto, su sonido y decidí comprarlo. El teclado lo tuve de adorno en mi época de estudiante, lo guardé y muchos años después supe que estaba considerado uno de los mejores teclados, el *IMB Modelo F*, que por ser capacitivo superaba en prestaciones al más conocido Model M, tambien de IBM.

En 2020 decidí ponerlo en marcha y encontré mucha información en Internet. En concreto el proyecto de *[Soarer][1]*, quien destripó el hardware y el software del microprocesador Intel 8048, lo que a su vez permitió el desarrollo de otros proyectos de reconversión de este teclado en modelo USB, como es el caso del proyecto *[TMK][2]*.

![Kishsaver](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/Teclado%20Completo.jpg "Pruebas conexión")

Este teclado es una matriz de 62 teclas, 8x8, en la que uno de los puntos de la matriz no es una tecla, el 63, es un código fijo que identifica el teclado, en mi caso 0xA3h. Para la lectura de la matriz se utiliza un controlador Intel 8048, aunque el teclado que yo tengo está marcado con un código diferente (8315), básicamente es un 8048, que se encarga de alimentar sucesivamenta cada una de las 8 filas, seleccionar cada una de las 8 columnas, mientras lee el valor 1 / 0 del pin 1, denominado TO. Interiormente el 8048 dispone de un microprocesador Intel de 8 bits, memoria RAM de 64 bytes y una ROM de 1kB, así como 3 puertos configurables de entrada salida tambien de 8 bits. La seleccion de las columnas es a través de un chip específico IBM (sensor capacitivo customizado IBM 9314) con entrada decimal (tres bits 0-7 que seleccionan la salida) y una salida por cada columna (8 salidas). Las entradas y salidas de Data, Clock y Buzzer se realizan a traves de puertas lógicas adicionales, que evitan cargar las salidas del 8048. Estas salidas están puestas en Pull-Up.

En concreto, para el modelo de 62 teclas, que los expertos llaman *Kishsaver*, realmente solo utiliza un puerto de 8 bits para el Strobe, la selección de filas, y mitad de otro puerto de 8 bits (bits 0, 1 y 2) para la selección de columnas. Un cuarto bit lo utiliza para la carga de los condensadores a través del IBM 9314 y un bit adicional para la lectura del Strobe de la tecla (que no existe) del código de teclado (ver imagen 8048).

![Intel8048](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/8048_replace_kishsaver2.png "Intel 8048")

La construcción es impresionante. Lleva dos planchas metálicas en las que encajan, en modo sandwich la placa de circuito impreso PCB y las teclas. La placa PCB está totalmente cubierta por una capa aislante por las dos caras, el soporte de la PCB hace de dieléctrico. Los contactos no son realmente contactos, son superficies que hacen de condensadores. El conjunto es sólido, el que yo tengo pesa 1.6Kg, pero he visto que el original con la caja de fundición pesa 2.8Kg. !Impresionante!

El teclado que yo tengo es un modelo de teclado español, con letra Ñ y otras teclas cambiadas de sitio respecto del teclado estándar, por lo menos la versión americana que documentó Soarer. A este teclado le encontré dos *Buckling springs*, los martillos de IBM rotos. Con estas características no resutla fácil la aplicación directa de un software estandar tipo *[Soarer][1]*, para aprovechar todas las capacidades de este teclado. 

![BucklingSpring](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/155px-Bucklingspring-animation-300ms.gif "Sistema IBM Buckling Spring")

Es posible conseguir hardware adicional para reemplazar la placa controladora original por una más moderna, con puerto USB, pero los precios no son precisamente económicos. Hablamos de unos 41$ más gastos de envío (Ver proyecto TMK). 

Este modelo resultó tan bueno, que a través de micromecenazgo se creó un proyecto que diseñó, fabricó y vende este tipo de teclados, las piezas son iguales a las originales, y se respetó la calidad de los materiales. Esta calidad y la falta de economías de escala no permite que sea barato, los precios, para cualquiera de estos teclados andan por los 350$ más gastos de envío. Venden también piezas sueltas, cajas, martillos *buckling spring*, cápsulas de teclas, controladoras. Ver tienda en *[Model F Keyboards][9]*. Recomiendo ver el análisis en Youtube de *[Chyrosran22][10]*. En cualquier caso, los precios en el mercado de segunda mano para los Modelos F originales, superan estos precios.


Tomé la decisión de poner mi teclado a funcionar y me propuse buscar alternativas utilizando la información que *[Soarer][1]* ha hecho pública. La primera alternativa fue conectar un adaptador entre el bus del teclado y PC, con salida USB, basada en *Arduino Nano*. Si esto no funcionaba, en segundo lugar tenía previston reemplazar el controlador Intel 8048, instalando un zócalo de 40 pines, al igual que hizo *[Soarer][1]*, y reprogramando todo desde cero. Esta segunda alternativa era la que ofrecía más probabilidades desde mi punto de vista, ya que tenía el presentimiento de que el Intel 8048 no funcionaba.

Más tarde encontré, en el Github de *[iflowfor8hours][3]*, como hacer un conversor USB utilizando una placa Pro Micro, una placa similar a la placa Tennsy, la que utiliza Soarer, aunque con una configuración más limitada. Ambas placas vienen a ser un Arduino Leonardo reformado, en miniatura, con un microcontrolador _*ATmega32U4*_. Este controlador lleva incorporado un puerto USB nativo, lo que facilita enormemente la simulación de dispositivos *HID* en el ordenador.

![ProMicro](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/ProMicro.jpg "Pro Micro")

El resultado no es inmediato, siguiendo las instrucciones de Soarer conseguí, despues de pelearme un poco con la información, salir del modo comando en el que se inicia el teclado y leer las teclas. Esto me permitió confirmar que el procesador IBM 8048 y el resto de la placa funcionan perfectamente despues de más de 35 años sin conectar.

Previamente a todo esto, desmonté el teclado, pues la suciedad acumulada en 35 años era considerable. Durante varios días estuve limpiando la placa, las cubiertas de acero, los soportes de las teclas y las teclas. Lo que más trabajo me dió fueron los soportes de las teclas ya que la cubierta de espuma que cubría las teclas, entre estas y la placa de acero superior, estaba desintegrada y pegada como con cola a los soportes de las teclas.

Una de las complicacines más difíciles fue el envío de comandos desde PC al teclado. Algunos comandos si los recibía sin problema, por ejemplo el comando RESET o SOFT_RESET, pero el resto de comandos como poner los *Break Codes* o los *Repit Codes* fué más complicado, parecía que los aceptaba y sin embarno no los ejecutaba. El problema estaba en que el teclado funciona siempre como maestro, manejando el reloj CLK, tanto en la lectura de códigos de escaneo como cuando se le envían datos de comandos, y yo intentaba enviarle los comandos controlando el reloj desde el PC. Una vez descubierto esto ya me puse a configurar un programa para que el IBM 4704 tenga un uso más funcional posible como teclado en español. Y funciona tan bien que es el teclado que utilizo:

Por cierto el teclado original tiene un beeper, un zumbador que suena en cada tecla pulsada. Creerme, el sonido de los martillos al pulsar la teclas hace innecesario dicho zumbador.

### Materiales:
* Teclado IBM 4704 62 teclas, Kishsaver, modelo F
* Sparkfun Pro Micro


### Programa:

El programa está realizado con el IDE Arduino, para su programación directa a través de cable USB. En la carpeta software se puede ver el programa comentado. El pineado está explicado en los comentarios, sólo es necesario montar el cableado para la comunicación con el IMB 4704 y el puerto USB. La conexión con el PC abre un puerto HID Keyboard standar, para el envío de la información tecleada, pero por otra parte abre simultáneamente un puerto COM, en el que permite visualizar los códigos de escaneo del telcado, los códigos de conversión de teclas y los comandos enviados.

![IBM4704](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/IBM4704.jpg "IBM 4704 Kishsaver")

### Construcción:

La construcción es en base es la plataforma **_Sparkfun Pro Micro_**, a la que se le han soldado conectores *duppond* hembra en los pines Vcc, GND, A3 y A4, los primeros para la alimentación de la placa con el procesador Intel 8048, los segundos para las fuciones de comunicación de IBM, CLK y DATA. 

![Martillos](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/interior%204.jpg "Interior, martillos")

![Inferior](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/Placa%20Base%20DOWN.jpg "Vista placa inferior")

![Superior](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/Placa%20Base%20UP.jpg "Vista placa superior")

Por otra parte se diseñó una caja funcional, similar a la original de IBM, impresa en PLA, con una impresora 3D.

![Caja3D](https://github.com/NacioSystems/IBM-4704-KEYBOARD/blob/master/Imagenes/Caja3D.JPG "Modelo caja 3D")


### Piezas impresas:

En la carpeta de piezas impresas, están los modelos Stl de la caja para el Kishsaver, imitando la caja original, modelo sacado a partir de la fotos ofrecidas por *[Kishy.ca][8]*.

Está tamibién el archivo FreeCad por si alguien quiere modificar dicha caja.

La impresora 3D que yo tengo tiene una superficie muy limitada, 200x200mm, lo que no me permite construir de una vez las dos partes, inferior y superior. Para poder imprimirlo, corté las piezas superior e inferior en dos partes dada una, derecha e izquierda, con unas dimensiones aceptables para el tamaño de la impresora. El material que utilicé es PLA de color dorado. 

Las mitades izquierda y derecha las pequé con cola y las uní con las levas inferior y superior para darle un refuerzo. La unión es con tornillos de métrica M3. Hay unas tuercas 4xM3 también embibidas en la parte superior, para sujetar con fuerza los tornillos. En la parte inferior tuve que meter un refuerzo, yo lo pegué con cola pero dejé espacio para tornillos, para hacerlo más rígido.


### Referencias:

* *[IBM 4700 Kishy.ca][5]*

* *[IBM 4704 to USB keyboard converter][6]*

* *[XT / AT / PS2 / Terminal to USB Converter with NKRO][7]*

* *[Sparkfun Pro Micro][4]*




### Autor:
**Ignacio Otero**

### Agradecimientos:

Muy agradecido tanto a *[Soarer][1]*, como *[iflowfor8hours][3]*, así como a *[http://kishy.ca][8]*, quienes me permitieron acceder a información esencial para conocer el funcionamiento de este teclado.

### Licencia:
Todos estos productos están liberados mediante Creative Commons Attribution-ShareAlike 4.0 International License.

[1]:https://deskthority.net/viewtopic.php?f=7&t=2510&start=

[2]:https://github.com/tmk/tmk_keyboard/tree/master/converter/ibm4704_usb

[3]:https://github.com/iflowfor8hours/6112884-pro-micro-controller

[4]:https://www.sparkfun.com/products/12640

[5]:http://kishy.ca/?p=648

[6]:https://github.com/tmk/tmk_keyboard/tree/master/converter/ibm4704_usb

[7]:https://geekhack.org/index.php?topic=17458.0

[8]:http://kishy.ca/?p=894

[9]:https://www.modelfkeyboards.com/

[10]:https://www.youtube.com/watch?v=nlbbBvS8wVY
