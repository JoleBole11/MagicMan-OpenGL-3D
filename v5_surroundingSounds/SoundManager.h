#pragma once
#include "fmod.hpp"
#include "glm.hpp"
#include <fmod_errors.h>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

class SoundManager {
public:
    static SoundManager& get_instance() {
        static SoundManager instance;
        return instance;
    }

    bool init_fmod() {
        FMOD_RESULT result = FMOD::System_Create(&audio_manager);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        result = audio_manager->init(50, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, nullptr);
        if (result != FMOD_OK) {
            std::cerr << "FMOD init error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        result = audio_manager->set3DSettings(1.0f, 1.0f, 1.0f);
        if (result != FMOD_OK) {
            std::cerr << "FMOD 3D settings error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

    bool init_audio() {
   
        if (!create_sound("sounds/FireSound.mp3", FMOD_DEFAULT | FMOD_3D, &fireSound)) return false;
        if (!create_sound("sounds/EnemyAttack.mp3", FMOD_DEFAULT | FMOD_3D, &enemyAttackSound)) return false;
        if (!create_sound("sounds/EnemyHit.mp3", FMOD_DEFAULT | FMOD_3D, &enemyHitSound)) return false;
        if (!create_sound("sounds/FireballHit.mp3", FMOD_DEFAULT | FMOD_3D, &fireballHitSound)) return false;
        if (!create_sound("sounds/FreezeHit.mp3", FMOD_DEFAULT | FMOD_3D, &freezeHitSound)) return false;
        if (!create_sound("sounds/MagicHit.mp3", FMOD_DEFAULT | FMOD_3D, &magicHitSound)) return false;
        if (!create_sound("sounds/PlayerHit.mp3", FMOD_DEFAULT | FMOD_3D, &playerHitSound)) return false;
        if (!create_sound("sounds/Gameplay.mp3", FMOD_LOOP_NORMAL | FMOD_2D, &gameplaySong)) return false;
        if (!create_sound("sounds/MainMenu.mp3", FMOD_LOOP_NORMAL | FMOD_2D, &mainMenuSong)) return false;

        return true;
    }

    void release() {

        if (fireSound) fireSound->release();
        if (enemyAttackSound) enemyAttackSound->release();
        if (enemyHitSound) enemyHitSound->release();
        if (fireballHitSound) fireballHitSound->release();
        if (freezeHitSound) freezeHitSound->release();
        if (magicHitSound) magicHitSound->release();
        if (playerHitSound) playerHitSound->release();
        if (gameplaySong) gameplaySong->release();
        if (mainMenuSong) mainMenuSong->release();

        if (audio_manager) {
            audio_manager->close();
            audio_manager->release();
            audio_manager = nullptr;
        }
    }

    void play_sound_on_position(FMOD::Sound* sound, FMOD::Channel*& channel, const glm::vec3& position) {

        if (!audio_manager) return;

        if (audio_manager->playSound(sound, nullptr, true, &channel) != FMOD_OK) return;

        FMOD_VECTOR pos = { position.x, position.y, position.z };
        channel->set3DAttributes(&pos, nullptr);
        channel->setPaused(false);
    }

    void playSong(FMOD::Sound* song) {
        musicChannel->stop();

        audio_manager->playSound(song, nullptr, false, &musicChannel);
        musicChannel->setVolume(0.14f);
    }

    void update_listener(const glm::vec3& pos, const glm::vec3& forward, const glm::vec3& up = { 0,1,0 })
    {
        if (!audio_manager) return;

        FMOD_VECTOR p = { pos.x, pos.y, pos.z };
        FMOD_VECTOR f = { forward.x, forward.y, forward.z };
        FMOD_VECTOR u = { up.x, up.y, up.z };
        FMOD_VECTOR v = { 0,0,0 };

        audio_manager->set3DListenerAttributes(0, &p, &v, &f, &u);
    }

    void update() {
        audio_manager->update();
    }

    void stopMusic() {
        musicChannel->stop();
    }

    SoundManager() = default;
    ~SoundManager() { release(); }

    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

    FMOD::System* audio_manager = nullptr;
    FMOD::Channel* surrounding_sounds = nullptr;
    FMOD::Channel* musicChannel;
    FMOD::Sound* fireSound = nullptr;
    FMOD::Sound* enemyAttackSound = nullptr;
    FMOD::Sound* enemyHitSound = nullptr;
    FMOD::Sound* fireballHitSound = nullptr;
    FMOD::Sound* freezeHitSound = nullptr;
    FMOD::Sound* magicHitSound = nullptr;
    FMOD::Sound* playerHitSound = nullptr;
    FMOD::Sound* gameplaySong = nullptr;
    FMOD::Sound* mainMenuSong = nullptr;
private:
    bool create_sound(const char* filePath, FMOD_MODE mode, FMOD::Sound** sound) {
        FMOD_RESULT result = audio_manager->createSound(filePath, mode, nullptr, sound);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << " | File: " << filePath << std::endl;
            return false;
        }
        return true;
    }
};