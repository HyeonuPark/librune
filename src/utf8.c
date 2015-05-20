#include <stdlib.h>
#include "librune.h"

const rn_UTF8Buffer rn_DefaultUTF8Buffer = {0, {0, 0, 0}};

rn_utf8 rn_utf8Encode(rn_unicode unicode)
{
    rn_utf8 utf8 = malloc(sizeof(rn_UTF8Buffer));

    /* this unicode will be converted to 1-byte utf8 code */
    if (!(unicode >> 7))
    {
        utf8->size = 1;
        utf8->bytes[0] = (rn_byte) unicode;
        return utf8;
    }

    /* this unicode will be converted to 2-byte utf8 code */
    if (!(unicode >> 11))
    {
        utf8->size = 2;
        utf8->bytes[0] = 0xC0 + ((unicode >> 6) & 0x1F);
        utf8->bytes[1] = 0x80 + (unicode & 0x3F);
        return utf8;
    }

    /* this unicode will be converted to 3-byte utf8 code */
    utf8->size = 3;
    utf8->bytes[0] = 0xE0 + ((unicode >> 12) & 0x0F);
    utf8->bytes[1] = 0x80 + ((unicode >> 6) & 0x3F);
    utf8->bytes[2] = 0x80 + (unicode & 0x3F);
    return utf8;
}

rn_utf8 rn_utf8DecodeStart()
{
    rn_utf8 utf8 = malloc(sizeof(rn_UTF8Buffer));
    *utf8 = rn_DefaultUTF8Buffer;
    return utf8;
}

rn_unicode rn_utf8DecodeNext(rn_utf8 utf8, rn_byte utf8Byte)
{
    /* if utf8 buffer is empty */
    if (!utf8->size)
    {
        *utf8 = rn_DefaultUTF8Buffer;

        /* if received byte is valid ascii code */
        if (!(utf8Byte & 0x80))
        {
            return (rn_unicode) utf8Byte;
        }

        /* if received byte is valid first position utf8 code */

        /* if this utf8 code's byte size is 2 */
        if (((utf8Byte & 0xE0) == (rn_byte) 0xC0) && (utf8Byte & 0x1F))
        {
            utf8->size = 2;
            utf8->bytes[0] = utf8Byte;
        }

        /* if this utf8 code's byte size is 3 */
        else if (((utf8Byte & 0xF0) == (rn_byte) 0xE0) && (utf8Byte & 0xF))
        {
            utf8->size = 3;
            utf8->bytes[0] = utf8Byte;
        }

        /* if received byte is invalid utf8 code */
        return (rn_unicode) 0;
    }

    /* if this byte is invalid utf8 code */
    if ((utf8Byte & 0xC0) != (rn_byte) 0x80) return (rn_unicode) 0;

    /* if utf8 buffer has single byte and it says it's size is 2 */
    if (utf8->size == 2)
    {
        return (((rn_unicode)(utf8->bytes[0] & 0x1F) << 6)
               + (rn_unicode)(utf8Byte & 0x3F));
    }

    /* if utf8 buffer says it's size is 3 */
    if (utf8->size == 3)
    {
        /* and it has only single byte */
        if (!utf8->bytes[1])
        {
            utf8->bytes[1] = utf8Byte;
            return (rn_unicode) 0;
        }

        /* now we has all 3 bytes */
        return (((rn_unicode)(utf8->bytes[0] & 0xF) << 12)
              + ((rn_unicode)(utf8->bytes[1] & 0x3F) << 6)
              +  (rn_unicode)(utf8Byte & 0x3F));
    }

    /* invalid utf8 buffer */
    *utf8 = rn_DefaultUTF8Buffer;
    return (rn_unicode) 0;
}
