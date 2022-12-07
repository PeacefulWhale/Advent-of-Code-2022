#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class Dir
{
public:
    Dir(std::string name, Dir *parent);
    Dir(std::string name, unsigned long size, Dir *parent);
    ~Dir();
    void add_child(Dir *child);
    Dir *get_child(std::string name);
    Dir *parent;
    std::vector<Dir *> children;
    unsigned long size;
    std::string name;
    bool file;
};

Dir::~Dir()
{
    int child_count = this->children.size();
    for (int i = 0; i < child_count; i++)
    {
        delete this->children.at(i);
    }
}

Dir::Dir(std::string name, Dir *parent)
{
    this->name = name;
    this->file = false;
    this->parent = NULL;
    this->size = 0;
    if (parent)
    {
        parent->add_child(this);
    }
}

Dir::Dir(std::string name, unsigned long size, Dir *parent)
{
    this->name = name;
    this->size = size;
    this->file = true;
    this->parent = NULL;
    if (parent)
    {
        parent->add_child(this);
    }
}

void Dir::add_child(Dir *child)
{
    child->parent = this;
    this->children.push_back(child);
    this->size += child->size;
    // Add size to all parent nodes.
    Dir *cur = this->parent;
    while (cur)
    {
        cur->size += child->size;
        cur = cur->parent;
    }
}

Dir *Dir::get_child(std::string name)
{
    Dir *child = NULL;
    for (auto itr : this->children)
    {
        if (itr->name == name)
        {
            child = itr;
            break;
        }
    }
    return child;
}

unsigned long part_1(Dir *cur)
{
    unsigned long count = 0;
    if (!cur->file)
    {
        for (auto itr : cur->children)
        {
            count += part_1(itr);
        }
        if (cur->size <= 100000)
        {
            count += cur->size;
        }
    }
    return count;
}

unsigned long part_2(Dir *cur, unsigned long target)
{
    unsigned long current = -1;
    if (!cur->file)
    {
        current = cur->size;
        for (auto itr : cur->children)
        {
            unsigned long temp = part_2(itr, target);
            if (temp <= current && temp >= target)
            {
                current = temp;
            }
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
    Dir *root = new Dir("/", NULL);
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
                else
                {
                    cur = cur->get_child(dir_name);
                }
            }
            // List Files / Directories.
            // I don't actually need to do anything with this however.
        }
        else
        {
            if (line[0] == 'd')
            {
                // Add the directory.
                std::string dir_name;
                ss >> temp >> dir_name;
                new Dir(dir_name, cur);
            }
            else
            {
                // Add the file.
                unsigned long file_size;
                std::string file_name;
                ss >> file_size >> file_name;
                new Dir(file_name, file_size, cur);
            }
        }
    }

    std::cout << "Part 1: " << part_1(root) << std::endl;
    unsigned long target = 30000000 - (70000000 - root->size);
    std::cout << "Part 2: " << part_2(root, target) << std::endl;
    delete root;
    return 0;
}