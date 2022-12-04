#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    int score[2];
    score[0] = score[1] = 0;
    while (std::getline(input_file, line))
    {
        // Parse line.
        int i, j, x, y;
        char c;
        std::stringstream ss(line);
        ss >> i >> c >> j >> c >> x >> c >> y;

        // Part 1:
        // In how many assignment pairs does one range fully contain the other?
        if ((i <= x && j >= y) || (i >= x && j <= y))
        {
            score[0]++;
            score[1]++;
        }

        //Part 2:
        // In how many assignment pairs do the ranges overlap?
        else if (y - i <= (j - i) + (y - x))
        {
            score[1]++;
        }
    }

    std::cout << "Part 1: " << score[0] << std::endl;
    std::cout << "Part 2: " << score[1] << std::endl;
    return 0;
}