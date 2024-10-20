#pragma once

#include "Lexer.hpp"

struct Parameter_t
{
    std::string m_szName;
    std::variant<std::string, int, std::vector<std::string>> m_varValue;
    std::string m_szDescription;
};

struct Section_t
{
    std::string m_szName;
    std::vector<Parameter_t> m_vecParameters;
    std::vector<Section_t> m_vecSections;
};

class CParser
{
public:
    explicit CParser(CLexer& lexer) : m_lexer(lexer)
    {
        m_CurrentToken = m_lexer.NextToken();
    }

    Section_t ParseSection()
    {
        ExpectToken(TOKEN_SECTION_e);
        std::string szSectionName = ExpectToken(TOKEN_IDENTIFIER_e).m_szValue;
        ExpectToken(TOKEN_LBRACE_e);

        Section_t tSection;
        tSection.m_szName = szSectionName;

        while (m_CurrentToken.m_eType != TOKEN_RBRACE_e)
        {
            if (m_CurrentToken.m_eType == TOKEN_PARAMETER_e)
            {
                tSection.m_vecParameters.push_back(ParseParameter());
            }
            else if (m_CurrentToken.m_eType == TOKEN_SECTION_e)
            {
                tSection.m_vecSections.push_back(ParseSection());
            }
            else
            {
                // Skip unexpected tokens until a parameter or section token is found
                while (m_CurrentToken.m_eType != TOKEN_PARAMETER_e && m_CurrentToken.m_eType != TOKEN_SECTION_e &&
                       m_CurrentToken.m_eType != TOKEN_RBRACE_e)
                {
                    m_CurrentToken = m_lexer.NextToken();
                }
                if (m_CurrentToken.m_eType == TOKEN_PARAMETER_e || m_CurrentToken.m_eType == TOKEN_SECTION_e)
                {
                    // Continue parsing parameters or sections
                    continue;
                }
                else
                {
                    throw std::runtime_error("Unexpected token: " + m_CurrentToken.m_szValue);
                }
            }
        }
        ExpectToken(TOKEN_RBRACE_e);

        return tSection;
    }

    std::vector<Section_t> Parse()
    {
        std::vector<Section_t> vecSections;
        while (m_CurrentToken.m_eType != TOKEN_END_e)
        {
            vecSections.push_back(ParseSection());
        }
        return vecSections;
    }

private:
    Token_t ExpectToken(TokenType_e eType)
    {
        if (m_CurrentToken.m_eType != eType)
        {
            throw std::runtime_error("Unexpected token: " + m_CurrentToken.m_szValue);
        }
        Token_t tToken = m_CurrentToken;
        m_CurrentToken = m_lexer.NextToken();
        return tToken;
    }

    Parameter_t ParseParameter()
    {
        ExpectToken(TOKEN_PARAMETER_e);
        std::string szParamName = ExpectToken(TOKEN_IDENTIFIER_e).m_szValue;
        ExpectToken(TOKEN_LBRACE_e);

        Parameter_t Parameter;
        Parameter.m_szName = szParamName;

        while (m_CurrentToken.m_eType != TOKEN_RBRACE_e)
        {
            std::string szAttribute = ExpectToken(TOKEN_IDENTIFIER_e).m_szValue;
            ExpectToken(TOKEN_EQUAL_e);

            if (szAttribute == "value")
                Parameter.m_varValue = ParseValue();
            else if (szAttribute == "description")
                Parameter.m_szDescription = ExpectToken(TOKEN_STRING_LITERAL_e).m_szValue;
        }

        ExpectToken(TOKEN_RBRACE_e);
        return Parameter;
    }

    std::variant<std::string, int, std::vector<std::string>> ParseValue()
    {
        if (m_CurrentToken.m_eType == TOKEN_STRING_LITERAL_e)
            return ExpectToken(TOKEN_STRING_LITERAL_e).m_szValue;
        else if (m_CurrentToken.m_eType == TOKEN_INTEGER_LITERAL_e)
            return std::stoi(ExpectToken(TOKEN_INTEGER_LITERAL_e).m_szValue);
        else if (m_CurrentToken.m_eType == TOKEN_LBRACKET_e)
        {
            std::vector<std::string> vecArrayValues;
            ExpectToken(TOKEN_LBRACKET_e);
            while (m_CurrentToken.m_eType != TOKEN_RBRACKET_e)
            {
                vecArrayValues.push_back(ExpectToken(TOKEN_STRING_LITERAL_e).m_szValue);
                if (m_CurrentToken.m_eType == TOKEN_COMMA_e)
                    ExpectToken(TOKEN_COMMA_e);
            }

            ExpectToken(TOKEN_RBRACKET_e);
            return vecArrayValues;
        }
        else
            throw std::runtime_error("Unexpected token in value");
    }

    CLexer& m_lexer;
    Token_t m_CurrentToken;
};
