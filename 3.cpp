#include <dirent.h>
#include <errno.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>

using namespace std;

/*function... might want it in some class?*/
int get_dir(string dir, map<string, string> &dirName)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }    

    cout << dir << endl;

    while ((dirp = readdir(dp)) != NULL) {
        string filename = string(dirp->d_name);
        if(filename.compare(".") != 0 && filename.compare(".."))
            cout << "\t" << filename << endl;
            dirName.insert(pair<string, string>(dir, filename));
    }
    closedir(dp);
    return 0;
}

int main(int argc, char* argv[])
{
    map<string, string> dirFiles;
    
    for (int j = 1; j < argc; ++j) {
        string dir = string(argv[j]);

        get_dir(dir, dirFiles);

        cout << endl << endl;

        // for (unsigned int i = 0; i < files.size(); i++) {
        //     cout << files[i] << endl;
        // }
    }

    for(map<string, string >::const_iterator it = dirFiles.begin(); it != dirFiles.end(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }


    return 0;
}