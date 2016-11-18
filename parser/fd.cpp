#include"fd.h"
#include<stdio.h>
#include<string>
#include<iostream>
FileDescriptor::FileDescriptor(char const *FileName) {
	fp.open(FileName, ios::in);
	char_number = 0;
	line_number = 1;
	buf_size = 265;
	buffer = new char[buf_size];
	int fileNameLength = strlen(FileName);
	file = new char[fileNameLength];
	fp.getline(buffer, buf_size);
	lineSize = strlen(buffer);
}
bool FileDescriptor::IsOpen() {
	return fp.is_open();
}
char * FileDescriptor::GetFileName() {
	return file;
}
char * FileDescriptor::GetCurrLine() {
	return buffer;
}
int FileDescriptor::GetLineNum() {
	return line_number;
}
int FileDescriptor::GetCharNum() {
	return char_number;
}
FileDescriptor::~FileDescriptor() {
	fp.close();
	delete file;
	delete []buffer;
}
bool FileDescriptor::isEof() {
	return fp.eof();
}
char FileDescriptor::GetChar() {
	
	char c=buffer[char_number];
	if (lineSize == char_number) {
		if (fp.eof() == true)
			return EOF;
		fp.getline(buffer, buf_size);
		lineSize = strlen(buffer);
		char_number = 0;
		line_number++;
		return '\n';
	}
	
		char_number++;
		return c;
}
void FileDescriptor::UngetChar(char c) {
	if (char_number == 0)return;
	char_number--;
	buffer[char_number] = c;
}
void FileDescriptor::ReportError(char * msg) {
	cout << msg << " at " << "line: " << line_number << ", column: " << char_number << endl;
	cout << buffer<<endl;
	for (int i = 0; i < char_number - 1; i++) {
		if (buffer[i] == '\t')
			cout << '\t';
		else
			cout << " ";
	}
	cout << "^" << endl;
	
}
