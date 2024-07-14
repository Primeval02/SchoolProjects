#ifndef _VT100_ANSI_
#define _VT100_ANSI_

#define VT_ESCAPE      033
#define vt_printf(x)   printf("%c[%s", VT_ESCAPE, x)

#define RESET          "0m"
#define BOLD           "1m"
#define FAINT          "2m"
#define ITALIC         "3m"
#define UNDERLINE      "4m"
#define BLINK          "5m"
#define R_BLINK        "6m"
#define REVERSE        "7m"
#define CONCEAL        "8m"

#define BLACK_F        "30m"
#define RED_F          "31m"
#define GREEN_F        "32m"
#define YELLOW_F       "33m"
#define BLUE_F         "34m"
#define MAGENTA_F      "35m"
#define CYAN_F         "36m"
#define WHITE_F        "37m"

#define BLACK_B        "40m"
#define RED_B          "41m"
#define GREEN_B        "42m"
#define YELLOW_B       "43m"
#define BLUE_B         "44m"
#define MAGENTA_B      "45m"
#define CYAN_B         "46m"
#define WHITE_B        "47m"

#endif
