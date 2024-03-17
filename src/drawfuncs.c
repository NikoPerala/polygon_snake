
Color uint32_to_Color(uint32_t color)
{
    Color c;

    c.r = (color >> (3 * 8)) & 0xff;
    c.g = (color >> (2 * 8)) & 0xff;
    c.b = (color >> (1 * 8)) & 0xff;
    c.a = (color >> (0 * 8)) & 0xff;

    return c;
}

