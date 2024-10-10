#include <Mouse.h>


/*
  Mouse de botoes
  Para placas de arduino micro, Leonardo, Pro Micro and Due boards only.
  O movimento do mouse é sempre relativo. 
  
  Código de exemplo criado originalmente por Tom Igoe disponivel em:
  http://www.arduino.cc/en/Tutorial/ButtonMouseControl

  A implementação das demais funções e adequações foi realizada por bolsistas do projeto CRTA.
  Mais informações estão disponíveis em: https://cta.ifrs.edu.br/
*/


#define DEBOUNCING 50     //tempo(millisegundos) para o DEBOUNCING
#define TEMPO 200         //tempo para fazer a funcao de acelerar o mouse(valor em milissegundos)
#define MULTIPLI 0.3     //Variar entre 0.001 e 0.08 para acelerar o avanco do mouse apos 800 milissegundos segurados
#define MULTIPLICA 1023      //Velocidade maxima que o mouse tera no maximo do joystick(0-1023)

//dizendo ao programa o numero dos pinos conectados aos botoes

// Para utilizar os modulos analogicos, descomentar esta secao
//const int analogico1 = A0; As portas analógicas somente serão utilizadas em conjunto com o módulo analógico
//const int analogico2 = A1;

// Botoes de acao
const int upButton = 2; //movimento acima 
const int downButton = 3; //movimento abaixo
const int leftButton = 5; //movimento esquerda
const int rightButton = 4; //movimento direito

const int mouseButton = 6; //clique esquerdo
const int rightmouseButton = 7; //clique direito
const int leftLong = 8; //clique esquerdo preso
const int doubleleft = 9; //clique esquer duplo
const int StickButton = 10; //botao scrool
const int velocidade = 11; //botao para alternar velocidade de movimento

int range = 1;              // multiplicador do movimento do mouse em X e Y, quanto maior o numero mais rapido fica o mouse
int responseDelay = 10;     // tempo de resposta do mouse(quanto maior o numero mais lento o mouse fica),numero em milisegundos
boolean variavel = 0, variavel2 = 1, variavel3 = 1, variavel4 = 1, variavel5 = 1, variavel6 = 1;
int d = 0;
unsigned long espaco = 0, a = 0;
float  t = 0.1;

void setup() {
  // inicializa as variaveis como entradas

  // Para utilizar os modulos analogicos, descomentar esta secao
  //pinMode(analogico1, INPUT);
  //pinMode(analogico2, INPUT);

  
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(mouseButton, INPUT_PULLUP);
  pinMode(leftLong, INPUT_PULLUP);
  pinMode(doubleleft, INPUT_PULLUP);
  pinMode(rightmouseButton, INPUT_PULLUP);
  pinMode(StickButton, INPUT_PULLUP);
  pinMode(velocidade, INPUT_PULLUP);
  
  // inicializa a biblioteca do mouse.h
  Mouse.begin();
  Serial.begin(9600);
}

void loop() {
  // Este loop ? infinito,e em cada vez que o loop reinicia ? feito a leitura dos bot?es
  int  xDistance = 0;
  int  yDistance = 0;

  boolean upState = !(digitalRead(upButton));
  boolean downState = !(digitalRead(downButton));
  boolean rightState = !(digitalRead(rightButton));
  boolean leftState = !(digitalRead(leftButton));

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
    xDistance = (leftState - rightState) * t;
    yDistance = (upState - downState) * t;
  } else {
    xDistance = (leftState - rightState) * range;
    yDistance = (upState - downState) * range;
    t = range;
  }
  if ((xDistance != 0) || (yDistance != 0)) {
    Mouse.move(xDistance, yDistance, 0);
  }
 //se o botao esquerdo ? apertado
  if (digitalRead(mouseButton) == LOW) {
    if (variavel2 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(mouseButton) == LOW) {
        Mouse.press(MOUSE_LEFT);
        variavel2 = 0;
      }
    }
  } else {
    variavel2 = 1;
  }
  if (digitalRead(StickButton) == LOW) {
    if (variavel6 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(StickButton) == LOW) {
        Mouse.press(MOUSE_LEFT);
        variavel6 = 0;
      }
    }
  } else {
    variavel6 = 1;
  }

  // se o botao direito ? apertado
  if (digitalRead(rightmouseButton) == LOW) {
    if (variavel3 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(rightmouseButton) == LOW) {
        Mouse.press(MOUSE_RIGHT);
        variavel3 = 0;
      }
    }
  } else {
    Mouse.release(MOUSE_RIGHT);
    variavel3 = 1;
  }

  //se o botao de clique duplo ? apertado
  if (digitalRead(doubleleft) == LOW) {
    if (variavel4 == 1) {
      delay(DEBOUNCING);
      if (digitalRead(doubleleft) == LOW) {
        // muda o estado l?gico do bot?o
        if (!Mouse.isPressed(MOUSE_LEFT)) {
          Mouse.click(MOUSE_LEFT);
          delay(200);
          Mouse.click(MOUSE_LEFT);
          delay(500);
          variavel4 = 0;
        }
      }
    }
  } else {
    variavel4 = 1;
  }
  //se o botao de clique longo ? apertado
  if (digitalRead(leftLong) == LOW) {
    delay(DEBOUNCING);
    if (digitalRead(leftLong) == LOW && variavel == 1) {
      // muda o estado l?gico do bot?o
      Mouse.press(MOUSE_LEFT);
      variavel = !variavel;
      variavel5 = !variavel5;
    }
  } else {
    variavel = 1;

  }
  if (variavel2 == 0 || variavel6 == 0) {
    variavel5 = 1;
  }
  if (variavel2 == 1 && variavel5 == 1 && variavel6 == 1) {
    Mouse.release(MOUSE_LEFT);
  }

  if ((digitalRead(velocidade) == LOW) && (espaco + 1000) < millis()) {
    espaco = millis();
    range = range + 2;
    if (range > 8) {
      range = 2;
    }
    Serial.println(range);
  }

  delay(responseDelay);
}
