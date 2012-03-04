#include "testApp.h"
/*
int testApp::colPins[8] = {13, 3, 4, 10, 6, 11, 15, 16};
int testApp::rowPins[8] = {9, 14, 8, 12, 1, 7, 2, 5};

 */
/*
//							0  1   2   3  4  5   6   7
int testApp::colPins[8] = {14, 4,  5, 11, 7, 12, 16, 17};
int testApp::rowPins[8] = {10, 15, 9, 13, 2,  8,  3, 6};

*/
// using analog 2-5 instead

int testApp::colPins[8] = {16, 4, 5, 11, 7, 12, 18, 19};
int testApp::rowPins[8] = {10, 17, 9, 13, 2, 8, 3, 6};


//--------------------------------------------------------------
void testApp::setup(){	 
	triggerBear = false;
	bearWasPlaying = false;
	
	// led matrix
	
	ledArd.connect("/dev/tty.usbserial-A9007OYe", 57600);
	controlArd.connect("/dev/tty.usbserial-A6008hrY", 57600);
	// motor and interactive controls
	ofAddListener(ledArd.EInitialized, this, &testApp::setupLedArduino);
	ofAddListener(controlArd.EInitialized, this, &testApp::setupControlArduino);
	
	bSetupLedArduino	= false;	// flag so we setup arduino when its ready, you don't need to touch this :)
	bSetupControlArduino = false;
	
	count = 0;
	
	ledRow = ledCol = prevLedRow = prevLedCol = 5;
	vid.loadMovie("love-short.mov");
	vid.stop();
	vid.setLoopState(OF_LOOP_NORMAL);
	rowCount = 0;
	vidIsPaused = false;
}

//--------------------------------------------------------------
void testApp::update(){	
	
	ofBackground(255,255,255);
	
	// update the sound playing system:
	ofSoundUpdate();
	updateArduino();
	

	count++;
	vid.idleMovie();
}

//--------------------------------------------------------------
void testApp::setupLedArduino(const int & version) {
	
	ofRemoveListener(ledArd.EInitialized, this, &testApp::setupLedArduino);
	// this is where you setup all the pins and pin modes, etc
	
	for (int i = 0; i < _LED_ARR_WIDTH; i++){	
		ledArd.sendDigitalPinMode(colPins[i], ARD_OUTPUT);
		ledArd.sendDigitalPinMode(rowPins[i], ARD_OUTPUT);
	}
	// turn off all the LEDS
	
	for(int i = 0; i < _LED_ARR_WIDTH; i++) {
		
		ledArd.sendDigital(colPins[i], ARD_HIGH);
		ledArd.sendDigital(rowPins[i], ARD_LOW);

	}

	
	bSetupLedArduino = true;
	cout << "led set up" << endl;
	vid.play();
	
}

//--------------------------------------------------------------
void testApp::setupControlArduino(const int & version) {
	
	// remove listener because we don't need it anymore
	ofRemoveListener(controlArd.EInitialized, this, &testApp::setupControlArduino);
	
	// this is where you setup all the pins and pin modes, etc
	//for (int i = 0; i < 13; i++){
	
	// this will be bear 1
	controlArd.sendDigitalPinMode(7, ARD_OUTPUT);
	
	//ard.sendDigitalPinMode(7, ARD_OUTPUT);
	
	//}
	
	 //ard.sendDigitalPinMode(13, ARD_OUTPUT);
	 //ard.sendAnalogPinReporting(0, ARD_ANALOG);	// AB: report data
	 //ard.sendDigitalPinMode(11, ARD_PWM);		// on diecimelia: 11 pwm?
	 
	bSetupControlArduino = true;

}


//--------------------------------------------------------------
void testApp::updateArduino(){
	
	
	// update the arduino, get any data or messages.
	controlArd.update();
	ledArd.update();
	/*
	// do not send anything until the arduino has been set up
	if (bSetupControlArduino) {
		cout << "both are setup" << endl; 
		//ard.sendPwm(11, (int)(128 + 128 * sin(ofGetElapsedTimef())));   // pwm...
		// something about an ir sensor
		
		
		// NOTE!!! do this sparingly.
		if(count % 2000 == 1000) {
			//controlArd.sendDigital(7, ARD_HIGH);
			
			// __THE_PROCESS_HAS_FORKED_AND_YOU_CANNOT_USE_THIS_COREFOUNDATION_FUNCTIONALITY___YOU_MUST_EXEC__() to debug.
			// The process has forked and you cannot use this CoreFoundation functionality safely. You MUST exec().
			
			 
			 
			//int pid = fork();
			//if(pid == 0) {
			//	
			//
			//	int s = system("say \"hello there you poopey face\"");
			//}
			 
		} else {
			controlArd.sendDigital(7, ARD_LOW);
		}
		
		
		
		
	}
	
	 */
	
	
	if(bSetupLedArduino && bSetupControlArduino) {
		// set the old cathode high, old anode low
		// prev row pin low, prev col pin high
		/*
		if(prevLedCol != ledCol || prevLedRow != ledRow) {
			ledArd.sendDigital(colPins[prevLedCol], ARD_HIGH);
			ledArd.sendDigital(rowPins[prevLedRow], ARD_LOW);

			ledArd.sendDigital(colPins[ledCol], ARD_LOW);
			ledArd.sendDigital(rowPins[ledRow], ARD_HIGH);
			
			// set the current cathode low, old anode high
			// curr row pin high, curr col pin low
			prevLedCol = ledCol;
			prevLedRow = ledRow;
		}*/
		
		// reset old pins
		unsigned char * pixels = vid.getPixels();
		int prevRow = rowCount == 0 ? 7 : rowCount - 1;
		for(int c = 0; c < 8; c++) {
			ledArd.sendDigital(colPins[c], ARD_HIGH);
		}
		ledArd.sendDigital(rowPins[prevRow], ARD_LOW);
		
		// set new pins
		for(int c = 0; c < 8; c++) {
			unsigned char p = pixels[(rowCount * 8 + c) * 3];
			if(p >128) {
				ledArd.sendDigital(colPins[c], ARD_LOW);
			}
		}
		ledArd.sendDigital(rowPins[rowCount], ARD_HIGH);
		
		rowCount = (rowCount+1) % 8;
		
		
		// motor
		
		int frameNum = vid.getCurrentFrame();
		switch (frameNum) {
			case 6:
			case 16:
			case 25:
			case 35:
			case 43:
			case 52:
			case 62:
			case 80:
			case 106:
			case 255:
			case 270:
			case 294:
			case 320:
				controlArd.sendDigital(7, ARD_HIGH);
				break;
			case 12:
			case 21:
			case 29:
			case 38:
			case 48:
			case 58:
			case 72:
			case 90:
			case 230:
			case 265:
			case 285:
			case 303:
			case 342:
				controlArd.sendDigital(7, ARD_LOW);
				break;
			default:
				break;
		}
		 
		
		
	}
	 /*
	
	if(bSetupLedArduino) {
		int currentRow = rowCount % 8;
		ledArd.sendDigital(currentRow == 0 ? 7 : currentRow - 1, ARD_LOW);
		ledArd.sendDigital(rowPins[currentRow], ARD_HIGH);
		for(int i = 0; i < _LED_ARR_WIDTH; i++) {
			ledArd.sendDigital(colPins[i], ARD_LOW);
		}
		
		rowCount++;
		
	}
	*/
	  
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(255, 0, 0);
	//char tempStr[255];
	string buf;
	//buf = bearWasPlaying ? "bear 1 is playing\n" : "bear 1 not playing";
	//buf = "count is: " + count;
	//cout << "col : " << ledCol << "\nrow: " << ledRow << endl;
	buf = "col : " + ofToString(ledCol, 2) + "\nrow: " + ofToString(ledRow, 2) + 
			"\n\nequivalent to pin diagram:\n" +
			"\tcol: " + ofToString(ledCol+1, 2) +
			"\n\trow: " + ofToString(ledRow + 1, 2) +
			"\n\nard pins: \n" 
			"\tcol: " + ofToString(colPins[ledCol], 2) + "\n" +
			"\trow: " + ofToString(rowPins[ledRow], 2) + "\n\n" + 
			"current frame: " + ofToString(vid.getCurrentFrame(), 5);
	
	
	ofDrawBitmapString(buf, 50, 50);
	vid.draw(200,200);
	
	
	/*
	
	// draw the background colors:
	float widthDiv = ofGetWidth() / 3.0f;
	ofSetHexColor(0xeeeeee);
	ofRect(0,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xffffff);
	ofRect(widthDiv,0,widthDiv,ofGetHeight()); 
	ofSetHexColor(0xdddddd);
	ofRect(widthDiv*2,0,widthDiv,ofGetHeight()); 
	

	//---------------------------------- synth:
	if (synth.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("synth !!", 50,50);
	
	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play\npct done: %f\nspeed: %f\npan: %f", synth.getPosition(),  synth.getSpeed(), synth.getPan());
	ofDrawBitmapString(testAppmpStr, 50,ofGetHeight()-50);



	//---------------------------------- beats:
	if (beats.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("beats !!", widthDiv+50,50);

	ofSetHexColor(0x000000);
	sprintf(tempStr, "click and drag\npct done: %f\nspeed: %f", beats.getPosition(),  beats.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv+50,ofGetHeight()-50);

	//---------------------------------- vocals:
	if (vocals.getIsPlaying()) ofSetHexColor(0xFF0000);
	else ofSetHexColor(0x000000);
	font.drawString("vocals !!", widthDiv*2+50,50);

	ofSetHexColor(0x000000);
	sprintf(tempStr, "click to play (multiplay)\npct done: %f\nspeed: %f", vocals.getPosition(),  vocals.getSpeed());
	ofDrawBitmapString(tempStr, widthDiv*2+50,ofGetHeight()-50);
	 */
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	switch (key) {
		case OF_KEY_LEFT:
			ledCol = max(0, ledCol - 1);
			break;
		case OF_KEY_RIGHT:
			ledCol = min(7, ledCol + 1);
			break;
		case OF_KEY_DOWN:
			ledRow = min(7, ledRow + 1);			
			break;
		case OF_KEY_UP:

			ledRow = max(0, ledRow - 1);
			break;
		case ' ':
			vidIsPaused = !vidIsPaused;
			vid.setPaused(vidIsPaused);
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
	// manual sound trigger here
	system("say -v Junior \"hello there you poopey face\"");
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
