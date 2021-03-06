#include "PWM.h"

//TODO: This code does not work on BBB 8.6 (Jessie) because the DTO is different from that of 7.11(Wheezy) FIX!!
/*
 * Constructor
 */
PWM::PWM(string dto, string pinName) : pwmPin(pinName)
{
	checkVersion();

	if(version.compare("wheezy") == 0)
	{
		this->pwmPinPath = this->pwmPath + this->pwmPin + "/";

		deployOverlay(this->amOverlay);
		deployOverlay(dto);

		usleep(250000); // Delay to allow the setup of sysfs
	}
	else if(version.compare("jessie") == 0)
	{
		this->pwmPinPath = this->pwmPath + "pwmchip0/" + this->pwmPin + "/";

		deployOverlay(dto);
		usleep(250000); // Delay to allow the setup of sysfs
		writeToFile(this->pwmPath + "pwmchip0/", "export", 0);
	}
}

/*
 * Destructor
 */
PWM::~PWM()
{
}

/*
 * Used to write to the PWM files to change the pin's properties
 *
 * path: The path to the PWM Pin
 * filename: The name of the file whose value you want to change
 * value: The value you want to assign to the 'filename'
 */
void PWM::writeToFile(string path, string filename, int value)
{
	ofstream fileStream;
	string valueToWrite = to_string(value);

	fileStream.open((path + filename).c_str());

	if(fileStream.is_open())
	{
		fileStream << valueToWrite;
		fileStream.close();
	}
	else
	{
		perror("open - writeToFile");
		exit(EXIT_FAILURE);
	}
}

/*
 * Used to read from the PWM files
 *
 * path: The path to the PWM pin
 * filename: The name of the file whose value you want to read
 */
string PWM::readFromFile(string path, string filename)
{
	ifstream fileStream;
	string value;

	fileStream.open( (path + filename).c_str());

	if(fileStream.is_open())
	{
		fileStream >> value;
		fileStream.close();
	}
	else
	{
		perror("open");
		exit(EXIT_FAILURE);
	}

	return value;
}

/*
 * Deploys the DTO necessary to work with PWM pins.
 *
 * overlay: The name of the DTO to be deployed.
 */
void PWM::deployOverlay(string overlay)
{
	ofstream myPwmFile;

	myPwmFile.open(this->slotsPath.c_str());

	if(myPwmFile.is_open())
	{
		myPwmFile << overlay;
		myPwmFile.close();
	}
	else
	{
		perror("open - deployOverlay");
		exit(EXIT_FAILURE);
	}
}

/*
 * Sets the:
 * 	- period
 * 	- duty
 * 	- polarity
 * 	- run
 *
 * 	attributes: The pin's properties that will be modified
 * 	value: The value that will be assigned to the pin's properties
 */
void PWM::setPinAttributes(string attributes, unsigned int value)
{
	writeToFile(this->pwmPinPath, attributes, value);
}

/*
 * Gets the:
 * 	- period
 * 	- duty
 * 	- polarity
 * 	- run
 *
 * 	attributes: The pin's properties that will be retreived
 */
int PWM::getPinAttributes(string attributes)
{
	return atoi(readFromFile(this->pwmPinPath, attributes).c_str());	// cat ./period
}

/*
 * Check the version of the BBB being used.
 * The path to slots is different between version 7.11 (wheezy) and 8.6 (jessie)
 */
void PWM::checkVersion()
{
	ifstream myFile;
	string osRelease = "/etc/os-release";
	string input;

	myFile.open(osRelease.c_str());
	getline(myFile, input);

	if( input.find("jessie") == string::npos)
	{
		version = "wheezy";
		pwmPath = "/sys/devices/ocp.3/";
		slotsPath = "/sys/devices/bone_capemgr.9/slots";
	}
	else
	{
		version = "jessie";
		pwmPath = "/sys/class/pwm/";
		slotsPath = "/sys/devices/platform/bone_capemgr/slots";
	}
}
