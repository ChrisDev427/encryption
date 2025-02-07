#include "../includes/EncryptFile.hpp"
#include "../includes/utils.hpp"

EncryptFile::EncryptFile( char **av ) {
    system("clear");
    checkMapping();
    loadMapping();
    if (av[1] ) { ifs = OpenRead( av[1] ); }

    if ( std::string(av[2]) == "encrypt" ) {

        std::string fileName_tmp = av[1];
        size_t pos = fileName_tmp.find(".decrypted");
        if (pos != std::string::npos) {
            fileName_tmp.erase(pos);  // Enlève ".morse" du nom de fichier
        }
        _outFileName = fileName_tmp + ".encrypted";
        ifs = OpenRead( av[1] );
        encrypt( ifs );

    } else {

        std::string fileName_tmp = av[1];
        size_t pos = fileName_tmp.find(".encrypted");
        if (pos != std::string::npos) {
            fileName_tmp.erase(pos);  // Enlève ".morse" du nom de fichier
        }
        _outFileName = fileName_tmp + ".decrypted";
        ifs = OpenRead( av[1] );
        decrypt( ifs );
    }
}

EncryptFile::~EncryptFile( void ) {

    if (ifs.is_open()) ifs.close();
    if (ofs.is_open()) ofs.close();
}

void EncryptFile::encrypt( std::ifstream& inFile ) {
    
    std::string                         line;
    std::string                         outLine;
    std::vector<std::string>            encryptedText;
    std::vector<std::string>            encryptedText_secondPass;
    std::random_device                  rd;  // Générateur de nombres aléatoires basé sur le matériel
    std::mt19937                        rng(rd()); // Initialisation avec une graine aléatoire
    std::uniform_int_distribution<int>  dist(0, 1999); // Plage de 0 à 1999

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t bytesRead = 0;

    // FIRST PASS ******************************************************************************************************************************
    while (std::getline(inFile, line)) {

        bytesRead += line.size() + 1;  // +1 pour le '\n'
        progressBar(bytesRead, fileSize, "First Pass "); // Mise à jour de la barre
        
        if (line.empty()) continue;
        outLine.clear();
        for (size_t i = 0; i < line.length(); i++) {
            // Skip spaces directly
            if (line[i] == ' ') {   
                outLine.append("]*["); 
                continue; 
            }
            // Find the character in map
            auto it = charMap.find( line[i] );
            if (it != charMap.end()) {
                int randomNumber = dist(rng); // Génère un nombre aléatoire
                outLine.append(it->second[randomNumber]); // remplace le caractere par la valeur correspondante [entre 0 et 1999]
            }
        }
        encryptedText.push_back(outLine);
    }
    std::cout << std::endl;

    // SECOND PASS ******************************************************************************************************************************
    fileSize = 0;
    for (const auto& line : encryptedText) {
        fileSize += line.size() + 1;  // +1 pour compter les '\n'
    }
    progressBar(0, fileSize, "Second Pass", true);  // Réinitialisation
    bytesRead = 0;
    size_t partialRead = 0;
    for ( const auto& line : encryptedText ) {
        
        bytesRead += line.size() + 1;
        partialRead += line.size() + 1;
        if (partialRead >= (fileSize / 100)) {  // Mise à jour tous les 1% lus
       
            progressBar(bytesRead, fileSize, "Second Pass");
            partialRead = 0;  // Réinitialiser uniquement quand on met à jour la barre
        }

        outLine.clear();
        for (size_t i = 0; i < line.length(); i++) {

            auto it = charMap.find( line[i] );
            if (it != charMap.end()) {
                int randomNumber = dist(rng); // Génère un nombre aléatoire
                outLine.append(it->second[randomNumber]); // remplace le caractere par la valeur correspondante [entre 0 et 1999]
            }
        }
        encryptedText_secondPass.push_back(outLine);
    }
    ofs = OpenWrite( _outFileName, "encrypted_files" );
    for ( const auto& str : encryptedText_secondPass ) {
        ofs << str << std::endl;
    }
    std::cout << B_GREEN << "\n\n✅ Encryption completed\n\n✅ '" << _outFileName << "' has been created." << std::endl;    
}

void EncryptFile::decrypt( std::ifstream& inFile ) {

    std::string                 line;
    std::string                 outLine;
    std::string                 extractedValue;
    std::vector<std::string>    decryptedText_secondPass;
    std::vector<std::string>    decryptedText_firstPass;
    size_t                      charCounter = 0;
    
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t bytesRead = 0;

    // FIRST PASS ******************************************************************************************************************************
    while (std::getline(inFile, line)) {

        bytesRead += line.size() + 1;  // +1 pour le '\n'
        progressBar(bytesRead, fileSize, "First Pass "); // Mise à jour de la barre
        outLine.clear();
        extractedValue.clear();

        for (size_t i = 0; i < line.length(); i++) {
            
            extractedValue += line[i];
            charCounter++;

            // Vérifier si on a une séquence complète de 10 caractères
            if (charCounter == 10) {
                outLine += findKeyByValue(extractedValue);
                charCounter = 0;
                extractedValue.clear();
            }
        }
        decryptedText_firstPass.push_back(outLine);
    }
    std::cout << std::endl;

   // SECOND PASS ******************************************************************************************************************************

    fileSize = 0;

    for (const auto& line : decryptedText_firstPass) {
        fileSize += line.size() + 1;  // +1 pour compter les '\n'
    }
    progressBar(0, fileSize, "Second Pass", true);  // Réinitialisation

    bytesRead = 0;
    size_t partialRead = 0;
    for (const auto& line : decryptedText_firstPass) {
       
        bytesRead += line.size() + 1;
        partialRead += line.size() + 1;
        if (partialRead >= (fileSize / 100)) {  // Mise à jour tous les 1% lus
       
            progressBar(bytesRead, fileSize, "Second Pass");
            partialRead = 0;  // Réinitialiser uniquement quand on met à jour la barre
        }
    
        outLine.clear();
        extractedValue.clear();
        for (size_t i = 0; i < line.length(); i++) {

            // Vérifier si on rencontre la séquence "]*["
            if (i + 2 < line.length() && line[i] == ']' && line[i + 1] == '*' && line[i + 2] == '[') {
                outLine += ' ';  // Remplace par un espace
                i += 2; // Sauter "]*["
                charCounter = 0; // Réinitialiser le compteur
                extractedValue.clear();
                continue;
            }

            extractedValue += line[i];
            charCounter++;

            // Vérifier si on a une séquence complète de 10 caractères
            if (charCounter == 10) {
                outLine += findKeyByValue(extractedValue);
                charCounter = 0;
                extractedValue.clear();
            }
        }
        decryptedText_secondPass.push_back(outLine);
    }
    ofs = OpenWrite( _outFileName, "decrypted_files" );
    for (const auto& str : decryptedText_secondPass ) {
        ofs << str << std::endl;
    }
    std::cout << B_GREEN << "\n\n✅ Decryption completed\n\n✅ '" << _outFileName << "' has been created." << std::endl;    
}

char EncryptFile::findKeyByValue(const std::string& targetValue) {
    auto it = reverseCharMap.find(targetValue);
    if (it != reverseCharMap.end()) {
        return it->second;  // Retourne la clé associée
    }
    return '\0';  // Retourne un caractère nul si non trouvé
}

void EncryptFile::checkMapping() {

    if (!ifs) {
        error("mapping.txt", "missing file, the mapping can't be loaded !");
        return;
    }

    const char *fileName = "./data/mapping.txt";
    std::ifstream                                               ifs = OpenRead(fileName);
    std::string                                                 line;
    size_t                                                      lineNb = 0;    
    std::unordered_map<std::string, std::pair<char, size_t>>    globalMap;

    while (std::getline(ifs, line)) {

        lineNb++;

        trimSpaces(line);  // Supprime les espaces inutiles
        cleanLine(line);   // Nettoie la ligne
        checkLine(line, lineNb);  // Vérifie la syntaxe

        if (line.empty()) continue;

        char key = line[0];  // Première lettre comme clé
        std::istringstream iss(line.substr(2)); // Ignore la clé et l’espace

        std::string value;
        while (iss >> value) {
            auto it = globalMap.find(value);
            if (it != globalMap.end()) {
                // Si la valeur existe déjà, afficher un message d'avertissement détaillé
                std::cerr << B_RED << "Error: " << fileName << ": Duplicate value: '" << value << "' found at line " 
                          << lineNb << " for key '" << key << "', but already used at line " 
                          << it->second.second << " for key '" << it->second.first << "'!" 
                          << RESET << std::endl;
                          exit (EXIT_FAILURE);
            } else {
                // Si la valeur est nouvelle, on l'ajoute avec la clé et le numéro de ligne
                globalMap[value] = {key, lineNb};
            }
        }
    }
    std::cout << B_GREEN << "\n✅ The mapping has been successfully checked." << RESET << std::endl;
}

void EncryptFile::loadMapping() {
    std::ifstream ifs = OpenRead("./data/mapping.txt");

    if (!ifs) {
        error("mapping.txt", "missing file, the mapping can't be loaded !");
        return;
    }

    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) continue;  // Ignorer les lignes vides

        char key = line[0];  // La clé est le premier caractère
        std::vector<std::string> values;
        std::istringstream iss(line.substr(2));  // Supprime la clé + l'espace après

        std::string value;
        while (iss >> value) {  // Lire chaque bloc séparé par un espace
            values.push_back(value);
            reverseCharMap[value] = key;  // Ajout direct dans la map inversée
        }

        charMap[key] = values;  // Ajouter à la map
    }

    std::cout << B_GREEN << "\n✅ The mapping has been successfully loaded.\n" << std::endl;

    // Affichage du mapping chargé
    // for (const auto& pair : charMap) {
    //     std::cout << pair.first << " -> ";
    //     for (const auto& str : pair.second) {
    //         std::cout << str << " ";
    //     }
    //     std::cout << std::endl;
    // }
}
