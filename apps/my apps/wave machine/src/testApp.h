#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include <time.h>
#include "ofxTweak.h"
#include "ofxTweakbarINI.h"

#define MOTOR_1_PIN_1 3    // H-bridge leg 1 pwm
#define MOTOR_1_PIN_2 5    // H-bridge leg 2 pwm
#define ENABLE_1      7    // H-bridge enable pin

#define MOTOR_2_PIN_1 9    // H-bridge leg 1 pwm
#define MOTOR_2_PIN_2 10   // H-bridge leg 2 pwm
#define ENABLE_2      12   // H-bridge enable pin

#define MOTOR1PIN1 

class testApp : public ofSimpleApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
		
	void setupArduino(const int & version);
	void updateArduino();
	void setupWaves();

	ofImage				bgImage;
	ofTrueTypeFont		font;
	ofArduino	ard;
	bool		bSetupArduino;			// flag variable for setting up arduino once
	
	time_t startTime;
	time_t oldSecondsFromStart;
	
	// time in seconds for the circle motion.
	int period;
	// time in seconds for the stationary part
	int restTime;
	
			ofxTweakbar* settings;
	
	float motor1RadOffset;
	float motor2RadOffset;
	float descendingMultiplier;

	float motor1Value;
	float motor2Value;
	float radians;
	
	bool pauseMotors;
	bool restartSystem;
	
	ofxIniFile* ini_file;
	ofxTweakbarINI* ini;
	
};

