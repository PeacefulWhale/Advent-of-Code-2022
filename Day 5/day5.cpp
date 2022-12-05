#include <iostream>
#include <fstream>
#include <stack>
#include <sstream>

void part_1_move(std::stack<char> *stacks, int n, int x, int y);
void part_2_move(std::stack<char> *stacks, int n, int x, int y);
std::stack<char> reverse(std::stack<char> x);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    std::getline(input_file, line);
    input_file.seekg(0, input_file.beg);
    int stack_count = 0;
    stack_count = (line.length() + 2) / 4;

    // Get stacks.
    std::stack<char> stacks[stack_count];
    while (std::getline(input_file, line) && line != "")
    {
        for (int i = 0; i < (int)line.length(); i++)
        {
            char letter = line[i];
            if (letter >= 'A' && letter <= 'Z')
            {
                stacks[i / 4].push(letter);
            }
        }
    }

    std::stack<char> stacks_1[stack_count];
    std::stack<char> stacks_2[stack_count];
    for (int i = 0; i < stack_count; i++)
    {
        stacks[i] = reverse(stacks[i]);
        stacks_1[i] = stacks[i];
        stacks_2[i] = stacks[i];
    }

    // Parse instructions.
    while (std::getline(input_file, line))
    {
        std::string temp;
        int n, x, y;
        std::stringstream ss(line);
        ss >> temp >> n >> temp >> x >> temp >> y;
        part_1_move(stacks_1, n, x - 1, y - 1);
        part_2_move(stacks_2, n, x - 1, y - 1);
    }
    // Part 1:
    // After the rearrangement procedure completes, what crate ends up on top of each stack?
    std::cout << "Part 1: ";
    for (int i = 0; i < stack_count; i++)
    {
        std::cout << stacks_1[i].top();
    }
    std::cout << std::endl;

    // Part 2:
    // After the rearrangement procedure completes, what crate ends up on top of each stack?
    std::cout << "Part 2: ";
    for (int i = 0; i < stack_count; i++)
    {
        std::cout << stacks_2[i].top();
    }
    std::cout << std::endl;
    return 0;
}

void part_1_move(std::stack<char> *stacks, int n, int x, int y)
{
    for (int i = 0; i < n; i++)
    {
        stacks[y].push(stacks[x].top());
        stacks[x].pop();
    }
}

void part_2_move(std::stack<char> *stacks, int n, int x, int y)
{
    std::stack<char> temp;
    for (int i = 0; i < n; i++)
    {
        temp.push(stacks[x].top());
        stacks[x].pop();
    }
    for (int i = 0; i < n; i++)
    {
        stacks[y].push(temp.top());
        temp.pop();
    }
}

std::stack<char> reverse(std::stack<char> x)
{
    std::stack<char> y;
    while (!x.empty())
    {
        y.push(x.top());
        x.pop();
    }
    return y;
}
