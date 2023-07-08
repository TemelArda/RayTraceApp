#include "../include/FileUtility.h"
#include <fstream>


namespace FileUtil
{

const std::unordered_multimap<std::string, std::string>  ParseFile(const std::string& fileName) 
{
   std::unordered_multimap<std::string, std::string> fileContents;
   std::ifstream inputFile;
   inputFile.open(fileName);  

   if (inputFile.is_open()) 
   {
      int i = 0;
      std::string currentLine;
    
      while (getline(inputFile, currentLine)) 
      {
         std::string key = currentLine.substr(0, currentLine.find(" "));
         std::string value = currentLine.substr(currentLine.find(" ") + 1, currentLine.length());
         fileContents.emplace(key, value);
      }

      inputFile.close();
      return fileContents;
   }
}
}// namespace FileUtil

