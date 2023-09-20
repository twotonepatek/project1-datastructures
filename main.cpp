#include <iostream>
#include <string>
#include <cstdlib>
#include <chrono>
#include <thread>

#ifdef _MSC_VER  // DO NOT BREAK APART THE IF PREPROCESSOR DIRECTIVES
	#include <windows.h> 
#else			
	#include <iostream>
	#include <cstring>
	#include <cstdlib>
#endif			


///////////////////////////////////////////////////////////////////////////
// Global Constants
///////////////////////////////////////////////////////////////////////////

const int CANVAS_ROW = 24;
const int CANVAS_COL = 24;

const int BOX_SIZE = 2;
const int SNAKE_SIZE = 4;
const int BIGBLINK_SIZE = 9;
const int BOAT_SIZE = 3;

const int MAX_STEPS = 30;

const char ALIVE = 'X';
const char DEAD = '.';


///////////////////////////////////////////////////////////////////////////
// Type Definitions
///////////////////////////////////////////////////////////////////////////

class Matrix;
class Simulation;
class Life {
public:
	int getCol() const {
		return col;
	}
	int getRow() const {
		return row;
	}
	int getHeight() const {
		return height;
	}
	int getWidth() const {
		return width;
	}
	char getFigure(int r, int c) const {
		return sprite[r][c];
	}
	void inMatrix(Matrix *m) {
		matrix = m;
	}
	void inSimulation(Simulation *s) {
		simulation = s;
	}
	bool areWeInASimulation() {
		return simulation != nullptr;
	}
protected:
	int col;
	int row;
	int height;
	int width;
	char **sprite;
	Matrix *matrix;
	Simulation *simulation;
};

class Snake : public Life {
public:
	Snake(int r, int c) {
		col = c;
		row = r;
		height = SNAKE_SIZE;
		width = SNAKE_SIZE;

		//Allocate space for figure
		sprite = new char*[height];
		for (int i = 0; i < height; i++) {
			sprite[i] = new char[width];
		}

		//Initialize figure
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sprite[i][j] = DEAD;
			}
		}
		sprite[0][0] = ALIVE;
		sprite[1][0] = ALIVE;
		sprite[1][1] = ALIVE;
		sprite[0][2] = ALIVE;
		sprite[0][3] = ALIVE;
		sprite[1][3] = ALIVE;
	}
	~Snake() {
		for (int i = 0; i < height; i++) {
			delete[] sprite[i];
		}
		delete[] sprite;
	}
};

class Box : public Life {
public:
	Box(int r, int c) {
		col = c;
		row = r;
		height = BOX_SIZE;
		width = BOX_SIZE;

		//Allocate space for figure
		sprite = new char*[height];
		for (int i = 0; i < height; i++) {
			sprite[i] = new char[width];
		}

		//Initialize figure
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sprite[i][j] = ALIVE;
			}
		}

	}
	~Box() {
		for (int i = 0; i < height; i++) {
			delete[] sprite[i];
		}
		delete[] sprite;
	}
};

class Boat : public Life {
public:
	Boat(int r, int c) {
		col = c;
		row = r;
		height = BOAT_SIZE;
		width = BOAT_SIZE;

		// Allocate space for figure
		sprite = new char*[height];
		for (int i = 0; i < height; i++) {
			sprite[i] = new char[width];
		}

		// Initialize figure
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sprite[i][j] = DEAD;
			}
		}
		sprite[0][1] = ALIVE;
		sprite[1][2] = ALIVE;
		sprite[1][0] = ALIVE;
		sprite[2][1] = ALIVE;
		sprite[2][2] = ALIVE;
	}
	~Boat() {
		for (int i = 0; i < height; i++) {
			delete[] sprite[i];
		}
		delete[] sprite;
	}
};

class BigBlink : public Life {
public:
	BigBlink(int r, int c) {
		col = c;
		row = r;
		height = BIGBLINK_SIZE;
		width = BIGBLINK_SIZE;

		//Allocate space for figure
		sprite = new char*[height];
		for (int i = 0; i < height; i++) {
			sprite[i] = new char[width];
		}

		//Initialize figure
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				sprite[i][j] = DEAD;
			}
		}
		sprite[0][4] = ALIVE;
		sprite[1][4] = ALIVE;
		sprite[2][4] = ALIVE;

		sprite[6][4] = ALIVE;
		sprite[7][4] = ALIVE;
		sprite[8][4] = ALIVE;

		sprite[4][0] = ALIVE;
		sprite[4][1] = ALIVE;
		sprite[4][2] = ALIVE;

		sprite[4][6] = ALIVE;
		sprite[4][7] = ALIVE;
		sprite[4][8] = ALIVE;
	
	}
	~BigBlink() {
		for (int i = 0; i < height; i++) {
			delete[] sprite[i];
		}
		delete[] sprite;
	}
};

class Matrix {
public:
	Matrix() {
		toggle = true;
		grid_1 = new char*[CANVAS_ROW];
		grid_2 = new char*[CANVAS_ROW];

		for (char i = 0; i < CANVAS_ROW; i++) {
			grid_1[i] = new char[CANVAS_COL];
			grid_2[i] = new char[CANVAS_COL];
		}
		for (char i = 0; i < CANVAS_ROW; i++) {
			for (char j = 0; j < CANVAS_COL; j++) {
				grid_1[i][j] = DEAD;
			}
		}
	}
	~Matrix() {
		for (char i = 0; i < CANVAS_ROW; i++) {
			delete[] grid_1[i];
			delete[] grid_2[i];
		}
		delete[] grid_1;
		delete[] grid_2;

	}
	void render() const {
		clearScreen();
		if (toggle) {
			for (char i = 0; i < CANVAS_ROW; i++) {
				for (char j = 0; j < CANVAS_COL; j++) {
					std::cout << grid_1[i][j];
				}
				std::cout << std::endl;
			}
		}
		else {
			for (char i = 0; i < CANVAS_ROW; i++) {
				for (char j = 0; j < CANVAS_COL; j++) {
					std::cout << grid_2[i][j];
				}
				std::cout << std::endl;
			}
		}
		for (char i = 0; i < CANVAS_COL; i++) {
			std::cout << '=';
		}
		std::cout << std::endl;
	}
	void computeNextState() {
		if (toggle) {
			for (char i = 0; i < CANVAS_ROW; i++) {
				for (char j = 0; j < CANVAS_COL; j++) {
					grid_2[i][j] =
						nextState(grid_1[i][j], i, j, toggle);
				}
			}
			toggle = !toggle;
		}
		else {
			for (char i = 0; i < CANVAS_ROW; i++) {
				for (char j = 0; j < CANVAS_COL; j++) {
					grid_1[i][j] =
						nextState(grid_2[i][j], i, j, toggle);
				}
			}
			toggle = !toggle;
		}
	}
	bool initState(Life *life) {
		if (life == nullptr) {
			std::cout << "Cannot add nullptr life" << std::endl;
			return false;
		}
		for (char i = life->getRow(); i - life->getRow() < life->getHeight(); i++) {
			for (char j = life->getCol(); j - life->getCol() < life->getWidth(); j++) {
				if (i < CANVAS_ROW && j < CANVAS_COL) {
					grid_1[i][j] =
						life->getFigure(i - life->getRow(), j - life->getCol());
				}
			}
		}
		return true;
	}

private:

	void clearScreen() const {
#ifdef _MSC_VER  // DO NOT BREAK APART THE PREPROCESSOR DIRECTIVES
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(hConsole, &csbi);
		DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
		COORD upperLeft = { 0, 0 };
		DWORD dwCharsWritten;
		FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
			&dwCharsWritten);
		SetConsoleCursorPosition(hConsole, upperLeft);
#else 
		static const char* term = getenv("TERM");// will just write a newline in an Xcode output window
		if (term == nullptr || strcmp(term, "dumb") == 0)
			std::cout << std::endl;
		else {
			static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
			std::cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << std::flush;
		}
#endif
	}
	char nextState(char state, char row, char col, bool toggle) const {
		int yCoord = row;
		int xCoord = col;
		char neighbors = 0;
		if (toggle) {
			for (char i = yCoord - 1; i <= yCoord + 1; i++) {
				for (char j = xCoord - 1; j <= xCoord + 1; j++) {
					if (i == yCoord && j == xCoord) {
						continue;
					}
					if (i > -1 && i < CANVAS_ROW && j > -1 && j < CANVAS_COL) {
						if (grid_1[i][j] == ALIVE) {
							neighbors++;
						}
					}
				}
			}
		}
		else {
			for (char i = yCoord - 1; i <= yCoord + 1; i++) {
				for (char j = xCoord - 1; j <= xCoord + 1; j++) {
					if (i == yCoord && j == xCoord) {
						continue;
					}
					if (i > -1 && i < CANVAS_ROW && j > -1 && j < CANVAS_COL) {
						if (grid_2[i][j] == ALIVE) {
							neighbors++;
						}
					}
				}
			}
		}
		if (state == ALIVE) {
			return (neighbors > 1 && neighbors < 4) ? ALIVE : DEAD;
		}
		else {
			return (neighbors == 3) ? ALIVE : DEAD;
		}
	}

	// The rules of Conway's Simulation of Life requires each cell to
	// examine it's neighbors.  So, we have to check the entire matrix
	// first before we change it.  We can use two copies of the matrix, 
	// one to check the current state (current day) then another to generate 
	// the next state(for the next day). We can toggle between them each 
	// step, to avoid having to copy between matrix each step (day) of the game.
	char **	grid_1;
	char **	grid_2;
	bool	toggle;
};

class Simulation {
public:
	Simulation(Life **life, int numLife) {
		watchme = 1;
		steps = 0;
		automate = false;
		matrix = new Matrix();

		if (life != nullptr) {
			for (int i = 0; i < numLife; i++) {
				if (life[i] != nullptr) {
					bool success = matrix->initState(life[i]);
					if (!success) {
						std::cout << "Failed to add life to the matrix" << std::endl;
					}
				}
			}
		}
	}
	~Simulation() {
		delete matrix;
	}
	void simulate() {
		while (true) {
			matrix->render();

			if (!automate) {
				std::cout << "command (<space> to step, <a> to automate, <q> to quit): ";

				std::string action;
				std::getline(std::cin, action);

				switch (action[0])
				{

				default:
					std::cout << '\a' << std::endl;  // beep
					continue;
				case 'q':
					std::cout << "Quitting Game." << std::endl;
					return;
				case 's':

					continue;
				case ' ':

					break;
				case 'a':
					automate = true;
					break;
				}
			}
			else {
				if (steps >= MAX_STEPS) {
					std::cout << "Reached max steps, quitting." << std::endl;
					return;
				}
				delay(300);
			}
			steps++;
			matrix->computeNextState();
		}
	}
	void report() {
		std::string msg = "Hello World!"; // Replace Hello World with your answer.
		std::cout << msg << std::endl;
	}
	int two(int u) {
		return (u << 2) % 10; 
	}
	int three(int x) {
		if (x % 2 == 0)
			return 5;
		else
			return 3;
	}
	void one(int t) {
		int i = 0;
		int k = watchme;
		while (++i < t) {
			if (i == 0)
				k = 2;
			else if (i == 1)
				k = 7;
			else if (i == 4)
				k = 8;
			else if (i == 5)
				k = 5;
			else
				k = two(watchme);
			watchme = k;
		}
		k = three(watchme);
		watchme = k;
	}
private:
	void delay(int ms) const {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}

	int		watchme;
	Matrix * matrix;
	int		steps;
	bool	automate;
};

int main() {

	const int numLife = 4;
	Life **population = new Life*[numLife];

	population[0] = new Boat(8, 4); 
	population[1] = new Snake(18, 14);
	population[2] = new BigBlink(3, 13);
	population[3] = new Box(9, 13);
	
	// Create the game
	Simulation s(population, numLife);

	// Debugger Exercise
	s.one('n' - 'j');

	// Run the game
	s.simulate();

	// Report
	s.report();

	// Clean up
	delete population[0];
	delete population[1];
	delete population[2];
	delete population[3];
	delete[] population;

}