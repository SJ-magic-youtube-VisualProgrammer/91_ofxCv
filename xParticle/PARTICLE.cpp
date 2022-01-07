/************************************************************
************************************************************/
#include "PARTICLE.h"
#include <time.h>

using namespace ofxCv;
using namespace cv;

/************************************************************
PARTICLE methods
************************************************************/

/******************************
��SJ-magic-study/study__rand
	https://github.com/SJ-magic-study/study__rand
******************************/
PARTICLE::PARTICLE(){
	/********************
	********************/
	resetPos( ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ofGetHeight()/2 );
	resetVel(ofVec2f(0.0f, 0.0f));
	force.set(0, 0);
	
	init_Size_ColAlpha();
}

/******************************
******************************/
void PARTICLE::resetPos(ofVec2f center, float max_r){
	const float _PI = 3.141565;
	
	float radius = get_Random() * max_r;
	float theta = get_Random() * 2 * PI;
	
	position.x = center.x + radius * cos(theta);
	position.y = center.y + radius * sin(theta);
	
	/*
	position.x = get_Random() * ofGetWidth();
	position.y = get_Random() * ofGetHeight();
	*/
	
	throughOfWalls(); // ��ʂ���͂ݏo��ꍇ������̂ŁA��ʓ��ɖ߂��B
}

/******************************
******************************/
void PARTICLE::resetVel(const ofVec2f vec){
	velocity = vec;
}

/******************************
******************************/
void PARTICLE::init_Size_ColAlpha(){
	Size.x = ofMap( get_Random(), 0, 1, Gui_Global->Particle_Size_min, Gui_Global->Particle_Size_max );
	Size.y = Size.x;
	
	col_a = ofMap( get_Random(), 0, 1, Gui_Global->Particle_Alpha_min, Gui_Global->Particle_Alpha_max );
}

/******************************
******************************/
void PARTICLE::resetForce(){
	force.set(0, 0);
}

/******************************
******************************/
void PARTICLE::updateVel(int dt){
	velocity += force * dt;
	velocity = velocity * Gui_Global->ParticleFriction;
}

/******************************
******************************/
void PARTICLE::updatePos(int dt){
	position += velocity * dt;
}

/******************************
******************************/
void PARTICLE::update(int dt){
	updateVel(dt);
	updatePos(dt);
	
	throughOfWalls();
}


/******************************
******************************/
void PARTICLE::throughOfWalls(){
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth() - 1;
	float maxy = ofGetHeight() - 1;
	while (position.x < minx) {
		position.x = maxx + position.x;
	}
	while (position.y < miny) {
		position.y = maxy + position.y;
	}
	while (maxx < position.x) {
		position.x = position.x - maxx;
	}
	while (maxy < position.y) {
		position.y = position.y - maxy;
	}
}

/******************************
description
	�����t��������
	
param
	scale
		���S����̋����ɂ����Force�����܂邪�A����ɑ΂���scaling.
		���Ԃ͖��֌W�ł���_�ɒ��ӁB
******************************/
void PARTICLE::addAttractionForce(float x, float y, float radius)
{
	/********************
	********************/
	ofVec2f posOfForce;
	posOfForce.set(x,y);
	
	ofVec2f diff = position - posOfForce;
	float length = diff.length();

	bool b_OutOfRange = false;
	if (radius > 0){
		if (radius < length){
			b_OutOfRange = true;
		}
	}

	if (!b_OutOfRange){
		// float scale = 0.0001 + 0.0001 * cos(2 * 3.14 * ofGetElapsedTimef() / 30.0);
		
		// ��������_�ɂ�����̓x�N�g�����v�Z
		float pct = 1 - (length / radius);
		diff.normalize();
		force.x += - diff.x  * pct * Gui_Global->ParticleForceScale;
		force.y += - diff.y  * pct * Gui_Global->ParticleForceScale;
	}
}

/******************************
******************************/
void PARTICLE::addForce(const ofVec2f& _force)
{
    force += _force;
}

/************************************************************
PARTICLE_SET methods
************************************************************/
/******************************
******************************/
PARTICLE_SET::PARTICLE_SET()
: atraction(false)
{
	/********************
	��)	�ӂ��āAPARTICLE()����srand()��u���Ȃ�����
		PARTICLE�̐����ŁA�s�x srand()������A�����̌��ʂ��S�ē���(�d�Ȃ�)�ɂȂ��Ă��܂��B
	********************/
	srand((unsigned int)time(NULL));
	
}

/******************************
******************************/
PARTICLE_SET::~PARTICLE_SET()
{
}

/******************************
******************************/
void PARTICLE_SET::setup()
{
	/********************
	��C++ std::vector resize��reserve�̈Ⴂ
		https://suzulang.com/c-stdvector-resize%E3%81%A8reserve%E3%81%AE%E9%81%95%E3%81%84-%E5%8A%A0%E7%AD%86%E4%B8%AD%EF%BC%9F/
	
	��std::vector��resize��assign�̈Ⴂ (C++)
		https://minus9d.hatenablog.com/entry/2021/02/07/175159
	********************/
	clouds.resize(NUM_CLOUDS);
	
	/********************
	********************/
	for(int i = 0; i < NUM_PARTICLE_TEXTURES; i++){
		char buf[512];
		sprintf(buf, "texture/particleTexture_%d.png", i);
		TextureImg[i].load(buf);
	}
	
	VboSet.setup(NUM_VBOS, true);
	Refresh_vboVerts();
	Refresh_vboColor();
	VboSet.update_vertex_color();
	
	Refresh_vboTexCoords();
	VboSet.update_TexCoords();
	
	
	t_ofs__SingedNoise_for_CenterOfForce.x = get_Random() * 1000.0;
	t_ofs__SingedNoise_for_CenterOfForce.y = get_Random() * 1000.0;
}

/******************************
1--2
|  |
0--3
******************************/
void PARTICLE_SET::Refresh_vboVerts()
{
	for(int i = 0; i < NUM_CLOUDS; i++){
		VboSet.set_vboVerts(i * 4 + 0, clouds[i].getPos_LeftLow().x, clouds[i].getPos_LeftLow().y );
		VboSet.set_vboVerts(i * 4 + 1, clouds[i].getPos_LeftUp().x, clouds[i].getPos_LeftUp().y );
		VboSet.set_vboVerts(i * 4 + 2, clouds[i].getPos_RightUp().x, clouds[i].getPos_RightUp().y );
		VboSet.set_vboVerts(i * 4 + 3, clouds[i].getPos_RightLow().x, clouds[i].getPos_RightLow().y );
	}
}

/******************************
color��Speed down�Ƌ��Ƀ����������Ă����悤�ɂ����B
******************************/
void PARTICLE_SET::Refresh_vboColor()
{
	if(!Gui_Global->b_Particle_Fade){
		for(int i = 0; i < NUM_CLOUDS; i++){
			ofColor col = Gui_Global->Particle_col;
			col.a *= clouds[i].get_ColAlpha();
			
			VboSet.set_vboColor(i * 4 + 0, col);
			VboSet.set_vboColor(i * 4 + 1, col);
			VboSet.set_vboColor(i * 4 + 2, col);
			VboSet.set_vboColor(i * 4 + 3, col);
		}
		
	}else{
		for (int i = 0; i < clouds.size(); i++) {
			ofColor col = Gui_Global->Particle_col;
			col.a *= clouds[i].get_ColAlpha();
			
			const double Alpha_max = double(col.a) / 255;
			double tan = Alpha_max / Gui_Global->Particle_SpeedThresh;
			
			float Alpha;
			if(Gui_Global->Particle_SpeedThresh < clouds[i].get_Speed())	Alpha = Alpha_max;
			else															Alpha = tan * clouds[i].get_Speed();
			
			col.a = 255.0 * Alpha;
			
			VboSet.set_vboColor(i * 4 + 0, col);
			VboSet.set_vboColor(i * 4 + 1, col);
			VboSet.set_vboColor(i * 4 + 2, col);
			VboSet.set_vboColor(i * 4 + 3, col);
		}
	}
}

/******************************
******************************/
void PARTICLE_SET::Refresh_vboTexCoords()
{
	for(int i = 0; i < NUM_CLOUDS; i++){
		VboSet.set_TexCoords(i * 4 + 0, 0, TextureImg[0].getHeight());
		VboSet.set_TexCoords(i * 4 + 1, 0, 0);
		VboSet.set_TexCoords(i * 4 + 2, TextureImg[0].getWidth(), 0);
		VboSet.set_TexCoords(i * 4 + 3, TextureImg[0].getWidth(), TextureImg[0].getHeight());
	}
}

/******************************
******************************/
void PARTICLE_SET::reset()
{
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].resetPos(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ofGetWidth()/10);
		// clouds[i].resetVel(ofVec2f(0.0f, 0.0f));
	}
}

/******************************
******************************/
void PARTICLE_SET::SuperReset()
{
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].resetPos(ofVec2f(ofGetWidth()/2, ofGetHeight()/2), ofGetWidth()/20);
		clouds[i].resetVel(ofVec2f(0.0f, 0.0f));

	}
}

/******************************
******************************/
void PARTICLE_SET::make_VerticalFlow()
{
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].resetPos(ofVec2f(ofGetWidth()/2, 0), ofGetWidth()/25);
		clouds[i].resetVel(ofVec2f(0.0f, get_Random() * 1.0f));
	}
}

/******************************
******************************/
void PARTICLE_SET::make_HorizontalFlow()
{
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].resetPos(ofVec2f(0, ofGetHeight()/2), ofGetWidth()/25);
		clouds[i].resetVel(ofVec2f(get_Random() * 1.0f, 0.0f));
	}
}

/******************************
******************************/
void PARTICLE_SET::resetForce()
{
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].resetForce();
	}
}

/******************************
******************************/
void PARTICLE_SET::addForce_by_Flow(const ofVec2f& CamSize, ofxCv::FlowFarneback& fb)
{
	for (int i = 0; i < clouds.size(); i++){
		/********************
		particle���u�ԓI�ɉ�ʊO�ɂ���\��������B
			fb.getFlowOffset(pos.x, pos.y);
		�ŉ�ʊO��position�������œn���Ă��܂��ƁA�s��l���Ԃ��Ă��Ă��܂�(����������ʊO�̒l��n���Ă�������������)�̂ŁA������� if()�œ����Ă����B
		********************/
		if	(
				(0 <= clouds[i].get_pos().x) && (clouds[i].get_pos().x < ofGetWidth()) &&
				(0 <= clouds[i].get_pos().y) && (clouds[i].get_pos().y < ofGetHeight())
			)
		{
			ofVec2f force;
			ofVec2f pos;
			pos.x = clouds[i].get_pos().x * (float)(CamSize.x) / (float)(ofGetWidth());
			pos.y = clouds[i].get_pos().y * (float)(CamSize.y) / (float)(ofGetHeight());
			force = ofVec2f(fb.getFlowOffset(pos.x, pos.y).x, fb.getFlowOffset(pos.x, pos.y).y) * Gui_Global->flowScale;
			clouds[i].addForce(force);
		}
	}
}

/******************************
******************************/
void PARTICLE_SET::addForce_by_mouse(int now)
{
	/********************
	********************/
	{
		const float scale = 1.0;
		CenterOfForce.x = ofGetWidth()/2  + ofSignedNoise( double(now)/1000 / T_ForcePos + t_ofs__SingedNoise_for_CenterOfForce.x) * (ofGetWidth() * scale);
		CenterOfForce.y = ofGetHeight()/2 + ofSignedNoise( double(now)/1000 / T_ForcePos + t_ofs__SingedNoise_for_CenterOfForce.y) * (ofGetHeight() * scale);
	}
	
	/********************
	update the position of all particle.
	********************/
	for (int i = 0; i < clouds.size(); i++){
		if (Gui_Global->b_ParticleAttraction || atraction){
			if(Gui_Global->b_Particle_UseMouse)	clouds[i].addAttractionForce(ofGetAppPtr()->mouseX, ofGetAppPtr()->mouseY, ofGetWidth());
			else								clouds[i].addAttractionForce(CenterOfForce.x, CenterOfForce.y, ofGetWidth());
		}
	}
}

/******************************
��How to use mouseX in a class?
	https://forum.openframeworks.cc/t/how-to-use-mousex-in-a-class/3281
******************************/
void PARTICLE_SET::update(int now)
{
	/********************
	********************/
	// static int LastINT = ofGetElapsedTimeMillis();
	static int LastINT = now;
	// int now = ofGetElapsedTimeMillis();
	int dt = ofClamp(now - LastINT, 0, 100);
	
	LastINT = now;
	
	/********************
	update the position of all particle.
	********************/
	for (int i = 0; i < clouds.size(); i++){
		clouds[i].update(dt);
	}
	
	/********************
	********************/
	Refresh_vboVerts();
	VboSet.update_vertex();
	
	Refresh_vboColor();
	VboSet.update_color();
}

/******************************
******************************/
void PARTICLE_SET::draw()
{
	/********************
	********************/
	ofEnableAlphaBlending();
	// ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	// ofEnableSmoothing();
	
	/********************
	********************/
	ofSetColor(255, 255, 255, 255);
	TextureImg[int(std::floor(Gui_Global->Particle_Texture_id))].bind();
	VboSet.draw(GL_QUADS);	
	TextureImg[int(std::floor(Gui_Global->Particle_Texture_id))].unbind();
	/*
	glPointSize(Gui_Global->Particle_PointSize);
	glEnable(GL_POINT_SMOOTH);
	VboSet.draw(GL_POINTS);
	*/
	
	/********************
	********************/
	// �d�͂̓_��`��
	ofFill();
	if (atraction)	ofSetColor(255, 0, 0);
	else			ofSetColor(0, 255, 255);
	
	if(Gui_Global->b_Particle_UseMouse)	ofDrawCircle(ofGetAppPtr()->mouseX, ofGetAppPtr()->mouseY, 4);
	else								ofDrawCircle(CenterOfForce.x, CenterOfForce.y, 4);
}

/******************************
******************************/
void PARTICLE_SET::set_attractive(bool b_val)
{
	atraction = b_val;
}

