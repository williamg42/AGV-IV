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
#include <bitset>

#include "BlackUART.h"

char readArr[21];

char inByte[1] = { 255 };

bool startbutton, selectbutton, L3, R3 = 0;

int LY, LX, RY, RX, Up, Down, Left, Right, X, triangle, square1, circle, L1, L2,
		R1, R2 = 0;

int main() {

	BlackLib::BlackUART UART1(BlackLib::UART1, BlackLib::Baud57600,
			BlackLib::ParityNo, BlackLib::StopOne, BlackLib::Char8);

	BlackLib::BlackUART UART2(BlackLib::UART2, BlackLib::Baud57600,
			BlackLib::ParityNo, BlackLib::StopOne, BlackLib::Char8);

	bool isOpened = UART1.open(BlackLib::ReadWrite | BlackLib::NonBlock);

	if (!isOpened) {
		std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
		exit(1);
	}

	std::cout << std::endl;
	std::cout << "Device Path     : " << UART1.getPortName() << std::endl;
	std::cout << "Read Buf. Size  : " << UART1.getReadBufferSize() << std::endl;
	std::cout << "BaudRate In/Out : " << UART1.getBaudRate(BlackLib::input)
			<< "/" << UART1.getBaudRate(BlackLib::output) << std::endl;
	std::cout << "Character Size  : " << UART1.getCharacterSize() << std::endl;
	std::cout << "Stop Bit Size   : " << UART1.getStopBits() << std::endl;
	std::cout << "Parity          : " << UART1.getParity() << std::endl
			<< std::endl;

	isOpened = UART2.open(BlackLib::ReadWrite);

	if (!isOpened) {
		std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
		exit(1);
	}

	std::cout << std::endl;
	std::cout << "Device Path     : " << UART2.getPortName() << std::endl;
	std::cout << "Read Buf. Size  : " << UART2.getReadBufferSize() << std::endl;
	std::cout << "BaudRate In/Out : " << UART2.getBaudRate(BlackLib::input)
			<< "/" << UART2.getBaudRate(BlackLib::output) << std::endl;
	std::cout << "Character Size  : " << UART2.getCharacterSize() << std::endl;
	std::cout << "Stop Bit Size   : " << UART2.getStopBits() << std::endl;
	std::cout << "Parity          : " << UART2.getParity() << std::endl
			<< std::endl;

//	memset(readArr, 0, sizeof(readArr));

//	while (!UART1.write(writeArr, sizeof writeArr)) {
//	}
	while (1) {
		//usleep(1000);
		readArr[0] = 255;

		if (UART2.Byteavailable() > 19) {
			//std::cout << UART2.Byteavailable() << std::endl;
			usleep(1000);

			UART2.read(readArr, sizeof readArr);

			if (readArr[0] == 0) {

				startbutton = readArr[1]; //start (boolean)
				selectbutton = readArr[2]; //select (boolean)

				LY = readArr[3];
				LX = readArr[4];
				RY = readArr[5];
				RX = readArr[6];
				Up = readArr[7];
				Down = readArr[8];
				Left = readArr[9];
				Right = readArr[10];
				triangle = readArr[11];
				circle = readArr[12];
				square1 = readArr[13];
				X = readArr[14];
				L1 = readArr[15];
				L2 = readArr[16];
				L3 = readArr[17];
				R1 = readArr[18];
				R2 = readArr[19];
				R3 = readArr[20];

				readArr[0] = 255;



			}
		}
	}

	return 0;
}
