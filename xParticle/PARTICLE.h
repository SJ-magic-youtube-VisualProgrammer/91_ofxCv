/************************************************************
************************************************************/
#pragma once

/************************************************************
************************************************************/
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxCv.h"

#include "ofxVboSet.h"

#include "sj_common.h"


/************************************************************
class
************************************************************/

/**************************************************
description
	äePARTICLE(1ó±)ÇÃ
	- position
	- velocity
	- îÌforce
	Çä«óù.
	
	ï`âÊã@î\ÇÕï€éùÇµÇ»Ç¢.
	
unit	
	ms
	pixel
**************************************************/
class PARTICLE {
private:
	/****************************************
	****************************************/
	/********************
	********************/
	ofVec2f position;
	ofVec2f velocity;
	ofVec2f force;
	ofVec2f Size;
	float col_a;
	
	/****************************************
	****************************************/
	void updateVel(int dt);
	void updatePos(int dt);
	void throughOfWalls();
	void init_Size_ColAlpha();
	
public:
	/****************************************
	****************************************/
	PARTICLE();
	
	void resetPos(ofVec2f center, float max_r);
	void resetVel(const ofVec2f vec);
	void resetForce();
	void update(int dt);
	void addAttractionForce(float x, float y, float radius);
	void addForce(const ofVec2f& _force);
	
	float get_Speed()	{ return velocity.length(); }
	const ofVec2f& get_pos()	{ return position; }
	double get_ColAlpha()	{ return col_a; }
	
	ofVec2f getPos_LeftLow(){
		ofVec2f point;
		point.x = position.x - Size.x/2;
		point.y = position.y + Size.y/2;
		return point;
	}
	
	ofVec2f getPos_LeftUp(){
		ofVec2f point;
		point.x = position.x - Size.x/2;
		point.y = position.y - Size.y/2;
		return point;
	}
	
	ofVec2f getPos_RightUp(){
		ofVec2f point;
		point.x = position.x + Size.x/2;
		point.y = position.y - Size.y/2;
		return point;
	}
	
	ofVec2f getPos_RightLow(){
		ofVec2f point;
		point.x = position.x + Size.x/2;
		point.y = position.y + Size.y/2;
		return point;
	}
};

/**************************************************
**************************************************/
class PARTICLE_SET {
public:
	enum{
		NUM_PARTICLE_TEXTURES = 6,
	};

private:
	/********************
	********************/
	enum{
		// NUM_PARTICLES = 100000,
		// NUM_PARTICLES = 60000,
		// NUM_PARTICLES = 30000,
		// NUM_PARTICLES = 20000,
		
		// NUM_CLOUDS = 4000,
		NUM_CLOUDS = 5000,
		NUM_VBOS = NUM_CLOUDS * 4,
	};
	
	/********************
	********************/
	vector<PARTICLE> clouds;
	ofx__VBO_SET VboSet;
	ofImage TextureImg[NUM_PARTICLE_TEXTURES];
	
	bool atraction;
	
	ofVec2f CenterOfForce = ofVec2f(0, 0);
	ofVec2f t_ofs__SingedNoise_for_CenterOfForce = ofVec2f(0, 0);
	const float T_ForcePos = 8.0f;
	
	/********************
	********************/
	// void clear_VboSetting_gl();
	void Refresh_vboVerts();
	void Refresh_vboColor();
	void Refresh_vboTexCoords();
	

public:
	PARTICLE_SET();
	~PARTICLE_SET();
	
	void setup();
	void update(int now);
	void draw();
	
	void reset();
	void SuperReset();
	void make_VerticalFlow();
	void make_HorizontalFlow();
	void resetForce();
	void addForce_by_Flow(const ofVec2f& CamSize, ofxCv::FlowFarneback& fb);
	void addForce_by_mouse(int now);
	
	void set_attractive(bool b_val);
};

