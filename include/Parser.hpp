#pragma once

#include "Lexer.hpp"

struct ValidationRule_t {
  std::unordered_map<std::string, std::variant<std::string, int>> m_mapRules;
};

struct Parameter_t {
  std::string m_strName;
  std::variant<std::string, int, std::vector<std::string>> m_varValue;
  std::string m_strDescription;
  ValidationRule_t m_tValidation;
};

struct Section_t {
  std::string m_strName;
  std::vector<Parameter_t> m_vecParameters;
  std::vector<Section_t> m_vecSections;
};

class CParser {
public:
  explicit CParser(CLexer &lexer) : m_lexer(lexer) {
    m_tCurrentToken = m_lexer.nextToken();
  }

  Section_t parseSection() {
    expectToken(TOKEN_SECTION_e);
    std::string strSectionName = expectToken(TOKEN_IDENTIFIER_e).strValue;
    expectToken(TOKEN_LBRACE_e);

    Section_t tSection;
    tSection.m_strName = strSectionName;

    while (m_tCurrentToken.eType != TOKEN_RBRACE_e) {
      if (m_tCurrentToken.eType == TOKEN_PARAMETER_e) {
        tSection.m_vecParameters.push_back(parseParameter());
      } else if (m_tCurrentToken.eType == TOKEN_SECTION_e) {
        tSection.m_vecSections.push_back(parseSection());
      } else {
        // Skip unexpected tokens until a parameter or section token is found
        while (m_tCurrentToken.eType != TOKEN_PARAMETER_e &&
               m_tCurrentToken.eType != TOKEN_SECTION_e &&
               m_tCurrentToken.eType != TOKEN_RBRACE_e) {
          m_tCurrentToken = m_lexer.nextToken();
        }
        if (m_tCurrentToken.eType == TOKEN_PARAMETER_e ||
            m_tCurrentToken.eType == TOKEN_SECTION_e) {
          // Continue parsing parameters or sections
          continue;
        } else {
          throw std::runtime_error("Unexpected token: " +
                                   m_tCurrentToken.strValue);
        }
      }
    }
    expectToken(TOKEN_RBRACE_e);

    return tSection;
  }

  std::vector<Section_t> parse() {
    std::vector<Section_t> vecSections;
    while (m_tCurrentToken.eType != TOKEN_END_e) {
      vecSections.push_back(parseSection());
    }
    return vecSections;
  }

private:
  Token_t expectToken(TokenType_e eType) {
    if (m_tCurrentToken.eType != eType) {
      throw std::runtime_error("Unexpected token: " + m_tCurrentToken.strValue);
    }
    Token_t tToken = m_tCurrentToken;
    m_tCurrentToken = m_lexer.nextToken();
    return tToken;
  }

  Parameter_t parseParameter() {
    expectToken(TOKEN_PARAMETER_e);
    std::string strParamName = expectToken(TOKEN_IDENTIFIER_e).strValue;
    expectToken(TOKEN_LBRACE_e);

    Parameter_t tParameter;
    tParameter.m_strName = strParamName;

    while (m_tCurrentToken.eType != TOKEN_RBRACE_e) {
      std::string strAttribute = expectToken(TOKEN_IDENTIFIER_e).strValue;
      expectToken(TOKEN_EQUAL_e);

      if (strAttribute == "value") {
        tParameter.m_varValue = parseValue();

      } else if (strAttribute == "description") {
        tParameter.m_strDescription =
            expectToken(TOKEN_STRING_LITERAL_e).strValue;
      } /*else if (strAttribute == "validation")
      {
        tParameter.m_tValidation = expectToken(TOKEN_STRING_LITERAL_e).strValue;
      }*/
      else {
        ValidationRule_t tValidation;
        if (m_tCurrentToken.eType == TOKEN_STRING_LITERAL_e) {
          tValidation.m_mapRules[strAttribute] =
              expectToken(TOKEN_STRING_LITERAL_e).strValue;
        } else if (m_tCurrentToken.eType == TOKEN_INTEGER_LITERAL_e) {
          tValidation.m_mapRules[strAttribute] = std::to_string(
              std::stoi(expectToken(TOKEN_INTEGER_LITERAL_e).strValue));
        } else {
          throw std::runtime_error("Unexpected token in validation rule");
        }
        tParameter.m_tValidation = tValidation;
      }
    }
    expectToken(TOKEN_RBRACE_e);

    return tParameter;
  }

  std::variant<std::string, int, std::vector<std::string>> parseValue() {
    if (m_tCurrentToken.eType == TOKEN_STRING_LITERAL_e) {
      return expectToken(TOKEN_STRING_LITERAL_e).strValue;
    } else if (m_tCurrentToken.eType == TOKEN_INTEGER_LITERAL_e) {
      return std::stoi(expectToken(TOKEN_INTEGER_LITERAL_e).strValue);
    } else if (m_tCurrentToken.eType == TOKEN_LBRACKET_e) {
      std::vector<std::string> vecArrayValues;
      expectToken(TOKEN_LBRACKET_e);
      while (m_tCurrentToken.eType != TOKEN_RBRACKET_e) {
        vecArrayValues.push_back(expectToken(TOKEN_STRING_LITERAL_e).strValue);
        if (m_tCurrentToken.eType == TOKEN_COMMA_e) {
          expectToken(TOKEN_COMMA_e);
        }
      }
      expectToken(TOKEN_RBRACKET_e);
      return vecArrayValues;
    } else {
      throw std::runtime_error("Unexpected token in value");
    }
  }

  CLexer &m_lexer;
  Token_t m_tCurrentToken;
};
