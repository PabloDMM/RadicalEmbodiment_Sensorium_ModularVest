# Sensorium_ModularVest
***Sensorium (Radical Embodiment)*** is a project consisting in a modular illuminated vest that can display abstract information.

***This Repository contains all the files needed to reproduce the device.***
***This Repository is merely destined to the technological part of the project.***
***This Repository makes use of the programs: EagleCad, Arduino IDE, Tensorflow.***

If any other information is needed, please contact with the repository owner (pablodmm.isp@gmail.com) or any of the participants of the project.

## Description

Sensorium is the first prototype for the material tech part of the final project of Gabriel G. Regués. It consist on a modular illuminated vest conformed by an unespecific number of *Arduino* based modules connected to display specific information through an I2C net.

***Watch the Video by pressing the header:***

[![Watch the video](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_VideoHeader.png)](https://vimeo.com/330038499)


![alt text](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_DesignPhase.png)


As escribed in the final essay:

*Sensorium - Radical Embodiment - es un proyecto colectivo desarrollado en colaboración por Jaime Remirez de Ganuza, Gonzalo de Torres Sainz, Pablo de Miguel Morales y Gabriel G. Regües.
Este proyecto es una continuación de un proyecto anterior realizado a finales del 2018 por Jaime y Gabriel, al que llamaron El Culto de la Subversión1 y en el que se formularon algunas preguntas que siguen formando parte de este trabajo. El Culto de la Subversión se enfrentaba a la idea de un grupo social en el que se había impuesto una prenda única y se especulaba sobre las posibilidades de transformación de la misma, a medida que los integrantes entraban o dejaban de formar parte del grupo. Los diseñadores investigaban de qué manera cada individuo podía servir creativamente a la creación de los símbolos de representación de un colectivo organizado de forma igualitaria y horizontal. El proyecto se articuló en torno a la creación de una prenda modular, formada por un número determinado de piezas que podían combinarse de distintas maneras. Estas piezas estaban formadas en tela, en la que habían sido impresos unos elementos gráficos. La posibilidades de reconfiguración de la prenda daban lugar a distintas variaciones, tanto en el aspecto formal como gráfico, que representaban la integración o salida de individuos en el grupo.*

## System Overview

The system is based on a I2C net of *Arduino Nano* based modules that control 10 WS212B RGB LEDs each. The aim is to display abstract information incoming from a *Tensorflow* software system running on a PC based on global population database.

![alt text](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_SystemDiagram_01.1.jpg)
![alt text](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_SystemDiagram_02.2.jpg)

### Arduino Modules

The Arduino Modules are named *IED_GR_Module_Rev_02*. It consist on a triangular PCB equipped with an *Arduino Nano*, a I2C identification resistor array (8bit based, 254 possible directions) and 10 WS2812B RGB LEDs.

![alt text](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_Module.png)

Aditionally, it has 3 redundant pairs of miniUSB connectors (both male and female) destined to interconnect an undefined number of modules. The signals present in this connectors are:

* 5V (VCC)
* GND
* SDA (I2C)
* SCL (I2C)

#### Bill of Materials (BOM)
The Module is conformed by the following elements:

* PCB				1	Printed-Circuit-Board	
* Arduino Nano		1	Microprocesador	
* WS2812b  SMD5050	10	RGB LEDs	WS2812B
* Mini USB B female	3	Mini USB B female	
* Mini USB B male	3	Mini USB B male	
* R_DIR_[0:7]		7	10kOhm Resistor. Footprint 1206. Sets the I2C device direction.	
* R_I2C_[0:1]		2	10kOhm Resistor. Footprint 1206. I2C Bus PullUps	

#### PCB Characterictics
The Characteristics of the PCB are the following:

* Dimensions (Square):  100 x 87 mm
* Thickness:            1.6 mm
* Layers:               2 (TOP y BOT)
* Color PCB:            White
* Color Serigraphy:     Black

The PCBs were made through *Elecrow* ( https://www.elecrow.com/ )

#### Prototype Problems
The prototypes have a number of critical mistakes, that can be fixed through the following procedure:

* Step 1: Cut the Signal marked in Blue thorugh the points indicated.
* Step 2: Bypass the signal marked in Green.

![alt text](https://github.com/PabloDMM/Sensorium_ModularVest/blob/master/01_Resources/SEN_ModulePCB_Fix.png)

Anycase, if anyone plans to rebuild the devices, we strongly encourage to contact us to apply a large number of fixes.

#### Software
The modules make use of two *Arduino* sowfwares, one for *I2C Slave* and another for *I2C Master*:

* _SEN_ModuleI2cMaster.ino_: Distributed a long array of data incoming for USB Serial *Tensorflow* Software running in a PC. Only One module of the net performs as *Master*.

* _SEN_ModuleI2cSlave.ino_: Manages 10 WS2812B REG LEDs based of I2C incoming Data.

Additionally, a large number of preliminary *Arduino* programs can be found in the repository, as part of the development.

Please notice the Arduino Nano used need the *Old ATMega328 Bootloader*, not the new one.
