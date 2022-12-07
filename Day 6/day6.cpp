#include <iostream>
#include <fstream>

int get_marker(std::string line, int n);

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

    std::cout << "Part 1: " << get_marker(line, 4) << std::endl;
    std::cout << "Part 2: " << get_marker(line, 14) << std::endl;
    return 0;
}

int get_marker(std::string line, int n)
{
    // Create array of all the character counts.
    int characters[26];
    for (int i = 0; i < 26; i++)
    {
        characters[i] = 0;
    }
    for (int i = 0; i < (int)line.length(); i++)
    {
        // Increase character count for the new character.
        char character = line[i];
        int index = character - 'a';
        characters[index]++;
        if (i >= n)
        {
            // Decrease the character count for the character that's been removed.
            character = line[i - n];
            index = character - 'a';
            characters[index]--;
            // Check to see if this is the marker.
            bool marker = true;
            for (int j = 0; j < 26; j++)
            {
                if (characters[j] > 1)
                {
                    marker = false;
                    break;
                }
            }
            if (marker)
            {
                return i + 1;
            }
        }
    }
    return -1;
}