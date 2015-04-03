/* File:   apa102.h */
/* Date:   Wed 25 Mar 2015 10:15:11 PM CET */
/* Author: Fabian Wermelinger */
/* Tag:    APA102 LED */
/* Copyright 2015 Fabian Wermelinger. All Rights Reserved. */
#include <cstdint>
#include <vector>


class APA102
{
public:
    typedef uint8_t RAWTYPE;

    // representation of one LED
    struct LED
    {
        // order matters here, defined by APA102 datasheet
        RAWTYPE brightness, B, G, R;
        // color 3-tuples
        LED(const unsigned int bright, const RAWTYPE red, const RAWTYPE green, const RAWTYPE blue);
        LED(const RAWTYPE red, const RAWTYPE green, const RAWTYPE blue);
        // hex color
        LED(const unsigned int bright, const unsigned int color);
        LED(const unsigned int color);

        inline void set_brightness(const unsigned int bright)
        {
            brightness = 0xe0 + static_cast<RAWTYPE>(bright/100.0f * 31.0f);
        }

        inline void set_brightness_31(const unsigned int bright31)
        {
            brightness = 0xe0 + bright31;
        }

        inline void set_color(const RAWTYPE red, const RAWTYPE green, const RAWTYPE blue)
        {
            B = blue; G = green; R = red;
        }

        inline void set_color(const unsigned int hex_color)
        {
            B = hex_color; G = hex_color >> 8; R = hex_color >> 16;
        }

        void increase_brightness();
        void decrease_brightness();
        void state() const;
    };

private:
    const unsigned int n_led;
    const unsigned int nbits_start, nbits_end;

    std::vector<RAWTYPE> rawdata;
    LED * const leds;

public:
    APA102(const unsigned int n = 1);
    ~APA102();

    inline unsigned int size() const { return n_led; }
    inline LED * begin() const { return leds; }
    inline LED * end()   const { return leds + n_led; }
    inline LED& operator[](const unsigned int i) { return *(leds + i); }
    inline const LED& operator[](const unsigned int i) const { return *(leds + i); }

    inline unsigned int datasize() const { return rawdata.size(); }
    inline RAWTYPE* data() { return &rawdata[0]; }
};
