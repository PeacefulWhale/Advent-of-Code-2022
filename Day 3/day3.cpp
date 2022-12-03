#include <iostream>
#include <fstream>
#include <string>

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
    std::string lines[3];
    int offset = 0;
    if (input_file.is_open())
    {
        while (std::getline(input_file, line))
        {
            // Part 1:
            int midpoint = line.length() / 2;
            char item = '\0';
            for (int i = 0; i < midpoint && !item; i++)
            {
                for (int j = midpoint; j < midpoint * 2 && !item; j++)
                {
                    if (line[i] == line[j])
                    {
                        item = line[i];
                    }
                }
            }
            score[0] += 1 + ((item >= 'A' && item <= 'Z') ? (item - 'A') + 26 : item - 'a');

            // Part 2:
            lines[offset++] = line;
            if (offset == 3)
            {
                offset = 0;
                item = '\0';
                // Find common element between all 3 lines.
                for (int i = 0; i < (int) lines[0].length() && !item; i++)
                {
                    for (int j = 0; j < (int) lines[1].length() && !item; j++)
                    {
                        for (int w = 0; w < (int) lines[2].length() && !item; w++)
                        {
                            if (lines[0][i] == lines[1][j] && lines[1][j] == lines[2][w])
                            {
                                item = lines[0][i];
                            }
                        }
                    }
                }
                score[1] += 1 + ((item >= 'A' && item <= 'Z') ? (item - 'A') + 26 : item - 'a');
            }
        }
    }

    std::cout << "Part 1: " << score[0] << std::endl;
    std::cout << "Part 2: " << score[1] << std::endl;
    return 0;
}