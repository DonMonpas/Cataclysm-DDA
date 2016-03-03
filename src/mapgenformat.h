#ifndef MAPGENFORMAT_H
#define MAPGENFORMAT_H

#include <vector>
#include <memory>

#include "map.h"
/////

struct ter_furn_id;

/////
namespace mapf
{
namespace internal
{
class format_effect;
}
/** The return statement for this method is not finalized.
 * The following things will be acces from this return statements.
 * - the region of points set by given character
 * - the region of points set to a given terrian id
 * - possibly some other stuff
 * You will have specify the values you want to track with a parameter.
 */
void formatted_set_simple( map *m, const int startx, const int starty, const char *cstr,
                           internal::format_effect ter_b, internal::format_effect furn_b,
                           const bool empty_toilets = false );

internal::format_effect basic_bind( std::string characters, ... );

// Anything specified in here isn't finalized
namespace internal
{
struct format_data {
    std::map<char, int> bindings;
    bool fix_bindings( const char c );
};

// This class will become an interface in the future.
class format_effect
{
    private:
        std::string characters;
        std::vector<int> determiners;

    public:
        format_effect( std::string characters,
                       std::vector<int> &determiners );

        void execute( format_data &data );
};

} //END NAMESPACE mapf::internal

} //END NAMESPACE mapf

#endif
