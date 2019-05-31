/*
 Based on https://github.com/lyokato/libgeohash
 Modified for mapsoft2: slazav@altlinux.org, 2019-05-25
*/

#include <ctype.h>
#include <cstring>
#include <cassert>
#include "geohash.h"

#define MAX_HASH_LENGTH 22

static const char BASE32_ENCODE_TABLE[33] = "0123456789bcdefghjkmnpqrstuvwxyz";
static const char BASE32_DECODE_TABLE[44] = {
    /* 0 */   0, /* 1 */   1, /* 2 */   2, /* 3 */   3, /* 4 */   4,
    /* 5 */   5, /* 6 */   6, /* 7 */   7, /* 8 */   8, /* 9 */   9,
    /* : */  -1, /* ; */  -1, /* < */  -1, /* = */  -1, /* > */  -1,
    /* ? */  -1, /* @ */  -1, /* A */  -1, /* B */  10, /* C */  11,
    /* D */  12, /* E */  13, /* F */  14, /* G */  15, /* H */  16,
    /* I */  -1, /* J */  17, /* K */  18, /* L */  -1, /* M */  19,
    /* N */  20, /* O */  -1, /* P */  21, /* Q */  22, /* R */  23,
    /* S */  24, /* T */  25, /* U */  26, /* V */  27, /* W */  28,
    /* X */  29, /* Y */  30, /* Z */  31
};

static const char NEIGHBORS_TABLE[8][33] = {
    "p0r21436x8zb9dcf5h7kjnmqesgutwvy", /* NORTH EVEN */
    "bc01fg45238967deuvhjyznpkmstqrwx", /* NORTH ODD  */
    "bc01fg45238967deuvhjyznpkmstqrwx", /* EAST EVEN  */
    "p0r21436x8zb9dcf5h7kjnmqesgutwvy", /* EAST ODD   */
    "14365h7k9dcfesgujnmqp0r2twvyx8zb", /* SOUTH EVEN */
    "238967debc01fg45kmstqrwxuvhjyznp", /* SOUTH ODD  */
    "238967debc01fg45kmstqrwxuvhjyznp", /* WEST EVEN  */
    "14365h7k9dcfesgujnmqp0r2twvyx8zb"  /* WEST ODD   */
};

static const char BORDERS_TABLE[8][9] = {
    "prxz",     /* NORTH EVEN */
    "bcfguvyz", /* NORTH ODD */
    "bcfguvyz", /* EAST  EVEN */
    "prxz",     /* EAST  ODD */
    "028b",     /* SOUTH EVEN */
    "0145hjnp", /* SOUTH ODD */
    "0145hjnp", /* WEST  EVEN */
    "028b"      /* WEST  ODD */
};

bool
GEOHASH_verify(const std::string & hash) {
    for (auto c0: hash) {
        unsigned char c = toupper(c0);
        if (c < 0x30) return false;
        c -= 0x30;
        if (c > 43 || BASE32_DECODE_TABLE[c] == -1) return false;
    }
    return true;
}

#define REFINE_RANGE(mid, min, max, bits, offset) \
    mid = ((max) + (min)) / 2.0; \
    if (((bits) & (offset)) == (offset)) \
        (min) = mid; \
    else \
        (max) = mid;

dRect
GEOHASH_decode(const std::string & hash) {

    if (!GEOHASH_verify(hash)) return dRect();

    double min1 = -180, max1 = 180;
    double min2 =  -90, max2 =  90;

    for (auto c: hash) {
        // hash already verified.
        double mid;
        char bits = BASE32_DECODE_TABLE[toupper(c)-0x30];
        REFINE_RANGE(mid, min1, max1, bits, 0x10);
        REFINE_RANGE(mid, min2, max2, bits, 0x08);
        REFINE_RANGE(mid, min1, max1, bits, 0x04);
        REFINE_RANGE(mid, min2, max2, bits, 0x02);
        REFINE_RANGE(mid, min1, max1, bits, 0x01);
        std::swap(min1, min2);
        std::swap(max1, max2);
    }
    if (hash.size()%2){
      std::swap(min1, min2);
      std::swap(max1, max2);
    }
    return dRect(dPoint(min1,min2), dPoint(max1,max2));
}

#define SET_BIT(bits, mid, min, max, value, offset) \
    mid = ((max) + (min)) / 2.0; \
    if ((value) >= mid) { \
        (min) = mid; \
        (bits) |= (0x1 << (offset)); \
    } else { \
        (max) = mid; \
        (bits) |= (0x0 << (offset)); \
    }

std::string
GEOHASH_encode(const dPoint & p, unsigned int len) {
    assert(p.y >= -90.0);
    assert(p.y <= 90.0);
    assert(p.x >= -180.0);
    assert(p.x <= 180.0);
    assert(len <= MAX_HASH_LENGTH);

    std::string hash(len, ' ');

    double min1 = -180, max1 = 180, val1 = p.x;
    double min2 =  -90, max2 =  90, val2 = p.y;

    for (int i=0; i < len; i++) {
        double mid;
        unsigned char bits = 0;
        SET_BIT(bits, mid, min1, max1, val1, 4);
        SET_BIT(bits, mid, min2, max2, val2, 3);
        SET_BIT(bits, mid, min1, max1, val1, 2);
        SET_BIT(bits, mid, min2, max2, val2, 1);
        SET_BIT(bits, mid, min1, max1, val1, 0);

        hash[i] = BASE32_ENCODE_TABLE[bits];

        std::swap(val1, val2);
        std::swap(min1, min2);
        std::swap(max1, max2);
    }
    return hash;
}

std::string
GEOHASH_encode(const dRect & r, unsigned int maxlen) {
    // encode 4 corners
    std::string h1 = GEOHASH_encode(r.tlc(), maxlen);
    std::string h2 = GEOHASH_encode(r.trc(), maxlen);
    std::string h3 = GEOHASH_encode(r.brc(), maxlen);
    std::string h4 = GEOHASH_encode(r.blc(), maxlen);

    assert(h1.size() == h2.size());
    assert(h1.size() == h3.size());
    assert(h1.size() == h4.size());
    assert(h1.size() == maxlen);

    int i;
    for (i=0; i<maxlen; i++){
     if (h1[i] != h2[i] || h1[i] != h3[i] || h1[i] != h4[i]) break;
    }
    return h1.substr(0,i);
}

std::set<std::string>
GEOHASH_encode4(const dRect & r, unsigned int maxlen) {
    // encode 4 corners
    std::string h1 = GEOHASH_encode(r.tlc(), maxlen);
    std::string h2 = GEOHASH_encode(r.trc(), maxlen);
    std::string h3 = GEOHASH_encode(r.blc(), maxlen);
    std::string h4 = GEOHASH_encode(r.brc(), maxlen);
    std::string h1s, h2s, h3s, h4s;

    assert(h1.size() == h2.size());
    assert(h1.size() == h3.size());
    assert(h1.size() == h4.size());
    assert(h1.size() == maxlen);

    int i;
    for (i=0; i<maxlen; i++){
     // all 4 hashes are same
     if (h1[i] == h2[i] && h1[i] == h3[i] && h1[i] == h4[i]) continue;
     // check if 4 hashes are adjacent (or same)
     h1s = h1.substr(0,i+1);
     h2s = h2.substr(0,i+1);
     h3s = h3.substr(0,i+1);
     h4s = h4.substr(0,i+1);
     // 3 4
     // 1 2
     if ((h1s == h2s || GEOHASH_adjacent(h1s,2)==h2s) &&
         (h3s == h4s || GEOHASH_adjacent(h3s,2)==h4s) &&
         (h1s == h3s || GEOHASH_adjacent(h1s,0)==h3s) &&
         (h2s == h4s || GEOHASH_adjacent(h2s,0)==h4s)) continue;
     break;
    }
    h1s = h1.substr(0,i);
    h2s = h2.substr(0,i);
    h3s = h3.substr(0,i);
    h4s = h4.substr(0,i);
    // find unique hashes:
    std::set<std::string> ret;
    ret.insert(h1s);
    if (h2s!=h1s)
      ret.insert(h2s);
    if (h3s!=h1s && h3s!=h2s)
      ret.insert(h3s);
    if (h4s!=h1s && h4s!=h2s && h4s!=h3s)
      ret.insert(h4s);
    return ret;
}


std::string
GEOHASH_adjacent(const std::string & hash, int dir) {

    dir = dir%8;

    // even dirs
    if (dir==1) return GEOHASH_adjacent(GEOHASH_adjacent(hash,0),2);
    if (dir==3) return GEOHASH_adjacent(GEOHASH_adjacent(hash,4),2);
    if (dir==5) return GEOHASH_adjacent(GEOHASH_adjacent(hash,4),6);
    if (dir==7) return GEOHASH_adjacent(GEOHASH_adjacent(hash,0),6);

    // odd dirs
    int len  = hash.size();
    if (len==0) return "";

    char last = tolower(hash[len-1]);

    int idx  = dir + (len % 2);
    const char *border_table = BORDERS_TABLE[idx];

    std::string base(hash);
    base.resize(len-1);

    if (strchr(border_table, last) != NULL) {
        std::string refined_base = GEOHASH_adjacent(base, dir);
        if (refined_base == "") return "";
        base = refined_base;
    }

    const char *neighbor_table = NEIGHBORS_TABLE[idx];

    char *ptr = strchr((char*)neighbor_table, last);
    if (ptr == NULL) return "";

    idx = (int)(ptr - neighbor_table);
    base.push_back(BASE32_ENCODE_TABLE[idx]);

    return base;
}
