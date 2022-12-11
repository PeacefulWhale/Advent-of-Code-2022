#include <iostream>
#include <fstream>
#include <vector>

struct pos
{
    int x, y;
};

struct location
{
    pos position;
    int count;
};

struct rope_segment
{
    pos position;
    rope_segment *next;
};

int sign(int x);
int insert_location(std::vector<location> *locations, pos loc);
void simulate(rope_segment *rope, std::vector<location> *locations, char dir, int count);
rope_segment *new_rope(int n);
void free_rope(rope_segment *rope);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }

    // Starting Values.
    location start;
    std::vector<location> locations_part1;
    rope_segment *rope_part1 = new_rope(2);
    std::vector<location> locations_part2;
    rope_segment *rope_part2 = new_rope(10);
    start.count = start.position.x = start.position.y = 0;
    locations_part1.push_back(start);
    locations_part2.push_back(start);

    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;

    char dir;
    int count;
    while (input_file >> dir >> count)
    {
        simulate(rope_part1, &locations_part1, dir, count);
        simulate(rope_part2, &locations_part2, dir, count);
    }

    std::cout << "Part 1: " << locations_part1.size() << std::endl;
    std::cout << "Part 2: " << locations_part2.size() << std::endl;
    free_rope(rope_part1);
    free_rope(rope_part2);
}

int insert_location(std::vector<location> *locations, pos loc)
{
    location temp;
    temp.position = loc;
    temp.count = 1;
    int i;
    for (i = 0; i < (int)locations->size(); ++i)
    {
        if (locations->at(i).position.x == loc.x && locations->at(i).position.y == loc.y)
        {
            locations->at(i).count += 1;
            return 0;
        }
        // Sort by X and then Y
        if (loc.x < locations->at(i).position.x)
        {
            if (loc.x < locations->at(i).position.y)
            {
                break;
            }
        }
    }
    locations->insert(locations->begin() + i, temp);
    return 0;
}

rope_segment *new_rope(int n)
{
    if (n)
    {
        rope_segment *temp;
        temp = (rope_segment *)malloc(sizeof * temp);
        temp->position.x = temp->position.y = 0;
        temp->next = new_rope(n - 1);
        return temp;
    }
    else
    {
        return nullptr;
    }
}

void free_rope(rope_segment *rope)
{
    if (rope)
    {
        free(rope);
        free_rope(rope->next);
    }
}

void simulate(rope_segment *rope, std::vector<location> *locations, char dir, int count)
{
    for (int i = 0; i < count; ++i)
    {
        rope_segment *head = rope;
        rope_segment *tail = head->next;
        if (dir == 'R')
        {
            head->position.x += 1;
        }
        else if (dir == 'L')
        {
            head->position.x -= 1;
        }
        else if (dir == 'U')
        {
            head->position.y += 1;
        }
        else if (dir == 'D')
        {
            head->position.y -= 1;
        }
        // Update the tail.
        while (tail != nullptr)
        {
            pos dif;
            dif.x = head->position.x - tail->position.x;
            dif.y = head->position.y - tail->position.y;
            // Check to see if the tail is far enough away from the head.
            if (abs(dif.x) > 1 || abs(dif.y) > 1)
            {
                // Update tail position.
                if (abs(dif.x) > 1 && dif.y == 0)
                {
                    tail->position.x += sign(dif.x);
                }
                else if (abs(dif.y) > 1 && dif.x == 0)
                {
                    tail->position.y += sign(dif.y);
                }
                else if (abs(dif.y) + abs(dif.x) > 2)
                {
                    tail->position.x += sign(dif.x);
                    tail->position.y += sign(dif.y);
                }
                // Update head / tail.
                head = head->next;
                tail = head->next;
            }
            else
            {
                break;
            }
        }
        // Get the end piece.
        head = rope;
        while (head->next)
        {
            head = head->next;
        }
        insert_location(locations, head->position);
    }
}

int sign(int x)
{
    // Apparently faster than an if-statement.
    return (x > 0) - (x < 0);
}
