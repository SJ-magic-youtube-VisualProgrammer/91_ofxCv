/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include <stdio.h>

#include <ofMain.h>
#include <ofxGui.h>

/************************************************************
************************************************************/
#define ERROR_MSG(); printf("Error in %s:%d\n", __FILE__, __LINE__);

/************************************************************
************************************************************/

enum{
	WINDOW_WIDTH	= 1280,
	WINDOW_HEIGHT	= 720,
};




/************************************************************
************************************************************/

/**************************************************
**************************************************/
class GUI_GLOBAL{
private:
	/****************************************
	****************************************/
	
public:
	/****************************************
	****************************************/
	void setup(string GuiName, string FileName = "gui.xml", float x = 10, float y = 10);
	
	ofxGuiGroup Group_Particle;
		ofxToggle b_DispParticle;
		ofxFloatSlider ParticleFriction;
		ofxFloatSlider ParticleForceScale;
		ofxFloatSlider Particle_Texture_id;
		ofxFloatSlider Particle_Size_min;
		ofxFloatSlider Particle_Size_max;
		ofxColorSlider Particle_col;
		ofxFloatSlider Particle_Alpha_min;
		ofxFloatSlider Particle_Alpha_max;
		ofxFloatSlider Particle_SpeedThresh;
		ofxToggle b_Particle_Fade;
		ofxToggle b_Particle_UseMouse;
		ofxToggle b_ParticleAttraction;
		
	ofxGuiGroup Group_WebCam;
		ofxFloatSlider WebCam_Alpha;
		ofxToggle b_WebCam_Gray;
		
	ofxGuiGroup Group_Img;
		ofxFloatSlider img_Alpha;
	
	ofxGuiGroup Group_Cv;
		ofxToggle b_DispFlow;
		ofxColorSlider Flow_col;
		ofxFloatSlider fbPyrScale;
		ofxFloatSlider fbLevels;
		ofxFloatSlider fbWinSize;
		ofxFloatSlider fbIterations;
		ofxFloatSlider fbPolyN;
		ofxFloatSlider fbPolySigma;
		ofxFloatSlider flowScale;

	ofxPanel gui;
	
	bool b_Disp = false;
};

/************************************************************
************************************************************/
double LPF(double LastVal, double CurrentVal, double Alpha_dt, double dt);
double LPF(double LastVal, double CurrentVal, double Alpha);
double sj_max(double a, double b);

bool checkIf_ContentsExist(char* ret, char* buf);
void Align_StringOfData(string& s);
void print_separatoin();

double get_Random();
double get_SignedRandom();

/************************************************************
************************************************************/
extern GUI_GLOBAL* Gui_Global;

extern FILE* fp_Log;

extern int GPIO_0;
extern int GPIO_1;


/************************************************************
************************************************************/

