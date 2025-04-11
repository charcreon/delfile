#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <vector>
#include <string>

void print_help(const std::string& program_name)
{
    std::cout << "Usage: " << program_name << " [-h] <file_pattern1> [file_pattern2 ...]\n"
        << "Options:\n"
        << "  -h                Show this help message and exit.\n"
        << "  <file_pattern>    Specify file patterns to delete files matching the patterns.\n"
        << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " [-h] <file_pattern1> [file_pattern2 ...]" << std::endl;
        return 1;
    }

    // ヘルプオプションの処理
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-h")
        {
            print_help(argv[0]);
            return 0;
        }
    }

    // コマンドライン引数からファイルパターンを収集
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

                // 全てのファイルパターンに対して一致を確認
                for (const auto& file_pattern : file_patterns)
                {
                    if (std::filesystem::path(filename).string().find(file_pattern) != std::string::npos)
                    {
                        if (!std::filesystem::remove(entry.path()))
                        {
                            success = false;
                        }

                        // 一致した場合、次のファイルパターンのチェックをスキップ
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
