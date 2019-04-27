#ifndef READ_WORDS_H
#define READ_WORDS_H

#include <string>
#include <vector>
#include <iostream>

///\addtogroup libmapsoft
///@{

/// Read one line from the stream and extract words, separated by spaces.
/// - comments (everything from # symbol to end of the line) are skipped,
/// - empty lines are skipped,
/// - words are splitted by ' ' or '\t' symbols,
/// - words can be quoted by " or ',
/// - any symbol (including newline) can be escaped by '\',
std::vector<std::string> read_words(std::istream & ss);

///@}
#endif
