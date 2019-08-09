///\cond HIDDEN (do not show this in Doxyden)

#include <unistd.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include "err/err.h"
#include "string_pack.h"

// Packing and unpacking of VMapObj

using namespace std;


int
main(){
  try{

    // low-level functions for packing and unpacking strings and coordinates
    {
      std::ostringstream s1;
      string_pack_str(s1, "str1", "text1");
      string_pack_str(s1, "str2", "text2");

      dMultiLine crds1("[[[-180,-90],[0,0],[180,90]],[],[[37.11,56.20],[37.22,56.11]]]");
      string_pack_crds(s1, "crds", crds1);

      dRect bbox("[100,200,10,10]");
      string_pack_bbox(s1, "bbox", bbox);

      dMultiLine crds2;
      std::istringstream s2(s1.str());
      while (1){
        std::string tag = string_unpack_tag(s2);
        if (tag == "") break;
        else if (tag == "str1") assert(string_unpack_str(s2) == "text1");
        else if (tag == "str2") assert(string_unpack_str(s2) == "text2");
        else if (tag == "crds") crds2.push_back(string_unpack_crds(s2));
        else if (tag == "bbox") assert(string_unpack_bbox(s2) == bbox);
        else throw Err() << "Unknown tag: " << tag;
      }
      assert(dist(crds1,crds2)<1e-7);
    }

  }
  catch (Err e) {
    std::cerr << "Error: " << e.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
