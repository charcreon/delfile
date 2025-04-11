#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_pattern1> [file_pattern2 ...]" << std::endl;
        return 1;
    }

    std::vector<std::string> file_patterns;
    for (int i = 1; i < argc; ++i)
    {
        file_patterns.push_back(argv[i]);
    }

    bool success = true;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator("."))
        {
            if (std::filesystem::is_regular_file(entry))
            {
                const std::string filename = entry.path().filename().string();
                for (const auto& file_pattern : file_patterns)
                {
                    if (std::filesystem::path(filename).string().find(file_pattern) != std::string::npos)
                    {
                        if (!std::filesystem::remove(entry.path()))
                        {
                            success = false;
                        }
                        break;
                    }
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 2;
    }

    return success ? 0 : 3;
}
