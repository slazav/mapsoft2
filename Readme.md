##### New mapsoft library


#####################################################
#### Err class -- error handling

All mapsoft libraries throw human-readable text exceptions using a
simple Err class. There is also a possibility to transfer an integer error
code (default -1).


## Functions

```cpp
Err(int code=-1);   // create an error with some integer code
Err(const Err & o); // copy constructor

// operator<< for error messages
template <typename T> Err & operator<<(const T & o);

// get error code
int code() const {return code;}

// return error message
std::string str() const;
```

## Example:

```cpp
try {
  // throw an error with any text:
  throw Err() << "pipe " << n << "is blocked!";

  // some error code can be added:
  throw Err(-2) << "some other error";
}

// catch an error:
catch (Err E){
  cerr << E.get_error();
}
```
