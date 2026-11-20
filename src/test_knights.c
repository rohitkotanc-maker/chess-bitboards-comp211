#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#define NOT_A_FILE  0xfefefefefefefefeULL
#define NOT_H_FILE  0x7f7f7f7f7f7f7f7fULL
#define NOT_AB_FILE 0xfcfcfcfcfcfcfcfcULL
#define NOT_GH_FILE 0x3f3f3f3f3f3f3f3fULL

static uint64_t knight_attacks(uint64_t knights) {
    return ((knights << 17) & NOT_A_FILE) |
           ((knights << 15) & NOT_H_FILE) |
           ((knights << 10) & NOT_AB_FILE) |
           ((knights <<  6) & NOT_GH_FILE) |
           ((knights >> 17) & NOT_H_FILE) |
           ((knights >> 15) & NOT_A_FILE) |
           ((knights >> 10) & NOT_GH_FILE) |
           ((knights >>  6) & NOT_AB_FILE);
}


static uint64_t expected_attacks(int square) {
    static const int df[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };
    static const int dr[8] = { 2, 1, -1, -2, -2, -1, 1, 2 };
    int file = square % 8;
    int rank = square / 8;
    uint64_t answer = 0;
    for (int i = 0; i < 8; ++i) {
        int f = file + df[i];
        int r = rank + dr[i];
        if (f >= 0 && f < 8 && r >= 0 && r < 8) {
            answer |= 1ULL << (r * 8 + f);
        }
    }
    return answer;
}

int main(void) {
    for (int square = 0; square < 64; ++square) {
        assert(knight_attacks(1ULL << square) == expected_attacks(square));
    }

    assert(knight_attacks(1ULL << 0) == ((1ULL << 17) | (1ULL << 10)));
    assert(knight_attacks(1ULL << 63) == ((1ULL << 53) | (1ULL << 46)));
    assert(__builtin_popcountll(knight_attacks(1ULL << 27)) == 8);
    puts("All 64 knight positions passed (including edge and corner cases).");
    return 0;
}
