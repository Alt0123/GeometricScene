#include "LengthSumVisitor.hpp"

void LengthSumVisitor::Visit(Point &p){}

void LengthSumVisitor::Visit(Curve &c){}

void LengthSumVisitor::Visit(Line &l)
{
    total_length += std::hypot(l.GetPointStart().GetX() - l.GetPointEnd().GetX(),
                               l.GetPointStart().GetY() - l.GetPointEnd().GetY());
}

void LengthSumVisitor::Visit(Ellipse &el)
{
    Point p1;
    Point p2;
    const double delta_t = (el.GetTmax() - el.GetTmin()) / 100;
    for(double t = el.GetTmin(); t < el.GetTmax(); t += delta_t)
    {
        p1 = el.CalcFormula(t);
        p2 = el.CalcFormula(t + delta_t);
        total_length += std::hypot(p1.GetX() - p2.GetX(), p1.GetY() - p2.GetY());
    }
}

void LengthSumVisitor::Visit(Polyline &pl)
{
    for(int i = 0; i < pl.GetPointsCount() - 1; ++i)
    {
        total_length += std::hypot(pl.GetPoint(i).GetX() - pl.GetPoint(i + 1).GetX(),
                                   pl.GetPoint(i).GetY() - pl.GetPoint(i + 1).GetY());
    }
}
