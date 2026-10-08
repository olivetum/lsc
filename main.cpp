
#include<iterator>
#include <filesystem>
#include <iostream>
#include <cctype>
using namespace std;

#define RESET   "\033[0m"
#define GREEN   "\033[32m"      /* Green */
#define BLUE    "\033[34m"      /* Blue */

int main(const int argc, char *argv[]) {
        const char *path = (argc > 1) ? argv[1] : ".";
        bool isFlagged = false;
        isFlagged = argc > 2;

        vector<filesystem::directory_entry> entries(
                filesystem::directory_iterator{path},
                filesystem::directory_iterator{});

        vector<filesystem::directory_entry> dotEntries;
        ranges::copy_if(
                entries,
                back_inserter(dotEntries),
                [](const auto &entry) {
                        return entry.path().filename().string().starts_with('.');
                });

        entries.erase(
                remove_if(entries.begin(), entries.end(),
                          [](const filesystem::directory_entry& entry) {
                                  return entry.path().filename().string().starts_with('.');
                          }),
                entries.end()
        );

        ranges::sort(entries, {}, [](const filesystem::directory_entry &entry) {
                string name = entry.path().filename().string();

                for (char &c: name) {
                        c = static_cast<char>(
                                tolower(static_cast<unsigned char>(c))
                        );
                }
                return name;
        });

        ranges::sort(dotEntries, {}, [](const filesystem::directory_entry& entry) {
                string name = entry.path().filename().string();
                for (char &c: name) {
                        c = static_cast<char>(
                                tolower(static_cast<unsigned char>(c))
                        );
                }
                name.erase(0, 1);
                return name;
        });


        for (auto const &dir_entry: dotEntries) {
                if (dir_entry.is_directory()) {
                        cout << BLUE << dir_entry.path().string().substr(2) << RESET << "/" << endl;
                } else {
                        cout << dir_entry.path().string().substr(2) << endl;
                }
        }
        return 0;
}