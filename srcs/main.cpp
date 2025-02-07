
#include "../includes/EncryptFile.hpp"


int main(int ac, char **av) {
    
    if ((ac == 3)  && (std::string(av[2]) == "encrypt" || std::string(av[2]) == "decrypt")) {

       EncryptFile inst(av);

    }
    else {
        std::cout << "Error: This program need two arguments !" << std::endl;
        std::cout << "Ex: [./program name] [text file] [command: 'encrypt' or 'decrypt']" << std::endl;
    }
    
    return 0; 
}