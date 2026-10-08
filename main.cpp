#include<iterator>
#include <filesystem>
#include <iostream>
using namespace std;

namespace {
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
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

        void printEntries(const vector<filesystem::directory_entry> &entries) {
                for (auto const &dir_entry: entries) {
                        if (dir_entry.is_directory()) {
                                cout << BLUE << dir_entry.path().string().substr(2) << RESET << "/" << endl;
                        } else {
                                if (dir_entry.is_regular_file()) {
                                        std::filesystem::perms p = dir_entry.status().permissions();

                                        // Combine the execution bits
                                        auto exe_bits =
                                                        std::filesystem::perms::owner_exec |
                                                        std::filesystem::perms::group_exec |
                                                        std::filesystem::perms::others_exec;

                                        if ((p & exe_bits) != std::filesystem::perms::none) {
                                                cout << RED << dir_entry.path().string().substr(2) << RESET << endl;
                                        } else {
                                                cout << dir_entry.path().string().substr(2) << endl;
                                        }
                                }
                        }
                }
        }
}

int main(const int argc, char *argv[]) {
        const char *path = ((argc > 1) && (argv[1][0] != '-')) ? argv[1] : "./";
        bool showAll = false;

        // FLAGS
        vector<char> flags;
        if (argc > 1) {
                for (int i = 1; i < argc; ++i) {
                        if (argv[i][0] == '-' && argv[i][1] == 'a') {
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
