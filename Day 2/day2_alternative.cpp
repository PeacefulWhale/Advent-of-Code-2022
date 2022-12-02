#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    char line[4];
    int score[2];
    score[0] = score[1] = 0;
    if (input_file.is_open())
    {
        while (input_file.getline(line, 4))
        {
            // Part 1:
            int i = line[0] - 'A';
            int j = line[2] - 'X';
            score[0] += j + 1;
            if (i == j)
            {
                score[0] += 3;
            }
            else if ((i + 1) % 3 == j)
            {
                score[0] += 6;
            }

            // Part 2:
            if (j == 0)
            {
                score[1] += (((i + 2) % 3) + 1);
            }
            else if (j == 1)
            {
                score[1] += 3;
                score[1] += i + 1;
            }
            else
            {
                score[1] += 6;
                score[1] += (((i + 1) % 3) + 1);
            }
        }
    }

    std::cout << "Part 1: " << score[0] << std::endl;
    std::cout << "Part 2: " << score[1] << std::endl;
    return 0;
}