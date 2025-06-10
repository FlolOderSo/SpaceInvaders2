#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FastLED.h>
#include <GameLogic.h>
#include <stdio.h>

// Configure for your panel(s) as appropriate!
#define PANEL_WIDTH 64
#define PANEL_HEIGHT 64
#define PANELS_NUMBER 1
#define PIN_E 32

#define PANE_WIDTH PANEL_WIDTH * PANELS_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT

void draw(gameObjekt& temp);
void drawShot(gameObjekt& temp);
void drawAlien(gameObjekt& temp);
void drawShip(gameObjekt& temp);
void drawUI();
void testBoxes();

// placeholder for the matrix object
MatrixPanel_I2S_DMA *dma_display = nullptr;

uint16_t time_counter = 0, cycles = 0, fps = 0;
unsigned long fps_timer;

CRGB currentColor;
CRGBPalette16 palettes[] = {HeatColors_p, LavaColors_p, RainbowColors_p, RainbowStripeColors_p, CloudColors_p};
CRGBPalette16 currentPalette = palettes[0];

uint16_t myBLACK = dma_display->color565(0, 0, 0);
uint16_t myWHITE = dma_display->color565(255, 255, 255);
uint16_t myRED = dma_display->color565(255, 0, 0);
uint16_t myGREEN = dma_display->color565(0, 255, 0);
uint16_t myBLUE = dma_display->color565(0, 0, 255);

CRGB ColorFromCurrentPalette(uint8_t index = 0, uint8_t brightness = 255, TBlendType blendType = LINEARBLEND) {
    return ColorFromPalette(currentPalette, index, brightness, blendType);
}

void setup() {
    Serial.begin(115200);
    Serial.println(F("*****************************************************"));
    Serial.println(F("*                   Space Invaders                  *"));
    Serial.println(F("*****************************************************"));

    HUB75_I2S_CFG mxconfig;
    mxconfig.mx_height = PANEL_HEIGHT;
    mxconfig.chain_length = PANELS_NUMBER;
    mxconfig.gpio.e = PIN_E;
    

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    dma_display->setBrightness8(255); // Set brightness to 100%

    if (!dma_display->begin()) {
        Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
    }

    Serial.println("Fill screen: black");
    dma_display->fillScreenRGB888(0, 0, 0);
    delay(1000);

    currentPalette = RainbowColors_p;
    //Serial.println("Starting plasma effect...");
    fps_timer = millis();

    
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);

    pinMode(left, INPUT);
    pinMode(right, INPUT);
    pinMode(shot, INPUT);

    ledcSetup(pwmChannel, pwmFreq, pwmResolution);
    ledcAttachPin(ena, pwmChannel);

    setupGame();
    sleep(10);
}

void scoreToArr(int zahl, int array[8]) {
    if (zahl < 0 || zahl > 99999999) {
        for (int i = 0; i < 8; i++) array[i] = 0;
        return;
    }

    int temp[8];
    int stelle = 0;

    if (zahl == 0) {
        for (int i = 0; i < 8; i++) array[i] = 0;
        return;
    }

    while (zahl > 0 && stelle < 8) {
        temp[stelle++] = zahl % 10;
        zahl /= 10;
    }

    int nullen = 8 - stelle;
    for (int i = 0; i < nullen; i++) {
        array[i] = 0;
    }
    for (int i = 0; i < stelle; i++) {
        array[nullen + i] = temp[stelle - 1 - i];
    }
}


void draw(gameObjekt& temp) {
  switch (temp.gameObjektKind) {
      case 0:
      drawShip(temp);
          break;
      case 1:
          drawShot(temp);
          break;
      case 2:
          drawAlien(temp);
          break;
      default:
          break;
    }
}

void drawAlien(gameObjekt& temp) {
    for(int i=1; i < 8; i+=2){
        dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY + 6, 255, 0, 0);
    }
    for(int i=2; i < 7; i++){
        dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 1 + 6, 255, 0, 0);
    }
    for(int i=0; i < 9; i++){
        dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 2 + 6, 0, 0, 255);
    }
    for(int i=0; i < 9; i++){
        if((i!=1) && (i!=4) && (i!=7)){
            dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 3 + 6, 0, 0, 255);
        }
        for(int i=1; i < 8; i++){
            dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 4 + 6, 0, 0, 255);
        }
        for(int i=2; i < 7; i++){
            if((i!=4) && (i!=5) )
                dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 5 + 6, 255, 255, 255);
        }
        for(int i=2; i < 7; i++){
            if((i!=3) && (i!=4) && (i!=5))
                dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 6 + 6, 255, 255, 255);
        }
        for(int i=3; i < 6; i++){
            dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - 7 + 6, 255, 255, 255);
        }
    }
}

void drawShot(gameObjekt& temp) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
          dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY - j + 6, 255, 255, 255);
        }
    }
}

void drawShip(gameObjekt& temp){
    for (int i = 0; i < 16; i++) {
        dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i, GameFieldY - temp.sourceY + 6, 0, 10, 0);
    }
    dma_display->drawPixelRGB888(GameFieldX - temp.sourceX, GameFieldY - temp.sourceY + 5, 0, 10, 0);
    dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - 15, GameFieldY - temp.sourceY + 5, 0, 10, 0);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            dma_display->drawPixelRGB888(GameFieldX - temp.sourceX - i - 7, GameFieldY - temp.sourceY - j + 5, 0, 10, 0);
        }
    }
}

void drawNumber(int n, int x, int y){
    switch (n) {
        case 0:
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            for (int i = 0; i < 3; i++){
                for (int j = 1; j < 4; j++) {
                    if(i!=1){
                        dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6 - j, 255, 255, 255);
                    }
                }
            }
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        case 1:
            for (int i = 0; i < 5; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x , GameFieldY - y - i + 6, 255, 255, 255);
            }
                break;
        case 2:
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x - 2, GameFieldY - y + 5, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 4, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x, GameFieldY - y  + 3, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        case 3:
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x, GameFieldY - y + 5, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 4, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x, GameFieldY - y + 3, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        case 4:
            for (int i = 0; i < 5; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x , GameFieldY - y - i + 6, 255, 255, 255);
            }  
            dma_display->drawPixelRGB888(GameFieldX - x - 1 , GameFieldY - y + 4, 255, 255, 255); 
            for (int i = 2; i < 5; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - 2, GameFieldY - y - i + 6, 255, 255, 255);
            }     
            break;
        case 5:
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x - 2, GameFieldY - y + 3, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 4, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x, GameFieldY - y + 5, 255, 255, 255);
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        case 6:
            for (int i = 0; i < 3; i++) {
            dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            for (int i = 0; i < 3; i++) {
                if(i!=1){
                    dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 5, 255, 255, 255);
                }
            }
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 4, 255, 255, 255);
            }
            for (int i = 0; i < 2; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - 2, GameFieldY - y - i + 3, 255, 255, 255);
            }
            break;
        case 7:
        for (int i = 0; i < 5; i++) {
            dma_display->drawPixelRGB888(GameFieldX - x , GameFieldY - y - i + 6, 255, 255, 255);
            }
            for (int i = 1; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        case 8:
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 6, 255, 255, 255);
            }
            for (int i = 0; i < 3; i++){
                for (int j = 0; j < 3; j++) {
                    if(i!=1){
                        dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y - j + 5, 255, 255, 255);
                    }
                }
            }
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            dma_display->drawPixelRGB888(GameFieldX - x - 1, GameFieldY - y + 4, 255, 255, 255);
            break;
        case 9:
            for (int i = 0; i < 2; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x, GameFieldY - y - i + 6, 255, 255, 255);
            }
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 4, 255, 255, 255);
            }
            for (int i = 0; i < 3; i++) {
                if(i!=1){
                    dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 3, 255, 255, 255);
                }
            }
            for (int i = 0; i < 3; i++) {
                dma_display->drawPixelRGB888(GameFieldX - x - i, GameFieldY - y + 2, 255, 255, 255);
            }
            break;
        default:
            break;
      } 
}

void drawScore(){
    Serial.println("drawing Score");
    int arr[8];
    scoreToArr(score, arr);
    int x = 0;
    for(int i = 0; i<8; i++){
        x =((8-i) * 4) - 2; 
        drawNumber(arr[i], x, 60);
    }
}

void drawUI(){
    //drawing the line under the UI
    for(int i=0; i<64; i++){
        dma_display->drawPixelRGB888(i, 5, 0, 255, 0);
    }

    //drawing the Harts to display lives
    for(int off=0; off < live; off++){
        dma_display->drawPixelRGB888(3 + off * 6, 4, 255, 0, 0);
        for(int i =1; i < 4; i++){
            dma_display->drawPixelRGB888(i + 1 + off * 6, 3, 255, 0, 0);  
        }
        for(int i =0; i < 5; i++){
            dma_display->drawPixelRGB888(i + 1 + off * 6, 2, 255, 0, 0);  
        }
        for(int i =0; i < 5; i++){
            dma_display->drawPixelRGB888(i + 1 + off * 6, 1, 255, 0, 0);  
        }
        dma_display->drawPixelRGB888(2 + off * 6, 0, 255, 0, 0);
        dma_display->drawPixelRGB888(4 + off * 6, 0, 255, 0, 0);
    }

    //writing the score
    drawScore();

}

void loop() {
    movement = false;
    clearGameField();
    for(gameObjekt& obj : gameObjects){
        doStuff(obj);
    }
    spawnShot();
    for(gameObjekt& obj : gameObjects){
        drawHitbox(obj);
        Serial.print(obj.gameObjektKind);
    }
    newEnemy();
    //showHitboxField();
    dma_display->clearScreen();
    for(gameObjekt& obj : gameObjects){
        draw(obj);
    }
    drawUI();
    //showHitboxField(); // enable for debugng, but this is realy bad for performance
    if(!movement)delay(movementSpeed);
}