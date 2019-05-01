#ifndef IOFILTER_H
#define IOFILTER_H

#include <string>
#include <iostream>
#include <memory>


/*
  A class for filterng an std::istream through a program.

  constructor:
  IFilter flt(stream, progname);
  flt.stream() - returns reference to a new std::istream.
  Data from flt.stream() will be taken from the filter program
  which read them from original stream.
*/

class IFilter {
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:
    IFilter(std::istream & istr, const std::string & prog);
    ~IFilter();

    std::istream & stream();
};

#endif