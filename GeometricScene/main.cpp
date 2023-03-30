#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>


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
    
    virtual ~SceneElement() = default;
};


class Point : public SceneElement {
private:
    double x;
    double y;
    
public:
    Point()
    : x(0.0)
    , y(0.0)
    {};

    Point(const Point& a)
    : x(a.x)
    , y(a.y)
    {};

    Point(float a, float b)
    : x(a)
    , y(b)
    {};
    
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
        return Point(x * n, y * n);
    }
    
    friend std::ostream& operator << (std::ostream &out, const Point &point);
    
    friend std::istream& operator >> (std::istream &in, Point &point);
};

std::ostream& operator << (std::ostream &out, const Point &point)
{
    out << static_cast<int>(point.TypeId()) << " " << point.x << " " << point.y << " " ;
    return out;
}

std::istream& operator >> (std::istream &in, Point &point)
{
    char id;
    in >> id >> point.x >> point.y;
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
    
public:
    Line()
    : start()
    , end()
    {};

    Line(const Line& a) = default;

    Line(Point a, Point b)
    : start(a)
    , end(b)
    {};
    
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
    char id;
    in >> id >> line.start >> line.end;
    return in;
}


class Polyline : public Curve {
private:
    std::vector<Point> points;

public:
    Polyline()
    : points()
    {};

    Polyline(const Polyline& p) = default;

    Polyline(std::vector<Point> p)
    : points(p)
    {};

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
    char id;
    int size = 0;
    Point p;
    in >> id;
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

public:
    Ellipse()
    : c()
    , a()
    , b()
    {};

    Ellipse(const Ellipse& el) = default;

    Ellipse(Point _c, double _a, double _b)
    : c(_c)
    , a(_a)
    , b(_b)
    {};

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
    char id;
    in >> id >> el.c >> el.a >> el.b;
    return in;
}


class GeometricScene {
private:
    std::vector<std::shared_ptr<SceneElement>> items;
    size_t size;
public:
    
    void AddItem(SceneElement& it)
    {
        items.push_back(std::shared_ptr<SceneElement>(&it));
        size++;
    }
    
    void Write(std::ostream& out) const
    {
        out << size << std::endl;
        for (auto it : items)
        {
            it->Write(out);
            out << std::endl;
        }
    }
    
    void Read (std::istream& in)
    {
        int id, size;
        in >> size;
        for (int i = 0; i < size; ++i)
        {
            id = ReadItemID(in);
            ReadItemBody(in, static_cast<SceneTypeId>(id));
        }
    }
    
    int ReadItemID(std::istream& in)
    {
        int id;
        in >> id;
        return id;
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
};


int main(int argc, char* argv[])
{
    Point p (10, 10);
    Line l (Point(20, 10), Point(30, 50));
    Polyline pl (std::vector<Point>{ Point(20, 10), Point(30, 50), Point(30, 60) });
    Ellipse el (Point(10, 20), 11, 25);
    
    GeometricScene s;
    
    s.AddItem(* new Point(p));
    s.AddItem(* new Line(l));
    s.AddItem(* new Polyline(pl));
    s.AddItem(* new Ellipse(el));
    
    std::stringstream ss;
    s.Write(ss);
    s.Write(std::cout);
    std::cout << std::endl;
    
    GeometricScene s1;
    s1.Read(ss);
    s1.Write(std::cout);
    
    //std::cout << ss.str() << std::endl;
    std::cout << std::endl;
    return 0;
}

