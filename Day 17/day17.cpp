#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

struct brick
{
    std::pair<unsigned int, unsigned int> loc;
    std::vector<std::pair<int, int>> blocks;
};

struct state
{
    unsigned int seen, blocks, height;
};

#define BLOCK_COUNT 5
brick base_shapes[BLOCK_COUNT];

// There's probably a better way of doing this, but for now I'll just use large char map.
#define WIDTH 7
#define MAX_HEIGHT 10000
#define EMPTY '.'
#define PIECE '#'
char map[WIDTH][MAX_HEIGHT];

unsigned long long simulate(std::string input, unsigned long long block_count);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Initialize pieces.
    base_shapes[0].blocks.push_back({ 0, 0 });
    base_shapes[0].blocks.push_back({ 1, 0 });
    base_shapes[0].blocks.push_back({ 2, 0 });
    base_shapes[0].blocks.push_back({ 3, 0 });

    base_shapes[1].blocks.push_back({ 1, 0 });
    base_shapes[1].blocks.push_back({ 0, 1 });
    base_shapes[1].blocks.push_back({ 1, 1 });
    base_shapes[1].blocks.push_back({ 2, 1 });
    base_shapes[1].blocks.push_back({ 1, 2 });

    base_shapes[2].blocks.push_back({ 0, 0 });
    base_shapes[2].blocks.push_back({ 1, 0 });
    base_shapes[2].blocks.push_back({ 2, 0 });
    base_shapes[2].blocks.push_back({ 2, 1 });
    base_shapes[2].blocks.push_back({ 2, 2 });

    base_shapes[3].blocks.push_back({ 0, 0 });
    base_shapes[3].blocks.push_back({ 0, 1 });
    base_shapes[3].blocks.push_back({ 0, 2 });
    base_shapes[3].blocks.push_back({ 0, 3 });

    base_shapes[4].blocks.push_back({ 0, 0 });
    base_shapes[4].blocks.push_back({ 1, 0 });
    base_shapes[4].blocks.push_back({ 0, 1 });
    base_shapes[4].blocks.push_back({ 1, 1 });

    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    std::getline(input_file, line);

    std::cout << "Part 1: " << simulate(line, 2022) << std::endl;
    std::cout << "Part 2: " << simulate(line, 1000000000000) << std::endl;
    return 0;
}

unsigned long long simulate(std::string input, unsigned long long block_count)
{
    // Initialize map.
    for (int x = 0; x < WIDTH; ++x)
    {
        for (int y = 0; y < MAX_HEIGHT; ++y)
        {
            map[x][y] = EMPTY;
        }
    }
    // Initialize map variables.
    unsigned long long current_height = 0;
    unsigned long long current_block = 0;
    unsigned long long current_jet = 0;

    // For skipping as much "building" as possible:
    // Key: current_jet * 10 + (current_block % BLOCK_COUNT)
    // Values: times this pattern has been seen, height, block_count
    std::unordered_map<unsigned long long, state> states;
    bool skipped = false;
    unsigned long long skipped_height = 0;

    // Drop blocks.
    while (block_count)
    {
        brick block = base_shapes[current_block % BLOCK_COUNT];
        // Each rock appears so that its left edge is two units away from the left wall and its bottom edge is three units above the highest rock in the room.
        block.loc = { 2, current_height + 3 };
        // Drop block until it hits something.
        bool can_fall = true;
        while (can_fall)
        {
            // Move block left / right (if it can).
            bool can_move = true;
            int dir = (input.at(current_jet++) == '<') ? -1 : 1;
            if (current_jet >= (unsigned int)input.size())
            {
                current_jet = 0;
            }
            for (auto piece : block.blocks)
            {
                // Check to see if this piece can move in the given direction.
                if ((block.loc.first + piece.first + dir >= 0) && (block.loc.first + piece.first + dir < WIDTH))
                {
                    // In bounds.
                    // Check for other pieces in map.
                    if (map[block.loc.first + piece.first + dir][block.loc.second + piece.second] != EMPTY)
                    {
                        can_move = false;
                        break;
                    }
                }
                else
                {
                    can_move = false;
                    break;
                }
            }
            if (can_move)
            {
                block.loc.first += dir;
            }

            // Check to see if the block can move down.
            for (auto piece : block.blocks)
            {
                if (block.loc.second + piece.second >= 1)
                {
                    // In bounds.
                    // Check for other pieces in map.
                    if (map[block.loc.first + piece.first][block.loc.second + piece.second - 1] != EMPTY)
                    {
                        can_fall = false;
                        break;
                    }
                }
                else
                {
                    can_fall = false;
                    break;
                }
            }

            if (can_fall && block.loc.second > 0)
            {
                block.loc.second -= 1;
            }
        }
        // Get the highest block and add to map.
        int max = 0;
        for (auto piece : block.blocks)
        {
            if (piece.second > max)
            {
                max = piece.second;
            }
            map[piece.first + block.loc.first][piece.second + block.loc.second] = PIECE;
        }
        if (current_height < block.loc.second + max + 1)
        {
            current_height = block.loc.second + max + 1;
        }

        // Add current state to states.
        if (!skipped)
        {
            unsigned long long state_key = current_jet * 10 + (current_block % BLOCK_COUNT);
            if (states.count(state_key))
            {
                // If it's above 2 start "building" the tower.
                if (states.at(state_key).seen >= 2)
                {
                    skipped = true;
                    unsigned int pattern_height = current_height - states.at(state_key).height;
                    unsigned int pattern_blocks = current_block - states.at(state_key).blocks;
                    skipped_height = pattern_height * (block_count / pattern_blocks);
                    block_count -= pattern_blocks * (block_count / pattern_blocks);
                }
                else
                {
                    states.at(state_key).seen += 1;
                    states.at(state_key).height = current_height;
                    states.at(state_key).blocks = current_block;
                }
            }
            else
            {
                state current_state;
                current_state.seen = 1;
                current_state.height = current_height;
                current_state.blocks = current_block;
                states[state_key] = current_state;
            }
        }
        current_block += 1;
        block_count -= 1;
    }
    return current_height + skipped_height;
}
