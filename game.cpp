#include <GL/glut.h>
#include <cstring>


#define MATRIX_ROWS 5
#define MATRIX_COLS 6
#define RECT_WIDTH 1.45f
#define RECT_HEIGHT 0.75f

struct Retangle{
    float x;
    float y;
    bool active;
} matrix[MATRIX_ROWS][MATRIX_COLS];

int victory = 0;

float width = 0.0f;
float ballX = 5.5f;
float ballY = 0.75f;
float ballRadius = 0.1f;
float ballSpeedX = 0.002f;
float ballSpeedY = 0.002f;
float ballSpeedXAux = 0.002f;
float ballSpeedYAux = 0.002f;
int gameOver = 0;
int menu = 1;

void display();
void line(float, float, float, float, float);
void bar();
void ball();
void updateBall();
void drawMatrix();
void removeRectangle(int row, int col);
int checkMatrix(int, int);
int checkChangeSide(int, int);
void gameOverScreen();
void menuScreen();
void keyboard(unsigned char key, int x, int y);
void victoryScreen();
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 600);
    glutCreateWindow("Destruidor de Blocos");
    glEnable(GL_DEPTH_TEST);
    glutKeyboardFunc(keyboard);
    // Função chamada quando a janela está ociosa

    // Inicializa a matriz de retângulos
    for (int i = 0; i < MATRIX_ROWS; i++)
    {
        for (int j = 0; j < MATRIX_COLS; j++)
        {
            matrix[i][j].x = j * (RECT_WIDTH + 0.2f) + 0.2f;
            matrix[i][j].y = 10.0f - (i + 1) * (RECT_HEIGHT + 0.2f) - 0.2f;
            matrix[i][j].active = true;
        }
    }
    glutIdleFunc(updateBall);
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}

void display()
{
    glClearColor(0.0, 0.20, 0.40, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glOrtho(0.0, 10.0, 0.0, 10.0, -10.0, 10.0);
    line(5.0, 0.0, 0.0, 0.0, 10.0);
    line(5.0, 0.0, 10.0, 10.0, 10.0);
    line(5.0, 10.0, 10.0, 10.0, 0.0);
    line(5.0, 10.0, 0.0, 0.0, 0.0);

    bar();
    ball();
    drawMatrix();

    if (gameOver) {
        glDisable(GL_DEPTH_TEST);
        gameOverScreen();
        glEnable(GL_DEPTH_TEST);
    }

    if(menu > 0) {
        glDisable(GL_DEPTH_TEST);
        menuScreen();
        ballSpeedX = ballSpeedXAux;
        ballSpeedY = ballSpeedYAux;
        glEnable(GL_DEPTH_TEST);

    }
    if (victory) {
        glDisable(GL_DEPTH_TEST);
        victoryScreen();
        glEnable(GL_DEPTH_TEST);
    }

    ballSpeedXAux = ballSpeedX;
    ballSpeedYAux = ballSpeedY;

    glFlush();
    glutSwapBuffers();
}


void line(float width, float initialX, float initialY, float finalX, float finalY)
{
    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2f(initialX, initialY);
    glVertex2f(finalX, finalY);
    glEnd();
}

void bar()
{
    glPushMatrix();
    glBegin(GL_QUADS);
    glColor3f(0.541176,  0.603922, 0.74902);
    glVertex2f(width + 4, 0.50);
    glVertex2f(width + 6, 0.50);
    glVertex2f(width + 6, 0.25);
    glVertex2f(width + 4, 0.25);
    glEnd();
    glPopMatrix();
}

void ball()
{
    glPushMatrix();
    glTranslatef(ballX, ballY, 0.0f);
    glColor3f(1.0  ,0.01, 0.74902);
    glutSolidSphere(ballRadius, 20, 20);
    glPopMatrix();
}

void updateBall()
{
    if (menu > 0 || victory) {
        return;
    }
    // Atualiza a posição da bola
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    int remainingRectangles = 0;
    for (int i = 0; i < MATRIX_ROWS; i++) {
        for (int j = 0; j < MATRIX_COLS; j++) {
            if (matrix[i][j].active) {
                remainingRectangles++;
            }
        }
    }
    if (remainingRectangles == 0) {
        victory = 1;
    }

    // Verifica colisão com o retângulo da barra
    if (ballX + ballRadius >= width + 4.0f && ballX - ballRadius <= width + 6.0f &&
        ballY + ballRadius >= 0.25f && ballY - ballRadius <= 0.50f)
    {
        ballSpeedY = -ballSpeedY;

        while (ballX + ballRadius >= width + 4.0f && ballX - ballRadius <= width + 6.0f &&
            ballY + ballRadius >= 0.25f && ballY - ballRadius <= 0.50f)
        {
            ballX += ballSpeedX;
            ballY += ballSpeedY;
        }
        // Inverte a direção vertical da bola ao colidir com o retângulo da barra
    }

    if (ballY >= 5) {
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 6; j++) {
                if (checkMatrix(i, j)) {
                    removeRectangle(i, j);

                    // Verifica a colisão com a lateral esquerda do retângulo
                    if (ballX + ballRadius >= matrix[i][j].x && ballX - ballRadius <= matrix[i][j].x) {
                        ballSpeedX = -ballSpeedX;  // Inverte a direção horizontal da bola
                    }
                    // Verifica a colisão com a lateral direita do retângulo
                    else if (ballX - ballRadius <= matrix[i][j].x + RECT_WIDTH && ballX + ballRadius >= matrix[i][j].x + RECT_WIDTH) {
                        ballSpeedX = -ballSpeedX;  // Inverte a direção horizontal da bola
                    }
                    if (ballY + ballRadius >= matrix[i][j].y && ballY - ballRadius <= matrix[i][j].y) {
                        ballSpeedY = -ballSpeedY;  // Inverte a direção horizontal da bola
                    }
                    // Verifica a colisão com a lateral direita do retângulo
                    else if (ballY - ballRadius <= matrix[i][j].y + RECT_WIDTH && ballY + ballRadius >= matrix[i][j].y + RECT_WIDTH) {
                        ballSpeedY = -ballSpeedY;  // Inverte a direção horizontal da bola
                    }

                }
            }
        }
    }

    // Verifica colisão com as bordas da janela
    if (ballX + ballRadius >= 10.0f || ballX - ballRadius <= 0.0f) {
        // Inverte a direção horizontal da bola ao colidir com as bordas laterais
        ballSpeedX = -ballSpeedX;
    }
    if (ballY + ballRadius >= 10.0f) {
        // Inverte a direção vertical da bola ao colidir com as bordas superior e inferior
        ballSpeedY = -ballSpeedY;
    }

    if (ballY - ballRadius <= 0.0f) {
        ballSpeedX = 0.0;
        ballSpeedY = 0.0;
        gameOver = 1;
    }

    glutPostRedisplay(); // Solicita a atualização da tela
}

int checkMatrix(int i, int j) {
    if (
        matrix[i][j].active &&
        ballX >= matrix[i][j].x && ballX <= matrix[i][j].x + RECT_WIDTH &&
        ballY >= matrix[i][j].y && ballY <= matrix[i][j].y + RECT_HEIGHT
    ) {
        return 1;
    }
    return 0;
}

void drawMatrix()
{
    for (int i = 0; i < MATRIX_ROWS; i++)
    {
        for (int j = 0; j < MATRIX_COLS; j++)
        {
            if (matrix[i][j].active)
            {
                glPushMatrix();
                glTranslatef(matrix[i][j].x, matrix[i][j].y, 0.0f);
                glColor3f(0.0, 0.603922, 0.2);
                glBegin(GL_QUADS);
                glVertex2f(0.0f, 0.0f);
                glVertex2f(RECT_WIDTH, 0.0f);
                glVertex2f(RECT_WIDTH, RECT_HEIGHT);
                glVertex2f(0.0f, RECT_HEIGHT);
                glEnd();
                glPopMatrix();
            }
        }
    }
}

void removeRectangle(int row, int col)
{
    matrix[row][col].active = false;
}

void gameOverScreen()
{
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(2, 7);
    glVertex2f(8, 7);
    glVertex2f(8, 3);
    glVertex2f(2, 3);
    glEnd();

    glPushMatrix();
    glTranslatef(4.5f, 4.5f, 0.0f);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0f, 0.0f);
    const char* text = "Game Over";
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    glPopMatrix();
}

void victoryScreen()
{
    glBegin(GL_QUADS);
    glColor3f(0.5, 1.0, 1.0);
    glVertex2f(2, 7);
    glVertex2f(8, 7);
    glVertex2f(8, 3);
    glVertex2f(2, 3);
    glEnd();

    glPushMatrix();
    glTranslatef(3.5f, 4.5f, 0.0f);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0f, 0.0f);
    const char* text = "Vitoria!";
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    glPopMatrix();
}



void menuScreen()
{
    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(2, 8);
    glVertex2f(8, 8);
    glVertex2f(8, 2);
    glVertex2f(2, 2);
    glEnd();

    glPushMatrix();
    glTranslatef(3.5f, 6.5f, 0.0f);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0f, 0.0f);
    const char* text = "Menu";
    for (int i = 0; i < strlen(text); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.5f, 5.5f, 0.0f);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0f, 0.0f);
    const char* text2 = "- 'm' para menu";
    for (int i = 0; i < strlen(text2); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text2[i]);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.5f, 4.5f, 0.0f);
    glColor3f(0.0, 0.0, 0.0);
    glRasterPos2f(-1.0f, 0.0f);
    const char* text3 = "- 'n' novo jogo";
    for (int i = 0; i < strlen(text3); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text3[i]);
    }
    glPopMatrix();
}


void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'd':
            width += 0.6f; // Incrementa a largura
            break;
        case 'a':
            width -= 0.6f; // Decrementa a largura
            break;
        case 'm':
            ballSpeedX = ballSpeedXAux;
            ballSpeedY = ballSpeedYAux;
            menu *= -1;
            break;
        case 'n':
            if (menu > 0) {
                for (int i = 0; i < MATRIX_ROWS; i++) {
                    for (int j = 0; j < MATRIX_COLS; j++) {
                        matrix[i][j].active = true;
                    }
                }
                gameOver = 0;
                ballSpeedX = 0.002f;
                ballSpeedY = 0.002f;
                ballSpeedXAux = 0.002f;
                ballSpeedYAux = 0.002f;
                ballX = 5.5f;
                ballY = 0.75f;
            }
            break;
    }

    glutPostRedisplay(); // Solicita a atualização da tela
}
