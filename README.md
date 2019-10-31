# Sensorium_ModularVest
Sensorium is a project consisting in a modular illuminated vest that can display abstract information.

This Repository contains all the files needed to reproduce the device. 
This Repository is merely destined to the technological part of the project.
This Repository makes use of the programs: EagleCad, Arduino IDE, Tensorflow.

If any other information is needed, please contact with the repository owner or any of the participants of the project.

Description

Sensorium is the first prototype for the material tech part of the final project of Gabriel G. Regués. It consist on a modular illuminated vest conformed by an unespecific number of Arduino based modules connected to display specific information through an I2C net.

[AQUI VIDEO Y FOTOS]

As escribed in the final essay:

"Sensorium es un proyecto colectivo desarrollado en colaboración por Jaime Remirez de Ganuza, Gonzalo de Torres Sainz, Pablo de Miguel Morales y Gabriel G. Regües.
Este proyecto es una continuación de un proyecto anterior realizado a finales del 2018 por Jaime y Gabriel, al que llamaron El Culto de la Subversión1 y en el que se formularon algunas preguntas que siguen formando parte de este trabajo. El Culto de la Subversión se enfrentaba a la idea de un grupo social en el que se había impuesto una prenda única y se especulaba sobre las posibilidades de transformación
de la misma, a medida que los integrantes entraban o dejaban de formar parte del grupo. Los diseñadores investigaban de qué manera cada individuo podía servir creativamente a la creación de los símbolos de representación de un colectivo organizado de forma igualitaria y horizontal. El proyecto se articuló en torno a la creación de una prenda modular, formada por un número determinado de piezas que podían
combinarse de distintas maneras. Estas piezas estaban formadas en tela, en la que habían sido impresos unos elementos gráficos. La posibilidades de reconfiguración de la prenda daban lugar a distintas variaciones, tanto en el aspecto formal como gráfico, que representaban la integración o salida de individuos en el grupo."

System Overview

The system is based on a I2C net of Arduino Nano based modules that control 10 WS212B RGB LEDs each. The aim is to display abstract information incoming from a Tensorflow software system running on a PC based on global population database.

[SYSTEM DIAGRAM]

Arduino Modules
The Arduino Modules are named "IED_GR_Module_Rev_02". It consist on a triangular PCB equipped with an Arduino Nano, a I2C identification resistor array and 10 WS2812B RGB LEDs.

Aditionally, it has 3 redundant pairs of miniUSB connectors (both male and female) destined to interconnect an undefined number of modules. The signals present in this connectors are:

- 5V (VCC)
- GND
- SDA (I2C)
- SCL (I2C)

The Schematic of the modules is displayed in the file

