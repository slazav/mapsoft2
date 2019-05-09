#ifndef IOFILTER_H
#define IOFILTER_H

#include <string>
#include <iostream>
#include <memory>


/*
  A class for filterng an std::istream through a program.

  constructor:

  IFilter flt(stream, progname) -- run a program and send the stream to its stdin.
  IFilter flt(progname)         -- run a program without sending anything to stdin.

  flt.stream() - returns reference to a new std::istream attached to stdout of the program.
*/

class IFilter {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:
    IFilter(std::istream & istr, const std::string & prog);

    IFilter(const std::string & prog);

    ~IFilter();

    std::istream & stream();
};

class OFilter {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:
    OFilter(std::ostream & ostr, const std::string & prog);

    OFilter(const std::string & prog);

    ~OFilter();

    std::ostream & stream();
};

#endif