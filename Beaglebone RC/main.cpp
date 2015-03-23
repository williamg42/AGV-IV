/*
 * main.cpp
 *
 *  Created on: Mar 23, 2015
 *      Author: william
 */

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>

#include "BlackUART.h"

char writeArr[22] = "BlackLib UART EXAMPLE";
    char readArr[22];



int main() {

	BlackLib::BlackUART UART1(BlackLib::UART1, BlackLib::Baud9600,
			BlackLib::ParityEven, BlackLib::StopOne, BlackLib::Char8);

	BlackLib::BlackUART UART2(BlackLib::UART2, BlackLib::Baud9600,
			BlackLib::ParityEven, BlackLib::StopOne, BlackLib::Char8);




	    bool isOpened = UART1.open( BlackLib::ReadWrite );


	    if( !isOpened )
	    {
	        std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
	        exit(1);
	    }

	    std::cout << std::endl;
	    std::cout << "Device Path     : " << UART1.getPortName() << std::endl;
	    std::cout << "Read Buf. Size  : " << UART1.getReadBufferSize() << std::endl;
	    std::cout << "BaudRate In/Out : " << UART1.getBaudRate( BlackLib::input) << "/"
	                                      << UART1.getBaudRate( BlackLib::output) << std::endl;
	    std::cout << "Character Size  : " << UART1.getCharacterSize() << std::endl;
	    std::cout << "Stop Bit Size   : " << UART1.getStopBits() << std::endl;
	    std::cout << "Parity          : " << UART1.getParity() << std::endl << std::endl;


	     isOpened = UART2.open( BlackLib::ReadWrite  );


	   	    if( !isOpened )
	   	    {
	   	        std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
	   	        exit(1);
	   	    }

	   	    std::cout << std::endl;
	   	    std::cout << "Device Path     : " << UART2.getPortName() << std::endl;
	   	    std::cout << "Read Buf. Size  : " << UART2.getReadBufferSize() << std::endl;
	   	    std::cout << "BaudRate In/Out : " << UART2.getBaudRate( BlackLib::input) << "/"
	   	                                      << UART2.getBaudRate( BlackLib::output) << std::endl;
	   	    std::cout << "Character Size  : " << UART2.getCharacterSize() << std::endl;
	   	    std::cout << "Stop Bit Size   : " << UART2.getStopBits() << std::endl;
	   	    std::cout << "Parity          : " << UART2.getParity() << std::endl << std::endl;


int i = 0;
memset(readArr,0,sizeof(readArr));
while(i < 5 || !UART1.write(writeArr, sizeof writeArr))
{
	i++;
}

usleep(40000);

i = 0;
while(i < 5 || !UART2.read(writeArr, sizeof writeArr))
{
	i++;
}

std::cout << std::setw(44) << "ONESHOT WRITE/READ DEMO WITH ARRAYS" << std::endl;
    std::cout << std::setw(15) << "SENT" << std::setw(26) << "RECEIVED" << std::endl;
    std::cout << std::string(50,'-');

    std::cout << "\n   " << writeArr;
    std::cout << "\t" << readArr << std::endl << std::endl << std::endl;



	return 0;
}



/*!
\file Example1.cpp
\brief Example code source for class serialib.
This example open the device on ttyACM0.
(USB to RS232 converter under linux).
If the opening is successful, it sends the AT command
and waits for a string being received from the device.
After 5 seconds, if no valid data are received from the
device, reception is giving up.
\author Philippe Lucidarme (University of Angers) <serialib@googlegroups.com>
\version 1.2
\date 05/01/2011

#include <stdio.h>
#include "serialib.h"
#ifdef __linux__
#define DEVICE_PORT "/dev/ttyO4" // ttyS0 for linux
#endif
#define PORT "/dev/ttyO5"
GPIO gpioenable(26);
int main()
{
serialib LS;
serialib LD; // Object of the serialib class
int Ret; // Used for return values
char Buffer[128];
gpioenable.gpio_export();
gpioenable.gpio_set_dir(1);
gpioenable.gpio_set_value(1);
// Open serial port
Ret=LS.Open(DEVICE_PORT,115200); // Open serial link at 115200 bauds
if (Ret!=1) { // If an error occured...
printf ("Error while opening port. Permission problem ?\n"); // ... display a message ...
return Ret; // ... quit the application
}
printf ("Serial port opened successfully !\n");
Ret=LD.Open(PORT,115200); // Open serial link at 115200 bauds
if (Ret!=1) { // If an error occured...
printf ("Error while opening port. Permission problem ?\n"); // ... display a message ...
return Ret; // ... quit the application
}
printf ("Serial port opened successfully !\n");
while(1)
{
// Write the AT command on the serial port
Ret=LD.ReadString(Buffer,'\n',128,5000); // Read a maximum of 128 characters with a timeout of 5 seconds
// The final character of the string must be a line feed ('\n')
if (Ret>0) // If a string has been read from, print the string
printf ("String read from serial port : %s",Buffer);
else
printf ("TimeOut reached. No data received !\n"); // If not, print a message.
Ret=LD.WriteString(Buffer); // Send the command on the serial port
if (Ret!=1) { // If the writting operation failed ...
printf ("Error while writing data\n"); // ... display a message ...
return Ret; // ... quit the application.
}
printf ("Write operation is successful \n");
Ret=LS.ReadString(Buffer,'\n',128,5000); // Read a maximum of 128 characters with a timeout of 5 seconds
// The final character of the string must be a line feed ('\n')
if (Ret>0) // If a string has been read from, print the string
printf ("String read from serial port : %s",Buffer);
else
printf ("TimeOut reached. No data received !\n"); // If not, print a message.
Ret=LS.WriteString(Buffer); // Send the command on the serial port
if (Ret!=1) { // If the writting operation failed ...
printf ("Error while writing data\n"); // ... display a message ...
return Ret; // ... quit the application.
}
printf ("Write operation is successful \n");
}
// Close the connection with the device
LS.Close();
LD.Close();
return 0;
}
*/
