#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>
#include <stdint.h>

enum class SceneTypeId
{
    Point = 0,
    Line,
    Polyline,
    Ellipse
};

class Curve;
class Line;
class Polyline;
class Ellipse;
class Point;


class Visitor
{
public:
    virtual void Visit(Point& p) = 0;
    
    virtual void Visit(Line& l) = 0;
    
    virtual void Visit(Curve& c) = 0;
    
    virtual void Visit(Ellipse& el) = 0;
    
    virtual void Visit(Polyline& pl) = 0;
};


class SceneElement {
public:
    virtual void Write (std::ostream&) const = 0;
    
    virtual void Read (std::istream&) = 0;
    
    virtual void ReadForScene (std::istream&) = 0;
    
    virtual SceneTypeId TypeId() const = 0;
    
    virtual void Accept(Visitor& v) = 0;
    
    virtual ~SceneElement() = default;
};


class Point : public SceneElement {
private:
    double x;
    double y;
    uint32_t color;
    
public:
    Point()
    : x(0.0)
    , y(0.0)
    , color(0x0000FF)
    {};

    Point(const Point& a)
    : x(a.x)
    , y(a.y)
    , color(a.color)
    {};

    Point(float a, float b, uint32_t c)
    : x(a)
    , y(b)
    , color(c)
    {};
    
    Point(float a, float b)
    : x(a)
    , y(b)
    , color(0x0000FF)
    {};
    
    virtual void Accept(Visitor& v) override
    {
        v.Visit(*this);
    }
    
    void Scale(float n)
    {
        x *= n;
        y *= n;
    }

    SceneTypeId TypeId () const override
    {
        return SceneTypeId::Point;
    }
    
    double GetX() const
    {
        return x;
    }
    
    void SetX(double _x)
    {
        x = _x;
    }
    
    double GetY() const
    {
        return y;
    }
    
    void SetY(double _y)
    {
        y = _y;
    }
    
    void Write (std::ostream& out) const override
    {
        out << *this;
    }
    
    void Read (std::istream& in) override
    {
        in >> *this;
    }
    
    void ReadForScene (std::istream& in) override
    {
        in >> x >> y;
    }
    
    bool operator == (const Point& to) const
    {
        return ((x == to.x) && (y == to.y));
    }
    
    Point operator + (const Point& p) const
    {
        return Point(x + p.x, y + p.y);
    }
    
    Point operator - (const Point& p) const
    {
        return Point(x - p.x, y - p.y);
    }

    Point operator * (const double& n) const
    {
        return Point (x * n, y * n);
    }
    
    friend std::ostream& operator << (std::ostream &out, const Point &point);
    
    friend std::istream& operator >> (std::istream &in, Point &point);
};

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


class Curve : public SceneElement {
public:
    virtual double GetTmin() const = 0;

    virtual double GetTmax() const = 0;

    virtual Point CalcFormula(double t) const = 0;
};


class Line : public Curve {
private:
    Point start;
    Point end;
    uint32_t color;
    
public:
    Line()
    : start()
    , end()
    {};

    Line(const Line& a) = default;

    Line(Point a, Point b, uint32_t c)
    : start(a)
    , end(b)
    , color(c)
    {};
    
    Line(Point a, Point b)
    : start(a)
    , end(b)
    , color(0x0000FF)
    {};
    
    Point GetPointStart() const
    {
        return start;
    }
    
    Point GetPointEnd() const
    {
        return end;
    }
    
    virtual void Accept(Visitor& v) override
    {
        v.Visit(*this);
    }
    
    SceneTypeId TypeId () const override
    {
        return SceneTypeId::Line;
    }

    double GetTmin() const override
    {
        return 0;
    }

    double GetTmax() const override
    {
        return 1;
    }

    Point CalcFormula(double t) const override
    {
        return start * (1 - t) + end * t;
    }

    void Scale(float n) {
        start.Scale(n);
        end.Scale(n);
    }

    bool operator == (const Line& to) const
    {
        return ((start == to.start) && (end == to.end));
    }

    double Lenght() const
    {
        return std::hypot(start.GetX() - end.GetX(), start.GetY() - end.GetY());
    }
    
    void Write(std::ostream& out) const override
    {
        out << *this;
    }
    
    void Read (std::istream& in) override
    {
        in >> *this;
    }
    
    void ReadForScene (std::istream& in) override
    {
        in >> start >> end;
    }
    
    friend std::ostream& operator << (std::ostream &out, const Line &line);
    
    friend std::istream& operator >> (std::istream &in, Line &line);
};

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


class Polyline : public Curve {
private:
    std::vector<Point> points;
    uint32_t color;
    
public:
    Polyline()
    : points()
    {};

    Polyline(const Polyline& p) = default;

    Polyline(std::vector<Point> p, uint32_t c)
    : points(p)
    , color(c)
    {};

    size_t GetPointsCount() const
    {
        return points.size();
    }
    
    Point GetPoint(size_t index) const
    {
        return points[index];
    }
    
    virtual void Accept(Visitor& v) override
    {
        v.Visit(*this);
    }
    
    SceneTypeId TypeId () const override
    {
        return SceneTypeId::Polyline;
    }
    
    double GetTmin() const override
    {
        return 0.0;
    }

    double GetTmax() const override
    {
        return 0.0;
    }
    
    Point CalcFormula(double t) const override
    {
        return Point();
    }

    void Scale(double n)
    {
        for (auto& i : points)
        {
            i.Scale(n);
        }
    }
    
    void Write(std::ostream& out) const override
    {
        out << *this;
    }
    
    void Read (std::istream& in) override
    {
        in >> *this;
    }
    
    void ReadForScene (std::istream& in) override
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
    
    friend std::ostream& operator << (std::ostream &out, const Polyline &pl);
    
    friend std::istream& operator >> (std::istream &in, Polyline &pl);
};

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


class Ellipse : public Curve {
private:
    Point c;
    double a, b;
    uint32_t color;
    
public:
    Ellipse()
    : c()
    , a()
    , b()
    , color()
    {};

    Ellipse(const Ellipse& el) = default;

    Ellipse(Point _c, double _a, double _b, uint32_t c)
    : c(_c)
    , a(_a)
    , b(_b)
    , color(c)
    {};

    double GetA()
    {
        return a;
    }
    
    double GetB()
    {
        return b;
    }
    
    Point GetCenter()
    {
        return c;
    }
    
    virtual void Accept(Visitor& v) override
    {
        v.Visit(*this);
    }
    
    SceneTypeId TypeId () const override
    {
        return SceneTypeId::Ellipse;
    }
    
    double GetTmin() const override
    {
        return 0;
    }

    double GetTmax() const override
    {
        return 2 * 3.14;
    }

    Point CalcFormula(double t) const override
    {
        return Point(cos(t) * a + c.GetX(), sin(t) * b + c.GetY());
    }
    
    void Write(std::ostream& out) const override
    {
        out << *this;
    }
    
    void Read (std::istream& in) override
    {
        in >> *this;
    }
    
    void ReadForScene (std::istream& in) override
    {
        in >> c >> a >> b;
    }
    
    friend std::istream& operator >> (std::istream &in, Ellipse &el);
    
    friend std::ostream& operator << (std::ostream &out, const Ellipse &el);
};

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


class GeometricScene {
private:
    std::vector<std::shared_ptr<SceneElement>> items;
    int index { 0 };
    
public:
    void AddItem(SceneElement& it)
    {
        items.push_back(std::shared_ptr<SceneElement>(&it));
    }
    
    void Write(std::ostream& out) const
    {
        out << items.size() << std::endl;
        for (auto it : items)
        {
            it->Write(out);
            out << std::endl;
        }
    }
    
    void Read (std::istream& in)
    {
        int size { 0 };
        in >> size;
        for (int i = 0; i < size; ++i)
        {
            ReadItemBody(in, ReadItemID(in));
        }
    }
    
    SceneTypeId ReadItemID(std::istream& in)
    {
        int id;
        in >> id;
        return static_cast<SceneTypeId>(id);
    }
    
    void ReadItemBody(std::istream& in, SceneTypeId id)
    {
        Point p;
        Line l;
        Polyline pl;
        Ellipse el;
        
        switch (id)
        {
            case SceneTypeId::Point:
                p.ReadForScene(in);
                AddItem(* new Point(p));
                break;
                
            case SceneTypeId::Line:
                l.ReadForScene(in);
                AddItem(* new Line(l));
                break;
                
            case SceneTypeId::Ellipse:
                el.ReadForScene(in);
                AddItem(* new Ellipse(el));
                break;
                
            case SceneTypeId::Polyline:
                pl.ReadForScene(in);
                AddItem(* new Polyline(pl));
                break;
                
            default:
                break;
        }
    }
    
    //Итератор
    std::shared_ptr<SceneElement> Iterator()
    {
        if (!items.empty())
        {
            return items[index];
        }
        
        return nullptr;
    }
    
    bool First()
    {
        if (items.empty())
        {
            return false;
        }
        
        index = 0;
        return true;
    }
    
    bool Next()
    {
        if (IsDone())
        {
            return true;
        }
        
        index++;
        return false;
    }
        
    bool IsDone()
    {
        return index == items.size();
    }
    
};


struct LengthSumVisitor : public Visitor
{
    double total_length { 0 };
    
    void Visit(Point &p) override {}
    
    void Visit(Curve &c) override {}
    
    void Visit(Line &l) override
    {
        total_length += std::hypot(l.GetPointStart().GetX() - l.GetPointEnd().GetX(),
                                   l.GetPointStart().GetY() - l.GetPointEnd().GetY());
    }
    
    void Visit(Ellipse &el) override
    {
        Point p;
        const double delta_t = el.GetTmax() - el.GetTmin() / 100;
        for(double t = el.GetTmin(); t < el.GetTmax(); t += delta_t)
        {
            p = el.CalcFormula(t);
            total_length += std::hypot(p.GetX() - (p.GetX() + delta_t), p.GetY() - (p.GetY() + delta_t));
        }
    }
    
    void Visit(Polyline &pl) override
    {
        for(int i = 0; i < pl.GetPointsCount() - 1; ++i)
        {
            total_length += std::hypot(pl.GetPoint(i).GetX() - pl.GetPoint(i + 1).GetX(),
                                       pl.GetPoint(i).GetY() - pl.GetPoint(i + 1).GetY());
        }
    }
};


int main(int argc, char* argv[])
{
    Point p (10, 10, 0x808000);
    Line l (Point(20, 10), Point(30, 50), 0x0000FF);
    Polyline pl (std::vector<Point>{ Point(20, 10), Point(30, 50) }, 0x00FF00);
    Ellipse el (Point(10, 20), 11, 25, 0x808000);
    
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

