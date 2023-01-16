#include "day22.hpp"

// Characters the map utilizes.
#define EMPTY ' '
#define CLEAR '.'
#define WALL '#'

// This just has to be larger than the input to guarantee that the hash function works.
#define MAX_SIZE 10000

// 2D Map stuff.
template<>
struct std::hash<pos>
{
    std::size_t operator()(const pos &loc) const
    {
        return (loc.x * MAX_SIZE) + loc.y;
    }
};
std::unordered_map<pos, char> map;
int follow_map(pos current_pos, std::string instructions, bool cube);
pos get_movement(int facing);

// Cube stuff.
int face_size;
struct edge_mapping
{
    // For creation of the 2D cube.
    struct edge_mapping *other_edge = nullptr;
    // For linking edges together.
    pos3 edge_center;
    // For 3D cube to 2D mapping.
    pos map_a, map_b;
    int facing;
};
#define EDGE_COUNT 24
edge_mapping edges[EDGE_COUNT];
std::unordered_map<pos, bool> edge_visited;
// Cube functions.
void cube_wrap(pos &current_pos, int &facing);
void generate_edges(pos current_pos, int &current_edge, pos3 face, pos3 up, pos3 right);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    std::ifstream input_file(argv[1]);
    std::string line;
    std::string instructions;
    int y_offset = 0;
    pos start_pos;
    start_pos.y = 0;
    while (std::getline(input_file, line))
    {
        if (line == "")
        {
            // Get the instruction line.
            std::getline(input_file, instructions);
        }
        else
        {
            pos temp;
            temp.y = y_offset;
            for (temp.x = 0; temp.x < (int)line.length(); ++temp.x)
            {
                if (line[temp.x] != EMPTY)
                {
                    map[temp] = line[temp.x];
                    if (!start_pos.x && line[temp.x] == CLEAR)
                    {
                        start_pos.x = temp.x;
                    }
                }
            }
            y_offset += 1;
        }
    }
    // Part 1.
    std::cout << "Part 1: " << follow_map(start_pos, instructions, false) << std::endl;
    // Part 2.
    face_size = std::sqrt((map.size() / 6));
    int current_edge = 0;
    generate_edges(start_pos, current_edge, { 0, 0, 2 }, { 0, 2, 0 }, { 2, 0, 0 });
    // Link edges together.
    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        for (int j = i + 1; j < EDGE_COUNT; j++)
        {
            if (edges[i].edge_center == edges[j].edge_center)
            {
                edges[i].other_edge = edges + j;
                edges[j].other_edge = edges + i;
            }
        }
    }
    std::cout << "Part 2: " << follow_map(start_pos, instructions, true) << std::endl;
    return 0;
}

int follow_map(pos current_pos, std::string instructions, bool cube)
{
    int facing = 0;
    for (int i = 0; i < (int)instructions.length(); ++i)
    {
        if (instructions[i] == 'R')
        {
            facing = wrap(facing + 1, 0, 3);
        }
        else if (instructions[i] == 'L')
        {
            facing = wrap(facing - 1, 0, 3);
        }
        else
        {
            // This is a number, move forward.
            int walk_count = 0;
            do
            {
                walk_count *= 10;
                walk_count += instructions[i] - '0';
                i += 1;
            } while (i < (int)instructions.length() && instructions[i] != 'R' && instructions[i] != 'L');
            i -= 1;
            // Move forward.
            for (int walk = 0; walk < walk_count; ++walk)
            {
                // This has to be inside because of part 2.
                pos movement = get_movement(facing);
                // Check to see if this movement is valid.
                if (map.contains(current_pos + movement))
                {
                    if (map.at(current_pos + movement) == CLEAR)
                    {
                        // Valid movement.
                        current_pos += movement;
                    }
                    else
                    {
                        // Blocked... no more moving forward.
                        break;
                    }
                }
                else
                {
                    // Try to wrap around.
                    if (cube)
                    {
                        cube_wrap(current_pos, facing);
                    }
                    else
                    {
                        // Move from the opposite direction until there is nothing.
                        // The map is convex, so this will work.
                        pos temp_pos;
                        for (temp_pos = current_pos; map.contains(temp_pos - movement); temp_pos -= movement);
                        if (map.at(temp_pos) == CLEAR)
                        {
                            // Valid place to move.
                            current_pos = temp_pos;
                        }
                        else
                        {
                            // Blocked... no more moving forward.
                            break;
                        }
                    }
                }
            }
        }
    }
    return ((current_pos.y + 1) * 1000) + ((current_pos.x + 1) * 4) + facing;
}

// Wrap around the cube.
void cube_wrap(pos &current_pos, int &facing)
{
    // Get the edge that this is attached to.
    edge_mapping current_edge, new_edge;
    for (int i = 0; i < EDGE_COUNT; ++i)
    {
        if (on_edge(current_pos, facing, edges[i].map_a, edges[i].map_b, edges[i].facing))
        {
            current_edge = edges[i];
            new_edge = *edges[i].other_edge;
            break;
        }
    }

    // Move from the current edge to the other edge (if possible).
    int new_facing = new_edge.facing;

    // Get new temp_pos.
    pos movement;
    if (new_edge.map_a.x != new_edge.map_b.x)
    {
        movement = { 1, 0 };
    }
    else
    {
        movement = { 0, 1 };
    }

    // Calculate new position.
    int edge_distance;
    pos new_pos;
    if (facing == new_facing ||
        ((facing == 2 && new_facing == 1) || (facing == 1 && new_facing == 2)) ||
        ((facing == 3 && new_facing == 0) || (facing == 0 && new_facing == 3)))
    {
        // Do not flip.
        edge_distance = distance(current_pos, current_edge.map_a);
        new_pos = new_edge.map_a + (movement * edge_distance);
    }
    else
    {
        // Must be flipped.
        edge_distance = distance(current_pos, current_edge.map_b);
        new_pos = new_edge.map_a + (movement * edge_distance);
    }

    // Update location and map facing if this is a valid movement.
    if (map.contains(new_pos) && map.at(new_pos) == CLEAR)
    {
        current_pos = new_pos;
        facing = new_facing;
    }
    return;
}

void generate_edges(pos current_pos, int &current_edge, pos3 face, pos3 up, pos3 right)
{
    // Make sure that this is a valid face first.
    // Check boundaries
    if ((current_pos.x < 0 || current_pos.x > face_size * 4) ||
        (current_pos.y < 0 || current_pos.y > face_size * 4))
    {
        return;
    }
    // Check to see if this position exists in the map.
    if (!map.contains(current_pos))
    {
        return;
    }
    // Check to see if this position has already been visited.
    if (edge_visited.contains(current_pos))
    {
        return;
    }
    edge_visited[current_pos] = true;
    // Helpers.
    edge_mapping a, b, c, d;

    // Create positions of 2D faces.
    a.map_a = current_pos;
    a.map_b = current_pos;
    a.map_a.x += face_size - 1;
    a.map_b.x += face_size - 1;
    a.map_b.y += face_size - 1;
    b.map_a = current_pos;
    b.map_b = current_pos;
    b.map_a.y += face_size - 1;
    b.map_b.x += face_size - 1;
    b.map_b.y += face_size - 1;
    c.map_a = current_pos;
    c.map_b = current_pos;
    c.map_b.y += face_size - 1;
    d.map_a = current_pos;
    d.map_b = current_pos;
    d.map_b.x += face_size - 1;

    // Create 2D orientations.
    a.facing = 2;
    b.facing = 3;
    c.facing = 0;
    d.facing = 1;

    // Generate the 3D positions of the edges.
    a.edge_center = face + right;
    b.edge_center = face - up;
    c.edge_center = face - right;
    d.edge_center = face + up;

    // Add edges to the edge map.
    edges[current_edge++] = a;
    edges[current_edge++] = b;
    edges[current_edge++] = c;
    edges[current_edge++] = d;

    // Check for new faces.
    pos temp_pos;
    pos3 temp_face, temp_up, temp_right;

    // Fold down.
    temp_pos = current_pos;
    // Modify temp values.
    // Move temp_pos down.
    temp_pos.y += face_size;
    // Rotate up and right and temp_face.
    temp_up = rotate(up, right);
    temp_right = rotate(right, right);
    temp_face = rotate(face, right);
    generate_edges(temp_pos, current_edge, temp_face, temp_up, temp_right);

    // Fold right.
    temp_pos = current_pos;
    // Modify temp values.
    // Move temp_pos right.
    temp_pos.x += face_size;
    // Rotate up and right and temp_face.
    temp_up = rotate(up, up);
    temp_right = rotate(right, up);
    temp_face = rotate(face, up);
    generate_edges(temp_pos, current_edge, temp_face, temp_up, temp_right);

    // Fold left.
    temp_pos = current_pos;
    // Modify temp values.
    // Move temp_pos left.
    temp_pos.x -= face_size;
    // Rotate up and right and temp_face.
    temp_up = rotate(up, up * -1);
    temp_right = rotate(right, up * -1);
    temp_face = rotate(face, up * -1);
    generate_edges(temp_pos, current_edge, temp_face, temp_up, temp_right);
}
