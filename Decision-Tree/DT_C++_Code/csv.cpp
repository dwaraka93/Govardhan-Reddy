#include "csv.hpp"
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

vector<vector<int>> read_csv(const string &filename) {
  fstream fin;
  string line;
  vector<vector<int>> csv;
    
  fin.open(filename,ios::in);
  if(!fin.is_open())
  {
      cout << "error opening file: "<<filename<< endl;
      return csv;
  }
  while(getline(fin, line)) {
    splitstring strline(line);
    vector<string> fields;
    vector<int> cols;
      fields = strline.split(',');
    //split(fields, line, is_any_of(","));
    // Use const reference
    for(const auto& field : fields) {
      cols.push_back(stoi(field));
    }
    csv.push_back(cols);
  }
  return csv;
}


// split: receives a char delimiter; returns a vector of strings
// By default ignores repeated delimiters, unless argument rep == 1.
vector<string>& splitstring::split(char delim, int rep) {
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    string work = data();
    string buf = "";
    int i = 0;
    while (i < work.length()) {
        if (work[i] != delim)
            buf += work[i];
        else if (rep == 1) {
            flds.push_back(buf);
            buf = "";
        } else if (buf.length() > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
        flds.push_back(buf);
    return flds;
}
