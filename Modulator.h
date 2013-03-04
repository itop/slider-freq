#ifndef MODULATOR_H
#define MODULATOR_H

#include <math.h>

class Modulators {
public:
    class Modulator {
        public:
            virtual float Modulate(float w, float t) const = 0;
    };

private:
    //"Piano" by iq
    class PianoModulator : public Modulator {
        virtual float Modulate(float w, float t) const
        {
            float y = 0.6*sin(1.0*w*t)*exp(-0.0008*w*t);
            y += 0.3*sin(2.0*w*t)*exp(-0.0010*w*t);
            y += 0.1*sin(4.0*w*t)*exp(-0.0015*w*t);
            y += 0.2*y*y*y;
            y *= 0.9 + 0.1*cos(70.0*t);
            y  = 2.0*y*exp(-22.0*t) + y;

            //Clamp y to 0..1
            y = y > 1.0 ? 1.0 : y;
            y = y < 0.0 ? 0.0 : y;

            return y;
        }
    };

    //"Guitar" by iq
    class GuitarModulator : public Modulator {
        virtual float Modulate(float w, float t) const
        {
            float f = cos(0.251*w*t);
            float y  = 0.5*cos(1.0*w*t+3.14*f)*exp(-0.0007*w*t);
            y += 0.2*cos(2.0*w*t+3.14*f)*exp(-0.0009*w*t);
            y += 0.2*cos(4.0*w*t+3.14*f)*exp(-0.0016*w*t);
            y += 0.1*cos(8.0*w*t+3.14*f)*exp(-0.0020*w*t);
            y *= 0.9 + 0.1*cos(70.0*t);
            y = 2.0*y*exp(-22.0*t) + y;

            //Clamp y to 0..1
            y = y > 1.0 ? 1.0 : y;
            y = y < 0.0 ? 0.0 : y;

            return y;
        }
    };

public:
    static const Modulator* GetPiano() {
        static PianoModulator piano;
        return &piano;
    }

    static const Modulator* GetGuitar()
    {
        static GuitarModulator guitar;
        return &guitar;
    }

};

#endif