#include<fstream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<windows.h>


using namespace std;
char* AppendString(char* in, char* append)
{
	char* ret = (char*)malloc(sizeof(in) + sizeof(append) + 4);
	strcpy(ret, in);
	strcat(ret,  append);
	return ret;
}

int main(int argc, char **argv)
{
	char* filename;
	if (argc != 2)
	{
		cout << "ERROR: no input file" << endl;
		system("PAUSE");
		exit(1);
	}
	else
	{
		filename = argv[1];
	}
	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;
	ifstream input;
	input.open(filename, ifstream::in | ifstream::binary);
	unsigned char *fileData;
	
	if (input.bad())
	{
		cout << "File could not be opened" << endl;
		system("PAUSE");
		return 1;
	}
	input.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
	input.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));
	input.clear();
	input.seekg(fileHeader.bfOffBits, ios_base::beg);
	fileData = (unsigned char*)malloc(infoHeader.biSizeImage);
	if (!fileData)
	{
		free(fileData);
		input.close();
		cout << "Could not allocate memory properly" << endl;
		system("PAUSE");
		return 1;
	}
	input.read((char*)fileData, infoHeader.biSizeImage);
	input.close();

	fstream DataFile;
	char* dataFilename = AppendString(filename,".data");
	DataFile.open(dataFilename, fstream::binary | fstream::out);
	DataFile.write((char*)fileData, infoHeader.biSizeImage);
	DataFile.close();

	if (DataFile.fail())
	{
		cout << "Could not open " << dataFilename << endl;
		system("PAUSE");
		exit(1);
	}
	cout << "When you are finished with databending the picture press any key" << endl;
	system("PAUSE");
	DataFile.open(dataFilename, fstream::binary | fstream::in);
	if (DataFile.fail())
	{
		cout << "Could not open " << dataFilename << endl;
		system("PAUSE");
		exit(1);
	}
	unsigned char *newData;
	newData = (unsigned char*)malloc(infoHeader.biSizeImage);
	DataFile.read((char*)newData, infoHeader.biSizeImage);
	DataFile.close();

	fstream output;
	char* outputFilename = AppendString(filename, ".output.bmp");
	output.open(outputFilename, fstream::binary | fstream::out);
	output.write((char*)&fileHeader, sizeof(BITMAPFILEHEADER));
	output.write((char*)&infoHeader, sizeof(BITMAPINFOHEADER));
	output.seekg(fileHeader.bfOffBits, ios_base::beg);
	output.write((char*)newData, infoHeader.biSizeImage);
	output.close();
}