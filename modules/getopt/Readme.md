## getopt -- mapsoft getopt wrapper.

parse_options() allows to build a command line interface with many
option groups:
```
program [<general_options>|<global_input_options>]\
        <input_file_1> [<input_options_1>]\
        <input_file_2> [<input_options_2>] ...\
        (--out|-o) <output_file> [<output_options>]
```
(example can be found in `getopt.test.cpp`)

Structure `GetOptEl` is used as an extention `getopt_long (3)`
structure. It contains option description and group number which is used
for selecting a few sets of options from a single list.
Class `GetOptSet` is a container for options.

`parse_options()` can be used to parse a set of options until the
first non-option element.

`parse_options_all()` can be used in simple tools to collect all options
and non-option arguments from a command line.

`HelpPrinter` is a class for formatting help messages. It can also write
`pod` format which can be converted to `html` or `man`.
