#ifndef _DISORDER_FONT_H_
#define _DISORDER_FONT_H_


namespace Disorder
{
	class Font : public RenderResource
	{
		typedef std::pair<UINT, UINT> CodePointRange;
		

	public:

		struct GlyphInfo 
		{
			UINT codePoint;
			TRect<float> uvRect;
			float aspectRatio;

			GlyphInfo(UINT id, const TRect<float>& rect, float aspect)
				: codePoint(id), uvRect(rect), aspectRatio(aspect)
			{
			}
		};

		typedef boost::unordered_map<UINT, GlyphInfo> CodePointMap;
 
		void LoadFontFromTrueTypeFile(std::string const& fontName);

		RenderSurfacePtr const& GetGlyphTexture() const
		{
			return _glyphsTexture;
		}

		 /** Returns the texture coordinates of the associated glyph. 
            @remarks Parameter is a short to allow both ASCII and wide chars.
            @param id The code point (unicode)
            @return A rectangle with the UV coordinates, or null UVs if the
				code point was not present
        */
        inline const TRect<float>& GetGlyphTexCoords(UINT id) const
        {
			CodePointMap::const_iterator i = _codePointMap.find(id);
			if (i != _codePointMap.end())
			{
				return i->second.uvRect;
			}
			else
			{
				static TRect<float> nullRect(0.0f, 0.0f, 0.0f, 0.0f);
				return nullRect;
			}
        }

		inline const GlyphInfo& GetGlyphInfo(UINT id) const
		{
			
			CodePointMap::const_iterator i = _codePointMap.find(id);
			if (i != _codePointMap.end())
			{
				return i->second;
			}
			else
			{
				static GlyphInfo nullRect(0,TRect<float>(0.0f, 0.0f, 0.0f, 0.0f),0.0f);
			 
				return nullRect;
			}
		}

        /** Sets the texture coordinates of a glyph.
        @remarks
            You only need to call this if you're setting up a font loaded from a texture manually.
        @note
            Also sets the aspect ratio (width / height) of this character. textureAspect
			is the width/height of the texture (may be non-square)
        */
        inline void SetGlyphTexCoords(UINT id, float u1, float v1, float u2, float v2, float textureAspect)
        {
			CodePointMap::iterator i = _codePointMap.find(id);
			if (i != _codePointMap.end())
			{
				i->second.uvRect.left = u1;
				i->second.uvRect.top = v1;
				i->second.uvRect.right = u2;
				i->second.uvRect.bottom = v2;
				i->second.aspectRatio = textureAspect * (u2 - u1)  / (v2 - v1);
			}
			else
			{
				_codePointMap.insert(CodePointMap::value_type(id, GlyphInfo(id, TRect<float>(u1, v1, u2, v2), textureAspect * (u2 - u1)  / (v2 - v1))));
			}

        }

		static FontPtr Create(float fontSize,unsigned int fontRevolution);

	protected:

		Font(float fontSize,unsigned int fontRevolution);

		float _fontSize; // in pixel.
		unsigned int _fontRevolution;
		/// Max distance to baseline of this (truetype) font
		int   _fontMaxBearingY;

		 /** Add a gap between letters vertically and horizonally
            prevents nasty artifacts caused by fonts atypically wide or tall characters. */
        UINT _characterSpacer;

		bool _antialiasColor;

		RenderSurfacePtr _glyphsTexture;

		std::vector<CodePointRange> _codePointRangeList;

		CodePointMap _codePointMap;

	};
}


#endif