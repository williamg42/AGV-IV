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

#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex m;
std::mutex m2;
std::mutex mgen;
std::condition_variable cv;
std::condition_variable cv2;
std::string data;
std::string data2;
bool newdata = false;
bool waitingfordata = false;
bool datatosend = false;

void Serialread_thread() {

	//Initialize serial data ports for reading
	while (1) {

		//Whatever logic need to read serial data
		usleep(2000000);

		mgen.lock();
		//Put data is some form of data
		data = "Example data";
		//Set new data flag
		newdata = true;
		std::cout << "UART packet received\n";
		mgen.unlock();

		// wait for the main
		{
			std::unique_lock<std::mutex> lk(m);
			cv.wait(lk, [] {return waitingfordata;});
			lk.unlock();
		}
		mgen.lock();
		std::cout << "Listening for more data" << '\n';
		waitingfordata = false;
		mgen.unlock();

	}

}

void Serialsender_thread() {

	//Initialize serial data ports for reading
	while (1) {

		{
			//Waits for new data to send flag to be high
			std::unique_lock<std::mutex> lk2(m2);
			cv2.wait(lk2, [] {return datatosend;});

			mgen.lock();
			//sets flag to false
			datatosend = false;
			//do whatever to send the data in the buffer
			std::cout << "Serial Data Sent" << std::endl;
			std::cout << data2 << " " << std::endl;
			mgen.unlock();
			lk2.unlock();
		}

	}

}

int main() {
	std::thread reader(Serialread_thread);
	reader.detach();
	std::thread sender(Serialsender_thread);
	sender.detach();

	while (1) {
		mgen.lock();
		bool local = newdata;
		mgen.unlock();

		switch (local) {

		case 0: {

			break;

		}

		case 1: {
			std::unique_lock<std::mutex> lk(m);
			// after the wait, we own the lock.
			newdata = false;
			std::cout << "Processing UART commands \n";
			mgen.lock();
			data += " after processing";
			mgen.unlock();

			// Send data back to main()
			waitingfordata = true;
			std::cout << "UART command processed\n";
			{
				std::lock_guard<std::mutex> lk2(m2);
				datatosend = true;
				mgen.lock();
				data2 = "Data out";

				std::cout << "Serial data sending" << std::endl;
				mgen.unlock();
			}

			cv2.notify_one();

			// Manual unlocking is done before notifying, to avoid waking up
			// the waiting thread only to block again (see notify_one for details)
			lk.unlock();
			cv.notify_one();
			break;

		}

		default: {

			break;
		}

		}
	}

	//never occurs
}


//Old working program

/*
 *
 * std::mutex m;
std::condition_variable cv;
std::string data;
bool newdata = false;
bool waitingfordata = false;

void Serialread_thread()
{
	usleep(20000000);
    data = "Example data";
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(m);
        newdata = true;
        std::cout << "Data signals main, ready for processing\n";
    }
    cv.notify_one();

    // wait for the main
    {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, []{return waitingfordata;});
    }
    std::cout << "Back in Data" << '\n';

}

int main()
{
    std::thread reader(Serialread_thread);
    reader.detach();

    // Wait until data() sends data
   std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, []{return newdata;});

    // after the wait, we own the lock.
    std::cout << "Main thread is processing data\n";
    data += " after processing";

    // Send data back to main()
    waitingfordata = true;
    std::cout << "Main thread signals data processing completed\n";

    // Manual unlocking is done before notifying, to avoid waking up
    // the waiting thread only to block again (see notify_one for details)
    lk.unlock();
    cv.notify_one();




    std::cout <<"Ending program" << std::endl;
}

*/

