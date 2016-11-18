#pragma once
#define SET 1          
#define UNSET 0 
#define BUFFER SIZE 256 
#include<fstream>
using namespace std;
class  FileDescriptor {
	/* You must fill in these fields */       
	private: 
	//	FILE *fp;
		ifstream fp;
		int line_number;  /* line number in the file */ 
		int lineSize;
			 int char_number;  /* character number in the line */ int flag;   /* to prevents two ungets in a row */ int buf_size;   /* stores the buffer size */ 
			 char *buffer;   /* buffer to store a line */ char *file;   /* file name, allocate memory for this */ bool isOpen; // add other fields or functions if you want 


public:

	/* Externally-visible functions: */ 
	char *GetFileName();
	bool isEof();
	bool IsOpen();                    
	// returns true if a file is open without errors,  
	//otherwise returns false 
	char *GetCurrLine();   
	// returns a pointer to buffer, null if EOF 
	int GetLineNum(); 
	int GetCharNum(); 
	FileDescriptor(char const *FileName);
	// opens the stdin FileName is Null 
	~FileDescriptor(); 
	//FileDescriptor();   
	//opens the stdin 
	//void Close ( );        
	// closes the file descriptor
	char  GetChar ();                         
	// Gets the current character in the file            
	// bool open_file_descriptor (char *filename);            
	void ReportError (char *msg);   
	// reports the error specifying the current  line and   
	//character 
	void  UngetChar (char c); 
	// puts back the current character, modifies char number 

};
