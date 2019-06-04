///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include "err/err.h"
#include "vmap.h"

// Packing and unpacking of VMapObj

using namespace std;

// low-level functions for packing and unpacking strings and coordinates
void vmap_pack_str(ostream & s, const char *tag, const std::string & str);
void vmap_pack_crds(ostream & s, const dMultiLine & ml);

std::string vmap_unpack_tag(istream & s);
std::string vmap_unpack_str(istream & s);
dLine vmap_unpack_crds(istream & s);

int
main(){
  try{

    // low-level functions for packing and unpacking strings and coordinates
    {
      std::ostringstream s1;
      vmap_pack_str(s1, "str1", "text1");
      vmap_pack_str(s1, "str2", "text2");

      dMultiLine crds1("[[[-180,-90],[0,0],[180,90]],[],[[37.11,56.20],[37.22,56.11]]]");
      vmap_pack_crds(s1, crds1);

      dMultiLine crds2;
      std::istringstream s2(s1.str());
      while (1){
        std::string tag = vmap_unpack_tag(s2);
        if (tag == "") break;
        else if (tag == "str1") assert(vmap_unpack_str(s2) == "text1");
        else if (tag == "str2") assert(vmap_unpack_str(s2) == "text2");
        else if (tag == "crds") crds2.push_back(vmap_unpack_crds(s2));
        else throw Err() << "Unknown tag: " << tag;
      }
      assert(dist(crds1,crds2)<1e-7);
    }

    // packing and unpacking of VMapObj
    {
      VMapObj o1,o2;
      o1.cl = LINE;
      o1.type = 0x2342;
      o1.dir  = BCK;
      o1.name = "object name\nsecond line";
      o1.comm = "object comment\nsecond line";
      o1.src = "object source\nsecond line";
      o1.push_back(dLine("[[0,0],[1,2],[3,4]]"));
      o1.push_back(dLine("[[5,4],[4,3],[3,2]]"));
      std::string pack = o1.pack();
      o2.unpack(pack);
      assert(o1==o2);
    }


  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
