#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>


#define ANDANDO 1
#define AGACHANDO 2
#define LEVANTANDO 3
#define PEGANDO 4

#define ANDARX 1
#define AGACHAR 2
#define PEGAR 3
#define LEVANTAR 4
#define LEVARPRABOCA 5
#define RESETAR 6
#define ANDARY 7


///Menu
int ACMENU = 0;


///Fila
#define MAX 5

typedef struct{
    float u;
    float v;
} posicao;

posicao dado[MAX];
int topo;
int op;
///Fim

///Textura
#define MAX_TEXTURAS 7

typedef struct BMPImagem{
    int   width;
    int   height;
    char *data;
}BMPImage;

GLuint texture_id[MAX_TEXTURAS]; /* identificador da textura */
///Fim

GLfloat angle=65.0, ang = 20.0, angY = 0.0, angZ = 0.0;
GLfloat fAspect;
GLdouble obsX=0, obsY=0, obsZ=200;

float ox = 6.0, oy = 6.0, oz = 7.0;

int ww=900, wh= 500;///x=900, y=650;
float dx, dy;
int check = 0;
float horizontalHomer = 0.0, verticalHomer= 0.0;
int direcaoHomer = 0, pernaDirHomer = 0, pernaEsqHomer = 0;
int flag = -1;
int cabZ = 0; andD = 90, andE = 90, perE = 0, braD = 90, braE = 90, cutDX = 0, cutDY = 0, cutEY = 0, ombD = 0, ombDX = 0, ombDZ = 0,ombE = 0, ombEX = 0, ombEZ = 0;
int rotBoca = 0, incrBoca = 3;
int incremento=8, iD = 12, iE = 12;
int RotBD = 90;

int AnimaAuto = -2;

int TvOn = 0;

int contadorMouse = 0;

posicao local;
float xold=0.0, yold = 0.0;
int lado = 1;

int showRosq = FALSE;

int acao;

///Agachar
int pernaRot=0 ; ///pernaRot=-90, troncoRot=45
float troncoRot=0.0, cintTY= 0.0,cintTZ=0.0, troncoTrl=0.0, troncoTrlZ=0.0; ///cintTY= -10.0,cintTZ=-10.0, troncoTrl=-14.0


GLfloat ctrlpoints[4][4][3] = {
 {{-4.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {1.0, 0.0, 0.0},{4.0, 0.0, 0.0}},
 {{-4.0, 0.0, -2.0}, {-1.0, 4.0, -2.0}, {1.0, 4.0, -2.0}, {4.0, 0.0, -2.0}},
 {{-4.0, 0.0, -4.0}, {-1.0, 4.0, -4.0}, {1.0, 4.0, -4.0}, {4.0, 0.0, -4.0}},
 {{-4.0, 0.0, -6.0}, {-1.0, 0.0, -6.0}, {1.0, 0.0, -6.0}, {4.0, 0.0, -6.0}}
};

GLfloat points[5][5][3] = {
 {{-1.5, 0.0, 0.5}, {-1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.5, 0.0, 0.5}, {1.0, 0.0, 0.5}},
 {{-2.0, 0.0, 1.0}, {-1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {2.0, 0.0, 1.0}},
 {{-2.0, 0.0, 2.0}, {-1.0, 1.0, 2.0}, {0.0, 1.0, 2.0}, {1.0, 0.0, 2.0}, {2.0, 0.0, 2.0}},
 {{-1.5, 0.0, 2.5}, {-1.0, 0.0, 3.0}, {0.0, 0.0, 3.0}, {0.5, 0.0, 2.5}, {2.0, 0.0, 3.0}},
 {{-2.0, 0.0, 4.0}, {-1.0, 0.0, 4.0}, {0.0, 0.0, 4.0}, {1.0, 0.0, 4.0}, {2.0, 0.0, 4.0}}
};


GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0}; //White Color
GLfloat qaRed[] = {1.0, 0.0, 0.0, 1.0};

///Fila
void FilaInserir(posicao x){
    if (topo==MAX)
        return;

    dado[topo] = x;
    topo++;

}

posicao FilaRemover(posicao x[]){
    int i;
    posicao elemento;

    if (topo==0){///Fila Vazia
        elemento.u = elemento.v = 1000; //controle para saber q esta vaxia
        return elemento;
    }


    ///Há elementos na Fila
    elemento = dado[0];
    for (i=0; i<=topo-1; i++)
        dado[i] = x[i+1];

    topo--;;
    return elemento;
}
///Fim Fila


///Textura
void getBitmapImageData( char *pFileName, BMPImage *pImage ){
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
	int i;

    if( (pFile = fopen(pFileName, "rb") ) == NULL )
		printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);

    // Seek forward to width and height info
    fseek( pFile, 18, SEEK_CUR );

    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);

    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);

    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);

    if( nNumPlanes != 1 )
		printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);

    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
		printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);

    if( nNumBPP != 24 )
		printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);

    // Seek forward to image data
    fseek( pFile, 24, SEEK_CUR );

	// Calculate the image's total size in bytes. Note how we multiply the
	// result of (width * height) by 3. This is becuase a 24 bit color BMP
	// file will give you 3 bytes per pixel.
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
		printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);

    //
	// Finally, rearrange BGR to RGB
	//

	char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
	{
		charTemp = pImage->data[i];
		pImage->data[i] = pImage->data[i+2];
		pImage->data[i+2] = charTemp;
    }
}

void CarregaTextura(char* Filename, int id){
    BMPImage textura;

    getBitmapImageData( Filename, &textura);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture_id[id]);

    glBindTexture(GL_TEXTURE_2D, texture_id[id]);
    /*Define modo de preenchimento se textura for menor ou maior*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    /*Definicao dos parametros da textura*/
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);

}



///Iluminação
void Iluminacao(){///Superbible: listing 5.3 (pag. 204)
    GLfloat luzDifusa[4]={0.3,0.3,0.3,1.0}; // "cor"
    GLfloat luzAmbiente0[4]={0.5,0.5,0.5,1.0};
    GLfloat luzAmbiente1[4]={0.3,0.3,0.3,1.0};

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);

    glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa);

    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
}

void lampada1(float x, float y, float z){

//dene a posição e parâmetros da luz
    float position[] = {x, y, z, 1.0f};
    float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float direction[] = {0.0, -10.0f, 0.0};

    glLightfv(GL_LIGHT4, GL_POSITION, position);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT4, GL_SPECULAR, white);


    glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, direction); //vetor ←-direção
    glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 40.0f); //espalhamento angular
    glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 0.3f); //atenuação angular

//ativa a iluminação
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(x, y, z);
    glutSolidCube(4);
    glPopMatrix();
}

void lampada2(float x, float y, float z){

//dene a posição e parâmetros da luz
    float position[] = {x, y, z, 1.0f};
    float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float direction[] = {0.0, -10.0f, 0.0};

    glLightfv(GL_LIGHT2, GL_POSITION, position);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, white);


    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direction); //vetor ←-direção
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 50.0f); //espalhamento angular
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5f); //atenuação angular

//ativa a iluminação
    glEnable(GL_LIGHTING);

    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glTranslatef(x, y, z);
    glutSolidCube(4);
    glPopMatrix();
}

void IluminacaoTV(float x, float y, float z){
    //dene a posição e parâmetros da luz
    float position[] = {x, y, z, 1.0f};
    float white[] = {0.0f, 0.0f, 1.0f, 1.0f};
    float direction[] = {0.0, 0.0f, -30.0};

    glLightfv(GL_LIGHT3, GL_POSITION, position);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT3, GL_SPECULAR, white);


    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, direction); //vetor ←-direção
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 50.0f); //espalhamento angular
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.5f); //atenuação angular

//ativa a iluminação
    glEnable(GL_LIGHTING);

    glPushMatrix();
        glColor3f(1.0,1.0,1.0);
        glTranslatef(x, y, z);
        glutSolidCube(4);
    glPopMatrix();
}



///Objeto que aparece
void rosquinha(float x){
    glPushMatrix();
        glColor3f(0.8, 0.556, 0.262);
        glTranslatef(0,4.0,0.0);//glTranslatef(x,4.0,0.0);
        glRotatef(90, 1,0,0);
        glutSolidTorus(2, 3, 8, 15);
    glPopMatrix();
}

void caixa_rosq(posicao x){
    int i;

    glPushMatrix();
        glTranslatef(x.u,4.0,x.v);

        glColor3f(1,1,1);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture_id[6]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(11, 0, 10);// 0 0
            glTexCoord2f(1.0f, 0.0f); glVertex3f(11, 0, -10);//1 0
            glTexCoord2f(1.0f, 1.0f); glVertex3f(11, 10, -10);//1 1
            glTexCoord2f(0.0f, 1.0f); glVertex3f(11, 10, 10);//0 1
        glEnd();

        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-11, 0, -10);// 0 0
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-11, 0, 10);//1 0
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-11, 10, 10);//1 1
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-11, 10, -10);//0 1
        glEnd();

        glDisable(GL_TEXTURE_2D);

        glPushMatrix();//Baixo
            glColor3f(1,1,1);
            glScalef(1,0.05,1);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();//Lateral E
            glColor3f(1,1,1);
            glTranslatef(-10,5.0,0.0);
            glScalef(0.05,0.5,1);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();//Lateral D
            glColor3f(1,1,1);
            glTranslatef(10,5.0,0.0);
            glScalef(0.05,0.5,1);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();//frente
            glColor3f(1,1,1);
            glTranslatef(0,5.0,10.0);
            glScalef(1,0.5,0.05);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();//traz
            glColor3f(1,1,1);
            glTranslatef(0,5.0,-10.0);
            glScalef(1,0.5,0.05);
            glutSolidCube(20);
        glPopMatrix();

        for(i=-8; i<=8; i+=4){
            if(!(acao >=4 && i == 0 && acao !=7)){//sumir a rosquinha da caixa quando pegar ela
                glPushMatrix();//rosquinhas da direita
                    glColor3f(0.8, 0.556, 0.262);
                    glTranslatef(5,5.0,i);
                    glRotatef(0, 1,0,0);
                    glutSolidTorus(2, 3, 8, 15);
                glPopMatrix();
            }


            glPushMatrix();//rosquinhas da esquerda
                glColor3f(0.8, 0.556, 0.262);
                glTranslatef(-5,5.0,i);
                glRotatef(0, 1,0,0);
                glutSolidTorus(2, 3, 8, 15);
            glPopMatrix();
        }

    glPopMatrix();
}



///Estrutura da Sala
void chao(){
    glPushMatrix();
        glColor3f(0.0f, 0.636f, 0.62f);
        glScalef(2.0, -0.015, 1.0);
        glutSolidCube(200);
	glPopMatrix();
}

void paradeFrontal(int l){
    glPushMatrix();
        glColor3f(0.941, 0.603, 0.615);
        glTranslatef(0, -30, l*100);
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0, 100);
        glScalef(2.0, -0.015, 0.7);
        glutSolidCube(200);
	glPopMatrix();
}

void paredeLateral(int l){

    glPushMatrix();
        glColor3f(0.941, 0.603, 0.615);
        glTranslatef(l*200, -30, 0);
        glRotatef(-90, 0, 1, 0);
        glRotatef(-90, 1, 0, 0);
        glTranslatef(0, 0, 100);
        glScalef(1.0, -0.015, 0.7);
        glutSolidCube(200);
    glPopMatrix();
}


///Funçoes de apoio
void circulo(int x, int y, int raio){
    int i;
	int triangleAmount = 40; //# of triangles used to draw circle
	GLfloat twicePi = 2.0f * M_PI;



	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(
		            x + (raio * cos(i *  twicePi / triangleAmount)),
			    y + (raio * sin(i * twicePi / triangleAmount))
			);
		}
	glEnd();
}

void spline(){
    int i,j;
    glMapGrid2f(8, 0.0, 1.0, 8, 0.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 8, 0, 8);

//    glPointSize(5.0);
//    glBegin(GL_POINTS);
//        for(i=0; i<5; i++){
//            for(j=0; j<5; j++){
//                glColor3f(cores[i][0],cores[i][1],cores[i][2]);
//                glVertex3f(points[i][j][0], points[i][j][1], points[i][j][2]);
//            }
//        }
//    glEnd();
}

void circuloLinha(float radius){
    int i;
   glBegin(GL_LINE_LOOP);

   for (i=0; i <= 360; i++)
   {
      float degInRad = i*(M_PI/180);
      glVertex2f(cos(degInRad)*radius,sin(degInRad)*radius);
   }

   glEnd();
}


///Objetos
void tapete(){

    glPushMatrix();
        glTranslatef(0.0,3.0,0.0);
        glRotatef(-90, 1,0,0);
        glScalef(9.0,5.0,1.0);

        glPushMatrix();//Tapete 1 (Rosa)
            glColor3f(0.964f, 0.376f, 0.492f);
            circulo(2,-3, 9);
        glPopMatrix();

        glPushMatrix();//Tapete 2 (Roxo)
            glTranslatef(0.0,0.0,0.052);
            glColor3f(0.748, 0.208, 0.588);
            circulo(2,-3, 7);
        glPopMatrix();

        glPushMatrix();//Tapete 3 (Azul)
            glTranslatef(0.0,0.0,0.12);
            glColor3f(0.0, 0.532, 0.8);
            circulo(2,-3, 5);
        glPopMatrix();

        glPushMatrix();//Tapete 4 (Salmao)
            glTranslatef(0.0,0.0,0.2);
            glColor3f(0.88, 0.556, 0.304);
            circulo(2,-3, 3);
        glPopMatrix();
    glPopMatrix();
}

void sofa(){

    glPushMatrix();
        glColor3f(0.876f, 0.436f, 0.12f);
        glTranslatef(20,10.7,-71);

        glPushMatrix();///encosto
            glTranslatef(0,21,-21);
            glScalef(5.0,2.0,0.4);
            glutSolidCube(30);
        glPopMatrix();

        glPushMatrix();///braço esq
            glTranslatef(-68,10.5,0);
            glScalef(0.5,1.3,1.0);
            glutSolidCube(30);
        glPopMatrix();

        glPushMatrix();///braço dir
            glTranslatef(68,11,0);
            glScalef(0.5,1.3,1.0);
            glutSolidCube(30);
        glPopMatrix();

        glPushMatrix();///onde senta
            glTranslatef(0,-3,0);
            glScalef(4.0,0.4,1.0);
            glutSolidCube(30);
        glPopMatrix();

        glPushMatrix();///Almofada meio
            glTranslatef(0,7,0);
            glScalef(4.0,0.9,3.0);
            glutSolidCube(10);
        glPopMatrix();

        glPushMatrix();///Almofada dir
            glTranslatef(40,7,0);
            glScalef(3.9,0.9,3.0);
            glutSolidCube(10);
        glPopMatrix();

        glPushMatrix();///Almofada dir
            glTranslatef(-40,7,0);
            glScalef(3.9,0.9,3.0);
            glutSolidCube(10);
        glPopMatrix();



	glPopMatrix();
}

void criado_mudo(){
    glPushMatrix();

        glTranslatef(-80.0,6.0,-80.0);
        glColor3f(0.652, 0.208, 0.584);

        glPushMatrix();///Parte de trás
            glTranslatef(0.0,10.0,-13.0);
            glScalef(1.0, 1.2, 0.10);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();///Apoio Esquerdo
            glTranslatef(-12.0,10.0,0.0);
            glScalef(0.15, 1.0, 1.0);
            glutSolidCube(28);
        glPopMatrix();

        glPushMatrix();///Parte inferior
            glTranslatef(0.0,0.0,0.0);
            glScalef(1.0, 0.10, 1.4);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();///Apoio Direito
            glTranslatef(12.0,10.0,0.0);
            glScalef(0.15, 1.0, 1.0);
            glutSolidCube(28);
        glPopMatrix();



        glPushMatrix();///Parte com gaveta
            glTranslatef(0.0,28.0,0.0);
            glScalef(1.0, 0.45, 1.0);
            glutSolidCube(28);
        glPopMatrix();

        glPushMatrix();///Tampa de cima
            glTranslatef(0.0,35.0,0.0);
            glScalef(1.0, 0.10, 1.0);
            glutSolidCube(30);
        glPopMatrix();

        glPushMatrix();///Puxador gaveta
            glTranslatef(0.0,30.0,15.0);
            glutSolidSphere(2,30,30);
        glPopMatrix();

        glPushMatrix();///Gaveta
            glColor3f(0.0, 0.0,0.0);
            glTranslatef(-10.0,26.0,14.2);
             glBegin(GL_LINE_LOOP);
                   glVertex2i(0,7);
                   glVertex2i(0,0);
                   glVertex2i(20,0);
                   glVertex2i(20,7);
            glEnd();
        glPopMatrix();

        abajur_de_mesa();
        livros();


        glPushMatrix();///foto 1
            glColor3f(1, 1, 1);
            glTranslatef(5,38,10);
            glRotated(90,1,0,0);
            glRotated(-45,0,0,1);

            glPushMatrix();
                glColor3f(1,1,1);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_id[4]);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 1.0f); glVertex3f(10, -6.5, -2);// 0 0
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(-10, -6.5, -2);//1 0
                    glTexCoord2f(1.0f, 0.0f); glVertex3f(-10, 6.5, -2);//1 1
                    glTexCoord2f(1.0f, 1.0f); glVertex3f(10, 6.5, -2);//0 1
                glEnd();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            glScaled(4.0,2.5,0.7);
            glutSolidCube(5);
        glPopMatrix();



    glPopMatrix();

}

void estante(){
    glPushMatrix();

        glTranslatef(-200.0 ,0.0,28.0);
        glRotatef(-90, 0,1,0);
        glColor3f(0.652, 0.208, 0.584);

        glPushMatrix();///Apoio - frontal esq
                glTranslatef(-30.0, 0.0, -8.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 1.5, 1.5, 50, 30, 30);
        glPopMatrix();

        glPushMatrix();///Apoio - posterior esq
                glTranslatef(-30.0, 0.0, -18.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 1.5, 1.5, 50, 30, 30);
        glPopMatrix();

        glPushMatrix();///Apoio - frontal esq
                glTranslatef(30.0, 0.0, -8.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 1.5, 1.5, 50, 30, 30);
        glPopMatrix();

        glPushMatrix();///Apoio - frontal esq
                glTranslatef(30.0, 0.0, -18.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 1.5, 1.5, 50, 30, 30);
        glPopMatrix();


        glPushMatrix();///Cima
            glTranslatef(0.0,50.0,-13.0);
            glScalef(5.0, 0.10, 1.0);
            glutSolidCube(20);
        glPopMatrix();


        glPushMatrix();///Meio
            glTranslatef(0.0,30.0,-13.0);
            glScalef(5.0, 0.10, 1.0);
            glutSolidCube(20);
        glPopMatrix();

        glPushMatrix();///Embaixo
            glTranslatef(0.0,5.0,-13.0);
            glScalef(5.0, 0.10, 1.0);
            glutSolidCube(20);
        glPopMatrix();

        fotografia(30, 2);//1
        fotografia(-30, 3);//1


    glPopMatrix();

}

void fotografia (int x, int id){
    glPushMatrix();///foto 1
            glColor3f(1, 1, 1);
            glTranslatef(x,60,-20);

            glPushMatrix();
                glColor3f(1,1,1);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_id[id]);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(8, -7, -1.5);// 0 0
                    glTexCoord2f(1.0f, 0.0f); glVertex3f(-8, -7, -1.5);//1 0
                    glTexCoord2f(1.0f, 1.0f); glVertex3f(-8, 7, -1.5);//1 1
                    glTexCoord2f(0.0f, 1.0f); glVertex3f(8, 7, -1.5);//0 1
                glEnd();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(0,-3,3);
                glRotated(-45, 1,0,0);
                glScaled(1.0,3.0,0.4);
                glutSolidCube(3);
            glPopMatrix();

            glScaled(4.0,3.0,0.4);
            glutSolidCube(5);
        glPopMatrix();



}

void livros(){
    glPushMatrix();

        glTranslatef(0.0,0.0,20.0);

        glPushMatrix();///Livro Laranja
            glColor3f(0.824, 0.46, 0.24);
            glTranslatef(0.0,10.0,-13.0);
            glScalef(0.20, 1.0, 0.8);
            glutSolidCube(15);
        glPopMatrix();

        glPushMatrix();///Livro Verde
            glColor3f(0.288, 0.62, 0.3);
            glTranslatef(3.2,10.0,-13.0);
            glScalef(0.20, 1.0, 0.8);
            glutSolidCube(15);
        glPopMatrix();

        glPushMatrix();///Livro Vermelho
            glColor3f(0.736, 0.284, 0.176);
            glTranslatef(6.2,10.0,-13.0);
            glScalef(0.20, 1.0, 0.8);
            glutSolidCube(15);
        glPopMatrix();
    glPopMatrix();
}

void quadro(){
    glPushMatrix();

        glTranslatef(20.0,90.0,-95.0);

        glPushMatrix();///Corda
            glColor3f(0.0, 0.0,0.0);
            glTranslatef(0.0,5.0,0.0);
            glRotatef(45,0,0,1);
             glBegin(GL_LINE_LOOP);
                   glVertex2i(0,20);
                   glVertex2i(20,0);
                   glVertex2i(20,20);
            glEnd();
        glPopMatrix();

        glPushMatrix();///Moldura
            glColor3f(0.796, 0.548, 0.072);
            glScalef(1.5, 1.0, 0.1);
            glutSolidCube(40);
        glPopMatrix();


        glPushMatrix();
            glColor3f(1,1,1);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture_id[0]);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -20, 2.3);
                glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -20, 2.3);
                glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 20, 2.3);
                glTexCoord2f(0.0f, 1.0f); glVertex3f(-30, 20, 2.3);
            glEnd();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();


    glPopMatrix();

}

void abajur_de_mesa(){

    lampada2(-4.75,65.0,-3.0);

    glPushMatrix();
            glTranslatef(-5.0,36.0,-3.0);

            glPushMatrix();///Base-cilindro
                glColor3f(0.0, 0.712, 0.54);
                glTranslatef(0.0, 12.0, 0.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 1, 1, 20, 30, 30);
            glPopMatrix();

            glPushMatrix();///Base-esfera

                glTranslatef(0.0, 10.0, 0.0);
                //glRotatef(-90, 1,0,0);
                glutSolidSphere(5.0,30,30);
            glPopMatrix();

            glPushMatrix();///Base-cilindro
                glColor3f(1.0, 0.684, 0.316);
                //glTranslatef(0.0, 2.0, 0.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 5, 5, 10, 30, 30);
            glPopMatrix();


            glPushMatrix();///Proteção da Lampada
                glColor3f(0.652, 0.208, 0.584);
                glTranslatef(0.0, 20.0, 0.0);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 10.0, 5.0, 15, 30, 30);
            glPopMatrix();

        glPopMatrix();

}

void tv(){

    IluminacaoTV(20.0,20.0,70.0);

    glPushMatrix ();
        glTranslatef(20,20,80);
        glScalef(0.7, 0.7, 0.7);

        ///Botoes
        glPushMatrix ();//quadrado
            glColor3f(0.388, 0.298, 0.650);
            glTranslatef(-20.0, -7,-20);
            glScalef(1.0,2.0,1.0);
            glutSolidCube(5);
        glPopMatrix ();

        glPushMatrix ();//quadrado
            glTranslatef(-20.0, 5,-20);
            glutSolidSphere(3,30,30);
        glPopMatrix ();

        glPushMatrix ();//quadrado
            glTranslatef(-20.0, 14,-20);
            glutSolidSphere(3,30,30);
        glPopMatrix ();

        glPushMatrix ();///pezinho frontal dir
            glColor3f(0,0,0);
            glTranslatef(20.0, -22.5, -10.0);
            glutSolidCube(5);
        glPopMatrix ();

        glPushMatrix ();///pezinho frontal esq
            glTranslatef(-20.0, -22.5, -10.0);
            glutSolidCube(5);
        glPopMatrix ();

        glPushMatrix ();///pezinho posterior dir
            glTranslatef(20.0, -22.5, 10.0);
            glutSolidCube(5);
        glPopMatrix ();

        glPushMatrix ();///pezinho posterior esq
            glTranslatef(-20.0, -22.5, 10.0);
            glutSolidCube(5);
        glPopMatrix ();

        glPushMatrix ();///TV
            glColor3f(0.568, 0.470, 0.850);
            glScalef(1.5, 1.0, 1.0);
            glutSolidCube(40);
        glPopMatrix ();

        glPushMatrix();///Base-antena
            glColor3f(0.576, 0.576, 0.576);
            glTranslatef(0.0, 20.0, 0.0);
            glRotatef(-90, 1,0,0);
            gluCylinder(gluNewQuadric(), 8, 3, 6, 30, 30);
        glPopMatrix();

        glPushMatrix();///Tampa da base
            glTranslatef(0.0, 26.0, 0.0);
            glRotatef(-90,1,0,0);
            circulo(0,0, 3);
        glPopMatrix();

        glPushMatrix();///Antena Esq
                glTranslatef(0.0, 25.0, 0.0);
                glRotatef(45, 0,0,1);
                glRotatef(-90, 1,0,0);
                gluCylinder(gluNewQuadric(), 0.5, 0.5, 20, 30, 30);
        glPopMatrix();

        glPushMatrix();///Antena Dir
            glTranslatef(0.0, 25.0, 0.0);
            glRotatef(-45, 0,0,1);
            glRotatef(-90, 1,0,0);
            gluCylinder(gluNewQuadric(), 0.5, 0.5, 20, 30, 30);
        glPopMatrix();

        glPushMatrix();///Tela (Jornal)
            glColor3f(1,1,1);
            if(TvOn == 1){
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_id[1]);
                glBegin(GL_QUADS);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(25, -15, -21);// 0 0
                    glTexCoord2f(1.0f, 0.0f); glVertex3f(-13, -15, -21);//1 0
                    glTexCoord2f(1.0f, 1.0f); glVertex3f(-13, 17, -21);//1 1
                    glTexCoord2f(0.0f, 1.0f); glVertex3f(25, 17, -21);//0 1
                glEnd();
                glDisable(GL_TEXTURE_2D);
            }
            else{
                glColor3f(0,0,0);
                glBegin(GL_QUADS);
                    glVertex3f(25, -15, -21);// 0 0
                    glVertex3f(-13, -15, -21);//1 0
                    glVertex3f(-13, 17, -21);//1 1
                    glVertex3f(25, 17, -21);//0 1
                glEnd();
            }

        glPopMatrix();



    glPopMatrix ();


}

void abajur_de_chao(){

    lampada1(115.0,70.0,-80.0);

    glPushMatrix();
        glTranslatef(130.0,2.0,-80.0);

        glPushMatrix();///Base
            glColor3f(0.652, 0.208, 0.584);
            glTranslatef(4.0, 0.0,-13.0);
            glScalef(8.0,8.0,8.0);
            spline();
        glPopMatrix();

        glPushMatrix();///Haste Maior
            glColor3f(0.652, 0.208, 0.584);
            glTranslatef(0.0, 2.0, 0.0);
            glRotatef(-90, 1,0,0);
            gluCylinder(gluNewQuadric(), 2.0, 2, 70, 30, 30);
        glPopMatrix();

        glPushMatrix();///Esfera Haste Maior/Media
            glColor3f(0.34, 0.656, 0.812);
            glTranslatef(0.0, 70.0, 0.0);
            glutSolidSphere(3.0, 30, 30);
        glPopMatrix();

        glPushMatrix();///Haste Media
            glTranslatef(0.0, 70.0, 0.0);
            glRotatef(-90, 0,1,0);
            glRotatef(-45, 1,0,0);
            gluCylinder(gluNewQuadric(), 1.2, 1.2, 20, 30, 30);
        glPopMatrix();

        glPushMatrix();///Esfera Haste Maior/Menor
            glTranslatef(-15.0, 85.0, 0.0);
            glutSolidSphere(2.0, 30, 30);
        glPopMatrix();

        glPushMatrix();///Haste Menor
            glTranslatef(-15.0, 70.0, 0.0);
            glRotatef(-90, 1,0,0);
            gluCylinder(gluNewQuadric(), 1.2, 1.2, 15, 30, 30);
        glPopMatrix();

        glPushMatrix();///Proteção da Lampada
            glColor3f(1.0, 0.5, 0.164);
            glTranslatef(-15.0, 60.0, 0.0);
            glRotatef(-90, 1,0,0);
            gluCylinder(gluNewQuadric(), 10.0, 5.0, 15, 30, 30);
        glPopMatrix();

    glPopMatrix();
}


///Função Constroi ambiente
void ambiente(){
    ///Chão
    chao();

    ///Paredes
    if((angY>=65.0 && angY<300.0) || (angY<=-65.0 && angY>-300.0))
        paradeFrontal(1);//frente

	if(!(angY>115 && angY<245) && !(angY<-115 && angY>-245))
        paradeFrontal(-1);//fundo

    if(!(angY>260 && angY<280)&& !(angY<-80 && angY>-100)){
        paredeLateral(1);//lateral direito

         glPushMatrix();//porta


            glTranslatef(200.0,70.0,0.0);
            glRotatef(90,0,1,0);

            glPushMatrix();
                glColor3f(0,0,0);
                glTranslatef(-15.0,0.0,-5.0);

                glPushMatrix();///Corda
                    glTranslatef(15.0,10.0,3.3);
                    glRotatef(45,0,0,1);
                     glBegin(GL_LINE_LOOP);
                           glVertex2i(0,20);
                           glVertex2i(20,0);
                           glVertex2i(20,20);
                    glEnd();
                glPopMatrix();
                glColor3f(0.7,0.7,0.7);

                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, texture_id[5]);
                glBegin(GL_QUADS);
                glPointSize(20);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -25, 2.3);
                    glTexCoord2f(1.0f, 0.0f); glVertex3f(30, -25, 2.3);
                    glTexCoord2f(1.0f, 1.0f); glVertex3f(30, 25, 2.3);
                    glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 25, 2.3);
                glEnd();
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

glColor3f(0.529, 0.407, 0.239);
            glScalef(1.0,1.5,0.1);
            glutSolidCube(40);
        glPopMatrix();




    }

	if(!(angY>80 && angY<100)&& !(angY<-260 && angY>-280)){
        paredeLateral(-1);//lateral esquerda

        glPushMatrix();//porta
            glColor3f(0.654, 0.301, 0.298);
            glTranslatef(-200.0,52.0,-60.0);

            glPushMatrix();//Maçaneta
                glTranslatef(4.0,0.0,15.0);
                glutSolidSphere(3,30,30);
            glPopMatrix();

            glRotatef(90,0,1,0);
            glScalef(1.0,2.0,0.1);
            glutSolidCube(50);
        glPopMatrix();
	}


    ///Objetos
    criado_mudo();
    sofa();
    abajur_de_chao();
    quadro();
    tapete();
    tv();
    estante();
}

void meiaEsfera(int scaley, int scalex, GLfloat r) {
    /*scalex - scaling of sphere around x-axis
   scaley - scaling of sphere around y-axis
   r - radius of sphere*/
   int i, j;
   GLfloat v[scalex*scaley][3];

   for (i=0; i<scalex; ++i) {
     for (j=0; j<scaley; ++j) {
       v[i*scaley+j][0]=r*cos(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
       v[i*scaley+j][1]=r*sin(i*M_PI/(2*scalex));
       v[i*scaley+j][2]=r*sin(j*2*M_PI/scaley)*cos(i*M_PI/(2*scalex));
     }
   }

   glBegin(GL_QUADS);
     for (i=0; i<scalex-1; ++i) {
       for (j=0; j<scaley; ++j) {
         glVertex3fv(v[i*scaley+j]);
         glVertex3fv(v[i*scaley+(j+1)%scaley]);
         glVertex3fv(v[(i+1)*scaley+(j+1)%scaley]);
         glVertex3fv(v[(i+1)*scaley+j]);
       }
     }
   glEnd();
 }


///Personagem
void cabeca(){
    glPushMatrix();///Cabeça
        glColor3f(1, 0.803, 0.094);

        glTranslatef(0.0,0.0,0.0);
        glRotated(0, 0, 1, 0);
        glTranslatef(0.0,0.0,0.0);

        glTranslatef(0.0,70.0,0.0);


        glPushMatrix();///Pescoço
            glTranslatef(0.0,-14.0,2.5);
            glutSolidSphere(6, 30,30);
            glRotatef(90, 1,0,0);
            gluCylinder(gluNewQuadric(), 6.0, 6.0, 5, 30, 30);



            glPushMatrix();///Rosto
                //glutSolidSphere(6, 30,30);
                glRotatef(177, 1,0,0);
                glRotated(cabZ, 0,0,1);
                gluCylinder(gluNewQuadric(), 6.0, 6.0, 13, 30, 30);


                glPushMatrix();///Nariz
                    glTranslatef(0.0,-8.0,8.5);
                    glutSolidSphere(1.3, 30,30);
                    glRotatef(-90, 1,0,0);
                    gluCylinder(gluNewQuadric(), 1.3, 1.3, 2, 30, 30);
                glPopMatrix();


                glPushMatrix();///Olhos
                    glPushMatrix();///Olhos dir
                        glColor3f(1,1,1);
                        glTranslatef(2.3,-4.0,10.5);
                        glRotatef(100, 1,0,0);
                        glutSolidSphere(3, 30,30);
                        glPushMatrix();///Iris dir
                            glColor3f(0,0,0);
                            glTranslatef(0.7,1.0,2.8);
                            glScalef(0.3,0.3,0.3);
                            glRotatef(80, 1,0,0);
                            spline();
                        glPopMatrix();
                    glPopMatrix();

                    glPushMatrix();///Olhos esq
                        glColor3f(1,1,1);
                        glTranslatef(-2.3,-4.0,10.5);
                        glRotatef(100, 1,0,0);
                        glutSolidSphere(3, 30,30);
                        glPushMatrix();///Iris esq
                            glColor3f(0,0,0);
                            glTranslatef(-0.7,1.0,2.8);
                            glScalef(0.3,0.3,0.3);
                            glRotatef(80, 1,0,0);
                            spline();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();





                glPushMatrix();//Cranio
                    glColor3f(1, 0.803, 0.094);
                    glTranslatef(0.0,0.0,13.0);
                    glutSolidSphere(6, 30,30);

                    glPushMatrix();///cabelo de cima frontal
                        glColor3f(0,0,0);
                        glTranslatef(0.0,0.0,5.0);
                        glRotatef(90,1,0,0);
                        circuloLinha(2.5);
                    glPopMatrix();

                    glPushMatrix();///cabelo de cima posterior
                        glColor3f(0,0,0);
                        glTranslatef(0.0,2.0,5.0);
                        glRotatef(90,1,0,0);
                        circuloLinha(2.5);
                    glPopMatrix();

                glPopMatrix();

                glPushMatrix();///Boca superior
                    glColor3f(0.831, 0.682, 0.490);
                    glTranslatef(2.0,1,4.0);
                    glRotatef(90,1,0,0);
                    glScalef(4.0,7.0,4.0);

                    spline();
                glPopMatrix();

                glPushMatrix();///Boca inferior
                    glColor3f(0.831, 0.682, 0.490);
                    glTranslatef(2.0,-9.0,4); //(2.0,-9.0,10.0)
                    glScalef(4.0,3.0,4.5);
                    glRotatef(-90,1,0,0);

                    glTranslatef(0.0,0.0,2.0);
                    glRotatef(rotBoca,1,0,0);//0 -> 15
                    glTranslatef(0.0,0.0,-2.0);

                    spline();
                glPopMatrix();

                glPushMatrix();//Fundo da Boca
                    glColor3f(0.3, 0.3, 0.3);
                   glTranslatef(1.7,-4.0,8.0);
                   glScalef(3.3,5.8,2.3);
                   glRotatef(180,1,0,0);
//                    glutSolidSphere(3,30,30);
                    spline();
                glPopMatrix();

            glPopMatrix();

        glPopMatrix();

    glPopMatrix();
}

void bracoD(){
    glPushMatrix();///Braço DIR
        glColor3f(1, 1,1);
        glTranslatef(-10.0,1.0,0.0);//ombro
        glRotatef(ombD,0,1,0); //40
        glRotatef(ombDX,1,0,0); //40
        glRotatef(ombDZ,0,0,1); //40
        glutSolidSphere(4, 30,30);

        glPushMatrix();//Braço superior
            glTranslatef(0.0,0.0,0.0);
            glRotatef(braD,1,0,0); //braD
            glRotatef(-25,0,1,0); //-25

            gluCylinder(gluNewQuadric(), 4.0, 4.0, 3, 30, 30);

            glColor3f(1, 0.803, 0.094);
            gluCylinder(gluNewQuadric(), 3.0, 3.0, 10, 30, 30);

            glPushMatrix();//cutuvelo
                glTranslatef(0.0,0.0,10.0);
                glRotatef(cutDX, 1, 0,0); //-90 cutDX
                glRotatef(cutDY, 0, 1,0); //-90
                glutSolidSphere(3, 30,30);

                glPushMatrix();//pulso
                    gluCylinder(gluNewQuadric(), 3.0, 3.0, 10, 30, 30);
                    glPushMatrix();//mao
                        glTranslatef(0.0,0.0,10.0);
                        glPushMatrix();
                        glScalef(1.0,1.0,1.5);
                        glRotatef(45, 0, 1,0);
                        glPopMatrix();
                        glutSolidSphere(3, 30,30);
                        if(acao <4 || acao >=6)
                            dedo_sem_rosq();
                        else
                            dedo_com_rosq();
                    glPopMatrix();

                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
    glPopMatrix();
}

void bracoE(){
    glPushMatrix();///Braço ESQ
            glColor3f(1, 1,1);
            glTranslatef(10.0,1.0,0.0);//ombro
            glRotatef(ombE,0,1,0); //40
            glRotatef(ombEX,1,0,0); //40
            glRotatef(ombEZ,0,0,1); //40
            glutSolidSphere(4, 30,30);

            //glRotatef(-180,1,0,0);

            glPushMatrix();//Braço superior
                glRotatef(25,0,0,1);
                glRotatef(braE,1,0,0);///andE
                gluCylinder(gluNewQuadric(), 4.0, 4.0, 3, 30, 30);
                glColor3f(1, 0.803, 0.094);
                gluCylinder(gluNewQuadric(), 3.0, 3.0, 10, 30, 30);

                glPushMatrix();//cutuvelo
                    glTranslatef(0.0,0.0,10.0);
                    glRotatef(cutEY, 0, 1,0); //-90
                    glutSolidSphere(3, 30,30);

                    glPushMatrix();//pulso
                        gluCylinder(gluNewQuadric(), 3.0, 3.0, 10, 30, 30);
                        glPushMatrix();//mao
                            glTranslatef(0.0,0.0,10.0);
                            glPushMatrix();
                            glScalef(1.0,1.0,1.5);
                            glRotatef(45, 0, 1,0);
                            glPopMatrix();
                            glutSolidSphere(3, 30,30);
                            dedo_sem_rosq();
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();
}

void tronco(){
    glPushMatrix();///tronco
        glTranslatef(0.0,48.0,2.5);
        glColor3f(1, 1,1);

        glPushMatrix();///Peito
            glutSolidSphere(8, 30,30);
            glRotatef(90,1,0,0);
            gluCylinder(gluNewQuadric(), 8.0, 8.0, 5, 30, 30);
        glPopMatrix();

        glPushMatrix();///Barriga
            glTranslatef(0.0,-10.0,0.0);
            glutSolidSphere(11, 30,30);
            glRotatef(90,1,0,0);
            gluCylinder(gluNewQuadric(), 11.0, 11.0, 5, 30, 30);
        glPopMatrix();

        bracoD();
        bracoE();


    glPopMatrix();
}

void pernas(){
    glPushMatrix();///Cintura
        glTranslatef(0.0,33.0,2.5);
        glPushMatrix();
            glColor3f(0.423, 0.686, 0.890);
            glTranslatef(0.0,cintTY,cintTZ);///PARA AGACHAR Y:-10 Z:-10
            glutSolidSphere(10, 30,30);
        glPopMatrix();

        glPushMatrix();///Perna Direita
            glColor3f(0.423, 0.686, 0.890);
            glTranslatef(5.0,-4.0,0.0);

            glPushMatrix();///Coxa
                glRotatef(andD,1,0,0);//andD
                glPushMatrix();
                    glTranslatef(0.0,0.0,10.0);
                    glRotatef(pernaRot,1,0,0);///PARA AGACHAR (-90)pernaRot
                    glTranslatef(0.0,0.0,-10.0);
                    gluCylinder(gluNewQuadric(), 4.0, 4.0, 10, 30, 30);
                glPopMatrix();

                glPushMatrix();///Joelho
                    glTranslatef(0.0,0.0,10.0);
                    glutSolidSphere(4, 30,30);
                    glPushMatrix();///Canela
                        glRotatef(pernaDirHomer,1,0,0);
                        gluCylinder(gluNewQuadric(), 4.0, 4.0, 8, 30, 30);
                        glPushMatrix();///Pé
                            glColor3f(0.415, 0.435, 0.411);
                            glRotatef(-90,1,0,0);
                            glTranslatef(0.0,-10.0,2.0);
                            glScalef(1.0,0.6,2.0);
                            glutSolidSphere(4,30,30);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();



        glPushMatrix();///Perna Esquerda
            glColor3f(0.423, 0.686, 0.890);
            glTranslatef(-4.5,-4.0,0.0);
            glPushMatrix();///Coxa
                glRotatef(andE,1,0,0);
                //glRotatef(perE,1,0,0);
                glPushMatrix();
                    glTranslatef(0.0,0.0,10.0);
                    glRotatef(pernaRot,1,0,0);///PARA AGACHAR
                    glTranslatef(0.0,0.0,-10.0);
                    gluCylinder(gluNewQuadric(), 4.0, 4.0, 10, 30, 30);
                glPopMatrix();

                glPushMatrix();///Joelho
                    glTranslatef(0.0,0.0,10.0);
                    glutSolidSphere(4, 30,30);
                    glPushMatrix();///Canela
                        glRotatef(pernaEsqHomer,1,0,0);// (90 ate 0)
                        gluCylinder(gluNewQuadric(), 4.0, 4.0, 8, 30, 30);
                        glPushMatrix();///Pé
                            glColor3f(0.415, 0.435, 0.411);
                            glRotatef(-90,1,0,0);
                            glTranslatef(0.0,-10.0,2.0);
                            glScalef(1.0,0.6,2.0);
                            glutSolidSphere(4,30,30);
                        glPopMatrix();
                    glPopMatrix();
                glPopMatrix();
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

void homer(){
    glPushMatrix();
        glTranslated(horizontalHomer,-4.0,verticalHomer);
        glRotatef(direcaoHomer,0,1,0);
        glScalef(1.3,1.3,1.3);
        glPushMatrix();
            glTranslated(0.0,troncoTrl,0.0);///PARA AGACHAR EM Y NEGATIVO (-14)
            glTranslated(0.0,0.0,troncoTrlZ);
            glTranslated(0.0,48.0,2.5);
            glRotatef(troncoRot,1,0,0);///PARA AGACHAR (45)
            glTranslated(0.0,-48.0,-2.5);
            cabeca();
            tronco();
        glPopMatrix();

        pernas();
    glPopMatrix();
}

void dedo_com_rosq(){
    glPushMatrix();//dedos

        glPushMatrix();
            glTranslatef(0.0,0.0,2.0);
            glRotatef(90, 0, 1,0);
            glPushMatrix();//dedo1
                glTranslatef(0.0,1.45,3.3);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
            glPushMatrix();//dedo2
                glTranslatef(0.0,0.0,3.5);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
            glPushMatrix();//dedo3
                glTranslatef(0.0,-1.45,3.3);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();//dedao
            glTranslatef(2.0,1.0,-1.0);
            glTranslatef(2.6,0.0,0.0);
            glRotatef(90,1,0,0);
            glRotatef(-60,0,1,0);
            glTranslatef(-2.6,0.0,0.0);
            gluCylinder(gluNewQuadric(), 0.6, 0.6, 2, 30, 30);
            glutSolidSphere(0.6, 30,30);
        glPopMatrix();

        glPushMatrix();//Rosquinha
            glTranslatef(5.0,0.0,-3.0);
            glScalef(0.8,0.8,0.8);
            glRotatef(90,1,0,0);
            rosquinha(2);
        glPopMatrix();

    glPopMatrix();
}

void dedo_sem_rosq(){
    glPushMatrix();//dedos

        glPushMatrix();
            glTranslatef(0.0,0.0,1.0);
            //glRotatef(90, 0, 1,0);
            glPushMatrix();//dedo1
                glTranslatef(0.0,1.45,3.3);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
            glPushMatrix();//dedo2
                glTranslatef(0.0,0.0,3.5);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
            glPushMatrix();//dedo3
                glTranslatef(0.0,-1.45,3.3);
                glRotatef(180,1,0,0);
                gluCylinder(gluNewQuadric(), 0.8, 0.8, 2, 30, 30);
                glutSolidSphere(0.8, 30,30);
            glPopMatrix();
        glPopMatrix();

        glPushMatrix();//dedao
            glTranslatef(0.0,5.0,0.0);
            glTranslatef(2.6,0.0,0.0);
            glRotatef(90,1,0,0);
            glTranslatef(-2.6,0.0,0.0);
            gluCylinder(gluNewQuadric(), 0.6, 0.6, 2, 30, 30);
            glutSolidSphere(0.6, 30,30);
        glPopMatrix();

    glPopMatrix();
}

/**//**//**//**/
void Display(void){
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glTranslatef(0.0, -50.0, 0.0);

    glRotatef(ang, 1,0,0);
    glRotatef(angY, 0,1,0);
    glRotatef(angZ, 0,0,1);

    if(showRosq == TRUE)
        caixa_rosq(local);


    homer();

	ambiente();

	glutSwapBuffers();
}

void Init(void){
    GLdouble h = 0.1 * tan(((angle*180)/M_PI)/2);
    GLdouble w = h*fAspect;
    glClearColor(0, 0, 0, 1.0f);//backgroud 0.811, 0.811, 0.811
    Iluminacao();


    ///Spline
    glMap2f(GL_MAP2_VERTEX_3,
            0.0, 1.0, 3.0, 4.0,
            0.0, 1.0, 15.0, 4.0,
            &points[0][0][0]);
    glEnable(GL_MAP2_VERTEX_3);

    glEnable(GL_AUTO_NORMAL); //Habilitar pra tudo

    glEnable(GL_DEPTH_TEST);


    glEnable(GL_LIGHT2);//ligar lampadas
    glEnable(GL_LIGHT4);

    glOrtho(-ox,ox,-oy,oy,-oz,oz);

    local.u = 1000;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(MAX_TEXTURAS,&texture_id);//melhor aqui

}

void Viewing(void){
    glViewport(0,0, ww,wh);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    gluPerspective(angle,fAspect,0.1,500);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
}

void Reshape(GLsizei w, GLsizei h){
	/* Para previnir uma divisão por zero */
	if ( h == 0 )
		h = 1;

	/* Calcula a correção de aspecto */
	fAspect = (GLfloat)w/(GLfloat)h;
	ww=w; wh=h;

	Viewing();
}


void HandleMouse(int button, int state, int x, int y) {
    int temp;
    posicao pos;
    float u, v;

    if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN && AnimaAuto == -2) {
		    u = (((float)x/ww)*520 - 260);//X
		    v = (((float)y/wh)*260 - 130);//Y

            pos.u = u; pos.v = v;

            obsX = 0; obsY = 180; obsZ = 70;
            ang = 20.0; angY = 0.0; angZ = 0.0;
            glLoadIdentity();
            gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
            glutPostRedisplay();

            FilaInserir(pos);

		}
}

void keyboard ( unsigned char key, int x, int y ) {
    switch ( key )
    {
        case 'a':
            ang -= 5;
            break;
        case 's':
            ang += 5;
            break;
        case 'd':
            angY -= 5;
            break;
        case 'f':
            angY += 5;
            break;
        case 'g':
            angZ -= 5;
            break;
        case 'h':
            angZ += 5;
            break;
        case 27: ///ESC sai do programa
            exit(0);
            break;
    }

    if(ang>360.0 || ang<-360.0)
        ang =0.0;
    if(angY>360.0 || angY<-360.0)
        angY =0.0;
    if(angZ>360.0 || angZ<-360.0)
        angZ =0.0;


	Viewing();
	glutPostRedisplay();
}

void SpecialKeys(int key, int x, int y){
    switch (key){
    /*case GLUT_KEY_LEFT :
        obsX -=10;
        break;
    case GLUT_KEY_RIGHT :
        obsX +=10;
        break;
    case GLUT_KEY_UP :
        obsY +=10;
        break;
    case GLUT_KEY_DOWN :
        obsY -=10;
        break;

    case GLUT_KEY_HOME :
        obsZ +=10;
        break;
    case GLUT_KEY_END :
        obsZ -=10;
        break;*/
    /*case GLUT_KEY_RIGHT:
        flag = ANDANDO;
        horizontalHomer +=2;
        direcaoHomer = 90;
        movimento();
        break;
    case GLUT_KEY_LEFT :
        flag = ANDANDO;
        horizontalHomer -=2;
        direcaoHomer = -90;
        movimento();
        break;
    case GLUT_KEY_UP :
        flag = ANDANDO;
        verticalHomer -=2;
        direcaoHomer = 180;
        movimento();
        break;
    case GLUT_KEY_DOWN :
        flag = ANDANDO;
        verticalHomer +=2;
        direcaoHomer = 0;
        movimento();
        break;

    case GLUT_KEY_HOME :
        flag = LEVANTANDO;
        movimento();
        break;
    case GLUT_KEY_END :
        flag = AGACHANDO;
        movimento();
        break;
    case GLUT_KEY_PAGE_UP:
        flag = PEGANDO;
        incremento = 8;
        movimento();
        break;
    case GLUT_KEY_PAGE_DOWN:
        flag = PEGANDO;
        incremento = -8;
        movimento();
        break;*/

        case GLUT_KEY_RIGHT:
            switch(ACMENU){
                case 1://MODO VISUALIZACAO
                    angY -= 5;
                    break;
                case 2://Animacao Automatica
                    break;
                case 3://Braco Direito
                    if(ombDZ <10)
                        ombDZ +=4;
                    break;
                case 4://Braco Esquerdo
                    if(ombEZ < 100)
                        ombEZ +=4;
                    break;
                case 11://Cabeca
                    if(cabZ < 85)
                        cabZ +=3;
                    break;
                default:
                    break;
            }

        break;

    case GLUT_KEY_LEFT :
            switch(ACMENU){
                case 1://MODO VISUALIZACAO
                    angY += 5;
                    break;
                case 2://Animacao Automatica
                    break;
                case 3://Braco Direito
                    if(ombDZ > -100)
                        ombDZ -=4;
                    break;
                case 4://Braco Esquerdo
                    if(ombEZ > -10)
                        ombEZ -=4;
                    break;
                case 11://Cabeca
                    if(cabZ > -85)
                        cabZ -=3;
                    break;
                default:
                    break;
            }

        break;

    case GLUT_KEY_UP :
        switch(ACMENU){
            case 1://MODO VISUALIZACAO
                ang += 5;
                break;
            case 2://Animacao Automatica
                break;
            case 3://Braco Direito
                if(ombDX > -180)
                    ombDX -=3;
                break;
            case 4://Braco Esquerdo
                if(ombEX > -180)
                    ombEX -=3;
                break;
            case 5://Antebraco Direito
                if(cutDY > 0)
                    cutDY -=2;
                break;
            case 6://Antebraco Esquerdo
                if(cutEY < 0)
                    cutEY +=2;
                break;
            case 7://Perna Direita
                if(andD > 0)
                    andD -=2;
                printf("%d\n", andD);
                break;
            case 8://Perna Esquerdo
                if(andE > 0)
                    andE-=2;
                break;
            case 9://Joelho Direita
                if(pernaDirHomer > 0)
                    pernaDirHomer -=2;
                break;
            case 10://Joelho Esquerdo
                if(pernaEsqHomer > 0)
                    pernaEsqHomer -=2;
                break;
            case 11://Cabeca
                break;
            case 12://Zoom
                if(obsZ > 40)
                    obsZ -=10;
                break;
            default:
                break;
        }

        break;
    case GLUT_KEY_DOWN :
        switch(ACMENU){
            case 1://MODO VISUALIZACAO
                ang -= 5;
                break;
            case 2://Animacao Automatica
                break;
            case 3://Braco Direito
                if(ombDX < 60)
                    ombDX +=3;
                break;
            case 4://Braco Esquerdo
                if(ombEX < 60)
                    ombEX +=3;
                break;
            case 5://Antebraco Direito
                if(cutDY < 50)
                    cutDY +=2;
                break;
            case 6://Antebraco Esquerdo
                if(cutEY > -50)
                    cutEY -=2;
                break;
            case 7://Perna Direita
                if(andD < 90)
                    andD +=2;
                break;
            case 8://Perna Esquerdo
                if(andE < 90)
                    andE +=2;
                break;
            case 9://Joelho Direita
                if(pernaDirHomer <90)
                    pernaDirHomer +=2;
                break;
            case 10://Joelho Esquerdo
                if(pernaEsqHomer < 90)
                    pernaEsqHomer +=2;
            case 11://Cabeca
                break;
            case 12://Zoom
                if(obsZ < 350)
                    obsZ +=10;
                break;
            default:
                break;
        }
        break;
    }

    glLoadIdentity();
    gluLookAt(obsX,obsY,obsZ, 0,0,0, 0,1,0);
    glutPostRedisplay();
}


void movimento(){

    switch(flag){
        case ANDANDO:
            ///Perna
            andD = braD +=incremento;
            andE -=incremento;

            if(andD == 130 || andD == 50){
                incremento = -incremento;
            }

            ///Canela
            if(andD > 90)
                pernaDirHomer +=iD;
            else
                pernaDirHomer = 0;
            if(andD == 130 || andD == 50)
                iD = -iD;

            if(andE > 90)
                pernaEsqHomer -=iE;
            else
                pernaEsqHomer = 0;
            if(andE == 130 || andE == 50)
                iE = -iE;


            break;

        case AGACHANDO:
            ///int pernaRot=0, troncoRot=0; ///pernaRot=-90, troncoRot=45
            ///float cintTY= 0.0,cintTZ=0.0, troncoTrl=0.0; ///cintTY= -10.0,cintTZ=-10.0, troncoTrl=-14.0
            if(pernaRot >= -90){
                pernaRot -= 2;
                troncoRot +=1;
                cintTY -=0.22;
                cintTZ -=0.22;
                troncoTrl -= 0.31;
            }
            break;

        case LEVANTANDO:
            if(pernaRot <= 0){
                pernaRot += 2;
                troncoRot -=1;
                cintTY +=0.22;
                cintTZ +=0.22;
                troncoTrl += 0.31;
            }
            break;

        case PEGANDO:
           if(braD  > -94 && braD < 122)
                braD -=incremento;
            printf("%d\n", braD);
            break;


    }
    flag = -1;
}


void timerFunc(int value){
    ///1000 corresponde a um valor nulo
    int pos;

    //animação automatica
    float x = 18, y = -50;

    if(local.u == 1000){
        showRosq = TRUE;
        if(ACMENU <= 1)
            reseta();
        local = FilaRemover(dado);
        acao = ANDARY;
    }
    else{
        pos = local.v+5;


        if(acao == ANDARY){
            ///Andando
            if(yold> local.v){//Rotação
                direcaoHomer = 180;
            }
            else{
                direcaoHomer = 0;
            }

            pos = local.v;
//            if(xold< local.v){
//                direcaoHomer = 0;
//                pos = local.v;
//            }

            if(pos%2 != 0)
                pos = pos - 1;

            if(verticalHomer > pos)//Translacao
                verticalHomer -=2;

            if(verticalHomer < pos)
                verticalHomer +=2;

            andD +=incremento; braD = andD;
            andE -=incremento; braE = andE;

            if(andD == 130 || andD == 50)
                incremento = -incremento;


            ///movimenta canela
            if(andD > 90)
                pernaDirHomer +=iD;
            else
                pernaDirHomer = 0;
            if(andD == 130 || andD == 50)
                iD = -iD;

            if(andE > 90)
                pernaEsqHomer -=iE;
            else
                pernaEsqHomer = 0;
            if(andE == 130 || andE == 50)
                iE = -iE;

            if(verticalHomer == pos){
                acao = ANDARX;
                andD = andE = 90;
                pernaDirHomer = pernaEsqHomer = 0;
                pos = local.u+10;
                //local = 1000;
            }
        }
        else{
            if(acao == ANDARX){
                ///Andando
                    if(xold> local.u){//Rotação
                        direcaoHomer = -90;
                        pos = local.u+30;
                    }
                    if(xold< local.u){
                        direcaoHomer = 90;
                        pos = local.u-30;
                    }

                    if(pos%2 != 0)
                        pos = pos - 1;

                    if(horizontalHomer > pos)//Translacao
                        horizontalHomer -=2;

                    if(horizontalHomer < pos)
                        horizontalHomer +=2;

                    andD +=incremento; braD = andD;
                    andE -=incremento; braE = andE;

                    if(andD == 130 || andD == 50)
                        incremento = -incremento;


                    ///movimenta canela
                    if(andD > 90)
                        pernaDirHomer +=iD;
                    else
                        pernaDirHomer = 0;
                    if(andD == 130 || andD == 50)
                        iD = -iD;

                    if(andE > 90)
                        pernaEsqHomer -=iE;
                    else
                        pernaEsqHomer = 0;
                    if(andE == 130 || andE == 50)
                        iE = -iE;

                    if(horizontalHomer == pos){
                        acao = AGACHAR;
                        andD = andE = 90;
                        braE = 90;
                        pernaDirHomer = pernaEsqHomer = 0;
                        //local = 1000;
                    }

            }
            else {
                if(acao == AGACHAR){
                    if(pernaRot >= -90){
                        pernaRot -= 5;
                        troncoRot +=2.5;
                        cintTY -=0.55;
                        cintTZ -=0.55;
                        troncoTrl -= 0.77;
                    }
                    else{
                        acao = PEGAR;
                        braD = 90;
                    }
                }
                else{
                    if(acao == PEGAR){
                        if(braD >5){
                            braD -= 8;
                            cutDY +=4;
                        }
                        else{
                            acao = LEVANTAR;
                        }

                    }
                    else{
                        if(acao == LEVANTAR){
                            if(pernaRot <= 0){
                                pernaRot += 5;//2
                                troncoRot -=2.5;//1
                                cintTY +=0.55;//0.22
                                cintTZ +=0.55;//0.22
                                troncoTrl += 0.77;//0.31
                                cutDY -=2;
                            }
                            else{
                                acao = LEVARPRABOCA;
                                cutDY = 0; //confirmar q o braco esta reto
                            }
                        }
                        else{
                            if(acao == LEVARPRABOCA){
                                if(cutDX > -90){
                                    cutDX -=8;
                                    if(ombD < 60)
                                        ombD += 4;
                                    if(rotBoca < 18)
                                        rotBoca += 3;


                                }
                                else{
                                    acao = RESETAR;
                                }
                            }
                            else{
                                if(acao == RESETAR){
                                    if(cutDX <= 0){
                                        cutDX +=8;
                                        if(ombD >= 0)
                                            ombD -= 4;
                                        if(rotBoca >= 0)
                                            rotBoca -= 3;
                                        if(braD <=90)
                                            braD += 8;
                                    }
                                    else{
                                        andD = andE = braD = 90;
                                        pernaDirHomer = pernaEsqHomer = 0;
                                        xold = local.u;
                                        yold = local.v;
                                        local.u = 1000;
                                        ombD = 0;
                                        cutDX = cutDY = 0;
                                        showRosq = FALSE;//SOME COM A CAIXA

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }

    if(AnimaAuto != -1 || AnimaAuto != -2){
        pos = y;//animação automatica
        switch(AnimaAuto){
            case 0://Anda y
                if(yold> y)//Rotação
                    direcaoHomer = 180;
                else
                    direcaoHomer = 0;

                pos = y;

                if(pos%2 != 0)
                    pos = pos - 1;

                if(verticalHomer > pos)//Translacao
                    verticalHomer -=2;

                if(verticalHomer < pos)
                    verticalHomer +=2;

                andD +=incremento; braD = andD;
                andE -=incremento; braE = andE;

                if(andD == 130 || andD == 50)
                    incremento = -incremento;


                ///movimenta canela
                if(andD > 90)
                    pernaDirHomer +=iD;
                else
                    pernaDirHomer = 0;
                if(andD == 130 || andD == 50)
                    iD = -iD;

                if(andE > 90)
                    pernaEsqHomer -=iE;
                else
                    pernaEsqHomer = 0;
                if(andE == 130 || andE == 50)
                    iE = -iE;

                if(verticalHomer == pos){
                    andD = andE = 90;
                    pernaDirHomer = pernaEsqHomer = 0;
                    AnimaAuto = 1;
                    direcaoHomer = 0;
                }
            break;

            case 1://Anda X
                if(xold> x)//Rotação
                    direcaoHomer = -90;
                else
                    direcaoHomer = 90;

                pos = x;

                if(pos%2 != 0)
                    pos = pos - 1;

                if(horizontalHomer > pos)//Translacao
                    horizontalHomer -=2;

                if(horizontalHomer < pos)
                    horizontalHomer +=2;

                andD +=incremento; braD = andD;
                andE -=incremento; braE = andE;

                if(andD == 130 || andD == 50)
                    incremento = -incremento;


                ///movimenta canela
                if(andD > 90)
                    pernaDirHomer +=iD;
                else
                    pernaDirHomer = 0;
                if(andD == 130 || andD == 50)
                    iD = -iD;

                if(andE > 90)
                    pernaEsqHomer -=iE;
                else
                    pernaEsqHomer = 0;
                if(andE == 130 || andE == 50)
                    iE = -iE;

                if(horizontalHomer == pos){
                    andD = andE = 90;
                    pernaDirHomer = pernaEsqHomer = 0;
                    AnimaAuto = 2;
                    direcaoHomer = 0;
                }
                break;

            case 2:
                if(pernaRot >= -45){
                        pernaRot -= 5;
                        //troncoRot +=2.5;
                        cintTY -=0.55;
                        cintTZ -=0.55;
                        troncoTrl -= 0.7;
                        troncoTrlZ -= 0.50;
                    }
                else{//Terminando Apaga luz ambiente e liga tv
                    glDisable(GL_LIGHT0);
                    glEnable(GL_LIGHT1);//Escuro
                    glDisable(GL_LIGHT2);//lampada 2
                    glEnable(GL_LIGHT3);//luz tv
                    glDisable(GL_LIGHT4);//lampada 1

                    TvOn = 1;
                    AnimaAuto = -1;
                }
                break;

            case 3:
                if(pernaRot < 0){
                        pernaRot += 5;
                        //troncoRot -=2.5;
                        cintTY +=0.55;
                        cintTZ +=0.55;
                        troncoTrl += 0.77;
                        troncoTrlZ += 0.50;
                    }
                else{
                    glEnable(GL_LIGHT0);//Luz ambiente escuro
                    glDisable(GL_LIGHT1);//Luz ambiente escuro
                    glEnable(GL_LIGHT2);//lampada 2
                    glDisable(GL_LIGHT3);//luz tv
                    glEnable(GL_LIGHT4);//lampada 1
                    TvOn = 0;
                    AnimaAuto = -2;
                }
                break;

        }
    }

    Viewing();
	glutPostRedisplay();
    glutTimerFunc(40,timerFunc, 1);
}


void reseta(){
    andD = 90; andE = 90; braD = 90; braE = 90;
    pernaDirHomer = 0; pernaEsqHomer = 0;
}


void ProcessMenu(int value){

    ACMENU = value;

    switch(ACMENU){
        case 0:
            exit(1);
            break;
        case 1://MODO VISUALIZACAO
            obsX = 0; obsY = 0; obsZ = 200;
             angY = 0.0; angZ = 0.0;//ang = 20.0;
            break;
        case 2://Animacao Automatica
            AnimaAuto = 0;
            break;
        case 3://Braco Direito
            break;
        case 4://Braco Esquerdo
            break;
        case 5://Antebraco Direito
            break;
        case 6://Antebraco Esquerdo
            break;
        case 7://Perna Direita
            break;
        case 8://Perna Esquerdo
            break;
        case 9://Joelho Direita
            break;
        case 10://Joelho Esquerdo
            break;
        case 11://Cabeca
            break;
        case 12://Zoom
            break;
        case 13:
            AnimaAuto = 3;
            break;
        default:
            break;
    }

    glutPostRedisplay();    // Redisplay
}


int main ( int argc, char** argv ){
    glutInit  ( &argc, argv );
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH| GLUT_RGB | GLUT_MULTISAMPLE);

	glutInitWindowSize(ww,wh);
	glutCreateWindow("Os Simpsons e as rosquinhas magicas");
glEnable(GLUT_MULTISAMPLE);
	Init();
	CarregaTextura("pintura.bmp", 0);
	CarregaTextura("jornal.bmp", 1);
	CarregaTextura("foto1.bmp", 2);
	CarregaTextura("foto2.bmp", 3);
	CarregaTextura("livro.bmp", 4);
	CarregaTextura("familia.bmp", 5);
	CarregaTextura("lard.bmp", 6);


	glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);

    glutTimerFunc(10,timerFunc,0);

    glutKeyboardFunc ( keyboard );
    glutSpecialFunc(SpecialKeys);
	glutMouseFunc(HandleMouse);

	// Cria um Menu
	glutCreateMenu(ProcessMenu);
        glutAddMenuEntry("Modo Visualizacao", 1);
        glutAddMenuEntry("Assistir TV", 2);
        glutAddMenuEntry("Parar de Assistir TV", 13);
        glutAddMenuEntry("Braco Direito", 3);
        glutAddMenuEntry("Braco Esquerdo", 4);
        glutAddMenuEntry("Antebraco Direito", 5);
        glutAddMenuEntry("Antebraco Esquerdo", 6);
        glutAddMenuEntry("Perna Direito", 7);
        glutAddMenuEntry("Perna Esquerdo", 8);
        glutAddMenuEntry("Joelho Direita", 9);
        glutAddMenuEntry("Joelho Esquerdo", 10);
        glutAddMenuEntry("Cabeca", 11);
        glutAddMenuEntry("Zoom", 12);
        glutAddMenuEntry("Sair", 0);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
}
