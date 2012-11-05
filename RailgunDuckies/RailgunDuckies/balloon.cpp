#include "balloon.h"

using namespace std;

std::vector<glm::vec3> balloon::balVert;
std::vector<GLuint> balloon::balIndex;
std::vector<glm::vec3> balloon::normVert;
std::vector<glm::vec3> balloon::colVert;
static const GLsizei VertexCount = 6; 
static const GLsizei VertexSize = 4;
const int stacks = 20;
const int slices = 36;

//Define directional function for computing the vertex array. 
inline int left(int c) {
	if ( c != 0 ) {
		return c - 1;
	} else {
		return c + (slices - 1);
	}
}

inline int down(int r) {
	if (r != slices - 1) {
		return r + slices;
	} else {
		return r;
	}
}

inline int right(int c ) {
	if ( c != slices - 1) {
		return c + 1;
	} else {
		return c - (slices - 2);
	}
}

inline int up(int r) {
	if (r != 0) {
		return r - slices;
	} else {
		return r;
	}
}


//Constuctor that puts the balloon at the origin. 
balloon::balloon() {
	this->position.x = this->position.y = this->position.z = 0.0;
}

/*
Constructor that will place the balloon at a given point in space, and
will assign the balloon a point value. 
*/
balloon::balloon(float x, float y, float z, int p) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
	this->points = p;
}

//Converts degrees to radians for trig functions. 
float radians(float deg) {
	return 3.14159f/180.0f*deg;
}

/*
This function draws the balloon by computing vertices from mathematical 
functions.  The top half of the balloon is drawn and a hemisphere, and the 
bottom half of the balloon is drawn from a cosine function.  There is also 
few lines that will display the point value of each balloon hovering above
the balloon.  The vertices are stored in a vertex array and then a normal 
array is computed along side the vertex array.  This allows us to draw the
balloons with a vertex buffer.  
*/
void balloon::drawBalloon() {
	GLfloat mat_ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	//Store modelview matrix
	glPushMatrix();

	//Move to the specified coordinates to draw the balloon at
	glTranslatef(this->position.x, this->position.y, this->position.z);

	//store the matrix before displaying the point value
	glPushMatrix();
	char score_string[16];
	sprintf_s(score_string, "%d", this->points);

	glDisable(GL_LIGHTING);
	glTranslatef(-0.75, 1.5, -0.5);
	glScalef(0.01f, 0.01f, 1.0f);
	glColor3f(0, 0, 0);
	glutStrokeString(GLUT_STROKE_ROMAN, (const unsigned char *) score_string);
	glEnable(GL_LIGHTING);
	glPopMatrix();


	int index = 0;
	float r = 0;

	glm::vec3 PositionData[slices*stacks];
	glm::vec3 NormalArray[slices*stacks];
	GLuint IndexData[3*slices*stacks];
	/*
	To draw the balloon, navigate from the top to the bottom along the y-axis.  
	at each height, compute the radius according to the proper function(either 
	an arc or a cosine curve). After the proper radius is computed, rotate around
	the y-axis and store each point in the vertex array. 
	*/


	if (balVert.size() == 0) {
		balVert.resize(stacks*slices);

		for (int stack = 0; stack < stacks; ++stack) {
			float curY = 1-(2.65f/((float)stacks))*stack;
		
			//Check Y position and determine function to control balloon radius
			if (curY >= 0) {
				r = sqrt(1 - pow(curY, 2));
			} else {
				r = cos(curY);
			}

			for (int slice = 0; slice < slices; ++slice) {
				
				float rotAng = (360/(float)slices)*slice;
				float deltaX = cos(radians(rotAng));
				float deltaZ = sin(radians(rotAng));
				balVert[stack*slices + slice] = (glm::vec3(r*deltaX, curY, r*deltaZ));
			}

		}

		
		//To calculate the index arrays, choose the vertex at the index, move one to
		//the right (i+1) and then move one below that(i+36).  Then for the other 
		//triangle choose one to the right (i+1), one below (i+36), and one below and 
		//to the right (i+37).  


		for (int i = 0; i < balVert.size() - slices; i++) {
			

			balIndex.push_back(i);
			balIndex.push_back(left(i));
			balIndex.push_back(left(down(i)));
			balIndex.push_back(down(i));


			if (i > slices) {
				glm::vec3 temp1 = glm::cross(balVert[up(i)], balVert[right(i)]);
				glm::vec3 temp2 = glm::cross(balVert[right(i)], balVert[down(i)]);
				glm::vec3 temp3 = glm::cross(balVert[down(i)], balVert[left(i)]);
				glm::vec3 temp4 = glm::cross(balVert[left(i)], balVert[up(i)]);

				normVert.push_back(glm::normalize((temp1+temp2+temp3+temp4)));
			}

		}
	}

	glColor3d(1, 0, 0);
	glVertexPointer(3, GL_FLOAT, 0, &(balVert[0]));

	glEnableClientState(GL_VERTEX_ARRAY);	
    glEnableClientState(GL_NORMAL_ARRAY);	
//	glEnableClientState(GL_COLOR_ARRAY);

	glNormalPointer(GL_FLOAT, 0, &(normVert[0]));
//	glColorPointer(3, GL_FLOAT, 0, &(balVert[0]));
	glDrawElements(GL_QUADS, balIndex.size(), GL_UNSIGNED_INT, &balIndex[0]);

//	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glTranslated(0, -1.6, 0);
	glutSolidSphere(0.07, 10, 10);
	glTranslated(0, -0.07, 0);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.05, 0.09, 20, 20);
	


	glPopMatrix();
	
}

/*
Allows for updating balloon position by specifying new coordinates. 
*/
void balloon::updateBalloon(float x, float y, float z) {
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;
}

/*
Special Draw function for beauty mode.  Translates only in the z direction
to keep the balloon centered on the screen.  The function takes in the game
time which is used to control the rotation of the balloon. 
*/
void balloon::drawBBalloon(double time) {
	glPushMatrix();
	glTranslated(0, 0, -5);
	glRotated((time/1000) * 45.0, 0.1, 1, 0.1);
	this->drawBalloon();
	glPopMatrix();
}

//Allow game class to set point value of balloon.
void balloon::setPoints(int p) {
	this->points = p;
}

//Allow game class to get the point value of a balloon. 
int balloon::getPoints() {
	return this->points;
}

glm::vec3 balloon::getBalPos() {
	return this->position;
}

void balloon::destroy() {
	this->position.z = 100;
}