#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

	ofSetVerticalSync(true);
	
	ofSetFrameRate(30);
	
	ofBackground(255,0,130);

	bgImage.loadImage("firmata.png");
	font.loadFont("franklinGothic.otf", 20);

	ard.connect("/dev/tty.usbserial-A9007OYe", 57600);
	
	// listen for EInitialized notification. this indicates that
	// the arduino is ready to receive commands and it is safe to
	// call setupArduino()
	ofAddListener(ard.EInitialized, this, &testApp::setupArduino);


	bSetupArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
	

	
	period = 45;
	restTime = 0; //20;
	
	motor1RadOffset = motor2RadOffset = 0;
	descendingMultiplier = .7719; // TODO: double check this value is correct.
	
	settings = ofxTweakbars::create("settings", "Basic Settings");
	settings->addFloat("motor1RadOffset", &motor1RadOffset)->setMin(-2*PI)->setMax(2*PI);
	settings->addFloat("motor2RadOffset", &motor2RadOffset)->setMin(-2*PI)->setMax(2*PI);
	settings->addFloat("descendingMultiplier", &descendingMultiplier)->setMin(0)->setMax(1);
	settings->addInt("wave period", &period)->setMin(30)->setMax(500);
	settings->addInt("restTime", &restTime)->setMin(0)->setMax(120);
	
	ini_file = new ofxIniFile("settings.ini");
	ini = new ofxTweakbarINI(settings, ini_file);
	
	
	restartSystem = false;
	pauseMotors = true;
}

void testApp::setupWaves(){
	startTime = time(NULL);
	oldSecondsFromStart = 0;
	pauseMotors = false;
	
}

//--------------------------------------------------------------
void testApp::update(){
	if(restartSystem) {
		restartSystem = false;
		setupWaves();
	}
	updateArduino();

}

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);

	// this is where you setup all the pins and pin modes, etc
	
	ard.sendDigitalPinMode(MOTOR_1_PIN_1, ARD_PWM);
	ard.sendDigitalPinMode(MOTOR_1_PIN_2, ARD_PWM);	
	ard.sendDigitalPinMode(MOTOR_2_PIN_1, ARD_PWM);	
	ard.sendDigitalPinMode(MOTOR_2_PIN_2, ARD_PWM);	
	

	ard.sendDigitalPinMode(ENABLE_1, ARD_OUTPUT);
	ard.sendDigitalPinMode(ENABLE_2, ARD_OUTPUT);	
	
	ard.sendDigitalPinMode(13, ARD_OUTPUT);
	
	ard.sendDigital(ENABLE_1, ARD_HIGH, false);
	ard.sendDigital(ENABLE_2, ARD_HIGH, false);
	
	ard.sendDigital(13, ARD_HIGH);
	
	bSetupArduino = true;
	
}

//--------------------------------------------------------------
void testApp::updateArduino(){

	// update the arduino, get any data or messages.
	ard.update();
	
	// do not send anything until the arduino has been set up
	if (bSetupArduino) {
		if(!pauseMotors) {
			
			int secondsFromStart = time(NULL) - startTime;
			cout << "time: " << time(NULL) << endl;
			
			if(secondsFromStart > oldSecondsFromStart) {
				cout << "seconds from start: " << secondsFromStart << endl;
				oldSecondsFromStart = secondsFromStart;
				double rads = ((double)secondsFromStart) / period * 2 * PI;
				// 
				float s = motor1Value  = sin(rads);
				float c = motor2Value = cos(rads);
				
				
				if(s > 0) {
					ard.sendPwm(MOTOR_1_PIN_1, 255 * motor1Value);
					ard.sendPwm(MOTOR_1_PIN_2, ARD_LOW);
				} else {  
					ard.sendPwm(MOTOR_1_PIN_2, abs(255 * motor1Value) * descendingMultiplier);
					ard.sendPwm(MOTOR_1_PIN_1, ARD_LOW);
				}
				
				if(c > 0) {
					ard.sendPwm(MOTOR_2_PIN_1, 255 * motor2Value);
					ard.sendPwm(MOTOR_2_PIN_2, ARD_LOW);
				} else {  
					ard.sendPwm(MOTOR_2_PIN_2, abs(255 * motor2Value) * descendingMultiplier);
					ard.sendPwm(MOTOR_2_PIN_1, ARD_LOW);
				}   		
			}
		}
		

		
		
	}
	

}


//--------------------------------------------------------------
void testApp::draw(){
	//bgImage.draw(0,0);
		ofxTweakbars::draw();
	
	
	if (bSetupArduino){
		ofDrawBitmapString("motor1Value: " + ofToString(motor1Value) + 
						   "\nmotor2Value: " + ofToString(motor2Value) + 
						   "radians:     " + ofToString(radians) +
						   "\n\n" +
						   "press 'a' or 's' to jog motor 1 forwards or backwards.\n" +
						   "press 'q' or 'w' to jog motor 2 forwards or backwards.\n" +
						   "and press spacebar to stop motors completely"				   
						   , 400, 20);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
	// these might be backwards
	switch (key) {
		case 'a':
			ard.sendPwm(MOTOR_1_PIN_1, 255);
			ard.sendPwm(MOTOR_1_PIN_2, ARD_LOW);
			sleep(1000);
			ard.sendPwm(MOTOR_1_PIN_1, ARD_LOW);
			break;
		case 'o':
		case 's':
			ard.sendPwm(MOTOR_1_PIN_1, ARD_LOW);
			ard.sendPwm(MOTOR_1_PIN_2, 255);
						sleep(1000);
						ard.sendPwm(MOTOR_1_PIN_2, ARD_LOW);
			break;
		case '\'':
		case ',':
			ard.sendPwm(MOTOR_2_PIN_1, 255);
			ard.sendPwm(MOTOR_2_PIN_2, ARD_LOW);
			sleep(1000);			
			ard.sendPwm(MOTOR_2_PIN_1, ARD_LOW);			
			break;
		case 'q':
		case 'w':
			ard.sendPwm(MOTOR_2_PIN_1, ARD_LOW);
			ard.sendPwm(MOTOR_2_PIN_2, 255);
			sleep(1000);			
			ard.sendPwm(MOTOR_2_PIN_2, ARD_LOW);
			break;
		case ' ': 
			pauseMotors = !pauseMotors; 
			break;
		case '0':
			restartSystem = true;
		case '1':
			// if time, debug this.
			//ini->store();
			
		default:
			break;
	}
	
	
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}