#ifndef LIGHT_H
#define	LIGHT_H

#include <GL/gl.h>

#include <string>

using namespace std;

class Light {
public:
    /**
     * Vytvori nove svetlo.
     */
    Light(string& _name);

    /**
     * Zdestruuje svetlo.
     */
    virtual ~Light(void);
private:

    /** Oznaceni svetla. */
    string name;


};

#endif	/* LIGHT_H */

