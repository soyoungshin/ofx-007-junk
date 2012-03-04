#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include <iostream>
#include <stdio.h>

#define _LED_ARR_WIDTH 8
#define _MOTOR_PIN 14

class testApp : public ofBaseApp{

	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void setupLedArduino(const int & version);
		void setupControlArduino(const int & version);
		void updateArduino();
	
		ofSoundPlayer  bearVoice;
		bool triggerBear;
		// save state of bear playing
		bool bearWasPlaying;
	
		ofArduino	ledArd, controlArd;
		bool		bSetupLedArduino, bSetupControlArduino;			// flag variable for setting up arduino once
		int count;
	
		static int colPins[8];
		static int rowPins[8];
	
		int ledRow, ledCol, prevLedRow, prevLedCol;
	
		int rowCount;
	
		ofVideoPlayer vid;
	bool vidIsPaused;
	
		
};

