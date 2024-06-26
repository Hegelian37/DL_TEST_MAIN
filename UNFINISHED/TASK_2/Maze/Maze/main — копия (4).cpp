#include <iostream>
#include <vector>
#include <random>
#include <stack>
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
        placeTreasureAndTraps();
        if (size_ % 2 == 0) createOuterWalls();
        placeEntranceAndExit();
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
        stack<Coords> path;
        Coords start = { 1, 1 };
        maze[start.y][start.x] = ROAD;
        path.push(start);

        while (!path.empty())
        {
            Coords current = path.top();
            vector<Coords> neighbors = getNeighbors(current);

            bool moved = false;
            shuffle(neighbors.begin(), neighbors.end(), random_engine);

            for (const auto& neighbor : neighbors)
            {
                if (isValid(neighbor) && maze[neighbor.y][neighbor.x] == WALL && size_ % 2 != 0)
                {
                    Coords between = { current.x + (neighbor.x - current.x) / 2, current.y + (neighbor.y - current.y) / 2 };
                    maze[between.y][between.x] = ROAD;
                    maze[neighbor.y][neighbor.x] = ROAD;
                    path.push(neighbor);
                    moved = true;
                    break;
                }
                else if (isValid(neighbor) && maze[neighbor.y][neighbor.x] == WALL && size_ % 2 == 0)
                {
                    Coords between = { current.x + (neighbor.x - current.x) / 2, current.y + (neighbor.y - current.y) / 2 };
                    maze[between.y - 1][between.x - 1] = ROAD;
                    maze[neighbor.y][neighbor.x] = ROAD;
                    path.push(neighbor);
                    moved = true;
                    break;
                }
                else
                {
                    cout << "";
                }
            }

            if (!moved)
            {
                path.pop();
            }
        }
    }

    vector<Coords> getNeighbors(const Coords& current) const
    {
        vector<Coords> neighbors;

        vector<Coords> directions = { {0, -2}, {0, 2}, {-2, 0}, {2, 0} };

        for (const auto& dir : directions)
        {
            Coords neighbor = { current.x + dir.x, current.y + dir.y };
            if (isValid(neighbor))
            {
                neighbors.push_back(neighbor);
            }
        }

        return neighbors;
    }

    bool isValid(const Coords& coords) const
    {
        return coords.x >= 0 && coords.y >= 0 && coords.x < size_ && coords.y < size_;
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

    void createOuterWalls()
    {
        for (int x = 0; x < size_; ++x)
        {
            maze[0][x] = WALL;
            maze[size_ - 1][x] = WALL;
        }
        for (int y = 0; y < size_; ++y)
        {
            maze[y][0] = WALL;
            maze[y][size_ - 1] = WALL;
        }
    }
};

int main()
{
    int size;
    cout << "Enter size of the maze - single dimension (minimum 5): ";
    cin >> size;

    if (size < 5)
    {
        cout << "\nSize is too small. Minimum size is 5.\n";
        main();
    }

    Maze maze(size);
    maze.display();
    
    system("pause");
    return 0;
}
