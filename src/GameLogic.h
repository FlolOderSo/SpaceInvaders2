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
int cooldown = 0;

struct gameObjekt {
    int gameObjektKind; // Typ des Objekts
    int sourceX;        // Position X
    int sourceY;        // Position Y
    int speed;          // Geschwindigkeit (für Aliens)
    
    gameObjekt(int kind, int x, int y){
        gameObjektKind = kind;
        Serial.print("new");
        sourceX = x;
        sourceY = y;
        speed = (std::rand() % 6) - 2; // Zufällige Geschwindigkeit für Aliens
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

void clearGameField(){
    for(int i = 0; i<GameFieldY-1;i++){
        for(int j = 0; j<GameFieldX-1; j++){
            GameField[i][j] = 0;
        }
    }
}

void setupGame() {
    std::srand(std::time(0));
    gameObjects.emplace_back(SPACESHIP, 1, 1); // Das Raumschiff initialisieren
    clearGameField();
}

void removeGameObjekt(const gameObjekt &obj) {
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(),
        [&](const gameObjekt& current) {
            return &current == &obj; // exakt dasselbe Objekt
        });

    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
}


void newEnemy(){
    Serial.print("try to spawn");
    if((std::rand() % 8) == 1){
        int sourceX = 1 + std::rand() % 55;
        bool check=true;
        for(int i=0; i<9; i++){
            if(GameField[51][sourceX + i]!=0){
                check=false;
            }
        }
        if(check){
                insert(gameObjekt(ALIEN, sourceX, 51));
            }
    }
}

void drawHitboxSpaceship(gameObjekt &temp) {
    for (int i = 0; i < 16; i++) {
        GameField[temp.sourceX + i][temp.sourceY] = SPACESHIP;
    }
    GameField[temp.sourceX][temp.sourceY + 1] = SPACESHIP;
    GameField[temp.sourceX + 15][temp.sourceY + 1] = SPACESHIP;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            GameField[temp.sourceX + i + 7][temp.sourceY + j + 1] = SPACESHIP;
        }
    }
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

void doStuffSpaceship( gameObjekt &obj){
    //movement
}

void doStuffShot(gameObjekt &obj){
    obj.sourceY++;
}

void doStuffAlien(gameObjekt &obj){
    if(obj.sourceY <= 1){
        removeGameObjekt(obj);
        return;
    }
    else{
    obj.sourceY-=1;
    Serial.println("Y: ");
    Serial.println(obj.sourceY);
    Serial.println("\n");
    }
    if((obj.speed>0 && obj.sourceX + obj.speed>55)||(obj.speed<0 && obj.sourceX + obj.speed<1)){
        obj.speed *= -1;
    }
    obj.sourceX += obj.speed;
    
}

void doStuff(gameObjekt &temp) {
    switch (temp.gameObjektKind) {
        case SPACESHIP:
            doStuffSpaceship(temp);
            break;
        case SHOT:
            doStuffShot(temp);
            break;
        case ALIEN:
            doStuffAlien(temp);
            break;
    }
}
