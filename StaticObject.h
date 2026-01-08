#pragma once
#include "WorldObject.h"

class StaticObject : public WorldObject {
public:
    // Constructor
    StaticObject(int x, int y, std::string id, char glyph);

    virtual ~StaticObject();

    void update() override;
};