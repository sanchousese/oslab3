#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <string.h>

using namespace std;

struct stat file_stats;
vector<int> sizes = vector<int>();

bool read_and_store_sizes(string dir) {
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(dir.c_str())) == NULL)
        return false;


    while ((dirp = readdir(dp)) != NULL) {

        string filename = string(dirp->d_name);

        if (filename.compare(".") != 0 && filename.compare("..")) {

            string realFileName = dir + "/" + dirp->d_name;
            char charRealName[realFileName.size() + 1];
            strcpy(charRealName, realFileName.c_str());

            if (!read_and_store_sizes(realFileName)) {
                if (!stat(charRealName, &file_stats)) {
                    cout << filename << "\t";
                    sizes.push_back(file_stats.st_size);
                    cout << "size: " << file_stats.st_size << endl;
                } else {
                    cout << " (stat() failed for this file)\n";
                }
            }

        }
    }
    closedir(dp);
    return true;
}

int main(int argc, char *argv[]) {
    for (int j = 1; j < argc; ++j) {
        string dir = string(argv[j]);

        read_and_store_sizes(dir);

        for (int k = 0; k < sizes.size(); ++k) {
            cout << sizes[k] << endl;
        }
    }

    return 0;
}