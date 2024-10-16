#include <Mouse.h>
#include <Keyboard.h>

/*
  Mouse de botoes com funcionalidade: movimento do cursor, clique, clique duplo, clique preso, clique direito, scroll bidirecional e ajuste de velocidade (cursor e scroll)
  Para placas de arduino micro, Leonardo, Pro Micro and Due boards only.
  O movimento do mouse é sempre relativo. 
  
  Código de exemplo criado originalmente por Tom Igoe e modificado por bolsistas do projeto CRTA.
  
*/

#define DEBOUNCING 50
#define TEMPO 200
#define MULTIPLI 0.08
#define MULTIPLICA 1023
#define SCROLL_SPEED_LOW 2
#define SCROLL_SPEED_MEDIUM 5
#define SCROLL_SPEED_HIGH 10

// Pinos dos botões
const int upButton = 2;
const int downButton = 3;
const int leftButton = 5;
const int rightButton = 4;
const int mouseButton = 6;
const int rightmouseButton = 7;
const int leftLong = 8;
const int doubleleft = 9;
const int ScrollButton = 10;
const int velocidade = 11;

int range = 1;
int responseDelay = 10;
boolean variavel = 0, variavel2 = 1, variavel3 = 1, variavel4 = 1, variavel5 = 1, variavel6 = 1;
int d = 0;
unsigned long espaco = 0, a = 0;
float t = 0.1;
boolean scrollMode = false;
int scrollSpeed = SCROLL_SPEED_MEDIUM;
int scrollSpeedLevel = 1; // 0 = baixo, 1 = médio, 2 = alto

void setup() {
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(mouseButton, INPUT_PULLUP);
  pinMode(leftLong, INPUT_PULLUP);
  pinMode(doubleleft, INPUT_PULLUP);
  pinMode(rightmouseButton, INPUT_PULLUP);
  pinMode(ScrollButton, INPUT_PULLUP);
  pinMode(velocidade, INPUT_PULLUP);
  
  Mouse.begin();
  Keyboard.begin();
  Serial.begin(9600);
}

void loop() {
  int xDistance = 0;
  int yDistance = 0;

  boolean upState = !digitalRead(upButton);
  boolean downState = !digitalRead(downButton);
  boolean rightState = !digitalRead(rightButton);
  boolean leftState = !digitalRead(leftButton);

  // Controle do modo de scroll
  if (digitalRead(ScrollButton) == LOW) {
    if (variavel6 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(ScrollButton) == LOW) {
        scrollMode = !scrollMode; // Alterna entre modo normal e modo de scroll
        variavel6 = 0;
      }
    }
  } else {
    variavel6 = 1;
  }

  // Controle da velocidade de scroll e movimento
  if ((digitalRead(velocidade) == LOW) && (espaco + 1000) < millis()) {
    espaco = millis();
    scrollSpeedLevel = (scrollSpeedLevel + 1) % 3;
    switch (scrollSpeedLevel) {
      case 0:
        scrollSpeed = SCROLL_SPEED_LOW;
        range = 2;
        break;
      case 1:
        scrollSpeed = SCROLL_SPEED_MEDIUM;
        range = 4;
        break;
      case 2:
        scrollSpeed = SCROLL_SPEED_HIGH;
        range = 8;
        break;
    }
    Serial.print("Scroll/Move Speed: ");
    Serial.println(scrollSpeed);
  }

  if (scrollMode) {
    // Modo de scroll bidirecional
    int scrollY = (downState - upState) * scrollSpeed;
    int scrollX = (rightState - leftState) * scrollSpeed;

    if (scrollY != 0) {
      Mouse.move(0, 0, scrollY);
    }

    if (scrollX != 0) {
      // Simula o scroll horizontal
      Keyboard.press(KEY_LEFT_SHIFT);
      Mouse.move(0, 0, scrollX);
      Keyboard.release(KEY_LEFT_SHIFT);
    }
  } else {
    // Modo de movimento normal do mouse
    if (((upState | downState | rightState | leftState) == 1) && d == 0) {
      a = millis();
      d = 1;
    }

    if (((upState | downState | rightState | leftState) == 0) && d == 1) {
      a = 0;
      d = 0;
    }

    if ((a + TEMPO) < (millis()) && d == 1) {
      t = t + MULTIPLI;
      xDistance = (rightState - leftState) * t;
      yDistance = (downState - upState) * t;
    } else {
      xDistance = (rightState - leftState) * range;
      yDistance = (downState - upState) * range;
      t = range;
    }
    if ((xDistance != 0) || (yDistance != 0)) {
      Mouse.move(xDistance, yDistance, 0);
    }
  }

  // Controle do botão esquerdo do mouse
  if (digitalRead(mouseButton) == LOW) {
    if (variavel2 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(mouseButton) == LOW) {
        Mouse.press(MOUSE_LEFT);
        variavel2 = 0;
      }
    }
  } else {
    if (variavel2 == 0) {
      Mouse.release(MOUSE_LEFT);
      variavel2 = 1;
    }
  }

  // Controle do botão direito do mouse
  if (digitalRead(rightmouseButton) == LOW) {
    if (variavel3 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(rightmouseButton) == LOW) {
        Mouse.press(MOUSE_RIGHT);
        variavel3 = 0;
      }
    }
  } else {
    if (variavel3 == 0) {
      Mouse.release(MOUSE_RIGHT);
      variavel3 = 1;
    }
  }

  // Controle de duplo clique
  if (digitalRead(doubleleft) == LOW) {
    if (variavel4 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(doubleleft) == LOW) {
        Mouse.click(MOUSE_LEFT);
        delay(100);
        Mouse.click(MOUSE_LEFT);
        variavel4 = 0;
      }
    }
  } else {
    variavel4 = 1;
  }

  // Controle de clique longo
  if (digitalRead(leftLong) == LOW) {
    if (variavel == 1) {
      delay(DEBOUNCING);
      if (digitalRead(leftLong) == LOW) {
        Mouse.press(MOUSE_LEFT);
        variavel = 0;
        variavel5 = 0;
      }
    }
  } else {
    if (variavel == 0) {
      Mouse.release(MOUSE_LEFT);
      variavel = 1;
      variavel5 = 1;
    }
  }

  delay(responseDelay);
}
