/************************************************************
************************************************************/
#include "ofApp.h"

/************************************************************
************************************************************/

/******************************
******************************/
ofApp::ofApp(int _Cam_id)
: Cam_id(_Cam_id)
{
	font.load("font/RictyDiminished-Regular.ttf", 15, true, true, true);
}

/******************************
******************************/
ofApp::~ofApp(){
	printf("Good-bye\n");
	fflush(stdout);
}


/******************************
******************************/
void ofApp::setup(){
	/********************
	********************/
	ofSetWindowTitle("ofxCv");
	
	ofSetWindowShape( WINDOW_WIDTH, WINDOW_HEIGHT );
	ofSetVerticalSync(true);
	ofSetFrameRate(FrameRate);
	
	ofSetEscapeQuitsApp(false);
	
	/********************
	********************/
	setup_Camera();
	
	/********************
	********************/
	gui.setup();
	gui.add(gui_blur.setup("blur", 16, 1, 30));
	gui.add(gui_Canny_th1.setup("Canny_1", 50, 1, 500));
	gui.add(gui_Canny_th2.setup("Canny_2", 150, 1, 500));
}

/******************************
******************************/
void ofApp::setup_Camera()
{
	/********************
	********************/
	printf("> setup camera\n");
	fflush(stdout);
	
	/********************
	細かく情報出してもらう
	********************/
	ofSetLogLevel(OF_LOG_VERBOSE);
	cam.setVerbose(true);
	
	vector< ofVideoDevice > Devices = cam.listDevices();// 上 2行がないと、List表示されない.
	
	/********************
	********************/
	if(Cam_id == -1){
		ofExit();
		return;
	}else{
		if(Devices.size() <= Cam_id) { ERROR_MSG(); ofExit(); return; }
		
		cam.setDeviceID(Cam_id);
		printf("\n> cam set device id = %d\n", Cam_id);
		printf("> DeviceID	 = %d\n", Devices[Cam_id].id);
		printf("> CamName	  = %s\n", Devices[Cam_id].deviceName.c_str());
		printf("> HardwareName = %s\n", Devices[Cam_id].hardwareName.c_str());
		printf("> serialID	 = %s\n", Devices[Cam_id].serialID.c_str());
		
		printf( "\n> Cam size asked  = (%d, %d)\n", int(CAM_WIDTH), int(CAM_HEIGHT) );
		/*
			https://openframeworks.cc/documentation/video/ofVideoGrabber/#show_setDesiredFrameRate
				Set's the desired frame rate of the grabber. This should be called before initGrabber(), which will try to initialize at the desired frame rate. 
				Not all frame rates will be supported, but this at least gives you some abilitity to try grab at different rates.
		*/
		cam.setDesiredFrameRate(30);
		cam.initGrabber(CAM_WIDTH, CAM_HEIGHT);
		printf( "> Cam size actual = (%d, %d)\n\n", int(cam.getWidth()), int(cam.getHeight()) );
		fflush(stdout);
	}
}

/******************************
******************************/
void ofApp::update(){
	if(gui_Canny_th2 < gui_Canny_th1) gui_Canny_th1 = (float)gui_Canny_th2;
	
	cam.update();
	
	if(cam.isFrameNew()){
		ofxCv::GaussianBlur(cam, blur, gui_blur);
		ofxCv::convertColor(cam, gray, CV_RGB2GRAY);
		ofxCv::Canny(gray, edge, gui_Canny_th1, gui_Canny_th2);
		
		gray.update();
		edge.update();
		blur.update();
	}
}

/******************************
******************************/
void ofApp::draw(){
	/********************
	********************/
	ofBackground(0);
	
	/********************
	********************/
	ofSetColor(255, 255, 255);
	
	cam.draw(0, 0);
	if(blur.isAllocated())	blur.draw(320, 0);
	if(gray.isAllocated())	gray.draw(0, 240);
	if(edge.isAllocated())	edge.draw(320, 240);
	
	/********************
	********************/
	gui.draw();
}

/******************************
******************************/
void ofApp::keyPressed(int key){
}

/******************************
******************************/
void ofApp::keyReleased(int key){

}

/******************************
******************************/
void ofApp::mouseMoved(int x, int y ){

}

/******************************
******************************/
void ofApp::mouseDragged(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mousePressed(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseReleased(int x, int y, int button){

}

/******************************
******************************/
void ofApp::mouseEntered(int x, int y){

}

/******************************
******************************/
void ofApp::mouseExited(int x, int y){

}

/******************************
******************************/
void ofApp::windowResized(int w, int h){

}

/******************************
******************************/
void ofApp::gotMessage(ofMessage msg){

}

/******************************
******************************/
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
