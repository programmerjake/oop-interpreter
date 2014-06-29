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
        if(curChar() == L'=')
        {
            token = TokenType::AmpersandEqual;
            tokenValue += curChar();
            tokenLocation += curLocation();
            nextChar();
        }
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

void Parser::Tokenizer::parseString()
{
    if(curChar() != L'\"')
        expected({L"\""}, curLocation());
    token = TokenType::StringValue;
    tokenLocation = curLocation();
    tokenLocation += nextLocation();
    for(;;)
    {
        if(curChar() == WEOF || (iswspace(curChar()) && !iswblank(curChar())))
            expected({L"\""}, curLocation());
        if(curChar() == L'\"')
        {
            tokenLocation += curLocation();
            if(nextChar() == L'\"')
            {
                tokenValue += curChar();
                tokenLocation += curLocation();
            }
            else
                return;
        }
        else
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
    retval[L"AddressOf"] = TokenType::AddressOf;
    retval[L"And"] = TokenType::And;
    retval[L"AndAlso"] = TokenType::AndAlso;
    retval[L"As"] = TokenType::As;
    retval[L"Boolean"] = TokenType::Boolean;
    retval[L"ByRef"] = TokenType::ByRef;
    retval[L"Byte"] = TokenType::Byte;
    retval[L"ByVal"] = TokenType::ByVal;
    retval[L"Case"] = TokenType::Case;
    retval[L"Catch"] = TokenType::Catch;
    retval[L"CBool"] = TokenType::CBool;
    retval[L"CByte"] = TokenType::CByte;
    retval[L"CChar"] = TokenType::CChar;
    retval[L"CDbl"] = TokenType::CDbl;
    retval[L"Char"] = TokenType::Char;
    retval[L"CInt"] = TokenType::CInt;
    retval[L"Class"] = TokenType::Class;
    retval[L"CLng"] = TokenType::CLng;
    retval[L"Const"] = TokenType::Const;
    retval[L"Continue"] = TokenType::Continue;
    retval[L"ContinueDo"] = TokenType::ContinueDo;
    retval[L"Continue Do"] = TokenType::ContinueDo;
    retval[L"ContinueFor"] = TokenType::ContinueFor;
    retval[L"Continue For"] = TokenType::ContinueFor;
    retval[L"CSByte"] = TokenType::CSByte;
    retval[L"CShort"] = TokenType::CShort;
    retval[L"CSng"] = TokenType::CSng;
    retval[L"CStr"] = TokenType::CStr;
    retval[L"CType"] = TokenType::CType;
    retval[L"CUInt"] = TokenType::CUInt;
    retval[L"CULng"] = TokenType::CULng;
    retval[L"CUShort"] = TokenType::CUShort;
    retval[L"Declare"] = TokenType::Declare;
    retval[L"Delete"] = TokenType::Delete;
    retval[L"Dim"] = TokenType::Dim;
    retval[L"Do"] = TokenType::Do;
    retval[L"Double"] = TokenType::Double;
    retval[L"Each"] = TokenType::Each;
    retval[L"Else"] = TokenType::Else;
    retval[L"ElseIf"] = TokenType::ElseIf;
    retval[L"End"] = TokenType::End;
    retval[L"EndClass"] = TokenType::EndClass;
    retval[L"End Class"] = TokenType::EndClass;
    retval[L"EndEnum"] = TokenType::EndEnum;
    retval[L"End Enum"] = TokenType::EndEnum;
    retval[L"EndFunction"] = TokenType::EndFunction;
    retval[L"End Function"] = TokenType::EndFunction;
    retval[L"EndIf"] = TokenType::EndIf;
    retval[L"End If"] = TokenType::EndIf;
    retval[L"EndInterface"] = TokenType::EndInterface;
    retval[L"End Interface"] = TokenType::EndInterface;
    retval[L"EndNamespace"] = TokenType::EndNamespace;
    retval[L"End Namespace"] = TokenType::EndNamespace;
    retval[L"EndOperator"] = TokenType::EndOperator;
    retval[L"End Operator"] = TokenType::EndOperator;
    retval[L"EndSelect"] = TokenType::EndSelect;
    retval[L"End Select"] = TokenType::EndSelect;
    retval[L"EndStructure"] = TokenType::EndStructure;
    retval[L"End Structure"] = TokenType::EndStructure;
    retval[L"EndSub"] = TokenType::EndSub;
    retval[L"End Sub"] = TokenType::EndSub;
    retval[L"EndTry"] = TokenType::EndTry;
    retval[L"End Try"] = TokenType::EndTry;
    retval[L"Enum"] = TokenType::Enum;
    retval[L"Exit"] = TokenType::Exit;
    retval[L"ExitDo"] = TokenType::ExitDo;
    retval[L"Exit Do"] = TokenType::ExitDo;
    retval[L"ExitFor"] = TokenType::ExitFor;
    retval[L"Exit For"] = TokenType::ExitFor;
    retval[L"ExitFunction"] = TokenType::ExitFunction;
    retval[L"Exit Function"] = TokenType::ExitFunction;
    retval[L"ExitSelect"] = TokenType::ExitSelect;
    retval[L"Exit Select"] = TokenType::ExitSelect;
    retval[L"ExitSub"] = TokenType::ExitSub;
    retval[L"Exit Sub"] = TokenType::ExitSub;
    retval[L"False"] = TokenType::False;
    retval[L"Finally"] = TokenType::Finally;
    retval[L"For"] = TokenType::For;
    retval[L"Friend"] = TokenType::Friend;
    retval[L"Function"] = TokenType::Function;
    retval[L"Global"] = TokenType::Global;
    retval[L"If"] = TokenType::If;
    retval[L"Implements"] = TokenType::Implements;
    retval[L"Imports"] = TokenType::Imports;
    retval[L"In"] = TokenType::In;
    retval[L"Inherits"] = TokenType::Inherits;
    retval[L"Integer"] = TokenType::Integer;
    retval[L"Interface"] = TokenType::Interface;
    retval[L"Long"] = TokenType::Long;
    retval[L"Loop"] = TokenType::Loop;
    retval[L"Me"] = TokenType::Me;
    retval[L"Mod"] = TokenType::Mod;
    retval[L"MyBase"] = TokenType::MyBase;
    retval[L"MyClass"] = TokenType::MyClass;
    retval[L"Namespace"] = TokenType::Namespace;
    retval[L"Narrowing"] = TokenType::Narrowing;
    retval[L"New"] = TokenType::New;
    retval[L"Next"] = TokenType::Next;
    retval[L"Not"] = TokenType::Not;
    retval[L"NotInheritable"] = TokenType::NotInheritable;
    retval[L"NotOverridable"] = TokenType::NotOverridable;
    retval[L"Object"] = TokenType::Object;
    retval[L"Operator"] = TokenType::Operator;
    retval[L"Optional"] = TokenType::Optional;
    retval[L"Or"] = TokenType::Or;
    retval[L"OrElse"] = TokenType::OrElse;
    retval[L"Overloads"] = TokenType::Overloads;
    retval[L"Overridable"] = TokenType::Overridable;
    retval[L"Overrides"] = TokenType::Overrides;
    retval[L"ParamArray"] = TokenType::ParamArray;
    retval[L"Pointer"] = TokenType::Pointer;
    retval[L"Private"] = TokenType::Private;
    retval[L"Protected"] = TokenType::Protected;
    retval[L"Public"] = TokenType::Public;
    retval[L"Return"] = TokenType::Return;
    retval[L"SByte"] = TokenType::SByte;
    retval[L"Select"] = TokenType::Select;
    retval[L"Shared"] = TokenType::Shared;
    retval[L"Short"] = TokenType::Short;
    retval[L"Single"] = TokenType::Single;
    retval[L"Static"] = TokenType::Static;
    retval[L"Step"] = TokenType::Step;
    retval[L"String"] = TokenType::String;
    retval[L"Structure"] = TokenType::Structure;
    retval[L"Sub"] = TokenType::Sub;
    retval[L"Then"] = TokenType::Then;
    retval[L"Throw"] = TokenType::Throw;
    retval[L"To"] = TokenType::To;
    retval[L"True"] = TokenType::True;
    retval[L"Try"] = TokenType::Try;
    retval[L"TypeOf"] = TokenType::TypeOf;
    retval[L"UInteger"] = TokenType::UInteger;
    retval[L"ULong"] = TokenType::ULong;
    retval[L"UShort"] = TokenType::UShort;
    retval[L"Using"] = TokenType::Using;
    retval[L"While"] = TokenType::While;
    retval[L"Widening"] = TokenType::Widening;
    retval[L"Xor"] = TokenType::Xor;
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
    case TokenType::Continue:
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
    TokenType lastToken = token;
    if(token == TokenType::Eof)
        return;
    parseWhitespace();
    token = TokenType::Eof;
    tokenLocation = curLocation();
    tokenValue = L"";
    if(curChar() == WEOF)
    {
        if(lastToken == TokenType::LineEnd)
            token = TokenType::Eof;
        else
            token = TokenType::LineEnd;
        return;
    }
    if(iswspace(curChar()))
    {
        token = TokenType::LineStart;
        LocationRange location = curLocation();
        parseNewLine();
        tokenLocation = curLocation().start;
        if(curChar() == WEOF)
        {
            tokenLocation = location.start;
            token = TokenType::LineEnd;
            return;
        }
        putback(TokenType::LineEnd, L"", LocationRange(location.start));
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
    if(curChar() == L'\"')
    {
        parseString();
        return;
    }
    tokenValue = curChar();
    tokenLocation = curLocation();
    switch(curChar())
    {
    case L'+':
        token = TokenType::Plus;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::PlusEqual;
            nextChar();
        }
        break;
    case L'-':
        token = TokenType::Minus;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::MinusEqual;
            nextChar();
        }
        break;
    case L'*':
        token = TokenType::Star;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::StarEqual;
            nextChar();
        }
        break;
    case L'/':
        token = TokenType::FSlash;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::FSlashEqual;
            nextChar();
        }
        break;
    case L'\\':
        token = TokenType::BSlash;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::BSlashEqual;
            nextChar();
        }
        break;
    case L'^':
        token = TokenType::Caret;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::CaretEqual;
            nextChar();
        }
        break;
    case L'=':
        token = TokenType::Equal;
        nextChar();
        break;
    case L'<':
        token = TokenType::LessThan;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::LessEqual;
            nextChar();
        }
        else if(curChar() == L'<')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::LShift;
            if(nextChar() == L'=')
            {
                tokenValue += curChar();
                tokenLocation += curLocation();
                token = TokenType::LShiftEqual;
                nextChar();
            }
        }
        else if(curChar() == L'>')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::NotEqual;
            nextChar();
        }
        break;
    case L'>':
        token = TokenType::GreaterThan;
        if(nextChar() == L'=')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::GreaterEqual;
            nextChar();
        }
        else if(curChar() == L'>')
        {
            tokenValue += curChar();
            tokenLocation += curLocation();
            token = TokenType::RShift;
            if(nextChar() == L'=')
            {
                tokenValue += curChar();
                tokenLocation += curLocation();
                token = TokenType::RShiftEqual;
                nextChar();
            }
        }
        break;
    case L'(':
        token = TokenType::LParen;
        nextChar();
        break;
    case L')':
        token = TokenType::RParen;
        nextChar();
        break;
    case L'{':
        token = TokenType::LBrace;
        nextChar();
        break;
    case L'}':
        token = TokenType::RBrace;
        nextChar();
        break;
    case L',':
        token = TokenType::Comma;
        nextChar();
        break;
    case L':':
        token = TokenType::Colon;
        if(nextChar() == L'=')
        {
            token = TokenType::ColonEqual;
            tokenLocation += curLocation();
            tokenValue += curChar();
            nextChar();
        }
        break;
    case L';':
        token = TokenType::Semicolon;
        nextChar();
        break;
    case L'#':
        token = TokenType::Pound;
        nextChar();
        break;
    default:
        error(L"invalid character", curLocation());
    }
}

void Parser::run()
{
    try
    {
        while(curTokenType() != TokenType::Eof)
        {
            wcout << curTokenLocation().start << L" to " << curTokenLocation().end << L" : ";
            if(curTokenType() == TokenType::LineStart)
                wcout << L"<Line Start>" << endl;
            else if(curTokenType() == TokenType::LineEnd)
                wcout << L"<Line End>" << endl;
            else
                wcout << curTokenValue() << endl;
            nextTokenType();
        }
    }
    catch(Exception & e)
    {
        wcout << L"\nError : " << e.what() << endl;
    }
}
