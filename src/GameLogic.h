#include <iostream>
#include <list>
#include <cstdlib>
#include <ctime>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FastLED.h>

const int SPACESHIP = 0;
const int SHOT = 1;
const int ALIEN = 2;

const int GameFieldX = 64;
const int GameFieldY = 58;

int GameField[GameFieldX][GameFieldY]; // 64x64 Spielfeld mit 6 Pixel hohen Bereich für Punkte und Leben

int live=3;
int score=56789;

struct gameObjekt {
    int gameObjektKind; // Typ des Objekts
    int sourceX;        // Position X
    int sourceY;        // Position Y
    int speed;          // Geschwindigkeit (für Aliens)
    
    gameObjekt(int kind, int x, int y) : gameObjektKind(kind), sourceX(x), sourceY(y), speed(0) {
        if (kind == ALIEN) {
            speed = (std::rand() % 5) - 2; // Zufällige Geschwindigkeit für Aliens
        }
    }
};

// Liste für alle Spielobjekte
std::list<gameObjekt> gameObjects;

void drawHitbox(gameObjekt &temp);

void insert(gameObjekt obj) {
    auto it = gameObjects.begin();
    
    while (it != gameObjects.end() && it->gameObjektKind < obj.gameObjektKind) {
        ++it;
    }
    
    gameObjects.insert(it, obj);
}

void setupGame() {
    std::srand(std::time(0));
    gameObjects.emplace_back(SPACESHIP, 0, 0); // Das Raumschiff initialisieren
    for(int i = 0; i<GameFieldY-1;i++){
        for(int j = 0; j<GameFieldX-1; j++){
            GameField[i][j] = 0;
        }
    }
}

void drawHitboxSpaceship(gameObjekt &temp) {
    // TODO: Implementieren, wenn das Modell final ist
}

void drawHitboxShot(gameObjekt &temp) {
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            GameField[temp.sourceX + i][temp.sourceY + j] = SHOT;
        }
    }
}

void drawHitboxAlien(gameObjekt &temp) {
    for (int i = 1; i < 8; i += 2) {
        GameField[temp.sourceX + i][temp.sourceY] = ALIEN;
    }
    for (int i = 2; i < 7; i++) {
        GameField[temp.sourceX + i][temp.sourceY + 1] = ALIEN;
    }
    for (int j = 0; j < 2; j++) {
        for (int i = 0; i < 9; i++) {
            GameField[temp.sourceX + i][temp.sourceY + 2 + j] = ALIEN;
        }
    }
    for (int i = 1; i < 8; i++) {
        GameField[temp.sourceX + i][temp.sourceY + 4] = ALIEN;
    }
    for (int j = 0; j < 2; j++) {
        for (int i = 2; i < 7; i++) {
            GameField[temp.sourceX + i][temp.sourceY + 5 + j] = ALIEN;
        }
    }
    for (int i = 3; i < 6; i++) {
        GameField[temp.sourceX + i][temp.sourceY + 7] = ALIEN;
    }
}

void showHitboxField() {
    for(int i = GameFieldY-1; i>0; i--){
        for(int j = GameFieldX-1; j>0; j--){
            if(GameField[j][i]==0){
                Serial.print("0");
            }
            else{
                Serial.print(GameField[j][i]);
            }
            Serial.print(" ");
        }
        Serial.printf("\n");
    }
}

void drawHitbox(gameObjekt &temp) {
    switch (temp.gameObjektKind) {
        case SPACESHIP:
            drawHitboxSpaceship(temp);
            break;
        case SHOT:
            drawHitboxShot(temp);
            break;
        case ALIEN:
            drawHitboxAlien(temp);
            break;
    }
}

void drawField() {
    for (int i = 0; i < GameFieldY; i++) {
        for (int j = 0; j < GameFieldX; j++) {
            Serial.print(GameField[j][i]);
        }
        Serial.print("\n");
    }
}
