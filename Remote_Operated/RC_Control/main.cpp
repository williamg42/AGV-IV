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
#include <stdint.h>
#include "pruPWM.h"
#include "BlackUART.h"
#include "crc.h"
#include "BlackGPIO.h"
#include "MovingAverageFilter.h"

long map(long x, long in_min, long in_max, long out_min, long out_max);

char readArr[23];

char message[23];

bool startbutton, selectbutton, L3, R3 = 0;

int LY, LX, RY, RX, Up, Down, Left, Right, X, triangle, square1, circle, L1, L2,
    R1, R2 = 0;

bool Lights = false;

int pastx = 0;

unsigned short checksum = 0;

MovingAverageFilter LeftChannel(100);
MovingAverageFilter RightChannel(100);

int main() {

	crcInit();

	PRUPWM *myPWM = new PRUPWM();
	BlackLib::BlackGPIO  FloodLights(BlackLib::GPIO_60, BlackLib::output);
	FloodLights.setValue(BlackLib::low);


	// Set a 2s failsafe timeout
	myPWM->setFailsafeTimeout(2000);


	// Start the PRU
	myPWM->start();


	BlackLib::BlackUART UART1(BlackLib::UART1, BlackLib::Baud57600,
	                          BlackLib::ParityNo, BlackLib::StopOne, BlackLib::Char8);

	BlackLib::BlackUART UART2(BlackLib::UART2, BlackLib::Baud57600,
	                          BlackLib::ParityNo, BlackLib::StopOne, BlackLib::Char8);

	bool isOpened = UART1.open(BlackLib::ReadWrite | BlackLib::NonBlock);

	if (!isOpened) {
		std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
		exit(1);
	}

	isOpened = UART2.open(BlackLib::ReadWrite);

	if (!isOpened) {
		std::cout << "UART DEVICE CAN\'T OPEN.;" << std::endl;
		exit(1);
	}


	while (1) {

		readArr[0] = 255;

		if (UART2.Byteavailable() > 19) {

			usleep(2000);

			UART2.read(readArr, sizeof readArr);

			//check packets to see if they are good.

			if (0)//crcFast((unsigned char *) readArr, 23) > 0)
			{
				//error, bad message

				std::cout << "Message Bad" << std::endl;

				startbutton = readArr[1]; //start (boolean)
				selectbutton = readArr[2]; //select (boolean)

				LY = 127;
				LX = 127;
				RY = 127;
				RX = 127;
				Up = 0;
				Down = 0;
				Left = 0;
				Right = 0;
				triangle = 0;
				circle = 0;
				square1 = 0;
				X = 0;
				L1 = 0;
				L2 = 0;
				L3 = 0;
				R1 = 0;
				R2 = 0;
				R3 = 0;
			}


			else
			{
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
				}


			}


			/////////////////////////////////////////////////////////////



			if (X ==  0 && pastx != 0)
			{
				Lights = !Lights;

			}

			if (Lights)
				FloodLights.setValue(BlackLib::high);
			else
				FloodLights.setValue(BlackLib::low);

			long Left = LeftChannel.process(LY);
			long Right = RightChannel.process(RY);

			std::cout << Left << std::endl;
			std::cout << Right << std::endl;


			int pru0 = map(Left, 0, 255, 670000, 2330000);
			int pru1 = map(Right, 0, 255, 670000, 2330000);


			myPWM->setChannelValue(0, pru0); //Left Motor
			myPWM->setChannelValue(7, pru1); //Right Motor


			UART2.write("1");

			usleep ((7 + 25) * 1);



			readArr[0] = 255;
			pastx = X;


		}


	}

	return 0;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


