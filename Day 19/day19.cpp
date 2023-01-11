#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

struct bp
{
    int id;
    int robots[4][3]; // ore, cly, obs, geo;
};

struct game_state
{
    // Material counts.
    int minerals[4] = {}; // ore, cly, obs, geo;
    // Robot counts.
    int robots[4] = {}; // rob_ore, rob_cly, rob_obs, rob_geo;
    int time = 0;
};

int path_find(bp costs, int time);
void forward_state(game_state *state, int max_time);
void insert_state(std::vector<game_state> *states, game_state *state);
int possible_score(game_state state, int max_time);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    std::ifstream input_file(argv[1]);
    std::string line;
    // Regex stuff.
    std::regex re_string(".*?(\\d+).*?(\\d+).*?(\\d+).*?(\\d+).*?(\\d+).*?(\\d+).*?(\\d+)");
    std::smatch matches;

    // Parse input.
    // Part 1:
    int part_1 = 0;
    // Part 2:
    int bp_count = 0;
    int part_2 = 1;
    while (std::getline(input_file, line))
    {
        std::regex_search(line, matches, re_string);
        bp blueprint;
        blueprint.id = std::stoi(matches[1]);
        blueprint.robots[0][0] = std::stoi(matches[2]);
        blueprint.robots[0][1] = 0;
        blueprint.robots[0][2] = 0;
        blueprint.robots[1][0] = std::stoi(matches[3]);
        blueprint.robots[1][1] = 0;
        blueprint.robots[1][2] = 0;
        blueprint.robots[2][0] = std::stoi(matches[4]);
        blueprint.robots[2][1] = std::stoi(matches[5]);
        blueprint.robots[2][2] = 0;
        blueprint.robots[3][0] = std::stoi(matches[6]);
        blueprint.robots[3][1] = 0;
        blueprint.robots[3][2] = std::stoi(matches[7]);
        part_1 += blueprint.id * path_find(blueprint, 24);
        if (bp_count < 3)
        {
            part_2 *= path_find(blueprint, 32);
            bp_count += 1;
        }
    }
    std::cout << "Part 1: " << part_1 << std::endl;
    std::cout << "Part 2: " << part_2 << std::endl;
    return 0;
}

int path_find(bp costs, int max_time)
{
    game_state start_state;
    start_state.time = 1;
    start_state.robots[0] = 1;
    start_state.minerals[0] = 1;
    std::vector<game_state> states;
    states.push_back(start_state);

    // Cache to ensure that doubling back doesn't happen.
    int visited[max_time][max_time][max_time][max_time];
    for (int a = 0; a < max_time; ++a)
    {
        for (int b = 0; b < max_time; ++b)
        {
            for (int c = 0; c < max_time; ++c)
            {
                for (int d = 0; d < max_time; ++d)
                {
                    visited[a][b][c][d] = max_time;
                }
            }
        }
    }

    // Max costs for restricting the creation of new bots.
    int max_costs[3] = {};
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (max_costs[j] < costs.robots[i][j])
            {
                max_costs[j] = costs.robots[i][j];
            }
        }
    }

    int max_geodes = 0;
    while (!states.empty())
    {
        game_state current_state = states.at(0);
        states.erase(states.begin());

        // Build Robots.
        for (int i = 0; i < 4; ++i)
        {
            // Should this bot be built?
            if (i < 3 && current_state.robots[i] >= max_costs[i])
            {
                // No... it should not be.
                continue;
            }

            // Can this bot be built?
            bool can_build = true;
            int turns = 1;
            for (int j = 0; j < 3; ++j)
            {
                if (costs.robots[i][j])
                {
                    if (current_state.robots[j])
                    {
                        // Yes it can.
                        // Calculate how much time it would take to do so.
                        // Division with rounding up.
                        int temp_time = (((costs.robots[i][j] - current_state.minerals[j]) + (current_state.robots[j] - 1)) / current_state.robots[j]) + 1;
                        if (temp_time > turns)
                        {
                            turns = temp_time;
                        }
                    }
                    else
                    {
                        can_build = false;
                        break;
                    }
                }
            }
            if (can_build && current_state.time + turns <= max_time)
            {
                game_state build_state = current_state;
                // Build the robot and increment all resources.
                forward_state(&build_state, turns);
                // Adjust minerals.
                for (int j = 0; j < 3; ++j)
                {
                    build_state.minerals[j] -= costs.robots[i][j];
                }
                // Build robot.
                build_state.robots[i] += 1;
                // Geodes have special handling.
                if (i == 3)
                {
                    build_state.minerals[3] += max_time - build_state.time;
                    if (build_state.minerals[3] > max_geodes)
                    {
                        max_geodes = build_state.minerals[3];
                    }
                }
                // Pruning / optimization stuff before insertion...

                // Check to see if this position has already been visited by a better time.
                if (visited[build_state.robots[0]][build_state.robots[1]][build_state.robots[2]][build_state.robots[3]] < build_state.time)
                {
                    // Yep, don't bother checking this one out.
                    continue;
                }
                else
                {
                    // Update best time to this state.
                    visited[build_state.robots[0]][build_state.robots[1]][build_state.robots[2]][build_state.robots[3]] = build_state.time;
                }

                if (build_state.time > max_time)
                {
                    // This is an end state.
                    continue;
                }

                if (possible_score(build_state, max_time) < max_geodes)
                {
                    // This state cannot beat the current best geode count, so skip it.
                    continue;
                }

                // Okay, this state is fine to insert.
                insert_state(&states, &build_state);
            }
        }
    }

    return max_geodes;
}

void forward_state(game_state *state, int time)
{
    for (int i = 0; i < 3; ++i)
    {
        state->minerals[i] += time * state->robots[i];
    }
    state->time += time;
}

int possible_score(game_state state, int max_time)
{
    int robots = 1;
    int geodes = 0;
    for (int i = state.time + 1; i <= max_time; ++i)
    {
        geodes += robots;
        robots += 1;
    }
    return state.minerals[3] + geodes;
}

void insert_state(std::vector<game_state> *states, game_state *state)
{
    int index;
    bool insert = true;
    // Man this is going to get ugly...
    for (index = 0; index < (int)states->size(); ++index)
    {
        // Order by robots.
        if (state->robots[3] > states->at(index).robots[3])
        {
            break;
        }
        else if (state->robots[3] == states->at(index).robots[3])
        {
            if (state->robots[2] > states->at(index).robots[2])
            {
                break;
            }
            else if (state->robots[2] == states->at(index).robots[2])
            {
                if (state->robots[1] > states->at(index).robots[1])
                {
                    break;
                }
                else if (state->robots[1] == states->at(index).robots[1])
                {
                    if (state->robots[0] > states->at(index).robots[0])
                    {
                        break;
                    }
                    else if (state->robots[0] == states->at(index).robots[0])
                    {
                        // Okay... it has the same number of robots...
                        // Don't bother to insert it then.
                        // I think this "optimization" slightly overlaps with my "visited" cache, but I think that's okay.
                        insert = false;
                    }
                }
            }
        }
    }
    if (insert)
    {
        states->insert(states->begin() + index, *state);
    }
}
