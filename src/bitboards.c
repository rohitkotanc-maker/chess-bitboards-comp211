#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* a1 = bit 0, b1 = bit 1, ..., h8 = bit 63. */
#define NOT_A_FILE  0xfefefefefefefefeULL
#define NOT_H_FILE  0x7f7f7f7f7f7f7f7fULL
#define NOT_AB_FILE 0xfcfcfcfcfcfcfcfcULL
#define NOT_GH_FILE 0x3f3f3f3f3f3f3f3fULL

static uint64_t square_mask(int square) {
    return 1ULL << square;
}

static int parse_square(const char *name) {
    if (strlen(name) != 2 || name[0] < 'a' || name[0] > 'h' ||
        name[1] < '1' || name[1] > '8') {
        return -1;
    }
    return (name[1] - '1') * 8 + (name[0] - 'a');
}

static uint64_t knight_attacks(uint64_t knights) {
    /* Filter destination files so a shift cannot wrap across a rank. */
    return ((knights << 17) & NOT_A_FILE) |
           ((knights << 15) & NOT_H_FILE) |
           ((knights << 10) & NOT_AB_FILE) |
           ((knights <<  6) & NOT_GH_FILE) |
           ((knights >> 17) & NOT_H_FILE) |
           ((knights >> 15) & NOT_A_FILE) |
           ((knights >> 10) & NOT_GH_FILE) |
           ((knights >>  6) & NOT_AB_FILE);
}

static void print_board(uint64_t pieces, uint64_t attacks) {
    printf("    a b c d e f g h\n");
    for (int rank = 7; rank >= 0; --rank) {
        printf(" %d  ", rank + 1);
        for (int file = 0; file < 8; ++file) {
            uint64_t bit = square_mask(rank * 8 + file);
            char symbol = (pieces & bit) ? 'N' : (attacks & bit) ? '*' : '.';
            printf("%c ", symbol);
        }
        printf(" %d\n", rank + 1);
    }
    printf("    a b c d e f g h\n");
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [square, e.g. d4]\n", argv[0]);
        return 1;
    }
    const char *name = argc == 2 ? argv[1] : "d4";
    int square = parse_square(name);
    if (square < 0) {
        fprintf(stderr, "Invalid square '%s' (use a1 through h8).\n", name);
        return 1;
    }

    uint64_t knight = square_mask(square);
    uint64_t attacks = knight_attacks(knight);
    printf("Knight on %s; N = knight, * = attacked square\n", name);
    print_board(knight, attacks);
    printf("Knight bitboard: 0x%016" PRIx64 "\n", knight);
    printf("Attack bitboard: 0x%016" PRIx64 "\n\n", attacks);

    /* A separate example of changing the white-pawn bitboard. */
    uint64_t white_pawns = 0x000000000000ff00ULL;
    uint64_t e2 = square_mask(12);
    uint64_t e4 = square_mask(28);
    printf("White pawns before e2-e4: 0x%016" PRIx64 "\n", white_pawns);
    white_pawns &= ~e2;  /* Clear e2 using AND and NOT. */
    white_pawns |= e4;   /* Set e4 using OR. */
    printf("White pawns after  e2-e4: 0x%016" PRIx64 "\n", white_pawns);

    return 0;
}
