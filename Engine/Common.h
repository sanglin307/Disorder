#ifndef _DISORDER_COMMON_H_
#define _DISORDER_COMMON_H_


namespace Disorder
{

	// while check if should exit mainloop  
	extern bool GIsRequestingExit;

	extern unsigned long GFrameCounter;
	extern unsigned int  GDrawTriNumber;
 
	void sysSleep(float deltaseconds);
	 

	void DebugOutput(const char* expr,const char* file,int line,const char* message);

	template< typename T > struct TRect
    {
        T left, top, right, bottom;
        TRect() : left(0), top(0), right(0), bottom(0) {}
        TRect( T const & l, T const & t, T const & r, T const & b )
        : left( l ), top( t ), right( r ), bottom( b )
        {
        }
        TRect( TRect const & o )
        : left( o.left ), top( o.top ), right( o.right ), bottom( o.bottom )
        {
        }
        TRect & operator=( TRect const & o )
        {
        left = o.left;
        top = o.top;
        right = o.right;
        bottom = o.bottom;
        return *this;
        }
        T width() const
        {
        return right - left;
        }
        T height() const
        {
        return bottom - top;
        }
		bool isNull() const
		{
			return width() == 0 || height() == 0;
		}
		void setNull()
		{
			left = right = top = bottom = 0;
		}
		TRect & merge(const TRect& rhs)
		{
			if (isNull())
			{
				*this = rhs;
			}
			else if (!rhs.isNull())
			{
				left = std::min(left, rhs.left);
				right = std::max(right, rhs.right);
				top = std::min(top, rhs.top);
				bottom = std::max(bottom, rhs.bottom);
			}

			return *this;

		}
		TRect intersect(const TRect& rhs) const
		{
			TRect ret;
			if (isNull() || rhs.isNull())
			{
				// empty
				return ret;
			}
			else
			{
				ret.left = std::max(left, rhs.left);
				ret.right = std::min(right, rhs.right);
				ret.top = std::max(top, rhs.top);
				ret.bottom = std::min(bottom, rhs.bottom);
			}

			if (ret.left > ret.right || ret.top > ret.bottom)
			{
				// no intersection, return empty
				ret.left = ret.top = ret.right = ret.bottom = 0;
			}

			return ret;

		}

     };
}

#endif