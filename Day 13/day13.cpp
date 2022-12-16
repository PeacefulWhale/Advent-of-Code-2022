#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct packet
{
    packet *parent;
    bool integer;
    int number;
    std::vector<packet *> packets;
};

int insert(std::vector<packet *> *packets, packet *pack);
int compare(packet *left, packet *right);
packet *convert(std::string str);
packet *new_packet(packet *parent);
void free_packets(packet *pack);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line1;
    std::string line2;
    int index = 1;
    // Part 1:
    int part1 = 0;
    // Part 2:
    std::vector<packet *> packets;
    while (std::getline(input_file, line1))
    {
        std::getline(input_file, line2);
        packet *left = convert(line1);
        packet *right = convert(line2);
        // Part 1:
        if (compare(left, right) == 1)
        {
            part1 += index;
        }
        // Part 2:
        insert(&packets, left);
        insert(&packets, right);
        // Skip line and increment index.
        std::getline(input_file, line1);
        ++index;
    }
    std::cout << "Part 1: " << part1 << std::endl;
    std::cout << "Part 2: " << insert(&packets, convert("[[2]]")) * insert(&packets, convert("[[6]]")) << std::endl;
    // Free space.
    for (auto itr : packets)
    {
        free_packets(itr);
    }
    return 0;
}

int insert(std::vector<packet *> *packets, packet *pack)
{
    int i;
    for (i = 0; i < (int)packets->size(); ++i)
    {
        if (compare(packets->at(i), pack) == -1)
        {
            break;
        }
    }
    packets->insert(packets->begin() + i, pack);
    return i + 1;
}

int compare(packet *left, packet *right)
{
    // Both values are integers.
    if (left->integer && right->integer)
    {
        if (left->number < right->number)
        {
            return 1;
        }
        else if (left->number == right->number)
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
    // Both values are lists.
    else if (!left->integer && !right->integer)
    {
        // Check packets.
        int comp_value = 0;
        int l_size = left->packets.size();
        int r_size = right->packets.size();
        // Check all the packets contained within the packet.
        int l = 0, r = 0;
        while (!comp_value && (l < (int)left->packets.size() && r < (int)right->packets.size()))
        {
            packet *tmp_l, *tmp_r;
            tmp_l = left->packets.at(l);
            l += 1;
            tmp_r = right->packets.at(r);
            r += 1;
            comp_value = compare(tmp_l, tmp_r);
        }
        // If the comp_value is 0, then all the values within the lists were equal.
        // Check the length of the lists instead.
        if (comp_value == 0)
        {
            if (l_size < r_size)
            {
                return 1;
            }
            else if (l_size == r_size)
            {
                return 0;
            }
            else
            {
                return -1;
            }
        }
        else
        {
            return comp_value;
        }
    }
    // One value is an integer, while the other is a list.
    else
    {
        // Convert the integers to single element lists.
        if (left->integer)
        {
            packet *tmp = new_packet(left);
            tmp->integer = true;
            tmp->number = left->number;
            left->integer = false;
            left->packets.push_back(tmp);
        }
        if (right->integer)
        {
            packet *tmp = new_packet(right);
            tmp->integer = true;
            tmp->number = right->number;
            right->integer = false;
            right->packets.push_back(tmp);
        }
        return compare(left, right);
    }
}

packet *convert(std::string str)
{
    // Convert from a string to a packet list.
    packet *root = new_packet(nullptr);
    packet *current = root;
    for (int i = 0; i < (int)str.size(); ++i)
    {
        if (str[i] == '[')
        {
            // Create a new packet.
            packet *temp = new_packet(current);
            temp->integer = false;
            current->packets.push_back(temp);
            current = temp;
        }
        else if (str[i] == ']')
        {
            // Go to parent packet.
            current = current->parent;
        }
        else if (str[i] != ',')
        {
            // Add number to the packet.
            packet *temp = new_packet(current);
            temp->integer = true;
            std::stringstream(str.substr(i, 2)) >> temp->number;
            current->packets.push_back(temp);
        }
    }
    return root;
}

packet *new_packet(packet *parent)
{
    packet *temp;
    temp = (packet *)malloc(sizeof(*temp));
    temp->parent = parent;
    return temp;
}

void free_packets(packet *pack)
{
    if (pack)
    {
        for (auto itr : pack->packets)
        {
            free_packets(itr);
        }
    }
}
