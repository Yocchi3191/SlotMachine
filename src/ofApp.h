#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);

	void gotMessage(ofMessage msg);

	//シリアル通信インスタンスの作成
	ofSerial serial;

	//カメラを2台設定 
	ofCamera cam;
	ofLight light;

	ofSoundPlayer push, reset;
	ofSoundPlayer pair1, pair2, pair3, pair4;
	ofSoundPlayer bgm;
	ofTrueTypeFont coinUi, playUi;

	void stopCheck();
	void reelCheck();
	void pairCheck();
	void startAllReel();

	void myCout(string text);

};

class Slot {
public:
	vector<string> reel; //リール
	string curReel; //表示するリール
	int reelNum; //リールの何番目を表示するか
	int reelLength; //リールの配列長
	bool buttonPushed; //ボタン

	ofVec3f pos;
	float radius;
	float height;
	ofVec3f rot;
	ofTexture tex;

	ofImage imgSlot;
	ofTexture texSlot;

	Slot() {
		reel = { "1", "2", "3", "4", "5", "6" };
		reelLength = reel.size() - 1;
		reelNum = ofRandom(0, reelLength);
		curReel = reel[reelNum];
		buttonPushed = true;

		pos = ofVec3f(0, 0, 0);
		radius = 2.5f;
		height = 2.5f;
		rot = ofVec3f(1, 0, 90);

		imgSlot.loadImage("images/slot.png");
		texSlot.loadData(imgSlot);
	}


	void update() {
		if (!buttonPushed) {
			reelUpdate();
		}
	}
	void reelUpdate() {
		curReel = reel[reelNum];
		if (reelNum < reelLength) {
			reelNum++;
		}
		else {
			reelNum = 0;
		}
	}

	void draw() {
		ofPushMatrix();
		ofSetCylinderResolution(reel.size(), 8);
		ofTranslate(0, 0, 0);
		ofRotateDeg(rot.z, 0, 0, 1);
		//ボタンが押されていなければリールを回す
		if (!buttonPushed) {
			rot.y = ofMap(reelNum, 0, reelLength + 1, 0, 360);
		}
		float fixedRot = rot.y + (360 / 6) / 2 + 90;
		ofRotateDeg(fixedRot, 0, 1, 0);
		ofTranslate(pos);
		texSlot.bind();
		ofDrawCylinder(radius, height);
		texSlot.unbind();
		ofPopMatrix();
	}
};