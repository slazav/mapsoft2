#ifndef READ_WORDS_H
#define READ_WORDS_H

#include <string>
#include <vector>
#include <iostream>

///\addtogroup libmapsoft
///@{

/*
 Read one line from the stream and extract words, separated by spaces.
 - Comments (everything from # symbol to end of the line) are skipped.
 - Empty lines are skipped.
 - Words are splitted by ' ' or '\t' symbols.
 - Words can be quoted by " or '.
 - Any symbol (including newline) can be escaped by '\'.
 - Empty vector is returned only at the end of input.
 - If line_num parameter is not NULL, it should be initialized
   by 0,0 before the first call. Then line number of the first word and
   number of the last read wline is returned there.
 - If lc parameter is true then all characters are converted to lower case.
*/
std::vector<std::string> read_words(
  std::istream & ss,
  int line_num[2] = NULL,
  const bool lc = false);

///@}
#endif
