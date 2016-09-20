nLife is a cellular automaton I developed based off of Conway's Game of Life (https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

The idea was to adapt Life to allow for more states than just "alive" and "dead".

nLife uses the same basic setup as Life, but with modified rules for what happens each cycle.

-A cell grows by increasing its state number by 1.
-A cell shrinks by decreasing its state number by 1.

-A cell with a state number of 0 cannot shrink.

-A cell surrounded by exactly 3 cells of higher state numbers will grow next cycle.

-Otherwise, if the cell is surrounded by 2 or 3 cells of higher or equal state numbers will stay the same next cycle.

-Otherwise, the cell shrinks.

Conway's Game of Life is actually contained within nLife. Any pattern that starts with only cells of states 1 or 0 plays out exactly the same as it would in Life.

