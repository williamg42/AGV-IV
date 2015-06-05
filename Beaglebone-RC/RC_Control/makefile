CXXFLAGS=-Wall -std=c++0x -I./include -I./include/v2_0

all: RemoteControl

RemoteControl: main.cpp  src/pruPWM.o src/pru.o include/pruPWM.h include/pru.h pwm.bin ./MovingAverageFilter.o ./MovingAverageFilter.h 
	g++ $(CXXFLAGS) main.cpp src/pruPWM.o src/pru.o ./MovingAverageFilter.o -lprussdrv -lpthread -lBlackLib -o RemoteControl

pwm.bin: src/pwm.p include/pru.hp
	cd src; pasm -b pwm.p; mv pwm.bin ../; cd ..
	
clean:
	rm pwm.bin prupwm src/*.o ./*.o

	
	
