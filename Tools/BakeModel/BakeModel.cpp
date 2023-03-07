#include "BakeModel.h"

#include "cxxopts.hpp"

int main(int argc, char** argv)
{
    cxxopts::Options options("BakeModel", "This is a tool designed for DQ");
    options.add_options()
        ("o,output", "Output path", cxxopts::value<std::string>())
        ("f,file", "File path", cxxopts::value<std::string>())
        ("h,help", "Show help about this program");

    try 
    {
        auto result = options.parse(argc, argv);
        if (result.count("help"))
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("file"))
        {
            if (result.count("output"))
            {
                if (!std::filesystem::exists(result["file"].as<std::string>()))
                {
                    std::cerr << "Error: File path does not exist" << std::endl;
                    return 1;
                }

                if (std::string reason; !PreCheckModel(result["file"].as<std::string>(), reason))
                {
                    std::cerr << "Error: " << reason << std::endl;
                    return 1;
                }

                if (std::ofstream ofs(result["output"].as<std::string>(), std::ios::out | std::ios::binary); !ofs.is_open())
                {
                    std::cerr << "Error: Can not create output file" << std::endl;
                    return 1;
                }

                auto modelData = GetModelData(result["file"].as<std::string>());
                auto jsonStr = GetJSONString(result["output"].as<std::string>(), modelData);
                std::ofstream ofs(result["output"].as<std::string>(), std::ios::out | std::ios::binary);
                ofs << jsonStr;
            }
            else
            {
                std::cerr << "Error: Need an output path" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cerr << "Error: Need a file path" << std::endl;
            return 1;
        }

    }
    catch (const cxxopts::exceptions::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}