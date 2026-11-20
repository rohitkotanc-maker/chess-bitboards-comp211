# Chess Bitboards: How Chess Engines Use Bitwise Operators

## Overview

For my Checkoff 3 project, I looked at how chess engines use bitwise operators to make chessboards and calculate attacks. I chose this as I wanted to see how different concepts in class are actually applied.

A chessboard has 64 squares and a bitboard represents a set of them using a 64-bit integer ('uint64_t'). Each bit corresponds to a square, and a bit set to 1 means the square belongs to the set while a 0 indicates that it does not. Engines use different bitboards to represent difference piece types and colors.

## What I Explored

### 1. Representing Chess Pieces with Bits

I started learning by understanding how chessboard squares correspond to each bits. A common convention is having a1 at bit 0 and h8 at bit 63.

For example, a white pawn on e2 can be represented in C with:

```c
uint64_t pawn = 1ULL << 12;
```

The left shift places a 1 at the bit corresponding to e2.

### 2. Moving Pieces with Bitwise Operators

I explored how AND, OR, and NOT can update a bitboard when a piece moves.

For example, moving a white pawn from e2 to e4:

```c
uint64_t from = 1ULL << 12;
uint64_t to = 1ULL << 28;

white_pawns &= ~from;
white_pawns |= to;
```

AND with NOT clears the pawn's original square, while OR sets the destination square. This example demonstrates how the bitboard changes; it does not check whether the move is legal.

### 3. Generating Knight Attacks

Another application I explored was generating the squares a knight can attack.

A knight moves in an L shape and can attack up to eight squares. Bit shifts can generate its attack positions, but shifts alone can sometimes cause invalid wraparound across the edges of the chessboard.

Bit masks prevent this by filtering out moves that cross those edges.

I found this particularly interesting because the engine can represent several possible attacks in a single integer.

### 4. Magic Bitboards

I also learned about magic bitboards, which are used to calculate attacks for sliding pieces such as rooks and bishops.

Unlike knights, these pieces can be blocked by other pieces. Their attack patterns therefore depend on the occupied squares.

Magic bitboards use relevant occupancy information, multiplication by a specially chosen magic number, and bit shifts to calculate an index into a precomputed lookup table.

One question I had was how these magic numbers are selected. I learned that developers generally use programs to search for suitable numbers and test whether they produce valid lookup-table mappings.

## Bitboards vs. Arrays

A traditional chessboard representation might use an array with 64 entries. This makes it straightforward to inspect an individual square.

Bitboards offer advantages when working with groups of squares. For example, bitwise AND can find overlapping positions, and OR can combine the locations of different piece types.

However, bitboards are not automatically faster for every operation. They can also be more complicated to implement, and some chess engines use a combination of arrays and bitboards.

## What I Learned

My biggest takeaway was that bitwise operators can do much more than manipulate individual binary digits. They can also represent and process information about an entire set of chessboard squares.

I was especially interested in how simple operations like AND, OR, and shifts could be extended to more advanced techniques like magic bitboards, combining binary arithmetic with precomputed data.

This project helped me connect the material from COMP 211 to the design of real chess software.

## Code Examples

The `src/` directory contains C examples demonstrating bitboard operations and knight attack generation.

### Compiling and Running

To compile the main program:

```bash
cc -std=c11 -Wall -Wextra -pedantic src/bitboards.c -o bitboards
```

To run the program with a knight on d4:

```bash
./bitboards d4
```

To compile and run the knight attack tests:

```bash
cc -std=c11 -Wall -Wextra -pedantic src/test_knights.c -o test_knights
./test_knights
```

These examples use standard C and do not require additional libraries.

## Repository Contents

- `README.md` — Project overview and key discoveries
- `src/` — C examples and tests
- `slides/` — Presentation slides
- `REFERENCES.md` — Additional learning resources

## References

Additional resources are listed in [REFERENCES.md](REFERENCES.md).
