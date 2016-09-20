/*
A excerpt from a much larger exercise in creating a model-viewer. 
This was a 1st-person flying camera I threw together in a few hours without any prior experience with camera management.
*/

void idle(int value)
{
	glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(10, idle, 0);
	glRotatef(1, 0, 1, 0);
	glutPostRedisplay();
	if (locked)
	{
		glutWarpPointer(X_CENTER, Y_CENTER);
	}
	if (moveForward)
	{
		camerax -= sin(cameraAnglex * 2 * M_PI/360);
		cameraz += cos(cameraAnglex * 2 * M_PI/360);
	}
	if (moveBackward)
	{
		camerax += sin(cameraAnglex * 2 * M_PI/360);
		cameraz -= cos(cameraAnglex * 2 * M_PI/360);
	}
	if (moveRight)
	{
		camerax -= cos(cameraAnglex * 2 * M_PI/360);
		cameraz -= sin(cameraAnglex * 2 * M_PI/360);
	}
	if (moveLeft)
	{
		camerax += cos(cameraAnglex * 2 * M_PI/360);
		cameraz += sin(cameraAnglex * 2 * M_PI/360);
	}
	if (moveUp)
	{
		cameray += 1;
	}
	if (moveDown)
	{
		cameray -= 1;
	}
}

void mouseLook(int x, int y)
{
	if (locked)
	{
	cameraAnglex += float(x - X_CENTER) * R_SPEED * 4 * M_PI/360;
	cameraAngley += -float(y - Y_CENTER) * R_SPEED * 4 * M_PI/360;
	
	if (cameraAnglex > 360)
	{
		cameraAnglex = cameraAnglex - 360;
	}
	if (cameraAnglex < 0)
	{
		cameraAnglex = cameraAnglex + 360;
	}
	if (cameraAngley > 90)
	{
		cameraAngley = 90;
	}
	else if (cameraAngley < -90)
	{
		cameraAngley = -90;
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 50, -400);
	
	glRotatef(cameraAngley, 1, 0, 0);
	glRotatef(cameraAnglex, 0, 1, 0);
	
	glTranslatef(-camerax, -cameray, -cameraz);
	}
}