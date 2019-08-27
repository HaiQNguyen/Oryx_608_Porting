# SAME54 Oryx ATECC608 Porting

## Motivation

This project intents to integrate the ATECC608 functionalities into the Oryx network stack.

ATECC608 secure elements is resposible for storing device information for authentication process and offload the MCU with cryptographic cypybilities.

## Requirement

* Atmel Studio
* Terminal Program
* SAME54-Xplained kit
* ATECC608 secure element
* Internet connection

## Setup

The project is generated with Atmel Start allowing us to add and modify MCU's clock rate and  peripherals.

The Oryx stack and Crypto Authen Library from michochip is integrated afterwards into the project.

An ATECC608 pre-provision with TrustnGo is attached to the system.

### Serial console

The project will output the debug log on the serial port with the following configuration:

* Baudrate: 115200
* Data: 8 bit
* Stopbit: 1
* Parity: none

### Project file setup

* If your secure element is not provisioned with TrustnGo Scheme, please go to main.c, line 777 and comment out function *certStoreInit()*.

* In the main.c, please go to line 97, line 125, line 154 to modify the authentication information (trustedCaList, aws_pri_key, aws_cert respectively).

* Compile and Run the project.

## Change log

### Version 0.0.1

Draft

## Contact

Quang Hai Nguyen - Field Application Engineer

Arrow Central Europe GmnbH

qnguyen@arroweurope.com
