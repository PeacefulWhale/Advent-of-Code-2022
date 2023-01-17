#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include <iostream>

// This is all just copy / pasted from day 22.
#define MAX_SIZE 10000
#define ELF '#'

struct pos
{
    int x = 0;
    int y = 0;
    bool operator==(const pos other) const
    {
        return (x == other.x && y == other.y);
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

// Simulation function.
int simulate(std::unordered_set<pos> current_pos, int turns, bool part_one);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Oh yeah... You can just have different scopes whenever, neat.
    // Parse input.
    std::unordered_set<pos> start_pos;
    {
        std::ifstream input_file(argv[1]);
        std::string line;
        for (int y_offset = 0; std::getline(input_file, line); ++y_offset)
        {
            pos temp;
            temp.y = y_offset;
            for (temp.x = 0; temp.x < (int)line.length(); ++temp.x)
            {
                if (line[temp.x] == ELF)
                {
                    start_pos.insert(temp);
                }
            }
        }
    }

    std::cout << "Part 1: " << simulate(start_pos, 10, true) << std::endl;
    std::cout << "Part 2: " << simulate(start_pos, 100000, false) << std::endl;
    return 0;
}

int simulate(std::unordered_set<pos> current_pos, int turns, bool part_one)
{
    int min_x, min_y, max_x, max_y;
    bool finished = false;
    int turn;
    for (turn = 0; turn < turns && !finished; ++turn)
    {
        finished = true;
        min_x = MAX_SIZE;
        min_y = MAX_SIZE;
        max_x = -MAX_SIZE;
        max_y = -MAX_SIZE;
        std::unordered_map<pos, pos> proposed_pos;
        std::unordered_map<pos, int> movement_counts;
        for (auto elf_itr = current_pos.begin(); elf_itr != current_pos.end(); ++elf_itr)
        {
            pos elf = *elf_itr;
            pos old_pos = elf;
            pos north, east;
            north.y = -1;
            east.x = 1;
            //  N
            // W E
            //  S
            bool N = !current_pos.contains(elf + north);
            bool NE = !current_pos.contains(elf + north + east);
            bool E = !current_pos.contains(elf + east);
            bool SE = !current_pos.contains(elf - north + east);
            bool S = !current_pos.contains(elf - north);
            bool SW = !current_pos.contains(elf - north - east);
            bool W = !current_pos.contains(elf - east);
            bool NW = !current_pos.contains(elf + north - east);

            // Does the elf need to move?
            if (!(N && NE && E && SE && S && SW && W && NW))
            {
                // Because the elves propose different directions each turn...
                std::pair<bool, pos> movements[4] = {
                    {N && NE && NW, elf + north},
                    {S && SE && SW, elf - north},
                    {W && NW && SW, elf - east},
                    {E && NE && SE, elf + east}
                };

                // Go through the different possible proposals according to the current turn % 4.
                for (int i = 0; i < 4; ++i)
                {
                    if (movements[(i + turn) % 4].first)
                    {
                        elf = movements[(i + turn) % 4].second;
                        break;
                    }
                }
                finished = false;
            }

            // Add the elf's current position and proposed position.
            proposed_pos[old_pos] = elf;
            movement_counts[elf] += 1;
        }

        // Go through all the proposed positions, and see if they can move.
        std::unordered_set<pos> new_pos;
        for (auto itr : proposed_pos)
        {
            pos elf;
            if (movement_counts.at(itr.second) == 1)
            {
                // Elf can move here.
                elf = itr.second;
            }
            else
            {
                // Elf cannot move here.
                elf = itr.first;
            }
            new_pos.insert(elf);
            if (part_one)
            {
                // Update min/max.
                if (elf.x < min_x)
                {
                    min_x = elf.x;
                }
                else if (elf.x > max_x)
                {
                    max_x = elf.x;
                }
                if (elf.y < min_y)
                {
                    min_y = elf.y;
                }
                else if (elf.y > max_y)
                {
                    max_y = elf.y;
                }
            }
        }
        // Only actually "move" if the simulation must continue.
        if (!finished)
        {
            current_pos = new_pos;
        }
    }

    // And that's it, the simulation is over.
    if (part_one)
    {
        // Return the number of empty spaces in the smallest rectangle.
        return (((max_x - min_x) + 1) * ((max_y - min_y) + 1)) - current_pos.size();
    }
    else
    {
        // Return the current turn.
        return turn;
    }
}
