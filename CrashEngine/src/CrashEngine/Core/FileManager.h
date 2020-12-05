#pragma once

#include <fstream>
#include <string>
#include <sys/stat.h>

namespace CrashEngine {

	class FileManager
	{
	public:

		inline static void CreateFile(std::string directory)
		{
			if (!exists_test3(directory))
			{
				std::ofstream outfile(directory);
				outfile.close();
			}

		}

		inline static void CreateLogFile(std::string fileName)
		{
			std::string path = "C:\\EngineDev\\CrashEngine\\Logs\\" + fileName;

			if (!exists_test3(path))
			{
				std::ofstream outfile(path);
				outfile << "Created new file: " << fileName << std::endl;
				outfile.close();
			}
		}

		inline static void AddTextToFile(std::string directory, std::string text)
		{
			std::fstream file;
			file.open(directory, std::ios::app);
			file << text << std::endl;
			file.close();
		}

		inline static void AddTextLog(std::string fileName, std::string text)
		{
			std::string path = "C:\\EngineDev\\CrashEngine\\Logs\\" + fileName;

			std::fstream file;
			file.open(path, std::ios::app);
			file << text << std::endl;
			file.close();
		}

		inline static bool exists_test3(const std::string& name) 
		{
			struct stat buffer;
			return (stat(name.c_str(), &buffer) == 0);
		}


	};
}