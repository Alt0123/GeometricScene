#include "Shape.hpp"
#include "LengthSumVisitor.hpp"
//---------------------
//Point
//---------------------
Point::Point()
: x(0.0)
, y(0.0)
, color(0x0000FF)
{};

Point::Point(const Point& a)
: x(a.x)
, y(a.y)
, color(a.color)
{};

Point::Point(float a, float b, uint32_t c)
: x(a)
, y(b)
, color(c)
{};

Point::Point(float a, float b)
: x(a)
, y(b)
, color(0x0000FF)
{};

void Point::Accept(Visitor& v)
{
    v.Visit(*this);
}

void Point::Scale(float n)
{
    x *= n;
    y *= n;
}

SceneTypeId Point::TypeId () const
{
    return SceneTypeId::Point;
}

double Point::GetX() const
{
    return x;
}

void Point::SetX(double _x)
{
    x = _x;
}

double Point::GetY() const
{
    return y;
}

void Point::SetY(double _y)
{
    y = _y;
}

void Point::Write (std::ostream& out) const
{
    out << *this;
}

void Point::Read (std::istream& in)
{
    in >> *this;
}

void Point::ReadForScene (std::istream& in)
{
    in >> x >> y;
}

bool Point::operator == (const Point& to) const
{
    return ((x == to.x) && (y == to.y));
}

Point Point::operator + (const Point& p) const
{
    return Point(x + p.x, y + p.y);
}

Point Point::operator - (const Point& p) const
{
    return Point(x - p.x, y - p.y);
}

Point Point::operator * (const double& n) const
{
    return Point (x * n, y * n);
}

std::ostream& operator << (std::ostream &out, const Point &point)
{
    out << static_cast<int>(point.TypeId()) << " " << point.x << " " << point.y << " ";
    return out;
}

std::istream& operator >> (std::istream &in, Point &point)
{
    in.ignore(2);
    in >> point.x >> point.y;
    return in;
}

//---------------------
//Line
//---------------------
Line::Line()
: start()
, end()
{};

Line::Line(Point a, Point b, uint32_t c)
: start(a)
, end(b)
, color(c)
{};

Line::Line(Point a, Point b)
: start(a)
, end(b)
, color(0x0000FF)
{};

Point Line::GetPointStart() const
{
    return start;
}

Point Line::GetPointEnd() const
{
    return end;
}

void Line::Accept(Visitor& v)
{
    v.Visit(*this);
}

SceneTypeId Line::TypeId () const
{
    return SceneTypeId::Line;
}

double Line::GetTmin() const
{
    return 0;
}

double Line::GetTmax() const
{
    return 1;
}

Point Line::CalcFormula(double t) const
{
    return start * (1 - t) + end * t;
}

void Line::Scale(float n) {
    start.Scale(n);
    end.Scale(n);
}

bool Line::operator == (const Line& to) const
{
    return ((start == to.start) && (end == to.end));
}

double Line::Lenght() const
{
    return std::hypot(start.GetX() - end.GetX(), start.GetY() - end.GetY());
}

void Line::Write(std::ostream& out) const
{
    out << *this;
}

void Line::Read (std::istream& in)
{
    in >> *this;
}

void Line::ReadForScene (std::istream& in)
{
    in >> start >> end;
}
std::ostream& operator << (std::ostream &out, const Line &line)
{
    out << static_cast<int>(line.TypeId())<< " " << line.start << line.end;
    return out;
}

std::istream& operator >> (std::istream &in, Line &line)
{
    in.ignore(2);
    in >> line.start >> line.end;
    return in;
}

//---------------------
//Polyline
//---------------------

Polyline::Polyline()
: points()
{};

Polyline::Polyline(std::vector<Point> p, uint32_t c)
: points(p)
, color(c)
{};

size_t Polyline::GetPointsCount() const
{
    return points.size();
}

Point Polyline::GetPoint(size_t index) const
{
    return points[index];
}

void Polyline::Accept(Visitor& v)
{
    v.Visit(*this);
}

SceneTypeId Polyline::TypeId () const
{
    return SceneTypeId::Polyline;
}

double Polyline::GetTmin() const
{
    return 0.0;
}

double Polyline::GetTmax() const
{
    return 0.0;
}

Point Polyline::CalcFormula(double t) const
{
    return Point();
}

void Polyline::Scale(double n)
{
    for (auto& i : points)
    {
        i.Scale(n);
    }
}

void Polyline::Write(std::ostream& out) const
{
    out << *this;
}

void Polyline::Read(std::istream& in)
{
    in >> *this;
}

void Polyline::ReadForScene(std::istream& in)
{
    int size = 0;
    Point p;
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> p;
        points.push_back(p);
    }
}
std::ostream& operator << (std::ostream &out, const Polyline &pl)
{
    out << static_cast<int>(pl.TypeId()) << ' ';
    out << pl.points.size() << ' ';
    for (auto i : pl.points)
    {
        out << i;
    }
    return out;
}

std::istream& operator >> (std::istream &in, Polyline &pl)
{
    int size = 0;
    Point p;
    in.ignore(2);
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        in >> p;
        pl.points.push_back(p);
    }
    return in;
}

//---------------------
//Ellipse
//---------------------

Ellipse::Ellipse()
: c()
, a()
, b()
, color()
{};

Ellipse::Ellipse(Point _c, double _a, double _b, uint32_t c)
: c(_c)
, a(_a)
, b(_b)
, color(c)
{};

double Ellipse::GetA()
{
    return a;
}

double Ellipse::GetB()
{
    return b;
}

Point Ellipse::GetCenter()
{
    return c;
}

void Ellipse::Accept(Visitor& v) 
{
    v.Visit(*this);
}

SceneTypeId Ellipse::TypeId () const
{
    return SceneTypeId::Ellipse;
}

double Ellipse::GetTmin() const
{
    return 0;
}

double Ellipse::GetTmax() const
{
    return 2 * 3.14;
}

Point Ellipse::CalcFormula(double t) const
{
    return Point(cos(t) * a + c.GetX(), sin(t) * b + c.GetY());
}

void Ellipse::Write(std::ostream& out) const
{
    out << *this;
}

void Ellipse::Read (std::istream& in)
{
    in >> *this;
}

void Ellipse::ReadForScene (std::istream& in)
{
    in >> c >> a >> b;
}


std::ostream& operator << (std::ostream &out, const Ellipse &el)
{
    out << static_cast<int>(el.TypeId()) << " " << el.c << el.a << " " << el.b << " ";
    return out;
}

std::istream& operator >> (std::istream &in, Ellipse &el)
{
    in.ignore(2);
    in >> el.c >> el.a >> el.b;
    return in;
}
