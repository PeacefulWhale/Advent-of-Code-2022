#include <unordered_map>
#include <fstream>
#include <iostream>

int wrap(int number, int min, int max)
{
    if (number < min) return (max + 1) - (min - number);
    else if (number > max) return (min - 1) + (number - max);
    else return number;
}

// There are much better way sof doing this, but I wanted to mess around with maps / structs more.
struct pos
{
    int x = 0;
    int y = 0;
    // Oh, I can pass by reference instead of address... neat.
    bool operator==(const pos other) const
    {
        return (x == other.x && y == other.y);
    }
    pos operator+(const pos other) const
    {
        return { x + other.x, y + other.y };
    }
    pos operator-(const pos other) const
    {
        return { x - other.x, y - other.y };
    }
    pos operator*(const int other) const
    {
        return { x * other, y * other };
    }
    void operator+=(const pos other)
    {
        x += other.x;
        y += other.y;
    }
    void operator-=(const pos other)
    {
        x -= other.x;
        y -= other.y;
    }
    void operator*=(const int other)
    {
        x *= other;
        y *= other;
    }
};

// I feel like this would be less performant than not extended the base pos class.
// Especially with the pos::function calls...
// Oh well, it's good to know that this sort of thing is possible.
// But I don't know if it's good practice.
struct pos3: public pos
{
    int z = 0;
    bool operator==(const pos3 other) const
    {
        return (pos::operator==(other) && z == other.z);
    }
    pos3 operator+(const pos3 other) const
    {
        return { pos::operator+(other), z + other.z };
    }
    pos3 operator-(const pos3 other) const
    {
        return { pos::operator-(other), z - other.z };
    }
    pos3 operator*(const int other) const
    {
        return { pos::operator*(other), z * other };
    }
    void operator+=(const pos3 other)
    {
        pos::operator+=(other);
        z += other.z;
    }
    void operator-=(const pos3 other)
    {
        pos::operator-=(other);
        z -= other.z;
    }
    void operator*=(const int other)
    {
        pos::operator*=(other);
        z *= other;
    }
};

// All rotations are in 90 degree angles.
// So no need for fancy math.
// Hardcoding these avoids problems like gimbal lock.
// Rotate 90 degrees according to the given axis.
pos3 rotate(pos3 a, pos3 axis)
{
    // Swap z and y.
    if (axis.x > 0)
    {
        if (a.z)
        {
            return { a.x, -a.z, -a.y };
        }
        else
        {
            return { a.x, a.z, a.y };
        }
    }
    else if (axis.x < 0)
    {
        if (a.z)
        {
            return { a.x, a.z, a.y };
        }
        else
        {
            return { a.x, -a.z, -a.y };
        }
    }
    // Swap x and z.
    else if (axis.y > 0)
    {
        if (a.x)
        {
            return { -a.z, a.y, -a.x };
        }
        else
        {
            return { a.z, a.y, a.x };
        }
    }
    else if (axis.y < 0)
    {
        if (a.x)
        {
            return { a.z, a.y, a.x };
        }
        else
        {
            return { -a.z, a.y, -a.x };
        }
    }
    // Swap y and x.
    else if (axis.z > 0)
    {
        if (a.y)
        {
            return { -a.y, -a.x, a.z };
        }
        else
        {
            return { a.y, a.x, a.z };
        }
    }
    else if (axis.z < 0)
    {
        if (a.y)
        {
            return { a.y, a.x, a.z };
        }
        else
        {
            return { -a.y, -a.x, a.z };
        }
    }
    else
    {
        // This only happens if a 0,0,0 axis is returned.
        return { 0, 0, 0 };
    }
}

pos get_movement(int facing)
{
    pos movement;
    switch (facing)
    {
    case 0:
        movement.x = 1;
        break;
    case 1:
        movement.y = 1;
        break;
    case 2:
        movement.x = -1;
        break;
    case 3:
        movement.y = -1;
        break;
    }
    return movement;
}

bool on_edge(pos x, int facing, pos a, pos b, int test_facing)
{
    // If these points construct a triangle with an area of 0 they are colinear.
    if (((x.x * (a.y - b.y)) + (a.x * (b.y - a.y)) + (b.x * (a.y - b.y))) > 0)
    {
        return false;
    }
    else
    {
        // And check to see if x is between a and b.
        // Offset b by 1.
        if (((x.x >= a.x && x.x <= b.x) || (x.x >= b.x && x.x <= a.x)) &&
            ((x.y >= a.y && x.y <= b.y) || (x.y >= b.y && x.y <= a.y)))
        {
            // Some edges overlap.
            // Confirm that the current facing and the facing when moving to the edge are opposite.
            return !((facing + test_facing) % 2);
        }
        else
        {
            return false;
        }
    }
}

// Manhattan distance.
int distance(pos a, pos b)
{
    return std::abs((b.x - a.x) + (b.y - a.y));
}
