#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <queue>
#include <cmath>

struct GameState
{
    int time;
    unsigned long released_pressure;
    int current_position;
    std::vector<bool> opened;
    unsigned long best_possible;
    std::vector<int> visits;
};

std::vector<int> get_distances(std::vector<std::vector<int>> connections, int source);
unsigned long path_find(std::vector<std::vector<int>> distances, std::vector<int> flow_rates, int start, int max_time, bool elephant);
unsigned long possible_score(std::vector<int> *flow_rates, GameState *state, int max_time);
void insert_state(std::vector<GameState> *states, GameState *state);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;
    // Regex Stuff...
    std::regex match_one("Valve\\s(.*?)\\s.*?=(\\d*?);.*?valve.*?\\s");
    std::regex match_two("([A-Z]{2})");
    std::smatch matches;
    // Actual Valve Pointers.
    // Stuff to assemble the valves.
    std::vector<std::string> valve_names;
    std::vector<std::vector<std::string>> str_connections;

    // Graph.
    std::vector<int> flow_rates;
    int start_pos;
    int temp_index = 0;
    while (std::getline(input_file, line))
    {
        std::regex_search(line, matches, match_one);
        valve_names.push_back(matches[1]);
        if (matches[1] == "AA")
        {
            start_pos = temp_index;
        }
        temp_index += 1;
        flow_rates.push_back(std::stoi(matches[2]));
        std::vector<std::string> temp_connections;
        // Add connections.
        line = line.substr(matches[0].length());
        while (std::regex_search(line, matches, match_two))
        {
            temp_connections.push_back(matches[1]);
            if ((int)matches[0].length() + 1 < (int)line.size())
            {
                line = line.substr(matches[0].length() + 1);
            }
            else
            {
                break;
            }
        }
        str_connections.push_back(temp_connections);
    }

    std::vector<std::vector<int>> connections;
    // Convert str_connections to integer connections.
    for (int i = 0; i < (int)str_connections.size(); ++i)
    {
        std::vector<std::string> temp_str_connections = str_connections.at(i);
        std::vector<int> temp_connections;
        for (int j = 0; j < (int)temp_str_connections.size(); ++j)
        {
            int index;
            for (index = 0; index < (int)valve_names.size(); ++index)
            {
                if (valve_names.at(index) == temp_str_connections.at(j))
                {
                    break;
                }
            }
            temp_connections.push_back(index);
        }
        connections.push_back(temp_connections);
    }

    // Find the distances from every node to every other node.
    std::vector<std::vector<int>> distances;
    for (int source = 0; source < (int)connections.size(); ++source)
    {
        distances.push_back(get_distances(connections, source));
    }

    // Part 1:
    std::cout << "Part 1: " << path_find(distances, flow_rates, start_pos, 30, false) << std::endl;

    // Part 2:
    std::cout << "Part 2: " << path_find(distances, flow_rates, start_pos, 26, true) << std::endl;
    return 0;
}

unsigned long path_find(std::vector<std::vector<int>> distances, std::vector<int> flow_rates, int start, int max_time, bool elephant)
{
    GameState start_state;
    start_state.current_position = start;
    int valve_count = (int)distances.size();
    for (int i = 0; i < valve_count; ++i)
    {
        start_state.opened.push_back(false);
    }
    start_state.released_pressure = 0;
    start_state.time = 0;
    start_state.best_possible = possible_score(&flow_rates, &start_state, max_time);
    start_state.visits.push_back(start_state.current_position);
    std::vector<GameState> states;
    states.push_back(start_state);
    // For the elephant:
    std::vector<GameState> end_states;
    while (!states.empty())
    {
        GameState current_state = states.at(0);
        int current_position = current_state.current_position;
        states.erase(states.begin());
        end_states.push_back(current_state);
        // If it is the max time or "failure" state, we must skip the creation of new valves.
        bool finished = true;
        for (int i = 0; i < (int)current_state.opened.size(); ++i)
        {
            if (!current_state.opened.at(i) && flow_rates.at(i))
            {
                finished = false;
            }
        }
        if (finished || current_state.time == max_time)
        {
            continue;
        }
        if (!current_state.opened.at(current_position) && flow_rates.at(current_position))
        {
            GameState new_state = current_state;
            new_state.time += 1;
            new_state.released_pressure += flow_rates.at(current_position) * (max_time - new_state.time);
            new_state.opened.at(current_position) = true;
            new_state.best_possible = possible_score(&flow_rates, &new_state, max_time);
            if (new_state.time <= max_time)
            {
                insert_state(&states, &new_state);
            }
        }
        // Only move to new nodes if this node is already opened.
        else if (current_state.opened.at(current_position) || (!current_state.opened.at(current_position) && !flow_rates.at(current_position)))
        {
            // Move to all the new locations.
            for (int i = 0; i < valve_count; ++i)
            {
                if (i != current_position && flow_rates.at(i) && !current_state.opened.at(i))
                {
                    // Only move to new path if:
                    // - Time is less than max_time
                    // - It's possible to get a better score.
                    // - The room has a nozzle to open.
                    // - The room has not been opened.
                    GameState new_state = current_state;
                    new_state.time += distances.at(current_position).at(i);
                    new_state.current_position = i;
                    new_state.best_possible = possible_score(&flow_rates, &new_state, max_time);
                    new_state.visits.push_back(new_state.current_position);
                    if (new_state.time <= max_time)
                    {
                        insert_state(&states, &new_state);
                    }
                }
            }
        }
    }
    unsigned long best_score = 0;
    if (elephant)
    {
        for (int i = (int)end_states.size() - 1; i >= 1; --i)
        {
            for (int j = i - 1; j >= 0; --j)
            {
                unsigned long temp_score = end_states.at(i).released_pressure + end_states.at(j).released_pressure;
                if (temp_score > best_score)
                {
                    // Check to see if this is a valid pair.
                    bool valid = true;
                    for (int x = 1; x < (int)end_states.at(i).visits.size() && valid; ++x)
                    {
                        for (int y = 1; y < (int)end_states.at(j).visits.size() && valid; ++y)
                        {
                            if (end_states.at(i).visits.at(x) == end_states.at(j).visits.at(y))
                            {
                                valid = false;
                            }
                        }
                    }
                    if (valid)
                    {
                        best_score = temp_score;
                    }
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < (int)end_states.size(); ++i)
        {
            if (end_states.at(i).released_pressure > best_score)
            {
                best_score = end_states.at(i).released_pressure;
            }
        }
    }
    return best_score;
}

unsigned long possible_score(std::vector<int> *flow_rates, GameState *state, int max_time)
{
    unsigned long possible_score = state->released_pressure;
    for (int i = 0; i < (int)flow_rates->size(); ++i)
    {
        if (!state->opened.at(i) && flow_rates->at(i))
        {
            possible_score += flow_rates->at(i) * (max_time - state->time);
        }
    }
    return possible_score;
}

void insert_state(std::vector<GameState> *states, GameState *state)
{
    int index;
    for (index = 0; index < (int)states->size(); ++index)
    {
        if (state->released_pressure >= states->at(index).released_pressure)
        {
            break;
        }
    }
    states->insert(states->begin() + index, *state);
}

std::vector<int> get_distances(std::vector<std::vector<int>> connections, int source)
{
    std::vector<int> distances;
    for (int i = 0; i < (int)connections.size(); ++i)
    {
        distances.push_back(-1);
    }
    std::queue<int> to_visit;
    std::queue<int> next_depth;
    to_visit.push(source);
    int distance = 0;
    // Traverse down the tree... I was too lazy to make this recursive.
    while (!to_visit.empty() || !next_depth.empty())
    {
        int current_node = to_visit.front();
        to_visit.pop();
        if (distances.at(current_node) == -1)
        {
            distances.at(current_node) = distance;
            // Add connections to the next_depth source.
            std::vector<int> current_connections = connections.at(current_node);
            for (int i = 0; i < (int)current_connections.size(); ++i)
            {
                int connection = current_connections.at(i);
                if (distances.at(connection) == -1)
                {
                    next_depth.push(connection);
                }
            }
        }
        if (to_visit.empty())
        {
            distance += 1;
            to_visit = next_depth;
            next_depth = std::queue<int>();
        }
    }
    return distances;
}
