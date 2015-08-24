#include <Wire.h>

typedef enum {ERROR = -1, FALSE, TRUE} LOGICAL;

#define BOOL(x) (!(!(x)))

#define BitSet(arg,posn) ((arg) | (1L << (posn)))
#define BitClr(arg,posn) ((arg) & ~(1L << (posn)))
#define BitTst(arg,posn) BOOL((arg) & (1L << (posn)))
#define BitFlp(arg,posn) ((arg) ^ (1L << (posn)))


#define MODE_REGISTER	0x05
#define MODE_STANDBY 0x0
#define MODE_GESTURE 0x1
#define MODE_TOUCH	 0x2
#define MODE_FULL    0x3
#define MODE_RAW_ADC 0x4


#define RAWADC_REGISTER		0xD0
#define RAWADC_CVD_BOTH		0x0
#define RAWADC_RESULT_1		0X1
#define RAWADC_RESULT_2		0x2

//Bit positions in CMD register
#define CMD_REGISTER	0x04
#define CMD_BS		0x0
#define CMD_MFG		0x3
#define CMD_CFG		0x5
#define CMD_DEF		0x6
#define CMD_NV		0x7

#define TOUCHSTATE_REGISTER 0x10
#define TOUCHSTATE_TCH		0
#define TOUCHSTATE_GES		1
#define TOUCHSTATE_LRG		2
//TODO
#define TOUCHSTATE_FRAME

#define TOUCHX_REGISTER 0x11
#define TOUCHY_REGISTER 0x12

#define TOUCHLSB_REGISTER 0x13
#define TOUCHLSB_Y
#define TOUCHLSB_X


//Configuration Addresses:
#define CONFIG_NUMBEROFXCHANNELS			0x20		//Default value: 0x09
#define CONFIG_NUMBEROFYCHANNELS			0x21		//Default value: 0x06
#define CONFIG_SCANCOUNT					0x22		//Default value: 0x06
#define CONFIG_TOUCHTHRESHX					0x23		//Default value: 0x37
#define CONFIG_TOUCHTHRESHY					0x24		//Default value: 0x28
#define CONFIG_ACTIVEPERIODL				0x25		//Default value: 0x85
#define CONFIG_ACTIVEPERIODH				0x26		//Default value: 0x02
#define CONFIG_IDLEPERIODL					0x27		//Default value: 0x4C
#define CONFIG_IDLEPERIODH					0x28		//Default value: 0x06
#define CONFIG_IDLETIMEOUT					0x29		//Default value: 0x10
#define CONFIG_HYSTERESIS					0x2A		//Default value: 0x04
#define CONFIG_DEBOUNCEUP					0x2B		//Default value: 0x01
#define CONFIG_DEBOUNCEDOWN					0x2C		//Default value: 0x01
#define CONFIG_BASEINTERVALL				0x2D		//Default value: 0x0A
#define CONFIG_BASEINTERVALH				0x2E		//Default value: 0x00
#define CONFIG_BASEPOSFILTER				0x2F		//Default value: 0x14
#define CONFIG_BASENEGFILTER				0x30		//Default value: 0x14
#define CONFIG_FILTERTYPE					0x31		//Default value: 0x02
#define CONFIG_FILTERSTRENGTH				0x32		//Default value: 0x01
#define CONFIG_BASEFILTERTYPE				0x33		//Default value: 0x01
#define CONFIG_BASEFILTERSTRENGTH			0x34		//Default value: 0x05
#define CONFIG_LARGEACTIVATIONTHRESHL		0x35		//Default value: 0x00
#define CONFIG_LARGEACTIVATIONTHRESHH		0x36		//Default value: 0x00
#define CONFIG_HORIZONTALSWIPEDISTANCE		0x37		//Default value: 0x40
#define CONFIG_VERTICALSWIPEDISTANCE		0x38		//Default value: 0x40
#define CONFIG_SWIPEHOLDBOUNDARY			0x39		//Default value: 0x19
#define CONFIG_TAPDISTANCE					0x3A		//Default value: 0x19
#define CONFIG_DISTANCEBETWEENTAPS			0x3B		//Default value: 0x40
#define CONFIG_TAPHOLDTIMEL					0x3C		//Default value: 0x32
#define CONFIG_TAPHOLDTIMEH					0x3D		//Default value: 0x00
#define CONFIG_GESTURECLICKTIME				0x3E		//Default value: 0x0C
#define CONFIG_SWIPEHOLDTHRESH				0x3F		//Default value: 0x20
#define CONFIG_MINSWIPEVELOCITY				0x40		//Default value: 0x04
#define CONFIG_HORIZONTALGESTUREANGLE		0x41		//Default value: 0x2D
#define CONFIG_VERTICALGESTUREANGLE			0x42		//Default value: 0x2D
#define CONFIG_I2CADDR						0x43		//Default value: 0x25

void setup()
{
	Wire.begin();
	Serial.begin(115200);
	pinMode(A0, INPUT);
	pinMode(A1, INPUT);
	writeRegister(MODE_REGISTER, MODE_TOUCH);

	writeRegister(CONFIG_FILTERTYPE, 0x02);

	writeRegister(CONFIG_TOUCHTHRESHX, 0x05);//0x37
	writeRegister(CONFIG_TOUCHTHRESHY, 0x05); //0x28
}

byte sensorvalues[15] = {0};

void loop()
{
	while(digitalRead(A1) != 0) {}

	readSensorValues(sensorvalues);
	//readRawValues(sensorvalues);
	for (byte i = 0; i < 15; i++) {
		Serial.print(sensorvalues[i]);
		Serial.print(", ");
	}
	Serial.println();




}



byte writeRegister(byte address, byte val) {
	Wire.beginTransmission(0x25);
	Wire.write(address);
	Wire.write(val);
	Wire.endTransmission();


	return readRegister(address);
}



byte readRegister(byte address) {
	byte rx = 0;

	Wire.beginTransmission(0x25);
	Wire.write(address);
	Wire.endTransmission();

	Wire.requestFrom(0x25, 1);

	while(Wire.available()) 
	{
		rx = Wire.read();
	}

	return rx;
}

void readSensorValues(byte * values) {
	int startAddress = 0x80;
	for (byte i = 0; i < 15; i++) {
		values[i] = readRegister(startAddress + i);
	}
}

void readRawValues(byte * values) {
	int startAddress = 0x90;
	for (byte i = 0; i< 15; i++) {
		values[i] = readRegister(startAddress + i);
	}
}


