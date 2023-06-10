#include <sstream>

#include "RunDemo.hpp"
#include "SceneElement.hpp"
#include "LengthSumVisitor.hpp"
#include "GeometricScene.hpp"

void RunDemo()
{
    Point p (10, 10, 0xFFFFFF);
    Line l (Point(20, 10), Point(30, 50), 0x0000FF);
    Polyline pl (std::vector<Point>{ Point(20, 10), Point(30, 50) }, 0x808000);
    Ellipse el (Point(10, 20), 1, 1, 0x00FF00);
    
    GeometricScene s;
    
    s.AddItem(* new Point(p));
    s.AddItem(* new Line(l));
    s.AddItem(* new Polyline(pl));
    s.AddItem(* new Ellipse(el));

    LengthSumVisitor v;

    for (s.First(); !s.IsDone(); s.Next())
    {
        s.Iterator()->Accept(v);
    }
    
    std::cout << v.total_length << std::endl;
    
    std::stringstream ss;
    s.Write(ss);
    std::cout << std::endl;
    std::cout << ss.str() << std::endl;

    GeometricScene s1;
    s1.Read(ss);
    std::sort(s1.begin(), s1.end(), [](std::shared_ptr<SceneElement> el1, std::shared_ptr<SceneElement> el2)
    {
        return el1->GetColor() < el2->GetColor();
    });
    
    s1.Write(std::cout);
    std::cout << std::endl;
}
