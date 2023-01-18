#include <iostream>
#include <fstream>
#include <cmath>

long long snafu_decimal(std::string snafu);
std::string decimal_snafu(long long num);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }

    long long sum = 0;
    {
        std::ifstream input_file(argv[1]);
        std::string line;
        while (std::getline(input_file, line) && line != "")
        {
            sum += snafu_decimal(line);
        }
    }
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Part 1: " << decimal_snafu(sum) << std::endl;
    return 0;
}

long long snafu_decimal(std::string snafu)
{
    // Convert from a snafu to an decimal.
    long long value = 0;
    for (int i = 0; i < snafu.size(); ++i)
    {
        long long five = std::pow(5, (snafu.size() - 1) - i);
        switch (snafu[i])
        {
        case '2':
            value += five;
        case '1':
            value += five;
            break;
        case '0':
            break;
        case '=':
            value -= five;
        case '-':
            value -= five;
            break;
        }
    }
    return value;
}

std::string decimal_snafu(long long num)
{
    // Convert from a decimal to a snafu number.
    std::string snafu;
    for (int i = (int)std::round(std::log(num) / std::log(5)); i >= 0; --i)
    {
        long long five = std::pow(5, i);
        // log_base(num) = log(num) / log(base)
        long long offset = (long long)std::round((float)num / (float)five);
        num -= offset * five;
        switch (offset)
        {
        case 2:
            snafu += "2";
            break;
        case 1:
            snafu += "1";
            break;
        case 0:
            snafu += "0";
            break;
        case -1:
            snafu += "-";
            break;
        case -2:
            snafu += "=";
            break;
        }
    }
    return snafu;
}
