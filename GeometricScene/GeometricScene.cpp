#include "GeometricScene.hpp"
#include "LengthSumVisitor.hpp"


void GeometricScene::AddItem(SceneElement& it)
{
    items.push_back(std::shared_ptr<SceneElement>(&it));
}

void GeometricScene::Write(std::ostream& out) const
{
    out << items.size() << std::endl;
    for (auto it : items)
    {
        it->Write(out);
        out << std::endl;
    }
}

void GeometricScene::Read (std::istream& in)
{
    int size { 0 };
    in >> size;
    for (int i = 0; i < size; ++i)
    {
        ReadItemBody(in, ReadItemID(in));
    }
}

SceneTypeId GeometricScene::ReadItemID(std::istream& in)
{
    int id;
    in >> id;
    return static_cast<SceneTypeId>(id);
}

void GeometricScene::ReadItemBody(std::istream& in, SceneTypeId id)
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
std::shared_ptr<SceneElement> GeometricScene::Iterator()
{
    if (!items.empty())
    {
        return items[index];
    }
    
    return nullptr;
}

bool GeometricScene::First()
{
    if (items.empty())
    {
        return false;
    }
    
    index = 0;
    return true;
}

bool GeometricScene::Next()
{
    if (IsDone())
    {
        return true;
    }
    
    index++;
    return false;
}
    
bool GeometricScene::IsDone()
{
    return index == items.size();
}
