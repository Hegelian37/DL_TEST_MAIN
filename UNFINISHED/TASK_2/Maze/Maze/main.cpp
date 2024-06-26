#include <iostream>
#include <vector>
#include <random>
#include <stack>
#include <algorithm>
#include <queue>
#include <tuple>

using namespace std;

enum class CellType
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
        Maze(int size, int seed = -1) : size_(size), maze(size, vector<char>(size, static_cast<char>(CellType::WALL))), random_engine(seed >= 0 ? seed : random_device()())
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

        void walker()
        {
            Coords start = findCoordsOf(CellType::ENTRANCE);
            Coords exit = findCoordsOf(CellType::EXIT);

            queue<Coords> q;
            vector<vector<bool>> visited(size_, vector<bool>(size_, false));
            vector<vector<Coords>> parent(size_, vector<Coords>(size_, { -1, -1 }));

            q.push(start);
            visited[start.y][start.x] = true;

            while (!q.empty())
            {
                Coords current = q.front();
                q.pop();

                if (current.x == exit.x && current.y == exit.y)
                {
                    break;
                }

                vector<Coords> neighbors = getNeighbors(current);
                for (const auto& neighbor : neighbors)
                {
                    if (!visited[neighbor.y][neighbor.x] && maze[neighbor.y][neighbor.x] != static_cast<char>(CellType::WALL))
                    {
                        visited[neighbor.y][neighbor.x] = true;
                        parent[neighbor.y][neighbor.x] = current;
                        q.push(neighbor);
                    }
                }
            }

            Coords current = exit;
            while (!(current.x == start.x && current.y == start.y))
            {
                Coords prev = parent[current.y][current.x];
                if (maze[current.y][current.x] == static_cast<char>(CellType::TRAP))
                {
                    maze[current.y][current.x] = static_cast<char>(CellType::ROAD);
                }
                current = prev;
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
            maze[start.y][start.x] = static_cast<char>(CellType::ROAD);
            path.push(start);

            while (!path.empty())
            {
                Coords current = path.top();
                vector<Coords> neighbors = getNeighbors(current);

                bool moved = false;
                shuffle(neighbors.begin(), neighbors.end(), random_engine);

                for (const auto& neighbor : neighbors)
                {
                    if (isValid(neighbor) && maze[neighbor.y][neighbor.x] == static_cast<char>(CellType::WALL))
                    {
                        Coords between = { current.x + (neighbor.x - current.x) / 2, current.y + (neighbor.y - current.y) / 2 };
                        maze[between.y][between.x] = static_cast<char>(CellType::ROAD);
                        maze[neighbor.y][neighbor.x] = static_cast<char>(CellType::ROAD);
                        path.push(neighbor);
                        moved = true;
                        break;
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
                if (isValid(neighbor)) {
                    neighbors.push_back(neighbor);
                }
            }

            return neighbors;
        }

        bool isValid(const Coords& coords) const
        {
            return coords.x >= 0 && coords.y >= 0 && coords.x < size_ && coords.y < size_;
        }

        Coords findCoordsOf(CellType type) const
        {
            for (int y = 0; y < size_; ++y)
            {
                for (int x = 0; x < size_; ++x)
                {
                    if (maze[y][x] == static_cast<char>(type))
                    {
                        return { x, y };
                    }
                }
            }
            return { -1, -1 };
        }

        void placeEntranceAndExit()
        {
            maze[1][1] = static_cast<char>(CellType::ENTRANCE);
            if (size_ % 2 != 0)
            {
                maze[size_ - 2][size_ - 2] = static_cast<char>(CellType::ROAD);
                maze[size_ - 2][size_ - 1] = static_cast<char>(CellType::EXIT);
            }
            else
            {
                maze[size_ - 2][size_ - 3] = static_cast<char>(CellType::ROAD);
                maze[size_ - 2][size_ - 2] = static_cast<char>(CellType::EXIT);
            }
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
                    if (maze[y][x] == static_cast<char>(CellType::ROAD))
                    {
                        empty_cells.push_back({ x, y });
                    }
                }
            }

            shuffle(empty_cells.begin(), empty_cells.end(), random_engine);

            if (treasure && !empty_cells.empty())
            {
                Coords pos = empty_cells.back();
                maze[pos.y][pos.x] = static_cast<char>(CellType::TREASURE);
                empty_cells.pop_back();
            }

            for (int i = 0; i < traps && !empty_cells.empty(); i++)
            {
                Coords pos = empty_cells.back();
                maze[pos.y][pos.x] = static_cast<char>(CellType::TRAP);
                empty_cells.pop_back();
            }
        }

        void createOuterWalls()
        {
            for (int x = 0; x < size_; x++)
            {
                maze[0][x] = static_cast<char>(CellType::WALL);
                maze[size_ - 1][x] = static_cast<char>(CellType::WALL);
            }
            for (int y = 0; y < size_; y++)
            {
                maze[y][0] = static_cast<char>(CellType::WALL);
                maze[y][size_ - 1] = static_cast<char>(CellType::WALL);
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
        cout << "\nSize is too small. Write a correct input.\n";
        main();
    }

    Maze maze(size);
    maze.walker();
    maze.display();

    system("pause");
    return 0;
}

