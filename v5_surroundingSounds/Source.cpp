#include <freeglut.h>
#include <GL/glu.h>
#include "SceneManager.h"
#include "IntroScene.h"
#include "GameScene.h"
#include "Input.h"
#include <memory>
#include <iostream>
#include "MainMenuScene.h"
#include "GameOverScene.h"

PFNGLACTIVETEXTUREARBPROC Material::glActiveTextureARB = nullptr;
PFNGLMULTITEXCOORD2FARBPROC Material::glMultiTexCoord2fARB = nullptr;
PlayerPrefs* PlayerPrefs::instance = nullptr;

glm::vec2 window_size = { 1000, 1000 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

int delay = 16;

void init_physics()
{

}

void init_game() {
    glClearColor(window_color.r, window_color.g, window_color.b, window_color.a);
    glEnable(GL_DEPTH_TEST);
    init_physics();

    GLfloat light_pos[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
    GLfloat diffuse_pos[4] = { 1, 1, 1, 1 };
    GLfloat ambient_pos[4] = { 0.2f, 0.2f, 0.2f, 1 };

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_pos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, diffuse_pos);

    SoundManager::get_instance().init_fmod();
    SoundManager::get_instance().init_audio();
}

void init_glut(int argv, char** argc) {
    glutInit(&argv, argc);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_size.x, window_size.y);
    glutInitWindowPosition(window_position.x, window_position.y);
    glutCreateWindow("MagicMan");
}

void timer(int val) {
    glutPostRedisplay();
    glutTimerFunc(delay, timer, 0);
}

void game_loop() {
	SceneManager::getInstance()->game_loop();
}

int main(int argv, char** argc) {
    init_glut(argv, argc);
    init_game();

    glutDisplayFunc(game_loop);
    glutTimerFunc(delay, timer, 0);

    Input::init();

	PlayerPrefs* playerPrefs = PlayerPrefs::getInstance();

    SceneManager* sceneManager = SceneManager::getInstance();
    sceneManager->addScene("Intro", std::make_unique<IntroScene>());
	sceneManager->addScene("Menu", std::make_unique<MainMenuScene>());
    sceneManager->addScene("Game", std::make_unique<GameScene>());
    sceneManager->addScene("GameOver", std::make_unique<GameOverScene>());
    sceneManager->changeScene("Menu");

    glutMainLoop();
    SoundManager::get_instance().release();

    return 0;
}