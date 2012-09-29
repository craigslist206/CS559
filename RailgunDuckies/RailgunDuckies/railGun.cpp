#include <gl/freeglut.h>
#include "railGun.h"
#include <algorithm>

		
//Constructor
railGun::railGun() {
	x = 0;
	y = 0;
	z = 0;
	rx = 0;
	ry = 0;
}

//Draw duck based on current duck coords
void railGun::drawGun() {

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 }; 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glPushMatrix();
	glTranslated(x, y, z);
	glColor3d(0.5, 0.5, 0.5);

	//Draw Base
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	glTranslated(0, 0, -2);
	glutSolidCone(1.5, 2, 50, 50);
	glPopMatrix();

	//Rotate Gun
	glRotated(rx, 1, 0, 0);
	glRotated(ry, 0, 1, 0);

	/*Draw gun body
	glPushMatrix();
	glTranslated(0, 0.5, 0);
	glutSolidCube(1);
	glPopMatrix();*/

	//Draw barrell
	glTranslated(0, .5, 0);

	glPushMatrix();
	glTranslated(.375, .375, -1);
	glScaled(.25, .25, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(.375, -.375, -1);
	glScaled(.25, .25, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-.375, -.375, -1);
	glScaled(.25, .25, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-.375, .375, -1);
	glScaled(.25, .25, 6);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 1.875);
	glScaled(1, 1,.25);
	glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -3.875);
	glScaled(1, 1, .25);
	glutSolidCube(1);
	glPopMatrix();




	glPopMatrix();
}

void railGun::updateGun() {

}