#include "Light.h"

Light::Light(int _number, ptree& _config) {
    id = _number;
    switch (_number) {
        case 0:
            name = GL_LIGHT0;
            break;
        case 1:
            name = GL_LIGHT1;
            break;
        case 2:
            name = GL_LIGHT2;
            break;
        case 3:
            name = GL_LIGHT3;
            break;
        case 4:
            name = GL_LIGHT4;
            break;
        case 5:
            name = GL_LIGHT5;
            break;
        case 6:
            name = GL_LIGHT6;
            break;
        case 7:
            name = GL_LIGHT7;
            break;
    }
    try {
        string _name = lexical_cast<string>(_number);
        loadConfig(_name, _config);
    } catch (bad_lexical_cast exception) {
        cerr << "Light " << id << "::constructor -> Lexical Cast." << endl;
        exit(1);
    }
}

Light::~Light(void) {
}

void Light::switchOn(void) {
    glEnable(name);
    cout << "rozsviceno";
    state = true;
}

void Light::switchOff(void) {
    glDisable(name);
    cout << "zhasnuto";
    state = false;
}

void Light::setAmbient(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) {
    ambient[0] = _r;
    ambient[1] = _g;
    ambient[2] = _b;
    ambient[3] = _a;
    glLightfv(name, GL_AMBIENT, ambient);
}

void Light::setDiffuse(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) {
    diffuse[0] = _r;
    diffuse[1] = _g;
    diffuse[2] = _b;
    diffuse[3] = _a;
    glLightfv(name, GL_DIFFUSE, diffuse);
}

void Light::setSpecular(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) {
    specular[0] = _r;
    specular[1] = _g;
    specular[2] = _b;
    specular[3] = _a;
    glLightfv(name, GL_SPECULAR, specular);
}

void Light::setPosition(GLfloat _x, GLfloat _y, GLfloat _z, GLfloat _w) {
    position[0] = _x;
    position[1] = _y;
    position[2] = _z;
    position[3] = _w;
    glLightfv(name, GL_POSITION, position);
}

void Light::rePosition(void) {
    glLightfv(name, GL_POSITION, position);
}

void Light::setSpotDirection(GLfloat _x, GLfloat _y, GLfloat _z) {
    spotDirection[0] = _x;
    spotDirection[1] = _y;
    spotDirection[2] = _z;
    glLightfv(name, GL_SPOT_DIRECTION, spotDirection);
}

void Light::reSpotDirection(void) {
    glLightfv(name, GL_SPOT_DIRECTION, spotDirection);
}

void Light::setSpotExponent(GLfloat _exponent) {
    spotExponent = _exponent;
    glLightf(name, GL_SPOT_EXPONENT, spotExponent);
}

void Light::setSpotCutOff(GLfloat _cutOff) {
    spotCutOff = _cutOff;
    glLightf(name, GL_SPOT_CUTOFF, spotCutOff);
}

void Light::setAttenuation(GLfloat _constant, GLfloat _linear, GLfloat _quadratic) {
    attenuation[0] = _constant;
    attenuation[1] = _linear;
    attenuation[2] = _quadratic;

    glLightf(name, GL_CONSTANT_ATTENUATION, attenuation[0]);
    glLightf(name, GL_LINEAR_ATTENUATION, attenuation[1]);
    glLightf(name, GL_QUADRATIC_ATTENUATION, attenuation[2]);
}

bool Light::getState(void) {
    return state;
}

void Light::loadConfig(const string& _name, ptree& _config) {
    string path = "config.lights.light-" + _name + ".";    
    try {
        // Nastavi se promenne svetlu.
        dynamic = _config.get<bool>(path + "dynamic");
        state = _config.get<bool>(path + "state");
        if (state == true) {
            switchOn();
        } else {
            switchOff();
        }
        setAmbient(
                _config.get<float>(path + "ambient.r"),
                _config.get<float>(path + "ambient.g"),
                _config.get<float>(path + "ambient.b"),
                _config.get<float>(path + "ambient.a")
                );
        setDiffuse(
                _config.get<float>(path + "diffuse.r"),
                _config.get<float>(path + "diffuse.g"),
                _config.get<float>(path + "diffuse.b"),
                _config.get<float>(path + "diffuse.a")
                );
        setSpecular(
                _config.get<float>(path + "specular.r"),
                _config.get<float>(path + "specular.g"),
                _config.get<float>(path + "specular.b"),
                _config.get<float>(path + "specular.a")
                );
        setPosition(
                _config.get<float>(path + "position.x"),
                _config.get<float>(path + "position.y"),
                _config.get<float>(path + "position.z"),
                _config.get<float>(path + "position.w")
                );
        setSpotDirection(
                _config.get<float>(path + "spot-direction.x"),
                _config.get<float>(path + "spot-direction.y"),
                _config.get<float>(path + "spot-direction.z")
                );
        setSpotExponent(_config.get<float>(path + "spot-exponent"));
        setSpotCutOff(_config.get<float>(path + "spot-cutoff"));
        setAttenuation(
                _config.get<float>(path + "attenuation.constant"),
                _config.get<float>(path + "attenuation.linear"),
                _config.get<float>(path + "attenuation.quadratic")
                );
    } catch (ptree_bad_path exception) {
        cerr << "Light " + _name + "::loadConfig -> PTree Bad Path." << endl << exception.what() << endl;
        exit(1);
    } catch (ptree_bad_data exception) {
        cerr << "Light " + _name + "::loadConfig -> PTree Bad Data." << endl << exception.what() << endl;
        exit(1);
    }
}

ostream & operator<<(ostream& os, Light& light) {
    os << "Light " << light.id << endl;
    os << "- ambient(" << light.ambient[0] << ", " << light.ambient[1] << ", " << light.ambient[2] << ", " << light.ambient[3] << ")" << endl;
    os << "- diffuse(" << light.diffuse[0] << ", " << light.diffuse[1] << ", " << light.diffuse[2] << ", " << light.diffuse[3] << ")" << endl;
    os << "- specular(" << light.specular[0] << ", " << light.specular[1] << ", " << light.specular[2] << ", " << light.specular[3] << ")" << endl;
    os << "- position(" << light.position[0] << ", " << light.position[1] << ", " << light.position[2] << ", " << light.position[3] << ")" << endl;
    os << "- spotDirection(" << light.spotDirection[0] << ", " << light.spotDirection[1] << ", " << light.spotDirection[2] << ", " << light.spotDirection[3] << ")" << endl;
    os << "- spotExponent(" << light.spotExponent << ")" << endl;
    os << "- spotCutOff(" << light.spotCutOff << ")" << endl;
    os << "- attenuation(" << light.attenuation[0] << ", " << light.attenuation[1] << ", " << light.attenuation[2] << ")" << endl;
    os << "- state(" << light.state << ")" << endl;
    os << endl;
    return os;
}

