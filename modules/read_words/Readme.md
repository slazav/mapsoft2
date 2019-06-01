## read_words

Read one line from the stream and extract words, separated by spaces.
 - Comments (everything from # symbol to end of the line) are skipped.
 - Empty lines are skipped.
 - Words are splitted by ' ' or '\t' symbols.
 - Words can be quoted by " or '.
 - Any symbol (including newline) can be escaped by '\'.
 - Empty vector is returned only at the end of input.
 - If line_num parameter is not NULL, it should be initialized
   by 0,0 before the first call. Then starting line number of the first
   word and number of the last line read is returned there.
 - If lc parameter is true then all characters are converted to lower case.

`std::vector<std::string> read_words(
   std::istream & ss, int line_num[] = NULL, const bool lc=false);`

------------
## Changelog:

2019.06.01 V.Zavjalov 1.1:
- Do not stop reading if newlines is inside quotes.
- Add line counting (line_num parameter).
- Add option for case-insensetive reading (lc parameter).

2019.05.02 V.Zavjalov 1.0:
- First version
