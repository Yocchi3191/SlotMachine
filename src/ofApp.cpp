#include "ofApp.h"

vector<Slot> slots;
bool allSlotStopped;

ofVec3f lightPos;
ofVec3f camPos;
int camLookAt;

int coin;
int bet;
bool noCoin;

//--------------------------------------------------------------
void ofApp::setup() {
	cout << "スロットを動かす : スペース or 一番右のボタン" << endl;
	cout << "スロットを止める: 方向キー 左, 上, 右 or ３つの丸いボタン" << endl;

	//シリアル通信を開始
	serial.setup("COM3", 9600);

	//効果音のセット
	push.load("sounds/typewriter.mp3");
	reset.load("sounds/reset.mp3");

	pair1.load("sounds/syakin.mp3"); //シャキーン
	pair2.load("sounds/kirakira.mp3"); //キラキラ
	pair2.setVolume(2);
	pair3.load("sounds/koto.mp3"); //琴
	pair4.load("sounds/kyuin3.wav"); //キュイーンキュイーン
	pair4.setVolume(3);

	bgm.load("sounds/bgm.wav"); //ティロリロ
	bgm.setLoop(true);
	bgm.play();

	//フォントのセット
	coinUi.load(OF_TTF_SANS, 30);
	playUi.load("fonts/GenEiMGothic2-Bold.ttf", 20);
	cout << playUi.load("fonts/GenEiMGothic2-Bold.ttf", 20) << endl;

	ofSetFrameRate(24);

	//カメラの初期設定
	camPos = ofVec3f(0.0, 0.0, -6.0);
	cam.setNearClip(0.5);
	cam.setFarClip(1000);
	cam.setFov(60);
	cam.setPosition(camPos);

	light.enable();  //lightを有効に  

	//テクスチャマッピングの設定
	ofDisableArbTex();

	for (int count = 0; count < 3; count++) {
		slots.push_back(Slot());
	}
	for (float i = 0; i < 3; i++) {
		float width = slots[i].height + 0.1;
		slots[i].pos.y = -width + width * i;
	}
	allSlotStopped = false;

	coin = 100;
	bet = 5;
}

//--------------------------------------------------------------
void ofApp::update() {
	//光の設定 
	light.setAmbientColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setDiffuseColor((ofFloatColor(0.6, 0.6, 0.6, 1)));
	light.setSpecularColor((ofFloatColor(0.6, 0.6, 0.6, 1)));

	cam.lookAt(slots[1].pos);

	//シリアル通信のデータを格納
	int arduinoData = serial.readByte();


	if (allSlotStopped) { //すべてのスロットが止まっていれば
		if (arduinoData == 0) {
			myCout("RESET!!");
			reset.play();
			startAllReel();
		}
	}
	else { //動いているスロットがあれば
		for (Slot& slot : slots) {
			cout << "|" << slot.curReel;
			slot.update();
		}
		//Arduinoからのデータの読み取り
		if (serial.available() > 0) {
			push.play();
			switch (arduinoData)
			{
			case(1):
				slots[0].buttonPushed = true;;
				break;
			case(2):
				slots[1].buttonPushed = true;;
				break;
			case(3):
				slots[2].buttonPushed = true;;
				break;
			default:
				break;
			}
		}

		cout << "|" << endl;
		stopCheck();

		//コインが切れたとき用の処理
		if (coin < bet && arduinoData == 0) {
			reset.play();
			myCout("RESTART!!!");
			coin = 50;
		}


		//次のループのために、残っている通信データを処理
		while (serial.available() > 0) {
			serial.readByte();
			cout << arduinoData << endl;
		}
	}


	//コイン枚数のチェック
	if (coin <= 0) {
		noCoin = true;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	cam.begin();
	ofEnableDepthTest();  //深さ判定を有効に  
	ofEnableSmoothing();  //スムージング処理を有効に 
	ofEnableLighting();  //Lightingを有効に 
	ofBackground(ofFloatColor(0));
	ofPushMatrix();

	lightPos.set(0.0, 0.0, -5.0);
	light.setPosition(lightPos);
	light.setPointLight();

	for (Slot& slot : slots) {
		slot.draw();
	}


	ofPopMatrix();
	ofDisableDepthTest();
	ofDisableSmoothing();
	ofDisableLighting();
	cam.end();

	ofSetColor(255);
	coinUi.drawString(to_string(coin) + " coins", 0, ofGetWindowHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (!allSlotStopped) {
		//スロットの回転を止める
		if (key == OF_KEY_LEFT) {
			push.play();
			slots[0].buttonPushed = true;
			myCout("PUSH!!");
		}
		if (key == OF_KEY_UP) {
			push.play();
			slots[1].buttonPushed = true;
			myCout("PUSH!!");
		}
		if (key == OF_KEY_RIGHT) {
			push.play();
			slots[2].buttonPushed = true;
			myCout("PUSH!!");
		}
	}
	//スロットを再度動かす
	if (key == ' ' && allSlotStopped) {
		reset.play();
		startAllReel();
	}

	if (coin < bet && key == OF_KEY_RETURN) {
		myCout("RESTART!!!");
		coin = 50;
		startAllReel();
		reset.play();
	}
}

//--------------------------------------------------------------
void ofApp::stopCheck() {
	if (slots[0].buttonPushed &&
		slots[1].buttonPushed &&
		slots[2].buttonPushed)
	{
		allSlotStopped = true;
		reelCheck();

		string coinText = "Remain " + to_string(coin) + " coin";
		myCout(coinText);
	}
}
void ofApp::reelCheck() {
	if (slots[0].curReel == slots[1].curReel &&
		slots[1].curReel == slots[2].curReel)
	{
		pairCheck();
	}
}
void ofApp::pairCheck() {
	//絵柄ペアごとの処理
	if (slots[0].curReel == "1") {
		//JAC
		pair1.play();
		coin += bet;
		myCout("REPLAY!!");
	}
	if (slots[0].curReel == "2") {
		//ベル
		pair1.play();
		coin += bet * 3;
		myCout("BELL!!");
	}
	if (slots[0].curReel == "3") {
		//7
		pair1.play();
		pair2.play();
		for (int i = 0;i < 3;i++) {
			pair4.play();
		}
		coin += bet * 7;
		myCout("LUCKY SEVEN!!1");
	}
	if (slots[0].curReel == "4") {
		//スイカ
		pair1.play();
		coin += bet * 6;
		myCout("Suica!!");
	}
	if (slots[0].curReel == "5") {
		//MYU
		pair1.play();
		pair3.play();
		for (int i = 0;i < 3;i++) {
			pair4.play();
		}
		coin += bet * 10;
		myCout("!!!! MYU !!!!");
	}
	if (slots[0].curReel == "6") {
		//チェリー
		pair1.play();
		coin += bet;
		myCout("Cherry!!");
	}
}
void ofApp::startAllReel() {
	if (coin >= bet) {
		for (Slot& slot : slots) {
			slot.buttonPushed = false;
		}
		coin -= bet;
		allSlotStopped = false;
		myCout("RESET!!");
	}
	else {
		cout << "Oh!! You has No Coin!" << endl;
		cout << "press ENTER to RESTART" << endl;
	}
}
void ofApp::myCout(string text) {
	cout << endl;
	cout << text << endl;
	cout << endl;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
