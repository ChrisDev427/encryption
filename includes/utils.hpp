#ifndef UTILS_HPP
# define UTILS_HPP
# include "policeColor.hpp"
# include <string>
# include <iostream>
# include <fstream>
# include <map>
# include <vector>
# include <thread>
# include <chrono>


void            error(std::string  const s1, const char *s2);
void            trimSpaces( std::string& line );
void            cleanLine( std::string & line );
void            checkLine( std::string line, size_t lineNb );
std::ifstream    OpenRead( const char *file );
std::ofstream    OpenWrite( const std::string& filePath, const std::string& dirName );
void progressBar(size_t bytesRead, size_t totalSize, const std::string& phase, bool reset = false);





#endif /*UTILS_HPP*/