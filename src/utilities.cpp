#include "../headers/utilities.hpp"

vector<path> getFilesInProject(string const& pathName)
{
    vector<path> files;
    filesystem::path p{pathName};
    if (is_regular_file(p)) {
        files.emplace_back(p);
        return files;
    }

    for (auto const& file : make_iterator_range(recursive_directory_iterator{p}, {})) {
        regex jackFile{R"(.*\.jack)"};
        if (is_regular_file(file) && regex_search(file.path().string().begin(), file.path().string().end(), jackFile)) {
            files.emplace_back(file);
        }
    }

    return files;
}
