#include "parser.h"
#include <iostream>

using namespace std;

tuple<wstring, LocationRange> Parser::Tokenizer::parseWord()
{
    LocationRange location = curLocation();
    wstring retval;
    if(!isValidIdentifierStartCharacter(curChar()))
        expected({L"word"}, curLocation());
    retval += curChar();
    while(isValidIdentifierCharacter(nextChar()))
    {
        location += curLocation();
        retval += curChar();
    }
    return make_tuple(retval, location);
}

LocationRange Parser::Tokenizer::parseComment()
{
    LocationRange location = curLocation();
    if(curChar() != L'\'')
        expected({L"'"}, location);
    location += nextLocation();
    size_t line = curLocation().start.line;
    while(curChar() != WEOF && curLocation().end.line == line)
    {
        location += nextLocation();
    }
    return location;
}

LocationRange Parser::Tokenizer::parseWhitespace()
{
    LocationRange location = curLocation().start;
    while(curChar() == L'\'' || iswblank(curChar()) || curChar() == L'_')
    {
        if(curChar() == L'\'')
        {
            location += parseComment();
        }
        else if(curChar() == L'_')
        {
            LocationRange underlineLocation = curLocation();
            LocationRange oldLocation = location;
            nextChar();
            while(curChar() == L'\'' || iswblank(curChar()))
            {
                if(curChar() == L'\'')
                {
                    location += parseComment();
                }
                else
                    location += nextLocation();
            }
            if(!iswspace(curChar()))
            {
                putback(L'_', underlineLocation);
                location = oldLocation;
                break;
            }
            else
            {
                location += nextLocation();
                size_t line = curLocation().start.line;
                while(iswspace(curChar()) && curLocation().end.line == line)
                {
                    nextChar();
                }
            }
        }
        else
            location += nextLocation();
    }
    return location;
}

LocationRange Parser::Tokenizer::parseNewLine()
{
    LocationRange location = parseWhitespace();
    if(!iswspace(curChar()) && curChar() != WEOF)
        expected({L"end of line"}, curLocation());
    while(iswspace(curChar()))
    {
        location += nextLocation();
        location += parseWhitespace();
    }
    return location;
}

static bool iswodigit(wint_t ch)
{
    return ch >= '0' && ch < '8';
}

void Parser::Tokenizer::parseNumber()
{
    token = TokenType::IntegerValue;
    tokenLocation = curLocation();
    if(curChar() == L'&')
    {
        tokenValue += curChar();
        if(iswodigit(nextChar()))
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            while(iswodigit(nextChar()))
            {
                tokenValue += curChar();
                tokenLocation += curLocation();
            }
            return;
        }
        if(curChar() == L'H' || curChar() == L'h')
        {
            tokenValue += L'H';
            tokenLocation += curLocation();
            while(iswxdigit(nextChar()))
            {
                tokenValue += towupper(curChar());
                tokenLocation += curLocation();
            }
            return;
        }
        token = TokenType::Ampersand;
        return;
    }
    while(iswdigit(curChar()))
    {
        tokenValue += curChar();
        tokenLocation += curLocation();
        nextChar();
    }
    if(curChar() == L'.')
    {
        tokenValue += curChar();
        LocationRange periodLocation = curLocation();
        if(!iswdigit(nextChar()))
        {
            if(tokenValue == L".")
            {
                token = TokenType::Period;
                tokenLocation = periodLocation;
                return;
            }
            else
            {
                tokenValue.erase(tokenValue.size() - 1);
                putback(L'.', periodLocation);
                return;
            }
        }
        tokenLocation += periodLocation;
        token = TokenType::FloatValue;
        while(iswdigit(curChar()))
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            nextChar();
        }
    }
    if(curChar() == L'e' || curChar() == L'E')
    {
        token = TokenType::FloatValue;
        tokenValue += L'e';
        tokenLocation += curLocation();
        nextChar();
        bool gotSign = false;
        if(curChar() == L'+' || curChar() == L'-')
        {
            if(curChar() == L'-')
                tokenValue += curChar();
            tokenLocation += curLocation();
            nextChar();
            gotSign = true;
        }
        if(!iswdigit(curChar()))
            expected((gotSign ? initializer_list<const wchar_t *>{L"digit"} : initializer_list<const wchar_t *>{L"digit", L"+", L"-"}), curLocation());
        while(iswdigit(curChar()))
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            nextChar();
        }
    }
}

unordered_map<wstring, Parser::TokenType> Parser::Tokenizer::makeKeywordMap()
{
    unordered_map<wstring, TokenType> retval;
    retval[L"End"] = TokenType::End;
    retval[L"Exit"] = TokenType::Exit;
    retval[L"EndIf"] = TokenType::EndIf;
    retval[L"If"] = TokenType::If;
    retval[L"End If"] = TokenType::EndIf;
    return retval;
}

void Parser::Tokenizer::checkForKeyword()
{
    thread_local unordered_map<wstring, TokenType> keywordMap = makeKeywordMap();
    auto iter = keywordMap.find(tokenValue);
    if(iter == keywordMap.end())
        return;
    token = get<1>(*iter);
    switch(token)
    {
    case TokenType::End:
    case TokenType::Exit:
        break;
    default:
        return;
    }
    parseWhitespace();
    if(!isValidIdentifierStartCharacter(curChar()))
        return;
    auto v = parseWord();
    wstring newTokenValue = tokenValue + L" " + get<0>(v);
    iter = keywordMap.find(newTokenValue);
    if(iter == keywordMap.end())
    {
        TokenType tempToken = token;
        wstring tempTokenValue = tokenValue;
        LocationRange tempTokenLocation = tokenLocation;
        token = TokenType::Identifier;
        tokenValue = get<0>(v);
        tokenLocation = get<1>(v);
        putback(tempToken, tempTokenValue, tempTokenLocation);
        return;
    }
    tokenLocation += get<1>(v);
    tokenValue = newTokenValue;
    token = get<1>(*iter);
}

void Parser::Tokenizer::nextToken()
{
    if(putbackToken != TokenType::Eof)
    {
        token = putbackToken;
        putbackToken = TokenType::Eof;
        tokenLocation = putbackTokenLocation;
        tokenValue = putbackTokenValue;
        if(token == TokenType::Identifier)
            checkForKeyword();
        return;
    }
    if(token == TokenType::Eof)
        return;
    parseWhitespace();
    token = TokenType::Eof;
    tokenLocation = curLocation();
    tokenValue = L"";
    if(curChar() == WEOF)
    {
        token = TokenType::Eof;
        return;
    }
    if(iswspace(curChar()))
    {
        tokenValue = L"\n";
        token = TokenType::NewLine;
        tokenLocation = parseNewLine();
        return;
    }
    if(isValidIdentifierStartCharacter(curChar()))
    {
        auto v = parseWord();
        token = TokenType::Identifier;
        tokenValue = get<0>(v);
        tokenLocation = get<1>(v);
        checkForKeyword();
        return;
    }
    if(iswdigit(curChar()) || curChar() == L'.' || curChar() == L'&')
    {
        parseNumber();
        return;
    }
    error(L"invalid character", curLocation());
}

void Parser::run()
{
    try
    {
        while(curTokenType() != TokenType::Eof)
        {
            if(curTokenType() != TokenType::NewLine)
                wcout << curTokenValue() << endl;
            else
                wcout << L"<New Line>" << endl;
            nextTokenType();
        }
    }
    catch(Exception & e)
    {
        wcout << L"\nError : " << e.what() << endl;
    }
}
