#include <iostream>
#include <fstream>
#include <vector>

struct link
{
    long value;
    int index;
};

std::vector<link> mix(std::vector<link> numbers, int count);
long sum_vector(std::vector<link> numbers);
// Math...
int wrap(int number, int min, int max)
{
    if (number < min) return (max + 1) - (min - number);
    else if (number > max) return (min - 1) + (number - max);
    else return number;
}

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    std::ifstream input_file(argv[1]);

    std::vector<link> input;
    int value, index = 0;
    while (input_file >> value)
    {
        link temp;
        temp.value = value;
        temp.index = index;
        input.push_back(temp);
        index += 1;
    }
    // Part 1.
    std::vector<link> part_1 = mix(input, 1);
    std::cout << "Part 1: " << sum_vector(part_1) << std::endl;

    // Part 2.
    std::vector<link> part_2 = input;
    for (auto &x : part_2) x.value *= 811589153;
    part_2 = mix(part_2, 10);
    std::cout << "Part 2: " << sum_vector(part_2) << std::endl;

    return 0;
}

long sum_vector(std::vector<link> numbers)
{
    // Find zero index.
    int zero_index;
    for (zero_index = 0; zero_index < (int)numbers.size() && numbers.at(zero_index).value != 0; ++zero_index);
    return numbers.at((zero_index + 1000) % numbers.size()).value
        + numbers.at((zero_index + 2000) % numbers.size()).value
        + numbers.at((zero_index + 3000) % numbers.size()).value;
}

std::vector<link> mix(std::vector<link> numbers, int count)
{
    int size = (int)numbers.size();
    for (int repetition = 0; repetition < count; ++repetition)
    {
        // Mix numbers.
        int index = 0;
        while (index < size)
        {
            for (int i = 0; i < size; ++i)
            {
                link temp = numbers.at(i);
                if (temp.index == index)
                {
                    if (temp.value)
                    {
                        // Math to calculate the new position.
                        int offset = temp.value % (size - 1);
                        int new_pos = (i + offset);
                        new_pos = wrap(new_pos, 1, size - 1);
                        // Move the number.
                        numbers.erase(numbers.begin() + i);
                        numbers.insert(numbers.begin() + new_pos, temp);
                    }
                    index += 1;
                    break;
                }
            }
        }
    }
    return numbers;
}
