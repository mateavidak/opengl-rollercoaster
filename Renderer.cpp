#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

// Forward deklaracije struktura
struct Passenger {
    bool occupied;
    bool belted;
    bool sick;
    float x;
    float y;
    int seatNumber;
};

struct RollercoasterCar {
    float positionOnTrack;
    float speed;
    float x;
    float y;
    float rotation;
};

extern unsigned int passengerTexture;
extern unsigned int carTexture;
extern unsigned int beltTexture;
extern unsigned int infoTexture;
extern unsigned int sickPassengerTexture;
extern unsigned int backgroundTexture; 
extern unsigned int rectShader;
extern unsigned int colorShader;
extern RollercoasterCar car;
extern Passenger passengers[];
extern std::vector<float> trackVertices;
extern int numTrackPoints;


extern unsigned int loadImageToTexture(const char* filePath);
extern unsigned int createShader(const char* vsSource, const char* fsSource);


unsigned int VAOrect, VBOrect;
unsigned int VAOtrack, VBOtrack;

#define NUM_SEATS 8

void preprocessTexture(unsigned int& texture, const char* filepath) {
    texture = loadImageToTexture(filepath);
    if (texture == 0) {
        std::cout << "GRESKA: Nije ucitana tekstura: " << filepath << std::endl;
        return;
    }

    std::cout << "OK: Ucitana tekstura: " << filepath << " (ID: " << texture << ")" << std::endl;

    glBindTexture(GL_TEXTURE_2D, texture);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void initVAOs() {
    // VAO/VBO za pravougaonike
    glGenVertexArrays(1, &VAOrect);
    glGenBuffers(1, &VBOrect);

    glBindVertexArray(VAOrect);
    glBindBuffer(GL_ARRAY_BUFFER, VBOrect);

    glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // VAO/VBO za prugu
    glGenVertexArrays(1, &VAOtrack);
    glGenBuffers(1, &VBOtrack);

    glBindVertexArray(VAOtrack);
    glBindBuffer(GL_ARRAY_BUFFER, VBOtrack);
    glBufferData(GL_ARRAY_BUFFER, trackVertices.size() * sizeof(float),
        trackVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void initRenderer() {
    // Ucitavanje tekstura
    preprocessTexture(backgroundTexture, "res/background.png");  
    preprocessTexture(passengerTexture, "res/person.png");
    preprocessTexture(carTexture, "res/car.png");
    preprocessTexture(beltTexture, "res/belt.png");
    preprocessTexture(infoTexture, "res/info.png");
    preprocessTexture(sickPassengerTexture, "res/sick person.png");

    // Kreiranje shader programa
    rectShader = createShader("rect.vert", "rect.frag");
    colorShader = createShader("color.vert", "color.frag");

    // Inicijalizacija VAO/VBO
    initVAOs();
}

void cleanupRenderer() {
    glDeleteProgram(rectShader);
    glDeleteProgram(colorShader);
    glDeleteTextures(1, &passengerTexture);
    glDeleteTextures(1, &carTexture);
    glDeleteTextures(1, &beltTexture);
    glDeleteTextures(1, &infoTexture);
    glDeleteTextures(1, &sickPassengerTexture);
    glDeleteTextures(1, &backgroundTexture);  

    glDeleteVertexArrays(1, &VAOrect);
    glDeleteBuffers(1, &VBOrect);
    glDeleteVertexArrays(1, &VAOtrack);
    glDeleteBuffers(1, &VBOtrack);
}

void drawRect(float x, float y, float width, float height, unsigned int texture, float alpha) {
    glUseProgram(rectShader);

    // 4 verteksa u TRIANGLE_FAN redosledu
    float vertices[] = {
        x - width / 2, y - height / 2,  0.0f, 0.0f,  // donji levi
        x + width / 2, y - height / 2,  1.0f, 0.0f,  // donji desni
        x + width / 2, y + height / 2,  1.0f, 1.0f,  // gornji desni
        x - width / 2, y + height / 2,  0.0f, 1.0f   // gornji levi
    };

    glBindVertexArray(VAOrect);
    glBindBuffer(GL_ARRAY_BUFFER, VBOrect);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glUniform1f(glGetUniformLocation(rectShader, "uAlpha"), alpha);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(rectShader, "uTex"), 0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void drawTrack() {
    glUseProgram(colorShader);

    glBindVertexArray(VAOtrack);

    glUniform4f(glGetUniformLocation(colorShader, "uColor"), 0.3f, 0.3f, 0.3f, 1.0f);

    glLineWidth(5.0f);
    glDrawArrays(GL_LINE_LOOP, 0, numTrackPoints);
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Crtanje pozadine
    drawRect(0.0f, 0.0f, 2.0f, 2.0f, backgroundTexture, 1.0f);

    // Crtanje pruge
    drawTrack();

    // Crtanje vozila 
    drawRect(car.x, car.y, 0.32f, 0.32f, carTexture, 1.0f);

    // Crtanje putnika 
    int passengersPerRow = 4;
    float rowWidth = 0.33f;
    float seatSpacing = rowWidth / passengersPerRow;
    float startOffsetX = -0.16f;
    float rowSpacing = 0.08f;

    for (int i = 0; i < NUM_SEATS; i++) {
        if (passengers[i].occupied) {
            int row = i / passengersPerRow;
            int seatInRow = i % passengersPerRow;

            float passengerX = car.x + startOffsetX + (seatInRow + 0.5f) * seatSpacing;
            float passengerY = car.y + 0.06f - (row * rowSpacing);

            passengers[i].x = passengerX;
            passengers[i].y = passengerY;

            unsigned int texToUse = passengers[i].sick ? sickPassengerTexture : passengerTexture;
            drawRect(passengerX, passengerY, 0.080f, 0.100f, texToUse, 1.0f);

            if (passengers[i].belted) {
                drawRect(passengerX, passengerY, 0.090f, 0.050f, beltTexture, 1.0f);
            }
        }
    }

    // Info tabla
    drawRect(0.75f, 0.85f, 0.4f, 0.2f, infoTexture, 0.8f);
}