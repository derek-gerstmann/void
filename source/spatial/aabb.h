
#ifndef __AABB_H__
#define __AABB_H__

#include "compute_types.h"
#include "compute_math.h"

#include <OpenGL/gl.h>

template<class PT>
class AxisAlignedBox
{
public:
    typedef PT                      PointType;
    typedef PointType::Dimensions   Dimensions;

    VD_DECLARE_ENUM(OverlapType,
        Inside,
        Intersects,
        Outside);

    AxisAlignedBox( void ) :
        MinPoint(+VD_F32_MAX, +VD_F32_MAX, +VD_F32_MAX),
        MaxPoint(-VD_F32_MAX, -VD_F32_MAX, -VD_F32_MAX)
    {
        // EMPTY!
    }

    AxisAlignedBox( const AxisAlignedBox &a ):
        MinPoint( a.MinPoint ),
        MaxPoint( a.MaxPoint )
    {
        // EMPTY!
    }

    AxisAlignedBox(
        const PointType& minimum,
        const PointType& maximum
    ) :
        MinPoint(minimum),
        MaxPoint(maximum)
    {
        // EMPTY!
    }
        
    AxisAlignedBox( const PointType &rkCenter, const RealType fRadius ):
        MinPoint( rkCenter.x - fRadius, rkCenter.y - fRadius, rkCenter.z - fRadius ),
        MaxPoint( rkCenter.x + fRadius, rkCenter.y + fRadius, rkCenter.z + fRadius )
    {
        // EMPTY!
    }
    
    bool operator == ( const AxisAlignedBox &a ) const
    {
        return !( *this != a );
    }
    
    bool operator != ( const AxisAlignedBox &a ) const
    {
        return ( (( MinPoint.x - a.MinPoint.x ) != 0.0f) || (( MaxPoint.x - a.MaxPoint.x ) != 0.0f) ||
                 (( MinPoint.y - a.MinPoint.y ) != 0.0f) || (( MaxPoint.y - a.MaxPoint.y ) != 0.0f) ||
                 (( MinPoint.z - a.MinPoint.z ) != 0.0f) || (( MaxPoint.z - a.MaxPoint.z ) != 0.0f) );
    }
        
    bool IsValid( void ) const
    {
        return ( MinPoint.x < MaxPoint.x ) && ( MinPoint.y < MaxPoint.y ) && ( MinPoint.z < MaxPoint.z );
    }

    operator bool() const 
    {
        return IsValid();
    }

    void Invalidate( void )
    {
        MinPoint.x = +VD_F32_MAX;
        MaxPoint.x = -VD_F32_MAX;
        MinPoint.y = +VD_F32_MAX; 
        MaxPoint.y = -VD_F32_MAX;
        MinPoint.z = +VD_F32_MAX;
        MaxPoint.z = -VD_F32_MAX;
    }
    
    PointType GetCenter( void ) const
    {
        PointType kExtents = MinPoint + MaxPoint;
        return kExtents * 0.5f;
    }
    
    RealType GetRadius( void ) const
    {
        PointType kSize = GetSize() * 0.5f;
        return length(kSize);
    }
    
    RealType GetWidth( void ) const    { return MaxPoint.x - MinPoint.x; }
    RealType GetHeight( void ) const   { return MaxPoint.y - MinPoint.y; }
    RealType GetDepth( void ) const    { return MaxPoint.z - MinPoint.z; }

    PointType GetSize( void ) const 
    { 
        return PointType( 
            MaxPoint.x - MinPoint.x, 
            MaxPoint.y - MinPoint.y, 
            MaxPoint.z - MinPoint.z ); 
    }
    
    RealType GetVerticalDiscRadius( void ) const
    {
        RealType fSX = ( MaxPoint.x - MinPoint.x );
        RealType fSY = ( MaxPoint.y - MinPoint.y );

        fSX *= 0.5f;
        fSY *= 0.5f;
        
        return Core::Sqrt( fSX * fSX + fSY * fSY );
    }
    
    const AxisAlignedBox operator + ( const AxisAlignedBox &a ) const
    {
        return AxisAlignedBox( 
            MinPoint( MinPoint.x, a.MinPoint.x ),
            MaxPoint( MaxPoint.x, a.MaxPoint.x ),
            MinPoint( MinPoint.y, a.MinPoint.y ),
            MaxPoint( MaxPoint.y, a.MaxPoint.y ),
            MinPoint( MinPoint.z, a.MinPoint.z ),
            MaxPoint( MaxPoint.z, a.MaxPoint.z ));
    }

    const AxisAlignedBox operator + ( const PointType &rkPoint ) const
    {
        return AxisAlignedBox(
            MinPoint( MinPoint.x, rkPoint.x ),
            MaxPoint( MaxPoint.x, rkPoint.x ),
            MinPoint( MinPoint.y, rkPoint.y ),
            MaxPoint( MaxPoint.y, rkPoint.y ),
            MinPoint( MinPoint.z, rkPoint.z ),
            MaxPoint( MaxPoint.z, rkPoint.z ));
    }
    
    const AxisAlignedBox operator + ( const RealType fDelta ) const
    {
        return AxisAlignedBox(
            MinPoint.x - fDelta, MaxPoint.x + fDelta,
            MinPoint.y - fDelta, MaxPoint.y + fDelta,
            MinPoint.z - fDelta, MaxPoint.z + fDelta );
    }

    const AxisAlignedBox operator - ( const RealType fDelta ) const
    {
        return AxisAlignedBox(
            MinPoint.x + fDelta, MaxPoint.x - fDelta,
            MinPoint.y + fDelta, MaxPoint.y - fDelta,
            MinPoint.z + fDelta, MaxPoint.z - fDelta );
    }
     
    AxisAlignedBox &operator += ( const AxisAlignedBox &a )
    {
        MinPoint.x = Core::Min( MinPoint.x, a.MinPoint.x ),
        MaxPoint.x = Core::Max( MaxPoint.x, a.MaxPoint.x ),
        MinPoint.y = Core::Min( MinPoint.y, a.MinPoint.y ),
        MaxPoint.y = Core::Max( MaxPoint.y, a.MaxPoint.y ),
        MinPoint.z = Core::Min( MinPoint.z, a.MinPoint.z ),
        MaxPoint.z = Core::Max( MaxPoint.z, a.MaxPoint.z );

        return *this;
    }

    bool IsOutside( const AxisAlignedBox &a ) const
    {
        return ( a.MinPoint.x < MinPoint.x || a.MaxPoint.x > MaxPoint.x ||
                 a.MinPoint.y < MinPoint.y || a.MaxPoint.y > MaxPoint.y ||
                 a.MinPoint.z < MinPoint.z || a.MaxPoint.z > MaxPoint.z );
    }

    void Extend( const AxisAlignedBox &a )
    {
        MinPoint.x = Core::Min( MinPoint.x, a.MinPoint.x ),
        MaxPoint.x = Core::Max( MaxPoint.x, a.MaxPoint.x ),
        MinPoint.y = Core::Min( MinPoint.y, a.MinPoint.y ),
        MaxPoint.y = Core::Max( MaxPoint.y, a.MaxPoint.y ),
        MinPoint.z = Core::Min( MinPoint.z, a.MinPoint.z ),
        MaxPoint.z = Core::Max( MaxPoint.z, a.MaxPoint.z );
    }
    
    AxisAlignedBox &operator += ( const PointType &rkPoint )
    {
        MinPoint.x = Core::Min( MinPoint.x, rkPoint.x );
        MaxPoint.x = Core::Max( MaxPoint.x, rkPoint.x );
        MinPoint.y = Core::Min( MinPoint.y, rkPoint.y );
        MaxPoint.y = Core::Max( MaxPoint.y, rkPoint.y );
        MinPoint.z = Core::Min( MinPoint.z, rkPoint.z );
        MaxPoint.z = Core::Max( MaxPoint.z, rkPoint.z );

        return *this;
    }

    void Extend( const PointType &rkPoint )
    {
        MinPoint.x = Core::Min( MinPoint.x, rkPoint.x );
        MaxPoint.x = Core::Max( MaxPoint.x, rkPoint.x );
        MinPoint.y = Core::Min( MinPoint.y, rkPoint.y );
        MaxPoint.y = Core::Max( MaxPoint.y, rkPoint.y );
        MinPoint.z = Core::Min( MinPoint.z, rkPoint.z );
        MaxPoint.z = Core::Max( MaxPoint.z, rkPoint.z );
    }

    void Extend( const PointType &rkPoint, RealType fRadius )
    {
        MinPoint.x = Core::Min( MinPoint.x, rkPoint.x - fRadius );
        MaxPoint.x = Core::Max( MaxPoint.x, rkPoint.x + fRadius);
        MinPoint.y = Core::Min( MinPoint.y, rkPoint.y - fRadius );
        MaxPoint.y = Core::Max( MaxPoint.y, rkPoint.y + fRadius);
        MinPoint.z = Core::Min( MinPoint.z, rkPoint.z - fRadius );
        MaxPoint.z = Core::Max( MaxPoint.z, rkPoint.z + fRadius );
    }

    AxisAlignedBox &operator += ( const RealType fDelta )
    {
        MinPoint.x -= fDelta;
        MaxPoint.x += fDelta;
        MinPoint.y -= fDelta;
        MaxPoint.y += fDelta;
        MinPoint.z -= fDelta;
        MaxPoint.z += fDelta;

        return *this;
    }

    void Outset( RealType fDelta )
    {
        MinPoint.x -= fDelta;
        MaxPoint.x += fDelta;
        MinPoint.y -= fDelta;
        MaxPoint.y += fDelta;
        MinPoint.z -= fDelta;
        MaxPoint.z += fDelta;
    }
    
    AxisAlignedBox &operator -= ( const RealType fDelta )
    {
        MinPoint.x += fDelta;
        MaxPoint.x -= fDelta;
        MinPoint.y += fDelta;
        MaxPoint.y -= fDelta;
        MinPoint.z += fDelta;
        MaxPoint.z -= fDelta;

        return *this;
    }

    void Translate( const PointType &rkPoint )
    {
        MinPoint.x += rkPoint.x;
        MaxPoint.x += rkPoint.x;
        MinPoint.y += rkPoint.y;
        MaxPoint.y += rkPoint.y;
        MinPoint.z += rkPoint.z;
        MaxPoint.z += rkPoint.z;
    }

    void Transform( const RealType16 &rkM )
    {
        PointType akCorners[8];
        GetCorners( akCorners );

        Invalidate();
            
        Extend( rkM * akCorners[0] );
        Extend( rkM * akCorners[1] );
        Extend( rkM * akCorners[2] );
        Extend( rkM * akCorners[3] );
        Extend( rkM * akCorners[4] );
        Extend( rkM * akCorners[5] );
        Extend( rkM * akCorners[6] );
        Extend( rkM * akCorners[7] );
    }
    
    void GetCorners( PointType akCorners[8] ) const
    {
        akCorners[0] = PointType( MinPoint.x, MinPoint.y, MinPoint.z );
        akCorners[1] = PointType( MinPoint.x, MaxPoint.y, MinPoint.z );
        akCorners[2] = PointType( MaxPoint.x, MaxPoint.y, MinPoint.z );
        akCorners[3] = PointType( MaxPoint.x, MinPoint.y, MinPoint.z );

        akCorners[4] = PointType( MinPoint.x, MinPoint.y, MaxPoint.z );
        akCorners[5] = PointType( MinPoint.x, MaxPoint.y, MaxPoint.z );
        akCorners[6] = PointType( MaxPoint.x, MaxPoint.y, MaxPoint.z );
        akCorners[7] = PointType( MaxPoint.x, MinPoint.y, MaxPoint.z );
    }

    bool Contains( const PointType &rkPoint ) const
    {
        return ( rkPoint.x >= MinPoint.x && rkPoint.x <= MaxPoint.x &&
                 rkPoint.y >= MinPoint.y && rkPoint.y <= MaxPoint.y &&
                 rkPoint.z >= MinPoint.z && rkPoint.z <= MaxPoint.z );
    }
    
    OverlapType Intersects( const AxisAlignedBox &rkOther ) const
    {
        if ( rkOther.MinPoint.x >= MinPoint.x && rkOther.MaxPoint.x <= MaxPoint.x &&
             rkOther.MinPoint.y >= MinPoint.y && rkOther.MaxPoint.y <= MaxPoint.y &&
             rkOther.MinPoint.z >= MinPoint.z && rkOther.MaxPoint.z <= MaxPoint.z )
        {
            return OverlapType::Inside;
        }

        bool bOverlapX = ( MinPoint.x < rkOther.MaxPoint.x && MaxPoint.x > rkOther.MinPoint.x );
        bool bOverlapY = ( MinPoint.y < rkOther.MaxPoint.y && MaxPoint.y > rkOther.MinPoint.y );
        bool bOverlapZ = ( MinPoint.z < rkOther.MaxPoint.z && MaxPoint.z > rkOther.MinPoint.z );
        
        return ( bOverlapX && bOverlapY && bOverlapZ ) ? (OverlapType::Intersects) : OverlapType::Outside;
    }

    OverlapType::Value IntersectsSphere( 
        const PointType &c, RealType fRadius ) const
    {
        return Intersects( 
            AxisAlignedBox( c, fRadius )
        );
    }

    void render() const
    {
        if ( !isValid() ) 
            return;
        
        PointType akCorners[8];
        getCorners(akCorners);
             
        glDisable( GL_LIGHTING );

        glBegin( GL_LINE_STRIP );
        {
            for(uint i = 0; i < 8; i++)
                glVertex3f(akCorners[i].x, akCorners[i].y, akCorners[i].z);
        }
        glEnd();
        
        glBegin( GL_LINES );
        {
            glVertex3f(akCorners[7].x, akCorners[7].y, akCorners[7].z);
            glVertex3f(akCorners[3].x, akCorners[3].y, akCorners[3].z);

            glVertex3f(akCorners[6].x, akCorners[6].y, akCorners[6].z);
            glVertex3f(akCorners[2].x, akCorners[2].y, akCorners[2].z);

            glVertex3f(akCorners[5].x, akCorners[5].y, akCorners[5].z);
            glVertex3f(akCorners[1].x, akCorners[1].y, akCorners[1].z);
        }
        glEnd();
        
        glEnable( GL_LIGHTING );
    }

public:

    PointType min;
    PointType max;

};

#endif
