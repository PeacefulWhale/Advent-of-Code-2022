# Day Fifteen

To solve part one I created a list of "ranges" on the given line of where zones were excluded based off sensors.

I simply used the manhattan distance, and then the distance of the sensor to the line. While using ranges was a little bit more complicated than just a very long array, it's quicker and more memory efficient.

Part two was a little bit more complicated.

To solve it I calculated all the points of intersection of the outside part of the beacon ranges. I then took all the intersections where there were 4 beacons that they intersected with (as a valid location requires this), and checked them against all the other beacons to find the valid location.

In my input, I only had 2 locations that had 4 intersections, but it's possible that there are a lot more intersection locations between 4 beacons in other inputs. Maybe I just got lucky.
