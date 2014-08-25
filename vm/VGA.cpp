#include "main.hpp"

#ifndef VGA_C
#define VGA_C

VGA_dev::VGA_dev() {
  vram = std::vector<byte>(SCREEN_WIDTH*SCREEN_HEIGHT);
  pos = 0;
}

void VGA_dev::power() {
  std::shared_ptr<VGA_port> vgap = std::make_shared<VGA_port>(this);
  machine->_databridge->register_port(3,vgap);
  glutInitDisplayMode(GLUT_SINGLE);
  glutInitWindowSize(300, 300);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Hello world :D");
}

void VGA_dev::tick() {
  display();
}

void VGA_dev::redraw() {
  glBindTexture(GL_TEXTURE_2D, 1);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, vram.data());
}

void VGA_dev::draw() {
  glEnable (GL_TEXTURE_2D);

  glBindTexture (GL_TEXTURE_2D, 13);
  glBegin (GL_QUADS);
  glTexCoord2f (0.0, 0.0);
  glVertex3f (0.0, 0.0, 0.0);
  glTexCoord2f (1.0, 0.0);
  glVertex3f (10.0, 0.0, 0.0);
  glTexCoord2f (1.0, 1.0);
  glVertex3f (10.0, 10.0, 0.0);
  glTexCoord2f (0.0, 1.0);
  glVertex3f (0.0, 10.0, 0.0);
  glEnd ();

  glDisable (GL_TEXTURE_2D);
}

void VGA_dev::display() {
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (0, 0, -20); /* eye position */

  draw();

	glutSwapBuffers();
}

#endif
