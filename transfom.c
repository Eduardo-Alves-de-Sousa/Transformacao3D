#include <GL/glut.h>
#include <stdlib.h>
#include <GL/glu.h>

GLfloat angle, fAspect;
GLfloat rotaX, rotaY, rotaX_init, rotaY_init;
GLfloat obsX, obsY, obsZ, obsX_init, obsY_init, obsZ_ini;

int x_init, y_init, bot;

void Desenhar()
{
	// Limpa a janela e o depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Cor do objeto
	glColor3f(0.0f, 0.0f, 1.0f);

	// Desenhar o tamanho do objeto
	glutSolidTeapot(30.0f);

	glutSwapBuffers();

	glRotatef(60.0,0.0,0.0,1.0);

}
// Função usada para especificar a posicao do observador virtual
void PosicionaObservador()
{
    //Sistema de coordenadas
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Posiciona e orienta o observador
    glTranslatef(-obsX, -obsY, -obsZ);
    glRotatef(rotaX, 1, 0, 0);
    glRotatef(rotaY, 0, 1, 0);
}

// Inicializar parâmetros de rendering
void Inicializar(void)
{
    GLfloat luzAmbiente[4]={0.2,0.2,0.2,1.0}; 
	GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	// "cor" 
	GLfloat luzEspecular[4]={1.0, 1.0, 1.0, 1.0}; // "brilho" 
	GLfloat posicaoLuz[4]={0.0, 50.0, 50.0, 1.0};

	// Capacidade de brilho do material
	GLfloat especularidade[4]={1.0,1.0,1.0,1.0}; 
	GLint especMaterial = 60;

 	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	// Habilita o modelo de colorização
	glShadeModel(GL_SMOOTH);

	// Define a refletância do material 
	glMaterialfv(GL_FRONT,GL_SPECULAR, especularidade);
	// Define a concentração do brilho
	glMateriali(GL_FRONT,GL_SHININESS,especMaterial);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz 
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular );
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

	// Habilita a definição da cor do material a partir da cor corrente
	glEnable(GL_COLOR_MATERIAL);
	//Habilita o uso de iluminação
	glEnable(GL_LIGHTING);  
	// Habilita a luz 
	glEnable(GL_LIGHT0);
	// Habilita o depth-buffering
	glEnable(GL_DEPTH_TEST);

	angle=45;
    rotaX = 0;
    rotaY = 0;
    obsX = obsY = 0;
    obsZ = 250;
}
// Função usada para especificar o volume de visualização
void EspecificarParametrosVisualização(void)
{
    // Especifica sistema de coordenadas de projecao
    glMatrixMode(GL_PROJECTION);
    // Inicializar sistema de coordenadas de projecao
    glLoadIdentity();
    // Especifica a projecao perspectiva
    // (angulo, aspecto, zMin, zMax)
    gluPerspective(angle, fAspect, 0.5, 1000);
    PosicionaObservador();
}
// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void Teclado(unsigned char teclas, int x, int y)
{
    if (teclas == 27) // ESC
        exit(0);
    if (teclas == 'p')
    {
        glEnable(GL_DEPTH_TEST);
        glutPostRedisplay();
    }
    if (teclas == 'P')
    {
        glDisable(GL_DEPTH_TEST);
        glutPostRedisplay();
    }
}
void TeclasEspeciais(int teclas, int x, int y)
{
    switch (teclas)
    {
	//FUnção de Escala
    case GLUT_KEY_HOME:
        if (angle >= 10)
            angle -= 5;
        break;
    case GLUT_KEY_END:
        if (angle <= 150)
            angle += 5;
        break;

	//Função para Transladar
	case GLUT_KEY_LEFT:
	if(angle >=30){
		obsX ++;
		obsY ++;
	}       
        break;
	case GLUT_KEY_RIGHT:
	if(angle >=30){
		obsX --;
		obsY --;
	}   
        break;
	
	//Funções para Rotacionar
	case GLUT_KEY_INSERT:
		rotaX ++;
		rotaY ++;
		break;
	case GLUT_KEY_F8:
		rotaX --;
		rotaY --;
		break;
    }
    EspecificarParametrosVisualização();
    glutPostRedisplay();
}
// Função callback para eventos de botoes do mouse
void GerenciarMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        x_init = x;
        y_init = y;
        obsX_init = obsX;
        obsY_init = obsY;
        obsZ_ini = obsZ;
        rotaX_init = rotaX;
        rotaY_init = rotaY;
        bot = button;
    }
    else
        bot = -1;
}
#define SENS_ROT 5.0
#define SENS_OBS 10.0
#define SENS_TRANSL 10.0
void GerenciaMovimentos(int x, int y)
{
    if (bot == GLUT_LEFT_BUTTON)
    { 
        int deltax = x_init - x;
        int deltay = y_init - y;
        rotaY = rotaY_init - deltax / SENS_ROT;
        rotaX = rotaX_init - deltay / SENS_ROT;
    }
    else if (bot == GLUT_RIGHT_BUTTON) 
    {
        int deltaz = y_init - y;

        obsZ = obsZ_ini + deltaz / SENS_OBS;
    }

    else if (bot == GLUT_MIDDLE_BUTTON) 

    {
        int deltax = x_init - x;
        int deltay = y_init - y;

        obsX = obsX_init + deltax / SENS_TRANSL;
        obsY = obsY_init - deltay / SENS_TRANSL;
    }
    PosicionaObservador();
    glutPostRedisplay();
}
//Função de redimensionamento da janela de visualização
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
    if (h == 0)
        h = 1;
    // Especifica as dimensoes da viewport
    glViewport(0, 0, w, h);
    // Calcula a correção de aspecto
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificarParametrosVisualização();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(450, 450);
    glutCreateWindow("TRANSFORMACOES 3D");
    glutDisplayFunc(Desenhar);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(TeclasEspeciais);
    glutMouseFunc(GerenciarMouse);
    glutMotionFunc(GerenciaMovimentos);
    Inicializar();
    glutMainLoop();
}