#include "../include/Parser.hpp"

#include <fstream>
#include <sstream>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string strInput = buffer.str();

    CLexer lexer(strInput);
    CParser parser(lexer);

    try
    {
        auto vecSections = parser.Parse();

        // Print parsed configuration
        for (const auto& section : vecSections)
        {
            std::cout << "Section: " << section.m_szName << std::endl;
            for (const auto& parameter : section.m_vecParameters)
            {
                std::cout << "  Parameter: " << parameter.m_szName << std::endl;
                std::cout << "    Value: ";
                if (std::holds_alternative<std::string>(parameter.m_varValue))
                {
                    std::cout << std::get<std::string>(parameter.m_varValue) << std::endl;
                }
                else if (std::holds_alternative<int>(parameter.m_varValue))
                {
                    std::cout << std::get<int>(parameter.m_varValue) << std::endl;
                }
                else if (std::holds_alternative<std::vector<std::string>>(parameter.m_varValue))
                {
                    for (const auto& val : std::get<std::vector<std::string>>(parameter.m_varValue))
                    {
                        std::cout << val << " ";
                    }
                    std::cout << std::endl;
                }

                std::cout << "    Description: " << parameter.m_szDescription << std::endl;
            }
            for (const auto& subSection : section.m_vecSections)
            {
                std::cout << "  Sub-section: " << subSection.m_szName << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
