#include "ofApp.h"

void ofApp::setup() {
    useGaussian = false;
	
	ofSetFrameRate(30);
	
	cam.setDeviceID(1);
	cam.setDesiredFrameRate(30);
    cam.setup(640, 480);
    
    gui.setup();
    gui.add(useGaussian.set("Use Gaussian", false));
    gui.add(radius.set("Radius", 50, 0, 100));
}

void ofApp::update() {
    cam.update();
    if(cam.isFrameNew()) {
		ofxCv::copy(cam, img); // need this to allocate.
		
		cv::Mat mat_cam = ofxCv::toCv(cam);
		cv::Mat mat_img = ofxCv::toCv(img);
		
		/********************
		filter size must be odd.
		********************/
		if(radius % 2 == 0) cv::GaussianBlur(mat_cam, mat_img, cv::Size(radius + 1, radius + 1), 0/* sigmaX */);
		else				cv::GaussianBlur(mat_cam, mat_img, cv::Size(radius, radius), 0/* sigmaX */);
		
        img.update();
    }
}

void ofApp::draw() {
    if(img.isAllocated()) {
        img.draw(0, 0);
    }
    gui.draw();
}
