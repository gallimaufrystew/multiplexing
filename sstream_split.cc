#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

int main() {
    
    std::ifstream ifs("tunnel.conf");
    
    std::string line;
    //std::vector<std::string> pairs(0);

    while (std::getline(ifs, line)) {
        //std::stringstream ss(line);
        std::string key,value;
        std::istringstream is(line);
        is >> key >> value;
        //while (std::getline(ss,part)) {
            std::cout << key << " = " << value << std::endl;
            //pairs.push_back(part);
        //}
    }
    ifs.close();

    return 0;
} 
