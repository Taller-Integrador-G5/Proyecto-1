# Manual de Uso - LoRa APRS iGate

## Introducción

El LoRa APRS iGate es un dispositivo diseñado para la transmisión y recepción de datos de telemetría, posición de unidades y mensajes en el sistema de radioaficionados APRS. Este manual proporciona instrucciones para la configuración y operación del dispositivo.

## Contenidos
1. [Descripción del Dispositivo](#descripción-del-dispositivo)
2. [Componentes Necesarios](#componentes-necesarios)
3. [Configuración Inicial](#configuración-inicial)
4. [Operación del Dispositivo](#operación-del-dispositivo)

## Descripción del Dispositivo

El iGate es un dispositivo que permite la transmisión de paquetes de datos recibidos por radiofrecuencia al sistema APRS-IS mediante una conexión Wi-Fi. Opera en una frecuencia de 433.755 MHz utilizando modulación LoRa y protocolo APRS.

## Componentes Necesarios

| Componente           | Descripción         | Cantidad | Precio ($) |
|----------------------|---------------------|----------|------------|
| LILYGO TTGO LoRa32   | Módulo iGate        | 1        | 20         |
| PL802030             | Batería Litio       | 1        | 6.5        |
| PRT-08670            | Cable               | 1        | 1.4        |

## Configuración Inicial

### Paso 1: Conectar el Hardware
1. Conecte la batería PL802030 al módulo LILYGO TTGO LoRa32.
2. Asegúrese de que todos los cables estén correctamente conectados.

### Paso 2: Configurar el Software
1. Descargue el software desde el repositorio oficial [LoRa APRS iGate GitHub](https://github.com/richonguzman/LoRa_APRS_iGate).
2. Siga las instrucciones del repositorio para instalar las dependencias necesarias.

### Paso 3: Configuración del iGate
1. Configure la frecuencia de operación en 433.755 MHz.
2. Ingrese las coordenadas de latitud y longitud del lugar de operación.
3. Configure el indicativo y símbolo del dispositivo para la red APRS.

## Operación del Dispositivo

1. Encienda el dispositivo y asegúrese de que esté conectado a una red Wi-Fi.
2. Monitoree el estado del iGate en la plataforma APRS.
3. Verifique que los datos de telemetría y posición se transmitan correctamente.

