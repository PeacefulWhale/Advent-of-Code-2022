#include <iostream>
#include <fstream>
#include <sstream>

#define MAX_SIZE 30
#define EMPTY ' '
#define LAVA '@'
#define STEAM '.'

char map[MAX_SIZE][MAX_SIZE][MAX_SIZE];

void expand_steam(int x, int y, int z);
int count_area(char check, int max_x, int max_y, int max_z);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    int max_x, max_y, max_z;
    max_x = max_y = max_z = 0;
    std::ifstream input_file(argv[1]);
    std::string line;

    // Reset map.
    for (int x = 0; x < MAX_SIZE; ++x)
    {
        for (int y = 0; y < MAX_SIZE; ++y)
        {
            for (int z = 0; z < MAX_SIZE; ++z)
            {
                map[x][y][z] = EMPTY;
            }
        }
    }

    while (std::getline(input_file, line))
    {
        int x, y, z;
        char temp;
        std::stringstream ss(line);
        ss >> x >> temp >> y >> temp >> z;
        // +1 so I don't have to worry about boundary checks...
        x += 1;
        y += 1;
        z += 1;
        map[x][y][z] = LAVA;
        if (x > max_x)
        {
            max_x = x;
        }
        if (y > max_y)
        {
            max_y = y;
        }
        if (z > max_z)
        {
            max_z = z;
        }
    }
    // Part 1. Just count sides not touching lava.
    std::cout << "Part 1: " << count_area(EMPTY, max_x, max_y, max_z) << std::endl;
    // Part 2. Expand with steam.
    expand_steam(0, 0, 0);
    // Count the sides touching steam.
    std::cout << "Part 2: " << count_area(STEAM, max_x, max_y, max_z) << std::endl;
    return 0;
}

int count_area(char check, int max_x, int max_y, int max_z)
{
    int count = 0;
    for (int x = 1; x <= max_x; ++x)
    {
        for (int y = 1; y <= max_y; ++y)
        {
            for (int z = 1; z <= max_z; ++z)
            {
                if (map[x][y][z] == LAVA)
                {
                    // Count exposed sides.
                    if (map[x + 1][y][z] == check) { ++count; }
                    if (map[x - 1][y][z] == check) { ++count; }
                    if (map[x][y + 1][z] == check) { ++count; }
                    if (map[x][y - 1][z] == check) { ++count; }
                    if (map[x][y][z + 1] == check) { ++count; }
                    if (map[x][y][z - 1] == check) { ++count; }
                }
            }
        }
    }
    return count;
}

void expand_steam(int x, int y, int z)
{
    if ((x >= 0 && x < MAX_SIZE) && (y >= 0 && y < MAX_SIZE) && (z >= 0 && z < MAX_SIZE))
    {
        if (map[x][y][z] == EMPTY)
        {
            map[x][y][z] = STEAM;
            expand_steam(x + 1, y, z);
            expand_steam(x - 1, y, z);
            expand_steam(x, y + 1, z);
            expand_steam(x, y - 1, z);
            expand_steam(x, y, z + 1);
            expand_steam(x, y, z - 1);
        }
    }
}
