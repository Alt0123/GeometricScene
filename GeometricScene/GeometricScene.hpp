#ifndef GeometricScene_hpp
#define GeometricScene_hpp

#include <stdio.h>
#include <vector>

#include "SceneElement.hpp"
#include "Shape.hpp"

class GeometricScene {
private:
    std::vector<std::shared_ptr<SceneElement>> items;
    int index { 0 };
    
public:
    void AddItem(SceneElement& it);
    
    void Write(std::ostream& out) const;
    
    void Read (std::istream& in);
    
    SceneTypeId ReadItemID(std::istream& in);
    
    void ReadItemBody(std::istream& in, SceneTypeId id);
    
    //Итератор
    std::shared_ptr<SceneElement> Iterator();
    
    bool First();
    
    bool Next();
        
    bool IsDone();
};
#endif /* GeometricScene_hpp */
