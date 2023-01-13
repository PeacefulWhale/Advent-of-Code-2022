#include <iostream>
#include <fstream>
#include <unordered_map>

struct monkey
{
    long long value = 0;
    std::string a, b;
    char operation = 0;
    bool human;
};

// I'm too lazy to read the input into a tree...
std::unordered_map<std::string, monkey> monkey_map;

long long part_1(std::string name);
long long part_2(std::string current_name, std::string human);
bool human_direction(std::string current_name, std::string human);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    std::ifstream input_file(argv[1]);
    std::string line;
    while (std::getline(input_file, line))
    {
        monkey temp;
        std::string name = line.substr(0, 4);
        if (line.length() == 17)
        {
            // Monkey's got some operations to do...
            temp.a = line.substr(6, 4);
            temp.operation = line[11];
            temp.b = line.substr(13, 4);
        }
        else
        {
            // Get number...
            temp.value = std::stol(line.substr(6, line.length()));
        }
        monkey_map[name] = temp;
    }

    // Part 1:
    // Just do the math on all the leaves recursively.
    std::cout << "Part 1: " << part_1("root") << std::endl;

    // Part 2:
    // Calculate the direction to head towards the human.
    human_direction("root", "humn");
    monkey_map.at("root").operation = '=';
    // It's kinda like a BST...
    // Side without X get's calculated, as it searches down towards X.
    // The operand on the "branch" towards X is then inverted.
    std::cout << "Part 2: " << part_2("root", "humn") << std::endl;
    return 0;
}

long long part_2(std::string current_name, std::string human)
{
    // Build up operations
    long long value = 0;
    // I could have made this recursive, but this was easier.
    while (current_name != human)
    {
        monkey current = monkey_map.at(current_name);
        // Get the side that does not contain X.
        long long y;
        std::string next_name;
        bool swap;
        if (monkey_map.at(current.a).human)
        {
            y = part_1(current.b);
            next_name = current.a;
            swap = false;
        }
        else
        {
            y = part_1(current.a);
            next_name = current.b;
            swap = true;
        }
        // Apply the opposite operation.
        switch (current.operation)
        {
        case '=':
            value = y;
            break;
        case '+':
            value -= y;
            break;
        case '-':
        // Oder of operations!
            if (swap)
            {
                value = y - value;
            }
            else
            {
                value += y;
            }
            break;
        case '*':
            value /= y;
            break;
        case '/':
        // Order of operations!
            if (swap)
            {
                value = y / value;
            }
            else
            {
                value *= y;
            }
            break;
        }
        // Update current stuff.
        current_name = next_name;
    }
    return value;
}

bool human_direction(std::string current_name, std::string human)
{
    if (current_name != "")
    {
        monkey *current = &monkey_map.at(current_name);
        if (current_name == human)
        {
            current->human = true;
        }
        else
        {
            current->human = human_direction(current->a, human) || human_direction(current->b, human);
        }
        return current->human;
    }
    else
    {
        return false;
    }
}

long long part_1(std::string name)
{
    monkey current = monkey_map.at(name);
    if (!current.operation)
    {
        return current.value;
    }
    else
    {
        // Indention on these always feels weird to me...
        long long value = 0;
        switch (current.operation) {
        case '+':
            value = part_1(current.a) + part_1(current.b);
            break;
        case '-':
            value = part_1(current.a) - part_1(current.b);
            break;
        case '*':
            value = part_1(current.a) * part_1(current.b);
            break;
        case '/':
            value = part_1(current.a) / part_1(current.b);
            break;
        }
        return value;
    }
}
