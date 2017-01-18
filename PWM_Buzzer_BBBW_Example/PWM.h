#ifndef H_PWM_H_
#define H_PWM_H_

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

using namespace std;

/* PWM Device Tree Overlays
 * am33xx_pwm
 * bone_pwm_P9_16
 * bone_pwm_P8_13
 * bone_pwm_P9_21
 * bone_pwm_P8_19
 * bone_pwm_P9_22
 * bone_pwm_P8_34
 * bone_pwm_P9_28
 * bone_pwm_P8_36
 * bone_pwm_P9_29
 * bone_pwm_P8_45
 * bone_pwm_P9_31
 * bone_pwm_P8_46
 * bone_pwm_P9_42
 * bone_pwm_P9_14
 */

class PWM
{
private:
	string pwmPin;
	string pwmPinPath;
	const string amOverlay = "am33xx_pwm";
	const string pwmPath = "/sys/devices/ocp.3/";
	string slotsPath = "/sys/devices/bone_capemgr.9/slots";


	void writeToFile(string path, string filename, int value);
	string readFromFile(string path, string filename);
	void deployOverlay(string overlay);
	void checkVersion();

public:

	PWM(string dto, string pinName);
	~PWM();
	void setPinAttributes(string attributes, unsigned int value);
	int getPinAttributes(string attributes);
};

#endif /* H_PWM_H_ */
