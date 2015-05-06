#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*function... might want it in some class?*/
int get_dir(string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        string filename = string(dirp->d_name);
        if(filename.compare(".") != 0 && filename.compare(".."))
            files.push_back(filename);
    }
    closedir(dp);
    return 0;
}

int main(int argc, char* argv[])
{
    for (int j = 1; j < argc; ++j) {
        string dir = string(argv[j]);
        vector<string> files = vector<string>();

        get_dir(dir, files);

        for (unsigned int i = 0; i < files.size(); i++) {
            cout << files[i] << endl;
        }
    }

    return 0;
}