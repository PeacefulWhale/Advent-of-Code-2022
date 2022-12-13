#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }

    std::ifstream input_file(argv[1]);
    std::string line;

    // Build up the register.
    int max_cycles = 240;
    int reg[max_cycles];
    for (int i = 0; i < max_cycles; ++i)
    {
        reg[i] = 0;
    }
    int cycle = 0;
    while (input_file >> line && cycle < max_cycles)
    {
        if (line[0] != 'n')
        {
            int value;
            input_file >> value;
            cycle += 2;
            reg[cycle] = value;
        }
        else
        {
            cycle += 1;
        }
    }
    // Add up the register values.
    reg[0] = 1;
    for (int i = 0; i < max_cycles - 1; ++i)
    {
        reg[i + 1] += reg[i];
    }

    // Part 1:
    int part1 = 0;
    for (int i = 20; i <= cycle; i += 40)
    {
        part1 += i * reg[i - 1];
    }
    std::cout << "Part 1: " << part1 << std::endl;

    // Part 2:
    std::cout << "Part 2: " << std::endl;
    for (int y = 0; y < max_cycles; y += 40)
    {
        for (int x = 0; x < 40; ++x)
        {
            if (x >= reg[y + x] - 1 && x <= reg[y + x] + 1)
            {
                std::cout << "#";
            }
            else
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    return 0;
}
