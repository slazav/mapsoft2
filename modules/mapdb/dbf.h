#ifndef DBF_H
#define DBF_H

#include <memory>
#include <string>

// Shapelib wrapper for DBF files
class Dbf{
  private:
    class Impl;
    std::unique_ptr<Impl> impl;

  public:

    Dbf(const char *fbase, bool create);
    ~Dbf();

    // number of fields
    int nfields() const;

    // number of records
    int nrecords() const;

    // field type:
    //'C' (String), 'D' (Date), 'F' (Float)
    // 'N' (Numeric, with or without decimal)
    // 'L' (Logical), 'M' (Memo: 10 digits .DBT block ptr),
    // ' ' (field out of range).
    char field_type(int fid) const;

    // find field ID ny name (-1 if not found)
    int field_find(const char * name) const;

    // add string field, return field ID or -1 on failure
    int add_str_field(const char *name, int size);

    // write string attribute
    bool put_str(int id, int fid, const char * val);

    // get attribute (string representation of any type)
    // or empty string in case of error
    std::string get_str(int id, int fid);

};

#endif
