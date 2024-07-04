#include <iostream>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include <iomanip>


using std::cout;
using std::endl;
using std::vector;


const int CONSOLE_SIZE = 20; // game field width and height
bool FOOD_ON_FIELD = false;
bool GAME = true;


struct snakeUnit {
	int x;
	int y;
	const char character = '*';

	snakeUnit(int x, int y) :
		x(x),
		y(y) {}
	snakeUnit() {}
};

struct apple : snakeUnit {
	const char character = '*';
	apple() : snakeUnit() {}
	void generate() {
		this->x = (rand() % CONSOLE_SIZE) + 1;
		this->y = (rand() % CONSOLE_SIZE) + 1;

		FOOD_ON_FIELD = true;
	}
};

class Snake {
private:
	int headX = 0;
	int headY = 0;

	int directionX = 0;
	int directionY = -1;
public:
	Snake(int startLength) {
		int x;
		int y;

		for (int i = 0; i < startLength; i++) {
			if (snakeBody.size() == 0) {
				x = CONSOLE_SIZE / 2;
				y = CONSOLE_SIZE / 2;
			}
			else {
				x = snakeBody[snakeBody.size() - 1].x;
				y = snakeBody[snakeBody.size() - 1].y + 1;
			}
			const snakeUnit unit = snakeUnit(x, y);
			snakeBody.push_back(unit);
		}
	}
	vector<snakeUnit> snakeBody;
	
	void catchKeybord() {
		if (_kbhit()) {
			const char pressedKey = _getch();
			switch (pressedKey) {
			case 'w':
				if (directionY != 1) {
					directionY = -1; // Up
					directionX = 0;
				}
				break;
			case 'a':
				if (directionX != 1) {
					directionX = -1; // Left
					directionY = 0;
				}
				break;
			case 's':
				if (directionY != -1) {
					directionY = 1; // Up
					directionX = 0;
				}
				break;
			case 'd':
				if (directionX != -1) {
					directionX = 1; // Left
					directionY = 0;
				}
				break;
			}
		}
	}
	void move() {
		for (size_t i = snakeBody.size() - 1; i >= 1; i--) {
			snakeBody[i].x = snakeBody[i - 1].x;
			snakeBody[i].y = snakeBody[i - 1].y;
		}
	
		snakeBody[0].x += directionX;
		snakeBody[0].y += directionY;
	}

	void checkIntraction(apple& food) {
		if (snakeBody[0].x == food.x && snakeBody[0].y == food.y) {
			const snakeUnit unit = snakeUnit(snakeBody[snakeBody.size() - 1].x + 1, snakeBody[snakeBody.size() - 1].y);
			snakeBody.push_back(unit);
			FOOD_ON_FIELD = false;
		}
		if ((snakeBody[0].x == 0 || snakeBody[0].x == CONSOLE_SIZE - 2) || (snakeBody[0].y == 0 || snakeBody[0].y == CONSOLE_SIZE - 2)) {
			GAME = false;
		}
	}

	void update() {
		catchKeybord();
		move();
	}
};

void generateField(char(&field)[CONSOLE_SIZE][CONSOLE_SIZE], Snake& snake, apple& food) {
	for (int i = 0; i < CONSOLE_SIZE; i++) {
		for (int j = 0; j < CONSOLE_SIZE; j++) {
			if ((i == 0 || i == CONSOLE_SIZE - 1) || (j == 0 || j == CONSOLE_SIZE - 1)) {
				field[i][j] = '#';
			}
			else {
				field[i][j] = ' ';
			}
		}
	}

	for (auto& item : snake.snakeBody) {
		field[item.y + 1][item.x + 1] = item.character;
	}
	
	if (!FOOD_ON_FIELD) {
		food.generate();
	}
	else {
		snake.checkIntraction(food);
	}
	field[food.y][food.x] = food.character;
}

void draw(char(&field)[CONSOLE_SIZE][CONSOLE_SIZE]) {
	// Output field
	for (int i = 0; i < CONSOLE_SIZE; i++) {
		for (int j = 0; j < CONSOLE_SIZE; j++) {
			cout << field[i][j] << std::setw(2);
		}
		cout << endl;
	}
}

int main() {
	Snake snake(5);
	apple apple;

	char field[CONSOLE_SIZE][CONSOLE_SIZE] = {};

	while (GAME) {
		system("cls");
		snake.update();
		generateField(field, snake, apple);
		draw(field);
		Sleep(500);
	}
	return 0;
}
