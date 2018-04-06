#include <numeric>
#include <cassert>
#include <string>
#include <vector>
#include "ozi/ozi.h"

using namespace std;

string merge_strings(const vector<string> & v){
  string ret;
  for (size_t i=0; i!=v.size(); ++i) ret += "[" + v[i] + "] ";
  return ret;
}

int main() {

  assert( merge_strings(unpack_ozi_csv(",aa,b,c,ddd,,eee")) ==
          "[] [aa] [b] [c] [ddd] [] [eee] ");
  assert( merge_strings(unpack_ozi_csv("v,,,eee,,")) ==
          "[v] [] [] [eee] [] [] ");
  assert( merge_strings(unpack_ozi_csv("a,b", 5)) ==
          "[a] [b] [] [] [] ");
  assert( merge_strings(unpack_ozi_csv("a", 5)) ==
          "[a] [] [] [] [] ");
  assert( merge_strings(unpack_ozi_csv(",", 5)) ==
          "[] [] [] [] [] ");
  assert( merge_strings(unpack_ozi_csv("", 5)) ==
          "[] [] [] [] [] ");
  assert( merge_strings(unpack_ozi_csv("a,b,c,,d", 1)) ==
          "[a] ");

  assert( pack_ozi_csv(unpack_ozi_csv(",aa,b,c,ddd,,eee")) ==
          ",aa,b,c,ddd,,eee");
  assert( pack_ozi_csv(unpack_ozi_csv("v,,,eee,,")) ==
          "v,,,eee");
  assert( pack_ozi_csv(unpack_ozi_csv("a,b", 5)) ==
          "a,b");
  assert( pack_ozi_csv(unpack_ozi_csv("a,b,c,,d", 1)) == "a");

}
