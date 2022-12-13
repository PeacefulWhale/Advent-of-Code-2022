#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

struct Monkey
{
    std::stack<unsigned long> items;
    char operation;
    int operand;
    bool old;
    int test;
    int t;
    int f;
    unsigned long inspections;
};

void simulate_monkeys(std::vector<Monkey> *monkeys, int rounds, int div, unsigned long mod);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::vector<Monkey> monkeys;
    std::ifstream input_file(argv[1]);
    std::string line;
    while (std::getline(input_file, line))
    {
        // Skip the first line.
        std::getline(input_file, line);
        std::stringstream ss(line);
        std::string ts;
        Monkey temp_monkey;
        temp_monkey.inspections = 0;
        ss >> ts >> ts;
        // Get Items.
        int item;
        char tc;
        while (ss >> item)
        {
            temp_monkey.items.push(item);
            ss >> tc;
        }
        // Get Operation.
        std::getline(input_file, line);
        std::stringstream(line) >> ts >> ts >> ts >> ts >> temp_monkey.operation >> ts;
        if (ts == "old")
        {
            temp_monkey.old = true;
        }
        else
        {
            temp_monkey.old = false;
            std::stringstream(ts) >> temp_monkey.operand;
        }

        // Get Test.
        std::getline(input_file, line);
        std::stringstream(line) >> ts >> ts >> ts >> temp_monkey.test;

        // Get True.
        std::getline(input_file, line);
        std::stringstream(line) >> ts >> ts >> ts >> ts >> ts >> temp_monkey.t;

        // Get False.
        std::getline(input_file, line);
        std::stringstream(line) >> ts >> ts >> ts >> ts >> ts >> temp_monkey.f;

        // Add monkey to the vector of monkeys.
        monkeys.push_back(temp_monkey);
        // Get rid of the empty last line.
        std::getline(input_file, line);
    }

    // If the mod value is a multiple of all the test values, it shouldn't lead to those test values being changed by mod.
    unsigned long mod = 1;
    for (int i = 0; i < (int)monkeys.size(); ++i)
    {
        mod *= monkeys.at(i).test;
    }

    // Copy monkeys for part 2.
    std::vector<Monkey> copy = monkeys;

    // Part 1.
    simulate_monkeys(&monkeys, 20, 3, 0);
    unsigned long a = 0, b = 0;
    for (int i = 0; i < (int)monkeys.size(); ++i)
    {
        unsigned long inspections = monkeys.at(i).inspections;
        if (inspections > a)
        {
            b = a;
            a = inspections;
        }
        else if (inspections > b)
        {
            b = inspections;
        }
    }
    std::cout << "Part 1: " << a * b << std::endl;

    // Part 2.
    monkeys = copy;
    simulate_monkeys(&monkeys, 10000, 0, mod);
    a = 0, b = 0;
    for (int i = 0; i < (int)monkeys.size(); ++i)
    {
        unsigned long inspections = monkeys.at(i).inspections;
        if (inspections > a)
        {
            b = a;
            a = inspections;
        }
        else if (inspections > b)
        {
            b = inspections;
        }
    }
    std::cout << "Part 2: " << a * b << std::endl;

    return 0;
}

void simulate_monkeys(std::vector<Monkey> *monkeys, int rounds, int div, unsigned long mod)
{
    for (int round = 0; round < rounds; ++round)
    {
        for (int i = 0; i < (int)monkeys->size(); ++i)
        {
            while (!monkeys->at(i).items.empty())
            {
                monkeys->at(i).inspections += 1;
                unsigned long item = monkeys->at(i).items.top();
                if (mod)
                {
                    item %= mod;
                }
                monkeys->at(i).items.pop();
                if (monkeys->at(i).operation == '*')
                {
                    if (monkeys->at(i).old)
                    {
                        item *= item;
                    }
                    else
                    {
                        item *= monkeys->at(i).operand;
                    }
                }
                else if (monkeys->at(i).operation == '+')
                {
                    if (monkeys->at(i).old)
                    {
                        item += item;
                    }
                    else
                    {
                        item += monkeys->at(i).operand;
                    }
                }
                if (div)
                {
                    item /= div;
                }
                if (item % monkeys->at(i).test == 0)
                {
                    monkeys->at(monkeys->at(i).t).items.push(item);
                }
                else
                {
                    monkeys->at(monkeys->at(i).f).items.push(item);
                }
            }
        }
    }
}
