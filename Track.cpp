#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

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

extern std::vector<float> trackVertices;
extern int numTrackPoints;
extern int currentState;
extern RollercoasterCar car;
extern Passenger passengers[];
extern float emergencyTimer;

#define NUM_SEATS 8

// Inicijalizacija pruge
void initTrack() {
    numTrackPoints = 300;
    trackVertices.clear();

    float baselineY = -0.65f;

    for (int i = 0; i < numTrackPoints; i++) {
        float t = (float)i / (numTrackPoints - 1);
        float x = -0.85f + t * 1.7f;
        float y = baselineY;


        if (t >= 0.0f && t <= 0.25f) {
            float localT = (t - 0.0f) / 0.25f;
            y = baselineY + 0.95f * sin(localT * M_PI);
        }
        else if (t > 0.25f && t < 0.35f) {
            y = baselineY;
        }
        else if (t >= 0.35f && t <= 0.60f) {
            float localT = (t - 0.35f) / 0.25f;
            y = baselineY + 0.95f * sin(localT * M_PI);  
        }
        else if (t > 0.60f && t < 0.70f) {
            y = baselineY;
        }
        else if (t >= 0.70f && t <= 0.95f) {
            float localT = (t - 0.70f) / 0.25f;
            y = baselineY + 0.95f * sin(localT * M_PI);
        }
        else {
            y = baselineY;
        }

        trackVertices.push_back(x);
        trackVertices.push_back(y);
    }
}

// Dobijanje pozicije na pruzi
void getTrackPosition(float t, float& x, float& y, float& angle) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float exactIndex = t * (numTrackPoints - 1);
    int index = (int)exactIndex;
    float fraction = exactIndex - index;

    if (index >= numTrackPoints - 1) {
        index = numTrackPoints - 2;
        fraction = 1.0f;
    }

    float x1 = trackVertices[index * 2];
    float y1 = trackVertices[index * 2 + 1];
    float x2 = trackVertices[(index + 1) * 2];
    float y2 = trackVertices[(index + 1) * 2 + 1];

    x = x1 + (x2 - x1) * fraction;
    y = y1 + (y2 - y1) * fraction;
    angle = atan2(y2 - y1, x2 - x1);
}

// Dobijanje nagiba pruge
float getTrackSlope(float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;

    float exactIndex = t * (numTrackPoints - 1);
    int index = (int)exactIndex;

    if (index >= numTrackPoints - 1) {
        index = numTrackPoints - 2;
    }

    float x1 = trackVertices[index * 2];
    float y1 = trackVertices[index * 2 + 1];
    float x2 = trackVertices[(index + 1) * 2];
    float y2 = trackVertices[(index + 1) * 2 + 1];

    float dx = x2 - x1;
    float dy = y2 - y1;

    if (fabs(dx) < 0.0001f) return 0.0f;

    return dy / dx;
}

void updatePhysics(float deltaTime) {
    if (currentState == 1) { // RIDING
        float slope = getTrackSlope(car.positionOnTrack);
        float gravity = -9.81f * 0.05f;
        float acceleration = gravity * slope;

        const float MAX_VELOCITY = 0.8f;
        const float MIN_VELOCITY = 0.2f;

        car.speed += acceleration * deltaTime;

        if (car.speed > MAX_VELOCITY) car.speed = MAX_VELOCITY;
        if (car.speed < MIN_VELOCITY) car.speed = MIN_VELOCITY;

        car.positionOnTrack += car.speed * deltaTime;

        if (car.positionOnTrack >= 1.0f) {
            car.positionOnTrack = 1.0f;
            currentState = 4; // UNLOADING
            car.speed = 0.0f;
        }

        getTrackPosition(car.positionOnTrack, car.x, car.y, car.rotation);
    }
    else if (currentState == 2) { // EMERGENCY_STOP
        car.speed -= 0.5f * deltaTime;
        if (car.speed < 0.0f) car.speed = 0.0f;

        car.positionOnTrack += car.speed * deltaTime;
        getTrackPosition(car.positionOnTrack, car.x, car.y, car.rotation);

        emergencyTimer += deltaTime;
        if (emergencyTimer >= 10.0f && car.speed == 0.0f) {
            currentState = 3; // RETURNING
            emergencyTimer = 0.0f;
        }
    }
    else if (currentState == 3) { // RETURNING
        car.speed = -0.05f;
        car.positionOnTrack += car.speed * deltaTime;

        if (car.positionOnTrack <= 0.0f) {
            car.positionOnTrack = 0.0f;
            car.speed = 0.0f;
            currentState = 4; // UNLOADING
        }

        getTrackPosition(car.positionOnTrack, car.x, car.y, car.rotation);
    }
    else if (currentState == 4) { // UNLOADING
        // Automatsko odvezivanje
        for (int i = 0; i < NUM_SEATS; i++) {
            if (passengers[i].occupied) {
                passengers[i].belted = false;
            }
        }
    }
}