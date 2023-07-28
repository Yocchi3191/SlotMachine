void setup() {
  Serial.begin(9600);

  // ピンをセット
  pinMode(3, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
}

void loop() {
  //左のボタン
  int switch3 = digitalRead(3);
  if (switch3 == LOW) {
    Serial.write(1);
    delay(10);  // データ送信後に遅延を追加
  }

  //中央のボタン
  int switch6 = digitalRead(6);
  if (switch6 == LOW) {
    Serial.write(2);
    delay(10);  // データ送信後に遅延を追加
  }

  //右のボタン
  int switch9 = digitalRead(9);
  if (switch9 == LOW) {
    Serial.write(3);
    delay(10);  // データ送信後に遅延を追加
  }

  //リセットボタン
  int switch11 = digitalRead(11);
  if (switch11 == LOW) {
    Serial.write(0);
    delay(10);  // データ送信後に遅延を追加
  }
}