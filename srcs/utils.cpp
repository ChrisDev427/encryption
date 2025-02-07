#include "../includes/utils.hpp"
#include <filesystem>

void    error(std::string  const s1, const char *s2) {

    std::cout << B_RED << "Error: ";
    std::cout << s1 << " ";
    if ( s2 )
        std::cout << s2;
    std::cout << RESET << std::endl;
    exit (EXIT_FAILURE); 
}

void trimSpaces( std::string& line ) {

    int i = 0;
    while( isspace(line[i]) ) { i++; }
    if ( i > 0 ){
        line = line.substr(i);
    }

    std::string::reverse_iterator rit = line.rbegin();
    while (rit != line.rend() && std::isspace(*rit)) { rit++; }
    line.erase(rit.base(), line.end());
}

void cleanLine( std::string & line ) {

    for (size_t i = 0; i < line.size(); i++) {
        if (isspace(line[i])) {
            while (i + 1 < line.size() && isspace(line[i + 1])) {
                line.erase(i + 1, 1);
            }
        }
    }
}

void checkLine( std::string line, size_t lineNb ) {

    if ( line[1] != ' ' ) {
        error( "Syntax: line: " + std::to_string(lineNb), NULL );
    }
    size_t rankNb = 0;

    for (size_t i = 2; i < line.size(); i++) {
        size_t keyLen = 0;
        rankNb++;
        while ( line[i] && line[i] != ' ' ) { i++; keyLen++; }
        if ( keyLen != 10 ) {
            error( ("Syntax: line: " + std::to_string(lineNb)) 
                + (" key: " + std::to_string(rankNb)), "Key must be 10 chars" );
        }
    }
}

std::ifstream    OpenRead( const char *file ) {
    
    std::ifstream   ifs(file);
    if ( !ifs ) {

        error( file, "no file match found" );

    }
    return ifs;
}

std::string getFileName(const std::string& filePath) {
    return std::filesystem::path(filePath).filename().string();
}

std::ofstream OpenWrite(const std::string& filePath, const std::string& dirName) {
    std::string fileName = getFileName(filePath);
    std::string directory = dirName;  // Nom du dossier où stocker les fichiers chiffrés
    std::string fullPath = directory + "/" + fileName;

    // Vérifier si le dossier existe, sinon le créer
    if (!std::filesystem::exists(directory)) {
        std::filesystem::create_directory(directory);
    }

    std::ofstream ofs(fullPath);
    if (!ofs) {
        std::cerr << " Error: Unable to create file '" << fullPath << "'." << std::endl;
        exit(EXIT_FAILURE);
    }

    return ofs;
}


void progressBar(size_t bytesRead, size_t totalSize, const std::string& phase, bool reset) {
    static size_t lastProgress = 0;
    if (reset) {
        lastProgress = 0;  // Réinitialisation
    }
    int barWidth = 50;
    size_t progress = (bytesRead * 100) / totalSize;

    // Empêcher les mises à jour trop fréquentes
    if (progress <= lastProgress) return;

    // Animation plus fluide : interpolation entre l'ancienne et la nouvelle valeur
    for (size_t p = lastProgress + 1; p <= progress; ++p) {
        std::cout << B_GREEN << "\r" << phase << " ["; 

        int pos = (p * barWidth) / 100;
        for (int i = 0; i < barWidth; i++) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }

        std::cout << "] " << p << "%";
        std::cout.flush();

        std::this_thread::sleep_for(std::chrono::milliseconds(25));  // Ajuste la fluidité
    }
    lastProgress = progress;
}