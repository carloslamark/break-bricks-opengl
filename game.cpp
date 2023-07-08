#include <GL/glut.h>

float width = 0.0f;
float armRotation = 0.0f;
float foreArmRotation = 0.0f;
float handRotation = 0.0f;

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

void ombro();
void primeiroBraco();
void cotovelo();
void antebraco();
void pulso();
void mao();
void dedo(int ang);
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

    // Verifica colisão com o retângulo
    if (ballX + ballRadius >= width + 4.0f && ballX - ballRadius <= width + 6.0f &&
        ballY + ballRadius >= 0.25f && ballY - ballRadius <= 0.50f)
    {
        // Inverte a direção vertical da bola ao colidir com o retângulo
        ballSpeedY = -ballSpeedY;
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
