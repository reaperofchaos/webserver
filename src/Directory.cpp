#include "Directory.h"

using namespace std;
namespace fs = std::filesystem;

std::vector<fs::path> Directory::findInDir(fs::path pathToCheck, bool hidden){
        std::vector<fs::path> files = std::vector<fs::path>{}; 
        const bool isFolder = fs::is_directory(pathToCheck);
        if(isFolder){
            for(const auto & file : fs::directory_iterator(pathToCheck)){
                if(hidden){
                    files.push_back(file); 
                }else{
                    string folderName  = file.path().stem();
                    if(folderName[0] != '.'){
                        files.push_back(file); 
                    }
                }
            }
        }
        return files;
    }

    std::vector<fs::path> Directory::findFoldersInDir(fs::path pathToCheck, bool hidden){
        std::vector<fs::path> folders = std::vector<fs::path>{}; 
        const bool isFolder = fs::is_directory(pathToCheck);
        if(isFolder){
            for(const auto & file : fs::directory_iterator(pathToCheck)){
                const bool fileIsFolder = fs::is_directory(file.path());
                if(fileIsFolder){
                    if(!hidden){
                        folders.push_back(file); 
                    }else{
                        string folderName  = file.path().stem();
                        if(folderName[0] != '.'){
                            folders.push_back(file); 
                        }
                    }
                }
            }
        }
        return folders;
    }

    std::vector<fs::path> Directory::findFilesInDir(fs::path pathToCheck, bool hidden){
        std::vector<fs::path> folders = std::vector<fs::path>{}; 
        const bool isFolder = fs::is_directory(pathToCheck);
        if(isFolder){
            for(const auto & file : fs::directory_iterator(pathToCheck)){
                const bool fileIsFolder = fs::is_directory(file.path());
                if(!fileIsFolder){
                    if(!hidden){
                        folders.push_back(file); 
                    }else{
                        string folderName  = file.path().stem();
                        if(folderName[0] != '.'){
                            folders.push_back(file); 
                        }
                    }
                }
            }
        }
        return folders;
    }

    std::vector<fs::path> Directory::findFilesOfTypeInDir(fs::path pathToCheck, bool hidden, std::string type){
        std::vector<fs::path> folders = std::vector<fs::path>{}; 
        const bool isFolder = fs::is_directory(pathToCheck);
        if(isFolder){
            for(const auto & file : fs::directory_iterator(pathToCheck)){
                const bool fileIsFolder = fs::is_directory(file.path());
                const bool isValidType = file.path().extension() == type; 
                if(!fileIsFolder){
                    if(!hidden && isValidType){
                        folders.push_back(file); 
                    }else{
                        string folderName  = file.path().stem();
                        if(folderName[0] != '.' && isValidType){
                            folders.push_back(file); 
                        }
                    }
                }
            }
        }
        return folders;
    }

    fs::path Directory::findFolderPathNested(std::string nameOfFolder, fs::path pathToCheck){
            fs::path found; 
            for(const auto & file : fs::recursive_directory_iterator(pathToCheck)){
                vector<fs::path> folders = findFoldersInDir(pathToCheck, false);
                if(file.path().stem() == nameOfFolder){
                    std::string parentPath = file.path().parent_path();
                    const bool isNodeModule = parentPath.find("node_modules") != std::string::npos;

                    bool isFolder = fs::is_directory(file.path());
                    if(isFolder && !isNodeModule){
                        return file.path(); 
                    }
                }
            }
            return found; 
    }

    fs::path Directory::findFolderPath(std::string nameOfFolder, fs::path pathToCheck){
            fs::path found; 
            for(const auto & file : fs::directory_iterator(pathToCheck)){
                vector<fs::path> folders = findFoldersInDir(pathToCheck, false);
                if(file.path().stem() == nameOfFolder){
                    std::string parentPath = file.path().parent_path();

                    bool isFolder = fs::is_directory(file.path());
                    if(isFolder){
                        return file.path(); 
                    }
                }
            }
            return found; 
    }

    bool Directory::findFolder(std::string nameOfFolder, fs::path pathToCheck){
        fs::path emptyPath; 
        return (findFolderPath(nameOfFolder, pathToCheck) != emptyPath) ? true: false; 
    }

    bool Directory::findFolderNested(std::string nameOfFolder, fs::path pathToCheck){
        fs::path emptyPath; 
        return (findFolderPathNested(nameOfFolder, pathToCheck) != emptyPath) ? true: false; 
    }

    fs::path Directory::findFolderResultsNested (std::string nameOfFolder, fs::path pathToCheck){
        std::cout << "Checking for " << nameOfFolder << " in " << pathToCheck.stem() << "\n"; 
        fs::path sourcePath;
        bool srcFolderExists = findFolderNested(nameOfFolder, pathToCheck); 
        sourcePath = findFolderPathNested(nameOfFolder, pathToCheck);
        if(srcFolderExists){
            std::cout << "Folder " << nameOfFolder << " found at path " << fs::canonical(sourcePath) << "\n"; 
        }else{
            std::cout << "Folder " << nameOfFolder<< " was not found.\n"; 
        }
        return sourcePath; 
    }

    fs::path Directory::findFolderResults (std::string nameOfFolder, fs::path pathToCheck){
        std::cout << "Checking for " << nameOfFolder << " in " << pathToCheck.stem() << "\n"; 
        fs::path sourcePath;
        bool srcFolderExists = findFolder(nameOfFolder, pathToCheck); 
        sourcePath = findFolderPath(nameOfFolder, pathToCheck);
        if(srcFolderExists){
            std::cout << "Folder " << nameOfFolder << " found at path " << fs::canonical(sourcePath) << "\n"; 
        }else{
            std::cout << "Folder " << nameOfFolder<< " was not found.\n"; 
        }
        return sourcePath; 
    }

    fs::path Directory::findFilePath(std::string nameOfFile, fs::path pathToCheck){
            //std::cout << "Checking for " << nameOfFile << " in " << pathToCheck.stem() << "\n"; 
            fs::path found; 
            for(const auto & file : fs::recursive_directory_iterator(pathToCheck)){

                if(file.path().filename() == nameOfFile){
                    bool isFolder = fs::is_directory(file.path());
                    if(!isFolder){
                        return file.path(); 
                    }
                }
            }
            return found; 
    }

    fs::path Directory::findFilePathForType(std::string nameOfFile, fs::path pathToCheck, std::string type){
            fs::path found; 
            for(const auto & file : fs::recursive_directory_iterator(pathToCheck)){
                if(file.path().stem() == nameOfFile){
                    bool isFolder = fs::is_directory(file.path());
                    bool isFileType = file.path().extension() == type; 
                    if(!isFolder && isFileType){
                        return file.path(); 
                    }
                }
            }
            return found; 
    }

    bool Directory::checkIfFileExists(std::string nameOfFile, fs::path pathToCheck){
        fs::path emptyPath; 
        return (findFilePath(nameOfFile, pathToCheck) != emptyPath) ? true: false; 
    }

    void Directory::displayPathList(std::string listName, std::vector<fs::path> pathList){
        std::cout << listName << ": \n"; 
        for(fs::path file : pathList){
            std::cout << "\t\t" << file.filename() << "\n"; 
        }
        std::cout << "\n"; 
    }

    bool Directory::isComponent(fs::path folder){
        //1. get folders for path
        std::vector<fs::path> subFolders = findFoldersInDir(folder, false);
        for(fs::path subFolder : subFolders){
            if(subFolder.stem() == "component" || subFolder.stem() == "components"){
                return true; 
            }
        }
        return false; 
    }

    std::vector<fs::path> Directory::getComponents(fs::path path){
        std::vector<fs::path> sourceTopFolders = Directory::findFoldersInDir(path, false);
        std::vector<fs::path> components;
        for(fs::path subFolder: sourceTopFolders){
            if(isComponent(subFolder)){
                components.push_back(subFolder);
            }
        }
        return components; 
    }

