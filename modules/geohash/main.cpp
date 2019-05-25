#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>

#include "geohash.h"

void show_usage() {
  std::cerr <<
     "usage: \n"
     "    geohash 'hash'\n\n"
     "example: \n"
     "    geohash c216ne\n"
     "    geohash c23nb62w20sth\n"
     "    geohash xn774c06kdtve\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        show_usage();
        return EXIT_FAILURE;
    }

    std::string hash(argv[1]);

    if (!GEOHASH_verify(hash)) {
        std::cerr << "invalid hash: " << hash << "\n\n";
        return EXIT_FAILURE;
    }

    std::cout
      << "Area: " << GEOHASH_decode(hash) << "\n";

    std::cout
       << "Neighbours:\n"
       << " N: " << GEOHASH_adjacent(hash, 0) << "\n"
       << "NE: " << GEOHASH_adjacent(hash, 1) << "\n"
       << " E: " << GEOHASH_adjacent(hash, 2) << "\n"
       << "SE: " << GEOHASH_adjacent(hash, 3) << "\n"
       << " S: " << GEOHASH_adjacent(hash, 4) << "\n"
       << "SW: " << GEOHASH_adjacent(hash, 5) << "\n"
       << " W: " << GEOHASH_adjacent(hash, 6) << "\n"
       << "NW: " << GEOHASH_adjacent(hash, 7) << "\n";

    return EXIT_SUCCESS;
}

