#ifndef H_GPIO_H_
#define H_GPIO_H_

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdlib.h>



using namespace std;


class GPIO
{
private:
	int gpioPin;
	string gpioPinPath;
	const string gpioPath = "/sys/class/gpio/";

	void writeToFile(string path, string fileName, string value);
	void setUnsetGPIO(string fileName);

public:
	GPIO(int pin);
	~GPIO();

	void setPinAttributes(string fileName, string value);
	void edgeListen();
};

#endif /* H_GPIO_H_ */
