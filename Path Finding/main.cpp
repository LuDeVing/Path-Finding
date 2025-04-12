#include <algorithm>
#include <iostream>
#include <random>
#include <queue>
#include <set>

#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define MED_BLOCKS

#ifdef FEW_BLOCKS

int blockSize = 10;

const int sw = 800, sh = 400;
const int vsw = 80, vsh = 40;

int pixelSize = 3;

#endif

#ifdef MED_BLOCKS

int blockSize = 10;

const int sw = 1200, sh = 600;
const int vsw = 120, vsh = 60;

int pixelSize = 2;

#endif

#ifdef MANY_BLOCKS

int blockSize = 5;

const int sw = 1000, sh = 1000;
const int vsw = 200, vsh = 200;

int pixelSize = 1;

#endif

class App : public olc::PixelGameEngine {

private:

	struct node {

		int x, y;
		int cost, costA, costB;

		node(int x, int y, int cost, int costA, int costB) {
			
			this->x = x;
			this->y = y;
			
			this->cost = cost;
			this->costA = costA;
			this->costB = costB;

		}

		bool operator < (const node& o2) const {

			if (cost == o2.cost) {

				if (costB == o2.costB)
					return (costA < o2.costA);

				return (costB < o2.costB);

			}

			return (cost < o2.cost);
		
		}


	};

	enum Type {NONE, START, END, TRY, PAHT, BARRICADE};

	std::pair <Type, olc::Pixel> types[6] = {
		{Type::START, olc::GREEN},
		{Type::END, olc::RED},
		{Type::BARRICADE, olc::Pixel(128, 128, 128)},
		{Type::NONE, olc::WHITE},
		{Type::TRY, olc::Pixel(175, 238, 238)},
		{Type::PAHT, olc::Pixel(152, 251, 152) }
	};

	int curTypesIdx = 0;

	class block {
	
	private:

		Type type = Type::NONE;
		olc::Pixel color = olc::WHITE;

	public:

		block() = default;

		Type getType() {
			return this->type;
		}

		void setType(Type newType, olc::Pixel newColor = olc::Pixel()) {
			this->type = newType;
			this->color = newColor;
		}

		void setType(std::pair <Type, olc::Pixel>& tp) {
			this->type = tp.first;
			this->color = tp.second;
		}

		olc::Pixel getColor() {
			return this->color;
		}

	};

	block blocks[vsh][vsw];
	bool used[vsh][vsw];

	bool inEditorMode = true;

	olc::Pixel borderColor = olc::Pixel(206, 206, 206);

	std::vector <std::pair <int, int> > searchPath;
	std::vector <std::pair <int, int> > shortestPath;

	int searchIdx = 0;
	int shortestIdx = 0;

	bool currentMode = true;
	bool currentModeFunc = true;

public:

	App() {
		sAppName = "Example";
	}

private:

	// helper functiond 

	int calcDist(int x1, int y1, int x2, int y2) {
		
		int dx = abs(x1 - x2);
		int dy = abs(y1 - y2);
		int mn = std::min(dx, dy);
		int mx = std::max(dx, dy);

		return mn * 14 + (mx - mn) * 10;

	}

	void draw(int x, int y, olc::Pixel currentColor) {

		for (int curY = y; curY <= y + blockSize; curY++) {
			for (int curX = x; curX <= x + blockSize; curX++) {

				if (curY == y || curY == y + blockSize || curX == x || curX == x + blockSize) {
					Draw(curX, curY, borderColor);
				}
				else Draw(curX, curY, currentColor);
			
			}
		}

	}

	void getInputs() {

		if (GetKey(olc::K1).bPressed) {
			curTypesIdx = 0;
		}
		if (GetKey(olc::K2).bPressed) {
			curTypesIdx = 1;
		}
		if (GetKey(olc::K3).bPressed) {
			curTypesIdx = 2;
		}
		if (GetKey(olc::K4).bPressed) {
			curTypesIdx = 3;
		}

		int xCord = GetMouseX();
		int yCord = GetMouseY();

		size_t x = std::min(xCord / blockSize, sw / blockSize - 1);
		size_t y = std::min(yCord / blockSize, sh / blockSize - 1);

		if (GetMouse(0).bHeld) {

			if (types[curTypesIdx].first == Type::START || types[curTypesIdx].first == Type::END) {

				for (int curY = 0; curY < vsh; curY++) {
					for (int curX = 0; curX < vsw; curX++) {
						if (blocks[curY][curX].getType() == types[curTypesIdx].first)
							blocks[curY][curX].setType(types[3]);
					}
				}

			}

			blocks[y][x].setType(types[curTypesIdx]);

		}

		if (GetKey(olc::S).bPressed) {

			int hasBoth = 0;

			for (int curY = 0; curY < vsh; curY++)
				for (int curX = 0; curX < vsw; curX++)
					if (blocks[curY][curX].getType() == Type::END
					|| blocks[curY][curX].getType() == Type::START) hasBoth++;

			if (hasBoth) inEditorMode = false;

		}

	}

	void reset() {

		searchPath.clear();
		shortestPath.clear();

		searchIdx = 0;
		shortestIdx = 0;

		inEditorMode = true;

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {
				
				blocks[curY][curX] = block();
				used[curY][curX] = false;

			}
		}
	}

	void fillSet(std::multiset <node>& st, std::pair <int, int> cur, std::pair <int, int> par,
		std::vector < std::vector <std::pair <int, int> > >& last,
		std::pair <int, int> startIdx, std::pair <int, int> endIdx) {

		if (cur.first >= 0 && cur.first < vsw &&
			cur.second >= 0 && cur.second < vsh &&
			!used[cur.second][cur.first]) {

			used[cur.second][cur.first] = true;
			last[cur.second][cur.first] = par;

			int costA = calcDist(cur.first, cur.second, startIdx.first, startIdx.second);
			int costB = calcDist(cur.first, cur.second, endIdx.first, endIdx.second);

			node Node = node(cur.first, cur.second, costA + costB, costA, costB);

			st.insert(Node);

		}

	}

	// pathfinding algorithms

	void bfs() {

		std::pair <int, int> startIdx;
		std::pair <int, int> endIdx;
		std::pair <int, int> last[vsh][vsw];

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {

				if (blocks[curY][curX].getType() == Type::START)
					startIdx = { curX, curY };

				if (blocks[curY][curX].getType() == Type::END)
					endIdx = { curX, curY };

				used[curY][curX] = false;

				if (blocks[curY][curX].getType() == Type::BARRICADE)
					used[curY][curX] = true;

				last[curY][curX] = { -1, -1 };

			}
		}

		std::queue <std::pair <std::pair <int, int>, bool> > q;

		last[startIdx.second][startIdx.first] = startIdx;
		used[startIdx.second][startIdx.first] = true;

		q.push({ startIdx, 0 });

		while (!q.empty()) {

			std::pair <int, int> cur = q.front().first;
			bool dir = q.front().second;

			searchPath.push_back(cur);
			q.pop();

			if (dir) goto P2;

		P1:

			if (cur.first + 1 < vsw && !used[cur.second][cur.first + 1]) {

				q.push({ {cur.first + 1, cur.second}, true });
				used[cur.second][cur.first + 1] = true;
				last[cur.second][cur.first + 1] = cur;

				if (blocks[cur.second][cur.first + 1].getType() == Type::END) {
					endIdx = cur;
					break;
				}

			}

			if (cur.first - 1 >= 0 && !used[cur.second][cur.first - 1]) {

				q.push({ { cur.first - 1, cur.second }, true });
				used[cur.second][cur.first - 1] = true;
				last[cur.second][cur.first - 1] = cur;

				if (blocks[cur.second][cur.first - 1].getType() == Type::END) {
					endIdx = cur;
					break;
				}

			}

			if (dir) goto P3;

		P2:

			if (cur.second + 1 < vsh && !used[cur.second + 1][cur.first]) {

				q.push({ { cur.first, cur.second + 1} , false });
				used[cur.second + 1][cur.first] = true;
				last[cur.second + 1][cur.first] = cur;

				if (blocks[cur.second + 1][cur.first].getType() == Type::END) {
					endIdx = cur;
					break;
				}

			}

			if (cur.second - 1 >= 0 && !used[cur.second - 1][cur.first]) {

				q.push({ { cur.first, cur.second - 1 }, false });
				used[cur.second - 1][cur.first] = true;
				last[cur.second - 1][cur.first] = cur;

				if (blocks[cur.second - 1][cur.first].getType() == Type::END) {
					endIdx = cur;
					break;
				}

			}

			if (dir) goto P1;

		P3:;

		}

		while (endIdx != startIdx) {

			if (endIdx == std::pair<int, int>({ -1, -1 }))
				break;

			shortestPath.push_back({ endIdx });
			endIdx = last[endIdx.second][endIdx.first];

		}

		if (endIdx == std::pair<int, int>({ -1, -1 })) {

			searchPath.clear();
			shortestPath.clear();

			searchIdx = 0;
			shortestIdx = 0;

			inEditorMode = true;

			return;

		}

		shortestPath.push_back(startIdx);

		std::reverse(shortestPath.begin(), shortestPath.end());

	}

	void aStar(bool wantReturn = false) {

		std::pair <int, int> startIdx = { -1, -1 };
		std::pair <int, int> endIdx = { -1, -1 };
		std::vector < std::vector <std::pair <int, int> > > last =
			std::vector < std::vector < std::pair <int, int> > >(vsh, std::vector < std::pair <int, int> >(vsw));

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {

				if (blocks[curY][curX].getType() == Type::START)
					startIdx = { curX, curY };

				if (blocks[curY][curX].getType() == Type::END)
					endIdx = { curX, curY };

				used[curY][curX] = false;

				if (blocks[curY][curX].getType() == Type::BARRICADE)
					used[curY][curX] = true;

				last[curY][curX] = { -1, -1 };

			}
		}

		std::multiset <node> st;

		st.insert(node(startIdx.first, startIdx.second,
			calcDist(startIdx.first, startIdx.second, endIdx.first, endIdx.second), 0,
			calcDist(startIdx.first, startIdx.second, endIdx.first, endIdx.second)));

		std::pair <int, int> cur = startIdx;

		used[startIdx.second][startIdx.first] = true;
		last[startIdx.second][startIdx.first] = { startIdx.first, startIdx.second };

		while (cur != endIdx && !st.empty()) {

			auto ptr = st.begin();
			cur = { ptr->x, ptr->y };
			st.erase(st.begin());

			if (cur == endIdx) break;

			if(!wantReturn) searchPath.push_back(cur);

			fillSet(st, { cur.first - 1, cur.second }, cur, last, startIdx, endIdx);
			fillSet(st, { cur.first + 1, cur.second }, cur, last, startIdx, endIdx);
			fillSet(st, { cur.first, cur.second - 1 }, cur, last, startIdx, endIdx);
			fillSet(st, { cur.first, cur.second + 1 }, cur, last, startIdx, endIdx);

		}

		if (wantReturn) {
			
			if(cur == endIdx)
				shortestPath.push_back({ endIdx });
		
			return;
		
		}


		while (endIdx != startIdx) {

			if (endIdx == std::pair<int, int>({ -1, -1 }))
				break;

			shortestPath.push_back({ endIdx });
			endIdx = last[endIdx.second][endIdx.first];

		}

		shortestPath.push_back(startIdx);

		std::reverse(shortestPath.begin(), shortestPath.end());

	}

	// different modes;

	void normalMode() {

		if (GetKey(olc::R).bPressed) reset();

		if (inEditorMode) getInputs();
		else {

			if (shortestPath.size() == 0) {
				if (currentModeFunc) bfs();
				else aStar();
			}

			if (inEditorMode) return;

			if (searchIdx < searchPath.size()) {

				std::pair <int, int> cur = searchPath[searchIdx];
				Type tp = blocks[cur.second][cur.first].getType();

				if (tp != Type::START && tp != Type::END)
					blocks[cur.second][cur.first].setType(types[4]);

				searchIdx++;
			}
			else if (shortestIdx < shortestPath.size()) {

				std::pair <int, int> cur = shortestPath[shortestIdx];
				Type tp = blocks[cur.second][cur.first].getType();

				if (tp != Type::START && tp != Type::END)
					blocks[cur.second][cur.first].setType(types[5]);

				shortestIdx++;

			}

		}

	}

	void liveMode() {

		if (GetKey(olc::R).bPressed) reset();

		getInputs();

		int hasBoth = 0;

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {

				if (blocks[curY][curX].getType() == Type::PAHT
				||  blocks[curY][curX].getType() == Type::TRY)
					blocks[curY][curX].setType(types[3]);

				if (blocks[curY][curX].getType() == Type::START ||
				blocks[curY][curX].getType() == Type::END) hasBoth++;

			}
		}		
		
		if (hasBoth < 2) return;

		aStar();

		while (searchIdx < searchPath.size()) {

			std::pair <int, int> cur = searchPath[searchIdx];
			Type tp = blocks[cur.second][cur.first].getType();

			if (tp != Type::START && tp != Type::END)
				blocks[cur.second][cur.first].setType(types[4]);

			searchIdx++;
		}

		while (shortestIdx < shortestPath.size()) {

			std::pair <int, int> cur = shortestPath[shortestIdx];
			Type tp = blocks[cur.second][cur.first].getType();

			if (tp != Type::START && tp != Type::END)
				blocks[cur.second][cur.first].setType(types[5]);

			shortestIdx++;

		}

		searchPath.clear();
		shortestPath.clear();

		searchIdx = 0;
		shortestIdx = 0;

	}

	// maze generation

	void generateRandomBlocks() {

		bool hasStart = false;
		bool hasEnd = false;

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {
				
				if (blocks[curY][curX].getType() == Type::START)
					hasStart = true;

				if (blocks[curY][curX].getType() == Type::END)
					hasEnd = true;
			
			}
		}
	

		if(!hasStart) blocks[vsh - 1][0].setType(types[0]);
		if(!hasEnd) blocks[0][vsw - 1].setType(types[1]);
		
		std::vector <std::pair <int, int> > cords;

		for (int curY = 0; curY < vsh; curY++) {
			for (int curX = 0; curX < vsw; curX++) {
				cords.push_back({ curX, curY });
			}
		}

		auto rng = std::default_random_engine{};
		std::shuffle(cords.begin(), cords.end(), rng);

		for (int k = 0; k < cords.size(); k++) {

			int curX = cords[k].first;
			int curY = cords[k].second;

			Type tp = blocks[curY][curX].getType();

			bool spotGood = (tp == Type::NONE || tp == Type::PAHT || tp == Type::TRY);

			if (!spotGood || rand() % 2) continue;

			blocks[curY][curX].setType(types[2]);

			aStar(true);

			if (shortestPath.size() == 0)
				blocks[curY][curX].setType(types[3]);

			searchPath.clear();
			shortestPath.clear();

		}

		

	}

public:

	bool OnUserCreate() override {

		std::cout << "Press 1 to choose start\n"
				  << "Press 2 to choose end\n"
				  << "Press 3 to choose barrier\n"
				  << "Press 4 to erase block\n"
				  << "Press R to reset\n"
				  << "Press S to start\n"
	   			  << "Press L to switch between live mode and normal mode\n"
				  << "Press F to switch between bfs and A* (live mode only uses A*)\n"
				  << "Press G to generate a random possible path\n";

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {

		if (GetKey(olc::G).bPressed) {
			generateRandomBlocks();
		}

		if (GetKey(olc::L).bPressed) {
			currentMode = abs(1 - currentMode);
			reset();
		}

		if (GetKey(olc::F).bPressed) {
			currentModeFunc = abs(1 - currentModeFunc);
		}

		if(currentMode) normalMode();
		else liveMode();

		for (int y = 0; y < vsh; y++) {
			for (int x = 0; x < vsw; x++) {
				draw(x * blockSize, y * blockSize, blocks[y][x].getColor());
			}
		}

		return true;
	
	}
};

int main() {

	App app;

	if (app.Construct(sw + 1, sh + 1, pixelSize, pixelSize))
		app.Start();

	return 0;

}
