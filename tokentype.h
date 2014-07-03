#ifndef TOKENTYPE_H_INCLUDED
#define TOKENTYPE_H_INCLUDED

#include <tuple>
#include "location.h"

using namespace std;

enum class TokenType
{
    Eof,
    LineStart,
    LineEnd,

    Identifier,

    AddressOf,
    And,
    AndAlso,
    As,
    Block,
    Boolean,
    ByRef,
    Byte,
    ByVal,
    Case,
    Catch,
    CBool,
    CByte,
    CChar,
    CDbl,
    Char,
    CInt,
    Class,
    CLng,
    Const,
    Continue,
    ContinueDo,
    ContinueFor,
    CSByte,
    CShort,
    CSng,
    CStr,
    CType,
    CUInt,
    CULng,
    CUShort,
    Declare,
    Delete,
    Dim,
    Do,
    Double,
    Each,
    Else,
    ElseIf,
    End,
    EndBlock,
    EndClass,
    EndEnum,
    EndFunction,
    EndIf,
    EndInterface,
    EndNamespace,
    EndOperator,
    EndSelect,
    EndStructure,
    EndSub,
    EndTry,
    Enum,
    Exit,
    ExitDo,
    ExitFor,
    ExitFunction,
    ExitOperator,
    ExitSelect,
    ExitSub,
    False,
    Finally,
    For,
    Friend,
    Function,
    Global,
    If,
    Implements,
    Imports,
    In,
    Inherits,
    Integer,
    Interface,
    Long,
    Loop,
    Me,
    Mod,
    MyBase,
    MyClass,
    Namespace,
    Narrowing,
    New,
    Next,
    Not,
    Nothing,
    NotInheritable,
    NotOverridable,
    Object,
    Operator,
    Optional,
    Or,
    OrElse,
    Overloads,
    Overridable,
    Overrides,
    ParamArray,
    Pointer,
    Private,
    Protected,
    Public,
    Return,
    SByte,
    Select,
    Shared,
    Short,
    Single,
    Static,
    Step,
    String,
    Structure,
    Sub,
    Then,
    Throw,
    To,
    True,
    Try,
    TypeOf,
    UInteger,
    ULong,
    UShort,
    Using,
    While,
    Widening,
    Xor,



    FloatValue,
    IntegerValue,
    StringValue,

    Period,
    Ampersand,
    Plus,
    Minus,
    Star,
    FSlash,
    BSlash,
    Caret,
    Equal,
    PlusEqual,
    MinusEqual,
    StarEqual,
    FSlashEqual,
    BSlashEqual,
    CaretEqual,
    LessThan,
    GreaterThan,
    AmpersandEqual,
    RShiftEqual,
    LShiftEqual,
    GreaterEqual,
    LessEqual,
    RShift,
    LShift,
    NotEqual,
    LParen,
    RParen,
    LBrace,
    RBrace,
    Comma,
    Colon,
    Semicolon,
    ColonEqual,
    Pound,
};

wstring getTokenAsPrintableString(TokenType token);

struct Token final
{
    TokenType type;
    wstring value;
    LocationRange location;
    Token(TokenType type, wstring value, LocationRange location = LocationRange(Location(0, 0)))
        : type(type), value(value), location(location)
    {
    }
    Token(TokenType type)
        : type(type), location(Location(0, 0))
    {
        if(type == TokenType::Identifier)
            value = L"Identifier";
        else if(type == TokenType::FloatValue)
            value = L"0.0";
        else if(type == TokenType::IntegerValue)
            value = L"0";
        else
            value = getTokenAsPrintableString(type);
    }
    wstring toString() const
    {
        if(type == TokenType::Identifier)
            return getTokenAsPrintableString(type) + L" : " + value;
        return getTokenAsPrintableString(type);
    }
    wstring toSourceString() const
    {
        if(type == TokenType::Identifier || type == TokenType::FloatValue || type == TokenType::IntegerValue)
            return value;
        if(type == TokenType::StringValue)
        {
            wostringstream os;
            os << L"\"" << hex;
            for(wchar_t ch : value)
            {
                if(ch == L'\"')
                    os << L"\"\"";
                else if(iswgraph(ch) || iswblank(ch))
                    os << ch;
                else
                    os << L"\" + CChar(&H" << (unsigned)ch << ") + \"";
            }
            os << L"\"";
            return os.str();
        }
        return getTokenAsPrintableString(type);
    }
};

#endif // TOKENTYPE_H_INCLUDED
