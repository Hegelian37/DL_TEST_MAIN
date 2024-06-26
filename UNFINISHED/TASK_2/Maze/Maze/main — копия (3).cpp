#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <stack>
#include <queue>
#include <algorithm>

using namespace std;

enum CellType
{
    WALL = '#',
    ROAD = '.',
    ENTRANCE = 'O',
    EXIT = 'X',
    TREASURE = '$',
    TRAP = 'T'
};

struct Coords
{
    int x, y;
};

class Maze
{
public:
    Maze(int size, int seed = -1) : size_(size), maze(size, vector<char>(size, WALL)), random_engine(seed >= 0 ? seed : random_device()())
    {
        generate();
        placeEntranceAndExit();
        placeTreasureAndTraps();
    }

    void display() const
    {
        for (const auto& row : maze)
        {
            for (char cell : row)
            {
                cout << cell << ' ';
            }
            cout << '\n';
        }
    }

private:
    int size_;
    vector<vector<char>> maze;
    mt19937 random_engine;

    void generate()
    {
        stack<Coords> stack;
        Coords start = { 1, 1 };
        maze[start.y][start.x] = ROAD;
        stack.push(start);

        while (!stack.empty())
        {
            Coords current = stack.top();
            stack.pop();

            array<Coords, 4> directions = shuffleDirections();

            for (const Coords& dir : directions)
            {
                Coords next = { current.x + dir.x * 2, current.y + dir.y * 2 };

                if (isValid(next) && maze[next.y][next.x] == WALL)
                {
                    maze[current.y + dir.y][current.x + dir.x] = ROAD;
                    maze[next.y][next.x] = ROAD;
                    stack.push(next);
                }
            }
        }
    }

    array<Coords, 4> shuffleDirections()
    {
        array<Coords, 4> directions = { {{0, -1}, {1, 0}, {0, 1}, {-1, 0}} };
        shuffle(directions.begin(), directions.end(), random_engine);
        return directions;
    }

    bool isValid(const Coords& coords) const
    {
        return coords.x > 0 && coords.y > 0 && coords.x < size_ - 1 && coords.y < size_ - 1;
    }

    void placeEntranceAndExit()
    {
        maze[1][1] = ENTRANCE;
        maze[size_ - 2][size_ - 2] = ROAD;
        maze[size_ - 2][size_ - 1] = EXIT;
    }

    void placeTreasureAndTraps()
    {
        uniform_int_distribution<int> trap_dist(3, 5);
        uniform_int_distribution<int> treasure_dist(0, 1);
        int traps = trap_dist(random_engine);
        int treasure = treasure_dist(random_engine);

        vector<Coords> empty_cells;
        for (int y = 1; y < size_ - 1; y++)
        {
            for (int x = 1; x < size_ - 1; x++)
            {
                if (maze[y][x] == ROAD)
                {
                    empty_cells.push_back({ x, y });
                }
            }
        }

        shuffle(empty_cells.begin(), empty_cells.end(), random_engine);

        if (treasure && !empty_cells.empty())
        {
            Coords pos = empty_cells.back();
            maze[pos.y][pos.x] = TREASURE;
            empty_cells.pop_back();
        }

        for (int i = 0; i < traps && !empty_cells.empty(); i++)
        {
            Coords pos = empty_cells.back();
            maze[pos.y][pos.x] = TRAP;
            empty_cells.pop_back();
        }
    }
};

int main()
{
    int size;
    cout << "Enter size of the maze - single dimension: ";
    cin >> size;
    if (size < 5)
    {
        cout << "Too small size!";
        return 1;
    }

    Maze maze(size);
    maze.display();

    return 0;
}
