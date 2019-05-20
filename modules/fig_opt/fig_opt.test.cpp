///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "fig_opt.h"

using namespace std;

int
main(){
  try {

    vector<string> str;
    str.push_back("comm1");
    str.push_back("comm2=a");
    str.push_back("\\key1=val1");
    str.push_back("\\key2");
    str.push_back("\\key3=");

    Opt o;
    fig_get_opts(str, o);
    assert (o.size() == 3);
    assert (o.get<string>("key1") == "val1");
    assert (o.get<string>("key2") == "1");
    assert (o.get<string>("key3") == "");

    fig_del_opts(str);
    assert(str.size() == 2);
    assert(str[0] == "comm1");
    assert(str[1] == "comm2=a");

    fig_del_opts(str);
    assert(str.size() == 2);

    fig_add_opts(str, o);
    assert(str.size() == 5);
    assert(str[2] == "\\key1=val1");
    assert(str[3] == "\\key2=1");
    assert(str[4] == "\\key3=");

    fig_add_opts(str, o);
    assert(str.size() == 5);

    o.put("key2", 125);
    o.put("key4", 10);
    fig_add_opts(str, o);
    assert(str.size() == 6);
    assert(str[2] == "\\key1=val1");
    assert(str[3] == "\\key2=125");
    assert(str[4] == "\\key3=");
    assert(str[5] == "\\key4=10");

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
