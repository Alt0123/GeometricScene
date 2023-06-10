#ifndef SceneElement_hpp
#define SceneElement_hpp

#include <stdio.h>
#include <iostream>

class Visitor;

enum class SceneTypeId
{
    Point = 0,
    Line,
    Polyline,
    Ellipse
};

class SceneElement {
public:
    virtual void Write (std::ostream&) const = 0;
    
    virtual void Read (std::istream&) = 0;
    
    virtual void ReadForScene (std::istream&) = 0;
    
    virtual SceneTypeId TypeId() const = 0;
    
    virtual uint32_t GetColor() = 0;
    
    virtual void Accept(Visitor& v) = 0;
    
    virtual ~SceneElement() = default;
};

#endif /* SceneElement_hpp */
