#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
	//16 : 9 �ŉ�ʃT�C�Y������
	int size = 80;
	int asptHor = 16 * size;
	int	asptVer = 9 * size;

	ofSetupOpenGL(asptHor, asptVer, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
