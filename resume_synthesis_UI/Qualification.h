#include "stdafx.h"
using namespace std;

class qualification
{
public:
	int nmatches;
	vector<string> qualifies;
	vector<string> keywords;
	vector<int>    match_ind;
	string output_all;
	qualification();
	~qualification();
	void clean_content();
	int get_keywords(const char* key_text);
	int get_qual_list();
	int match_keywords_qual();
	void print_qualifies();
	void print_char();
private:
	int nkeywords;
	int nquals;
	vector<string> qualifies_nocase;
	vector<string> keywords_nocase;
	void vector_case_lower(vector<string> &lowcase);
};