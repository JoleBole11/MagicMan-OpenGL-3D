#include "IntroScene.h"
#include "SceneManager.h"
#include <freeglut.h>

std::unique_ptr<Font> IntroScene::shared_font = nullptr;

IntroScene::IntroScene() : Scene("Intro"), duration(5.0f), timer(0.0f) {
}

IntroScene::~IntroScene() {
    cleanup();
}

void IntroScene::initialize() {
    shared_font = std::make_unique<Font>(
        "fonts/Roboto-Regular.ttf",
        36
    );

    text = std::make_unique<GameObject>("Text");
    text->add_component<Text>(
        shared_font.get(),
        "MagicMan by Bogdan Cvetkovic 5934",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    text->get_component<Transform>()->position = glm::vec3(225.0f, 800.0f, 0.0f);

    initialized = true;
}

void IntroScene::update(float dt) {
    timer += dt;

    if (timer >= duration) {
        SceneManager::getInstance()->changeScene("Menu");
    }

    text->update(dt);
}

void IntroScene::update_physics(float dt)
{
}

void IntroScene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    render3d();
    render2d();
}

void IntroScene::render2d()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMultMatrixf(glm::value_ptr(glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), -1.0f, 1.0f)));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_CULL_FACE);

    auto* tf2 = text->get_component<Transform>();
    tf2->rotation = glm::quat{ 1,0,0,0 };
    text->render();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void IntroScene::render3d()
{
}

void IntroScene::cleanup() {
    delete &text;
    text = nullptr;
}

void IntroScene::game_loop()
{
    float current_time = glutGet(GLUT_ELAPSED_TIME);
    delta_time = (float)((current_time - previous_time) / 1000);
    previous_time = current_time;

    update(delta_time);
    update_physics(delta_time);
    render();

    glutSwapBuffers();
    glutPostRedisplay();
}

void IntroScene::onEnter() {
    timer = 0.0f;
}

void IntroScene::onExit()
{
}
