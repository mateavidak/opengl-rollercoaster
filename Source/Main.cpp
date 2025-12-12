#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <vector>
#include "../Header/Util.h"

// Konstante
#define NUM_SEATS 8
const float TARGET_FPS = 75.0f;

// Globalne promenljive za dimenzije ekrana
int screenWidth = 1920;
int screenHeight = 1080;

// Stanja igre
enum GameState {
    LOADING_PASSENGERS,
    RIDING,
    EMERGENCY_STOP,
    RETURNING,
    UNLOADING
};

// Struktura za putnika
struct Passenger {
    bool occupied = false;
    bool belted = false;
    bool sick = false;
    float x = 0.0f;
    float y = 0.0f;
    int seatNumber = 0;
};

// Struktura za vozilo
struct RollercoasterCar {
    float positionOnTrack = 0.0f;
    float speed = 0.0f;
    float x = -0.85f;
    float y = 0.0f;
    float rotation = 0.0f;
};

// Globalne promenljive
int currentState = LOADING_PASSENGERS;
Passenger passengers[NUM_SEATS];
RollercoasterCar car;
float emergencyTimer = 0.0f;

// Pruga
std::vector<float> trackVertices;
int numTrackPoints = 0;

// Teksture
unsigned int passengerTexture = 0;
unsigned int carTexture = 0;
unsigned int beltTexture = 0;
unsigned int infoTexture = 0;
unsigned int sickPassengerTexture = 0;
unsigned int backgroundTexture = 0;

// Sejderi
unsigned int rectShader = 0;
unsigned int colorShader = 0;

// Kursor
GLFWcursor* railCursor = nullptr;

// Forward deklaracije funkcija iz drugih fajlova
void initTrack();
void getTrackPosition(float t, float& x, float& y, float& angle);
void updatePhysics(float deltaTime);
void initRenderer();
void cleanupRenderer();
void renderScene();

// Mouse callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        float ndcX = (xpos / screenWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (ypos / screenHeight) * 2.0f;

        if (currentState == LOADING_PASSENGERS) {
            for (int i = 0; i < NUM_SEATS; i++) {
                if (passengers[i].occupied && !passengers[i].belted) {
                    float dx = ndcX - passengers[i].x;
                    float dy = ndcY - passengers[i].y;
                    float dist = sqrt(dx * dx + dy * dy);

                    if (dist < 0.05f) {
                        passengers[i].belted = true;
                        std::cout << "Putnik " << (i + 1) << " zavezan!" << std::endl;
                        break;
                    }
                }
            }
        }
        else if (currentState == UNLOADING) {
            for (int i = 0; i < NUM_SEATS; i++) {
                if (passengers[i].occupied) {
                    float dx = ndcX - passengers[i].x;
                    float dy = ndcY - passengers[i].y;
                    float dist = sqrt(dx * dx + dy * dy);

                    if (dist < 0.05f) {
                        passengers[i].occupied = false;
                        passengers[i].belted = false;
                        passengers[i].sick = false;
                        std::cout << "Putnik " << (i + 1) << " napustio vozilo!" << std::endl;

                        bool allGone = true;
                        for (int j = 0; j < NUM_SEATS; j++) {
                            if (passengers[j].occupied) {
                                allGone = false;
                                break;
                            }
                        }

                        if (allGone) {
                            std::cout << "Svi putnici napustili vozilo. Spremno za novo ukrcavanje!" << std::endl;
                            currentState = LOADING_PASSENGERS;
                            car.positionOnTrack = 0.0f;
                            getTrackPosition(0.0f, car.x, car.y, car.rotation);
                        }
                        break;
                    }
                }
            }
        }
    }
}

// Keyboard callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (action == GLFW_PRESS) {
        if (currentState == LOADING_PASSENGERS) {
            if (key == GLFW_KEY_SPACE) {
                for (int i = 0; i < NUM_SEATS; i++) {
                    if (!passengers[i].occupied) {
                        passengers[i].occupied = true;
                        passengers[i].belted = false;
                        passengers[i].sick = false;
                        passengers[i].seatNumber = i + 1;
                        std::cout << "Putnik " << (i + 1) << " ukrcan!" << std::endl;
                        break;
                    }
                }
            }
            else if (key == GLFW_KEY_ENTER) {
                bool canStart = true;
                bool hasPassengers = false;

                for (int i = 0; i < NUM_SEATS; i++) {
                    if (passengers[i].occupied) {
                        hasPassengers = true;
                        if (!passengers[i].belted) {
                            canStart = false;
                            break;
                        }
                    }
                }

                if (canStart && hasPassengers) {
                    std::cout << "Voznja pocinje!" << std::endl;
                    currentState = RIDING;
                    car.speed = 0.1f;
                }
                else if (!hasPassengers) {
                    std::cout << "Nema putnika!" << std::endl;
                }
                else {
                    std::cout << "Svi putnici moraju biti zavezani!" << std::endl;
                }
            }
        }
        else if (currentState == RIDING) {
            if (key >= GLFW_KEY_1 && key <= GLFW_KEY_8) {
                int seatNum = key - GLFW_KEY_1;
                if (passengers[seatNum].occupied) {
                    passengers[seatNum].sick = true;
                    std::cout << "Putniku " << (seatNum + 1) << " je muka! HITAN STOP!" << std::endl;
                    currentState = EMERGENCY_STOP;
                    emergencyTimer = 0.0f;
                }
            }
        }
    }
}

int main()
{
    if (!glfwInit()) {
        std::cout << "GLFW nije uspeo da se inicijalizuje!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    screenWidth = mode->width;
    screenHeight = mode->height;

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Rolerkoster 2D", monitor, NULL);
    if (window == NULL) {
        std::cout << "Prozor nije uspeo da se kreira." << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW nije uspeo da se inicijalizuje." << std::endl;
        glfwTerminate();
        return -1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ✅ UČITAVANJE KURSORA
    railCursor = loadImageToCursor("Resources/cursor.png");
    if (railCursor) {
        glfwSetCursor(window, railCursor);
        std::cout << "OK: Ucitan kursor!" << std::endl;
    }
    else {
        std::cout << "UPOZORENJE: Kursor nije ucitan, koristi se default!" << std::endl;
    }

    std::cout << "=== ROLERKOSTER 2D ===" << std::endl;
    std::cout << "SPACE - Dodaj putnika" << std::endl;
    std::cout << "LMB - Zavezi putnika / Iskrcaj putnika" << std::endl;
    std::cout << "ENTER - Pokreni voznju" << std::endl;
    std::cout << "1-8 - Putniku pozli tokom voznje" << std::endl;
    std::cout << "ESC - Izlaz" << std::endl;

    // Inicijalizacija
    initTrack();
    initRenderer();
    getTrackPosition(0.0f, car.x, car.y, car.rotation);

    for (int i = 0; i < NUM_SEATS; i++) {
        passengers[i].occupied = false;
        passengers[i].belted = false;
        passengers[i].sick = false;
        passengers[i].seatNumber = i + 1;
    }

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    // Glavni game loop
    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        double currentFrameTime = glfwGetTime();
        double deltaTime = currentFrameTime - lastFrameTime;

        if (deltaTime >= 1.0 / TARGET_FPS) {
            updatePhysics((float)deltaTime);
            renderScene();

            glfwSwapBuffers(window);
            lastFrameTime = currentFrameTime;
        }

        glfwPollEvents();
    }

    // Cleanup
    std::cout << "Zatvaranje programa..." << std::endl;

    // ✅ BRISANJE KURSORA
    if (railCursor) {
        glfwDestroyCursor(railCursor);
    }

    cleanupRenderer();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}