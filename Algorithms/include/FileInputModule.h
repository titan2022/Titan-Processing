#ifndef FILE_INPUT_MODULE_H
#define FILE_INPUT_MODULE_H

#include "InputModule.h"
#include <filesystem>

using std::string;

class FileInputModule : public InputModule
{
public:
	FileInputModule(const string& name, const string& path);
	string folderPath;
	bool isValid() const override;
	void initialize() override;
	void execute() override;
private:
	vector<string> filePaths;
	std::filesystem::directory_iterator dirIterator;
	static const vector<ImageType> imgTypes;
	int index;
};

#endif