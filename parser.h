#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

#include <cwchar>
#include <cwctype>
#include <string>
#include <iterator>
#include <memory>
#include <algorithm>
#include <tuple>
#include <sstream>
#include <initializer_list>
#include <unordered_map>

using namespace std;

const size_t TabWidth = 4;

struct Location
{
    size_t line, column;
    Location()
        : line(1), column(1)
    {
    }
    Location(size_t line, size_t column)
        : line(line), column(column)
    {
    }
    enum class LocationState
    {
        Start,
        GotReturn,
    };
    Location getNext(wint_t ch, LocationState & state) const
    {
        switch(ch)
        {
        case WEOF:
            state = LocationState::Start;
            return Location(line, column);
        case L'\r':
            state = LocationState::GotReturn;
            return Location(line + 1, 1);
        case L'\n':
            if(state == LocationState::GotReturn)
            {
                state = LocationState::Start;
                return Location(line, 1);
            }
            state = LocationState::Start;
            return Location(line + 1, 1);
        case L'\t':
            state = LocationState::Start;
            return Location(line, column + TabWidth - (column - 1) % TabWidth);
        default:
            state = LocationState::Start;
            return Location(line, column + 1);
        }
    }
    friend int compare(const Location & a, const Location & b)
    {
        if(a.line < b.line)
            return -1;
        if(a.line > b.line)
            return 1;
        if(a.column < b.column)
            return -1;
        if(a.column > b.column)
            return 1;
        return 0;
    }
    friend bool operator ==(const Location & a, const Location & b)
    {
        return a.line == b.line && a.column == b.column;
    }
    friend bool operator !=(const Location & a, const Location & b)
    {
        return a.line != b.line && a.column != b.column;
    }
    friend bool operator <(const Location & a, const Location & b)
    {
        return compare(a, b) < 0;
    }
    friend bool operator <=(const Location & a, const Location & b)
    {
        return compare(a, b) <= 0;
    }
    friend bool operator >(const Location & a, const Location & b)
    {
        return compare(a, b) > 0;
    }
    friend bool operator >=(const Location & a, const Location & b)
    {
        return compare(a, b) >= 0;
    }
    wstring toString() const
    {
        wostringstream os;
        os << L"line #" << line << " column #" << column;
        return os.str();
    }
};

struct LocationRange
{
    Location start;
    Location end;
    LocationRange(Location loc = Location())
        : start(loc), end(loc)
    {
    }
    LocationRange(Location start, Location end)
        : start(min(start, end)), end(max(start, end))
    {
    }
    const LocationRange & operator =(Location loc)
    {
        start = loc;
        end = loc;
        return *this;
    }
    friend LocationRange operator +(LocationRange a, LocationRange b)
    {
        return LocationRange(min(a.start, b.start), max(a.end, b.end));
    }
    const LocationRange & operator +=(LocationRange b)
    {
        start = min(start, b.start);
        end = max(end, b.end);
        return *this;
    }
    const LocationRange & advance(wint_t ch, Location::LocationState & state)
    {
        start = end;
        end = end.getNext(ch, state);
        return *this;
    }
    wstring toString() const
    {
        return start.toString();
    }
};

class ParserInput
{
    wstring buffer;
    wint_t currentChar;
    size_t currentBufferIndex;
    LocationRange location;
    Location::LocationState locationState = Location::LocationState::Start;
protected:
    virtual wstring getNextBuffer() = 0;
public:
    ParserInput(wstring buffer)
        : buffer(buffer), currentChar(buffer.size() > 0 ? buffer[0] : WEOF), currentBufferIndex(buffer.size() > 0 ? 1 : 0), locationState(Location::LocationState::Start)
    {
        location = LocationRange().advance(currentChar, locationState);
    }
    wint_t getCurrentChar() const
    {
        return currentChar;
    }
    LocationRange getCurrentLocation() const
    {
        return location;
    }
    wint_t getNextChar()
    {
        if(currentChar == WEOF)
            return currentChar;
        if(currentBufferIndex >= buffer.size())
        {
            currentBufferIndex = 0;
            buffer = getNextBuffer();
        }
        if(buffer.size() == 0)
            currentChar = WEOF;
        else
            currentChar = buffer[currentBufferIndex++];
        location.advance(currentChar, locationState);
        return currentChar;
    }
    LocationRange getNextLocation()
    {
        getNextChar();
        return location;
    }
    operator bool() const
    {
        return currentChar != WEOF;
    }
    bool operator !() const
    {
        return currentChar == WEOF;
    }
};

class IStreamParserInput : public ParserInput
{
private:
    static wstring read(wistream & is)
    {
        if(!&is || !is)
            return L"";
        wstring retval;
        retval.resize(256);
        is.read(&retval[0], retval.size());
        retval.resize(is.gcount());
        return retval;
    }
    shared_ptr<wistream> is;
protected:
    virtual wstring getNextBuffer() override
    {
        return read(*is.get());
    }
public:
    IStreamParserInput(shared_ptr<wistream> is)
        : ParserInput(read(*is.get())), is(is)
    {
    }
    IStreamParserInput(wistream & is)
        : ParserInput(read(is)), is(&is, [](wistream *){})
    {
    }
};

class Exception
{
    wstring msg;
public:
    Exception(wstring msg)
        : msg(msg)
    {
    }
    virtual ~Exception()
    {
    }
    const wstring & what() const
    {
        return msg;
    }
};

struct ParseError : public Exception
{
    const LocationRange location;
    ParseError(wstring msg, LocationRange location)
        : Exception(location.toString() + L" : " + msg), location(location)
    {
    }
};

class Parser
{
private:
    static void error(wstring msg, LocationRange location)
    {
        throw ParseError(msg, location);
    }
    template <typename T>
    static void expected(initializer_list<T> expectedList, LocationRange location)
    {
        wstring msg = L"expected : ";
        if(expectedList.size() == 1)
        {
            msg += *expectedList.begin();
        }
        else if(expectedList.size() == 2)
        {
            msg += expectedList.begin()[0];
            msg += L" or ";
            msg += expectedList.begin()[1];
        }
        else if(expectedList.size() >= 3)
        {
            for(size_t i = 0; i < expectedList.size(); i++)
            {
                if(i > 0)
                    msg += L", ";
                if(i == expectedList.size() - 1)
                    msg += L"or ";
                msg += expectedList.begin()[i];
            }
        }
        error(msg, location);
    }
    static void unexpected(wstring msg, LocationRange location)
    {
        error(L"unexpected : " + msg, location);
    }
    enum class TokenType
    {
        Eof,
        NewLine,
        Identifier,
        FloatValue,
        IntegerValue,
        End,
        Exit,
        Period,
        Ampersand,
        If,
        EndIf,
    };
    class Tokenizer
    {
    private:
        shared_ptr<ParserInput> parserInput;
        wint_t putbackChar;
        LocationRange putbackLocation;
        static bool isValidIdentifierStartCharacter(wchar_t ch)
        {
            if(iswalpha(ch) || ch == L'_')
                return true;
            return false;
        }
        static bool isValidIdentifierCharacter(wchar_t ch)
        {
            if(iswalnum(ch) || ch == L'_')
                return true;
            return false;
        }
        wint_t curChar()
        {
            if(putbackChar != WEOF)
                return putbackChar;
            return parserInput->getCurrentChar();
        }
        LocationRange curLocation()
        {
            if(putbackChar != WEOF)
                return putbackLocation;
            return parserInput->getCurrentLocation();
        }
        wint_t nextChar()
        {
            if(putbackChar != WEOF)
            {
                putbackChar = WEOF;
                return parserInput->getCurrentChar();
            }
            putbackLocation = parserInput->getCurrentLocation();
            return parserInput->getNextChar();
        }
        LocationRange nextLocation()
        {
            if(putbackChar != WEOF)
            {
                putbackChar = WEOF;
                return parserInput->getCurrentLocation();
            }
            putbackLocation = parserInput->getCurrentLocation();
            return parserInput->getNextLocation();
        }
        void putback(wint_t ch, LocationRange location)
        {
            putbackChar = ch;
            putbackLocation = location;
        }
        tuple<wstring, LocationRange> parseWord();
        LocationRange parseComment();
        LocationRange parseWhitespace();
        LocationRange parseNewLine();
        void parseNumber();
        TokenType token;
        LocationRange tokenLocation;
        wstring tokenValue;
        TokenType putbackToken;
        LocationRange putbackTokenLocation;
        wstring putbackTokenValue;
        static unordered_map<wstring, TokenType> makeKeywordMap();
        void checkForKeyword();
        void putback(TokenType tokenType, wstring value, LocationRange location)
        {
            putbackToken = token;
            putbackTokenValue = tokenValue;
            putbackTokenLocation = tokenLocation;
            token = tokenType;
            tokenValue = value;
            tokenLocation = location;
        }
        void nextToken();
    public:
        Tokenizer(shared_ptr<ParserInput> parserInput)
            : parserInput(parserInput), putbackChar(WEOF), token(TokenType::NewLine), putbackToken(TokenType::Eof)
        {
            nextToken();
        }
        TokenType currentTokenType() const
        {
            return token;
        }
        LocationRange currentTokenLocation() const
        {
            return tokenLocation;
        }
        wstring currentTokenValue() const
        {
            return tokenValue;
        }
        TokenType nextTokenType()
        {
            nextToken();
            return token;
        }
        LocationRange nextTokenLocation()
        {
            nextToken();
            return tokenLocation;
        }
        wstring nextTokenValue()
        {
            nextToken();
            return tokenValue;
        }
    };
    Tokenizer tokenizer;
    TokenType putbackTokenType;
    wstring putbackTokenValue;
    LocationRange putbackTokenLocation;
public:
    Parser(shared_ptr<ParserInput> parserInput)
        : tokenizer(parserInput), putbackTokenType(TokenType::Eof)
    {
    }
private:
    TokenType curTokenType() const
    {
        if(putbackTokenType != TokenType::Eof)
            return putbackTokenType;
        return tokenizer.currentTokenType();
    }
    LocationRange curTokenLocation() const
    {
        if(putbackTokenType != TokenType::Eof)
            return putbackTokenLocation;
        return tokenizer.currentTokenLocation();
    }
    wstring curTokenValue() const
    {
        if(putbackTokenType != TokenType::Eof)
            return putbackTokenValue;
        return tokenizer.currentTokenValue();
    }
    TokenType nextTokenType()
    {
        if(putbackTokenType != TokenType::Eof)
        {
            putbackTokenType = TokenType::Eof;
            return tokenizer.currentTokenType();
        }
        return tokenizer.nextTokenType();
    }
    LocationRange nextTokenLocation()
    {
        if(putbackTokenType != TokenType::Eof)
        {
            putbackTokenType = TokenType::Eof;
            return tokenizer.currentTokenLocation();
        }
        return tokenizer.nextTokenLocation();
    }
    wstring nextTokenValue()
    {
        if(putbackTokenType != TokenType::Eof)
        {
            putbackTokenType = TokenType::Eof;
            return tokenizer.currentTokenValue();
        }
        return tokenizer.nextTokenValue();
    }
    void putback(TokenType tokenType, wstring tokenValue, LocationRange tokenLocation)
    {
        putbackTokenType = tokenType;
        putbackTokenValue = tokenValue;
        putbackTokenLocation = tokenLocation;
    }
public:
    void run();
};

#endif // PARSER_H_INCLUDED
