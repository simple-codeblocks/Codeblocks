///////////////////////////////////////////////////////////////////////////////
// Name:		fnb_resources.cpp 
// Purpose:     implementation of the resources used by wxFlatNotebook
// Author:      Eran Ifrah <eran.ifrah@gmail.com>
// Created:     30/12/2005
// Modified:    01/01/2006
// Copyright:   Eran Ifrah (c)
// Licence:     wxWindows license <http://www.wxwidgets.org/licence3.txt>
///////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
//
// XPM Images required by this control
//
///////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Images used by the control
////////////////////////////////////////////////////////////

/* XPM */
const char *signpost_xpm[] = {
/* columns rows colors chars-per-pixel */
"16 16 104 2",
"   c Black",
"M  c #3FB524",
"3  c #4CBAFF",
"{  c #004C00",
"1  c #006BE9",
"P  c #007200",
"!  c #0065DD",
"-  c #003FE5",
"Q  c #0056BC",
"*  c #0062E4",
"t  c #0945A7",
"7  c #2BBF1F",
"W  c #0050B8",
")  c #D59700",
"L  c #0E8508",
"A  c #0047B0",
"u  c #3D93E5",
"x  c #0044AD",
":  c #46C733",
"(  c #B18800",
"X  c #E3C247",
"O. c #D69B00",
"E  c #0041B1",
"`  c #824400",
"|  c #C59200",
".. c #7B4000",
"*. c #763C00",
"G  c #056ED4",
"f  c #088806",
"[  c #005100",
"p  c #006DDF",
"g  c #007700",
"U  c #6D6A00",
"<  c #4A4357",
"Z  c #0038B7",
"%  c #E1B024",
"K  c #16A40B",
" . c #986500",
"h  c #007100",
"e  c #002C9F",
"J  c #018101",
"C  c #4A3D58",
";  c #009400",
"v  c #078BF0",
"$. c #AC6F00",
"F  c #0055BC",
"s  c #36B01A",
"D  c #0052B8",
",  c #138E00",
"=  c #0058D3",
"5  c #47A9FF",
"~  c #006500",
"S  c #004CB2",
"n  c #047F02",
"T  c #572E17",
"m  c #37BA1A",
"X. c #D89D00",
"}  c #E49C00",
"6  c #0084FF",
"+  c #D3990C",
"l  c #0040A6",
"a  c #008200",
"_  c #9C6700",
"d  c #86EA4D",
"O  c #DCC042",
"#  c #C7C334",
"&. c #835000",
"B  c #007900",
"k  c #0037A9",
"#. c #7B3F00",
"y  c #3879CB",
"%. c #A26F00",
"N  c #007300",
".  c #D0A000",
"&  c #626651",
"j  c #594855",
"'  c #0031B3",
"q  c #007000",
"2  c #31AAFF",
"Y  c #A96700",
"]  c #0034C6",
"R  c #0028A7",
"@. c #9B6800",
"o  c #EAD96A",
"V  c #006700",
"4  c #4CACFB",
"9  c #9EFF63",
"^  c #006400",
"o. c #7A4000",
"b  c #0080E5",
"z  c #0045AC",
">  c #66DD42",
"I  c #005E00",
"c  c #125EBE",
"H  c #0080F5",
"w  c #584955",
"8  c #8BF35A",
"@  c #8E9300",
"/  c #005800",
"+. c #CA9700",
"i  c #0071DE",
"0  c #2DA51D",
"$  c #FFDE65",
"r  c #003098",
/* pixels */
"                                ",
"                                ",
"        . X o O +               ",
"        @ # $ % & * = -         ",
"      ; : > , < 1 2 3 4 5 6     ",
"    7 8 9 0 q w e r t y u i p   ",
"  a s d f g h j k l z x c v b   ",
"  n m M N B V C Z A S D F G H   ",
"  J K L P I U Y T R E W Q !     ",
"  h ~ ^ / ( ) _ `     ' ]       ",
"      [ { } |  ...              ",
"          X.|  .o.              ",
"          O.+.@.#.              ",
"          $.%.&.*.              ",
"                                ",
"                                "
};
unsigned char signpost_alpha[]={
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   7, 174, 240, 238, 148,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,  43, 254, 255, 255, 253, 104,  48,   4,   0,   0,   0,   0,
0,   0,   0,  60, 221, 255, 255, 255, 254, 254, 254, 244, 193,  51,   0,   0,
0,   0, 126, 252, 255, 255, 255, 255, 255, 255, 254, 254, 254, 224,  20,   0,
0,  33, 252, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 183,   0,
0, 137, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 254, 128,   0,
0, 211, 254, 254, 255, 255, 255, 255, 246, 147, 216, 254, 225,  81,   0,   0,
0,  20, 162, 246, 234, 248, 255, 255, 238,   0,   0,  29,  11,   0,   0,   0,
0,   0,   0,  11,  20, 231, 255, 255, 235,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0, 223, 255, 255, 233,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0, 214, 255, 255, 231,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0, 137, 240, 236, 131,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 }; 

/* XPM */
const char *left_arrow_disabled_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #555555",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"````````.```````",
"```````..```````",
"``````.`.```````",
"`````.``.```````",
"````.```.```````",
"`````.``.```````",
"``````.`.```````",
"```````..```````",
"````````.```````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};

/* XPM */
const char *x_button_pressed_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #9e9ede",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.############.`",
"`.############.`",
"`.###aa####aa#.`",
"`.####aa##aa##.`",
"`.#####aaaa###.`",
"`.######aa####.`",
"`.#####aaaa###.`",
"`.####aa##aa##.`",
"`.###aa####aa#.`",
"`.############.`",
"`..............`",
"````````````````",
"````````````````"
};


/* XPM */
const char *tab_x_button_pressed_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #008080",
"# c #008080",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.############.`",
"`.############.`",
"`.###aa####aa#.`",
"`.####aa##aa##.`",
"`.#####aaaa###.`",
"`.######aa####.`",
"`.#####aaaa###.`",
"`.####aa##aa##.`",
"`.###aa####aa#.`",
"`.############.`",
"`..............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *left_arrow_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #555555",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"````````.```````",
"```````..```````",
"``````...```````",
"`````....```````",
"````.....```````",
"`````....```````",
"``````...```````",
"```````..```````",
"````````.```````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};

/* XPM */
const char *x_button_hilite_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #c9dafb",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.############.`",
"`.##aa####aa##.`",
"`.###aa##aa###.`",
"`.####aaaa####.`",
"`.#####aa#####.`",
"`.####aaaa####.`",
"`.###aa##aa###.`",
"`.##aa####aa##.`",
"`.############.`",
"`.############.`",
"`..............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *x_button_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #555555",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````..````..````",
"`````..``..`````",
"``````....``````",
"```````..```````",
"``````....``````",
"`````..``..`````",
"````..````..````",
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};

/* XPM */
const char *left_arrow_pressed_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #9e9ede",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.############.`",
"`.#######a####.`",
"`.######aa####.`",
"`.#####aaa####.`",
"`.####aaaa####.`",
"`.###aaaaa####.`",
"`.####aaaa####.`",
"`.#####aaa####.`",
"`.######aa####.`",
"`.#######a####.`",
"`..............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *left_arrow_hilite_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #c9dafb",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.######a#####.`",
"`.#####aa#####.`",
"`.####aaa#####.`",
"`.###aaaa#####.`",
"`.##aaaaa#####.`",
"`.###aaaa#####.`",
"`.####aaa#####.`",
"`.#####aa#####.`",
"`.######a#####.`",
"`.############.`",
"`..............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *right_arrow_disabled_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #555555",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"```````.````````",
"```````..```````",
"```````.`.``````",
"```````.``.`````",
"```````.```.````",
"```````.``.`````",
"```````.`.``````",
"```````..```````",
"```````.````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};

/* XPM */
const char *right_arrow_hilite_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #c9dafb",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.####a#######.`",
"`.####aa######.`",
"`.####aaa#####.`",
"`.####aaaa####.`",
"`.####aaaaa###.`",
"`.####aaaa####.`",
"`.####aaa#####.`",
"`.####aa######.`",
"`.####a#######.`",
"`.############.`",
"`..............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *right_arrow_pressed_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #9e9ede",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"`..............`",
"`.############.`",
"`.############.`",
"`.#####a######.`",
"`.#####aa#####.`",
"`.#####aaa####.`",
"`.#####aaaa###.`",
"`.#####aaaaa##.`",
"`.#####aaaa###.`",
"`.#####aaa####.`",
"`.#####aa#####.`",
"`.#####a######.`",
"`..............`",
"````````````````",
"````````````````"
};


/* XPM */
const char *right_arrow_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #555555",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"```````.````````",
"```````..```````",
"```````...``````",
"```````....`````",
"```````.....````",
"```````....`````",
"```````...``````",
"```````..```````",
"```````.````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};

// Arrow down bitmaps
/* XPM */
const char *down_arrow_hilite_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #c9dafb",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"``.............`",
"``.###########.`",
"``.###########.`",
"``.###########.`",
"``.#aaaaaaaaa#.`",
"``.##aaaaaaa##.`",
"``.###aaaaa###.`",
"``.####aaa####.`",
"``.#####a#####.`",
"``.###########.`",
"``.###########.`",
"``.###########.`",
"``.............`",
"````````````````",
"````````````````"
};

/* XPM */
const char *down_arrow_pressed_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #4766e0",
"# c #9e9ede",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"``.............`",
"``.###########.`",
"``.###########.`",
"``.###########.`",
"``.###########.`",
"``.###########.`",
"``.#aaaaaaaaa#.`",
"``.##aaaaaaa##.`",
"``.###aaaaa###.`",
"``.####aaa####.`",
"``.#####a#####.`",
"``.###########.`",
"``.............`",
"````````````````",
"````````````````"
};


/* XPM */
const char *down_arrow_xpm[] = {
/* width height num_colors chars_per_pixel */
"    16    16        8            1",
/* colors */
"` c #008080",
". c #000000",
"# c #000000",
"a c #000000",
"b c #000000",
"c c #000000",
"d c #000000",
"e c #000000",
/* pixels */
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````.........```",
"`````.......````",
"``````.....`````",
"```````...``````",
"````````.```````",
"````````````````",
"````````````````",
"````````````````",
"````````````````",
"````````````````"
};	

// Arrow down bitmaps
const int tab_x_size = 9;

// arrow_down_blue.xpm 16x16
unsigned char arrow_down_blue_alpha[]={
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 246, 250, 250, 248, 246, 245,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   2,  11,  11,   0, 255, 255, 255, 255, 255, 255,   0,  11,  11,   2,   0, 
  0,   7, 190, 254, 254, 255, 255, 255, 255, 255, 255, 254, 254, 186,   5,   0, 
  0,   0,  15, 209, 255, 255, 255, 255, 255, 255, 255, 255, 206,  13,   0,   0, 
  0,   0,   0,  28, 226, 255, 255, 255, 255, 255, 255, 223,  25,   0,   0,   0, 
  0,   0,   0,   0,  43, 238, 255, 255, 255, 255, 236,  40,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,  62, 247, 255, 255, 245,  58,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,  86, 253, 252,  82,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0, 112, 107,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
};
/* XPM */
const char *arrow_down_blue_xpm[] = {
/* columns rows colors chars-per-pixel */
"16 16 99 2",
"a  c #3E8FDC",
"   c Black",
"+. c #0068E2",
"Y  c #77B5F4",
"#  c #206BC4",
" . c #0065E7",
"I  c #0F68CE",
",  c #1C7AD3",
":  c #217ED9",
"`  c #0F62C4",
"o. c #377FCB",
"U  c #89C2FA",
"N  c #0050B8",
"u  c #84BBF1",
"d  c #004DB4",
"(  c #7DBAF7",
"&  c #338BE0",
"c  c #539DE8",
"E  c #62A9EE",
"X. c #3A7DC4",
"[  c #2E84DF",
"!  c #0053E1",
"M  c #0D56B4",
"k  c #1F6FCA",
"@  c #003BA2",
"D  c #60A6EB",
"o  c #0345AC",
"w  c #1054AA",
"A  c #8DC4F8",
"x  c #8DC1F2",
"e  c #000087",
".. c #006AEC",
"b  c #4893DD",
"g  c #00008E",
"V  c #0067E5",
"*  c #177BD5",
"O. c #0067EA",
"]  c #0064E8",
"9  c #3C8EE0",
"%  c #4B9AE4",
"'  c #0061E1",
"<  c #074DA7",
"~  c #0064EC",
"s  c #1155AA",
">  c #217DD8",
"G  c #559FE9",
"F  c #5AA3EA",
"3  c #3086DD",
"X  c #074AAF",
"^  c #1C73D5",
"W  c #508CC5",
"P  c #005BE7",
"|  c #1C6DC6",
"5  c #2982D6",
"/  c #91C9FE",
"l  c #3077CB",
"=  c #034AA7",
"Q  c #065ABF",
"Z  c #8AC5FD",
"+  c #0347AC",
"-  c #246CC4",
"p  c #4594E4",
"J  c #4084C2",
"L  c #0043D9",
"K  c #0454BA",
"q  c #3488D9",
"@. c #0069E0",
"}  c #79A7D3",
"H  c #52A0EB",
"j  c #005AC2",
"m  c #115AB4",
"y  c #2F78CE",
"f  c #0057C0",
"1  c #2A71C9",
"2  c #70AFEC",
"8  c #7AB7EF",
"h  c #005AD6",
"C  c #0D66CA",
"6  c #0A50A9",
"v  c #4E9AE6",
"_  c #6498C9",
"r  c #0054DD",
"4  c #2E85DB",
"{  c #9BD0FF",
"O  c #0546A9",
"T  c #6CAFF0",
"R  c #67ABF0",
".  c #0042AA",
")  c #76B7F7",
"i  c #4796E4",
";  c #60A7E7",
"B  c #003FAF",
"t  c #0048D1",
"n  c #246BBA",
"7  c #2E76CC",
"$  c #7EBCF0",
"z  c #5696DD",
"S  c #79B5F0",
"0  c #398EE0",
/* pixels */
"                                ",
"                                ",
"          . X o O + @           ",
"          # $ % & * =           ",
"          - ; : > , <           ",
"          1 2 3 4 5 6           ",
"          7 8 9 0 q w           ",
"  e r t   y u i p a s   d f g   ",
"  h j k l z x c v b n m M N B   ",
"    V C Z A S D F G H J K L     ",
"      P I U Y T R E W Q !       ",
"        ~ ^ / ( ) _ ` '         ",
"          ] [ { } |  .          ",
"            ..X.o.O.            ",
"              +.@.              ",
"                                "
};
// arrow_up_blue.xpm 16x16
unsigned char arrow_up_blue_alpha[]={
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,  95, 110,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,  72, 250, 252,  85,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,  51, 242, 255, 255, 247,  62,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,  35, 232, 255, 255, 255, 255, 238,  44,   0,   0,   0,   0, 
  0,   0,   0,  21, 218, 255, 255, 255, 255, 255, 255, 226,  28,   0,   0,   0, 
  0,   0,  11, 200, 255, 255, 255, 255, 255, 255, 255, 255, 210,  16,   0,   0, 
  0,   4, 180, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 192,   7,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 255, 255, 255, 255, 255, 255,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0, 241, 244, 245, 243, 241, 239,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
};
/* XPM */
const char *arrow_up_blue_xpm[] = {
/* columns rows colors chars-per-pixel */
"16 16 93 2",
"   c Black",
"n  c #2262AB",
"@  c #006BED",
"!  c #237ED9",
"#  c #006BF2",
"G  c #81BAF0",
"(  c #4B9EE8",
"/  c #1B67C2",
":  c #BFE1FF",
"{  c #0444A8",
"B  c #005CD7",
"2  c #005CDF",
"N  c #0050B8",
")  c #1F81DC",
"D  c #145AAC",
"I  c #73B2EC",
"T  c #2983D9",
">  c #7DBAF5",
"O  c #4998EA",
"z  c #3F8ADB",
"9  c #62A9EC",
"5  c #B6DAFD",
"h  c #004DD2",
"s  c #519DE5",
"t  c #B4DAFE",
"L  c #1057AD",
"C  c #8FC1F3",
"4  c #0768CF",
"<  c #79B7F5",
"a  c #549EE7",
"*  c #257DDA",
"J  c #398DDF",
"R  c #0E54A9",
",  c #72B3F4",
"%  c #C6E4FF",
";  c #1471D3",
"$  c #2881DE",
"i  c #61A7EB",
"m  c #115BB5",
"u  c #66ABEF",
"Q  c #2681D9",
"c  c #559FE8",
"3  c #0061EC",
"-  c #0061ED",
"K  c #358ADC",
"k  c #1C70CD",
"r  c #0666CD",
"`  c #1378D5",
"Y  c #2E86DD",
"[  c #054AAF",
"W  c #67A9E8",
"^  c #084EA6",
"q  c #0B61C7",
"f  c #065AC3",
" . c #003AA0",
"H  c #3B8FE1",
"8  c #65A9ED",
"y  c #94C7F8",
"v  c #4F9CE6",
"6  c #7EBAF5",
".  c #006FE6",
"_  c #197DDB",
"F  c #2773CB",
"}  c #0141A0",
"E  c #1D68C5",
"o  c #006FF1",
"&  c #9CCFFF",
"j  c #0060C2",
"|  c #013EA0",
"0  c #61A9EF",
"=  c #0066E9",
"7  c #6BAEEF",
"S  c #4191E0",
"w  c #0060DD",
"l  c #2373CE",
"+  c #509DEC",
"U  c #3088DD",
"M  c #0F5DBC",
"p  c #5AA2E9",
"A  c #4495E4",
"'  c #054CA7",
"~  c #1F7CD5",
"b  c #4996E0",
"1  c #156ED0",
"d  c #51A1EC",
"Z  c #4796E4",
"e  c #004EE0",
"V  c #2975CC",
"g  c #0048D5",
"P  c #226EC8",
"x  c #6DB0EE",
"X  c #006BE1",
"]  c #0143A8",
/* pixels */
"                                ",
"              . X               ",
"            o O + @             ",
"          # $ % & * =           ",
"        - ; : > , < 1 2         ",
"      3 4 5 6 7 8 9 0 q w       ",
"    e r t y u i p a s d f g     ",
"  h j k l z x c v b n m M N B   ",
"          V C Z A S D           ",
"          F G H J K L           ",
"          P I U Y T R           ",
"          E W Q ! ~ ^           ",
"          / ( ) _ ` '           ",
"          ] [ { } |  .          ",
"                                ",
"                                "
};


//////////////////////////////////////////////////////////////////////////////////////////////////
//
// End of XPM Images
//
//////////////////////////////////////////////////////////////////////////////////////////////////
