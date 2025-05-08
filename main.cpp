// Para compilar a linha abaixo no terminal
//  g++ -o main.exe main.cpp -I"./include" -L"./lib/x64" -lfreeglut -lopengl32 -lglu32
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

GLfloat ang1 = 0, ang2 = 0, ang3 = 0;
GLfloat win = 25.0f;
GLfloat tx = 0;

float xComponentsSize = 2.5f;
float yComponentsSize = 2.5f;

// Estrutura para representar um segmento do braço robótico 
typedef struct { 
    float largura;          // Largura do segmento 
    float altura;           // Altura do segmento 
    float anguloDeRotacao; // Ângulo de rotação
    float posicaoX;        // Posição onde está o braço
    float cor[3];           // Cor RGB do segmento 
} SegmentoRobo; 
 
// Estrutura para representar a garra 
typedef struct { 
    float abertura;         // Ângulo de abertura da garra 
    float altura;      // Comprimento da garra 
    float largura;          // Largura da garra 
    float cor[3];           // Cor RGB da garra 
} Garra;

SegmentoRobo braco;
SegmentoRobo anteBraco;
Garra garra;

// Desenha uma de duas partes da garra
void desenharSegmentoDeGarra() {
    glLineWidth(5);
    glColor3f(garra.cor[0] , garra.cor[1], garra.cor[2]);
    glBegin(GL_POLYGON);
		glVertex2f(-garra.largura/2, 0);
		glVertex2f(garra.largura/2, 0);
		glVertex2f(garra.largura/2, garra.altura);
        glVertex2f(-garra.largura/2, garra.altura);
	glEnd();
}

// Desenha o braço
void desenharBraco() {
    glLineWidth(5);
    glBegin(GL_POLYGON);
		glVertex2f(-braco.largura/2, 0);
		glVertex2f(braco.largura/2, 0);
		glVertex2f(braco.largura/2, braco.altura);
        glVertex2f(-braco.largura/2, braco.altura);
	glEnd();
}

// Desenha o antebraço
void desenharAnteBraco() {
    glColor3f(anteBraco.cor[0] , anteBraco.cor[1], anteBraco.cor[2]);
    glLineWidth(5);
    glBegin(GL_POLYGON);
		glVertex2f(-anteBraco.largura/2, 0);
		glVertex2f(anteBraco.largura/2, 0);
		glVertex2f(anteBraco.largura/2, anteBraco.altura);
        glVertex2f(-anteBraco.largura/2, anteBraco.altura);
	glEnd();
}

// Desenha o chão
void desenharChao() {
    glColor3f(0.5f,0.5f,0.5f);
	glBegin(GL_LINES);
		glVertex2f(-win,0);
		glVertex2f(win,0);
	glEnd();
}

// Inicialização
void inicializa () {
    // Cor do limpa tela
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Eixos
    gluOrtho2D (-25.0f, 25.0f, -25.0f, 25.0f);
}

// Desenha o braço, antebraço e garra. Utiliza glPushMatrix() e glPopMatrix() para o desenho da garra
void display() {
    // Define a matriz de transformação
	glMatrixMode(GL_MODELVIEW);
	// Reseta matriz
	glLoadIdentity();
    gluOrtho2D (-25.0f, 25.0f, -25.0f, 25.0f);

    // Limpa tela
    glClear(GL_COLOR_BUFFER_BIT);

    glLineWidth(1);
    desenharChao();

    glColor3f(braco.cor[0], braco.cor[1], braco.cor[2]);
    glScalef(xComponentsSize, yComponentsSize,1.0f);

    glTranslatef(braco.posicaoX, 0.0f, 0.0f);
    glRotatef(braco.anguloDeRotacao, 0.0f, 0.0f, 1.0f);
    desenharBraco();

    glTranslatef(0.0f, braco.altura, 0.0f);
    glRotatef(anteBraco.anguloDeRotacao, 0.0f, 0.0f, 1.0f);
    desenharAnteBraco();

    glPushMatrix();

    glTranslatef(-(braco.largura)/2, braco.altura, 0.0f);
    glRotatef(garra.abertura, 0.0f, 0.0f, 1.0f);
    desenharSegmentoDeGarra();

    glPopMatrix();
    glTranslatef((braco.largura)/2, braco.altura, 0.0f);
    glRotatef(-garra.abertura, 0.0f, 0.0f, 1.0f);
    desenharSegmentoDeGarra();

    glFlush();
}

// Funções do teclado
void teclado(unsigned char key, int x, int y) {
    if (key == 'a') {
        braco.posicaoX -= 0.5;
    } else if (key == 'd') {
        braco.posicaoX += 0.5;
    } else if (key == 'e') {
        braco.anguloDeRotacao -= 2;
    } else if (key == 'q') {
        braco.anguloDeRotacao += 2;
    } else if (key == 'r') {
        anteBraco.anguloDeRotacao += 2;
    } else if (key == 't') {
        anteBraco.anguloDeRotacao -= 2;
    }  else if (key == 'y') {
        garra.abertura -= 2;
    }  else if (key == 'u') {
        garra.abertura += 2;
    } 

    glutPostRedisplay();
}

// Funções do click do mause, mudar a cor para click direito e esquerdo
void clickMouse(int button, int state, int x, int y) {

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        xComponentsSize += 0.1;
        yComponentsSize += 0.1;

    } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { 
        xComponentsSize -= 0.1;
        yComponentsSize -= 0.1;

    } else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        braco.cor[0] = (rand() % 256)/255.0;
        braco.cor[1] = (rand() % 256)/255.0;
        braco.cor[2] = (rand() % 256)/255.0;

        anteBraco.cor[0] = (rand() % 256)/255.0;
        anteBraco.cor[1] = (rand() % 256)/255.0;
        anteBraco.cor[2] = (rand() % 256)/255.0;
        
        garra.cor[0] = (rand() % 256)/255.0;
        garra.cor[1] = (rand() % 256)/255.0;
        garra.cor[2] = (rand() % 256)/255.0;
    }
}

// Função principal
int main(int argc, char** argv) {
    printf("-- Acoes --\nbraco: \n'a' ir para a esquerda\n'd' ir para a direita\n'e' rotacionar para direita\n'q' rotacionar para a esquerda\n\n");
    printf("Antebraco: \n'r' rotacionar para a esquerda\n't' rotacionar para a direita\n\n");
    printf("Garra: \n'y' fechar garra\n'u' abrir garra\n\n");
    printf("Mouse: \nclick direito diminui a escala dos componentes e click esquedo aumenta\nclick do meio muda a cor dos segmentos");

    // Iniciando variáveis das partes do braço
    braco.posicaoX = tx;
    braco.anguloDeRotacao = 0;
    braco.altura = 2;
    braco.largura = 1;
    braco.cor[0] = 0.36f;    
    braco.cor[1] = 0.2f;
    braco.cor[2] = 0.09f;

    anteBraco.anguloDeRotacao = 0;
    anteBraco.altura = 2;
    anteBraco.largura = 1;
    anteBraco.cor[0] = 0.0f;
    anteBraco.cor[1] = 0.0f;
    anteBraco.cor[2] = 1.0f;

    garra.abertura = 0;
    garra.altura = 0.7;
    garra.largura = 0.3;
    garra.cor[0] = 0.5f; 
    garra.cor[1] = 0.5f;
    garra.cor[2] = 0.0f;

    // Para rand no mouse
    srand(time(NULL));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(5,5);     
    glutInitWindowSize(500, 500);
    glutCreateWindow("Hello OpenGL");

    // Desenhar os segmentos do braco
    glutDisplayFunc(display);
    
    // Funcoes do teclado
    glutKeyboardFunc(teclado);

    // Inicializar
    inicializa();

    // Funcoes do mouse
    glutMouseFunc(clickMouse);

    glutMainLoop();
    return 0;
}