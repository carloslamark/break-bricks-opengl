#include <GL/glut.h>

#define MATRIX_ROWS 5
#define MATRIX_COLS 6
#define RECT_WIDTH 1.45f
#define RECT_HEIGHT 0.75f

struct Retangle{
    float x;
    float y;
    bool active;
} matrix[MATRIX_ROWS][MATRIX_COLS];



float width = 0.0f;
float ballX = 5.5f;
float ballY = 0.75f;
float ballRadius = 0.25f;
float ballSpeedX = 0.001f;
float ballSpeedY = 0.001f;

void display();
void line(float, float, float, float, float);
void bar();
void ball();
void updateBall();
void drawMatrix();
void removeRectangle(int row, int col);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(400, 600);
    glutCreateWindow("Destruidor de Blocos");
    glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(updateBall); // Função chamada quando a janela está ociosa

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

    glFlush();
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
    glColor3f(0.541176,  0.603922, 0.74902);
    glutSolidSphere(ballRadius, 20, 20);
    glPopMatrix();
}

void updateBall()
{
    // Atualiza a posição da bola
    ballX += ballSpeedX;
    ballY += ballSpeedY;

    // Verifica colisão com o retângulo da barra
    if (ballX + ballRadius >= width + 4.0f && ballX - ballRadius <= width + 6.0f &&
        ballY + ballRadius >= 0.25f && ballY - ballRadius <= 0.50f)
    {
        // Inverte a direção vertical da bola ao colidir com o retângulo da barra
        ballSpeedY = -ballSpeedY;
    }

    // Verifica colisão com os retângulos da matriz
    int col = static_cast<int>((ballX - (width + 4.0f)) / RECT_WIDTH);
    int row = MATRIX_ROWS - 1;
    if (row >= 0 && row < MATRIX_ROWS && col >= 0 && col < MATRIX_COLS && matrix[row][col].active)
    {
        // Inverte a direção vertical da bola ao colidir com um retângulo da matriz
        ballSpeedY = -ballSpeedY;

        // Remove o retângulo da matriz
        removeRectangle(row, col);
    }
    else
    {
        // Verifica colisão com os retângulos superiores da matriz
        for (int i = 0; i < MATRIX_ROWS; i++)
        {
            for (int j = 0; j < MATRIX_COLS; j++)
            {
                if (matrix[i][j].active)
                {
                    if (ballX + ballRadius >= matrix[i][j].x && ballX - ballRadius <= matrix[i][j].x + RECT_WIDTH &&
                        ballY + ballRadius >= matrix[i][j].y && ballY - ballRadius <= matrix[i][j].y + RECT_HEIGHT)
                    {
                        // Inverte a direção vertical da bola ao colidir com um retângulo da matriz
                        ballSpeedY = -ballSpeedY;

                        // Remove o retângulo da matriz
                        removeRectangle(i, j);
                        return; // Encerra a função após a colisão para evitar colisões múltiplas
                    }
                }
            }
        }
    }

    // Verifica colisão com as bordas da janela
    if (ballX + ballRadius >= 10.0f || ballX - ballRadius <= 0.0f)
    {
        // Inverte a direção horizontal da bola ao colidir com as bordas laterais
        ballSpeedX = -ballSpeedX;
    }
    if (ballY + ballRadius >= 10.0f || ballY - ballRadius <= 0.0f)
    {
        // Inverte a direção vertical da bola ao colidir com as bordas superior e inferior
        ballSpeedY = -ballSpeedY;
    }

    glutPostRedisplay(); // Solicita a atualização da tela
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
                glColor3f(0.541176, 0.603922, 0.74902);
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
    if (row >= 0 && row < MATRIX_ROWS && col >= 0 && col < MATRIX_COLS)
    {
        matrix[row][col].active = false;
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'd':
        width += 0.2f; // Incrementa a largura
        break;
    case 'a':
        width -= 0.2f; // Decrementa a largura
        break;
    }

    glutPostRedisplay(); // Solicita a atualização da tela
}
