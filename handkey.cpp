#include <Keyboard.h>
#include <Adafruit_MCP23017.h>
#include <Wire.h>
#include "handkey.h"

Adafruit_MCP23017 mcp;

const char* S[] = {"","a", "i", "u", "e", "o"};
const char* T[] = {"","","k", "s", "t","n","h","m","y","r","w","g","z","d","","b","p"};

double timeout = 0.7; //自動入力されるまでの待ち時間[秒]
int pin_a = 1; //ア行のpin番号
int pin_k = 2; //カ
int pin_s = 3; //サ
int pin_T = 5; //タ
int pin_n = 6; //ナ
int pin_h = 7; //ハ
int pin_m = 8; //マ
int pin_y = 9; //ヤ
int pin_r = 10; //ラ
int pin_w = 12; //ワ
int pin_ast = 0; //＊
int pin_shr = 11; //♯
int pin_space = 13;
int value(int i){
  switch(i){
    case 1: return mcp.digitalRead(pin_a);
    case 2: return mcp.digitalRead(pin_k);
    case 3: return mcp.digitalRead(pin_s);
    case 4: return mcp.digitalRead(pin_T);
    case 5: return mcp.digitalRead(pin_n);
    case 6: return mcp.digitalRead(pin_h);
    case 7: return mcp.digitalRead(pin_m);
    case 8: return mcp.digitalRead(pin_y);
    case 9: return mcp.digitalRead(pin_r);
    case 10: return mcp.digitalRead(pin_w);
    case 12: return mcp.digitalRead(pin_ast);
    case 11: return mcp.digitalRead(pin_shr);
    case 13: return mcp.digitalRead(pin_space);
  }
}


int time = 0;
int cnt = 0;
int cnt2 = 0;
int cnt3 = 0;
int vowel = 0;
int consonant = 0;
int astarisk = 0;
int mode = 0;

void keyboardSetup(){
  mcp.begin();
  //mcp.pinMode(7,OUTPUT);
  mcp.pinMode(pin_a, INPUT); 
  mcp.pullUp(pin_a, HIGH);
  mcp.pinMode(pin_k, INPUT); 
  mcp.pullUp(pin_k, HIGH);
  //pinMode(pin_a, INPUT_PULLUP); 
  mcp.pinMode(pin_s, INPUT); 
  mcp.pullUp(pin_s, HIGH);
  mcp.pinMode(pin_T, INPUT); 
  mcp.pullUp(pin_T, HIGH);
  mcp.pinMode(pin_n, INPUT); 
  mcp.pullUp(pin_n, HIGH);
  mcp.pinMode(pin_h, INPUT); 
  mcp.pullUp(pin_h, HIGH);
  mcp.pinMode(pin_m, INPUT); 
  mcp.pullUp(pin_m, HIGH);
  mcp.pinMode(pin_y, INPUT); 
  mcp.pullUp(pin_y, HIGH);
  mcp.pinMode(pin_r, INPUT); 
  mcp.pullUp(pin_r, HIGH);
  mcp.pinMode(pin_w, INPUT); 
  mcp.pullUp(pin_w, HIGH);
  mcp.pinMode(pin_ast, INPUT); 
  mcp.pullUp(pin_ast, HIGH);
  mcp.pinMode(pin_shr, INPUT); 
  mcp.pullUp(pin_shr, HIGH);
  mcp.pinMode(pin_space, INPUT); 
  mcp.pullUp(pin_space, HIGH);
  
  //Serial.begin(9600);
  Keyboard.begin();
  delay(5000); // 起動直後に5秒待機
}

void typeJapanese(const String &sequence) {
  for (int i = 0; i < sequence.length(); i++) {
    Keyboard.print(sequence[i]);
    delayMicroseconds(100); 
  }
  delayMicroseconds(100);
  /*
  Keyboard.press(KEY_RETURN); 
  delay(1);
  Keyboard.release(KEY_RETURN);
  delay(1);
  */
}

void key_delete(){
  Keyboard.press(KEY_BACKSPACE); 
  delay(1);
  Keyboard.release(KEY_BACKSPACE);
  delay(1);
}

void key_space(){
  Keyboard.press(' '); 
  delay(1);
  Keyboard.releaseAll();
  delay(1);
}

void key_return(){
  Keyboard.press(KEY_RETURN); 
  delay(1);
  Keyboard.release(KEY_RETURN);
  delay(1);
}

void key_hai(){
  //Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.print("-"); 
  delay(1);
  Keyboard.releaseAll();
  delay(1);
}

//k = 0　通常
//k = 1 濁点
//k = 2 半濁点
//k = 3 小文字
void print_hiragana(int i,int j,int k){
  //Keyboard.print(j);
  String hiragana = String(T[i]) + String(S[j]);
  if(i==10){
    if(j==1) hiragana = "wa";
    else if(j==3) hiragana = "wo";
    else if(j==5) hiragana = "nn";
    else if(j==7) {
      key_hai();
      //key_hai();
      return;
    }
    else hiragana = "";
  }
  else{
    if(k==1){
      hiragana = String(T[i+9]) + String(S[j]);
    }
    if(k==2){
      hiragana = String(T[16]) + String(S[j]);
    }
    if(k==3){
      hiragana = "x" + String(T[i]) + String(S[j]);
    }
  }
  typeJapanese(hiragana);
}



void keyboardControl(){
  //各pinにア行～ワ行、＊、♯を割り振っています。
  //1～10→ア行～ワ行、11→＊、12→♯

  int pinStatus = 0;
  for(int i=0;i<13;i++){
    pinStatus|=(((value(i+1))==LOW)<<i);
  }
//いずれかのpinがHIGHになっていれば、cntが1~100でループします。すべてがLOWの時はcntは0のままです。
  if(pinStatus != 0){
    if(cnt == 100) cnt = 1;
    else cnt += 1;
  }else{
    cnt = 0;
  }

  //タイムアウト
  //いずれかのpinがHIGHのときはtime=1、すべてのpinがLOWのとき、-300～-1まで順に動きます
  if(pinStatus != 0 && time != -1){
    time = 1;
  }else if(pinStatus != 0 && time == -1){
    time = 0;
  }else if(pinStatus == 0 && time == 1){
    time = -100*timeout; //ここの時間を変更できる
  }else if(pinStatus == 0 && time == -1){
    time = -1;
  }else{
    time += 1;
  }

    //入力が0.01秒で出力　ア→オ→ア
  if(pinStatus != 0 && cnt == 1 && ((pinStatus>>(11))&1)==0){
    //初期値のときはvowel=1にする。
    if(vowel == -1) vowel = 1;
    //ヤ行、ワ行、＊はループが3つ。
    else if(vowel != 5 && (consonant == 8)) vowel += 2;
    else if(vowel != 7 && (consonant == 10)) vowel += 2;
    //基本的にはループは5つ
    else if(consonant == 10 && vowel == 7) vowel == 1;
    else if(consonant != 10 && vowel == 5) vowel = 1;
    else vowel += 1;

    for(int i=1;i<=10;i++){
      if((pinStatus>>(i-1))&1){
        if(i == consonant){
          if(mode == 1 && astarisk == 0){
            key_delete();
          }else if(astarisk != 0) vowel = 1;
          astarisk = 0;
          print_hiragana(consonant,vowel,astarisk);
          mode = 1;
        }
      }
    }
    
  }
if(((pinStatus>>(11))&1) == 1 && cnt ==1 && (vowel != 0 && consonant != 0)){
  if(consonant == 2 || consonant == 3 || (consonant == 4 && vowel != 3)){
    if(astarisk == 1) astarisk = 0;
    else astarisk += 1;
  }else if(consonant == 6){
    if(astarisk == 2) astarisk = 0;
    else astarisk += 1;
  }else if(consonant == 1 || consonant == 8){
    if(astarisk == 3) astarisk = 0;
    else astarisk = 3;
  }else if(consonant == 4 && vowel == 3){
    if(astarisk == 3) astarisk = 0;
    else if(astarisk == 1) astarisk = 3;
    else astarisk += 1;
  }
  if(mode == 1){
    key_delete();
  }
  print_hiragana(consonant,vowel,astarisk);
  mode = 1;
}


//ある入力が待機になっているときに他のキー(＊、♯以外)を押すと待機文字が入力される。
  for(int i=1; i<=10; i++){
    if(((pinStatus>>(i-1)&1)) && consonant != i && consonant != 12){
      if(time != 0 && consonant != 0){
        if(vowel == 1){
          if(consonant == 10) vowel = 7;
          else vowel = 5;
        }else{
          if(consonant == 8 || consonant == 10) vowel -= 2;
          else vowel += -1;
        }
        mode = 0;
      }
      vowel = 1;
      consonant = i;
      astarisk = 0;
      print_hiragana(consonant,vowel,astarisk);
      mode = 1;
    }
  }



  //delete。53番がHIGHになると現在のvowel、consonantをリセットします。
  if(((pinStatus>>(pin_shr-1))&1)==0){
    cnt2 = 0;
  }else if(cnt2 == 2){
    cnt2 = 2;
  }else{
    cnt2 += 1;
  }
  //deleteボタンを押すと、「delete」と出力されます。(消す予定)
  if(cnt2 == 1){
    key_delete();
    //key_space();
    time = 0;
    cnt = 0;
    vowel = -1;
    consonant = 0;
  }

  if(((pinStatus>>(pin_space-1))&1)==0){
    cnt3 = 0;
  }else if(cnt2 == 2){
    cnt3 = 2;
  }else{
    cnt3 += 1;
  }
  //deleteボタンを押すと、「delete」と出力されます。(消す予定)
  if(cnt3 == 1){
    //key_delete();
    key_space();
    time = 0;
    cnt = 0;
    vowel = -1;
    consonant = 0;
  }


  //時間で出力
  if(time == -2 && consonant != 0){
    mode = 0;
    vowel = -1;
  }
}