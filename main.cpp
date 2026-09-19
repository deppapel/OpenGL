// anim.cpp
#ifdef _WIN32
#include <windows.h>   // for Windows
#endif
#include <GL/freeglut.h>
#include <cmath>
#include <cstdlib>

// ------------------------------------------------------------
// Global variables for animation state
// ------------------------------------------------------------
float posX = 0.0f, posY = 0.0f;   // object position (range -1..1)
float velX = 0.02f, velY = 0.02f; // automatic movement velocity
float size = 0.2f;                // half-width/height of the square
float r = 1.0f, g = 0.0f, b = 0.0f; // color (red initially)
bool autoMove = true;             // automatic movement enabled
bool isCircle = false;            // shape: false = square, true = circle

// ------------------------------------------------------------
// Function to draw a filled circle using many triangles
// ------------------------------------------------------------
void drawCircle(float cx, float cy, float radius) {
    const int segments = 40;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);           // center
    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * M_PI * i / segments;
        float x = cx + radius * cosf(angle);
        float y = cy + radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// ------------------------------------------------------------
// Display callback – draws the current shape
// ------------------------------------------------------------
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set color
    glColor3f(r, g, b);

    // Draw either a square or a circle
    if (isCircle) {
        drawCircle(posX, posY, size);
    } else {
        glBegin(GL_QUADS);
        glVertex2f(posX - size, posY - size);
        glVertex2f(posX + size, posY - size);
        glVertex2f(posX + size, posY + size);
        glVertex2f(posX - size, posY + size);
        glEnd();
    }

    glutSwapBuffers();
}

// ------------------------------------------------------------
// Idle function – updates automatic movement (animation)
// ------------------------------------------------------------
void idle() {
    if (autoMove) {
        // Move automatically
        posX += velX;
        posY += velY;

        // Bounce off window boundaries (range -1..1 considering size)
        if (posX + size > 1.0f) { posX = 1.0f - size; velX = -velX; }
        if (posX - size < -1.0f) { posX = -1.0f + size; velX = -velX; }
        if (posY + size > 1.0f) { posY = 1.0f - size; velY = -velY; }
        if (posY - size < -1.0f) { posY = -1.0f + size; velY = -velY; }
    }
    glutPostRedisplay();  // request a redraw
}

// ------------------------------------------------------------
// Keyboard callback – handle key presses
// ------------------------------------------------------------
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': case 'C':   // Cycle color
            r = (float)(rand() % 100) / 100.0f;
            g = (float)(rand() % 100) / 100.0f;
            b = (float)(rand() % 100) / 100.0f;
            break;

        case 's': case 'S':   // Toggle shape (square ? circle)
            isCircle = !isCircle;
            break;

        case '+': case '=':   // Increase size
            size += 0.02f;
            if (size > 0.5f) size = 0.5f;
            break;

        case '-': case '_':   // Decrease size
            size -= 0.02f;
            if (size < 0.05f) size = 0.05f;
            break;

        case 'r': case 'R':   // Reset everything
            posX = 0.0f; posY = 0.0f;
            velX = 0.02f; velY = 0.02f;
            size = 0.2f;
            r = 1.0f; g = 0.0f; b = 0.0f;
            autoMove = true;
            isCircle = false;
            break;

        case 'a': case 'A':   // Toggle automatic movement
            autoMove = !autoMove;
            break;

        case 27:              // ESC key – exit
            exit(0);
    }
    glutPostRedisplay();
}

// ------------------------------------------------------------
// Special keyboard callback (arrow keys)
// ------------------------------------------------------------
void specialKeys(int key, int x, int y) {
    // Disable automatic movement when user uses arrow keys
    if (autoMove) autoMove = false;

    float step = 0.05f;
    switch (key) {
        case GLUT_KEY_LEFT:  posX -= step; break;
        case GLUT_KEY_RIGHT: posX += step; break;
        case GLUT_KEY_DOWN:  posY -= step; break;
        case GLUT_KEY_UP:    posY += step; break;
    }
    // Clamp position to keep the shape inside the window
    if (posX + size > 1.0f) posX = 1.0f - size;
    if (posX - size < -1.0f) posX = -1.0f + size;
    if (posY + size > 1.0f) posY = 1.0f - size;
    if (posY - size < -1.0f) posY = -1.0f + size;

    glutPostRedisplay();
}

// ------------------------------------------------------------
// Initialization
// ------------------------------------------------------------
void init() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // dark blue background
}

// ------------------------------------------------------------
// Main entry point
// ------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Animation – Interactive Demo");

    init();

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
    return 0;
}