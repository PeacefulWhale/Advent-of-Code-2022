#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Dir
{
public:
    Dir(Dir *parent);
    ~Dir();
    void add_size(unsigned long size);
    Dir *parent;
    std::vector<Dir *> children;
    unsigned long size;
};

Dir::~Dir()
{
    int child_count = this->children.size();
    for (int i = 0; i < child_count; i++)
    {
        delete this->children.at(i);
    }
}

Dir::Dir(Dir *parent)
{
    this->parent = parent;
    this->size = 0;
    if (parent)
    {
        parent->children.push_back(this);
    }
}

void Dir::add_size(unsigned long size)
{
    this->size += size;
    if (this->parent)
    {
        this->parent->add_size(size);
    }
}

unsigned long part_1(Dir *cur)
{
    unsigned long count = 0;
    for (auto itr : cur->children)
    {
        count += part_1(itr);
    }
    if (cur->size <= 100000)
    {
        count += cur->size;
    }
    return count;
}

unsigned long part_2(Dir *cur, unsigned long target)
{
    unsigned long current = 0;
    current = cur->size;
    for (auto itr : cur->children)
    {
        unsigned long temp = part_2(itr, target);
        if (temp <= current && temp >= target)
        {
            current = temp;
        }
    }
    return current;
}

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    // The first line is always "$ cd /".
    std::getline(input_file, line);
    Dir *root = new Dir(NULL);
    Dir *cur = root;
    while (std::getline(input_file, line))
    {
        std::string temp;
        std::stringstream ss(line);
        if (line[0] == '$')
        {
            // Change Directory.
            if (line[2] == 'c')
            {
                std::string dir_name;
                ss >> temp >> temp >> dir_name;
                // Move up one dir.
                if (dir_name == "..")
                {
                    cur = cur->parent;
                }
                // Move down one dir.
                // Except this is actually always a new dir.
                else
                {
                    cur = new Dir(cur);
                }
            }
            // List Files / Directories.
            // I don't need to do anything with this.
        }
        else
        {
            if (line[0] != 'd')
            {
                // Add the file size.
                // I don't need to keep track of individual files.
                unsigned long file_size;
                std::string file_name;
                ss >> file_size >> file_name;
                cur->add_size(file_size);
            }
        }
    }

    std::cout << "Part 1: " << part_1(root) << std::endl;
    unsigned long target = 30000000 - (70000000 - root->size);
    std::cout << "Part 2: " << part_2(root, target) << std::endl;
    delete root;
    return 0;
}
