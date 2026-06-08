#include "GameOverScene.h"

std::unique_ptr<Font> GameOverScene::shared_font = nullptr;

GameOverScene::~GameOverScene()
{
	cleanup();
}

void GameOverScene::initialize()
{
    shared_font = std::make_unique<Font>(
        "fonts/Roboto-Regular.ttf",
        36
    );

    loseText = std::make_unique<GameObject>("loseText");
    loseText->add_component<Text>(
        shared_font.get(),
        "YOU LOSE",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    loseText->get_component<Transform>()->position = glm::vec3(400.0f, 800.0f, 0.0f);

    score = std::make_unique<GameObject>("score");
    score->add_component<Text>(
        shared_font.get(),
        "Score: " + std::to_string(PlayerPrefs::getInstance()->getLastScore()),
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    score->get_component<Transform>()->position = glm::vec3(400.0f, 450.0f, 0.0f);

    continueText = std::make_unique<GameObject>("continueText");
    continueText->add_component<Text>(
        shared_font.get(),
        "Press LMB to continue",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    continueText->get_component<Transform>()->position = glm::vec3(400.0f, 100.0f, 0.0f);
}

void GameOverScene::update(float deltaTime)
{
    if (Input::get_mouse_button_down(0)) {
        SceneManager::getInstance()->changeScene("Menu");
    }
}

void GameOverScene::update_physics(float deltaTime)
{
}

void GameOverScene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    render3d();
    render2d();
}

void GameOverScene::game_loop()
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

void GameOverScene::render2d()
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

    auto* tf1 = loseText->get_component<Transform>();
    tf1->rotation = glm::quat{ 1,0,0,0 };
    loseText->render();

    auto* tf2 = score->get_component<Transform>();
    tf2->rotation = glm::quat{ 1,0,0,0 };
    score->render();

    auto* tf3 = continueText->get_component<Transform>();
    tf3->rotation = glm::quat{ 1,0,0,0 };
    continueText->render();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void GameOverScene::render3d()
{
}

void GameOverScene::cleanup()
{
    loseText.reset();
    score.reset();
    continueText.reset();
    shared_font.reset();
    initialized = false;
}

void GameOverScene::onEnter()
{
}

void GameOverScene::onExit()
{
}
