#ifndef H__ANAGLYPH
#define H__ANAGLYPH

// Constantes
typedef enum
{
    WHITE,
    BLUE,
    GREEN,
    RED,
    CYAN,
    MAGENTA,
    YELLOW,
    BLACK,
    N_COLOR_CODE
} COLOR_CODE;


typedef struct anaglyphState
{
    GLenum              anaglyphBuffer;
    COLOR_CODE          leftColor;
    COLOR_CODE          rightColor;
    GLboolean*          leftMask;  // GLboolean anaglyphLeft[3];
    GLboolean*          rightMask; // GLboolean anaglyphRight[3];
} ANAGLYPH_STATE;


void initAnaglyphMode(void);
void anaglyph_getColorMask(int c, GLboolean *tab);


/* Transformation functions */
void anaglyph_glDrawBuffer(GLenum mode);

#endif /* H__ANAGLYPH */
