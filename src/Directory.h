#ifndef DIRECTORY_INCLUDED
#define DIRECTORY_INCLUDED
#include <string>  
#include <iostream>  
#include <sstream>  
#include <filesystem>  
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

class Directory {

    public: 
        static std::vector<fs::path> findInDir(fs::path pathToCheck, bool hidden);
        static std::vector<fs::path> findFoldersInDir(fs::path pathToCheck, bool hidden);
        static std::vector<fs::path> findFilesInDir(fs::path pathToCheck, bool hidden);
        static std::vector<fs::path> findFilesOfTypeInDir(fs::path pathToCheck, bool hidden, std::string type);
        static fs::path findFolderPath(std::string nameOfFolder, fs::path pathToCheck);
        static bool findFolder(std::string nameOfFolder, fs::path pathToCheck);
        static fs::path findFolderPathNested(std::string nameOfFolder, fs::path pathToCheck);
        static bool findFolderNested(std::string nameOfFolder, fs::path pathToCheck);
        static fs::path findFolderResults(std::string nameOfFolder, fs::path pathToCheck);
        static fs::path findFolderResultsNested(std::string nameOfFolder, fs::path pathToCheck);
        static fs::path findFilePath(std::string nameOfFile, fs::path pathToCheck);
        static fs::path findFilePathForType(std::string nameOfFile, fs::path pathToCheck, std::string type);
        static bool checkIfFileExists(std::string nameOfFile, fs::path pathToCheck);
        static void displayPathList(std::string listName, std::vector<fs::path> pathList);
        static bool isComponent(fs::path folder);
        static std::vector<fs::path> getComponents(fs::path path);
        
};
#endif