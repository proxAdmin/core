#pragma once

#define IMAGE_STUDIO_TEXT_RENDER

#ifdef IMAGE_STUDIO_TEXT_RENDER

#include <XmlUtils.h>
#include <AtlDefine.h>
#include <MediaFormatDefine.h>
#include <AtlDefine.h>

#include "Tracking.h"

#import "..\..\..\Redist\AVSImageStudio3.dll"				named_guids rename_namespace("ImageStudio"),	raw_interfaces_only

class ImageStudioTextRender
{
public:

	ImageStudioTextRender(void)
	{
		m_bEnableEditTrack	=	false;

		m_pImageTransforms	=	NULL;
		m_pTracking			=	NULL;

		m_pSource			=	NULL;
		m_pSurface			=	NULL;

		m_nWidth			=	-1;
		m_nHeight			=	-1;
		
		m_nLayoutWidth		=	-1;
		m_nLayoutHeight		=	-1;
	}

	~ImageStudioTextRender(void)
	{
		Clear ();

		RELEASEINTERFACE ( m_pImageTransforms );
	}


	inline void Clear ()
	{
		m_XmlString		=	_T("");
		m_XmlTransform	=	_T("");

		FlushCopySurface ( );
	}

	inline bool DrawXmlEffectText ( )
	{
		if ( m_XmlString.GetLength () > 1 )
		{
			if ( NULL == m_pImageTransforms )
			{
				CreateImageTransform ();
			}

			if ( NULL != m_pSource || NULL != m_pImageTransforms )
			{
				m_bEnableEditTrack	= m_pTracking->IsEditingModeEnabled ();

				if ( m_bEnableEditTrack )
				{
					BYTE* pBuffer	=	NULL;	m_pSource->get_Plane	( 0, &pBuffer );
					long Width		=	0;		m_pSource->get_Width	( &Width );
					long Height		=	0;		m_pSource->get_Height	( &Height );

					if ( NULL == pBuffer || 2 > Width || 2 > Height )
						return false;

					if ( m_nWidth != Width || m_nHeight != Height )
					{
						m_nWidth		=	Width;
						m_nHeight		=	Height;

						if ( NULL != m_pSurface )
						{
							delete [] m_pSurface;
							m_pSurface	=	NULL;
						}

						m_pSurface		=	new BYTE [ m_nWidth * m_nHeight * 4 ];
						if ( NULL == m_pSurface )
							return false;

						SetEffectXml ( m_XmlString );
					}

					UpdateXmlSource ( );

					if ( m_XmlTransform.GetLength () < 1 )
						return false;

					memcpy ( m_pSurface, pBuffer, m_nWidth * m_nHeight * 4 );

					VARIANT val;
					val.vt			=	VT_UNKNOWN;
					m_pSource->QueryInterface ( &val.punkVal );
					m_pSource->Release ();

					VARIANT_BOOL STATUS;
					//if ( SUCCEEDED ( m_pImageTransforms->SetXml ( _bstr_t ( m_XmlTransform ), &STATUS ) ) )
					if ( SUCCEEDED ( m_pImageTransforms->SetXml ( CComBSTR ( m_XmlTransform ), &STATUS ) ) )
					{
						if ( VARIANT_TRUE == STATUS )
						{
							if ( SUCCEEDED ( m_pImageTransforms->SetSource ( -10, val ) ) )
							{
								if ( SUCCEEDED ( m_pImageTransforms->Transform ( &STATUS ) ) )
								{
									if ( VARIANT_TRUE == STATUS )
									{
										if ( SUCCEEDED ( m_pImageTransforms->GetResult ( 0, &val ) ) )
										{
#ifdef _DEBUG
											// SourceToFilePNG ( (MediaCore::IAVSUncompressedVideoFrame*)m_pSource, L"c:\\test_01.png" );
											// ATLTRACE (_T("DrawXmlEffectText - OK\n") );
#endif										
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}


	inline bool SetTracking ( Tracking::CTrackObjectTextRotatable* pTracking )
	{
		m_pTracking	=	pTracking;

		return true;
	}

	inline bool SetEffectXml ( CString Xml )
	{
		m_XmlString	=	Xml;

		XmlUtils::CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( Xml ) )
		{
			m_Token		=	oXmlNode.GetAttributeOrValue ( _T("token"),	_T("~~``~~") );
		
			m_TextOptions.Format (
				_T("sourceid='10' ")		// FOR ONE SOURCE USE FOR DRAW EFFECT TEXT
				_T("metric='%s' ")																			
				_T("animation-fadein='%s' ")
				_T("animation-state='%s' ")
				_T("animation-fadeout='%s' ")
				_T("text-fillmode='%s' ")
				_T("text-typeeffects='%s' ")
				_T("token='%s' ")
				_T("font-name='%s' ")
				_T("font-bold='%s' ")
				_T("font-italic='%s' ")
				_T("font-underline='%s' ")
				_T("font-strikeout='%s' ")
				_T("font-stringalignmenthorizontal='%s' ")
				_T("font-stringalignmentvertical='%s' ")
				_T("brush-type='%s' ")
				_T("brush-color1='%s' ")
				_T("brush-color2='%s' ")
				_T("brush-alpha1='%s' ")
				_T("brush-alpha2='%s' ")
				_T("brush-texturepath='%s' ")
				_T("brush-texturealpha='%s' ")
				_T("brush-texturemode='%s' ")
				_T("brush-rectable='%s' ")
				_T("brush-rect-left='%s' ")
				_T("brush-rect-top='%s' ")
				_T("brush-rect-width='%s' ")
				_T("brush-rect-height='%s' ")
				_T("pen-antialiasbrush='%s' ")
				_T("shadow-visible='%s' ")
				
				//_T("shadow-distancex='%f' ")
				//_T("shadow-distancey='%f' ")
				//_T("shadow-blursize='%f' ")
				_T("shadow-distancex='%s' ")
				_T("shadow-distancey='%s' ")
				_T("shadow-blursize='%s' ")

				_T("shadow-color='%s' ")
				_T("shadow-alpha='%s' ")
				_T("edge-visible='%s' ")
				
				//_T("edge-dist='%f' ")
				_T("edge-dist='%s' ")

				_T("edge-color='%s' ")
				_T("edge-alpha='%s' ")
				_T("font-size='%s' " ),

				_T("3"), // oXmlNode.GetAttributeOrValue ( _T("metric"),				_T("0") ), //	L"metric='0' "
				oXmlNode.GetAttributeOrValue ( _T("animation-fadein"),					_T("0") ), //	L"animation-fadein='1' "
				oXmlNode.GetAttributeOrValue ( _T("animation-state"),					_T("0") ), //	L"animation-state='3' "
				oXmlNode.GetAttributeOrValue ( _T("animation-fadeout"),					_T("0") ), //	L"animation-fadeout='1' "
				oXmlNode.GetAttributeOrValue ( _T("text-fillmode"),						_T("0") ), //	L"text-fillmode='0' "
				oXmlNode.GetAttributeOrValue ( _T("text-typeeffects"),					_T("0") ), //	L"text-typeeffects='1' "
				m_Token, // oXmlNode.GetAttributeOrValue ( _T("token"),					_T("_") ), //	L"token='_' "
				oXmlNode.GetAttributeOrValue ( _T("font-name"),						_T("Arial") ), //	L"font-name='Arial' "
				oXmlNode.GetAttributeOrValue ( _T("font-bold"),							_T("0") ), //	L"font-bold='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-italic"),						_T("0") ), //	L"font-italic='0' "
				oXmlNode.GetAttributeOrValue ( _T("font-underline"),					_T("0") ), //	L"font-underline='0' "
				oXmlNode.GetAttributeOrValue ( _T("font-strikeout"),					_T("0") ), //	L"font-strikeout='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-stringalignmenthorizontal"),	_T("0") ), //	L"font-stringalignmenthorizontal='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-stringalignmentvertical"),		_T("0") ), //	L"font-stringalignmentvertical='1' "
				oXmlNode.GetAttributeOrValue ( _T("brush-type"),						_T("0") ), //	L"brush-type='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-color1"),						_T("0") ), //	L"brush-color1='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-color2"),						_T("65535") ), //	L"brush-color2='65535' "
				oXmlNode.GetAttributeOrValue ( _T("brush-alpha1"),						_T("255") ), //	L"brush-alpha1='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-alpha2"),						_T("255") ), //	L"brush-alpha2='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturepath"),					_T("") ), //	L"brush-texturepath='c:\' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturealpha"),				_T("0") ), //	L"brush-texturealpha='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturemode"),					_T("0") ), //	L"brush-texturemode='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rectable"),					_T("0") ), //	L"brush-rectable='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-left"),					_T("0") ), //	L"brush-rect-left='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-top"),					_T("0") ), //	L"brush-rect-top='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-width"),					_T("0") ), //	L"brush-rect-width='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-height"),					_T("0") ), //	L"brush-rect-height='0' "
				oXmlNode.GetAttributeOrValue ( _T("pen-antialiasbrush"),				_T("0") ), //	L"pen-antialiasbrush='1' "
				oXmlNode.GetAttributeOrValue ( _T("shadow-visible"),					_T("0") ), //	L"shadow-visible='1' "
				
				//_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-distancex"),		_T("0") ) ),// * m_nWidth,// * m_nLayoutWidth, //	L"shadow-distancex='5' "
				//_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-distancey"),		_T("0") ) ),// * m_nHeight,// * m_nLayoutHeight, //	L"shadow-distancey='10' "
				//_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-blursize"),		_T("0") ) ),// * m_nWidth,// * m_nLayoutWidth, //	L"shadow-blursize='3' "
				oXmlNode.GetAttributeOrValue ( _T("shadow-distancex"),					_T("0") ), //	
				oXmlNode.GetAttributeOrValue ( _T("shadow-distancey"),					_T("0") ), //	
				oXmlNode.GetAttributeOrValue ( _T("shadow-blursize"),					_T("0") ), //	

				oXmlNode.GetAttributeOrValue ( _T("shadow-color"),						_T("0") ), //	L"shadow-color='0' "
				oXmlNode.GetAttributeOrValue ( _T("shadow-alpha"),						_T("0") ), //	L"shadow-alpha='120' "
				oXmlNode.GetAttributeOrValue ( _T("edge-visible"),						_T("0") ), //	L"edge-visible='1' "
				
				//_tstof ( oXmlNode.GetAttributeOrValue ( _T("edge-dist"),				_T("0") ) ),// * m_nWidth,// * m_nLayoutWidth, //	L"edge-dist='3' "
				oXmlNode.GetAttributeOrValue ( _T("edge-dist"),							_T("0") ), //	L"edge-dist='3' "

				oXmlNode.GetAttributeOrValue ( _T("edge-color"),						_T("0") ), //	L"edge-color='65536' "
				oXmlNode.GetAttributeOrValue ( _T("edge-alpha"),						_T("0") ),  //	L"edge-alpha='192' >"
				oXmlNode.GetAttributeOrValue ( _T("font-size"),							_T("0") )	//
				);

			return true;
		}

		return false;
	}

	inline bool SetBounding ( double X0, double Y0, double X1, double Y1, double Angle, double Zoom )
	{
		m_X0	=	X0;
		m_Y0	=	Y0;
		m_X1	=	X1;
		m_Y1	=	Y1;

		m_Angle	=	Angle;
		m_Zoom	=	Zoom;

		return true;
	}

	inline bool SetSource ( IUnknown* pSource )
	{
		m_pSource	=	NULL;

		pSource->QueryInterface ( &m_pSource );
		pSource->Release ();

		return true;
	}

	inline void SetLayoutSizes ( int Width, int Height )
	{
		m_nLayoutWidth		=	Width;
		m_nLayoutHeight		=	Height;
	}

	inline BYTE* GetSurface ()
	{
		if ( m_bEnableEditTrack )
			return m_pSurface;

		return NULL;
	}

private:
	inline void FlushCopySurface ( )
	{
		if ( NULL != m_pSurface )
		{
			delete [] m_pSurface;
			m_pSurface	=	NULL;
		}
	}

	inline bool SourceToFilePNG ( MediaCore::IAVSUncompressedVideoFrame* pFrame, CStringW FileName )
	{
		if ( NULL == pFrame || FileName.GetLength () < 1 )
			return false;

		long Width		= 0;	pFrame->get_Width ( &Width );
		long Height		= 0;	pFrame->get_Height ( &Height );
		BYTE* pBuffer	= 0;	pFrame->get_Plane ( 0, &pBuffer );

		if ( Width < 0 || Height < 0 || NULL == pBuffer )
			return false;

		CLSID ImageGUID;
		ImageRendering::ImageUtils::GetEncoderCLSID ( L"image/png", &ImageGUID );
		Gdiplus::Bitmap SetBitmap ( Width, Height, 4 * Width, PixelFormat32bppARGB, pBuffer );
		if ( Gdiplus::Ok == SetBitmap.GetLastStatus () )
		{
			if ( Gdiplus::Ok == SetBitmap.Save ( FileName, &ImageGUID ) )
			{
				return true;
			}
		}

		return false;
	}

	inline bool UpdateXmlSource ( )
	{
		//double FonSize	=	static_cast<double> ( m_pTracking->GetFontSize() ) * m_Zoom;
		
		/*
		* &amp;		→ & (	ampersand,			U+0026	)
		* &lt;		→ < (	less-than sign,		U+003C	)
		* &gt;		→ > (	greater-than sign,	U+003E	)
		* &quot;	→ " (	quotation mark,		U+0022	)
		* &apos;	→ ' (	apostrophe,			U+0027	)
		*/

		CString Source	=	m_pTracking->GetString ();
		
		Source.Replace ( _T("&"),	_T("&amp;")		);
		Source.Replace ( _T("<"),	_T("&lt;")		);
		Source.Replace ( _T(">"),	_T("&gt;")		);
		Source.Replace ( _T("\""),	_T("&quot;")	);
		Source.Replace ( _T("\'"),	_T("&apos;")	);

		m_XmlTransform.Format (	_T("rect-left='%f' rect-top='%f' rect-right='%f' rect-bottom='%f' text='%s'")
			//_T(" font-size='%f'")
			_T(" font-angle='%f' %s >"),

			m_X0 / (double)m_nWidth,
			m_Y0 / (double)m_nHeight,
			m_X1 / (double)m_nWidth, 
			m_Y1 / (double)m_nHeight,
			m_Token + Source + m_Token,
			//FonSize, 
			m_Angle, m_TextOptions );

		m_XmlTransform	=	_T("<transforms><ImagePaint-DrawAnimatedText  ")	+	m_XmlTransform	+	_T("</ImagePaint-DrawAnimatedText></transforms>");

		return true;
	}

	inline bool CreateImageTransform ()
	{	
		RELEASEINTERFACE ( m_pImageTransforms );
		if ( FAILED ( CoCreateInstance ( ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&m_pImageTransforms ) ) )
		{
			return false;
		}

		return true;
	}


private:

	bool									m_bEnableEditTrack;

	double									m_X0;
	double									m_Y0;
	double									m_X1;
	double									m_Y1;
	double									m_Angle;
	double									m_Zoom;

	CString									m_TextOptions;
	CString									m_XmlTransform;
	CString									m_XmlString;
	CString									m_Token;

	int										m_nWidth;
	int										m_nHeight;

	MediaCore::IAVSUncompressedVideoFrame*	m_pSource;
	BYTE*									m_pSurface;

	Tracking::CTrackObjectTextRotatable*	m_pTracking;
	ImageStudio::IImageTransforms*			m_pImageTransforms;

	int										m_nLayoutWidth;
	int										m_nLayoutHeight;
};

class ImageStudioTextEffect
{
public:

	ImageStudioTextEffect(void)
	{
		m_pImageTransforms	=	NULL;

		m_pSource			=	NULL;
		m_pSurface			=	NULL;

		m_nWidth			=	-1;
		m_nHeight			=	-1;

		m_nBufferWidth		=	-1;
		m_nBufferHeight		=	-1;
		
		m_FontSize			=	0;

		m_X0				=	0.0;
		m_Y0				=	0.0;
		m_X1				=	0.0;
		m_Y1				=	0.0;
		m_Angle				=	0.0;
		m_Zoom				=	0.0;
	}

	~ImageStudioTextEffect(void)
	{
		Clear ();

		RELEASEINTERFACE ( m_pImageTransforms );
	}


	inline void Clear ()
	{
		m_XmlString		=	_T("");
		m_XmlTransform	=	_T("");
		m_Text			=	_T("");

		FlushSurface ( );
	}

	inline bool DrawXmlEffectText ( )
	{
		if ( m_XmlString.GetLength () > 1 )
		{
			if ( NULL == m_pImageTransforms )
			{
				CreateImageTransform ();
			}
			
			if ( NULL != m_pSource || NULL != m_pImageTransforms )
			{
				VARIANT val;
				val.vt			=	VT_UNKNOWN;
				m_pSource->QueryInterface ( &val.punkVal );
				m_pSource->Release ();

				if ( NULL == val.punkVal )
					return false;

				UpdateXmlSource ( );

				if ( TRUE )
				{
					if ( m_XmlTransform.GetLength () < 1 )
						return false;

					BYTE* pBuffer	=	NULL;	m_pSource->get_Buffer		( &pBuffer );
					long Width		=	0;		m_pSource->get_Width		( &Width );
					long Height		=	0;		m_pSource->get_Height		( &Height );
					long BufferSize	=	0;		m_pSource->get_BufferSize	( &BufferSize );

					if ( NULL == pBuffer || 1 > Width || 1 > Height )
						return false;

					if ( m_nBufferWidth * m_nBufferHeight < Width * Height )
					{
						m_nWidth			=	Width;
						m_nHeight			=	Height;

						m_nBufferWidth		=	Width;
						m_nBufferHeight		=	Height;

						if ( NULL != m_pSurface )
						{
							delete [] m_pSurface;
							m_pSurface	=	NULL;
						}

						m_pSurface		=	new BYTE [ m_nBufferWidth * m_nBufferHeight * 4 ];
					}
					else
					{
						m_nWidth		=	Width;
						m_nHeight		=	Height;
					}
					
					memcpy_s ( m_pSurface, m_nWidth * m_nHeight * 4, pBuffer, m_nWidth * m_nHeight * 4 );

					VARIANT_BOOL STATUS;
					if ( SUCCEEDED ( m_pImageTransforms->SetXml ( CComBSTR ( m_XmlTransform ), &STATUS ) ) )
					{
						if ( VARIANT_TRUE == STATUS )
						{
							if ( SUCCEEDED ( m_pImageTransforms->SetSource ( -10, val ) ) )
							{
								if ( SUCCEEDED ( m_pImageTransforms->Transform ( &STATUS ) ) )
								{
									if ( VARIANT_TRUE == STATUS )
									{			
										if ( SUCCEEDED ( m_pImageTransforms->GetResult ( 0, &val ) ) )
										{
											return true;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}


	inline bool SetEffectXml ( CString Xml )
	{
		m_XmlString	=	Xml;

		XmlUtils::CXmlNode oXmlNode;
		if ( oXmlNode.FromXmlString ( Xml ) )
		{
			m_FontSize		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T( "font-size" ),		_T("0") ) );

			m_nRectLeft		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T( "rect-left" ),		_T("0") ) );
			m_nRectTop		=	_tstof ( oXmlNode.GetAttributeOrValue ( _T( "rect-top" ),		_T("0") ) );
			m_nRectRight	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T( "rect-right" ),		_T("0") ) );
			m_nRectBottom	=	_tstof ( oXmlNode.GetAttributeOrValue ( _T( "rect-bottom" ),	_T("0") ) );

			CString Source	=	_T("");
			if ( 0 == m_Text.GetLength()  )
			{
				Source		=	oXmlNode.GetAttributeOrValue ( _T("text"),	_T("") );
			}
			else
			{
				Source		=	m_Text;
			}

			m_TextOptions.Format (
				_T("sourceid='10' ")		// FOR ONE SOURCE USE FOR DRAW EFFECT TEXT
				_T("metric='%s' ")	
				_T("text-dir-x1='%s' ")
				_T("text-dir-y1='%s' ")
				_T("text-dir-x2='%s' ")
				_T("text-dir-y2='%s' ")
				_T("animation-fadein='%s' ")
				_T("animation-state='%s' ")
				_T("animation-fadeout='%s' ")
				_T("text-fillmode='%s' ")
				_T("text-typeeffects='%s' ")
				_T("token='%s' ")
				_T("font-name='%s' ")
				_T("font-bold='%s' ")
				_T("font-italic='%s' ")
				_T("font-underline='%s' ")
				_T("font-strikeout='%s' ")
				_T("font-stringalignmenthorizontal='%s' ")
				_T("font-stringalignmentvertical='%s' ")
				_T("brush-type='%s' ")
				_T("brush-color1='%s' ")
				_T("brush-color2='%s' ")
				_T("brush-alpha1='%s' ")
				_T("brush-alpha2='%s' ")
				_T("brush-texturepath='%s' ")
				_T("brush-texturealpha='%s' ")
				_T("brush-texturemode='%s' ")
				_T("brush-rectable='%s' ")
				_T("brush-rect-left='%s' ")
				_T("brush-rect-top='%s' ")
				_T("brush-rect-width='%s' ")
				_T("brush-rect-height='%s' ")
				_T("pen-antialiasbrush='%s' ")
				_T("shadow-visible='%s' ")
				_T("shadow-distancex='%f' ")
				_T("shadow-distancey='%f' ")
				_T("shadow-blursize='%f' ")
				_T("shadow-color='%s' ")
				_T("shadow-alpha='%s' ")
				_T("edge-visible='%s' ")
				_T("edge-dist='%f' ")
				_T("edge-color='%s' ")
				_T("edge-alpha='%s' ")
				_T("font-angle='%s' ")
				_T("brush-gradient-angle='%s' ")
				_T("text-simpleline='%s' ")
				_T("text='%s' "),

				_T("0"), // oXmlNode.GetAttributeOrValue ( _T("metric"),				_T("0") ), //	L"metric='0' "
				oXmlNode.GetAttributeOrValue ( _T("text-dir-x1"),					_T("0") ), //
				oXmlNode.GetAttributeOrValue ( _T("text-dir-y1"),					_T("0") ), //
				oXmlNode.GetAttributeOrValue ( _T("text-dir-x2"),					_T("0") ), //
				oXmlNode.GetAttributeOrValue ( _T("text-dir-y2"),					_T("0") ), //
				oXmlNode.GetAttributeOrValue ( _T("animation-fadein"),					_T("0") ), //	L"animation-fadein='1' "
				oXmlNode.GetAttributeOrValue ( _T("animation-state"),					_T("0") ), //	L"animation-state='3' "
				oXmlNode.GetAttributeOrValue ( _T("animation-fadeout"),					_T("0") ), //	L"animation-fadeout='1' "
				oXmlNode.GetAttributeOrValue ( _T("text-fillmode"),						_T("0") ), //	L"text-fillmode='0' "
				oXmlNode.GetAttributeOrValue ( _T("text-typeeffects"),					_T("0") ), //	L"text-typeeffects='1' "
				oXmlNode.GetAttributeOrValue ( _T("token"),								_T("_") ), //	L"token='_' "
				oXmlNode.GetAttributeOrValue ( _T("font-name"),						_T("Arial") ), //	L"font-name='Arial' "
				oXmlNode.GetAttributeOrValue ( _T("font-bold"),							_T("0") ), //	L"font-bold='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-italic"),						_T("0") ), //	L"font-italic='0' "
				oXmlNode.GetAttributeOrValue ( _T("font-underline"),					_T("0") ), //	L"font-underline='0' "
				oXmlNode.GetAttributeOrValue ( _T("font-strikeout"),					_T("0") ), //	L"font-strikeout='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-stringalignmenthorizontal"),	_T("0") ), //	L"font-stringalignmenthorizontal='1' "
				oXmlNode.GetAttributeOrValue ( _T("font-stringalignmentvertical"),		_T("0") ), //	L"font-stringalignmentvertical='1' "
												   
				oXmlNode.GetAttributeOrValue ( _T("brush-type"),						_T("0") ), //	L"brush-type='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-color1"),						_T("0") ), //	L"brush-color1='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-color2"),						_T("65535") ), //	L"brush-color2='65535' "
				oXmlNode.GetAttributeOrValue ( _T("brush-alpha1"),						_T("255") ), //	L"brush-alpha1='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-alpha2"),						_T("255") ), //	L"brush-alpha2='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturepath"),					_T("") ), //	L"brush-texturepath='c:\' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturealpha"),				_T("0") ), //	L"brush-texturealpha='255' "
				oXmlNode.GetAttributeOrValue ( _T("brush-texturemode"),					_T("0") ), //	L"brush-texturemode='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rectable"),					_T("0") ), //	L"brush-rectable='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-left"),					_T("0") ), //	L"brush-rect-left='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-top"),					_T("0") ), //	L"brush-rect-top='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-width"),					_T("0") ), //	L"brush-rect-width='0' "
				oXmlNode.GetAttributeOrValue ( _T("brush-rect-height"),					_T("0") ), //	L"brush-rect-height='0' "
				oXmlNode.GetAttributeOrValue ( _T("pen-antialiasbrush"),				_T("0") ), //	L"pen-antialiasbrush='1' "
				oXmlNode.GetAttributeOrValue ( _T("shadow-visible"),					_T("0") ), //	L"shadow-visible='1' "

				//oXmlNode.GetAttributeOrValue ( _T("shadow-distancex"),				_T("0") ), //	L"shadow-visible='1' "
				//oXmlNode.GetAttributeOrValue ( _T("shadow-distancey"),				_T("0") ), //	L"shadow-visible='1' "
				//oXmlNode.GetAttributeOrValue ( _T("shadow-blursize"),					_T("0") ), //	L"shadow-visible='1' "
				_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-distancex"),			_T("0") ) ) * m_Zoom, //	L"shadow-distancex='5' "
				_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-distancey"),			_T("0") ) ) * m_Zoom, //	L"shadow-distancey='10' "
				_tstof ( oXmlNode.GetAttributeOrValue ( _T("shadow-blursize"),			_T("0") ) ) * m_Zoom, //	L"shadow-blursize='3' "
				
				oXmlNode.GetAttributeOrValue ( _T("shadow-color"),						_T("0") ), //	L"shadow-color='0' "
				oXmlNode.GetAttributeOrValue ( _T("shadow-alpha"),						_T("0") ), //	L"shadow-alpha='120' "
				oXmlNode.GetAttributeOrValue ( _T("edge-visible"),						_T("0") ), //	L"edge-visible='1' "
				
				//oXmlNode.GetAttributeOrValue ( _T("edge-dist"),						_T("0") ), //	L"edge-visible='1' "
				_tstof ( oXmlNode.GetAttributeOrValue ( _T("edge-dist"),				_T("0") ) ) * m_Zoom, //	L"edge-dist='3' "
				
				oXmlNode.GetAttributeOrValue ( _T("edge-color"),						_T("0") ),	//	L"edge-color='65536' "
				oXmlNode.GetAttributeOrValue ( _T("edge-alpha"),						_T("0") ),  //	L"edge-alpha='192' >"
				oXmlNode.GetAttributeOrValue ( _T("font-angle"),						_T("0") ),  //	L"font-angle='0' >"
				oXmlNode.GetAttributeOrValue ( _T("brush-gradient-angle"),				_T("0") ),  //	L"brush-gradient-angle='0' >"
				oXmlNode.GetAttributeOrValue ( _T("text-simpleline"),					_T("0") ),  //	_T("text-simpleline='%s' ")

				Source	//	oXmlNode.GetAttributeOrValue ( _T("text"),					_T("") )	//	L"text='TEXT' >"
				);

			return true;
		}

		return false;
	}

	inline void SetText ( CString Xml )
	{
		m_Text	=	Xml;
	}
	inline bool GetNoneNormalizedBounding ( double& X0, double& Y0, double& X1, double& Y1 )
	{
		X0	=	m_nRectLeft;
		Y0	=	m_nRectTop;
		X1	=	m_nRectRight;
		Y1	=	m_nRectBottom;

		return true;
	}

	inline bool SetBounding ( double X0, double Y0, double X1, double Y1, double Zoom )
	{
		m_X0	=	X0;
		m_Y0	=	Y0;
		m_X1	=	X1;
		m_Y1	=	Y1;

		m_Zoom	=	Zoom;

		return true;
	}

	inline bool SetSource ( IUnknown* pSource )
	{
		m_pSource	=	NULL;

		pSource->QueryInterface ( &m_pSource );
		pSource->Release ();

		return true;
	}

	inline BYTE* GetSurface ()
	{
		return m_pSurface;
	}

	inline bool IsEffect ()
	{
		if ( m_XmlString.GetLength () > 1 )
		{
			return true;
		}

		return false;
	}

private:
	
	inline void FlushSurface ( )
	{
		if ( NULL != m_pSurface )
		{
			delete [] m_pSurface;
			m_pSurface	=	NULL;
		}
	}

	inline bool UpdateXmlSource ( )
	{
		// int FonSize	=	static_cast<int> ( m_FontSize * m_Zoom );
		double FonSize	=	static_cast<double> ( m_FontSize * m_Zoom );

		m_X0 = (int)( m_X0 );
		m_Y0 = (int)( m_Y0 );
		m_X1 = (int)( m_X1 + 0.75  );
		m_Y1 = (int)( m_Y1 + 0.75  );

		m_XmlTransform.Format (	_T("rect-left='%f' rect-top='%f' rect-right='%f' rect-bottom='%f' font-size='%f' %s >"), m_X0, m_Y0, m_X1, m_Y1, FonSize, m_TextOptions );

		m_XmlTransform	=	_T("<transforms><ImagePaint-DrawAnimatedText  ")	+	m_XmlTransform	+	_T("</ImagePaint-DrawAnimatedText></transforms>");

		return true;
	}


	inline bool CreateImageTransform ()
	{
		if ( FAILED ( CoCreateInstance ( ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_ALL, ImageStudio::IID_IImageTransforms, (void**)&m_pImageTransforms ) ) )
		{
			return false;
		}

		//if ( 0 )
		//{
		//	m_nBufferWidth		=	5;
		//	m_nBufferHeight		=	5;

		//	m_nWidth			=	m_nBufferWidth;
		//	m_nHeight			=	m_nBufferHeight;

		//	if ( NULL != m_pSurface )
		//	{
		//		delete [] m_pSurface;
		//		m_pSurface	=	NULL;
		//	}

		//	m_pSurface		=	new BYTE [ m_nBufferWidth * m_nBufferHeight * 4 ];
		//}

		return true;
	}

private:
    
	double 									m_nRectLeft;
	double 									m_nRectTop;
	double 									m_nRectRight;
	double 									m_nRectBottom;

	double									m_X0;
	double									m_Y0;
	double									m_X1;
	double									m_Y1;
	double									m_Angle;
	double									m_Zoom;

	CString									m_Text;
	CString									m_TextOptions;
	CString									m_XmlTransform;
	CString									m_XmlString;

	int										m_nWidth;
	int										m_nHeight;

	int										m_nBufferWidth;
	int										m_nBufferHeight;

	double									m_FontSize;

	MediaCore::IAVSUncompressedVideoFrame*	m_pSource;
	BYTE*									m_pSurface;

	ImageStudio::IImageTransforms*			m_pImageTransforms;
};

class CSaveSurface
{
public:

	CSaveSurface ( )
	{
		m_nWidth		=	-1;
		m_nHeight		=	-1;
		
		m_nBufferWidth	=	-1;
		m_nBufferHeight	=	-1;
		
		m_pSurface		=	NULL;
		m_pSource		=	NULL;

	}

	~CSaveSurface ()
	{
		m_pSource	=	NULL;

		if ( NULL != m_pSurface )
		{
			delete [] m_pSurface;
			m_pSurface	=	NULL;
		}
	}

	inline void SetSource ( IUnknown* pSource )
	{
		m_pSource	=	NULL;

		if ( pSource )
		{
			pSource->QueryInterface ( &m_pSource );
			pSource->Release ();
		}
	}

	inline BOOL Copy ( )
	{
		if ( m_pSource )
		{
			BYTE* pBuffer	=	NULL;	m_pSource->get_Buffer	( &pBuffer );
			long Width		=	0L;		m_pSource->get_Width	( &Width );
			long Height		=	0L;		m_pSource->get_Height	( &Height );
			long BufferSize	=	0L;		m_pSource->get_BufferSize ( &BufferSize );
			
			if ( NULL == pBuffer || 1 > Width || 1 > Height )
				return FALSE;

			if ( ( m_nBufferWidth * m_nBufferHeight < Width * Height ) || NULL == m_pSurface )
			{
				m_nWidth		=	Width;
				m_nHeight		=	Height;
				
				m_nBufferWidth	=	__max ( m_nWidth,	m_nBufferWidth );
				m_nBufferHeight	=	__max ( m_nHeight,	m_nBufferHeight );

				if ( NULL != m_pSurface )
				{
					delete [] m_pSurface;
					m_pSurface	=	NULL;
				}

				m_pSurface		=	new BYTE [ m_nWidth * m_nHeight * 4 ];
			}
			else
			{
				m_nWidth		=	Width;
				m_nHeight		=	Height;
			}

			memcpy_s ( m_pSurface, m_nWidth * m_nHeight * 4, pBuffer, m_nWidth * m_nHeight * 4 );

			return TRUE;
		}

		return FALSE;
	}
	
	inline BYTE* GetSurface ()
	{
		return m_pSurface;
	}

private:

	int										m_nWidth;
	int										m_nHeight;

	int										m_nBufferWidth;
	int										m_nBufferHeight;

	MediaCore::IAVSUncompressedVideoFrame*	m_pSource;
	BYTE*									m_pSurface;
};

#endif
