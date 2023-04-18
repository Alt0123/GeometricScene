#ifndef LengthSumVisitor_hpp
#define LengthSumVisitor_hpp

#include <stdio.h>

#include "Shape.hpp"

class Visitor
{
public:
    virtual void Visit(Point& p) = 0;
    
    virtual void Visit(Line& l) = 0;
    
    virtual void Visit(Curve& c) = 0;
    
    virtual void Visit(Ellipse& el) = 0;
    
    virtual void Visit(Polyline& pl) = 0;
};



struct LengthSumVisitor : public Visitor
{
    double total_length { 0 };
    
    void Visit(Point &p) override;
    
    void Visit(Curve &c) override;
    
    void Visit(Line &l) override;
    
    void Visit(Ellipse &el) override;
    
    void Visit(Polyline &pl) override;
};

#endif /* LengthSumVisitor_hpp */
