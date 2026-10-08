#include<iterator>
#include <filesystem>
#include <iostream>
using namespace std;

namespace {
#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */

        std::string toLowerCase(std::string name, bool erase = false) {
                if (erase) {
                        name.erase(0, 1);
                }
                for (char &c: name) {
                        c = static_cast<char>(
                                tolower(static_cast<unsigned char>(c))
                        );
                }
                return name;
        }

        void printEntries(const vector<filesystem::directory_entry>& entries) {
                for (auto const &dir_entry: entries) {
                        if (dir_entry.is_directory()) {
                                cout << BLUE << dir_entry.path().string().substr(2) << RESET << "/" << endl;
                        } else {
                                cout << dir_entry.path().string().substr(2) << endl;
                        }
                }
        }
}

int main(const int argc, char *argv[]) {
        const char *path = (argc > 1) ? argv[1] : ".";
        bool showAll = false;

        // FLAGS
        vector<char> flags;
        if (argc > 2) {
                for (int i = 2; i < argc; ++i) {
                        if (argv[i][1] == 'a') {
                                showAll = true;
                        }
                }
        }

        vector<filesystem::directory_entry> entries(
                filesystem::directory_iterator{path},
                filesystem::directory_iterator{});

        // EXTRACTING DOT ENTRIES TO SEPARATE VECTOR
        vector<filesystem::directory_entry> dotEntries;
        std::ranges::copy_if(
                entries,
                back_inserter(dotEntries),
                [](const auto &entry) {
                        return entry.path().filename().string().starts_with('.');
                });

        entries.erase(
                std::ranges::remove_if(entries,
                                  [](const filesystem::directory_entry &entry) {
                                          return entry.path().filename().string().starts_with('.');
                                  }).begin(),
                entries.end()
        );

        ranges::sort(entries, {}, [](const filesystem::directory_entry &entry) {
                string name = entry.path().filename().string();
                return toLowerCase(name);
        });

        ranges::sort(dotEntries, {}, [](const filesystem::directory_entry &entry) {
                string name = entry.path().filename().string();
                return toLowerCase(name, true);
        });


        if (showAll)
                printEntries(dotEntries);
        printEntries(entries);
        return 0;
}
