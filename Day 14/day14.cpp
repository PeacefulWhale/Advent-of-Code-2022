#include <iostream>
#include <fstream>
#include <sstream>

struct pos
{
    int x, y;
};

void draw_line(char *map, int width, pos one, pos two);
int simulate(char *map, int width, int floor, pos start, bool part_one);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    // I'm just going to assume that these numbers are large enough...
    int width = 1000;
    int height = 1000;
    int floor = 0;
    char *map;
    map = (char *)malloc(sizeof(map) * width * height);
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            map[x + (y * width)] = ' ';
        }
    }

    while (std::getline(input_file, line))
    {
        std::stringstream ss(line);
        char temp_char;
        std::string temp_str;
        pos old_point;
        ss >> old_point.x >> temp_char >> old_point.y;
        while (ss >> temp_str)
        {
            pos new_point;
            ss >> new_point.x >> temp_char >> new_point.y;
            // Update the floor (for later use in the simulation).
            if (new_point.y > floor)
            {
                floor = new_point.y;
            }
            // Add line to map.
            draw_line(map, width, old_point, new_point);
            // Make the new point the old point.
            old_point = new_point;
        }
    }
    pos start;
    start.x = 500;
    start.y = 0;
    // Part 1:
    std::cout << "Part 1: " << simulate(map, width, floor, start, true) << std::endl;
    // Part 2:
    // Reset sand.
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            if (map[x + (y * width)] == 'o')
            {
                map[x + (y * width)] = ' ';
            }
        }
    }
    // Add line.
    for (int x = 0; x < width; ++x)
    {
        map[x + ((floor + 2) * width)] = '#';
    }
    std::cout << "Part 2: " << simulate(map, width, floor, start, false) << std::endl;
    free(map);
    return 0;
}

void draw_map(char *map, int width, pos top_left, pos bot_right)
{
    for (int y = top_left.y; y < bot_right.y; ++y)
    {
        for (int x = top_left.x; x < bot_right.x; ++x)
        {
            std::cout << map[x + (y * width)];
        }
        std::cout << std::endl;
    }
}

void draw_line(char *map, int width, pos one, pos two)
{
    // The direction that the lines are being drawn in is important.
    int x_dir = (one.x < two.x) ? 1 : -1;
    int y_dir = (one.y < two.y) ? 1 : -1;
    int x = one.x;
    int y = one.y;
    map[x + (y * width)] = '#';
    do
    {
        if (x != two.x)
        {
            x += x_dir;
        }
        if (y != two.y)
        {
            y += y_dir;
        }
        map[x + (y * width)] = '#';
    } while (x != two.x || y != two.y);
}


int simulate(char *map, int width, int floor, pos start, bool part_one)
{
    int sand_count = 0;
    while (true)
    {
        bool settled = false;
        pos grain = start;
        while (!settled)
        {
            // Part one end condition.
            if (part_one && grain.y > floor)
            {
                return sand_count;
            }
            else if (map[grain.x + ((grain.y + 1) * width)] == ' ')
            {
                // Grain can move down.
                grain.y += 1;
            }
            else
            {
                // Check the left side.
                if (map[(grain.x - 1) + ((grain.y + 1) * width)] == ' ')
                {
                    grain.x -= 1;
                    grain.y += 1;
                }
                // Check the right side.
                else if (map[(grain.x + 1) + ((grain.y + 1) * width)] == ' ')
                {
                    grain.x += 1;
                    grain.y += 1;
                }
                else
                {
                    // Grain cannot move any further down.
                    map[grain.x + (grain.y * width)] = 'o';
                    sand_count += 1;
                    settled = true;
                    // Part two end condition.
                    if (!part_one && (grain.x = start.x && grain.y == start.y))
                    {
                        return sand_count;
                    }
                }
            }
        }
    }
}
