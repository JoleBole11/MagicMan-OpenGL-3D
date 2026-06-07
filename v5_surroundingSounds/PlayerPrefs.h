#pragma once
#include <string.h>
#include <vector>
#include <algorithm>
#include <functional>

class PlayerPrefs {
private:
	static PlayerPrefs* instance;
	int highScores[4] = { 0, 0, 0, 0 };
public:
	static PlayerPrefs* getInstance() {
		if (!instance) {
			instance = new PlayerPrefs();
			instance->loadHighScores();
		}
		return instance;
	}

	void addScore(int score) {
		highScores[3] = score;
		std::sort(highScores, highScores + 4, std::greater<int>());
		saveHighScores();
	}

	int getHighScore(int pos) {
		return highScores[pos];
	}

	void saveHighScores() {
		FILE* file = fopen("highscores.txt", "w");
		if (file) {
			for (int i = 0; i < 3; ++i) {
				fprintf(file, "%d\n", highScores[i]);
			}
			fclose(file);
		}
	}

	void loadHighScores() {
		FILE* file = fopen("highscores.txt", "r");
		if (file) {
			for (int i = 0; i < 3; ++i) {
				fscanf(file, "%d", &highScores[i]);
			}
			fclose(file);
		}
	}
};