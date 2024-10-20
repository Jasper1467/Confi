#pragma once

#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

enum TokenType_e
{
    TOKEN_SECTION_e,
    TOKEN_PARAMETER_e,
    TOKEN_AS_e,
    TOKEN_STRING_e,
    TOKEN_INTEGER_e,
    TOKEN_ARRAY_e,
    TOKEN_ENUM_e,
    TOKEN_LBRACE_e,
    TOKEN_RBRACE_e,
    TOKEN_EQUAL_e,
    TOKEN_COMMA_e,
    TOKEN_LBRACKET_e,
    TOKEN_RBRACKET_e,
    TOKEN_IDENTIFIER_e,
    TOKEN_STRING_LITERAL_e,
    TOKEN_INTEGER_LITERAL_e,
    TOKEN_UNKNOWN_e,
    TOKEN_END_e
};

struct Token_t
{
    TokenType_e m_eType;
    std::string m_szValue;
};

class CLexer
{
public:
    explicit CLexer(const std::string& input) : m_strInput(input), m_nPos(0)
    {
    }

    Token_t NextToken()
    {
        while (m_nPos < m_strInput.size() && std::isspace(m_strInput[m_nPos]))
        {
            m_nPos++;
        }
        if (m_nPos >= m_strInput.size())
        {
            return { TOKEN_END_e, "" };
        }

        if (std::isalpha(m_strInput[m_nPos]))
        {
            std::string strIdentifier;
            while (m_nPos < m_strInput.size() && std::isalnum(m_strInput[m_nPos]))
            {
                strIdentifier += m_strInput[m_nPos++];
            }
            if (strIdentifier == "section")
                return { TOKEN_SECTION_e, strIdentifier };
            if (strIdentifier == "parameter")
                return { TOKEN_PARAMETER_e, strIdentifier };
            if (strIdentifier == "as")
                return { TOKEN_AS_e, strIdentifier };
            if (strIdentifier == "string")
                return { TOKEN_STRING_e, strIdentifier };
            if (strIdentifier == "integer")
                return { TOKEN_INTEGER_e, strIdentifier };
            if (strIdentifier == "array")
                return { TOKEN_ARRAY_e, strIdentifier };
            if (strIdentifier == "enum")
                return { TOKEN_ENUM_e, strIdentifier };
            return { TOKEN_IDENTIFIER_e, strIdentifier };
        }

        if (m_strInput[m_nPos] == '"')
        {
            m_nPos++;
            std::string strLiteral;
            while (m_nPos < m_strInput.size() && m_strInput[m_nPos] != '"')
            {
                strLiteral += m_strInput[m_nPos++];
            }
            m_nPos++; // Move past the closing quote
            return { TOKEN_STRING_LITERAL_e, strLiteral };
        }

        if (std::isdigit(m_strInput[m_nPos]))
        {
            std::string strNumLiteral;
            while (m_nPos < m_strInput.size() && std::isdigit(m_strInput[m_nPos]))
            {
                strNumLiteral += m_strInput[m_nPos++];
            }
            return { TOKEN_INTEGER_LITERAL_e, strNumLiteral };
        }

        switch (m_strInput[m_nPos])
        {
        case '{':
            m_nPos++;
            return { TOKEN_LBRACE_e, "{" };
        case '}':
            m_nPos++;
            return { TOKEN_RBRACE_e, "}" };
        case '=':
            m_nPos++;
            return { TOKEN_EQUAL_e, "=" };
        case ',':
            m_nPos++;
            return { TOKEN_COMMA_e, "," };
        case '[':
            m_nPos++;
            return { TOKEN_LBRACKET_e, "[" };
        case ']':
            m_nPos++;
            return { TOKEN_RBRACKET_e, "]" };
        default:
            return { TOKEN_UNKNOWN_e, std::string(1, m_strInput[m_nPos++]) };
        }
    }

private:
    std::string m_strInput;
    size_t m_nPos;
};
