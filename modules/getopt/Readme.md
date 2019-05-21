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

Structure `ext_option` is used as an extention of a structure from
`getopt_long (3)`. It contains option description and group number which
is used for selecting a few sets of options from a single list.

`print_options()` can be used for printing option list (with
descriptions) in text or in pod format (pod then can be used for
generating man pages).

`parse_options_all()` can be used in simple tools to collect all options
and non-option arguments from a command line.
