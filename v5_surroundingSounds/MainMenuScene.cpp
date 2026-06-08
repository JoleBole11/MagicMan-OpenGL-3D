#include "MainMenuScene.h"
#include "SoundManager.h"

std::unique_ptr<Font> MainMenuScene::shared_font = nullptr;

MainMenuScene::~MainMenuScene()
{
	cleanup();
}

void MainMenuScene::initialize()
{
    SoundManager::get_instance().playSong(SoundManager::get_instance().mainMenuSong);

    shared_font = std::make_unique<Font>(
        "fonts/Roboto-Regular.ttf",
        36
    );

    gameName = std::make_unique<GameObject>("gameName");
    gameName->add_component<Text>(
        shared_font.get(),
        "MagicMan",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    gameName->get_component<Transform>()->position = glm::vec3(400.0f, 800.0f, 0.0f);

    playText = std::make_unique<GameObject>("playText");
    playText->add_component<Text>(
        shared_font.get(),
        "Press LMB to Play",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    playText->get_component<Transform>()->position = glm::vec3(50.0f, 600.0f, 0.0f);

    quitText = std::make_unique<GameObject>("quitText");
    quitText->add_component<Text>(
        shared_font.get(),
        "Press Q to Quit",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    quitText->get_component<Transform>()->position = glm::vec3(50.0f, 500.0f, 0.0f);

    highScoreText = std::make_unique<GameObject>("highScoreText");
    highScoreText->add_component<Text>(
        shared_font.get(),
        "HighScores:",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    highScoreText->get_component<Transform>()->position = glm::vec3(700.0f, 625.0f, 0.0f);

    highScore1 = std::make_unique<GameObject>("highScore1");
    highScore1->add_component<Text>(
        shared_font.get(),
        "1. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(0)),
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    highScore1->get_component<Transform>()->position = glm::vec3(700.0f, 575, 0.0f);

    highScore2 = std::make_unique<GameObject>("highScore2");
    highScore2->add_component<Text>(
        shared_font.get(),
        "2. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(1)),
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    highScore2->get_component<Transform>()->position = glm::vec3(700.0f, 525, 0.0f);

    highScore3 = std::make_unique<GameObject>("highScore3");
    highScore3->add_component<Text>(
        shared_font.get(),
        "3. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(2)),
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    highScore3->get_component<Transform>()->position = glm::vec3(700.0f, 475, 0.0f);

    controlsText = std::make_unique<GameObject>("controlsText");
    controlsText->add_component<Text>(
        shared_font.get(),
        "Controls:",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    controlsText->get_component<Transform>()->position = glm::vec3(50.0f, 300.0f, 0.0f);

    magicText = std::make_unique<GameObject>("magicText");
    magicText->add_component<Text>(
        shared_font.get(),
        "Press 1 to select Magic Projectile",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    magicText->get_component<Transform>()->position = glm::vec3(50.0f, 250.0f, 0.0f);

    freezeText = std::make_unique<GameObject>("freezeText");
    freezeText->add_component<Text>(
        shared_font.get(),
        "Press 2 to select Freeze Projectile",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    freezeText->get_component<Transform>()->position = glm::vec3(50.0f, 200.0f, 0.0f);

    fireballText = std::make_unique<GameObject>("fireballText");
    fireballText->add_component<Text>(
        shared_font.get(),
        "Press 3 to select Fireball Projectile",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    fireballText->get_component<Transform>()->position = glm::vec3(50.0f, 150.0f, 0.0f);

    moveText = std::make_unique<GameObject>("moveText");
    moveText->add_component<Text>(
        shared_font.get(),
        "WASD to Move",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    moveText->get_component<Transform>()->position = glm::vec3(50.0f, 100.0f, 0.0f);

    fireText = std::make_unique<GameObject>("fireText");
    fireText->add_component<Text>(
        shared_font.get(),
        "Press LMB to Fire",
        glm::vec3(1.0f, 0.84f, 0.0f)
    );
    fireText->get_component<Transform>()->position = glm::vec3(50.0f, 50.0f, 0.0f);

    initialized = true;
}

void MainMenuScene::update(float deltaTime)
{
    if (Input::get_mouse_button_down(0)) {
        SceneManager::getInstance()->changeScene("Game");
    }
    if (Input::get_key_down('q')) {
        exit(0);
    } 
}

void MainMenuScene::update_physics(float deltaTime)
{
}

void MainMenuScene::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    render3d();
    render2d();
}

void MainMenuScene::game_loop()
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

void MainMenuScene::render2d()
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

    auto* tf1 = gameName->get_component<Transform>();
    tf1->rotation = glm::quat{ 1,0,0,0 };
    gameName->render();
    auto* tf2 = playText->get_component<Transform>();
    tf2->rotation = glm::quat{ 1,0,0,0 };
    playText->render();
    auto* tf3 = quitText->get_component<Transform>();
    tf3->rotation = glm::quat{ 1,0,0,0 };
    quitText->render();
    auto* tf4 = highScoreText->get_component<Transform>();
    tf4->rotation = glm::quat{ 1,0,0,0 };
    highScoreText->render();
    auto* tf5 = highScore1->get_component<Transform>();
    tf5->rotation = glm::quat{ 1,0,0,0 };
    highScore1->render();
    auto* tf6 = highScore2->get_component<Transform>();
    tf6->rotation = glm::quat{ 1,0,0,0 };
    highScore2->render();
    auto* tf7 = highScore3->get_component<Transform>();
    tf7->rotation = glm::quat{ 1,0,0,0 };
    highScore3->render();
    auto* tf8 = controlsText->get_component<Transform>();
    tf8->rotation = glm::quat{ 1,0,0,0 };
    controlsText->render();
    auto* tf9 = magicText->get_component<Transform>();
    tf9->rotation = glm::quat{ 1,0,0,0 };
    magicText->render();
    auto* tf10 = freezeText->get_component<Transform>();
    tf10->rotation = glm::quat{ 1,0,0,0 };
    freezeText->render();
    auto* tf11 = fireballText->get_component<Transform>();
    tf11->rotation = glm::quat{ 1,0,0,0 };
    fireballText->render();
    auto* tf12 = moveText->get_component<Transform>();
    tf12->rotation = glm::quat{ 1,0,0,0 };
    moveText->render();
    auto* tf13 = fireText->get_component<Transform>();
    tf13->rotation = glm::quat{ 1,0,0,0 };
    fireText->render();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void MainMenuScene::render3d()
{
}

void MainMenuScene::cleanup()
{
    gameName.reset();
    playText.reset();
    quitText.reset();
    highScoreText.reset();
    highScore1.reset();
    highScore2.reset();
    highScore3.reset();
    shared_font.reset();
    controlsText.reset();
    magicText.reset();
	freezeText.reset();
    fireballText.reset();
    moveText.reset();
    fireText.reset();

    SoundManager::get_instance().stopMusic();

    initialized = false;
}

void MainMenuScene::onEnter()
{
    highScore1->get_component<Text>()->setText("1. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(0)));
    highScore2->get_component<Text>()->setText("2. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(1)));
    highScore3->get_component<Text>()->setText("3. " + std::to_string(PlayerPrefs::getInstance()->getHighScore(2)));
}

void MainMenuScene::onExit()
{
    cleanup();
}
