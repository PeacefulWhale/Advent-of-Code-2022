#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

struct pos
{
    long long x, y;
};

struct intersection
{
    pos loc;
    int count;
};

struct line
{
    pos one, two;
};

struct range
{
    long long left, right;
};

void add_intersection(std::vector<intersection> *intersection_counts, pos point);
unsigned long part_one(std::vector<pos> sensors, std::vector<pos> beacons, long long y_line);
unsigned long long part_two(std::vector<pos> sensors, std::vector<pos> beacons, long long max_range);

int main(int argc, char **argv)
{
    if (!argc)
    {
        return -1;
    }
    // Parse input.
    std::ifstream input_file(argv[1]);
    std::string line;

    pos sensor;
    pos beacon;
    std::vector<pos> sensors, beacons;
    // Doing this with regex seems like the easiest way of parsing today's input...
    // Hopefully it's not too slow.
    std::regex match("=(.?\\d*).*?=(.?\\d*).*?=(.?\\d*).*?=(.?\\d*)");
    std::smatch matches;
    std::vector<range> beacon_ranges;
    while (std::getline(input_file, line))
    {
        std::regex_search(line, matches, match);
        sensor.x = std::stoll(matches[1]);
        sensor.y = std::stoll(matches[2]);
        beacon.x = std::stoll(matches[3]);
        beacon.y = std::stoll(matches[4]);
        // Part 1:
        // In the row where y=2000000, how many positions cannot contain a beacon?
        sensors.push_back(sensor);
        beacons.push_back(beacon);
    }
    std::cout << "Part 1: " << part_one(sensors, beacons, 2000000) << std::endl;
    std::cout << "Part 2: " << part_two(sensors, beacons, 4000000) << std::endl;
    return 0;
}

unsigned long part_one(std::vector<pos> sensors, std::vector<pos> beacons, long long y_line)
{
    std::vector<range> beacon_ranges;
    for (int i = 0; i < (int)sensors.size(); ++i)
    {
        pos sensor = sensors.at(i);
        pos beacon = beacons.at(i);
        unsigned long long distance = std::abs(sensor.x - beacon.x) + std::abs(sensor.y - beacon.y);
        // Get distance to y line.
        unsigned long long y_distance = std::abs(y_line - sensor.y);
        distance -= y_distance;
        if (distance > 0)
        {
            bool insert_range = true;
            range temp_range;
            temp_range.left = sensor.x - distance;
            temp_range.right = sensor.x + distance;
            // The temp range must be inserted into the proper location, and merged with other ranges if needed.
            for (int i = 0; i < (int)beacon_ranges.size(); ++i)
            {
                // Temp range fully overlaps this beacon.
                if (beacon_ranges.at(i).left > temp_range.left && beacon_ranges.at(i).right < temp_range.right)
                {
                    // Delete this range.
                    beacon_ranges.erase(beacon_ranges.begin() + i);
                }
                // Temp range is fully overlapped.
                else if (beacon_ranges.at(i).left < temp_range.left && beacon_ranges.at(i).right > temp_range.right)
                {
                    // "Delete" temp_range.
                    insert_range = false;
                    break;
                }
                // Temp range partially overlaps this range.
                else if (beacon_ranges.at(i).left > temp_range.left || beacon_ranges.at(i).right < temp_range.right)
                {
                    // Delete this range and expand temp_range.
                    if (beacon_ranges.at(i).left < temp_range.left)
                    {
                        temp_range.left = beacon_ranges.at(i).left;
                    }
                    if (beacon_ranges.at(i).right > temp_range.right)
                    {
                        temp_range.right = beacon_ranges.at(i).right;
                    }
                    beacon_ranges.erase(beacon_ranges.begin() + i);
                }
            }
            // Insert the range.
            if (insert_range)
            {
                beacon_ranges.push_back(temp_range);
            }
        }
    }
    // Part 1:
    unsigned long part_1 = 0;
    for (auto itr : beacon_ranges)
    {
        part_1 += itr.right - itr.left;
    }
    return part_1;
}

unsigned long long part_two(std::vector<pos> sensors, std::vector<pos> beacons, long long max_range)
{
    std::vector<line> possible_solutions;
    // Generate all lines of possible solutions.
    for (int i = 0; i < (int)sensors.size(); ++i)
    {
        pos sensor = sensors.at(i);
        pos beacon = beacons.at(i);
        unsigned long long distance = std::abs(sensor.x - beacon.x) + std::abs(sensor.y - beacon.y);
        distance += 1;
        // Calculate the vertices.
        pos vertices[4];
        vertices[0].x = sensor.x;
        vertices[0].y = sensor.y + distance;
        vertices[1].x = sensor.x + distance;
        vertices[1].y = sensor.y;
        vertices[2].x = sensor.x;
        vertices[2].y = sensor.y - distance;
        vertices[3].x = sensor.x - distance;
        vertices[3].y = sensor.y;
        // Add to the lines.
        line temp_line;
        temp_line.one = vertices[0];
        temp_line.two = vertices[1];
        possible_solutions.push_back(temp_line);
        temp_line.one = vertices[1];
        temp_line.two = vertices[2];
        possible_solutions.push_back(temp_line);
        temp_line.one = vertices[2];
        temp_line.two = vertices[3];
        possible_solutions.push_back(temp_line);
        temp_line.one = vertices[3];
        temp_line.two = vertices[0];
        possible_solutions.push_back(temp_line);
    }
    // Look for intersections.
    std::vector<intersection> intersection_counts;
    for (int i = 0; i < (int)possible_solutions.size() - 1; ++i)
    {
        for (int j = i + 1; j < (int)possible_solutions.size(); ++j)
        {
            line one = possible_solutions.at(i);
            line two = possible_solutions.at(j);
            int a_1 = (one.two.y - one.one.y) / (one.two.x - one.one.x);
            int a_2 = (two.two.y - two.one.y) / (two.two.x - two.one.x);
            if (a_1 != a_2)
            {
                long long b_1 = one.one.y - (a_1 * one.one.x);
                long long b_2 = two.one.y - (a_2 * two.one.x);
                pos point;
                point.x = -(b_1 - b_2) / (a_1 - a_2);
                point.y = (a_1 * point.x) + b_1;
                if ((point.x > std::min(one.one.x, one.two.x) && point.x < std::max(one.one.x, one.two.x)) && (point.x > std::min(two.one.x, two.two.x) && point.x < std::max(two.one.x, two.two.x)))
                {
                    // There's an intersection.
                    if ((point.x < max_range && point.y < max_range) && (point.x > 0 && point.y > 0))
                    {
                        // The intersection is in range.
                        add_intersection(&intersection_counts, point);
                    }
                }
            }
        }
    }
    // Find intersections with 4 intersection counts...
    // And ensure that they're not inside any other squares.
    // My input only had 2 places with >= 4 intersections, but just in case I've added this check.
    for (int i = 0; i < (int)intersection_counts.size(); ++i)
    {
        if (intersection_counts.at(i).count > 4)
        {
            bool valid = true;
            for (int j = 0; j < (int)sensors.size(); ++j)
            {
                pos sensor = sensors.at(j);
                pos beacon = beacons.at(j);
                unsigned long long distance = std::abs(sensor.x - beacon.x) + std::abs(sensor.y - beacon.y);
                unsigned long long distance_two = std::abs(sensor.x - intersection_counts.at(i).loc.x) + std::abs(sensor.y - intersection_counts.at(i).loc.y);
                if (distance_two < distance)
                {
                    valid = false;
                    break;
                }
            }
            if (valid)
            {
                // The solution has been found!
                return (intersection_counts.at(i).loc.x * 4000000) + intersection_counts.at(i).loc.y;
            }
        }
    }
    // This shouldn't happen, but just in case.
    return 0;
}

void add_intersection(std::vector<intersection> *intersection_counts, pos point)
{
    bool insert = true;
    for (int i = 0; i < (int)intersection_counts->size(); ++i)
    {
        if (intersection_counts->at(i).loc.x == point.x && intersection_counts->at(i).loc.y == point.y)
        {
            intersection_counts->at(i).count += 1;
            insert = false;
        }
    }
    if (insert)
    {
        intersection temp;
        temp.loc = point;
        temp.count = 1;
        intersection_counts->push_back(temp);
    }
}
