# Day Nineteen

Today's problem looks like a pathfinding problem... If I had to guess trying to use Dijkstra's algorithm or another BFS wouldn't work... So I'll see where I can get with an A* algorithm.

I don't know if the pathfinding algorithm that I implemented is really an A*, as there isn't really a heuristic, but it essentially some sort of DFS algorithm that runs "instantly". My algorithm worked well enough that I didn't have to change anything for part 2, as it still ran "instantly" (100-150ms according to hyperfine on my computer).

I actually got my optimizations / punnings working before I realized why I wasn't getting the right answers (I had forgotten to factor in existing minerals when calculating how far to "jump" ahead to make a new bot).

For me, today was mostly practicing debugging skills... but I still enjoyed it.
