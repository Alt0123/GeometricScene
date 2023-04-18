#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

#include "SceneElement.hpp"
#include "LengthSumVisitor.hpp"
#include "GeometricScene.hpp"


int main(int argc, char* argv[])
{
    Point p (10, 10, 0x808000);
    Line l (Point(20, 10), Point(30, 50), 0x0000FF);
    Polyline pl (std::vector<Point>{ Point(20, 10), Point(30, 50) }, 0x00FF00);
    Ellipse el (Point(10, 20), 1, 1, 0x808000);
    
    GeometricScene s;
    
//
//    s.AddItem(* new Point(p));
//    s.AddItem(* new Line(l));
//    s.AddItem(* new Polyline(pl));
    s.AddItem(* new Ellipse(el));

    LengthSumVisitor v;

    for (s.First(); !s.IsDone(); s.Next())
    {
        s.Iterator()->Accept(v);
    }
    
    std::cout << v.total_length << std::endl;
    
//    std::stringstream ss;
//    s.Write(std::cout);
//    s.Write(ss);
//    std::cout << std::endl;
//    std::cout << ss.str() << std::endl;
//
//    GeometricScene s1;
//    s1.Read(ss);
//    s1.Write(std::cout);
//    std::cout << std::endl;
    
    return 0;
}

