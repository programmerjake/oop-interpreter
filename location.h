#ifndef LOCATION_H_INCLUDED
#define LOCATION_H_INCLUDED

#include <cwchar>
#include <string>
#include <ostream>
#include <algorithm>
#include <sstream>

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
    friend wostream & operator <<(wostream & os, Location l)
    {
        return os << l.toString();
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

#endif // LOCATION_H_INCLUDED
