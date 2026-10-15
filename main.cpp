#include <iostream>
#include <string>

#include "commands.h"

int main(int argc, char* argv[]) {
    std::string file;
    std::string query;

    for (int i = 1; i + 1 < argc; i += 2) {
        std::string key = argv[i];
        if (key == "--file") {
            file = argv[i + 1];
        } else if (key == "--query") {
            query = argv[i + 1];
        }
    }

    if (file.empty() || query.empty()) {
        std::cout << "Usage: ./dbms --file <file> --query '<COMMAND args>'" << std::endl;
        return 1;
    }

    if (!runQuery(file, query)) {
        return 1;
    }
    return 0;
}