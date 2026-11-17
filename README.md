# Circuitos-Digitales
descripción: 
Basura Inteligente es un prototipo de bote de basura que abre su tapa automaticamente cuando un sensor ultrasonico HC-SR04 detecta una mano u objeto a menos de 20 cm. Un servomotor mueve la tapa y la cierra solo unos segundos despues de que la persona se aleja.

El proyecto incluye ademas dos botones que ilustran el uso de resistencias pull-up y pull-down:
- Boton (D2) pull-up = abre la tapa sin el sensor. 
- Boton (D3) pull-down = desactiva el mecanismo. 

# Problemática: 
Abrir un bote de basura con la mano o con un pedal obliga a tocar superficies sucias o a usar las dos manos. Esto genera varios problemas:

1.Higiene: la tapa y el pedal acumulan gérmenes y se tocan con las manos.
2.Accesibilidad: las personas con movilidad reducida o con las manos ocupadas (cocinando, cargando bolsas) tienen dificultad para abrirlo.
3.Olores y plagas: los botes que quedan abiertos o mal cerrados atraen insectos y dejan escapar olores.

# Componentes: 
Arduino UNO R3
Sensor de ondas HC-SR04
ServoMotor SG90
Pulsadores de 4 patas
Resistencia de 10kΩ
Protoboard
Cables Jumpers (macho- macho y macho- hembra) 

# Funcionamiento

El sistema funciona de forma automática y sin contacto: el Arduino mide continuamente la distancia y mueve la tapa según lo que detecte.

Sensor de ondas HC-SR04: mide la distancia frente al bote. Cuando detecta una mano u objeto a menos de 20 cm, envía la señal para abrir la tapa.
Servomotor: se encarga de abrir y cerrar la tapa. Se mueve a 90° para abrir y a 0° para cerrar.
Pulsador manual: abre la tapa sin usar el sensor, como respaldo si este falla.
Pulsador de bloqueo: mantiene la tapa cerrada e ignora tanto el sensor como el pulsador manual. Cada pulsación alterna entre bloqueado y desbloqueado.

La tapa permanece abierta mientras el sensor siga detectando presencia y se cierra 3 segundos después de que la persona se aleja.

# Conexiones
El Arduino UNO recibe las señales de los pulsadores y del sensor mediante sus entradas digitales. El sensor usa D8 para Trig y D9 para Echo, el pulsador manual está en D2 y el de bloqueo en D3. El servomotor recibe su señal de control por D10.
El pulsador manual utiliza una resistencia pull-up interna del Arduino, activada por software, y se conecta entre D2 y GND. El pulsador de bloqueo utiliza una resistencia de 10 kΩ en configuración pull-down entre D3 y GND para mantener su estado en LOW mientras no está presionado, y se conecta entre D3 y 5V.
Si se usa un servomotor grande, este debe alimentarse con una fuente externa de 5-6 V. La conexión entre la fuente externa y el Arduino debe compartir GND para garantizar una referencia común.

# Programación

El código fue desarrollado en Arduino IDE utilizando la librería Servo, incluida por defecto.Algunas de las funciones y elementos utilizados son:

Servo.h: permite controlar el servomotor, con attach() para asignar el pin y write() para indicar el ángulo.
pulseIn(): mide la duración del eco del sensor para calcular la distancia.
delayMicroseconds(): genera el pulso de 10 µs que activa la medición del sensor.
millis(): permite controlar el tiempo que la tapa permanece abierta y evitar rebotes en el pulsador de bloqueo.
INPUT_PULLUP: activa la resistencia pull-up interna del pulsador manual.
digitalRead(): lee el estado de los pulsadores.
bloqueada = !bloqueada: permite alternar entre el estado
Serial.println(): muestra la distancia medida y los mensajes BLOQUEADA y DESBLOQUEADA en el Monitor Serie.

