#pragma once
#include <string>
using namespace std;

void file_to_content(const string &filepath, string &response);
string req_to_response(string path, string &method);