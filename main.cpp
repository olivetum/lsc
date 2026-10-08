#include <filesystem>
#include <iostream>
using namespace std;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */

static bool checkForDir() { return true; };

int main(const int argc, char *argv[]) {
        const char *path = (argc > 1) ? argv[1] : ".";

        vector<filesystem::directory_entry> entries(
                filesystem::directory_iterator{path},
                filesystem::directory_iterator{});

        ranges::sort(entries, [](const filesystem::directory_entry &a,
                                 const filesystem::directory_entry &b) {
                return (a.path().filename().string().front() == '.') &&
                       !(b.path().filename().string().front() == '.');
        });

        for (auto const &dir_entry: entries) {
                if (dir_entry.is_directory()) {
                        cout << BLUE << dir_entry.path().string().substr(2) << RESET << "/" << endl;
                } else {
                        cout << dir_entry.path().string().substr(2) << endl;
                }
        }
        return 0;
}
