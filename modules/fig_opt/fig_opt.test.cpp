///\cond HIDDEN (do not show this in Doxyden)

#include <cassert>
#include "fig_opt.h"
#include "err/assert_err.h"

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
    assert_eq(o.size(), 3);
    assert_eq(o.get<string>("key1"), "val1");
    assert_eq(o.get<string>("key2"), "1");
    assert_eq(o.get<string>("key3"), "");

    fig_del_opts(str);
    assert_eq(str.size(), 2);
    assert_eq(str[0], "comm1");
    assert_eq(str[1], "comm2=a");

    fig_del_opts(str);
    assert_eq(str.size(), 2);

    fig_add_opts(str, o);
    assert_eq(str.size(), 5);
    assert_eq(str[2], "\\key1=val1");
    assert_eq(str[3], "\\key2=1");
    assert_eq(str[4], "\\key3=");

    fig_add_opts(str, o);
    assert_eq(str.size(), 5);

    o.put("key2", 125);
    o.put("key4", 10);
    fig_add_opts(str, o);
    assert_eq(str.size(), 6);
    assert_eq(str[2], "\\key1=val1");
    assert_eq(str[3], "\\key2=125");
    assert_eq(str[4], "\\key3=");
    assert_eq(str[5], "\\key4=10");

  }
  catch (Err E){
    std::cerr << "Error: " << E.str() << "\n";
    return 1;
  }
  return 0;
}

///\endcond
