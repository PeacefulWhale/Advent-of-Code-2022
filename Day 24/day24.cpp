#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// This is all just copy / pasted from day 22.
#define MAX_SIZE 1000
#define EMPTY '.'
#define UP '^'
#define DOWN 'v'
#define RIGHT '>'
#define LEFT '<'

int wrap(int number, int min, int max)
{
    if (number < min) return (max + 1) - (min - number);
    else if (number > max) return (min - 1) + (number - max);
    else return number;
}

struct pos
{
    int x = 0;
    int y = 0;
    bool operator==(const pos other) const
    {
        return (x == other.x && y == other.y);
    }
    bool operator>(const pos other) const
    {
        return (x > other.x && y > other.y);
    }
    bool operator>=(const pos other) const
    {
        return (x >= other.x && y >= other.y);
    }
    bool operator<(const pos other) const
    {
        return (x < other.x &&y < other.y);
    }
    bool operator<=(const pos other) const
    {
        return (x <= other.x && y <= other.y);
    }

    pos operator+(const pos other) const
    {
        return { x + other.x, y + other.y };
    }
    pos operator-(const pos other) const
    {
        return { x - other.x, y - other.y };
    }
    pos operator*(const int other) const
    {
        return { x * other, y * other };
    }
    void operator+=(const pos other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(const pos other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator*=(const int other)
    {
        x *= other;
        y *= other;
    }
};

// 2D Map stuff.
template<>
struct std::hash<pos>
{
    std::size_t operator()(const pos &loc) const
    {
        return (loc.x * MAX_SIZE) + loc.y;
    }
};

struct blizzards
{
    // Number of blizzards moving up, down, left, & right.
    int u = 0;
    int d = 0;
    int l = 0;
    int r = 0;
};

int path_find(std::unordered_map<pos, blizzards> current_map, pos start_pos, pos end_pos, pos bounds, int rounds);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    pos bounds;
    std::unordered_map<pos, blizzards> start_map;
    // Read and ignore the first line.
    {
        std::ifstream input_file(argv[1]);
        std::string line;
        // Get the size of the file.
        std::getline(input_file, line);
        bounds.x = line.size() - 2;
        while (std::getline(input_file, line) && line != "")
        {
            bounds.y += 1;
        }
        bounds.y -= 1;
        // Reset the file.
        input_file.clear();
        input_file.seekg(0, input_file.beg);
        // Ignore the first line.
        std::getline(input_file, line);

        for (int y_offset = 0; std::getline(input_file, line); ++y_offset)
        {
            pos temp_pos;
            temp_pos.y = y_offset;
            // Skip the first and last character.
            for (temp_pos.x = 0; temp_pos.x < (int)line.length() - 2; ++temp_pos.x)
            {
                blizzards temp_blizzard;
                switch (line[temp_pos.x + 1])
                {
                case UP:
                    temp_blizzard.u = 1;
                    break;
                case DOWN:
                    temp_blizzard.d = 1;
                    break;
                case RIGHT:
                    temp_blizzard.r = 1;
                    break;
                case LEFT:
                    temp_blizzard.l = 1;
                    break;
                }
                start_map[temp_pos] = temp_blizzard;
            }
        }
    }
    pos start_pos = { 0, -1 };
    pos end_pos = { bounds.x - 1, bounds.y };
    // Path find.
    std::cout << "Part 1: " << path_find(start_map, start_pos, end_pos, bounds, 1) << std::endl;
    std::cout << "Part 2: " << path_find(start_map, start_pos, end_pos, bounds, 3) << std::endl;
    return 0;
}

int path_find(std::unordered_map<pos, blizzards> current_map, pos start_pos, pos end_pos, pos bounds, int rounds)
{
    std::unordered_set<pos> positions;
    positions.insert(start_pos);
    int current_turn = 0;
    pos zero_bounds = { 0, 0 };
    int round = 0;
    while (true)
    {
        // Update positions.
        std::unordered_set<pos> new_positions;
        for (auto current_pos : positions)
        {
            if (current_pos == end_pos)
            {
                // Pathfinding is complete.
                // But if rounds remain we must continue.
                round += 1;
                // Swap end_pos and start_pos;
                pos temp = start_pos;
                start_pos = end_pos;
                end_pos = temp;
                new_positions.clear();
                new_positions.insert(start_pos);
                break;
            }
            // If this current position has a blizzard in it, it is not valid.
            if (current_map.contains(current_pos))
            {
                blizzards temp = current_map.at(current_pos);
                if (temp.u || temp.d || temp.r || temp.l)
                {
                    // Not a valid position.
                    continue;
                }
            }
            // Only look at moving into bounds.
            if ((current_pos >= zero_bounds && current_pos < bounds) || current_pos == start_pos)
            {
                // The position is in bounds, and has no blizzards present.
                // Generate new possible positions.
                new_positions.insert(current_pos);
                new_positions.insert(current_pos + (pos) { 1, 0 });
                new_positions.insert(current_pos + (pos) { -1, 0 });
                new_positions.insert(current_pos + (pos) { 0, 1 });
                new_positions.insert(current_pos + (pos) { 0, -1 });
            }
        }
        if (round == rounds)
        {
            // The walking is over.
            break;
        }

        positions = new_positions;
        // Move blizzards.
        std::unordered_map<pos, blizzards> new_map;
        for (int x = 0; x < bounds.x; ++x)
        {
            for (int y = 0; y < bounds.y; ++y)
            {
                pos temp = { x, y };
                if (current_map.contains(temp))
                {
                    blizzards temp_b = current_map.at(temp);
                    pos left, right, up, down;

                    // Calculate positions of moving blizzards.
                    left = temp;
                    left.x = wrap(left.x - 1, 0, bounds.x - 1);
                    right = temp;
                    right.x = wrap(right.x + 1, 0, bounds.x - 1);
                    up = temp;
                    up.y = wrap(up.y - 1, 0, bounds.y - 1);
                    down = temp;
                    down.y = wrap(down.y + 1, 0, bounds.y - 1);

                    // Update new map.
                    new_map[left].l = temp_b.l;
                    new_map[right].r = temp_b.r;
                    new_map[up].u = temp_b.u;
                    new_map[down].d = temp_b.d;
                }
            }
        }
        current_map = new_map;
        current_turn += 1;
    }
    return current_turn;
}
