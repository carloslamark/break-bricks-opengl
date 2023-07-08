#include <GL/glut.h>

// Manual:
//     Mover ombro-> teclas 'w' e 's'
//     Mover braço-> teclas 'q' e 'e'
//     Mover antebraço-> teclas 'a' e 'd'
//     Mover dedos-> teclas 'z' e 'c'

float width = 0.0f;
float armRotation = 0.0f;
float foreArmRotation = 0.0f;
float handRotation = 0.0f;

void display();
void line(float, float, float, float, float);
void bar();

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
    glutMainLoop();
    return 0;
}

void display()
{
    glClearColor(0.0, 0.20, 0.40, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glOrtho(0.0, 10.0, 0.0, 10.0, -10.0, 10.0);
    // 10.0 -> 100%
    // 0.0 -> 0%
    line(5.0, 0.0, 0.0, 0.0, 10.0);
    line(5.0, 0.0, 10.0, 10.0, 10.0);
    line(5.0, 10.0, 10.0, 10.0, 0.0);
    line(5.0, 10.0, 0.0, 0.0, 0.0);

    bar();

    glFlush();
}

void line(float width, float initialX, float initialY, float finalX, float finalY)
{
    // Define a largura da linha para 3 pixels
    glLineWidth(width);

    glBegin(GL_LINES);
    // Define os pontos inicial e final da linha
    glVertex2f(initialX, initialY);  // Ponto inicial
    glVertex2f(finalX, finalY);  // Ponto final
    glEnd();

}

void bar()
{
    glPushMatrix();

    glBegin(GL_QUADS);
    glColor3f(0.541176,  0.603922, 0.74902);
    glVertex2f(width + 4,0.50);
    glVertex2f(width + 6,0.50);
    glVertex2f(width + 6,0.25);
    glVertex2f(width + 4,0.25);
    glEnd();
    glPopMatrix();
}


// void ombro()
// {
//     glPushMatrix();
//     glTranslatef(0.5, height + 5, 1.0);
//     glColor3f(0.1804, 0.2, 0.251);
//     glutSolidSphere(0.5, 20, 20);
//     glPopMatrix();
// }

// void primeiroBraco()
// {
//     glPushMatrix();
//     glTranslatef(0.0, height, 0.0);

//     glTranslatef(0.5, +5.0, 0.0);
//     glRotatef(armRotation, 0.0, 0.0, 1.0);
//     glTranslatef(0.0, -5.0, 0.0);

//     glTranslatef(0.0, -height, 0.0);

//     glBegin(GL_QUADS);
//     glColor3f(0.541176,  0.603922, 0.74902);
//     glVertex2f(0.5, height + 5.25);
//     glVertex2f(3.5, height + 5.25);
//     glVertex2f(3.5, height + 4.75);
//     glVertex2f(0.5, height + 4.75);
//     glEnd();
//     glPopMatrix();
// }

// void cotovelo()
// {
//     glPushMatrix();
//     glTranslatef(0.0, height + 5, 0.0);

//     glTranslatef(0.5, 0.0, 0.0);
//     glRotatef(armRotation, 0.0, 0.0, 1.0);
//     glTranslatef(4.0, 0.0, 0.0);


//     glColor3f(0.1804, 0.2, 0.251);
//     glutSolidSphere(0.5, 20, 20);
//     glPopMatrix();
// }

// void antebraco()
// {
//     glPushMatrix();
//     glTranslatef(0.0, +height, 0.0);

//     glTranslatef(0.5, +5.0, 0.0);
//     glRotatef(armRotation, 0.0, 0.0, 1.0);
//     glTranslatef(1.0, -5.0, 0.0);

//     glTranslatef(3.0, +5.0, 0.0);
//     glRotatef(foreArmRotation, 0.0, 0.0, 1.0);
//     glTranslatef(-3.0, -5.0, 0.0);

//     glBegin(GL_QUADS);
//     glColor3f(0.541176,  0.603922, 0.74902);
//     glVertex2f(3.5, 5.25);
//     glVertex2f(6.5, 5.25);
//     glVertex2f(6.5, 4.75);
//     glVertex2f(3.5, 4.75);
//     glEnd();
//     glPopMatrix();
// }

// void pulso()
// {
//     glPushMatrix();
//     glTranslatef(0.0, height + 5.0, 0.0);

//     glTranslatef(0.5, 0.0, 0.0);
//     glRotatef(armRotation, 0.0, 0.0, 1.0);
//     glTranslatef(8.0, 0.0, 0.0);

//     glTranslatef(-4.0, 0.0, 0.0);
//     glRotatef(foreArmRotation, 0.0, 0.0, 1.0);
//     glTranslatef(+4.0, 0.0, 0.0);

//     glColor3f(0.1804, 0.2, 0.251);
//     glutSolidSphere(0.5, 20, 20);
//     glPopMatrix();
// }

// void mao()
// {
//     dedo(45);
//     dedo(0);
//     dedo(315);
// }

// void dedo(int ang)
// {
//     glPushMatrix();

//     glTranslatef(0.0, +height, 0.0);

//     glTranslatef(0.5, +5.0, 0.0);
//     glRotatef(armRotation, 0.0, 0.0, 1.0);
//     glTranslatef(1.0, -5.0, 0.0);

//     glTranslatef(3.0, +5.0, 0.0);
//     glRotatef(foreArmRotation, 0.0, 0.0, 1.0);
//     glTranslatef(-3.0, -5.0, 0.0);

//     glTranslatef(7.0, +5.0, 0.0);
//     glRotatef(ang, 0.0, 0.0, 1.0);
//     glTranslatef(-7.0, -5.0, 0.0);

//     glTranslatef(7.0, +5.0, 0.0);
//     glRotatef(handRotation, 0.0, 0.0, 1.0);
//     glTranslatef(-7.0, -5.0, 0.0);

//     glBegin(GL_QUADS);
//     glColor3f(0.541176,  0.603922, 0.74902);
//     glVertex2f(7.5, 5.125);
//     glVertex2f(8.0, 5.125);
//     glVertex2f(8.0, 4.875);
//     glVertex2f(7.5, 4.875);
//     glEnd();
//     glPopMatrix();
// }

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


