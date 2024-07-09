#include "yubimouse.h"
ADNS2083 Optical1 = ADNS2083(SCLK, SDIO);
//BleMouse Mouse;


void mouseSetup() {
  Optical1.begin();
  Optical1.config(0x00);
  Mouse.begin();
  Serial.begin(9600);

  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);

  delay(100);
}



void mouseScroll(double now) {
  static double sum = 0;
  sum += now;
  if (abs(sum) >= 1) {
    Mouse.move(0, 0, floor(sum));
    sum = sum - floor(sum);
  }
}

void mouseControl() {
  char x = 0, y = 0, r = 0;
  bool exist = (Optical1.mo() & (1 << 7)) > 0;
  if (exist) {
    x = Optical1.dx();
    y = -Optical1.dy();
  }
  //Serial.println((int)(x));
  //return;
  bool left = digitalRead(LEFT) == LOW, right = digitalRead(RIGHT) == LOW;
  sendInformation(x, y, left, right);
}
void sendInformation(char x, char y, bool left, bool right) {
  //クリックの安定性向上
  static long leftBegin = -1, rightBegin = -1;
  static bool leftPressing = false, rightPressing = false;
  const static char history_size = 7;
  static bool history[2][7] = { { false, false, false, false, false, false, false }, { false, false, false, false, false, false, false } };
  static int count[2] = { 0, 0 }, cursor = 0;

  static long last_click[2] = { -1, -1 };
  const long last_click_thres = 300;
  const int loop_time_estimate = 6;
  const long click_thres = 200;

  static int roll_val = 0;
  long decrease_count = 0;

  long now = millis();

  for (int i = 0; i < 2; i++) {
    bool tmp = left;
    if (i == 1) tmp = right;
    if (tmp && !history[i][cursor]) count[i]++;
    if (!tmp && history[i][cursor]) count[i]--;
    history[i][cursor] = tmp;
  }
  (cursor += 1) %= history_size;

  if (left) {
    if (leftBegin < 0) leftBegin = now;
  }
  if (~leftBegin && now - leftBegin > loop_time_estimate * history_size) {
    if (count[0] <= 2) {
      if (!leftPressing) {
        if (now - last_click[0] > last_click_thres) {
          //Serial.println("click");
          Mouse.click(MOUSE_LEFT);
          last_click[0] = now;
          leftBegin = -1;
        }
      } else {
        //Serial.println("release");
        Mouse.release(MOUSE_LEFT);
        leftPressing = false;
        leftBegin = -1;
      }
    }
    if (count[0] >= history_size - 2 && !leftPressing && now - leftBegin >= click_thres) {
      Mouse.press(MOUSE_LEFT);
      leftPressing = true;
      //Serial.println("press");
    }
  }

  if (right) {
    if (rightBegin < 0) rightBegin = now;
  }
  if (~rightBegin && now - rightBegin > loop_time_estimate * history_size) {
    if (count[1] <= 3) {
      if (!rightPressing) {
        if (now - last_click[1] > last_click_thres) {
          Mouse.click(MOUSE_RIGHT);
          last_click[1] = now;
          rightBegin = -1;
        }
      } else {
        rightPressing = false;
        rightBegin = -1;
      }
    }
    if (count[1] >= history_size - 3 && !rightPressing && now - rightBegin >= click_thres) {
      rightPressing = true;
    }
  }

  if (abs(x) > 0 || abs(y) > 0) {
    if (rightPressing) {
      mouseScroll(y / 30.);
      //Serial.println((int)now);
    } else {
      Mouse.move(x, y);
    }
  }
}