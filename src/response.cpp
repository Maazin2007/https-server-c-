#include "../include/response.h"
#include <fstream>
using namespace std;

string req_to_response(string path, string &method) {
    string response;
    // special case for / 
    if (path == "/") {
        file_to_content("www/index.html", response);
        return response;
    }
    // first we have to fromat the path the meet or standard
    // turning the path from /about -> www/about.html
    path = "www" + path;
    path+= ".html";
    file_to_content(path, response);
    return response;
}

void file_to_content(const string &filepath, string &response) {
    // open the file
    ifstream file(filepath);

    // check if it opened properly
    if (file.is_open()) {
        // read the entire file into a string
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        // need to add header to the response before addig our own content
        response = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n\r\n";
        response += content;
        // sucessfully filled content with file data
    } else {
        // file path does not exist
        response = "HTTP/1.1 404 Not Found\r\n" "Content-Type: text/html\r\n\r\n";
        response += "404";
    }
}
