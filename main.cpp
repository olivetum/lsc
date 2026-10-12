#include<iterator>
#include <filesystem>
#include <iostream>
using namespace std;

namespace {
#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLUE    "\033[34m"      /* Blue */

        string filename(filesystem::directory_entry const &dir_entry) {
                return dir_entry.path().filename().string();
        }

        void printEntries(const vector<filesystem::directory_entry> &entries) {
                for (auto const &dir_entry: entries) {
                        if (dir_entry.is_directory()) {
                                cout << BLUE << dir_entry.path().filename().string() << RESET << "/" << endl;
                        } else {
                                if (dir_entry.is_regular_file()) {
                                        std::filesystem::perms p = dir_entry.status().permissions();

                                        // Combine the execution bits
                                        auto exe_bits =
                                                        std::filesystem::perms::owner_exec |
                                                        std::filesystem::perms::group_exec |
                                                        std::filesystem::perms::others_exec;

                                        if ((p & exe_bits) != std::filesystem::perms::none) {
                                                cout << RED << dir_entry.path().filename().string() << RESET << endl;
                                        } else {
                                                cout << filename(dir_entry) << endl;
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
                        if (std::strcmp(argv[i], "-a") == 0) {
                                showAll = true;
                        }
                }
        }

        vector<filesystem::directory_entry> entries(
                filesystem::directory_iterator{path},
                filesystem::directory_iterator{});

        ranges::sort(entries, {}, [](const filesystem::directory_entry &entry) {
                string name = filename(entry);
                return name;
        });
        // EXTRACTING DOT ENTRIES TO SEPARATE VECTOR
        vector<filesystem::directory_entry> dotEntries;
        std::ranges::copy_if(
                entries,
                back_inserter(dotEntries),
                [](const auto &entry) {
                        return filename(entry).starts_with('.');
                });

        entries.erase(
                std::ranges::remove_if(entries,
                                       [](const filesystem::directory_entry &entry) {
                                               return filename(entry).starts_with('.');
                                       }).begin(),
                entries.end()
        );

        if (showAll)
                printEntries(dotEntries);
        printEntries(entries);
        return 0;
}
