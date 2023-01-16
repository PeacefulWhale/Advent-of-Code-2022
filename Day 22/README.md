# Day Twenty Two

Part one was pretty simple. I use an `unordered_map` again to keep track of my cells, even though a large char map probably would have worked better.

I just wanted to mess around more with maps because I found them interesting. It's probably much less performant than just a big char map, but oh well...

Then part two came along...

Jeez... Normally part two goes hand in hand with part one, and bites you if you did part one in a silly way, but today? Today part two came down from above with a folding chair.

I'm very tempted to just hardcode the folding of my cube, but that seems like less of a challenge...

Whelp... That took me a bit.

I decided to pair up edges by rotating faces in 3D space according to the 2D input, generating their edges (with the edge's mapping to the 2D map), and then pairing up edges with identical locations.

Once I generated the cube, wrapping around it was easy.

I did trip up a little bit on edges that needed to be "swapped", but then I realized that I could use the outgoing and incoming `facing` direction to determine whether or not I had to "swap" the edges.

Honestly even though I'd say part 2 wasn't incredibly easy to make a general solution for, I did enjoy it.
