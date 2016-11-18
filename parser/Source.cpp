#include"fd.h"
#include<iostream>
#include"scanner.h"
#include<queue>
using namespace std;
	string spaces = " \n\r\t";
	string operators = "()[]:.;,=+-*/!<>";
	string delimiters=spaces+operators;
bool SCANNER::isSpace(char c) {
	int pos = spaces.find(c);

	if(pos==string::npos)
		return false;
	return true;
}
bool SCANNER::islegalIdStart(char c) {
	if (c >= 'a'&&c <= 'z' || c >= 'A'&&c <= 'Z'||c=='_')
		return true;
	return false;
}
bool SCANNER::isNumber(char c) {
	if (c >= '0'&&c <= '9')
		return true;
	return false;
}
bool SCANNER::isIdDelimiter(char c) {
	if (c == EOF)return true;
	if (delimiters.find(c) == -1)
		return false;
	return true;
}
bool SCANNER::isLegalIdChar(char c) {
	if (islegalIdStart(c) || isNumber(c))
		return true;
	return false;
}
TOKEN * SCANNER::getId(FileDescriptor &f) {
	string id;
	char c = f.GetChar();
	while (isLegalIdChar(c)) {
		id.push_back(c);
		c = f.GetChar();
	}
	f.UngetChar(c);
	if (isIdDelimiter(c)) {

		TOKEN * token = new TOKEN();
		token->type = lx_identifier;
		token->str = id;
		return token;
	}
	else {
		f.ReportError("unvalid Identifier");
		exit(-1);
	}

}
string SCANNER::getInteger(FileDescriptor &f) {
	string number;
	char c = f.GetChar();
	while (isNumber(c))
	{
		number.push_back(c);
		c = f.GetChar();
	}
	if (c == '.') {
		f.ReportError("unvalid float number");
		exit(-1);
	}
	else if (isIdDelimiter(c)) {
		return number;
	}
	else {
		f.ReportError("unvalid float number");
		exit(-1);
	}


}
TOKEN * SCANNER::getNumber(FileDescriptor &f) {
	string number;
	char c = f.GetChar();
	TOKEN * token;
	while (isNumber(c))
	{
		number.push_back(c);
		c = f.GetChar();
	}
	if (c == '.') {
		number.append(getInteger(f));
		token = new TOKEN();
		token->float_value = atof(number.data());
		token->type = lx_float;
		return token;
	}
	else if (isIdDelimiter(c)) {
		token = new TOKEN();
		token->value = atoi(number.data());
		token->type = lx_integer;
		return token;

	}
	else {
		f.ReportError("unvalid intger number");
		exit(-1);
	}


}
TOKEN * SCANNER::getString(FileDescriptor &f) {
	string s;
	char c = f.GetChar();
	TOKEN * token=NULL;
	while (c!='\"')
	{
		s.push_back(c);
		c = f.GetChar();
		if (c == EOF) {
			f.ReportError("non terminated string");
			exit(-1);
		}
	}
	if (c=='\"') {
		token = new TOKEN();
		token->type = lx_string;
		token->str = s;
		return token;

	}
	else {
		f.ReportError("unvalid string");
	}
    return token;

}
bool SCANNER::isOperator(char c) {
	if (operators.find(c) == -1)
		return false;
	return true;
	
}
bool SCANNER::isNextEqualSign(FileDescriptor &f) {
	char c = f.GetChar();
	if (c == '=') {
		return true;
	}

	else
	{
		f.UngetChar(c);
		return false;
	}
}

void SCANNER::toKeyWord(TOKEN * t) {
	string word = t->str;
	int i;
	for ( i = 0; i < 30; i++) {
		if(word.compare(keyword[i])==0)
			break;
	}
	if (i < 30) {
		t->type = key_type[i];
	}
}
void SCANNER::eatComment(FileDescriptor & f) {
	char c = f.GetChar();
	while (c != '#'&&c != '\n'&&c!=EOF) {
		c = f.GetChar();
	}
	if (c == '#') {
		if (f.GetChar() == '#')
			return;
		else
			f.ReportError("unvalid end of comment");
	}
	else {
		return;
	}

}
SCANNER::SCANNER(string fileName){
    file=new FileDescriptor(fileName.data());
}
TOKEN * SCANNER::Scan() {
	string spaces = " \n\r\t";
	string operators = "()[]:.;,=+-*/!<>";
	string delimiters=spaces+operators;
	queue<TOKEN *> q;
		
	char c;
	c = file->GetChar();
	TOKEN * t=NULL;
	while (c!=EOF) {
		if (islegalIdStart(c)) {
			file->UngetChar(c);
			t=getId(*file);
			toKeyWord(t);
			q.push(t);
			if (t->type == lx_identifier) {
				cout << "identifier : " << t->str.data() << endl;
			}
			else{
				cout << "key word : " << t->str.data() << endl;
			}

		}
		else if (isNumber(c)) {
			file->UngetChar(c);
			t = getNumber(*file);
					q.push(t);
			if (t->type == lx_integer)
				cout << "integer :" << t->value << endl;
			else
				cout << "float :" << t->float_value << endl;
		}
		else if (c == '\"') {
			t = getString(*file);
					q.push(t);
			cout << "string :" << t->str.data() << endl;

		}
		else if (isOperator(c)) {
			t = new TOKEN();
			bool nextEqual = isNextEqualSign(*file);
			if (c == ':') {
				if (nextEqual) {
					t->type = lx_colon_eq;
					q.push(t);
				}
				else {
					t->type = lx_colon;
					q.push(t);
				}

			}
			else if (c == '!') {
				if (nextEqual) {
					t->type = lx_neq;
					q.push(t);
				}
				else
					file->ReportError("unvalid operator");

			}
			else if (c == '<') {
				if (nextEqual) {
					t->type = lx_le;
					q.push(t);
				}
				else {
					t->type = lx_lt;
					q.push(t);
				}

			}
			else if (c == '>') {
				if (nextEqual) {
					t->type = lx_ge;
					q.push(t);
				}
				else {
					t->type = lx_gt;
					q.push(t);
				}

			}
			else if (c == '-') {
				t->type = lx_minus;
					q.push(t);

			}
			else {
				int pos = operators.find(c);
					t->type = (LEXEME_TYPE)(34 + pos);
					q.push(t);
			}
			cout << "operator: " << c << endl;
		}
		else if (c == '#') {
			c=file->GetChar();
			if (c == '#') {
				eatComment(*file);
			}
			else {
				file->ReportError("unvalid comment");
				exit(-1);
			}
		}
		else if (isSpace(c)) {
			//cout << "space " << endl;
			// d nothing

		}
		else {
			file->ReportError("unvalid character");
			exit(-1);
		}

	c = file->GetChar();
			}
	
		
	return 0;
}
