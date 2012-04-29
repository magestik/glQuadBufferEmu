#include "glQuadBufferEmu.h"
#include "configfile.h"

static const char* def_cfg_file =
"# "PROGRAM_NAME" version "PROGRAM_VERSION" config file \n\n"
"## GENERAL OPTIONS ##\n\n"
"# This set the graphic mode that the emulator will operate on.\n"
"# Possibles values are: MONOSCOPIC, ANAGLYPH, SIDEBYSIDE, INTERLACED, FRAMESEQUENTIAL\n"
"STEREOMODE=FRAMESEQUENTIAL\n\n"
"## MONOSCOPIC MODE ##\n\n"
"# This set what eye must be drawn, possibles values are RIGHT, or LEFT\n"
"MONOSCOPICVIEW=RIGHT\n\n"
"## ANAGLYPH MODE ##\n\n"
"# The color of the left and right eye in anaglyph mode\n"
"# Possibles values are; WHITE, BLUE, GREEN, RED, CYAN, MAGENTA, YELLOW, BLACK\n"
"ANAGLYPH_LEFT_COLOR=RED\n"
"ANAGLYPH_RIGHT_COLOR=CYAN\n\n"
"## INTERLACED MODE ##\n\n"
"# This is how the interlacing will be done. VERTICAL, HORIZONTAL, CHECKERBOARD\n"
"INTERLACED_MODE=VERTICAL\n\n"
"## SIDE-BY-SIDE MODE ##\n\n"
"# This is how side by side mode must be handled.\n"
"# Possibles values are TOPBOTTOM, LEFTRIGHT, FRAMEPACKED\n"
"SIDE_BY_SIDE_MODE=LEFTRIGHT\n\n"
"## FRAMESEQUENTIAL MODE ##\n\n"
"# No option yet :-/";

static KEY_VALUES opt_stereomode = {
    {"MONOSCOPIC", "ANAGLYPH", "SIDEBYSIDE", "INTERLACED", "FRAMESEQUENTIAL", NULL},
    {MONOSCOPIC, ANAGLYPH, SIDEBYSIDE, INTERLACED, FRAMESEQUENTIAL}
};

static KEY_VALUES opt_monoscopicview = {
    {"RIGHT", "LEFT", NULL},
    {GL_RIGHT, GL_LEFT}
};

static KEY_VALUES opt_anaglyph_color = {
    {"WHITE", "BLUE", "GREEN", "RED", "CYAN", "MAGENTA", "YELLOW", "BLACK", NULL},
    {WHITE, BLUE, GREEN, RED, CYAN, MAGENTA, YELLOW, BLACK}
};

static KEY_VALUES opt_interlaced_mode = {
    {"VERTICAL", "HORIZONTAL", "CHECKERBOARD", NULL},
    {VERTICAL, HORIZONTAL, CHECKERBOARD}
};

static KEY_VALUES opt_sbs_mode = {
    {"TOPBOTTOM", "LEFTRIGHT", "FRAMEPACKED", NULL},
    {SBS_TOPBOTTOM, SBS_LEFTRIGHT, SBS_FRAMEPACKED}
};

void QuadBufferEmuLoadConf (void)
{
    configfile_init (def_cfg_file, PROGRAM_NAME);

    configfile_add_key ("STEREOMODE", &opt_stereomode, &QBState.mode);

    configfile_add_key ("MONOSCOPICVIEW", &opt_monoscopicview,
                        &QBState.monoscopic.view);

    configfile_add_key ("ANAGLYPH_LEFT_COLOR", &opt_anaglyph_color,
                        &QBState.anaglyph.leftColor);

    configfile_add_key ("ANAGLYPH_RIGHT_COLOR", &opt_anaglyph_color,
                        &QBState.anaglyph.rightColor);

    configfile_add_key ("INTERLACED_MODE", &opt_interlaced_mode,
                        &QBState.interlaced.mode);

    configfile_add_key ("SIDE_BY_SIDE_MODE", &opt_sbs_mode,
                        &QBState.sidebyside.mode);

    configfile_parse_options ();
}
