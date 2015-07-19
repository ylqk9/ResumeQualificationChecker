#include "stdafx.h"
#include "Qualification.h"

qualification::qualification() {
}

qualification::~qualification() {
}

void qualification::clean_content() {
	keywords.resize(0);
	match_ind.assign(0, nquals);
}

int qualification::get_keywords(const char* key_text) {
	string keyword;
	string keybuf(key_text);
	size_t keybg, keyend(0);
	do {
		keybg  = keybuf.find_first_not_of(" ,;\r\n\t",keyend);
		if(keybg != string::npos) {
			keyend = keybuf.find_first_of    (",;\r\n\t",keybg + 1);
			keyword = keybuf.substr(keybg, keyend - keybg);
			//trim
			size_t wdend = keyword.find_last_not_of(" ");
			keywords.push_back(keyword.substr(0,wdend + 1));
		}
	}
	while(keyend != string::npos && keybg != string::npos);
	keywords_nocase = keywords;
	vector_case_lower(keywords_nocase);
	if (keywords.size() == 0) return 1;
	else {
		nkeywords = keywords.size();
		return 0;
	}
}

int qualification::get_qual_list() {
	string file_name = "qualification_pool.txt";
	string qualbuf;
	ifstream QualFile(file_name.c_str(), ifstream::in);
	nquals = 0;
	qualifies.resize(0);
	while(!QualFile.eof()) {
		getline(QualFile, qualbuf, '\n');
		if(!qualbuf.empty()) {
			qualifies.push_back(qualbuf);
			++nquals;
		}
	}
	match_ind.resize(qualifies.size(), 0);
	qualifies_nocase = qualifies;
	vector_case_lower(qualifies_nocase);
	if (nquals == 0) return 1;
	else return 0;
}

int qualification::match_keywords_qual() {
	size_t found = 0;
	nmatches = 0;
	for(int i = 0; i < nkeywords; ++i) {
		for(int j = 0; j < nquals; ++j) {
			found = qualifies_nocase[j].find(keywords_nocase[i]);
			if(found != string::npos) match_ind[j] = 1;
		}
	}
	nmatches = 0;
	for(vector<int>::iterator j = match_ind.begin(); j != match_ind.end(); ++j)
		nmatches += *j;
	return nmatches;
}

void qualification::print_qualifies() {
	for(int i = 0; i < nquals; ++i) {
		if(match_ind[i] == 1) cout << qualifies[i] << endl;
	}
	return;
}

void qualification::print_char() {
	output_all.resize(0);
	for(int i = 0; i < nquals; ++i) {
		if(match_ind[i])
			output_all = output_all + qualifies[i] + "\r\n" ;
	}
}

void qualification::vector_case_lower(vector<string> &lowcase) {
	for(unsigned i = 0; i < lowcase.size(); ++i) {
		transform(lowcase[i].begin(), lowcase[i].end(), lowcase[i].begin(), ::tolower);
	}
	return;
}