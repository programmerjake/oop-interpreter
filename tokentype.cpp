#include "tokentype.h"
#include <cassert>

wstring getTokenAsPrintableString(TokenType token)
{
    switch(token)
    {
    case TokenType::Eof:
        return L"End Of File";
    case TokenType::LineStart:
        return L"Start Of Line";
    case TokenType::LineEnd:
        return L"End Of Line";
    case TokenType::Identifier:
        return L"Identifier";
    case TokenType::AddressOf:
        return L"AddressOf";
    case TokenType::And:
        return L"And";
    case TokenType::AndAlso:
        return L"AndAlso";
    case TokenType::As:
        return L"As";
    case TokenType::Block:
        return L"Block";
    case TokenType::Boolean:
        return L"Boolean";
    case TokenType::ByRef:
        return L"ByRef";
    case TokenType::Byte:
        return L"Byte";
    case TokenType::ByVal:
        return L"ByVal";
    case TokenType::Case:
        return L"Case";
    case TokenType::Catch:
        return L"Catch";
    case TokenType::CBool:
        return L"CBool";
    case TokenType::CByte:
        return L"CByte";
    case TokenType::CChar:
        return L"CChar";
    case TokenType::CDbl:
        return L"CDbl";
    case TokenType::Char:
        return L"Char";
    case TokenType::CInt:
        return L"CInt";
    case TokenType::Class:
        return L"Class";
    case TokenType::CLng:
        return L"CLng";
    case TokenType::Const:
        return L"Const";
    case TokenType::Continue:
        return L"Continue";
    case TokenType::ContinueDo:
        return L"Continue Do";
    case TokenType::ContinueFor:
        return L"Continue For";
    case TokenType::CSByte:
        return L"CSByte";
    case TokenType::CShort:
        return L"CShort";
    case TokenType::CSng:
        return L"CSng";
    case TokenType::CStr:
        return L"CStr";
    case TokenType::CType:
        return L"CType";
    case TokenType::CUInt:
        return L"CUInt";
    case TokenType::CULng:
        return L"CULng";
    case TokenType::CUShort:
        return L"CUShort";
    case TokenType::Declare:
        return L"Declare";
    case TokenType::Delete:
        return L"Delete";
    case TokenType::Dim:
        return L"Dim";
    case TokenType::Do:
        return L"Do";
    case TokenType::Double:
        return L"Double";
    case TokenType::Each:
        return L"Each";
    case TokenType::Else:
        return L"Else";
    case TokenType::ElseIf:
        return L"ElseIf";
    case TokenType::End:
        return L"End";
    case TokenType::EndBlock:
        return L"End Block";
    case TokenType::EndClass:
        return L"End Class";
    case TokenType::EndEnum:
        return L"End Enum";
    case TokenType::EndFunction:
        return L"End Function";
    case TokenType::EndIf:
        return L"End If";
    case TokenType::EndInterface:
        return L"End Interface";
    case TokenType::EndNamespace:
        return L"End Namespace";
    case TokenType::EndOperator:
        return L"End Operator";
    case TokenType::EndSelect:
        return L"End Select";
    case TokenType::EndStructure:
        return L"End Structure";
    case TokenType::EndSub:
        return L"End Sub";
    case TokenType::EndTry:
        return L"End Try";
    case TokenType::Enum:
        return L"Enum";
    case TokenType::Exit:
        return L"Exit";
    case TokenType::ExitDo:
        return L"Exit Do";
    case TokenType::ExitFor:
        return L"Exit For";
    case TokenType::ExitFunction:
        return L"Exit Function";
    case TokenType::ExitOperator:
        return L"Exit Operator";
    case TokenType::ExitSelect:
        return L"Exit Select";
    case TokenType::ExitSub:
        return L"Exit Sub";
    case TokenType::False:
        return L"False";
    case TokenType::Finally:
        return L"Finally";
    case TokenType::For:
        return L"For";
    case TokenType::Friend:
        return L"Friend";
    case TokenType::Function:
        return L"Function";
    case TokenType::Global:
        return L"Global";
    case TokenType::If:
        return L"If";
    case TokenType::Implements:
        return L"Implements";
    case TokenType::Imports:
        return L"Imports";
    case TokenType::In:
        return L"In";
    case TokenType::Inherits:
        return L"Inherits";
    case TokenType::Integer:
        return L"Integer";
    case TokenType::Interface:
        return L"Interface";
    case TokenType::Long:
        return L"Long";
    case TokenType::Loop:
        return L"Loop";
    case TokenType::Me:
        return L"Me";
    case TokenType::Mod:
        return L"Mod";
    case TokenType::MyBase:
        return L"MyBase";
    case TokenType::MyClass:
        return L"MyClass";
    case TokenType::Namespace:
        return L"Namespace";
    case TokenType::Narrowing:
        return L"Narrowing";
    case TokenType::New:
        return L"New";
    case TokenType::Next:
        return L"Next";
    case TokenType::Not:
        return L"Not";
    case TokenType::Nothing:
        return L"Nothing";
    case TokenType::NotInheritable:
        return L"NotInheritable";
    case TokenType::NotOverridable:
        return L"NotOverridable";
    case TokenType::Object:
        return L"Object";
    case TokenType::Operator:
        return L"Operator";
    case TokenType::Optional:
        return L"Optional";
    case TokenType::Or:
        return L"Or";
    case TokenType::OrElse:
        return L"OrElse";
    case TokenType::Overloads:
        return L"Overloads";
    case TokenType::Overridable:
        return L"Overridable";
    case TokenType::Overrides:
        return L"Overrides";
    case TokenType::ParamArray:
        return L"ParamArray";
    case TokenType::Pointer:
        return L"Pointer";
    case TokenType::Private:
        return L"Private";
    case TokenType::Protected:
        return L"Protected";
    case TokenType::Public:
        return L"Public";
    case TokenType::Return:
        return L"Return";
    case TokenType::SByte:
        return L"SByte";
    case TokenType::Select:
        return L"Select";
    case TokenType::Shared:
        return L"Shared";
    case TokenType::Short:
        return L"Short";
    case TokenType::Single:
        return L"Single";
    case TokenType::Static:
        return L"Static";
    case TokenType::Step:
        return L"Step";
    case TokenType::String:
        return L"String";
    case TokenType::Structure:
        return L"Structure";
    case TokenType::Sub:
        return L"Sub";
    case TokenType::Then:
        return L"Then";
    case TokenType::Throw:
        return L"Throw";
    case TokenType::To:
        return L"To";
    case TokenType::True:
        return L"True";
    case TokenType::Try:
        return L"Try";
    case TokenType::TypeOf:
        return L"TypeOf";
    case TokenType::UInteger:
        return L"UInteger";
    case TokenType::ULong:
        return L"ULong";
    case TokenType::UShort:
        return L"UShort";
    case TokenType::Using:
        return L"Using";
    case TokenType::While:
        return L"While";
    case TokenType::Widening:
        return L"Widening";
    case TokenType::Xor:
        return L"Xor";
    case TokenType::FloatValue:
        return L"Double Literal";
    case TokenType::IntegerValue:
        return L"Integer Literal";
    case TokenType::StringValue:
        return L"String Literal";
    case TokenType::Period:
        return L".";
    case TokenType::Ampersand:
        return L"&";
    case TokenType::Plus:
        return L"+";
    case TokenType::Minus:
        return L"-";
    case TokenType::Star:
        return L"*";
    case TokenType::FSlash:
        return L"/";
    case TokenType::BSlash:
        return L"\\";
    case TokenType::Caret:
        return L"^";
    case TokenType::Equal:
        return L"=";
    case TokenType::PlusEqual:
        return L"+=";
    case TokenType::MinusEqual:
        return L"-=";
    case TokenType::StarEqual:
        return L"*=";
    case TokenType::FSlashEqual:
        return L"/=";
    case TokenType::BSlashEqual:
        return L"\\=";
    case TokenType::CaretEqual:
        return L"^=";
    case TokenType::LessThan:
        return L"<";
    case TokenType::GreaterThan:
        return L">";
    case TokenType::AmpersandEqual:
        return L"&=";
    case TokenType::RShiftEqual:
        return L">>=";
    case TokenType::LShiftEqual:
        return L"<<=";
    case TokenType::GreaterEqual:
        return L">=";
    case TokenType::LessEqual:
        return L"<=";
    case TokenType::RShift:
        return L">>";
    case TokenType::LShift:
        return L"<<";
    case TokenType::NotEqual:
        return L"<>";
    case TokenType::LParen:
        return L"(";
    case TokenType::RParen:
        return L")";
    case TokenType::LBrace:
        return L"{";
    case TokenType::RBrace:
        return L"}";
    case TokenType::Comma:
        return L",";
    case TokenType::Colon:
        return L":";
    case TokenType::Semicolon:
        return L";";
    case TokenType::ColonEqual:
        return L":=";
    case TokenType::Pound:
        return L"#";
    }
    assert(false);
    return L"";
}

