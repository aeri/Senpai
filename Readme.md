

[comment]: <> (![alt text](https://i.imgur.com/foxYHs9.png "senpai logo")
<p align="center">
  <img  src="https://i.imgur.com/foxYHs9.png">
</p>

# El proyecto

**Senpai** es el codename elegido para el proyecto a relizar en la asignatura de Proyecto Hardware.

Se trata de hacer funcionar un proyecto en una placa de hardware real y analizar el código proporcionado en C y ensamblador.

## El juego
El **Reversi** (también conocido como Othello) es uno de los juegos de tablero más populares en el mundo. La clave de su éxito es que las reglas son muy sencillas, aunque jugar bien es muy difícil.

Cierta empresa quiere lanzar un sistema que juegue al Reversi contra una persona y que se ejecutará en un procesador ARM7. Como primer paso han escrito una versión beta del programa en C (como versión beta está sujeta a fallos, avisad si veis alguno). Pero no están contentos con el tiempo de cálculo del código y os piden que lo reduzcáis. Para ello os proponen acelerar la función más crítica del código: patron_volteo().

## Objetivos

* Interactuar con una placa real y ser capaces de ejecutar en ella y sobre el simulador.
* Profundizar en la interacción C / Ensamblador.
* Ser capaces de depurar el código ensamblador que genera un compilador a partir de un lenguaje en alto nivel.
* Gestionar la entrada/salida con dispositivos básicos, asignando valores a los registros internos de la placa desde un programa en C (utilizando las librerías de la placa).
* Aprender a desarrollar en C las rutinas de tratamiento de interrupción.
* Aprender a utilizar periféricos reales, como los temporizadores internos de la placa.
* Conocer la estructura segmentada en tres etapas del procesador ARM 7, uno de los más utilizados actualmente en sistemas empotrados.
* Familiarizarse con el entorno Eclipse sobre Windows7, con la generación cruzada de código para ARM y con su depuración.
* Desarrollar código en ensamblador ARM: adecuado para optimizar el rendimiento.
* Optimizar código tanto en ensamblador ARM como utilizando el las opciones del compilador optimizador.
* Entender la finalidad y el funcionamiento de las Aplication Binary Interface, ABI, en este caso el estándar AATPCS (ARM Application Procedure Call Standard), y combinar de manera eficiente código en ensamblador con código en C.
* Saber depurar código siguiendo el estado arquitectónico de la máquina: contenido de los registros y de la memoria.
* Gestionar el tiempo de trabajo del proyecto correctamente en función de la disponibilidad de acceso al sistema específico.

## Entorno

Esta práctica se realiza con Eclipse, junto con las herramientas de gcc para compilación cruzada y el complemento (plug-in) para simular procesadores **ARM7TDMI**.

Ejecutaremos nuestro código también directamente en la placa **Embest S3CEV40**. Para ello, se incluye un soporte especial que facilita conectarse a la placa de desarrollo

## Autores

* **Pablo Orduna**
* **Daniel Naval** - [Web](http://www.naval.cat)

## Licencia

This project is not licensed.