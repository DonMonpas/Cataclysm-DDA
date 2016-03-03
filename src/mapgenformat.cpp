#include <iostream>
#include <map>

#include <string>

#include <cassert>
#include <stdarg.h>
#include <algorithm>

#include "output.h"
#include "mapdata.h"
#include "mapgenformat.h"

namespace mapf
{

bool internal::format_data::fix_bindings(const char c)
{
    if( bindings.find(c) != bindings.end() ) {
        return false;
    }
    bindings[c] = int();
    return true;
}

void formatted_set_simple(map* m, const int startx, const int starty, const char* cstr,
                       internal::format_effect ter_b, internal::format_effect furn_b,
                       const bool empty_toilets)
{
    internal::format_data tdata;
    internal::format_data fdata;
    ter_b.execute(tdata);
    furn_b.execute(fdata);

    tdata.fix_bindings(' ');
    fdata.fix_bindings(' ');

    const char* p = cstr;
    int x = startx;
    int y = starty;
    while (*p != 0) {
        if (*p == '\n') {
            y++;
            x = startx;
        } else {
            tdata.fix_bindings(*p);
            fdata.fix_bindings(*p);
            // bindings should be ter_id / furn_id
            ter_id ter = ter_id( tdata.bindings[*p] );
            furn_id furn = furn_id( fdata.bindings[*p] );
            if (ter != t_null) {
                m->ter_set(x, y, ter);
            }
            if (furn != f_null) {
                if (furn == f_toilet && !empty_toilets) {
                    m->place_toilet(x, y);
                } else {
                    m->furn_set(x, y, furn);
                }
            }
            x++;
        }
        p++;
    }
}

internal::format_effect basic_bind(std::string characters, ...)
{
    characters.erase( std::remove_if(characters.begin(), characters.end(), isspace), characters.end());
    std::vector<int> determiners;
    va_list vl;
    va_start(vl,characters);
    determiners.resize(characters.size());
    for( size_t i = 0; i < characters.size(); ++i ) {
        determiners[i] = int( ter_id( va_arg(vl,int) ) );
    }
    va_end(vl);
    return internal::format_effect(characters, determiners);
}

namespace internal
{
    format_effect::format_effect(std::string characters, std::vector<int> &determiners)
        : characters( characters ), determiners( determiners )
    {
    }

    void format_effect::execute(format_data& data)
    {
        for( size_t i = 0; i < characters.size(); ++i ) {
            data.bindings[characters[i]] = determiners[i];
        }
    }
}

}//END NAMESPACE mapf
