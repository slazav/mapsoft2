#include "geo_nom.h"
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

// input/output for nom_scale_t
std::ostream & operator<< (std::ostream & s, const nom_scale_t & sc){
  switch(sc){
    case SC_1M:   s << "1:1000000"; break;
    case SC_500k: s <<  "1:500000"; break;
    case SC_200k: s <<  "1:200000"; break;
    case SC_100k: s <<  "1:100000"; break;
    case SC_50k:  s <<   "1:50000"; break;
    default: throw Err() << "wrong map scale: " << sc;
  }
  return s;
}

std::istream & operator>> (std::istream & s, nom_scale_t & sc){
  std::string str;
  s >> str;
  if (str == "1:1000000" || str == "1:1'000'000" || str == "1000000" ||
      str == "1M" || str == "10km/cm" || str == "10km") {sc=SC_1M; return s;}
  if (str == "1:500000" || str == "1:500'000" || str == "500000" ||
      str == "500k" || str == "5km/cm" || str == "5km") {sc=SC_500k; return s;}
  if (str == "1:200000" || str == "1:200'000" || str == "200000" ||
      str == "200k" || str == "2km/cm" || str == "2km") {sc=SC_200k; return s;}
  if (str == "1:100000" || str == "1:100'000" || str == "100000" ||
      str == "100k" || str == "1km/cm" || str == "1km") {sc=SC_100k; return s;}
  if (str == "1:50000" || str == "1:50'000" || str == "50000" ||
      str == "50k" || str == "500m/cm" || str == "500m") {sc=SC_50k; return s;}
  throw Err() << "can't parse nomenclature map scale: " << str;
}


// structure for a "simple" nomenclature name:
// A-01, A-01-1, A-01-01, A-01-001, or A-01-001-1
// zA-01, zA-01-1, zA-01-01, zA-01-001, or zA-01-001-1
struct nom_struct_t {
  bool north;
  char A;
  int N,N1,N2;
  nom_scale_t sc;
  nom_struct_t(): north(true), A(' '),
     N(0), N1(0), N2(0), sc(SC_1M) {}
};

// Read a number with fixed number of digits.
// On input dig - max.number of digits, on output dig is
// actual number of digits
void read_num(std::istream & f, int & num, int & dig){
  num=0;
  int d;
  for (d=0; d<dig; d++){
    if (f.eof()) break;
    char c = f.peek();
    if (c>='0' && c<='9') f.get();
    else break;
    num = num*10 + (c-'0');
  };
  dig = d;
}

// Read a comma-separated sequence of numbers, <n>,<n+1>,<n+2> etc.
// with same number of digits and 1,2 or 4 elements.
// <dig> on input - max.number of digits, on output - actual number.
// For 1-digit numbers only 1 or 2 numbers are read.
// For other numbers only 1, 2 or 4 numbers are read.
// For 2 numbers should be n1%2==1
// For 4 numbers should be n1%4==1
// Return false if input is bad.
bool read_nums(std::istream & f, int &num1, int &num2, int & dig){
  read_num(f, num1, dig);
  num2 = num1;
  int nn;
  int d;
  while (f.peek() == ',') {
    f.get();
    if (f.eof()) break;
    read_num(f, nn, d);
    if (d!=dig) return false;
    if (nn != num2+1) return false;
    num2=nn;
    if (dig==1 && num2-num1==1) break;
    if (num2-num1==3) break;
  }
  if (num2-num1==2) return false;
  if (num2-num1==1 && num1%2!=1) return false;
  if (num2-num1==3 && num1%4!=1) return false;
  return true;
}

// Read and check head: x?[a-u]-[0-9]{2}(,[0-9]{2})*
// Return false if parsing failed.
bool nom_parse(std::istream & f, nom_struct_t & M1, nom_struct_t & M2){
  M1 = M2 = nom_struct_t(); // initialize

  // Read first character. A..U, possibly prefixed with x
  char c = tolower(f.get());

  // x prefix means southern hemisphere
  if (c == 'x') {
    M1.north = M2.north = false;
    c = tolower(f.get());
  }

  if (f.fail() || c<'a' || c>'u') return false;

  M1.A = M2.A = c;

  // optional '-' separator after the character
  if (f.peek()=='-') f.get();

  // read first group of numbers (2 digits each, separated by ',')
  int n1,n2,dig = 2;
  if (!read_nums(f, n1, n2, dig) || dig!=2 ||
      n1<1 || n2>60) return false;


  M1.N = n1; M2.N = n2;
  if (f.eof() || f.peek() == -1) return true;

  if (n2!=n1) return false;
  if (f.peek() != '-' ) return true; // '-' separator -> stop reading
  f.get();

  // read second group of numbers (1, 2, or 3-digits, separated by ',')
  dig = 3;
  if (!read_nums(f, n1, n2, dig)) return false;
  if      (dig == 1) {
    if (n1<1 || n2>4) return false;
    M1.sc = M2.sc = SC_500k;
  }
  else if (dig == 2) {
    if (n1<1 || n2>36) return false;
    M1.sc = M2.sc = SC_200k;
  }
  else if (dig == 3) {
    if (n1<1 || n2>144) return false;
    M1.sc = M2.sc = SC_100k;
  }
  else return false;


  M1.N1=n1; M2.N1=n2;
  if (f.eof() || f.peek() == -1) return true;

  // 1:100000 may be splitted to 1:50000
  if (M1.sc == SC_100k) {
    if (f.peek() != '-' ) return true; // '-' separator -> stop reading
    f.get();
    dig=1;
    if (!read_nums(f, n1, n2, dig) || dig!=1) return false;
    if (n1<1 || n2>4) return false;
    M1.sc = M2.sc = SC_50k;
    M1.N2 = n1; M2.N2 = n2;
    if (f.eof() || f.peek() == -1) return true;
  }

  // 1:50000 may have form T33-100-3,4,T33-101-3,4 (T and U only!)
  if (M1.sc == SC_50k && M1.A>='t' && M1.A<='u') {
    if (f.peek() != ',' ) return true; // ',' separator -> stop reading
    f.get();

    nom_struct_t M3, M4;
    if (!nom_parse(f, M3, M4)) return false;

    // prefix and letter should be same
    if (M3.north!=M1.north || M3.A!=M1.A) return false;
    // first number should be same
    if (M3.N!=M4.N || M3.N!=M1.N) return false;
    // second number should differ by 1
    if (M3.N1 != M1.N1+1) return false;
    // third group should be same and contain two numbers
    if (M4.N2-M3.N2!=1 || M3.N2!=M1.N2 || M4.N2!=M2.N2) return false;
    M2 = M4;
    if (f.eof() || f.peek() == -1) return true;
  }

  // 1:500000 may have form T33-3,4,T34-3,4 (T and U only!)
  if (M1.sc == SC_500k && M1.A>='t' && M1.A<='u') {
    if (f.peek() != ',' ) return true; // ',' separator -> stop reading
    f.get();

    nom_struct_t M3, M4;
    if (!nom_parse(f, M3, M4)) return false;

    // prefix and letter should be same
    if (M3.north!=M1.north || M3.A!=M1.A) return false;

    // first number should differ by 1
    if (M4.N!=M3.N || M3.N != M1.N+1) return false;
    // second group should be same and contain two numbers
    if (M4.N1-M3.N1!=1 || M3.N1!=M1.N1 || M4.N1!=M2.N1) return false;
    M2 = M4;
    if (f.eof() || f.peek() == -1) return true;
  }
  // reading is not finished!
  return true;
}


// build range for a single sheet, like T01-001-1
dRect nom_to_range(const nom_struct_t & M){
  double lat1,lat2,lon1,lon2;

  // range for 1:1'000'000 map
  lat1 = (M.A-'a')*4; lat2=lat1+4;
  if (!M.north) {lat1 = -lat2; lat2=lat1+4; }
  lon1 = M.N*6 - 186;
  lon2 = lon1 + 6;

  int col,row;
  // range for 1:500'000 map
  if (M.sc == SC_500k) {
    col = (M.N1-1)%2;
    row = 1 - (M.N1-1)/2;
    lon1 += col*6.0/2; lon2 = lon1 + 6.0/2;
    lat1 += row*4.0/2; lat2 = lat1 + 4.0/2;
  }

  else if (M.sc == SC_200k) {
    col = (M.N1-1)%6;
    row = 5 - (M.N1-1)/6;
    lon1 += col*6.0/6; lon2=lon1+6.0/6;
    lat1 += row*4.0/6; lat2=lat1+4.0/6;
  }

  else if (M.sc == SC_100k || M.sc == SC_50k) {
    col = (M.N1-1)%12;
    row = 11 - (M.N1-1)/12;
    lon1 += col*6.0/12; lon2=lon1+6.0/12;
    lat1 += row*4.0/12; lat2=lat1+4.0/12;

    if (M.sc == SC_50k) {
      col = (M.N2-1)%2;
      row = 1 - (M.N2-1)/2;
      lon1 += col*6.0/24; lon2=lon1+6.0/24;
      lat1 += row*4.0/24; lat2=lat1+4.0/24;
    }
  }
  return dRect(dPoint(lon1,lat1), dPoint(lon2,lat2));
}


/************************************************************************/


dRect
nom_to_range(const string & key, nom_scale_t & scale, bool ex){
  istringstream f(key);

  nom_struct_t M1, M2;
  if (!nom_parse(f, M1, M2))
    throw Err() << "nom_to_range: can't parse name: " << key;
  scale = M1.sc;
  dRect r1 = nom_to_range(M1);
  dRect r2 = nom_to_range(M2);
  dRect r = expand(r1,r2);

  int k = int(r.w/r1.w);

  // extended
  if (ex && k==1){
    if (f.get()=='.') {
      int d1=3, d2=3;
      int k1, k2;
      read_num(f, k1, d1);
      if (f.get()!='x')
        throw Err() << "nom_to_range: can't parse name: " << key
                    << ": wrong <M>x<N> range";
      read_num(f, k2, d2);
      if (k1==0 || k2==0)
        throw Err() << "nom_to_range: can't parse name: " << key
                    << ": empty <M>x<N> range";

      r.w *= k1;
      r.h *= k2;
    }
  }
  else {
    // check if doubling is correct
    if (M1.A >= 'a' && M1.A <= 'o' && k!=1)
      throw Err() << "nom_to_range: can't parse name: " << key
                  << ": maps A .. O should be single";
    if (M1.A >= 'p' && M1.A <= 's' && k!=2)
      throw Err() << "nom_to_range: can't parse name: " << key
                  << ": maps P .. S should be in pairs";
    if (M1.A >= 't' && M1.A <= 'u' && k!=4)
      throw Err() << "nom_to_range: can't parse name: " << key
                  << ": maps T .. U should be in quadruples";
  }

  if (!f.eof() || f.peek() != -1)
    throw Err() << "nom_to_range: can't parse name: " << key
                  << ": extra symbols after the name";
  return r;
}

string
pt_to_nom(dPoint p, const nom_scale_t sc, const bool single){
    if ((p.x <-180) || (p.x>180) || (p.y<-90) || (p.y>90))
      throw Err() << "pt_to_nom: bad coordinates: " << p;

    char A = 'a' + (int)floor(abs(p.y)/4);
    int  B = 31 +  (int)floor(p.x/6);

    bool south = p.y<0;
    if (p.y<0) p.y=p.y+88;

    int mult=1;
    if (A>='p' && A<='s') mult = 2;
    if (A>='t' && A<='u') mult = 4;

    int n;
    switch (sc){
      case SC_1M:    n=1; break;
      case SC_500k:  n=2; break;
      case SC_200k:  n=6; break;
      case SC_100k: n=12; break;
      case SC_50k:  n=12; break;
      default: throw Err() << "pt_to_nom: bad scale: " << sc;
    }

    int row=n-1-(int)floor((p.y/4.0-floor(p.y/4))*n);
    int col=(int)floor((p.x/6.0-floor(p.x/6))*n);
    int C = row*n + col+1;

    int D=0;
    if (sc==SC_50k){
      int row1=1-(int)floor((p.y/4.0*n-floor(p.y/4*n))*2);
      int col1=(int)floor((p.x/6.0*n-floor(p.x/6*n))*2);
      D=row1*2+col1+1;
    }

    ostringstream out;

    out << (south?"x":"") << A << setfill('0');
    switch (sc){
      case SC_1M:
        if (mult==1 || single){
          out << setw(2) << B;
          break;
        }
        if (mult==2){
          B=((B-1)/2)*2+1;
          out << setw(2) << B << ","
              << setw(2) << B+1;
          break;
        }
        if (mult==4){
          B=((B-1)/4)*4+1;
          out << setw(2) << B << ","
              << setw(2) << B+1 << ","
              << setw(2) << B+2 << ","
              << setw(2) << B+3;
          break;
        }

      case SC_500k:
        if (mult==1 || single){
          out << setw(2) << B << "-"
              << setw(1) << C;
          break;
        }
        if (mult==2){
          C=((C-1)/2)*2+1;
          out << setw(2) << B << "-"
              << setw(1) << C << ","
              << setw(1) << C+1;
          break;
        }
        if (mult==4){
          B=((B-1)/2)*2+1;
          C=((C-1)/2)*2+1;
          out << setw(2) << B << "-"
              << setw(1) << C << ","
              << setw(1) << C+1
              << "," << (south?"x":"") << A
              << setw(2) << B+1 << "-"
              << setw(1) << C << ","
              << setw(1) << C+1;
          break;
        }

      case SC_200k:
        out << setfill('0') << setw(2) << B << "-";
        if (mult==1 || single){
          out << setw(2) << C;
          break;
        }
        if (mult==2){
          C=((C-1)/2)*2+1;
          out << setw(2) << C << ","
              << setw(2) << C+1;
          break;
        }
        if (mult==4){
          C=((C-1)/4)*4+1;
          out << setw(2) << C << ","
              << setw(2) << C+1 << ","
              << setw(2) << C+2 << ","
              << setw(2) << C+3;
          break;
        }

      case SC_100k:
        out << setfill('0') << setw(2) << B << "-";
        if (mult==1 || single){
          out << setw(3) << C;
          break;
        }
        if (mult==2){
          C=((C-1)/2)*2+1;
          out << setw(3) << C << ","
              << setw(3) << C+1;
          break;
        }
        if (mult==4){
          C=((C-1)/4)*4+1;
          out << setw(3) << C << ","
              << setw(3) << C+1 << ","
              << setw(3) << C+2 << ","
              << setw(3) << C+3;
          break;
        }

      case SC_50k:
        out << setfill('0') << setw(2) << B << "-";
        if (mult==1 || single){
          out << setw(3) << C << "-"
              << setw(1) << D;
          break;
        }
        if (mult==2){
          D=((D-1)/2)*2+1;
          out << setw(3) << C << "-"
              << setw(1) << D << "," << D+1;
          break;
        }
        if (mult==4){
          C=((C-1)/2)*2+1;
          D=((D-1)/2)*2+1;
          out << setw(3) << C << "-"
              << setw(1) << D << "," << D+1 << ","
              << (south?"x":"") << A
              << setw(2) << B << "-"
              << setw(3) << C+1 << "-"
              << setw(1) << D << "," << D+1;
          break;
        }

      default: throw Err() << "pt_to_nom: unknown scale: " << sc;
    }


    return out.str();
}

string
nom_shift(const std::string & name, const iPoint & shift, const bool single){
  nom_scale_t scale;
  dRect r=nom_to_range(name, scale, single);
  return pt_to_nom(r.tlc() + dPoint((0.1+shift.x)*r.w, (0.1+shift.y)*r.h), scale, single);
}


set<string>
range_to_nomlist(const dRect & range, const nom_scale_t rscale, const bool single){
  set<string> ret;
  dPoint delta(1e-9, 1e-9);

  string name=pt_to_nom(range.tlc()+delta, rscale, single);
  if (name == "") return ret;
  ret.insert(name);

  dRect r;
  for (int dy = 0; ;dy++){
    // vertical shift
    string name1 = nom_shift(name, iPoint(0,dy), single);
    for (int dx = 0; ;dx++){
      // horisontal shift: x-distance may differ from direct x-y shift!
      string name2 = nom_shift(name1, iPoint(dx,0), single);
      nom_scale_t sc1;
      r = nom_to_range(name2, sc1, single);
      if (r.tlc().x > range.brc().x - delta.x ||
          r.tlc().y > range.brc().y - delta.y) break;
      if (intersect(range, nom_to_range(name2,sc1,single)).empty()) continue;
      ret.insert(name2);
    }
    if (r.brc().y > range.brc().y) break;
  }
  return ret;
}

/********************************************************************/
// Functions for working with cordinate prefixes and lon0 parameter
// in soviet map coordinates

double
lon2lon0(const double lon){
  double lon0 =floor( lon/6.0 ) * 6 + 3;
  while (lon0>180)  lon0-=360;
  while (lon0<-180) lon0+=360;
  return lon0;
}

int
lon2pref(const double lon){
  double lon0 = lon2lon0(lon);
  return (lon0<0 ? 60:0) + (lon0-3)/6 + 1;
}

double
crdx2lon0(const double X){
  int pref= floor(X/1e6);
  if (pref==0) throw Err() << "zero coordinate prefix";
  return (pref-(pref>30 ? 60:0))*6-3;
}

double
crdx2nonpref(const double X){
  return X - floor( X/1e6 ) * 1e6;
}

