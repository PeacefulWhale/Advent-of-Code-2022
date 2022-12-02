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
    // counts[Enemy][Player]
    int counts[3][3];
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            counts[i][j] = 0;
        }
    }
    if (input_file.is_open())
    {
        while (input_file.getline(line, 4))
        {
            counts[line[0] - 'A'][line[2] - 'X']++;
        }
    }

    // Part 1:
    // X for Rock, Y for Paper, and Z for Scissors.
    int score = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // 1 for Rock, 2 for Paper, and 3 for Scissors.
            score += counts[i][j] * (1 + j);
            // 0 if you lost, 3 if the round was a draw, and 6 if you won.
            if (i == j)
            {
                score += counts[i][j] * 3;
            }
            else if ((i + 1) % 3 == j)
            {
                score += counts[i][j] * 6;
            }
        }
    }
    std::cout << "Part 1: " << score << std::endl;

    // Part 2:
    // X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win.
    score = 0;
    for (int i = 0; i < 3; i++)
    {
        // Need to lose.
        score += counts[i][0] * (((i + 2) % 3) + 1);
        // Need to draw.
        score += counts[i][1] * 3;
        score += counts[i][1] * (i + 1);
        // Need to win.
        score += counts[i][2] * 6;
        score += counts[i][2] * (((i + 1) % 3) + 1);
    }
    std::cout << "Part 2: " << score << std::endl;
    return 0;
}