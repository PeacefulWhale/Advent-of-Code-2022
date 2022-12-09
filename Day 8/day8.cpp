#include <iostream>
#include <fstream>

int part_1(int *trees, int width, int height);
int part_2(int *trees, int width, int height);
int scenic_score(int *trees, int width, int height, int x, int y);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    int height, width;
    height = width = 0;

    // Get height and width;
    std::getline(input_file, line);
    width = line.length();
    ++height;
    while (std::getline(input_file, line) && line != "")
    {
        ++height;
    }
    int *trees;
    trees = (int *)malloc(sizeof(trees) * (height * width));
    // Go back to the beginning and parse the input again.
    input_file.clear();
    input_file.seekg(0, std::ios::beg);
    for (int y = 0; y < height; ++y)
    {
        std::getline(input_file, line);
        for (int x = 0; x < width; ++x)
        {
            trees[x + (y * width)] = line[x] - '0';
        }
    }

    // Part 1.
    std::cout << "Part 1: " << part_1(trees, width, height) << std::endl;
    // Part 2.
    std::cout << "Part 2: " << part_2(trees, width, height) << std::endl;

    free(trees);
    return 0;
}

int part_1(int *trees, int width, int height)
{
    int *visible;
    visible = (int *)malloc(sizeof(visible) * width * height);

    // How many trees are visible from outside the grid?
    // Top->Down
    for (int x = 0; x < width; ++x)
    {
        int tallest = -1;
        for (int y = 0; y < height; ++y)
        {
            if (tallest < trees[x + (y * width)])
            {
                tallest = trees[x + (y * width)];
                visible[x + (y * width)] += 1;
            }
        }
    }
    // Down->Top
    for (int x = 0; x < width; ++x)
    {
        int tallest = -1;
        for (int y = height - 1; y >= 0; --y)
        {
            if (tallest < trees[x + (y * width)])
            {
                tallest = trees[x + (y * width)];
                visible[x + (y * width)] += 1;
            }
        }
    }
    // Left->Right
    for (int y = 0; y < height; ++y)
    {
        int tallest = -1;
        for (int x = 0; x < width; ++x)
        {
            if (tallest < trees[x + (y * width)])
            {
                tallest = trees[x + (y * width)];
                visible[x + (y * width)] += 1;
            }
        }
    }
    // Right->Left
    for (int y = 0; y < height; ++y)
    {
        int tallest = -1;
        for (int x = width - 1; x >= 0; --x)
        {
            if (tallest < trees[x + (y * width)])
            {
                tallest = trees[x + (y * width)];
                visible[x + (y * width)] += 1;
            }
        }
    }

    // Count up all the visible elements.
    int visible_count = 0;
    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            visible_count += (visible[x + (y * width)]) ? 1 : 0;
        }
    }
    free(visible);
    return visible_count;
}

int part_2(int *trees, int width, int height)
{
    // What is the highest scenic score possible for any tree?
    int highest_score = 0;
    // Skip edges because they'll always be 0.
    for (int x = 1; x < width - 1; ++x)
    {
        for (int y = 1; y < height - 1; ++y)
        {
            int score = scenic_score(trees, width, height, x, y);
            highest_score = (highest_score < score) ? score : highest_score;
        }
    }
    return highest_score;
}

int scenic_score(int *trees, int width, int height, int i, int j)
{
    int current_score, count;
    current_score = 1;
    int tree = trees[i + (j * width)];

    // Up.
    count = 0;
    for (int y = j - 1; y >= 0; --y)
    {
        count += 1;
        if (trees[i + (y * width)] >= tree)
        {
            break;
        }
    }
    current_score *= count;

    // Down.
    count = 0;
    for (int y = j + 1; y < height; ++y)
    {
        count += 1;
        if (trees[i + (y * width)] >= tree)
        {
            break;
        }
    }
    current_score *= count;

    // Right.
    count = 0;
    for (int x = i + 1; x < width; ++x)
    {
        count += 1;
        if (trees[x + (j * width)] >= tree)
        {
            break;
        }
    }
    current_score *= count;

    // Left.
    count = 0;
    for (int x = i - 1; x >= 0; --x)
    {
        count += 1;
        if (trees[x + (j * width)] >= tree)
        {
            break;
        }
    }
    current_score *= count;

    return current_score;
}