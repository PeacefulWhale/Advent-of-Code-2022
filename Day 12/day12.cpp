#include <iostream>
#include <fstream>
#include <queue>

struct pos
{
    char elevation;
    int x, y;
};

unsigned int path_find(char *map, int width, int height, pos start, pos end, bool part1);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    // Read input to char map.
    char *map;
    int width, height;
    std::getline(input_file, line);
    width = line.size();
    height = 1;
    while (std::getline(input_file, line))
    {
        height += 1;
    }
    map = (char *)malloc(sizeof(*map) * (width * height));
    input_file.clear();
    input_file.seekg(0, std::ios::beg);
    pos start, end;
    start.elevation = 'a';
    end.elevation = 'z';
    for (int y = 0; y < height; ++y)
    {
        std::getline(input_file, line);
        for (int x = 0; x < width; ++x)
        {
            if (line[x] == 'S')
            {
                start.x = x;
                start.y = y;
                map[x + (y * width)] = 'a';
            }
            else if (line[x] == 'E')
            {
                end.x = x;
                end.y = y;
                map[x + (y * width)] = 'z';
            }
            else
            {
                map[x + (y * width)] = line[x];
            }
        }
    }

    // Part 1:
    // BSF algorithm to find closest distance between the S and the E.
    // I don't need to actually generate a tree, just a queue of current nodes.
    std::cout << "Part 1: " << path_find(map, width, height, start, end, true) << std::endl;
    // Part 2:
    // BSF algorithm, but now it starts from the end and exists when it finds an elevation of 'a'.
    std::cout << "Part 2: " << path_find(map, width, height, end, start, false) << std::endl;
    free(map);
    return 0;
}

unsigned int path_find(char *input_map, int width, int height, pos start, pos end, bool part1)
{
    char *map;
    map = (char *)malloc(sizeof(*map) * (width * height));
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            map[x + (y * width)] = input_map[x + (y * width)];
        }
    }
    int path_length = 0;
    std::queue<pos> locations;
    locations.push(start);
    bool finished = false;
    while (!finished)
    {
        std::queue<pos> new_locations;
        while (!locations.empty() && !finished)
        {
            pos current_loc = locations.front();
            locations.pop();
            // Expand current locations.
            for (int y = -1; y <= 1 && !finished; ++y)
            {
                for (int x = -1; x <= 1 && !finished; ++x)
                {
                    if (!(x && y) && (current_loc.x + x >= 0 && current_loc.x + x < width) && (current_loc.y + y >= 0 && current_loc.y + y < height))
                    {
                        // Check to see if the search is over.
                        // Part 1 logic.
                        if (part1)
                        {
                            if ('z' <= current_loc.elevation + 1 && current_loc.x + x == end.x && current_loc.y + y == end.y)
                            {
                                finished = true;
                            }
                        }
                        // Part 2 logic.
                        else
                        {
                            if ('a' >= current_loc.elevation - 1)
                            {
                                finished = true;
                            }
                        }
                        char elevation = map[(current_loc.x + x) + ((current_loc.y + y) * width)];
                        // Part 1 / Part 2 move in different directions.
                        if (elevation != '.' && ((part1 && elevation <= current_loc.elevation + 1) || (!part1 && elevation >= current_loc.elevation - 1)))
                        {
                            // This is a valid location to move to.
                            pos new_loc;
                            new_loc.elevation = elevation;
                            new_loc.x = current_loc.x + x;
                            new_loc.y = current_loc.y + y;
                            new_locations.push(new_loc);
                            map[(current_loc.x + x) + ((current_loc.y + y) * width)] = '.';
                        }
                    }
                }
            }
        }
        path_length += 1;
        locations = new_locations;
    }
    free(map);
    return path_length;
}
