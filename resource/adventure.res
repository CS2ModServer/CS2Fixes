// No spaces in event names, max length 32
// All strings are case sensitive
// total game event byte length must be < 1024
//
// valid data key types are:
//   none   : value is not networked
//   string : a zero terminated string
//   bool   : unsigned int, 1 bit
//   byte   : unsigned int, 8 bit
//   short  : signed int, 16 bit
//   long   : signed int, 32 bit
//   float  : float, 32 bit

"cstrikeevents"
{
    "player_land"
    {
        "userid"            "byte" //player index who landed
    }
    "player_airborn"
    {
        "userid"            "byte" //player index who is airborn, could be jump, could be sent flying for some reason.
    }
    "example_event" //note that there is no comma separation needed between entries
    {
        "userid"            "byte"
    }
}