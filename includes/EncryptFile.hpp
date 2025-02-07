
#ifndef ENCRYPTFILE_HPP
# define ENCRYPTFILE_HPP
# include "policeColor.hpp"
# include <string>
# include <iostream>
# include <fstream>
# include <map>
# include <unordered_map>
# include <vector>
# include <sstream>
# include <set>
# include <random>

class EncryptFile {

public:

    EncryptFile( char **av );
    ~EncryptFile( void );
    

private:

    std::ifstream   ifs;
    std::ofstream   ofs;
    std::string      _outFileName;

    std::map<char, std::vector<std::string>> charMap;
    std::unordered_map<std::string, char> reverseCharMap;
    void            checkMapping();
    void            loadMapping();
    void            encrypt( std::ifstream& inFile );
    void            decrypt( std::ifstream& inFile );
    char findKeyByValue(const std::string& targetValue);
};





#endif /*ENCRYPTFILE_HPP*/