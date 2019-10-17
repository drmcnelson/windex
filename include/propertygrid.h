#pragma once
#include "wex.h"
namespace wex
{
/// A name value pair
class property
{
public:
    property(
        window& parent,
        const std::string& name,
        const std::string& value )
        : myName( name )
        , myValue( value )
        , W( windex::get())
        , myLabel( W.make<label>(parent) )
        , myEditbox( W.make<editbox>(parent) )
        , myCombobox( W.make<choice>(parent) )
        , myLabelWidth( 100 )
        , myType( eType::string )
    {
        myLabel.text( myName );
        myEditbox.text( myValue );
    }
    property(
        window& parent,
        const std::string& name,
        const std::vector< std::string >& value )
        : myName( name )
        , myValue( "" )
        , W( windex::get())
        , myLabel( W.make<label>(parent) )
        , myEditbox( W.make<editbox>(parent) )
        , myCombobox( W.make<choice>(parent) )
        , myLabelWidth( 100 )
        , myType( eType::choice )
    {
        myLabel.text( myName );
        for( auto& t : value )
        {
            myCombobox.add( t );
        }
    }
    void move( const std::vector<int>& r )
    {
        std::vector<int> rl( r );
        rl[2] = myLabelWidth;
        myLabel.move( rl );

        std::vector<int> re( r );
        re[0] += myLabelWidth;
        re[2] -= myLabelWidth;
        switch( myType )
        {
        case eType::string:
            myEditbox.move( re );
            break;
        case eType::choice:
            re[3] *= myCombobox.count();
            myCombobox.move( re );
            break;
        }
    }
    void labelWidth( int w )
    {
        myLabelWidth = w;
    }
    void bgcolor( int color )
    {
        myLabel.bgcolor( color );
    }
    const std::string& name() const
    {
        return myName;
    }
    const std::string value() const
    {
        switch( myType )
        {
        case eType::string:
            return myEditbox.text();

        case eType::choice:
            return myCombobox.SelectedText();
        }
        return std::string("");
    }

    // copy value from gui into myValue attribute
    void saveValue()
    {
        switch( myType )
        {
        case eType::string:
            myValue = myEditbox.text();
            break;
        case eType::choice:
            myValue = myCombobox.SelectedText();
            break;
        }
    }

    // get myValue attribute
    const std::string savedValue() const
    {
        return myValue;
    }
private:
    std::string myName;
    std::string myValue;
    wex::windex& W;
    wex::label& myLabel;
    wex::editbox& myEditbox;
    wex::choice& myCombobox;
    int myLabelWidth;
    enum class eType
    {
        string,
        choice
    }
    myType;
};
/// A grid of properties
class propertyGrid
{
public:
    propertyGrid( window& parent )
        : myParent( parent )
        , myHeight( 30 )
        , myWidth( 300 )
        , myLabelWidth( 100 )
        , myX( 10 )
        , myY( 10 )
        , myBGColor( 0xc8c8c8)
    {

    }
    void string(
        const std::string& name,
        const std::string& value )
    {
        property P( myParent, name, value );
        P.labelWidth( myLabelWidth );
        P.bgcolor( myBGColor );
        P.move( { myX, myY+(int)myProperty.size() * myHeight,
                  myWidth, myHeight
                } );
        myProperty.push_back( P );
    }
    void choice(
        const std::string& name,
        const std::vector< std::string >& choice )
    {
        property P( myParent, name, choice );
        P.labelWidth( myLabelWidth );
        P.bgcolor( myBGColor );
        P.move(
        {
            myX, myY+(int)myProperty.size() * myHeight,
            myWidth, myHeight
        } );
        myProperty.push_back( P );
    }
    void move( const std::vector<int>& r )
    {
        myX = r[0];
        myY = r[1];
        myWidth = r[2];
    }
    void labelWidth( int w )
    {
        myLabelWidth = w;
    }
    void bgcolor( int color )
    {
        myBGColor = color;
    }

    /// get pointer to property with name
    property* find( const std::string& name )
    {
        for( auto& p : myProperty )
        {
            if( p.name() == name )
                return &p;
        }
        return nullptr;
    }

    /// get value in editbox of property with name
    const std::string value( const std::string& name )
    {
        std::cout << "PG value " << name << "\n";
        property* p = find( name );
        if( ! p )
        {
            static std::string null;
            return null;
        }
        std::string v = p->value();
        return p->value();
    }

    /// save values in all property textboxes in the property's myValue attribute
    void saveValues()
    {
        for( auto& p : myProperty )
        {
            p.saveValue();
        }
    }
private:
    std::vector< property > myProperty;
    window& myParent;
    int myHeight;               // height of a single property
    int myWidth;
    int myLabelWidth;
    int myX, myY;
    int myBGColor;
};
}
