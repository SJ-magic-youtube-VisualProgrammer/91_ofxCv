/************************************************************
■ofxCvを使ってwebカメラの映像をぼかしたりグレーにしたり輪郭取ったり
	https://blanktar.jp/blog/2016/05/ofxcv-blur-grayscale-and-edge

■cv2.Canny(): Canny法によるエッジ検出の調整をいい感じにする
	https://qiita.com/Takarasawa_/items/1556bf8e0513dca34a19
	
■Canny Edge Detector
	https://docs.opencv.org/3.4/da/d5c/tutorial_canny_detector.html
	
	contents
		Hysteresis: The final step. Canny does use two thresholds (upper and lower):
			- If a pixel gradient is higher than the upper threshold, the pixel is accepted as an edge
			- If a pixel gradient value is below the lower threshold, then it is rejected.
			- If the pixel gradient is between the two thresholds, then it will be accepted only if it is connected to a pixel that is above the upper threshold.
			Canny recommended a upper:lower ratio between 2:1 and 3:1.
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include <ofxGui.h>
#include "ofxCv.h"

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/
class ofApp : public ofBaseApp{
private:
	/****************************************
	****************************************/
	enum{
		WINDOW_WIDTH = 640,
		WINDOW_HEIGHT = 480,
	};
	enum{
		CAM_WIDTH	= 320,
		CAM_HEIGHT	= 240,
	};
	
	
	/****************************************
	****************************************/
	/********************
	********************/
	int FrameRate = 30;
	int now = 0;
	ofTrueTypeFont font;

	/********************
	********************/
	ofVideoGrabber cam;
	int Cam_id;
	
	/********************
	********************/
	ofImage blur;
	ofImage gray;
	ofImage edge;
	
	/********************
	********************/
	ofxPanel gui;
	ofxFloatSlider gui_blur;
	ofxFloatSlider gui_Canny_th1;
	ofxFloatSlider gui_Canny_th2;
	
	
	/****************************************
	****************************************/
	void setup_Camera();
	
public:
	ofApp(int _Cam_id);
	~ofApp();
	
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
};
