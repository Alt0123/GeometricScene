#ifndef Shape_hpp
#define Shape_hpp

#include <stdio.h>
#include "SceneElement.hpp"

class Point : public SceneElement {
private:
    double x;
    double y;
    uint32_t color;
    
public:
    Point();

    Point(const Point& a);

    Point(float a, float b, uint32_t c);
    
    Point(float a, float b);
    
    void Accept(Visitor& v) override;
    
    void Scale(float n);

    SceneTypeId TypeId () const override;
    
    double GetX() const;
    
    void SetX(double _x);
    
    double GetY() const;
    
    void SetY(double _y);
    
    void Write (std::ostream& out) const override;
    
    void Read (std::istream& in) override;
    
    void ReadForScene (std::istream& in) override;
    
    bool operator == (const Point& to) const;
    
    Point operator + (const Point& p) const;
    
    Point operator - (const Point& p) const;

    Point operator * (const double& n) const;
    
    friend std::ostream& operator << (std::ostream &out, const Point &point);
    
    friend std::istream& operator >> (std::istream &in, Point &point);
};


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
    Line();

    Line(const Line& a) = default;

    Line(Point a, Point b, uint32_t c);
    
    Line(Point a, Point b);
    
    Point GetPointStart() const;
    
    Point GetPointEnd() const;
    
    void Accept(Visitor& v) override;
    
    SceneTypeId TypeId () const override;

    double GetTmin() const override;

    double GetTmax() const override;

    Point CalcFormula(double t) const override;

    void Scale(float n);

    bool operator == (const Line& to) const;

    double Lenght() const;
    
    void Write(std::ostream& out) const override;
    
    void Read (std::istream& in) override;
    
    void ReadForScene (std::istream& in) override;
    
    friend std::ostream& operator << (std::ostream &out, const Line &line);
    
    friend std::istream& operator >> (std::istream &in, Line &line);
};




class Polyline : public Curve {
private:
    std::vector<Point> points;
    uint32_t color;
    
public:
    Polyline();

    Polyline(const Polyline& p) = default;

    Polyline(std::vector<Point> p, uint32_t c);

    size_t GetPointsCount() const;
    
    Point GetPoint(size_t index) const;
    
    void Accept(Visitor& v) override;

    SceneTypeId TypeId () const override;
    
    double GetTmin() const override;

    double GetTmax() const override;
    
    Point CalcFormula(double t) const override;

    void Scale(double n);
    
    void Write(std::ostream& out) const override;
    
    void Read (std::istream& in) override;
    
    void ReadForScene (std::istream& in) override;
    
    friend std::ostream& operator << (std::ostream &out, const Polyline &pl);
    
    friend std::istream& operator >> (std::istream &in, Polyline &pl);
};




class Ellipse : public Curve {
private:
    Point c;
    double a, b;
    uint32_t color;
    
public:
    Ellipse();

    Ellipse(const Ellipse& el) = default;

    Ellipse(Point _c, double _a, double _b, uint32_t c);

    double GetA();
    
    double GetB();
    
    Point GetCenter();
    
    void Accept(Visitor& v) override;
    
    SceneTypeId TypeId () const override;
    
    double GetTmin() const override;

    double GetTmax() const override;

    Point CalcFormula(double t) const override;
    
    void Write(std::ostream& out) const override;
    
    void Read (std::istream& in) override;
    
    void ReadForScene (std::istream& in) override;
    
    friend std::istream& operator >> (std::istream &in, Ellipse &el);
    
    friend std::ostream& operator << (std::ostream &out, const Ellipse &el);
};

#endif /* Shape_hpp */
