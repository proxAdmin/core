#pragma once

#include <atlcoll.h>
#include <structures.h>
#include <rendering.h>
#include <messages.h>
#include <geometry.h>
#include <TimeMeasurer.h>

#include <defines.h>
#include <winuser.h>
#include "ImeUI.h"

// interfaces
namespace Tracking
{
	//-------------------------------------------------------------------------------------------
	// Event Handlers for mouse button
	//-------------------------------------------------------------------------------------------
	struct IMouseButtonHandlers
	{
		virtual bool OnMouseMove	( int MouseX, int MouseY )	= 0;
		virtual bool OnLButtonDown	( int MouseX, int MouseY )	= 0;
		virtual bool OnLButtonUp	( int MouseX, int MouseY )	= 0;
		virtual bool OnRButtonDown	( int MouseX, int MouseY )	= 0;
		virtual bool OnRButtonUp	( int MouseX, int MouseY )	= 0;
		virtual bool OnMButtonDown	( int MouseX, int MouseY )	= 0;
		virtual bool OnMButtonUp	( int MouseX, int MouseY )	= 0;

		virtual ~IMouseButtonHandlers () 
		{

		}
	};

	//-------------------------------------------------------------------------------------------
	//
	//-------------------------------------------------------------------------------------------
	struct ITracking : public IMouseButtonHandlers
	{
		virtual bool MoveTrack ( double dX, double dY ) = 0;
		virtual void SetReceiver ( Messages::CMessageReceiver* pReceiver ) = 0;
		virtual bool OnPaint ( HDC hDC, Structures::RECTI& rectWindow, Structures::RECTD& rectData ) = 0;
		virtual bool OnSetCursor ( int nX, int nY, int& nPointStatus, int& nChangeStatus ) = 0;
		virtual bool OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam ) = 0;
		virtual bool Command ( BSTR CommandXml, VARIANT* ParamValue ) = 0;

		virtual ~ITracking ()
		{

		}
	};

	struct IExternalBounding
	{
	public:
		IExternalBounding ()
		{
			m_bEnableMinBounding	=	false;
			m_bEnableMaxBounding	=	false;
		}

		virtual ~IExternalBounding ()
		{

		}

		virtual void SetBoundMax ( double X, double Y )
		{
			m_nBoundMaxWidth	=	X;
			m_nBoundMaxHeight	=	Y;
		}

		virtual void SetBoundMin ( double X, double Y )
		{
			m_nBoundMinWidth	=	X;
			m_nBoundMinHeight	=	Y;
		}

		virtual void EnableMaxBound ( bool Value )
		{
			m_bEnableMaxBounding	=	Value;
		}

		virtual void EnableMinBound ( bool Value )
		{
			m_bEnableMinBounding	=	Value;
		}

		virtual bool IsUpdateSizes ()
		{
			return false;
		}

	protected:

		bool	m_bEnableMinBounding;
		bool	m_bEnableMaxBounding;

		double	m_nBoundMaxWidth;
		double	m_nBoundMaxHeight;

		double	m_nBoundMinWidth;
		double	m_nBoundMinHeight;
	};

}

// base classes
namespace Tracking
{
	//------------------------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------------------------
	struct CTrackPoint
	{
	public:
		CTrackPoint () : dX ( 0.0 ), dY ( 0.0 ), nX ( 0 ), nY ( 0 )
		{
		}

		CTrackPoint ( double DX, double DY ) : dX ( DX ), dY ( DY )
		{

		}

		CTrackPoint ( double DX, double DY, int NX, int NY ) : dX ( DX ), dY ( DY ), nX ( NX ), nY ( NY )
		{

		}

		CTrackPoint ( const CTrackPoint& oPoint )
		{
			dX = oPoint.dX;
			dY = oPoint.dY;
			nX = oPoint.nX;
			nY = oPoint.nY;
		}


		inline CTrackPoint& operator= (const CTrackPoint& oPoint)
		{
			dX = oPoint.dX;
			dY = oPoint.dY;
			nX = oPoint.nX;
			nY = oPoint.nY;

			return *this;
		}

		inline void Create ( const double& X, const double& Y )
		{
			dX = X;
			dY = Y;
		}

		inline void Create ( const double& DX, const double& DY, const int& NX, const int& NY )
		{
			dX = DX;
			dY = DY;
			nX = NX;
			nY = NY;
		}

		inline void Create ( const CTrackPoint& oPoint )
		{
			dX = oPoint.dX;
			dY = oPoint.dY;
			nX = oPoint.nX;
			nY = oPoint.nY;
		}

	public:

		double	dX;
		double	dY;

		int		nX;
		int		nY;
	};

	//------------------------------------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------------------------------------
	class CTrackPoints 
	{
	public:

		CTrackPoints()
		{
			m_nCurrentPoint = -1;
		}

		virtual ~CTrackPoints()
		{

		}



		inline int GetCount()
		{
			return (int)m_arrPoints.GetCount();
		}
		inline int GetCurrentPointIndex()
		{
			return m_nCurrentPoint;
		}
		inline void SetCurrentPointIndex(int nIndex)
		{
			m_nCurrentPoint = nIndex;
		}



		inline int Add(double dX, double dY)
		{
			CTrackPoint oPoint(dX, dY);

			return Add(oPoint);
		}
		inline int Add(double dX, double dY, int nX, int nY)
		{
			CTrackPoint oPoint(dX, dY, nX, nY);

			return Add(oPoint);
		}
		inline int Add(const CTrackPoint& oPoint)
		{
			return (int)m_arrPoints.Add(oPoint);
		}
		inline int Add(const CTrackPoints& oPoints)
		{
			return (int)m_arrPoints.Append(oPoints.m_arrPoints);
		}

		inline void InsertAt(int nIndex, const CTrackPoint& oPoint)
		{
			m_arrPoints.InsertAt(nIndex, oPoint);
		}
		inline void Remove(int nIndex)
		{
			if (nIndex < 0 || nIndex >= GetCount())
				return;

			m_arrPoints.RemoveAt(nIndex);
		}
		inline void Clear()
		{
			m_arrPoints.RemoveAll();
		}


		inline CTrackPoint& GetAt ( int nIndex )
		{
			return m_arrPoints [ nIndex ];
		}

		inline CTrackPoint& operator[] ( int nIndex )
		{
			return m_arrPoints [ nIndex ];
		}

		inline void SetAt(int nIndex, const CTrackPoint& oPoint)
		{
			if (nIndex < 0 || nIndex >= GetCount())
				return;

			m_arrPoints[nIndex] = oPoint;
		}
		inline BOOL SetSize(int nSize, int nGrowBy = -1)
		{
			return m_arrPoints.SetCount(nSize, nGrowBy);
		}

		inline CTrackPoint& GetFirst()
		{
			return m_arrPoints[0];
		}
		inline CTrackPoint& GetLast()
		{
			return m_arrPoints[GetCount() - 1];
		}
		inline CTrackPoint& GetCurrentPoint()
		{
			return m_arrPoints[m_nCurrentPoint];
		}
		inline BOOL IsCurrentPointValid()
		{
			return (m_nCurrentPoint >= 0 && m_nCurrentPoint < GetCount());
		}

		inline CTrackPoints& operator= (const CTrackPoints& oPoints)
		{
			CopyFrom(oPoints);

			return *this;
		}
		inline void CopyFrom(const CTrackPoints& oPoints)
		{
			Clear();
			Add(oPoints);
		}

		virtual int FindByDouble(double dX, double dY, double dEpsilon = 0.5, BOOL bReverse = TRUE)
		{
			if (bReverse)
			{
				for (int index = GetCount() - 1; index >= 0; --index)
				{
					if (fabs(m_arrPoints[index].dX - dX) < dEpsilon && fabs(m_arrPoints[index].dY - dY) < dEpsilon)
						return index;
				}
			}
			else
			{
				for (int index = 0; index < GetCount(); ++index)
				{
					if (fabs(m_arrPoints[index].dX - dX) < dEpsilon && fabs(m_arrPoints[index].dY - dY) < dEpsilon)
						return index;
				}
			}

			return -1;
		}
		virtual int FindByInteger(int nX, int nY, int nEpsilon = 5, BOOL bReverse = TRUE)
		{
			if (bReverse)
			{
				for (int index = GetCount() - 1; index >= 0; --index)
				{
					if (fabs(m_arrPoints[index].nX - (double)nX) < nEpsilon && fabs(m_arrPoints[index].nY - (double)nY) < nEpsilon)
						return index;
				}
			}
			else
			{
				for (int index = 0; index < GetCount(); ++index)
				{
					if (fabs(m_arrPoints[index].nX - (double)nX) < nEpsilon && fabs(m_arrPoints[index].nY - (double)nY) < nEpsilon)
						return index;
				}
			}

			return -1;
		}

		inline void MapByDouble(double dL, double dT, double dR, double dB, int nL, int nT, int nR, int nB)
		{
			double dKoefX = (nR - nL)/(dR - dL);
			double dKoefY = (nB - nT)/(dB - dT);

			for (int index = 0; index < GetCount(); ++index)
			{
				m_arrPoints[index].nX = (int)(0.5 + nL + dKoefX*(m_arrPoints[index].dX - dL));
				m_arrPoints[index].nY = (int)(0.5 + nT + dKoefY*(m_arrPoints[index].dY - dT));
			}
		}
		inline void MapByInteger(int nL, int nT, int nR, int nB, double dL, double dT, double dR, double dB)
		{
			double dKoefX = (dR - dL)/(nR - nL);
			double dKoefY = (dB - dT)/(nB - nT);

			for (int index = 0; index < GetCount(); ++index)
			{
				m_arrPoints[index].dX = dL + dKoefX*(m_arrPoints[index].nX - nL);
				m_arrPoints[index].dY = dT + dKoefY*(m_arrPoints[index].nY - nT);
			}
		}
		inline void GetPointArray(POINT* pPoints, int nCount)
		{
			int nSize = min(nCount, GetCount());
			POINT* pPoint = pPoints;

			for (int index = 0; index < nSize; ++index, ++pPoint)
			{
				pPoint->x = m_arrPoints[index].nX;
				pPoint->y = m_arrPoints[index].nY;
			}
		}
		inline Structures::RECTI GetIntegerBoundingRect(int nIndexFirst = -1, int nIndexLast = -1)
		{
			if (nIndexFirst < 0)
				nIndexFirst = 0;
			if (nIndexLast < 0)
				nIndexLast = GetCount() - 1;
			if (nIndexLast >= GetCount())
				nIndexLast = GetCount() - 1;

			Structures::RECTI rectI;

			if (GetCount() < 0 || nIndexLast < nIndexFirst)
				return rectI;

			rectI.left = rectI.right = m_arrPoints[nIndexFirst].nX;
			rectI.top = rectI.bottom = m_arrPoints[nIndexFirst].nY;

			for (int index = nIndexFirst; index <= nIndexLast; ++index)
			{
				rectI.left = min(rectI.left, m_arrPoints[index].nX);
				rectI.top = min(rectI.top, m_arrPoints[index].nY);
				rectI.right = max(rectI.right, m_arrPoints[index].nX);
				rectI.bottom = max(rectI.bottom, m_arrPoints[index].nY);
#ifdef _DEBUG
	//			ATLTRACE (_T("LEFT : %d, TOP : %d, RIGHT : %d, BOTTOM : %d\n"), rectI.left, rectI.top, rectI.right, rectI.bottom );
#endif
			}

			return rectI;
		}
		inline Structures::RECTD GetUnrotatableRect( double Angle, int nIndexFirst = 0, int nIndexLast = 2, int nIndexCenter = 9)
		{
			Structures::RECTD rectD;

			double tX, tY;
			double cosA = cos( -Angle );
			double sinA = sin( -Angle );

			rectD.left   = m_arrPoints[nIndexFirst].dX;
			rectD.top    = m_arrPoints[nIndexFirst].dY;
			rectD.right  = m_arrPoints[nIndexLast].dX;
			rectD.bottom = m_arrPoints[nIndexLast].dY;

			tX = m_arrPoints[nIndexFirst].dX - m_arrPoints[nIndexCenter].dX;
			tY = m_arrPoints[nIndexFirst].dY - m_arrPoints[nIndexCenter].dY;				
			rectD.left  = m_arrPoints[nIndexCenter].dX + tX * cosA - tY * sinA;
			rectD.top   = m_arrPoints[nIndexCenter].dY + tX * sinA + tY * cosA;	

			tX = m_arrPoints[nIndexLast].dX - m_arrPoints[nIndexCenter].dX;
			tY = m_arrPoints[nIndexLast].dY - m_arrPoints[nIndexCenter].dY;
			rectD.right  = m_arrPoints[nIndexCenter].dX + tX * cosA - tY * sinA;
			rectD.bottom = m_arrPoints[nIndexCenter].dY + tX * sinA + tY * cosA;

			if( rectD.right < rectD.left ) 
			{ 
				double tmpRight		= rectD.right; 
				rectD.right			= rectD.left;
				rectD.left			= tmpRight;
			}
			if( rectD.top > rectD.bottom ) 
			{ 
				double tmpBottom	= rectD.bottom; 
				rectD.bottom		= rectD.top;
				rectD.top			= tmpBottom;
			}

			return rectD;
		}
		inline Structures::RECTD GetDoubleBoundingRect(int nIndexFirst = -1, int nIndexLast = -1)
		{
			if (nIndexFirst < 0)
				nIndexFirst = 0;
			if (nIndexLast < 0)
				nIndexLast = GetCount() - 1;
			if (nIndexLast >= GetCount())
				nIndexLast = GetCount() - 1;

			Structures::RECTD rectD;

			if (GetCount() < 0 || nIndexLast < nIndexFirst)
				return rectD;

			rectD.left = rectD.right = m_arrPoints[nIndexFirst].dX;
			rectD.top = rectD.bottom = m_arrPoints[nIndexFirst].dY;

			for (int index = nIndexFirst; index <= nIndexLast; ++index)
			{
				rectD.left = min(rectD.left, m_arrPoints[index].dX);
				rectD.top = min(rectD.top, m_arrPoints[index].dY);
				rectD.right = max(rectD.right, m_arrPoints[index].dX);
				rectD.bottom = max(rectD.bottom, m_arrPoints[index].dY);
			}

			return rectD;
		}
		inline void OffsetByDouble(double dOffsetX, double dOffsetY)
		{
			for (int index = 0; index < GetCount(); ++index)
			{
				m_arrPoints[index].dX += dOffsetX;
				m_arrPoints[index].dY += dOffsetY;
			}
		}
		inline void OffsetByInteger(int nOffsetX, int nOffsetY)
		{
			for (int index = 0; index < GetCount(); ++index)
			{
				m_arrPoints[index].nX += nOffsetX;
				m_arrPoints[index].nY += nOffsetY;
			}
		}


	protected:

		CAtlArray<CTrackPoint>	m_arrPoints;
		int						m_nCurrentPoint;
	};

	//------------------------------------------------------------------------------------------------------------------
	// Painter
	//------------------------------------------------------------------------------------------------------------------
	class CTrackPainter
	{
	public:
		CTrackPainter()
		{
			m_hDC				=	NULL;

			m_nColorLines1		=	-2;
			m_nColorLines2		=	-1;
			m_nColorPoints1		=	-2;
			m_nColorPoints2		=	-2;

			m_nPenLinesWidth	=	1;
			m_nPenPointsWidth	=	1;

			m_nPenLinesStyle	=	PS_DOT;
			m_nPenPointsStyle	=	PS_SOLID;

			m_nRotatePtnType	=	0;
			m_nRotatePtnWidth	=	1;

			m_nRotatePtnStyle	=	PS_SOLID;

			m_nRotatePtnColor1	=	-2;
			m_nRotatePtnColor2	=	-2;
		}

		virtual ~CTrackPainter()
		{
			ClearPainters ();
		}


		HDC GetDC()
		{
			return m_hDC;
		}
		BOOL Valid()
		{
			if (!m_hDC)
				return FALSE;

			return TRUE;
		}
		void SetColors(int nColorLines1, int nColorLines2, int nColorPoints1, int nColorPoints2)
		{
			m_nColorLines1 = nColorLines1;
			m_nColorLines2 = nColorLines2;
			m_nColorPoints1 = nColorPoints1;
			m_nColorPoints2 = nColorPoints2;
		}
		int GetColorLines1()
		{
			return m_nColorLines1;
		}
		int GetColorLines2()
		{
			return m_nColorLines2;
		}
		int GetColorPoints1()
		{
			return m_nColorPoints1;
		}
		int GetColorPoints2()
		{
			return m_nColorPoints2;
		}

		void Create(HDC hDC)
		{
			m_hDC	= hDC;

			m_oGdiPainter.Create ( m_hDC );
		}

		void Destroy ()
		{
			m_oGdiPainter.Destroy ();
		}


		BOOL DrawRectangle(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			Rendering::Gdi::DrawRectangle(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			return TRUE;
		}
		BOOL DrawRectangleRotatable( CTrackPoints* pTrack, int nRotationPointSize = 5, int nIndexRotationPoint = 8, int nIndexCenterPoint = 9 )
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			POINT* pPoints = new POINT[nIndexCenterPoint];
			pTrack->GetPointArray( pPoints, nIndexCenterPoint );

			Rendering::Gdi::DrawPolyline( m_hDC, pPoints, 4, TRUE );

			double posX = 0.0;
			double posY = 0.0;

			if( Geometry::GetLength((pPoints + nIndexRotationPoint)->x, (pPoints + nIndexRotationPoint)->y, (pPoints)->x, (pPoints)->y ) < Geometry::GetLength((pPoints + nIndexRotationPoint)->x, (pPoints + nIndexRotationPoint)->y, (pPoints + 3)->x, (pPoints + 3)->y ) )
			{
				posX = min( pPoints->x, (pPoints + 1)->x ) + abs( pPoints->x - (pPoints + 1)->x ) / 2.0;
				posY = min( pPoints->y, (pPoints + 1)->y ) + abs( pPoints->y - (pPoints + 1)->y ) / 2.0;
			}
			else
			{
				posX = min( (pPoints + 3)->x, (pPoints + 2)->x ) + abs( (pPoints + 3)->x - (pPoints + 2)->x ) / 2.0;
				posY = min( (pPoints + 3)->y, (pPoints + 2)->y ) + abs( (pPoints + 3)->y - (pPoints + 2)->y ) / 2.0;
			}

			// TODO : переделать на GDI обычный и вынести в отдельные настройки
			Rendering::Gdi::DrawLine( m_hDC, (int)(pPoints + nIndexRotationPoint)->x, (int)(pPoints + nIndexRotationPoint)->y, (int)posX, (int)posY );

			Rendering::Gdi::DrawRotatableSelector( m_hDC, (double)(pPoints + nIndexRotationPoint)->x, (double)(double)(pPoints + nIndexRotationPoint)->y, nRotationPointSize );

			if( pPoints )
			{
				delete []pPoints;
				pPoints = NULL;
			}
			return TRUE;
		}
		BOOL DrawEditText ( CTrackPoints* pTrack, double Zoom, CSimpleArray<CString> &lines, WCHAR* string, int posCaret, POINT selectInfo, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool isEdit, bool isScale  )
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid() || NULL == m_hDC)
				return FALSE;

			POINT* pPoints = new POINT[9];
			pTrack->GetPointArray( pPoints, 9 );

			Structures::RECTD rectD;
			rectD.left   = pPoints[0].x;
			rectD.top    = pPoints[0].y;
			rectD.right  = pPoints[2].x;
			rectD.bottom = pPoints[2].y;
			rectD.Normalize();

			Rendering::Gdi::DrawPolyline( m_hDC, pPoints, 4, TRUE );

			if( pPoints )
				delete []pPoints;

			int fontStyle = Gdiplus::FontStyleRegular;
			if(	fontType & FONTSTYLE_BOLD )			fontStyle |= Gdiplus::FontStyleBold;
			if( fontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( fontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( fontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)fontSize * (Gdiplus::REAL)Zoom, fontStyle, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat;

			switch( GetFontAlignedW( fontType ) )
			{
			case ALIGNED_LEFT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			case ALIGNED_CENTER:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter ); 
				break;
			case ALIGNED_RIGHT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentFar ); 
				break;
			default:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			}	
			int FontAlignW = byTop;
			switch( GetFontAlignedH( fontType ) )
			{
			case ALIGNED_TOP:
				FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				FontAlignW = byBottom;
				break;
			}

			if( isEdit )
			{
				if( isScale )
				{
					Structures::POINTD scale( 1.0, 1.0 );

					double maxWidthStr  = 0.0;
					double maxHeightStr = 0.0;

					RectF rectF, rectBB;

					double fontHeight = Rendering::Utils::GetFontHeight( &Gdiplus::Font( &fontFamily, 0.1f, fontStyle ) );
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						BSTR line = lines[i].AllocSysString();
						Rendering::Utils::MeasureString( m_hDC, line, -1, &Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), rectF, &rectBB ); 
						maxWidthStr = max( maxWidthStr, rectBB.Width );
						maxHeightStr += fontHeight;			
						SysFreeString( line );
					}

					if( maxWidthStr > 0 && maxHeightStr > 0 )
					{
						scale.x = rectD.GetWidth() / maxWidthStr;
						scale.y = rectD.GetHeight() / maxHeightStr;
					}
					
					Rendering::Gdi::DrawAngleScaleText( m_hDC, lines, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), FontColor, stringFormat, -M_PI_2, rectD ); 
					Rendering::Gdi::DrawScaleTextSelect ( m_hDC, lines, string, selectInfo.x, selectInfo.y, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), stringFormat, -M_PI_2, rectD, scale );
					Rendering::Gdi::DrawScaleCaret( m_hDC, lines, stringFormat, posCaret, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), -M_PI_2, rectD, scale ); 
				}
				else
				{
					Rendering::Gdi::DrawAngleText( m_hDC, lines, font, FontColor, stringFormat, FontAlignW, -M_PI_2, rectD ); 
					Rendering::Gdi::DrawTextSelect( m_hDC, lines, string, selectInfo.x, selectInfo.y, font, stringFormat, FontAlignW, -M_PI_2, rectD );
					Rendering::Gdi::DrawCaret( m_hDC, lines, stringFormat, FontAlignW, posCaret, font, -M_PI_2, rectD ); 
				}
			}
			return TRUE;
		}
		BOOL DrawEditTextRotatable ( CTrackPoints* pTrack, double Angle, double Zoom, CSimpleArray<CString> &lines, WCHAR* string, int posCaret, POINT selectInfo, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool isEdit, bool isScale )
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid() || NULL == m_hDC)
				return FALSE;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray( pPoints, 10 );

			Structures::RECTD rectD;

			Angle += M_PI_2;

			double tX, tY;
			double cosA = cos( -Angle );
			double sinA = sin( -Angle );

			rectD.left   = (pPoints + 0)->x;
			rectD.top    = (pPoints + 0)->y;
			rectD.right  = (pPoints + 2)->x;
			rectD.bottom = (pPoints + 2)->y;

			tX = (pPoints + 0)->x - (pPoints + 9)->x;
			tY = (pPoints + 0)->y - (pPoints + 9)->y;				
			rectD.left  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.top   = (pPoints + 9)->y + tX * sinA + tY * cosA;	

			tX = (pPoints + 2)->x - (pPoints + 9)->x;
			tY = (pPoints + 2)->y - (pPoints + 9)->y;
			rectD.right  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.bottom = (pPoints + 9)->y + tX * sinA + tY * cosA;

			if( rectD.right < rectD.left ) 
			{ 
				double tmpRight		= rectD.right; 
				rectD.right			= rectD.left;
				rectD.left			= tmpRight;
			}
			if( rectD.top > rectD.bottom ) 
			{ 
				double tmpBottom	= rectD.bottom; 
				rectD.bottom		= rectD.top;
				rectD.top			= tmpBottom;
			}	

			Rendering::Gdi::DrawPolyline( m_hDC, pPoints, 4, TRUE );
			int index = (Geometry::GetLength((pPoints + 8)->x, (pPoints + 8)->y, (pPoints + 4)->x, (pPoints + 4)->y ) > Geometry::GetLength((pPoints + 8)->x, (pPoints + 8)->y, (pPoints + 6)->x, (pPoints + 6)->y ) ) ? 6 : 4;
			Rendering::Gdi::DrawLine( m_hDC, (pPoints + 8)->x, (pPoints + 8)->y, (pPoints + index)->x, (pPoints + index)->y );
			Rendering::Gdi::DrawRotatableSelector( m_hDC, (pPoints + 8)->x, (pPoints + 8)->y, 5 );

			if( pPoints )
			{
				delete []pPoints; 
				pPoints = NULL;
			}

			int fontStyle = Gdiplus::FontStyleRegular;
			if(	fontType & FONTSTYLE_BOLD )			fontStyle |= Gdiplus::FontStyleBold;
			if( fontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( fontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( fontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)fontSize * (Gdiplus::REAL)Zoom, fontStyle, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat;

			switch( GetFontAlignedW( fontType ) )
			{
			case ALIGNED_LEFT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			case ALIGNED_CENTER:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter ); 
				break;
			case ALIGNED_RIGHT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentFar ); 
				break;
			default:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			}
			int FontAlignW = byTop;
			switch( GetFontAlignedH( fontType ) )
			{
			case ALIGNED_TOP:
				FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				FontAlignW = byBottom;
				break;
			}


			if( isEdit )
			{
				Angle -= M_PI_2;
				if( isScale )
				{
					Structures::POINTD scale( 1.0, 1.0 );

					double maxWidthStr  = 0.0;
					double maxHeightStr = 0.0;

					RectF rectF, rectBB;

					double fontHeight = Rendering::Utils::GetFontHeight( &Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ) );
					for( int i = 0; i < lines.GetSize(); ++i )
					{
						BSTR line = lines[i].AllocSysString();
						Rendering::Utils::MeasureString( m_hDC, line, -1, &Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), rectF, &rectBB ); 
						maxWidthStr = max( maxWidthStr, rectBB.Width );
						maxHeightStr += fontHeight;				
						SysFreeString( line );
					}

					if( maxWidthStr > 0 && maxHeightStr > 0 )
					{
						scale.x = rectD.GetWidth() / maxWidthStr;
						scale.y = rectD.GetHeight() / maxHeightStr;
					}

					Rendering::Gdi::DrawAngleScaleText( m_hDC, lines, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), FontColor, stringFormat, Angle, rectD ); 
					Rendering::Gdi::DrawScaleTextSelect( m_hDC, lines, string, selectInfo.x, selectInfo.y, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), stringFormat, Angle, rectD, scale );
					Rendering::Gdi::DrawScaleCaret( m_hDC, lines, stringFormat, posCaret, Gdiplus::Font( &fontFamily, 0.1f, fontStyle, Gdiplus::UnitPixel ), Angle, rectD, scale ); 
				}
				else
				{
					Rendering::Gdi::DrawAngleText( m_hDC, lines, font, FontColor, stringFormat, FontAlignW, Angle, rectD ); 
					Rendering::Gdi::DrawTextSelect( m_hDC, lines, string, selectInfo.x, selectInfo.y, font, stringFormat, FontAlignW, Angle, rectD );
					Rendering::Gdi::DrawCaret( m_hDC, lines, stringFormat, FontAlignW, posCaret, font, Angle, rectD ); 
				}
			}
			return TRUE;
		}

		BOOL DrawEditTextW ( CTrackPoints* pTrack, double Zoom, CSimpleArray<CStringW>& lines, WCHAR* string, int posCaret, POINT selectInfo, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool isEdit, bool isScale, bool EnableDrawText = true )
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid() || NULL == m_hDC)
				return FALSE;

			POINT* pPoints = new POINT[9];
			pTrack->GetPointArray( pPoints, 9 );

			Structures::RECTD rectD;
			rectD.left   = pPoints[0].x;
			rectD.top    = pPoints[0].y;
			rectD.right  = pPoints[2].x;
			rectD.bottom = pPoints[2].y;
			rectD.Normalize();

			Rendering::Gdi::DrawPolyline( m_hDC, pPoints, 4, TRUE );

			if( pPoints )
				delete []pPoints;

			int fontStyle = Gdiplus::FontStyleRegular;
			if(	fontType & FONTSTYLE_BOLD )			fontStyle |= Gdiplus::FontStyleBold;
			if( fontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( fontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( fontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;
			
			Gdiplus::StringFormat	stringFormat;
			
			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( FALSE == GetFont ( pFont, pFontFamily, fontName, (Gdiplus::REAL)fontSize * (Gdiplus::REAL)Zoom, fontStyle, Gdiplus::UnitPixel ) )
				return FALSE;

			switch ( GetFontAlignedW ( fontType ) )
			{
			case ALIGNED_LEFT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			case ALIGNED_CENTER:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter ); 
				break;
			case ALIGNED_RIGHT:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentFar ); 
				break;
			default:
				stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
				break;
			}	

			int FontAlignW = byTop;

			switch ( GetFontAlignedH ( fontType ) )
			{
			case ALIGNED_TOP:
				FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				FontAlignW = byBottom;
				break;
			}

			if ( isEdit )
			{
				if ( isScale )
				{
					Structures::POINTD scale ( 1.0, 1.0 );

					double maxWidthStr  = 0.0;
					double maxHeightStr = 0.0;

					RectF rectF, rectBB;

					Gdiplus::Font* pFont2				=	NULL;
					Gdiplus::FontFamily* pFontFamily2	=	NULL;

					if ( FALSE == GetFont ( pFont2, pFontFamily2, fontName, 0.1f, fontStyle, Gdiplus::UnitPoint ) )
					{
						RELEASEOBJECT ( pFont );
						RELEASEOBJECT ( pFontFamily );
						
						return FALSE;
					}

					double nFontHeight = Rendering::Utils::GetFontHeight ( pFont2 );
					
					RELEASEOBJECT ( pFont2 );
					RELEASEOBJECT ( pFontFamily2 );
				
					if ( FALSE == GetFont ( pFont2, pFontFamily2, fontName, 0.1f, fontStyle, Gdiplus::UnitPixel ) )
					{
						RELEASEOBJECT ( pFont );
						RELEASEOBJECT ( pFontFamily );
						
						return FALSE;
					}

					for ( int i = 0; i < lines.GetSize(); ++i )
					{
						Rendering::Utils::MeasureString ( m_hDC, lines[i], -1, pFont2, rectF, &rectBB ); 
						
						maxWidthStr		=	max ( maxWidthStr, rectBB.Width );
						maxHeightStr	+=	nFontHeight;			
					}

					if ( maxWidthStr > 0 && maxHeightStr > 0 )
					{
						scale.x = rectD.GetWidth() / maxWidthStr;
						scale.y = rectD.GetHeight() / maxHeightStr;
					}

					if ( EnableDrawText )
					{
						Rendering::TextRendering::DrawAngleScaleTextW	( m_hDC, lines, *pFont2, FontColor, stringFormat, -M_PI_2, rectD ); 
					}

					Rendering::TextRendering::DrawScaleTextSelectW		( m_hDC, lines, string, selectInfo.x, selectInfo.y, *pFont2, stringFormat, -M_PI_2, rectD, scale );
					Rendering::TextRendering::DrawScaleCaretW			( m_hDC, lines, stringFormat, posCaret, *pFont2, -M_PI_2, rectD, scale ); 

					RELEASEOBJECT ( pFont2 );
					RELEASEOBJECT ( pFontFamily2 );
				}
				else
				{
					if ( EnableDrawText )
					{
						Rendering::TextRendering::DrawAngleTextW	( m_hDC, lines, *pFont, FontColor, stringFormat, FontAlignW, -M_PI_2, rectD ); 
					}

					Rendering::TextRendering::DrawTextSelectW		( m_hDC, lines, string, selectInfo.x, selectInfo.y, *pFont, stringFormat, FontAlignW, -M_PI_2, rectD );
					Rendering::TextRendering::DrawCaretW			( m_hDC, lines, stringFormat, FontAlignW, posCaret, *pFont, -M_PI_2, rectD ); 
				}
			
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );
			}

			return TRUE;
		}

		BOOL DrawEditTextRotatableW ( CTrackPoints* pTrack, double Angle, double Zoom, CSimpleArray<CStringW>& lines, WCHAR* string, int posCaret, POINT selectInfo, CString& FontName, int fontSize, int fontType, int FontColor, bool isEdit, bool isScale, bool EnableDrawText = true )
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid() || NULL == m_hDC)
				return FALSE;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray ( pPoints, 10 );

			Structures::RECTD rectD;

			Angle += M_PI_2;

			double tX, tY;
			double cosA = cos( -Angle );
			double sinA = sin( -Angle );

			rectD.left   = (pPoints + 0)->x;
			rectD.top    = (pPoints + 0)->y;
			rectD.right  = (pPoints + 2)->x;
			rectD.bottom = (pPoints + 2)->y;

			tX = (pPoints + 0)->x - (pPoints + 9)->x;
			tY = (pPoints + 0)->y - (pPoints + 9)->y;				
			rectD.left  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.top   = (pPoints + 9)->y + tX * sinA + tY * cosA;	

			tX = (pPoints + 2)->x - (pPoints + 9)->x;
			tY = (pPoints + 2)->y - (pPoints + 9)->y;
			rectD.right  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.bottom = (pPoints + 9)->y + tX * sinA + tY * cosA;

			if( rectD.right < rectD.left ) 
			{ 
				double tmpRight		= rectD.right; 
				rectD.right			= rectD.left;
				rectD.left			= tmpRight;
			}
			if( rectD.top > rectD.bottom ) 
			{ 
				double tmpBottom	= rectD.bottom; 
				rectD.bottom		= rectD.top;
				rectD.top			= tmpBottom;
			}	

			Rendering::Gdi::DrawPolyline( m_hDC, pPoints, 4, TRUE );
			int index = (Geometry::GetLength((pPoints + 8)->x, (pPoints + 8)->y, (pPoints + 4)->x, (pPoints + 4)->y ) > Geometry::GetLength((pPoints + 8)->x, (pPoints + 8)->y, (pPoints + 6)->x, (pPoints + 6)->y ) ) ? 6 : 4;
			Rendering::Gdi::DrawLine( m_hDC, (pPoints + 8)->x, (pPoints + 8)->y, (pPoints + index)->x, (pPoints + index)->y );
			Rendering::Gdi::DrawRotatableSelector( m_hDC, (pPoints + 8)->x, (pPoints + 8)->y, 5 );

			if( pPoints )
			{
				delete [] pPoints; 
				pPoints = NULL;
			}

			if ( isEdit )
			{
				int fontStyle = Gdiplus::FontStyleRegular;

				if ( fontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
				if ( fontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
				if ( fontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
				if ( fontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

				//Gdiplus::FontFamily	fontFamily ( FontName );
				//Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)fontSize * (Gdiplus::REAL)Zoom, fontStyle, Gdiplus::UnitPixel );

				Gdiplus::Font* pFont				=	NULL;
				Gdiplus::FontFamily* pFontFamily	=	NULL;

				if ( FALSE == GetFont ( pFont, pFontFamily, FontName, (Gdiplus::REAL)fontSize * (Gdiplus::REAL)Zoom, fontStyle ) )
					return FALSE;

				Gdiplus::StringFormat	stringFormat;

				switch ( GetFontAlignedW ( fontType ) )
				{
				case ALIGNED_LEFT:
					stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
					break;
				case ALIGNED_CENTER:
					stringFormat.SetAlignment( Gdiplus::StringAlignmentCenter ); 
					break;
				case ALIGNED_RIGHT:
					stringFormat.SetAlignment( Gdiplus::StringAlignmentFar ); 
					break;
				default:
					stringFormat.SetAlignment( Gdiplus::StringAlignmentNear ); 
					break;
				}

				int FontAlignW = byTop;
				switch( GetFontAlignedH( fontType ) )
				{
				case ALIGNED_TOP:
					FontAlignW = byTop;
					break;
				case ALIGNED_MIDDLE:
					FontAlignW = byMiddle;
					break;
				case ALIGNED_BOTTOM:
					FontAlignW = byBottom;
					break;
				}

				Angle -= M_PI_2;
				
				if ( isScale )
				{
					Structures::POINTD scale( 1.0, 1.0 );

					double maxWidthStr  = 0.0;
					double maxHeightStr = 0.0;

					RectF rectF, rectBB;

					Gdiplus::Font* pFont2				=	NULL;
					Gdiplus::FontFamily* pFontFamily2	=	NULL;

					if ( FALSE == GetFont ( pFont2, pFontFamily2, FontName, 0.1f, fontStyle, Gdiplus::UnitPixel ) )
					{
						RELEASEOBJECT ( pFont );
						RELEASEOBJECT ( pFontFamily );
						
						return FALSE;
					}

					double nFontHeight = Rendering::Utils::GetFontHeight ( pFont2 );
				
					RELEASEOBJECT ( pFont2 );
					RELEASEOBJECT ( pFontFamily2 );
					
					if ( FALSE == GetFont ( pFont2, pFontFamily2, FontName, 0.1f, fontStyle, Gdiplus::UnitPixel ) )
					{
						RELEASEOBJECT ( pFont );
						RELEASEOBJECT ( pFontFamily );
						
						return FALSE;
					}
			
					for ( int i = 0; i < lines.GetSize(); ++i )
					{
						Rendering::Utils::MeasureString ( m_hDC, lines[i], -1, pFont2, rectF, &rectBB ); 
						
						maxWidthStr		=	max( maxWidthStr, rectBB.Width );
						maxHeightStr	+=	nFontHeight;				
					}

					if ( maxWidthStr > 0 && maxHeightStr > 0 )
					{
						scale.x = rectD.GetWidth() / maxWidthStr;
						scale.y = rectD.GetHeight() / maxHeightStr;
					}

					if ( EnableDrawText )
					{
						Rendering::TextRendering::DrawAngleScaleTextW	( m_hDC, lines, *pFont2, FontColor, stringFormat, Angle, rectD ); 
					}
					
					Rendering::TextRendering::DrawScaleTextSelectW		( m_hDC, lines, string, selectInfo.x, selectInfo.y, *pFont2, stringFormat, Angle, rectD, scale );
					Rendering::TextRendering::DrawScaleCaretW			( m_hDC, lines, stringFormat, posCaret, *pFont2, Angle, rectD, scale ); 
					
					RELEASEOBJECT ( pFont2 );
					RELEASEOBJECT ( pFontFamily2 );
				}
				else
				{
					if ( EnableDrawText )
					{
						Rendering::TextRendering::DrawAngleTextW		( m_hDC, lines, *pFont, FontColor, stringFormat, FontAlignW, Angle, rectD ); 
					}

					Rendering::TextRendering::DrawTextSelectW			( m_hDC, lines, string, selectInfo.x, selectInfo.y, *pFont, stringFormat, FontAlignW, Angle, rectD );
					Rendering::TextRendering::DrawCaretW				( m_hDC, lines, stringFormat, FontAlignW, posCaret, *pFont, Angle, rectD ); 
				}
			
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );
			}		

			return TRUE;
		}


		BOOL DrawEllipse(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 1 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			Rendering::Gdi::DrawEllipse(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			return TRUE;
		}
		BOOL DrawLine(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 2 || !Valid())
				return FALSE;

			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(0).nX, pTrack->GetAt(0).nY, pTrack->GetAt(1).nX, pTrack->GetAt(1).nY);

			return TRUE;
		}
		BOOL DrawPolyline(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 2 || !Valid())
				return FALSE;

			int nPointsCount = pTrack->GetCount();

			POINT* pPoints = new POINT[nPointsCount];

			pTrack->GetPointArray(pPoints, nPointsCount);

			Rendering::Gdi::DrawPolyline(m_hDC, pPoints, nPointsCount, FALSE);

			if (pPoints)
				delete []pPoints;

			return TRUE;
		}
		BOOL DrawPolygon(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 2 || !Valid())
				return FALSE;

			int nPointsCount = pTrack->GetCount();

			POINT* pPoints = new POINT[nPointsCount + 1];

			pTrack->GetPointArray(pPoints, nPointsCount);

			if (pTrack->GetCount() < 3)
				Rendering::Gdi::DrawPolyline(m_hDC, pPoints, nPointsCount, FALSE);
			else
			{
				pPoints[nPointsCount].x = pPoints[0].x;
				pPoints[nPointsCount].y = pPoints[0].y;

				Rendering::Gdi::DrawPolyline(m_hDC, pPoints, nPointsCount + 1, FALSE);
			}

			if (pPoints)
				delete []pPoints;

			return TRUE;
		}
		BOOL DrawTrajectoryPolyline(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 2 || !Valid())
				return FALSE;

			int nPointsCount = pTrack->GetCount();

			POINT* pPoints = new POINT[nPointsCount];

			pTrack->GetPointArray(pPoints, nPointsCount);

			Rendering::Gdi::DrawTrajectoryPolyline( m_hDC, pPoints, nPointsCount );

			if (pPoints)
				delete []pPoints;

			return TRUE;
		}
		BOOL DrawPie(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 5 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			Rendering::Gdi::DrawRectangle(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			// TODO: indexes of radial points for the pie
			int nPointIndexRadial1 = 4;
			int nPointIndexRadial2 = 5;
			/*
			// TODO: в DrawPie используется GetEllipsePointCoord без проверки
			Rendering::Gdi::DrawPie(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
			pTrack->GetAt(nPointIndexRadial1).nX, pTrack->GetAt(nPointIndexRadial1).nY,
			pTrack->GetAt(nPointIndexRadial2).nX, pTrack->GetAt(nPointIndexRadial2).nY);
			*/
			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexRadial1).nX, pTrack->GetAt(nPointIndexRadial1).nY,
				pTrack->GetAt(nPointIndexRadial2).nX, pTrack->GetAt(nPointIndexRadial2).nY);

			// get center
			int nCenterX = rectBounds.GetCenter().x;
			int nCenterY = rectBounds.GetCenter().y;

			// draw line from beg point of ellipse to center
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexRadial1).nX, pTrack->GetAt(nPointIndexRadial1).nY, nCenterX, nCenterY);

			// draw line from end point of ellipse to center
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexRadial2).nX, pTrack->GetAt(nPointIndexRadial2).nY, nCenterX, nCenterY);

			return TRUE;
		}
		BOOL DrawPieSimple(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 7 || !Valid())
				return FALSE;

			// indexes of radial points for the pie
			int nPointIndexRadial1 = 0;
			int nPointIndexRadial2 = 1;
			int nPointIndexCenter = 2;

			// get invisible rect on 3,4,5,6 points
			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(3, 6);

			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexRadial1).nX, pTrack->GetAt(nPointIndexRadial1).nY,
				pTrack->GetAt(nPointIndexRadial2).nX, pTrack->GetAt(nPointIndexRadial2).nY);

			// draw line from beg point of ellipse to center
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexRadial1).nX, pTrack->GetAt(nPointIndexRadial1).nY,
				pTrack->GetAt(nPointIndexCenter).nX, pTrack->GetAt(nPointIndexCenter).nY);

			// draw line from end point of ellipse to center
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexRadial2).nX, pTrack->GetAt(nPointIndexRadial2).nY,
				pTrack->GetAt(nPointIndexCenter).nX, pTrack->GetAt(nPointIndexCenter).nY);

			return TRUE;
		}
		BOOL DrawSector(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 14 || !Valid())
				return FALSE;

			// draw visible rect on 0,1,2,3 points
			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);
			Rendering::Gdi::DrawRectangle(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			// indexes of points for ellipses
			int nPointIndexOneEllipseBeg = 6;
			int nPointIndexOneEllipseEnd = 7;
			int nPointIndexTwoEllipseBeg = 4;
			int nPointIndexTwoEllipseEnd = 5;

			// draw first arc
			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexOneEllipseBeg).nX, pTrack->GetAt(nPointIndexOneEllipseBeg).nY,
				pTrack->GetAt(nPointIndexOneEllipseEnd).nX, pTrack->GetAt(nPointIndexOneEllipseEnd).nY);

			// get invisible rect on 8,9,10,11 points
			rectBounds = pTrack->GetIntegerBoundingRect(8, 11);

			// draw second arc
			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexTwoEllipseBeg).nX, pTrack->GetAt(nPointIndexTwoEllipseBeg).nY,
				pTrack->GetAt(nPointIndexTwoEllipseEnd).nX, pTrack->GetAt(nPointIndexTwoEllipseEnd).nY);

			// draw line from beg point of first ellipse to from beg point of second ellipse
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexOneEllipseBeg).nX, pTrack->GetAt(nPointIndexOneEllipseBeg).nY,
				pTrack->GetAt(nPointIndexTwoEllipseBeg).nX, pTrack->GetAt(nPointIndexTwoEllipseBeg).nY);

			// draw line from end point of first ellipse to from end point of second ellipse
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexOneEllipseEnd).nX, pTrack->GetAt(nPointIndexOneEllipseEnd).nY,
				pTrack->GetAt(nPointIndexTwoEllipseEnd).nX, pTrack->GetAt(nPointIndexTwoEllipseEnd).nY);

			return TRUE;
		}
		BOOL DrawSectorSimple(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 13 || !Valid())
				return FALSE;

			// get invisible rect on 5,6,7,8 points
			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(5, 8);

			// indexes of points for ellipses
			int nPointIndexOneEllipseBeg = 0;
			int nPointIndexOneEllipseEnd = 1;
			int nPointIndexTwoEllipseBeg = 2;
			int nPointIndexTwoEllipseEnd = 3;

			// draw first arc
			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexOneEllipseBeg).nX, pTrack->GetAt(nPointIndexOneEllipseBeg).nY,
				pTrack->GetAt(nPointIndexOneEllipseEnd).nX, pTrack->GetAt(nPointIndexOneEllipseEnd).nY);

			// get invisible rect on 9,10,11,12 points
			rectBounds = pTrack->GetIntegerBoundingRect(9, 12);

			// draw second arc
			Rendering::Gdi::DrawArc(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom, 
				pTrack->GetAt(nPointIndexTwoEllipseBeg).nX, pTrack->GetAt(nPointIndexTwoEllipseBeg).nY,
				pTrack->GetAt(nPointIndexTwoEllipseEnd).nX, pTrack->GetAt(nPointIndexTwoEllipseEnd).nY);

			// draw line from beg point of first ellipse to from beg point of second ellipse
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexOneEllipseBeg).nX, pTrack->GetAt(nPointIndexOneEllipseBeg).nY,
				pTrack->GetAt(nPointIndexTwoEllipseBeg).nX, pTrack->GetAt(nPointIndexTwoEllipseBeg).nY);

			// draw line from end point of first ellipse to from end point of second ellipse
			Rendering::Gdi::DrawLine(m_hDC, pTrack->GetAt(nPointIndexOneEllipseEnd).nX, pTrack->GetAt(nPointIndexOneEllipseEnd).nY,
				pTrack->GetAt(nPointIndexTwoEllipseEnd).nX, pTrack->GetAt(nPointIndexTwoEllipseEnd).nY);

			return TRUE;
		}
		BOOL DrawEllipseInRectangle(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 5 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			Rendering::Gdi::DrawRectangle(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			Rendering::Gdi::DrawEllipse(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			return TRUE;
		}
		BOOL DrawCircle(CTrackPoints* pTrack)
		{
			if (!pTrack || pTrack->GetCount() < 5 || !Valid())
				return FALSE;

			Structures::RECTI rectBounds = pTrack->GetIntegerBoundingRect(0, 3);

			Rendering::Gdi::DrawRectangle(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			Rendering::Gdi::DrawEllipse(m_hDC, rectBounds.left, rectBounds.top, rectBounds.right, rectBounds.bottom);

			return TRUE;
		}
		BOOL DrawPoints(CTrackPoints* pTrack, int nCount, int nSize = 5)
		{
			if (!pTrack || !Valid())
				return FALSE;

			if (nCount < 0)
				nCount = pTrack->GetCount();
			if (nCount >= pTrack->GetCount())
				nCount = pTrack->GetCount();

			for (int index = 0; index < nCount; ++index)
				Rectangle(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			//for (int index = 0; index < nCount; ++index)
			//	Ellipse(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			return TRUE;
		}
		BOOL DrawTrajectoryPoints(CTrackPoints* pTrack, int nCount, int nSize = 10)
		{
			if (!pTrack || !Valid())
				return FALSE;

			if( nCount < 2 || NULL == m_hDC ) return FALSE;
			Gdiplus::Graphics graphics( m_hDC );

			if (nCount < 0)
				nCount = pTrack->GetCount();
			if (nCount >= pTrack->GetCount())
				nCount = pTrack->GetCount();

			for( int index = 0; index < nCount; ++index )
				Rendering::Gdi::DrawTrajectoryPoint( &graphics, 
				Gdiplus::RectF ( 
				(Gdiplus::REAL)pTrack->GetAt(index).nX - (Gdiplus::REAL)nSize / 2.0f, 
				(Gdiplus::REAL)pTrack->GetAt(index).nY - (Gdiplus::REAL)nSize / 2,
				(Gdiplus::REAL)nSize, 
				(Gdiplus::REAL)nSize ) );

			//Rectangle(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);


			return TRUE;
		}

		BOOL DrawPointsCubicBezierCurve ( CTrackPoints* pTrack, int nCount, int nSize = 5 )
		{
			if (!pTrack || !Valid())
				return FALSE;

			if (nCount < 0)
				nCount = pTrack->GetCount();
			if (nCount >= pTrack->GetCount())
				nCount = pTrack->GetCount();

			//int index = 0;
			//Ellipse(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			for ( int index = 0; index < nCount-1; index += 3 )
				Rectangle(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			return TRUE;
		}

		BOOL DrawPointsCubicBezierCurveGdiPlus ( CTrackPoints* pTrack, int nCount, int nSize = 5 )
		{
			if (!pTrack || !Valid())
				return FALSE;

			if (nCount < 0)
				nCount = pTrack->GetCount();
			if (nCount >= pTrack->GetCount())
				nCount = pTrack->GetCount();

			Gdiplus::Graphics graphics( m_hDC );
			if ( graphics.GetLastStatus () == Gdiplus::Ok )
			{
				Gdiplus::Color Color01;	Color01.SetValue (m_nColorPoints1);		
				Gdiplus::Pen			BorderPen (Color01, (Gdiplus::REAL)2 );

				Gdiplus::Color Color02;	Color02.SetValue (m_nColorPoints2);		
				Gdiplus::SolidBrush	SolidBrush(Color02);

				graphics.SetSmoothingMode ( Gdiplus::SmoothingModeHighQuality );

				for ( int index = 0; index < nCount-1; index += 3 )
				{
					Gdiplus::RectF oRect ( (Gdiplus::REAL)pTrack->GetAt(index).nX - (Gdiplus::REAL)nSize * 0.5f, 
						(Gdiplus::REAL)pTrack->GetAt(index).nY - (Gdiplus::REAL)nSize * 0.5f,
						(Gdiplus::REAL)nSize, (Gdiplus::REAL)nSize );

					graphics.DrawEllipse ( &BorderPen,  oRect );
					graphics.FillEllipse ( &SolidBrush, oRect );
				}
			}

			//int index = 0;
			//Ellipse(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			//for ( int index = 0; index < nCount-1; index += 3 )
			//	Rectangle(m_hDC, pTrack->GetAt(index).nX - nSize - 1, pTrack->GetAt(index).nY - nSize - 1, pTrack->GetAt(index).nX + nSize, pTrack->GetAt(index).nY + nSize);

			return TRUE;
		}


		BOOL DrawPointCubicBezierCurve ( CTrackPoint point, int nSize = 5 )
		{
			Gdiplus::Graphics graphics( m_hDC );
			if ( graphics.GetLastStatus () == Gdiplus::Ok )
			{
				Gdiplus::Color Color01;	Color01.SetValue (m_nColorPoints1);		
				Gdiplus::Pen			BorderPen (Color01, (Gdiplus::REAL)2 );

				Gdiplus::Color Color02;	Color02.SetValue (m_nColorPoints2);		
				Gdiplus::SolidBrush	SolidBrush(Color02);

				graphics.SetSmoothingMode( Gdiplus::SmoothingModeHighQuality );
				Gdiplus::RectF oRect ( (Gdiplus::REAL)point.nX - (Gdiplus::REAL)nSize * 0.5f, 
					(Gdiplus::REAL)point.nY - (Gdiplus::REAL)nSize * 0.5f,
					(Gdiplus::REAL)nSize, (Gdiplus::REAL)nSize );

				graphics.DrawEllipse ( &BorderPen,  oRect );
				graphics.FillEllipse ( &SolidBrush, oRect );
			}

			//Ellipse ( m_hDC, point.nX - nSize - 1, point.nY - nSize - 1, point.nX + nSize, point.nY + nSize );
			//Rectangle ( m_hDC, point.nX - nSize - 1, point.nY - nSize - 1, point.nX + nSize, point.nY + nSize );

			return TRUE;
		}

		BOOL DrawCubicBezierCurve ( CTrackPoints* pTrack )
		{
			if ( NULL == pTrack || pTrack->GetCount() < 2 || !Valid() )
				return FALSE;

			int nPointsCount = pTrack->GetCount ();

			/*

			// если цвет задается BGR - то рисуем использую схему GDI
			if ( (m_nColorLines1 > 0x00FFFFFF && m_nColorLines2 > 0x00FFFFFF) )
			{
			// Paint with gdi
			POINT* pPoints = new POINT [ nPointsCount ];
			if ( NULL != pPoints )
			{
			pTrack->GetPointArray ( pPoints, nPointsCount );

			POINT Points [4];
			for ( int Index = 0; Index < nPointsCount; Index += 3 )
			{
			if ( Index + 3 > nPointsCount )
			break;

			// узловые точки - pPoints [ Index ] и pPoints [ Index + 3 ]
			// точки параллели текущей модицикационной точкой - pPoints [ Index + 1 ] и pPoints [ Index + 2 ]
			#ifdef _DEBUG
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index].x, pPoints[Index].y, pPoints[Index+1].x, pPoints[Index+1].y );
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index+1].x, pPoints[Index+1].y, pPoints[Index+2].x, pPoints[Index+2].y );
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index+2].x, pPoints[Index+2].y, pPoints[Index+3].x, pPoints[Index+3].y );
			#endif
			memcpy ( Points, pPoints + Index, sizeof (POINT) * 4L );
			::PolyBezier ( m_hDC, Points, 4L );
			}

			delete [] pPoints;
			}
			}
			else
			*/

			{	
				// Paint with gdi+

				Gdiplus::Graphics oGdiPlusPainter ( m_hDC );

				Gdiplus::GraphicsPath	BezierPath;

				Gdiplus::Color Color01;	Color01.SetValue (m_nColorLines1);						// Color01.SetFromCOLORREF(m_nColorLines1);
				Gdiplus::Pen			PenColor (Color01, (Gdiplus::REAL)m_nPenLinesWidth );	// Gdiplus::Color ( 255, 0, 255, 0 ), 2 );

				Gdiplus::Color Color02;	Color02.SetValue (m_nColorLines2);						// Color02.SetFromCOLORREF(m_nColorLines2);
				Gdiplus::Pen			PenCurve ( Color02, (Gdiplus::REAL)m_nPenLinesWidth );	// Gdiplus::Color ( 128, 0, 0, 0 ), 2 );

				// set another (not standart) cap arrow
				Gdiplus::GraphicsPath    ArrowPath;

				ArrowPath.AddLine ( Gdiplus::Point (  0, -1 ),	Gdiplus::Point ( -2, -5 ) );
				ArrowPath.AddLine ( Gdiplus::Point ( -2, -5 ),	Gdiplus::Point (  2, -5 ) );
				ArrowPath.AddLine ( Gdiplus::Point (  2, -5 ),	Gdiplus::Point (  0, -1 ) );

				//ArrowPath.AddLine ( Gdiplus::Point (  0, -1 ),	Gdiplus::Point ( -2, -4 ) );
				//ArrowPath.AddLine ( Gdiplus::Point ( -2, -4 ),	Gdiplus::Point (  2, -4 ) );
				//ArrowPath.AddLine ( Gdiplus::Point (  2, -4 ),	Gdiplus::Point (  0, -1 ) );

				Gdiplus::CustomLineCap   CustomCapArrow ( &ArrowPath, NULL );

				PenColor.SetStartCap ( Gdiplus::LineCapRoundAnchor );
				//PenColor.SetEndCap ( Gdiplus::LineCapArrowAnchor );
				PenColor.SetCustomEndCap ( &CustomCapArrow );
				PenColor.SetLineJoin ( Gdiplus::LineJoinRound );

				PenCurve.SetCustomEndCap ( &CustomCapArrow );
				PenCurve.SetLineJoin ( Gdiplus::LineJoinRound );

				for ( int i = 0; i < nPointsCount && i + 3 < nPointsCount; i += 3 )
				{
					BezierPath.AddBezier (	
						(Gdiplus::REAL) pTrack->GetAt ( 0 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 0 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 1 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 1 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 2 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 2 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 3 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 3 + i ).nY );
				}

				oGdiPlusPainter.SetSmoothingMode ( Gdiplus::SmoothingModeHighQuality );

				oGdiPlusPainter.TranslateTransform ( 2.0f, 2.0f );
				oGdiPlusPainter.DrawPath ( &PenCurve, &BezierPath );
				oGdiPlusPainter.ResetTransform ();
				oGdiPlusPainter.DrawPath( &PenColor, &BezierPath );
			}

			return TRUE;
		}



		BOOL DrawCubicBezierCurveGdiPlus ( CTrackPoints* pTrack )
		{
			if ( NULL == pTrack || pTrack->GetCount() < 2 || !Valid() )
				return FALSE;

			int nPointsCount = pTrack->GetCount ();

			/*

			// если цвет задается BGR - то рисуем использую схему GDI
			if ( (m_nColorLines1 > 0x00FFFFFF && m_nColorLines2 > 0x00FFFFFF) )
			{
			// Paint with gdi
			POINT* pPoints = new POINT [ nPointsCount ];
			if ( NULL != pPoints )
			{
			pTrack->GetPointArray ( pPoints, nPointsCount );

			POINT Points [4];
			for ( int Index = 0; Index < nPointsCount; Index += 3 )
			{
			if ( Index + 3 > nPointsCount )
			break;

			// узловые точки - pPoints [ Index ] и pPoints [ Index + 3 ]
			// точки параллели текущей модицикационной точкой - pPoints [ Index + 1 ] и pPoints [ Index + 2 ]
			#ifdef _DEBUG
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index].x, pPoints[Index].y, pPoints[Index+1].x, pPoints[Index+1].y );
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index+1].x, pPoints[Index+1].y, pPoints[Index+2].x, pPoints[Index+2].y );
			//Rendering::Gdi::DrawLine ( m_hDC, pPoints[Index+2].x, pPoints[Index+2].y, pPoints[Index+3].x, pPoints[Index+3].y );
			#endif
			memcpy ( Points, pPoints + Index, sizeof (POINT) * 4L );
			::PolyBezier ( m_hDC, Points, 4L );
			}

			delete [] pPoints;
			}
			}
			else
			*/

			{	
				// Paint with gdi+

				Gdiplus::Graphics oGdiPlusPainter ( m_hDC );

				Gdiplus::GraphicsPath	BezierPath;

				Gdiplus::Color Color01;	Color01.SetValue (m_nColorLines1);						// Color01.SetFromCOLORREF(m_nColorLines1);
				Gdiplus::Pen			PenColor (Color01, (Gdiplus::REAL)m_nPenLinesWidth );	// Gdiplus::Color ( 255, 0, 255, 0 ), 2 );

				Gdiplus::Color Color02;	Color02.SetValue (m_nColorLines2);						// Color02.SetFromCOLORREF(m_nColorLines2);
				Gdiplus::Pen			PenCurve ( Color02, (Gdiplus::REAL)m_nPenLinesWidth );	// Gdiplus::Color ( 128, 0, 0, 0 ), 2 );

				// set another (not standart) cap arrow
				Gdiplus::GraphicsPath    ArrowPath;

				ArrowPath.AddLine ( Gdiplus::Point (  0, -1 ),	Gdiplus::Point ( -2, -5 ) );
				ArrowPath.AddLine ( Gdiplus::Point ( -2, -5 ),	Gdiplus::Point (  2, -5 ) );
				ArrowPath.AddLine ( Gdiplus::Point (  2, -5 ),	Gdiplus::Point (  0, -1 ) );

				//ArrowPath.AddLine ( Gdiplus::Point (  0, -1 ),	Gdiplus::Point ( -2, -4 ) );
				//ArrowPath.AddLine ( Gdiplus::Point ( -2, -4 ),	Gdiplus::Point (  2, -4 ) );
				//ArrowPath.AddLine ( Gdiplus::Point (  2, -4 ),	Gdiplus::Point (  0, -1 ) );

				Gdiplus::CustomLineCap   CustomCapArrow ( &ArrowPath, NULL );

				PenColor.SetStartCap ( Gdiplus::LineCapRoundAnchor );
				//PenColor.SetEndCap ( Gdiplus::LineCapArrowAnchor );
				PenColor.SetCustomEndCap ( &CustomCapArrow );
				PenColor.SetLineJoin ( Gdiplus::LineJoinRound );

				PenCurve.SetCustomEndCap ( &CustomCapArrow );
				PenCurve.SetLineJoin ( Gdiplus::LineJoinRound );

				for ( int i = 0; i < nPointsCount && i + 3 < nPointsCount; i += 3 )
				{
					BezierPath.AddBezier (	
						(Gdiplus::REAL) pTrack->GetAt ( 0 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 0 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 1 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 1 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 2 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 2 + i ).nY,
						(Gdiplus::REAL) pTrack->GetAt ( 3 + i ).nX, (Gdiplus::REAL) pTrack->GetAt ( 3 + i ).nY );
				}

				oGdiPlusPainter.SetSmoothingMode ( Gdiplus::SmoothingModeHighQuality );

				oGdiPlusPainter.TranslateTransform ( 2.0f, 2.0f );
				oGdiPlusPainter.DrawPath ( &PenCurve, &BezierPath );
				oGdiPlusPainter.ResetTransform ();
				oGdiPlusPainter.DrawPath( &PenColor, &BezierPath );
			}

			return TRUE;
		}



		void SetPenLines ()
		{
			m_oGdiPainter.SetPen ( m_nColorLines1, m_nColorLines2, m_nPenLinesWidth, m_nPenLinesStyle );
		}

		void SetPenPoints ()
		{
			m_oGdiPainter.SetPen ( m_nColorPoints1, m_nColorPoints2, m_nPenPointsWidth, m_nPenPointsStyle );
		}



		void SetLinesProperty ( int	nWidth, int	nStyle, DWORD nColor1, DWORD nColor2, int nType )
		{
			m_nPenLinesStyle	=	nType;

			m_nPenLinesWidth	=	nWidth;

			m_nPenLinesStyle	=	nStyle; 

			m_nColorLines1		=	nColor1;
			m_nColorLines2		=	nColor2;
		}

		void SetPointsProperty ( int nWidth, int nStyle, DWORD nColor1, DWORD nColor2, int nType )
		{
			m_nPenPointsStyle	=	nType;

			m_nPenPointsWidth	=	nWidth;

			m_nPenPointsStyle	=	nStyle;

			m_nColorPoints1		=	nColor1;
			m_nColorPoints2		=	nColor2;
		}

		void GetLinesProperty ( int& nWidth, int& nStyle, DWORD& nColor1, DWORD& nColor2, int& nType )
		{
			nType				=	m_nPenLinesStyle;

			nWidth				=	m_nPenLinesWidth;

			nStyle				= 	m_nPenLinesStyle;

			nColor1				=	m_nColorLines1;	
			nColor2				=	m_nColorLines2;
		}

		void GetPointsProperty ( int& nWidth, int& nStyle, DWORD& nColor1, DWORD& nColor2, int& nType )
		{
			nType				=	m_nPenPointsType;

			nWidth				=	m_nPenPointsWidth;

			nStyle				=	m_nPenPointsStyle;

			nColor1				=	m_nColorPoints1;
			nColor2				=	m_nColorPoints2;
		}


		void SetRotatePointProperty ( int nWidth, int nStyle, DWORD nColor1, DWORD nColor2, int nType )
		{
			m_nRotatePtnType	=	nType;

			m_nRotatePtnWidth	=	nWidth;

			m_nRotatePtnStyle	=	nStyle;

			m_nRotatePtnColor1	=	nColor1;
			m_nRotatePtnColor2	=	nColor2;
		}

		void GetRotatePointProperty ( int& nWidth, int& nStyle, DWORD& nColor1, DWORD& nColor2, int& nType )
		{
			nType				=	m_nRotatePtnType;	

			nWidth				=	m_nRotatePtnWidth;

			nStyle				= 	m_nRotatePtnStyle;

			nColor1				=	m_nRotatePtnColor1;
			nColor2				=	m_nRotatePtnColor2;
		}							



		int GetPointsWidth ()
		{
			return m_nPenPointsWidth;
		}

		void AddInnerPainter ( CTrackPainter* pPainter )
		{
			m_arrInnerPainters.Add ( pPainter );
		}

		CTrackPainter* GetInnerPainter ( int Ind )
		{
			if ( Ind >= (int)m_arrInnerPainters.GetCount () )
				return NULL;

			return m_arrInnerPainters [ Ind ];
		}

		int CountInnerPainters ()
		{
			return (int)m_arrInnerPainters.GetCount ();
		}

	protected:

		void ClearPainters ()
		{
			for ( int i = 0; i < (int)m_arrInnerPainters.GetCount (); ++i )
			{
				if ( NULL != m_arrInnerPainters[i] )
					delete m_arrInnerPainters[i];
			}

			m_arrInnerPainters.RemoveAll ();
		}


		inline BOOL	GetFont ( Gdiplus::Font*& pFont, Gdiplus::FontFamily*& pFontFamily, CString FontName, Gdiplus::REAL FontSize, INT FontStyle, Gdiplus::Unit UnitStyle = Gdiplus::UnitPixel )
		{
			pFontFamily	=	new Gdiplus::FontFamily ( FontName );
			pFont		=	new Gdiplus::Font ( pFontFamily, FontSize, FontStyle, UnitStyle );

			if ( NULL == pFont && NULL == pFontFamily )
				return FALSE;

			if ( Gdiplus::Ok != pFont->GetLastStatus () )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );
			
				pFont	=	new Gdiplus::Font ( FontName, FontSize, FontStyle, Gdiplus::UnitPixel );
			
				if ( NULL == pFont )
					return FALSE;
			}

			return TRUE;
		}


	protected:

		HDC							m_hDC;

		int							m_nPenLinesWidth;
		int							m_nPenPointsWidth;

		int							m_nPenLinesType;
		int							m_nPenPointsType;

		int							m_nPenLinesStyle; 
		int							m_nPenPointsStyle;

		DWORD 						m_nColorLines1;
		DWORD 						m_nColorLines2;
		DWORD 						m_nColorPoints1;
		DWORD 						m_nColorPoints2;

		int							m_nRotatePtnType;
		int							m_nRotatePtnWidth;
		int							m_nRotatePtnStyle;
		DWORD						m_nRotatePtnColor1;
		DWORD						m_nRotatePtnColor2;

		Rendering::Gdi::CGdiPainter m_oGdiPainter;

		CAtlArray<CTrackPainter*>	m_arrInnerPainters;
	};
}

namespace Tracking
{
	// Залипание трекинга
	class CTrackSticking
	{
	public:
		// коды операции с трекингом, нужны для Round Stricking
		static const int nOperationMoveTrack	=	1;		// подвинули весь трек
		static const int nOperationMovePoint	=	2;		// подвинули точку трека
	public:

		CTrackSticking ()
		{
			m_bStickVirtualPixels	=	false;	
			m_bStickRealPixels		=	false;		

			m_nUnitSize				=	0;	
			m_nZoom					=	1.0;

			m_nAlingX				=	0.0;
			m_nAlingY				=	0.0;
		
			m_bRoundStickingLeft	=	false;
			m_bRoundStickingTop		=	false;
			m_bRoundStickingRight	=	false;
			m_bRoundStickingBottom	=	false;
			
			m_nImageWidth			=	0;
			m_nImageHeight			=	0;
		}

		inline void DoWork ( const int& TrackType, CTrackPoints* pPoints, const int& Operation = CTrackSticking::nOperationMovePoint )
		{
			if ( ( false == m_bStickVirtualPixels && false == m_bStickRealPixels ) || ( 0 == m_nUnitSize ) )
				return;

			if ( m_bStickVirtualPixels )
			{
				if ( m_bRoundStickingLeft || m_bRoundStickingTop || m_bRoundStickingRight || m_bRoundStickingBottom )
				{	 
					DoStickingOnSides ( TrackType, pPoints, Operation );

					return;
				}

				int CountPoints	=	0;

				if ( Tracking::Constants::sc_nTrackTypeEllipse == TrackType )
					CountPoints	=	5;

				if ( Tracking::Constants::sc_nTrackTypeRectangle == TrackType )
				{
					CorrectPoint ( pPoints->GetAt ( 0 ) );
					CorrectPoint ( pPoints->GetAt ( 1 ) );
					CorrectPoint ( pPoints->GetAt ( 2 ) );
					CorrectPoint ( pPoints->GetAt ( 3 ) );

					SetPointMiddle ( pPoints, 4, 0, 1 );
					SetPointMiddle ( pPoints, 5, 1, 2 );
					SetPointMiddle ( pPoints, 6, 2, 3 );
					SetPointMiddle ( pPoints, 7, 0, 3 );

					SetPointMiddle ( pPoints, 8, 0, 2 );

					return;
				}

				if ( Tracking::Constants::sc_nTrackTypeRectangleRotatableFixedAspect	== TrackType )
				{
					/*
					// прилипание к центру изображения трека
					*/

					double dX = pPoints->GetAt ( 5 ).dX;
					double dY = pPoints->GetAt ( 5 ).dY;

					long nX = pPoints->GetAt ( 5 ).nX;
					long nY = pPoints->GetAt ( 5 ).nY;

					if ( m_nImageWidth	* 0.5 - m_nUnitSize * 0.5 <= pPoints->GetAt ( 5 ).dX &&
						m_nImageWidth	* 0.5 + m_nUnitSize * 0.5 >= pPoints->GetAt ( 5 ).dX &&
						m_nImageHeight * 0.5 - m_nUnitSize * 0.5 <= pPoints->GetAt ( 5 ).dY &&
						m_nImageHeight * 0.5 + m_nUnitSize * 0.5 >= pPoints->GetAt ( 5 ).dY )
					{
						pPoints->GetAt ( 5 ).dX = m_nImageWidth		* 0.5;
						pPoints->GetAt ( 5 ).dY = m_nImageHeight	* 0.5;

						double OffSetX = dX - pPoints->GetAt ( 5 ).dX;
						double OffSetY = dY - pPoints->GetAt ( 5 ).dY;

						double OffSetNX = nX - pPoints->GetAt ( 5 ).nX;
						double OffSetNY = nY - pPoints->GetAt ( 5 ).nY;

						for ( int i = 0; i < 5; ++i )
						{
							pPoints->GetAt ( i ).dX -= OffSetX;
							pPoints->GetAt ( i ).dY -= OffSetY;

							pPoints->GetAt ( i ).nX -= (int)OffSetNX;
							pPoints->GetAt ( i ).nY -= (int)OffSetNY;
						}
					}

					return;
					
					/*
					// Прилипание трека центром к точкам

					double dX = pPoints->GetAt ( 5 ).dX;
					double dY = pPoints->GetAt ( 5 ).dY;

					long nX = pPoints->GetAt ( 5 ).nX;
					long nY = pPoints->GetAt ( 5 ).nY;

					CorrectPoint ( pPoints->GetAt ( 5 ) );

					double OffSetX = dX - pPoints->GetAt ( 5 ).dX;
					double OffSetY = dY - pPoints->GetAt ( 5 ).dY;
					
					double OffSetNX = nX - pPoints->GetAt ( 5 ).nX;
					double OffSetNY = nY - pPoints->GetAt ( 5 ).nY;

					for ( int i = 0; i < 5; ++i )
					{
						pPoints->GetAt ( i ).dX -= OffSetX;
						pPoints->GetAt ( i ).dY -= OffSetY;
					
						pPoints->GetAt ( i ).nX -= OffSetNX;
						pPoints->GetAt ( i ).nY -= OffSetNY;
					}

					return;
					*/
				}

				if ( Tracking::Constants::sc_nTrackTypePie == TrackType )
					CountPoints	=	4;

				if ( Tracking::Constants::sc_nTrackTypeSector == TrackType )
					CountPoints	=	4;

				if ( Tracking::Constants::sc_nTrackTypeEllipseInRectangle == TrackType )
				{
					CorrectPoint ( pPoints->GetAt ( 0 ) );
					CorrectPoint ( pPoints->GetAt ( 1 ) );
					CorrectPoint ( pPoints->GetAt ( 2 ) );
					CorrectPoint ( pPoints->GetAt ( 3 ) );

					SetPointMiddle ( pPoints, 4, 0, 2 );

					return;
				}
			
				if ( Tracking::Constants::sc_nTrackTypeCircleInRectangle == TrackType )
				{
					double dCenterX	= 0.5 * ( pPoints->GetAt(0).dX + pPoints->GetAt(1).dX );
					double dCenterY = 0.5 * ( pPoints->GetAt(0).dY + pPoints->GetAt(3).dY );

					double dRadiusX = fabs ( dCenterX - pPoints->GetAt(0).dX );
					double dRadiusY = fabs ( dCenterY - pPoints->GetAt(0).dY );

					CorrectPoint ( pPoints->GetAt ( 4 ) );

					pPoints->GetAt ( 0 ).Create ( pPoints->GetAt ( 4 ).dX - dRadiusX, pPoints->GetAt ( 4 ).dY - dRadiusY );
					pPoints->GetAt ( 1 ).Create ( pPoints->GetAt ( 4 ).dX + dRadiusX, pPoints->GetAt ( 4 ).dY - dRadiusY );
					pPoints->GetAt ( 2 ).Create ( pPoints->GetAt ( 4 ).dX + dRadiusX, pPoints->GetAt ( 4 ).dY + dRadiusY );
					pPoints->GetAt ( 3 ).Create ( pPoints->GetAt ( 4 ).dX - dRadiusX, pPoints->GetAt ( 4 ).dY + dRadiusY );

					return;
				}

				if ( Tracking::Constants::sc_nTrackTypeCircleInRectangle == TrackType )
				{
					double dCenterX	= 0.5 * ( pPoints->GetAt(0).dX + pPoints->GetAt(1).dX );
					double dCenterY = 0.5 * ( pPoints->GetAt(0).dY + pPoints->GetAt(3).dY );

					double dRadiusX = fabs ( dCenterX - pPoints->GetAt(0).dX );
					double dRadiusY = fabs ( dCenterY - pPoints->GetAt(0).dY );

					CorrectPoint ( pPoints->GetAt ( 4 ) );

					pPoints->GetAt ( 0 ).Create ( pPoints->GetAt ( 4 ).dX - dRadiusX, pPoints->GetAt ( 4 ).dY - dRadiusY );
					pPoints->GetAt ( 1 ).Create ( pPoints->GetAt ( 4 ).dX + dRadiusX, pPoints->GetAt ( 4 ).dY - dRadiusY );
					pPoints->GetAt ( 2 ).Create ( pPoints->GetAt ( 4 ).dX + dRadiusX, pPoints->GetAt ( 4 ).dY + dRadiusY );
					pPoints->GetAt ( 3 ).Create ( pPoints->GetAt ( 4 ).dX - dRadiusX, pPoints->GetAt ( 4 ).dY + dRadiusY );

					return;
				}

				for ( int i = 0; i < CountPoints; ++i )
				{
					CorrectPoint ( pPoints->GetAt ( i ) );
				}

				//static const int sc_nTrackTypeNone							= 0;						
				//static const int sc_nTrackTypePoint							= 1;		
				//static const int sc_nTrackTypeLine							= 2;
				//static const int sc_nTrackTypeEllipse							= 3;		+
				//static const int sc_nTrackTypeRectangle						= 4;		+
				//static const int sc_nTrackTypePolyline						= 5;-
				//static const int sc_nTrackTypePolygon							= 6;-
				//static const int sc_nTrackTypeRectangleFixedSize				= 7;		+
				//static const int sc_nTrackTypePie								= 8;
				//static const int sc_nTrackTypeSector							= 9;
				//static const int sc_nTrackTypeSectorSimple					= 10;
				//static const int sc_nTrackTypePieSimple						= 11;
				//static const int sc_nTrackTypeRectangleFixedAspect			= 12;
				//static const int sc_nTrackTypeEllipseInRectangle				= 13;
				//static const int sc_nTrackTypeCircleInRectangle				= 14;
				//static const int sc_nTrackTypeRectangleRotatable				= 15;
				//static const int sc_nTrackTypeTextRotatable					= 16;
				//static const int sc_nTrackTypeText							= 17;
				//static const int sc_nTrackTypeRectangleRotatableFixedAspect	= 18;
				//static const int sc_nTrackTypeTrajectoryPolyline				= 19;
				//static const int sc_nTrackTypeCubicBezierCurve				= 20;			// Безье второго порядка
			}
		}


		inline void SetUnitSize ( int Size )
		{
			m_nUnitSize			=	Size;
		}

		inline void SetVirtualMode ( bool Value )
		{
			if ( Value )
			{
				m_bStickVirtualPixels	=	true;	
				m_bStickRealPixels		=	false;		
			}
			else
			{
				m_bStickVirtualPixels	=	false;	
				m_bStickRealPixels		=	false;		
			}
		}

		inline void SetRealMode ( bool Value )
		{
			if ( Value )
			{
				m_bStickVirtualPixels	=	false;	
				m_bStickRealPixels		=	true;		
			}
			else
			{
				m_bStickVirtualPixels	=	false;	
				m_bStickRealPixels		=	false;		
			}
		}


		inline bool IsEnableSticking ()
		{
			if ( ( false == m_bStickVirtualPixels && false == m_bStickRealPixels ) || ( 0 == m_nUnitSize ) )
				return false;

			return true;
		}

		inline void SetAlign ( double X, double Y )
		{
			m_nAlingX		=	X;
			m_nAlingY		=	Y;
		}

		inline void SetRoundSticking ( bool Left, bool Top, bool Right, bool Bottom )
		{
			m_bRoundStickingLeft		=	Left;
			m_bRoundStickingTop			=	Top;
			m_bRoundStickingRight		=	Right;
			m_bRoundStickingBottom		=	Bottom;
		}

		inline void SetImageSizes ( long Width, long Height )
		{
			m_nImageWidth	=	Width;
			m_nImageHeight	=	Height;
		}

	public:

		inline void CorrectPoint	( CTrackPoint& oPoint )
		{
			oPoint.dX	= static_cast <double>	( ( (long)	( ( oPoint.dX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
			oPoint.dY	= static_cast <double>	( ( (long)	( ( oPoint.dY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
			oPoint.nX	= static_cast <int>		( ( (int)	( ( oPoint.nX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
			oPoint.nY	= static_cast <int>		( ( (int)	( ( oPoint.nY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
		}
		inline void CorrectPointX	( CTrackPoint& oPoint )
		{
			oPoint.dX	= static_cast <double>	( ( (long)	( ( oPoint.dX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
			oPoint.nX	= static_cast <int>		( ( (int)	( ( oPoint.nX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
		}
		inline void CorrectPointY	( CTrackPoint& oPoint )
		{
			oPoint.dY	= static_cast <double>	( ( (long)	( ( oPoint.dY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
			oPoint.nY	= static_cast <int>		( ( (int)	( ( oPoint.nY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
		}
		inline void CorrectPointP	( CTrackPoint* pPoint )
		{
			pPoint->dX	= static_cast <double>	( ( (long)	( ( pPoint->dX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
			pPoint->dY	= static_cast <double>	( ( (long)	( ( pPoint->dY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
			pPoint->nX	= static_cast <int>		( ( (int)	( ( pPoint->nX -	m_nAlingX+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingX );
			pPoint->nY	= static_cast <int>		( ( (int)	( ( pPoint->nY -	m_nAlingY+ m_nUnitSize * 65535 ) / m_nUnitSize )	- 65535 ) * m_nUnitSize	+	m_nAlingY );
		}

		inline void SetPointMiddle	( CTrackPoints* pPoints, long Index, long Src1, long Src2 )
		{
			pPoints->GetAt ( Index ).dX		=	( pPoints->GetAt ( Src1 ).dX + pPoints->GetAt ( Src2 ).dX ) * 0.5;
			pPoints->GetAt ( Index ).dY		=	( pPoints->GetAt ( Src1 ).dY + pPoints->GetAt ( Src2 ).dY ) * 0.5;
		}


		
	private:
		
		// залипание к сторонам
		inline void DoStickingOnSides	( const int& TrackType, CTrackPoints* pPoints, const int& Operation )
		{
			if ( Tracking::Constants::sc_nTrackTypeRectangle == TrackType )
			{
				if ( m_bRoundStickingLeft )
				{
					double dX	=	pPoints->GetAt ( 0 ).dX -  pPoints->GetAt ( 1 ).dX;
					int nX		=	pPoints->GetAt ( 0 ).nX -  pPoints->GetAt ( 1 ).nX;

					CTrackPoint PointX = pPoints->GetAt ( 0 ); 

					CorrectPointX ( pPoints->GetAt ( 0 ) );
					CorrectPointX ( pPoints->GetAt ( 3 ) );

					if ( PointX.dX != pPoints->GetAt ( 0 ).dX )
					{
						if ( CTrackSticking::nOperationMoveTrack == Operation )
						{
							pPoints->GetAt ( 1 ).dX =  pPoints->GetAt ( 0 ).dX - dX;
							pPoints->GetAt ( 1 ).nX =  pPoints->GetAt ( 0 ).nX - nX;
							pPoints->GetAt ( 2 ).dX =  pPoints->GetAt ( 0 ).dX - dX;
							pPoints->GetAt ( 2 ).nX =  pPoints->GetAt ( 0 ).nX - nX;
						}
					}
				}

				if ( m_bRoundStickingTop )
				{
					double dY	=	pPoints->GetAt ( 0 ).dY -  pPoints->GetAt ( 3 ).dY;
					int nY		=	pPoints->GetAt ( 0 ).nY -  pPoints->GetAt ( 3 ).nY;

					CTrackPoint PointY = pPoints->GetAt ( 0 ); 

					CorrectPointY ( pPoints->GetAt ( 0 ) );
					CorrectPointY ( pPoints->GetAt ( 1 ) );

					if ( PointY.dY != pPoints->GetAt ( 0 ).dY )
					{
						if ( CTrackSticking::nOperationMoveTrack == Operation )
						{
							pPoints->GetAt ( 2 ).dY =  pPoints->GetAt ( 0 ).dY - dY;
							pPoints->GetAt ( 2 ).nY =  pPoints->GetAt ( 0 ).nY - nY;
							pPoints->GetAt ( 3 ).dY =  pPoints->GetAt ( 0 ).dY - dY;
							pPoints->GetAt ( 3 ).nY =  pPoints->GetAt ( 0 ).nY - nY;
						}
					}
				}

				if ( m_bRoundStickingRight )
				{
					double dX	=	pPoints->GetAt ( 0 ).dX -  pPoints->GetAt ( 1 ).dX;
					int nX		=	pPoints->GetAt ( 0 ).nX -  pPoints->GetAt ( 1 ).nX;

					CTrackPoint PointX = pPoints->GetAt ( 0 ); 

					CorrectPointX ( pPoints->GetAt ( 1 ) );
					CorrectPointX ( pPoints->GetAt ( 2 ) );

					if ( PointX.dX != pPoints->GetAt ( 1 ).dX )
					{
						if ( CTrackSticking::nOperationMoveTrack == Operation )
						{
							pPoints->GetAt ( 0 ).dX =  pPoints->GetAt ( 1 ).dX + dX;
							pPoints->GetAt ( 0 ).nX =  pPoints->GetAt ( 1 ).nX + nX;
							pPoints->GetAt ( 3 ).dX =  pPoints->GetAt ( 1 ).dX + dX;
							pPoints->GetAt ( 3 ).nX =  pPoints->GetAt ( 1 ).nX + nX;
						}
					}
				}

				if ( m_bRoundStickingBottom	)
				{
					double dY	=	pPoints->GetAt ( 0 ).dY -  pPoints->GetAt ( 3 ).dY;
					int nY		=	pPoints->GetAt ( 0 ).nY -  pPoints->GetAt ( 3 ).nY;

					CTrackPoint PointY = pPoints->GetAt ( 0 ); 

					CorrectPointY ( pPoints->GetAt ( 3 ) );
					CorrectPointY ( pPoints->GetAt ( 2 ) );

					if ( PointY.dY != pPoints->GetAt ( 3 ).dY )
					{
						if ( CTrackSticking::nOperationMoveTrack == Operation )
						{
							pPoints->GetAt ( 0 ).dY =  pPoints->GetAt ( 3 ).dY + dY;
							pPoints->GetAt ( 0 ).nY =  pPoints->GetAt ( 3 ).nY + nY;
							pPoints->GetAt ( 1 ).dY =  pPoints->GetAt ( 3 ).dY + dY;
							pPoints->GetAt ( 1 ).nY =  pPoints->GetAt ( 3 ).nY + nY;
						}
					}
				}

				SetPointMiddle ( pPoints, 4, 0, 1 );
				SetPointMiddle ( pPoints, 5, 1, 2 );
				SetPointMiddle ( pPoints, 6, 2, 3 );
				SetPointMiddle ( pPoints, 7, 0, 3 );

				SetPointMiddle ( pPoints, 8, 0, 2 );
			}
		}


	private:

		bool	m_bStickVirtualPixels;		// прилипание к виртуальным размерам ( влияет зум )
		bool	m_bStickRealPixels;			// прилипание к пикселям экрана не зависит от зума

		int		m_nUnitSize;				// размер ячейки 
		double	m_nZoom;					// увеличения для MODE_REAL_PIXELS

		double	m_nAlingX;
		double	m_nAlingY;

		bool	m_bRoundStickingLeft; 
		bool	m_bRoundStickingTop;
		bool	m_bRoundStickingRight;
		bool	m_bRoundStickingBottom;

		long	m_nImageWidth;
		long	m_nImageHeight;
	};

}

// implementation
namespace Tracking
{
	//------------------------------------------------------------------------------------------------------------------
	// Track
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObject : public CTrackPoints, public IMouseButtonHandlers, public IExternalBounding
	{
	public:

		CTrackObject()
		{
			m_nType				= Constants::sc_nTrackTypeNone;
			m_pMessageReceiver	= NULL;
			m_bChildTrack		= false;

			m_nTrackID			= 0;
		}

		inline double GetPointAngle ( const int& IndPoint, const int& IndCenter )
		{
			return Geometry::GetAngle ( GetAt(IndCenter).dX, GetAt(IndCenter).dY, GetAt(IndPoint).dX, GetAt(IndPoint).dY );
		}
		inline double GetPointRadius ( const int& IndPoint, const int& IndCenter )
		{
			return Geometry::GetLength(GetAt(IndCenter).dX, GetAt(IndCenter).dY, GetAt(IndPoint).dX, GetAt(IndPoint).dY);
		}


		void SendMessage ( int nType, int nValue )
		{
			Messages::CDirectMessage::Send ( m_pMessageReceiver, nType, nValue, m_nTrackID );
		}

		virtual void OnPaintLines(CTrackPainter& oPainter)
		{
			if (IsTrackRectangleFixedSize())		oPainter.DrawRectangle(this);
			else if (IsTrackRectangleFixedAspect())	oPainter.DrawRectangle(this);
			else if (IsTrackEllipse())				oPainter.DrawEllipse(this);
			else if (IsTrackRectangle())			oPainter.DrawRectangle(this);
			else if (IsTrackLine())					oPainter.DrawLine(this);
			else if (IsTrackPolyline())				oPainter.DrawPolyline(this);
			else if (IsTrackPolygon())				oPainter.DrawPolygon(this);
			else if (IsTrackPie())					oPainter.DrawPie(this);
			else if (IsTrackPieSimple())			oPainter.DrawPieSimple(this);
			else if (IsTrackSector())				oPainter.DrawSector(this);
			else if (IsTrackSectorSimple())			oPainter.DrawSectorSimple(this);
			else if (IsTrackEllipseInRectangle())	oPainter.DrawEllipseInRectangle(this);
			else if (IsTrackCircle())				oPainter.DrawCircle(this);
			else if (IsTrackRectangleRotatable())	oPainter.DrawRectangleRotatable(this);
			//else if (IsTrackRectangleRotatableFixedAspect())	oPainter.DrawRectangleRotatable(this);
			//else if (IsTrackTextRotatable())		oPainter.DrawTextRotatable(this);
		}
		virtual void OnPaintPoints(CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount(), c_nTrackPointSizePaint);
		}


		virtual int GetPointUnderCursor(int nX, int nY)
		{
			return -1;
		}		

		inline int GetType ()
		{
			return m_nType;
		}
		inline void SetType(int nType)
		{
			m_nType = nType;
		}

		virtual void SetReceiver(Messages::CMessageReceiver* pReceiver)
		{
			m_pMessageReceiver = pReceiver;
		}

		BOOL IsTrackNone()
		{
			return (GetType() == Constants::sc_nTrackTypeNone);
		}
		BOOL IsTrackPoint()
		{
			return (GetType() == Constants::sc_nTrackTypePoint);
		}
		BOOL IsTrackLine()
		{
			return (GetType() == Constants::sc_nTrackTypeLine);
		}
		BOOL IsTrackEllipse()
		{
			return (GetType() == Constants::sc_nTrackTypeEllipse);
		}
		BOOL IsTrackRectangle()
		{
			return (GetType() == Constants::sc_nTrackTypeRectangle);
		}
		BOOL IsTrackRectangleFixedSize()
		{
			return (GetType() == Constants::sc_nTrackTypeRectangleFixedSize);
		}
		BOOL IsTrackRectangleFixedAspect()
		{
			return (GetType() == Constants::sc_nTrackTypeRectangleFixedAspect);
		}
		BOOL IsTrackPolyline()
		{
			return (GetType() == Constants::sc_nTrackTypePolyline);
		}
		BOOL IsTrackPolygon()
		{
			return (GetType() == Constants::sc_nTrackTypePolygon);
		}
		BOOL IsTrackPie()
		{
			return (GetType() == Constants::sc_nTrackTypePie);
		}
		BOOL IsTrackPieSimple()
		{
			return (GetType() == Constants::sc_nTrackTypePieSimple);
		}
		BOOL IsTrackSector()
		{
			return (GetType() == Constants::sc_nTrackTypeSector);
		}
		BOOL IsTrackSectorSimple()
		{
			return (GetType() == Constants::sc_nTrackTypeSectorSimple);
		}
		BOOL IsTrackCircle()
		{
			return (GetType() == Constants::sc_nTrackTypeCircleInRectangle);
		}
		BOOL IsTrackEllipseInRectangle()
		{
			return (GetType() == Constants::sc_nTrackTypeEllipseInRectangle);
		}
		BOOL IsTrackRectangleRotatable()
		{
			return (GetType() == Constants::sc_nTrackTypeRectangleRotatable);
		}
		BOOL IsTrackRectangleRotatableFixedAspect()
		{
			return (GetType() == Constants::sc_nTrackTypeRectangleRotatableFixedAspect);
		}
		BOOL IsTrackTextRotatable()
		{
			return (GetType() == Constants::sc_nTrackTypeTextRotatable);
		}
		BOOL IsTrackText()
		{
			return (GetType() == Constants::sc_nTrackTypeText);
		}
		BOOL IsTrackTrajectoryPolyline()
		{
			return (GetType() == Constants::sc_nTrackTypeTrajectoryPolyline);
		}

		BOOL IsTrackTypeCubicBezierCurve () 
		{
			return ( GetType() == Constants::sc_nTrackTypeCubicBezierCurve );
		}

		BOOL IsTracksTypeTrackEffectKenBurns ()
		{
			return ( GetType() == Constants::sc_nTrackTypeTrackEffectKenBurns );
		}
		void MoveTrack(double dX, double dY)
		{
			OffsetByDouble(dX, dY);
		}


		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			return FALSE;
		}
		virtual BOOL OnPaint(CTrackPainter& oPainter, Structures::RECTI rectWindow, Structures::RECTD rectData)
		{
			m_rectWindow = rectWindow;
			m_rectData = rectData;

			// check for valid tracking status
			if (GetCount() < 1 || IsTrackNone())
				return FALSE;

			//Rendering::Gdi::CGdiPainter oGdiPainter;
			//oGdiPainter.Create(oPainter.GetDC());

			// update tracking point coordinates
			MapByDouble(rectData.left, rectData.top, rectData.right, rectData.bottom, rectWindow.left, rectWindow.top, rectWindow.right, rectWindow.bottom);

			// draw tracking lines
			//oGdiPainter.SetPen(oPainter.GetColorLines1(), oPainter.GetColorLines2(), 1, PS_DOT);
			oPainter.SetPenLines ();
			OnPaintLines(oPainter);

			// draw tracking points
			//oGdiPainter.SetPen(oPainter.GetColorPoints1(), oPainter.GetColorPoints2(), 1, PS_SOLID);
			oPainter.SetPenPoints ();
			OnPaintPoints(oPainter);

			//oGdiPainter.Destroy();

			return TRUE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus )
		{
			// считает для текущей точки (с заданными координатами) ее статус
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			return FALSE;
		}


		inline void SetRectWindow ( const Structures::RECTI& rectWindow )
		{
			m_rectWindow = rectWindow;
		}

		inline void SetRectData ( const Structures::RECTD& rectData )
		{
			m_rectData = rectData;
		}


		inline void		SetChild ( bool Value )
		{
			m_bChildTrack = Value;
		}

		inline bool		IsTrackChild ()
		{
			return m_bChildTrack;
		}




		// получить ID трека
		inline void		SetTrackID ( int TrackID )
		{
			m_nTrackID = TrackID;
		}

		inline int		GetTrackID ()
		{
			return m_nTrackID;
		}


		inline CTrackSticking& GetTrackSticking ()
		{
			return m_oSticking;
		}


		inline void SetImageSizes ( long Width, long Height )
		{
			m_nImageWidth	=	Width;
			m_nImageHeight	=	Height;
		}

	protected:

		inline int MapToWindowX(double dX)
		{
			return (int)(m_rectWindow.left + (dX - m_rectData.left)*m_rectWindow.GetWidth()/m_rectData.GetWidth());
		}

		inline int MapToWindowY(double dY)
		{
			return (int)(m_rectWindow.top + (dY - m_rectData.top)*m_rectWindow.GetHeight()/m_rectData.GetHeight());
		}

		inline double MapToDataX(int nX)
		{
			return m_rectData.left + (nX - m_rectWindow.left)*m_rectData.GetWidth()/m_rectWindow.GetWidth();
		}

		inline double MapToDataY(int nY)
		{
			return m_rectData.top + (nY - m_rectWindow.top)*m_rectData.GetHeight()/m_rectWindow.GetHeight();
		}


	protected:

		// тип объекта (точка, линия или еще что...)
		int							m_nType;

		// родитель - ему будут отсылаться сообщения о статусе tracking'а
		Messages::CMessageReceiver* m_pMessageReceiver;

		// свойства последней отрисовки - нужны для пересчета координат точек при tracking'е
		Structures::RECTI			m_rectWindow;
		Structures::RECTD			m_rectData;

		// Трек имеет зависимость от другого трека
		bool						m_bChildTrack;

		// ID трека
		int							m_nTrackID;

		CTrackSticking				m_oSticking;

		long						m_nImageWidth;
		long						m_nImageHeight;
	};

	//------------------------------------------------------------------------------------------------------------------
	//	Points
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectPoints : public CTrackObject
	{
	public:

		inline void Create(double dX, double dY)
		{
			SetType(Constants::sc_nTrackTypePoint);

			Clear();
			Add(dX, dY);

			SetCurrentPointIndex(-1);
		}

		virtual int GetPointUnderCursor ( int MouseX, int MouseY )
		{
			return FindByInteger ( MouseX, MouseY, c_nTrackPointSizeTouch, TRUE );
		}	

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, TRUE);

		//	if (nTrackIndex < 0)
		//	{
		//		Add(MapToDataX(nX), MapToDataY(nY));

		//		nTrackIndex = GetCount() - 1;
		//	}

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnRButtonDown(int nX, int nY)
		//{
		//	if (GetCount() < 2)
		//		return;

		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, TRUE);

		//	if (nTrackIndex >= 0)
		//	{
		//		Remove(nTrackIndex);

		//		SetCurrentPointIndex(-1);

		//		SendMessage(Constants::sc_nMessageTrackPointRemove, nTrackIndex);
		//	}
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	if (GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		GetCurrentPoint().Create(dX, dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_RBUTTONDOWN)
				OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			if (GetCurrentPointIndex() >= 0 || FindByInteger(nX, nY) >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
					nPointStatus = c_nTrackPointStatusSizeAll;
				else
					nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusCanAdd;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount(), c_nTrackPointSizePaint);
		}


	public:
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if (GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				GetCurrentPoint().Create(dX, dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex = FindByInteger(MouseX, MouseY, c_nTrackPointSizeTouch, TRUE);

			if (nTrackIndex < 0)
			{
				Add(MapToDataX(MouseX), MapToDataY(MouseY));

				nTrackIndex = GetCount() - 1;
			}

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}


		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			if (GetCount() < 2)
				return false;

			int nTrackIndex = FindByInteger(MouseX, MouseY, c_nTrackPointSizeTouch, TRUE);

			if (nTrackIndex >= 0)
			{
				Remove(nTrackIndex);

				SetCurrentPointIndex(-1);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointRemove, nTrackIndex);
			}

			return true;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	};

	//------------------------------------------------------------------------------------------------------------------
	//	Line
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectLine : public CTrackObject
	{
	public:

		inline void Create(double dX1, double dY1, double dX2, double dY2)
		{
			SetType(Constants::sc_nTrackTypeLine);

			Clear();
			Add(dX1, dY1);
			Add(dX2, dY2);
			Add(0.5*(dX1 + dX2), 0.5*(dY1 + dY2));

			SetCurrentPointIndex(-1);
		}

		virtual int GetPointUnderCursor ( int MouseX, int MouseY )
		{
			return FindByInteger ( MouseX, MouseY, c_nTrackPointSizeTouch, TRUE );
		}	
	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	if (GetAsyncKeyState(VK_CONTROL) < 0)
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		GetCurrentPoint().Create(dX, dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			if (GetCurrentPointIndex() >= 0 || FindByInteger(nX, nY) >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawLine(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, 2, c_nTrackPointSizePaint);
		}

	public:

		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				GetCurrentPoint().Create(dX, dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}
			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex = FindByInteger(MouseX, MouseY, c_nTrackPointSizeTouch, FALSE);
			if ( -1 == nTrackIndex )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	};

	//------------------------------------------------------------------------------------------------------------------
	//	Ellipse
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectEllipse : public CTrackObject
	{
		//------------------------------------------------------------------------------------------------------------------
		//   /--0--\
		//  /       \
		// 3    4    1
		//  \       /
		//   \--2--/
		//------------------------------------------------------------------------------------------------------------------
	public:
		void Create(double dLeft, double dTop, double dRight, double dBottom)
		{
			double dCenterX = 0.5*(dLeft + dRight);
			double dCenterY = 0.5*(dTop + dBottom);

			SetType(Constants::sc_nTrackTypeEllipse);

			Clear();

			Add(dCenterX, dTop);
			Add(dRight, dCenterY);
			Add(dCenterX, dBottom);
			Add(dLeft, dCenterY);
			Add(dCenterX, dCenterY);

			SetCurrentPointIndex(-1);
		}



	protected:
		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect(0, 3);
				Structures::POINTI pointTrack(nX, nY);

				//				if (rectBounds.IsPointInside(pointTrack))
				if (IsPointInEllipse(rectBounds, pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

		BOOL IsPointInEllipse(Structures::RECTI& rect, Structures::POINTI& point)
		{
			// get center
			double dCenterX = 0.5*(rect.left + rect.right);
			double dCenterY = 0.5*(rect.top + rect.bottom);

			// get angle
			double dAngle = Geometry::GetAngle(dCenterX, dCenterY, point.x, point.y);

			//change point on ellipse
			double dX, dY;
			double dRadiusX = fabs(dCenterX - rect.left);
			double dRadiusY = fabs(dCenterY - rect.top);
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle);

			double dist1 = Geometry::GetLength(dCenterX, dCenterY, dCenterX + dX, dCenterY + dY);
			double dist2 = Geometry::GetLength(dCenterX, dCenterY, point.x, point.y);
			if (dist2 <= dist1)
				return TRUE;

			return FALSE;
		}


		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL) < 0)
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		if (nCurrentPoint == 0)
		//		{
		//			GetAt(0).dY = dY; 
		//			GetAt(1).dY = 0.5*(GetAt(0).dY + GetAt(2).dY);
		//			GetAt(3).dY = GetAt(1).dY;
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			GetAt(1).dX = dX; 
		//			GetAt(0).dX = 0.5*(GetAt(1).dX + GetAt(3).dX);
		//			GetAt(2).dX = GetAt(0).dX;
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			GetAt(2).dY = dY; 
		//			GetAt(1).dY = 0.5*(GetAt(0).dY + GetAt(2).dY);
		//			GetAt(3).dY = GetAt(1).dY;
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			GetAt(3).dX = dX; 
		//			GetAt(0).dX = 0.5*(GetAt(1).dX + GetAt(3).dX);
		//			GetAt(2).dX = GetAt(0).dX;
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}
	public:
		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ((nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if (nTrackIndex == 0 || nTrackIndex == 2)
			{
				nPointStatus = c_nTrackPointStatusVertical;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else if (nTrackIndex == 1 || nTrackIndex == 3)
			{
				nPointStatus = c_nTrackPointStatusHorizontal;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawEllipse(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, 4, c_nTrackPointSizePaint);
		}

	public:
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL) < 0)
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				m_oSticking.DoWork ( Tracking::Constants::sc_nTrackTypeEllipse, this );

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dY = dY; 
					GetAt(1).dY = 0.5*(GetAt(0).dY + GetAt(2).dY);
					GetAt(3).dY = GetAt(1).dY;
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX; 
					GetAt(0).dX = 0.5*(GetAt(1).dX + GetAt(3).dX);
					GetAt(2).dX = GetAt(0).dX;
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dY = dY; 
					GetAt(1).dY = 0.5*(GetAt(0).dY + GetAt(2).dY);
					GetAt(3).dY = GetAt(1).dY;
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX; 
					GetAt(0).dX = 0.5*(GetAt(1).dX + GetAt(3).dX);
					GetAt(2).dX = GetAt(0).dX;
				}

				m_oSticking.DoWork ( Tracking::Constants::sc_nTrackTypeEllipse, this );

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex = GetPointUnderCursor ( MouseX, MouseY );
			if ( -1 == nTrackIndex )
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect(0, 3);
				Structures::POINTI pointTrack ( MouseX, MouseY );

				if ( IsPointInEllipse ( rectBounds, pointTrack ) )
					return true;

				return false;
			}

			if (nTrackIndex == GetCount() - 1)
				GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}


		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	};

	//------------------------------------------------------------------------------------------------------------------
	//	Rectangle
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectRectangle : public CTrackObject
	{
		//------------------------------------------------------------------------------------------------------------------
		//   0---4---1
		//   |       |
		//   7   8   5
		//   |       |
		//   3---6---2
		//------------------------------------------------------------------------------------------------------------------
	public:
		void Create(double dLeft, double dTop, double dRight, double dBottom)
		{
			//_dLeft, _dTop, _dRight, _dBottom;
			SetType(Constants::sc_nTrackTypeRectangle);

			Clear();
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);

			Add(0.5*(dLeft + dRight), dTop);
			Add(dRight, 0.5*(dTop + dBottom));
			Add(0.5*(dLeft + dRight), dBottom);
			Add(dLeft, 0.5*(dTop + dBottom));

			Add(0.5*(dLeft + dRight), 0.5*(dTop + dBottom));

			SetCurrentPointIndex(-1);

			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;
		}


	protected:
		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		if (nCurrentPoint == 0)
		//		{
		//			GetAt(0).dX = dX;
		//			GetAt(0).dY = dY;
		//			GetAt(1).dY = dY;
		//			GetAt(3).dX = dX;
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			GetAt(1).dX = dX;
		//			GetAt(1).dY = dY;
		//			GetAt(0).dY = dY;
		//			GetAt(2).dX = dX;
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			GetAt(2).dX = dX;
		//			GetAt(2).dY = dY;
		//			GetAt(1).dX = dX;
		//			GetAt(3).dY = dY;
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			GetAt(3).dX = dX;
		//			GetAt(3).dY = dY;
		//			GetAt(2).dY = dY;
		//			GetAt(0).dX = dX;
		//		}
		//		else if (nCurrentPoint == 4)
		//		{
		//			GetAt(4).dY = dY;
		//			GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
		//			GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
		//		}
		//		else if (nCurrentPoint == 5)
		//		{
		//			GetAt(5).dX = dX;
		//			GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
		//			GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
		//		}
		//		else if (nCurrentPoint == 6)
		//		{
		//			GetAt(6).dY = dY;
		//			GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
		//			GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
		//		}
		//		else if (nCurrentPoint == 7)
		//		{
		//			GetAt(7).dX = dX;
		//			GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
		//			GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
		//		}

		//		// recompute centers
		//		if (nCurrentPoint >= 0 && nCurrentPoint < 4)
		//		{
		//			GetAt(4).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(0).dY);
		//			GetAt(5).Create(GetAt(2).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
		//			GetAt(6).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(2).dY);
		//			GetAt(7).Create(GetAt(0).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
		//		}

		//		// recompute corners
		//		if (nCurrentPoint >= 4 && nCurrentPoint < 8)
		//		{
		//			GetAt(0).Create(GetAt(7).dX, GetAt(4).dY);
		//			GetAt(1).Create(GetAt(5).dX, GetAt(4).dY);
		//			GetAt(2).Create(GetAt(5).dX, GetAt(6).dY);
		//			GetAt(3).Create(GetAt(7).dX, GetAt(6).dY);
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}
	public:
		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ((nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else
			{
				nChangeStatus = c_nTrackChangeStatusSize;

				if (nTrackIndex == 0 || nTrackIndex == 2)
					nPointStatus = c_nTrackPointStatusPoint;
				else if (nTrackIndex == 1 || nTrackIndex == 3)
					nPointStatus = c_nTrackPointStatusPoint;
				else if (nTrackIndex == 4 || nTrackIndex == 6)
					nPointStatus = c_nTrackPointStatusPoint;
				else if (nTrackIndex == 5 || nTrackIndex == 7)
					nPointStatus = c_nTrackPointStatusPoint;
				else
				{
					nPointStatus = c_nTrackPointStatusNone;
					nChangeStatus = c_nTrackChangeStatusNone;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangle(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}

	public:
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX ( MouseX );
			double dY = MapToDataY ( MouseY );

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				// делаем по нулевой точки
				if ( m_oSticking.IsEnableSticking () )
				{
					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );
					m_oSticking.DoWork ( m_nType, this, CTrackSticking::nOperationMoveTrack );
				}
				else
				{
					OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
#ifdef _DEBUG
					//ATLTRACE ( _T("FX :%f, FY : %f\n"), dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
#endif
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;
				}
				else if (nCurrentPoint == 4)
				{
					GetAt(4).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 5)
				{
					GetAt(5).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}
				else if (nCurrentPoint == 6)
				{
					GetAt(6).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 7)
				{
					GetAt(7).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 4)
				{
					GetAt(4).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(0).dY);
					GetAt(5).Create(GetAt(2).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
					GetAt(6).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(2).dY);
					GetAt(7).Create(GetAt(0).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
				}

				// recompute corners
				if (nCurrentPoint >= 4 && nCurrentPoint < 8)
				{
					GetAt(0).Create(GetAt(7).dX, GetAt(4).dY);
					GetAt(1).Create(GetAt(5).dX, GetAt(4).dY);
					GetAt(2).Create(GetAt(5).dX, GetAt(6).dY);
					GetAt(3).Create(GetAt(7).dX, GetAt(6).dY);
				}

				m_oSticking.DoWork ( m_nType, this );

				GetAt(8).Create ( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), 0.5 * ( GetAt(0).dY + GetAt(2).dY ) );

				SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}


			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex		=	GetPointUnderCursor ( MouseX, MouseY );

			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			// если не захватили точку, то проверяем попадаение в область трека
			if ( -1 == nTrackIndex )
			{
				Structures::RECTI rectBounds	= GetIntegerBoundingRect();
				Structures::POINTI pointTrack ( MouseX, MouseY );

				if ( rectBounds.IsPointInside ( pointTrack ) )
					return true;

				return false;
			}

			if (nTrackIndex == GetCount() - 1)
				GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}


		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	private:

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;
	};

	//------------------------------------------------------------------------------------------------------------------
	//	Rectangle
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectRectangleRotatable : public CTrackObject
	{
		//------------------------------------------------------------------------------------------------------------------
		//       8	
		//       |
		//   0---4---1
		//   |       |
		//   7   9   5
		//   |       |
		//   3---6---2
		//------------------------------------------------------------------------------------------------------------------
	public:
		void Create(double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0.0, double dStickAngle = 0.0 )
		{
			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 )	m_IsFirstResize = true;
			else															m_IsFirstResize = false;

			m_dStickAngle = dStickAngle;
			SetType( Constants::sc_nTrackTypeRectangleRotatable );

			Clear();
			Add( dLeft, dTop );
			Add( dRight, dTop );
			Add( dRight, dBottom );
			Add( dLeft, dBottom );

			Add( 0.5 * ( dLeft + dRight ), dTop );
			Add( dRight, 0.5 * ( dTop + dBottom ) );
			Add( 0.5 * ( dLeft + dRight ), dBottom );
			Add( dLeft, 0.5 * ( dTop + dBottom ) );

			Add( 0.5 * ( dLeft + dRight ), dTop - 30 );					//rotate selector
			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );	//center

			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			SetCurrentPointIndex(-1);

			RotateToAlpha( rad( dAngle ) );
		}

		inline double GetStickAngle () { return m_dStickAngle; }
	protected:

		int GetPointUnderCursor( int nX, int nY )
		{
			int nTrackIndex = FindByInteger( nX, nY, c_nTrackPointSizeTouch, FALSE );

			if( nTrackIndex < 0 )
			{
				Structures::RECTI rectD;

				double dX = MapToDataX( nX );
				double dY = MapToDataY( nY );

				double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;

				double tX, tY;
				double cosA = cos( -dAngle );
				double sinA = sin( -dAngle );

				tX = dX - GetAt(9).dX;
				tY = dY - GetAt(9).dY;				
				dX = GetAt(9).dX + tX * cosA - tY * sinA;
				dY = GetAt(9).dY + tX * sinA + tY * cosA;

				Structures::POINTI pointTrack ( (int)dX, (int)dY );

				rectD.left   = (int)GetAt(0).dX;
				rectD.top    = (int)GetAt(0).dY;
				rectD.right  = (int)GetAt(2).dX;
				rectD.bottom = (int)GetAt(2).dY;

				tX = GetAt(0).dX - GetAt(9).dX;
				tY = GetAt(0).dY - GetAt(9).dY;				
				rectD.left  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.top   = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );	

				tX = GetAt(2).dX - GetAt(9).dX;
				tY = GetAt(2).dY - GetAt(9).dY;
				rectD.right  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.bottom = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

				if( rectD.right < rectD.left ) 
				{ 
					double tmpRight		= rectD.right; 
					rectD.right			= rectD.left;
					rectD.left			= (int)tmpRight;
				}
				if( rectD.top > rectD.bottom ) 
				{ 
					double tmpBottom	= rectD.bottom; 
					rectD.bottom		= rectD.top;
					rectD.top			= (int)tmpBottom;
				}

				if( rectD.IsPointInside( pointTrack ) )
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

		void RotateToAlpha( double alpha )
		{
			for( int index = 0; index < 9; ++index )
				Geometry::RotatePoint( GetAt(index).dX, GetAt(index).dY, GetAt(9).dX, GetAt(9).dY, alpha );	
		}
		void StickAngle( double &dAngle, double angleStick )
		{
			int		countStick	 = 0;
			double  tmpBaseAngle = dAngle;
			double  rangeStick	 = rad(2);
			if( 0 == angleStick || rangeStick > angleStick ) angleStick = M_PI_2;
			if( dAngle < 0 ) angleStick = -angleStick;
			while( (dAngle < 0) ? (tmpBaseAngle < angleStick + rangeStick) : (tmpBaseAngle > angleStick - rangeStick) )
			{ 
				tmpBaseAngle -= angleStick;
				countStick++;
			}
			if( abs( tmpBaseAngle ) < rangeStick ) 
			{
				tmpBaseAngle = countStick * angleStick;
				dAngle = tmpBaseAngle;
			}
		}		


		//void OnLButtonDown( int nX, int nY )
		//{
		//	int nTrackIndex = GetPointUnderCursor( nX, nY );

		//	if( nTrackIndex == GetCount() - 1 )
		//		GetAt(nTrackIndex).Create( MapToDataX( nX ), MapToDataY( nY ) );

		//	SetCurrentPointIndex( nTrackIndex );

		//	SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );
		//}
		//void OnLButtonUp( int nX, int nY )
		//{
		//	m_IsFirstResize = false;
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove( int nX, int nY )
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	//int nCurrentPoint = GetCurrentPointIndex();
		//	int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

		//	if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		double dx, dy;
		//		double eX1, eX2, eY1, eY2;

		//		if (nCurrentPoint <= 8)
		//		{	
		//			eX1 = 1.0; eY1 = 0.0;
		//			eX2 = 0.0; eY2 = 1.0;

		//			double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
		//			Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
		//			Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );
		//		}

		//		if (nCurrentPoint == 0)
		//		{
		//			dX -= GetAt(0).dX;
		//			dY -= GetAt(0).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(0).dX += dX;
		//			GetAt(0).dY += dY;
		//			GetAt(1).dX += eX2*dy;
		//			GetAt(1).dY += eY2*dy;
		//			GetAt(3).dX += eX1*dx;
		//			GetAt(3).dY += eY1*dx;
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			dX -= GetAt(1).dX;
		//			dY -= GetAt(1).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(1).dX += dX;
		//			GetAt(1).dY += dY;
		//			GetAt(0).dX += eX2*dy;
		//			GetAt(0).dY += eY2*dy;
		//			GetAt(2).dX += eX1*dx;
		//			GetAt(2).dY += eY1*dx;

		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			dX -= GetAt(2).dX;
		//			dY -= GetAt(2).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(2).dX += dX;
		//			GetAt(2).dY += dY;
		//			GetAt(1).dX += eX1*dx;
		//			GetAt(1).dY += eY1*dx;
		//			GetAt(3).dX += eX2*dy;
		//			GetAt(3).dY += eY2*dy;
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			dX -= GetAt(3).dX;
		//			dY -= GetAt(3).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(3).dX += dX;
		//			GetAt(3).dY += dY;
		//			GetAt(0).dX += eX1*dx;
		//			GetAt(0).dY += eY1*dx;
		//			GetAt(2).dX += eX2*dy;
		//			GetAt(2).dY += eY2*dy;

		//		}
		//		else if (nCurrentPoint == 4)
		//		{
		//			dX -= GetAt(4).dX;
		//			dY -= GetAt(4).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(4).dX += dX;
		//			GetAt(4).dY += dY;
		//			GetAt(0).dX += eX2*dy;
		//			GetAt(0).dY += eY2*dy; 
		//			GetAt(1).dX += eX2*dy;
		//			GetAt(1).dY += eY2*dy;

		//		}
		//		else if (nCurrentPoint == 5)
		//		{
		//			dX -= GetAt(5).dX;
		//			dY -= GetAt(5).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(5).dX += dX;
		//			GetAt(5).dY += dY;
		//			GetAt(1).dX += eX1*dx;
		//			GetAt(1).dY += eY1*dx;
		//			GetAt(2).dX += eX1*dx;
		//			GetAt(2).dY += eY1*dx;					
		//		}
		//		else if (nCurrentPoint == 6)
		//		{
		//			dX -= GetAt(6).dX;
		//			dY -= GetAt(6).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(6).dX += dX;
		//			GetAt(6).dY += dY;
		//			GetAt(2).dX += eX2*dy;
		//			GetAt(2).dY += eY2*dy; 
		//			GetAt(3).dX += eX2*dy;
		//			GetAt(3).dY += eY2*dy;
		//		}
		//		else if (nCurrentPoint == 7)
		//		{
		//			dX -= GetAt(7).dX;
		//			dY -= GetAt(7).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(7).dX += dX;
		//			GetAt(7).dY += dY;
		//			GetAt(0).dX += eX1*dx;
		//			GetAt(0).dY += eY1*dx;
		//			GetAt(3).dX += eX1*dx;
		//			GetAt(3).dY += eY1*dx;	
		//		}				
		//		else if (nCurrentPoint == 8)
		//		{
		//			double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
		//			StickAngle( baseAngle, m_dStickAngle );
		//			double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
		//			RotateToAlpha( mAngle );
		//		}

		//		// recompute centers
		//		if (nCurrentPoint >= 0 && nCurrentPoint < 8)
		//		{
		//			GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
		//			GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
		//			GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
		//			GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

		//			double vX = GetAt(4).dX - GetAt(6).dX;
		//			double vY = GetAt(4).dY - GetAt(6).dY;

		//			double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
		//			if( length < 1 )
		//			{
		//				vX = -eX2; 
		//				vY = -eY2;
		//			}
		//			else
		//			{
		//				vX /= length;
		//				vY /= length;
		//			}

		//			GetAt(8).dX = GetAt(4).dX + vX * 30;
		//			GetAt(8).dY = GetAt(4).dY + vY * 30;
		//			GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ( nTrackIndex == 9 || (nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else if (nTrackIndex == 8)
			{
				nPointStatus = c_nTrackPointStatusRotation;
				nChangeStatus = c_nTrackChangeStatusRotation;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangleRotatable(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 3, c_nTrackPointSizePaint);
		}


	public:
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			//int nCurrentPoint = GetCurrentPointIndex();
			int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				double dx, dy;
				double eX1, eX2, eY1, eY2;

				if (nCurrentPoint <= 8)
				{	
					eX1 = 1.0; eY1 = 0.0;
					eX2 = 0.0; eY2 = 1.0;

					double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
					Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
					Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );
				}

				if (nCurrentPoint == 0)
				{
					dX -= GetAt(0).dX;
					dY -= GetAt(0).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(0).dX += dX;
					GetAt(0).dY += dY;
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;
				}
				else if (nCurrentPoint == 1)
				{
					dX -= GetAt(1).dX;
					dY -= GetAt(1).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(1).dX += dX;
					GetAt(1).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;

				}
				else if (nCurrentPoint == 2)
				{
					dX -= GetAt(2).dX;
					dY -= GetAt(2).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(2).dX += dX;
					GetAt(2).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 3)
				{
					dX -= GetAt(3).dX;
					dY -= GetAt(3).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(3).dX += dX;
					GetAt(3).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy;

				}
				else if (nCurrentPoint == 4)
				{
					dX -= GetAt(4).dX;
					dY -= GetAt(4).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(4).dX += dX;
					GetAt(4).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy; 
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;

				}
				else if (nCurrentPoint == 5)
				{
					dX -= GetAt(5).dX;
					dY -= GetAt(5).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(5).dX += dX;
					GetAt(5).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;					
				}
				else if (nCurrentPoint == 6)
				{
					dX -= GetAt(6).dX;
					dY -= GetAt(6).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(6).dX += dX;
					GetAt(6).dY += dY;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy; 
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 7)
				{
					dX -= GetAt(7).dX;
					dY -= GetAt(7).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(7).dX += dX;
					GetAt(7).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;	
				}				
				else if (nCurrentPoint == 8)
				{
					double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
					StickAngle( baseAngle, m_dStickAngle );
					double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
					RotateToAlpha( mAngle );
				}

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 8)
				{
					GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
					GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
					GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
					GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

					double vX = GetAt(4).dX - GetAt(6).dX;
					double vY = GetAt(4).dY - GetAt(6).dY;

					double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
					if( length < 1 )
					{
						vX = -eX2; 
						vY = -eY2;
					}
					else
					{
						vX /= length;
						vY /= length;
					}

					GetAt(8).dX = GetAt(4).dX + vX * 30;
					GetAt(8).dY = GetAt(4).dY + vY * 30;
					GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )
		{
			int nTrackIndex = GetPointUnderCursor(MouseX, MouseY);

			if ( -1 == nTrackIndex )
				return false;

			if (nTrackIndex == GetCount() - 1)
				GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}


		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )
		{
			m_IsFirstResize = false;

			if (!IsCurrentPointValid())
				return false;

			SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )
		{
			return false;
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )
		{
			return false;
		}

	private:

		bool	m_IsFirstResize;
		double	m_dStickAngle;
	};
	//------------------------------------------------------------------------------------------------------------------
	// Rectangle Rotatable Fixed Aspect
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectRectangleRotatableFixedAspect : public CTrackObject
	{
		//------------------------------------------------------------------------------------------------------------------
		//       4	
		//       |
		//   0-------1
		//   |       |
		//   |   5   |
		//   |       |
		//   3-------2
		//------------------------------------------------------------------------------------------------------------------
	public:
		void Create(double dLeft, double dTop, double dRight, double dBottom, double dAngle = 0.0, double dStickAngle = 0.0 )
		{
			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 )	m_IsFirstResize = true;
			else															m_IsFirstResize = false;

			m_dStickAngle = dStickAngle;
			SetType( Constants::sc_nTrackTypeRectangleRotatableFixedAspect );

			Clear();
			
			Add ( dLeft, dTop );
			Add ( dRight, dTop );
			Add ( dRight, dBottom );
			Add ( dLeft, dBottom );

			Add ( 0.5 * ( dLeft + dRight ), dTop - 30 );					//	rotate selector
			Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );		//	center

			Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			SetCurrentPointIndex(-1);

			double Angle = rad( dAngle );
			for ( int index = 0; index < 5; ++index )
			{
				Geometry::RotatePoint( GetAt(index).dX, GetAt(index).dY, GetAt(5).dX, GetAt(5).dY, Angle );	
			}

			m_bMoveCapturePoint	=	FALSE;
			m_nAspect			=	Geometry::GetLength ( GetAt(0).dX, GetAt(0).dY, GetAt(1).dX, GetAt(1).dY ) /
									Geometry::GetLength ( GetAt(2).dX, GetAt(2).dY, GetAt(1).dX, GetAt(1).dY );

			m_bSetNoResize		=	FALSE;
			
			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;
		}

		inline double GetStickAngle () { return m_dStickAngle; }
		inline void SetNoResize ( BOOL Value )
		{
			m_bSetNoResize	=	Value;
		}

		inline BOOL GetNoResize ( )
		{
			return m_bSetNoResize;
		}
		
		inline void SetSizesMode ( BOOL Value )
		{
			m_bMoveCapturePoint	=	Value;
		}

		inline BOOL GetSizesMode ()
		{
			return m_bMoveCapturePoint;
		}


	protected:

		int GetPointUnderCursor( int nX, int nY )
		{
			int nTrackIndex = FindByInteger ( nX, nY, c_nTrackPointSizeTouch, FALSE );
			
			if ( nTrackIndex < 0 )
			{
				Structures::RECTI rectD;

				double dX = MapToDataX( nX );
				double dY = MapToDataY( nY );

				double dAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY ) + M_PI_2;

				double tX, tY;
				double cosA = cos( -dAngle );
				double sinA = sin( -dAngle );

				tX = dX - GetAt(5).dX;
				tY = dY - GetAt(5).dY;				
				dX = GetAt(5).dX + tX * cosA - tY * sinA;
				dY = GetAt(5).dY + tX * sinA + tY * cosA;

				Structures::POINTI pointTrack ( (int)dX, (int)dY );

				rectD.left   = (int)GetAt(0).dX;
				rectD.top    = (int)GetAt(0).dY;
				rectD.right  = (int)GetAt(2).dX;
				rectD.bottom = (int)GetAt(2).dY;

				tX = GetAt(0).dX - GetAt(5).dX;
				tY = GetAt(0).dY - GetAt(5).dY;				
				rectD.left  = (int) ( GetAt(5).dX + tX * cosA - tY * sinA );
				rectD.top   = (int) ( GetAt(5).dY + tX * sinA + tY * cosA);	

				tX = GetAt(2).dX - GetAt(5).dX;
				tY = GetAt(2).dY - GetAt(5).dY;
				rectD.right  = (int) ( GetAt(5).dX + tX * cosA - tY * sinA );
				rectD.bottom = (int) ( GetAt(5).dY + tX * sinA + tY * cosA );

				if( rectD.right < rectD.left ) 
				{ 
					double tmpRight		= rectD.right; 
					rectD.right			= rectD.left;
					rectD.left			= (int)tmpRight;
				}
				if( rectD.top > rectD.bottom ) 
				{ 
					double tmpBottom	= rectD.bottom; 
					rectD.bottom		= rectD.top;
					rectD.top			= (int)tmpBottom;
				}

				if ( rectD.IsPointInside( pointTrack ) )
				{
					nTrackIndex = GetCount() - 1;
				}
			}

			//if ( m_bSetNoResize )
			{
				//ATLTRACE ( "POINT : %d\n", nTrackIndex );

				//if ( nTrackIndex == 6 ) // nTrackIndex >= 0 && nTrackIndex <= 4 )		// FIXED
				{
					//return 5;
				}
			}

			return nTrackIndex;
		}
		void StickAngle( double &dAngle, double angleStick )
		{
			int		countStick	 = 0;
			double  tmpBaseAngle = dAngle;
			double  rangeStick	 = rad(2);
			if( 0 == angleStick || rangeStick > angleStick ) angleStick = M_PI_2;
			if( dAngle < 0 ) angleStick = -angleStick;
			while( (dAngle < 0) ? (tmpBaseAngle < angleStick + rangeStick) : (tmpBaseAngle > angleStick - rangeStick) )
			{ 
				tmpBaseAngle -= angleStick;
				countStick++;
			}
			if( abs( tmpBaseAngle ) < rangeStick ) 
			{
				tmpBaseAngle = countStick * angleStick;
				dAngle = tmpBaseAngle;
			}
		}

		//void OnLButtonDown( int nX, int nY )
		//{
		//	int nTrackIndex = GetPointUnderCursor( nX, nY );

		//	if( nTrackIndex == GetCount() - 1 )
		//		GetAt(nTrackIndex).Create( MapToDataX( nX ), MapToDataY( nY ) );

		//	SetCurrentPointIndex( nTrackIndex );

		//	SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );
		//}
		//void OnLButtonUp( int nX, int nY )
		//{
		//	m_IsFirstResize = false;
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove( int nX, int nY )
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

		//	if( nCurrentPoint == 5 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL) )
		//	{
		//		OffsetByDouble( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
		//		SendMessage( Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex() );
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		if (nCurrentPoint < 4)
		//		{	
		//			double dAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY ) + M_PI_2;

		//			for( int i = 0; i < 5; ++i )
		//				Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, -dAngle );

		//			double dCenterX = 0.5 * ( GetAt(0).dX + GetAt(2).dX );
		//			double dCenterY = 0.5 * ( GetAt(0).dY + GetAt(2).dY );
		//			double dSizeX	= 0.5 * ( GetAt(2).dX - GetAt(0).dX );
		//			double dSizeY	= 0.5 * ( GetAt(2).dY - GetAt(0).dY );

		//			double dDistanceOld = _hypot( dSizeX, dSizeY );
		//			double dDistanceNew = _hypot( dX - dCenterX, dY - dCenterY );

		//			double dNewSizeX = dSizeX * dDistanceNew / max( 0.001, dDistanceOld );
		//			double dNewSizeY = dSizeY * dDistanceNew / max( 0.001, dDistanceOld );

		//			GetAt(0).Create( dCenterX - dNewSizeX, dCenterY - dNewSizeY );
		//			GetAt(1).Create( dCenterX + dNewSizeX, dCenterY - dNewSizeY );
		//			GetAt(2).Create( dCenterX + dNewSizeX, dCenterY + dNewSizeY );
		//			GetAt(3).Create( dCenterX - dNewSizeX, dCenterY + dNewSizeY );
		//			GetAt(4).Create( ( GetAt(1).dX + GetAt(0).dX ) / 2.0, GetAt(1).dY - 30.0 );
		//			GetAt(5).Create( dCenterX, dCenterY );

		//			for( int i = 0; i < 5; ++i )
		//				Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, dAngle );
		//		}	
		//		else if (nCurrentPoint == 4)
		//		{
		//			double baseAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, dX, dY );
		//			StickAngle( baseAngle, m_dStickAngle );
		//			double dAngle	 = baseAngle - Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY );
		//			for( int i = 0; i < 5; ++i )
		//				Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, dAngle );
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor ( int nX, int nY, int& nPointStatus, int& nChangeStatus )
		{
			nPointStatus		=	c_nTrackPointStatusNone;
			nChangeStatus		=	c_nTrackChangeStatusNone;

			int nTrackIndex		=	GetCurrentPointIndex();

			if ( nTrackIndex < 0 )
			{
				nTrackIndex		=	GetPointUnderCursor( nX, nY );
			}

			if ( nTrackIndex == 5 || ( nTrackIndex == GetCount() - 1 ) || ( nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0 ) )
			{
				nPointStatus	=	c_nTrackPointStatusSizeAll;
				nChangeStatus	=	c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 4 )
			{
				if ( FALSE == m_bSetNoResize )
				{
					nPointStatus	=	c_nTrackPointStatusPoint;
					nChangeStatus	=	c_nTrackChangeStatusSize;
				}
			}
			else if ( nTrackIndex == 4 )
			{
				nPointStatus	=	c_nTrackPointStatusRotation;
				nChangeStatus	=	c_nTrackChangeStatusRotation;
			}
			else
			{
				nPointStatus	=	c_nTrackPointStatusNone;
				nChangeStatus	=	c_nTrackChangeStatusNone;
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangleRotatable(this, c_nTrackPointSizePaint, 4, 5 );
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if ( FALSE == m_bSetNoResize )
			{
				oPainter.DrawPoints(this, GetCount() - 3, c_nTrackPointSizePaint);

				/*

				Gdiplus::Graphics render ( oPainter.GetDC () );
				Gdiplus::SolidBrush	oSolidBrush ( Gdiplus::Color::Red );
				Gdiplus::StringFormat	oStringFormat;

				for ( int i = 0; i < 6; ++i )
				{
					CStringW Format; Format.Format ( L"%d ", i );
					render.DrawString ( Format, -1, new Gdiplus::Font ( L"Times", 20 ), Gdiplus::RectF ( GetAt(i).nX - 20 / 2, GetAt(i).nY - 20 / 2, 25, 25 ), &oStringFormat, &oSolidBrush );
				}
				
				*/
			}
		}


	public:		
		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			/*
			
			ATLTRACE ("MOUSE : %f, %f, WINDOW : left : %d, top : %d, right : %d, bottom : %d, DATA : left : %f, top : %f, right : %f, bottom : %f \n", 
				dX, dY,
				m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom,
				m_rectData.left, m_rectData.top, m_rectData.right, m_rectData.bottom );
				
			*/

			int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

			if ( nCurrentPoint == 5 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL) )
			{
				// делаем по нулевой точки
				if ( m_oSticking.IsEnableSticking () )
				{
					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );
					
					m_oSticking.DoWork ( m_nType, this, CTrackSticking::nOperationMoveTrack );
				}
				else
				{
					OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());

				//OffsetByDouble( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );

				//if ( false == IsTrackChild () )
				//	SendMessage( Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex() );
			}
			else
			{
				if ( m_bMoveCapturePoint )
				{
					UpadateMovePointMode ( dX, dY, nCurrentPoint );
				}
				else
				{
					// correct rectangle points
					if ( nCurrentPoint < 4 )
					{	
						CTrackPoint Points[6]	=	{ GetAt(0), GetAt(1), GetAt(2), GetAt(3), GetAt(4), GetAt(5) };

						double dAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY ) + M_PI_2;

						for ( int i = 0; i < 5; ++i )
							Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, -dAngle );

						double dCenterX = 0.5 * ( GetAt(0).dX + GetAt(2).dX );
						double dCenterY = 0.5 * ( GetAt(0).dY + GetAt(2).dY );
						double dSizeX	= 0.5 * ( GetAt(2).dX - GetAt(0).dX );
						double dSizeY	= 0.5 * ( GetAt(2).dY - GetAt(0).dY );

						double dDistanceOld = _hypot ( dSizeX, dSizeY );
						double dDistanceNew = _hypot ( dX - dCenterX, dY - dCenterY );

						if ( dDistanceNew < 10.0 )
						{
							GetAt(0).Create ( Points[0] );
							GetAt(1).Create ( Points[1] );
							GetAt(2).Create ( Points[2] );
							GetAt(3).Create ( Points[3] );
							GetAt(4).Create ( Points[4] );
							GetAt(5).Create ( Points[5] );
						}
						else
						{
							double dNewSizeX = dSizeX * dDistanceNew / max ( 0.001, dDistanceOld );
							double dNewSizeY = dSizeY * dDistanceNew / max ( 0.001, dDistanceOld );

							GetAt(0).Create( dCenterX - dNewSizeX, dCenterY - dNewSizeY );
							GetAt(1).Create( dCenterX + dNewSizeX, dCenterY - dNewSizeY );
							GetAt(2).Create( dCenterX + dNewSizeX, dCenterY + dNewSizeY );
							GetAt(3).Create( dCenterX - dNewSizeX, dCenterY + dNewSizeY );
							GetAt(4).Create( ( GetAt(1).dX + GetAt(0).dX ) / 2.0, GetAt(1).dY - 30.0 );
							GetAt(5).Create( dCenterX, dCenterY );

							for( int i = 0; i < 5; ++i )
								Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, dAngle );
						}

						if ( m_bSetNoResize )
						{
							GetAt(0).Create ( Points[0] );
							GetAt(1).Create ( Points[1] );
							GetAt(2).Create ( Points[2] );
							GetAt(3).Create ( Points[3] );
							GetAt(4).Create ( Points[4] );
							GetAt(5).Create ( Points[5] );
						}
						else
						{
							if ( IsUpdateSizes () )
							{
								GetAt(0).Create ( Points[0] );
								GetAt(1).Create ( Points[1] );
								GetAt(2).Create ( Points[2] );
								GetAt(3).Create ( Points[3] );
								GetAt(4).Create ( Points[4] );
								GetAt(5).Create ( Points[5] );
							}
						}
					}	
					else 
					if ( 4 == nCurrentPoint )
					{
						double baseAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, dX, dY );
						
						StickAngle( baseAngle, m_dStickAngle );
						
						double dAngle	 = baseAngle - Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY );
						
						for( int i = 0; i < 5; ++i )
						{
							Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, dAngle );
						}
					}

				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex = GetPointUnderCursor ( MouseX, MouseY );

			if ( -1 == nTrackIndex )
				return false;
			
			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			if ( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			SetCurrentPointIndex( nTrackIndex );

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			m_IsFirstResize = false;

			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	private:
		
		virtual bool IsUpdateSizes ()
		{
			double SizeX = 0.5 * ( GetAt(2).dX - GetAt(0).dX );
			double SizeY = 0.5 * ( GetAt(2).dY - GetAt(0).dY );

			if ( m_bEnableMaxBounding )
			{
				if ( SizeX >= m_nBoundMaxWidth || SizeY >= m_nBoundMaxHeight )
					return true;
			}

			if ( m_bEnableMinBounding )
			{
				if ( SizeX <= m_nBoundMinWidth || SizeY <= m_nBoundMinHeight )
					return true;
			}

			return false;
		}

		inline void UpadateMovePointMode ( double X, double Y, long IndPoint )
		{
			if ( 4 == IndPoint )
			{
				double baseAngle = Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, X, Y );

				StickAngle( baseAngle, m_dStickAngle );

				double dAngle	 = baseAngle - Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY );

				for( int i = 0; i < 5; ++i )
				{
					Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, dAngle );
				}
			}
			else
			{
				if ( m_bSetNoResize )
					return;

				CTrackPoint Points[6]	=	{ GetAt(0), GetAt(1), GetAt(2), GetAt(3), GetAt(4), GetAt(5) };
				double dAngle			=	Geometry::GetAngle( GetAt(5).dX, GetAt(5).dY, GetAt(4).dX, GetAt(4).dY ) + M_PI_2;

				for ( int i = 0; i < 5; ++i )
				{
					Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, GetAt(5).dX, GetAt(5).dY, -dAngle );
				}

				double dCenterX		=	0.5 * ( GetAt(0).dX + GetAt(2).dX );
				double dCenterY		=	0.5 * ( GetAt(0).dY + GetAt(2).dY );
				double dSizeX		=	0.5 * ( GetAt(2).dX - GetAt(0).dX );
				double dSizeY		=	0.5 * ( GetAt(2).dY - GetAt(0).dY );

				double dDistanceOld	=	_hypot ( dSizeX, dSizeY );
				double dDistanceNew	=	_hypot ( X - dCenterX, Y - dCenterY );

				if ( dDistanceNew < 10.0 )
				{
					GetAt(0).Create ( Points[0] );
					GetAt(1).Create ( Points[1] );
					GetAt(2).Create ( Points[2] );
					GetAt(3).Create ( Points[3] );
					GetAt(4).Create ( Points[4] );
					GetAt(5).Create ( Points[5] );
				}
				else
				{	
					double RotateX = GetAt(5).dX;
					double RotateY = GetAt(5).dY;
				
					double NMouseX = X;
					double NMouseY = Y;

					Geometry::RotatePoint ( NMouseX, NMouseY, GetAt(5).dX, GetAt(5).dY, -dAngle );

#pragma region ("point0")
					if ( 0 == IndPoint )
					{
						double SizeX		=	GetAt(0).dX - GetAt(2).dX;
						double SizeY		=	GetAt(0).dY - GetAt(2).dY;

						double MoveSizeX	=	NMouseX - GetAt(2).dX;
						double MoveSizeY	=	NMouseY - GetAt(2).dY;

						if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
						{
							double OffSetY	=	-MoveSizeX / m_nAspect;

							GetAt(0).Create ( NMouseX,		GetAt(2).dY - OffSetY );
							GetAt(1).Create ( GetAt(2).dX,	GetAt(2).dY - OffSetY );
							GetAt(3).Create ( NMouseX,		GetAt(2).dY );
						}
						else
						{
							double OffSetX	=	-MoveSizeY * m_nAspect;

							GetAt(0).Create ( GetAt(2).dX - OffSetX,	NMouseY );
							GetAt(1).Create ( GetAt(2).dX,				NMouseY );
							GetAt(3).Create ( GetAt(2).dX - OffSetX,	GetAt(2).dY );
						}
						
						if ( SizeY >= 0.0 )
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY + 30.0 );
						}
						else
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY - 30.0 );
						}

						GetAt(5).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
					}
#pragma endregion
#pragma region ("point1")
					if ( 1 == IndPoint )
					{
						double SizeX		=	GetAt(1).dX - GetAt(3).dX;
						double SizeY		=	GetAt(1).dY - GetAt(3).dY;

						double MoveSizeX	=	NMouseX - GetAt(3).dX;
						double MoveSizeY	=	NMouseY - GetAt(3).dY;


						if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
						{
							double OffSetY	=	MoveSizeX / m_nAspect;

							GetAt(0).Create ( GetAt(3).dX,	GetAt(3).dY - OffSetY );
							GetAt(1).Create ( NMouseX,		GetAt(3).dY - OffSetY );
							GetAt(2).Create ( NMouseX,		GetAt(3).dY );
						}
						else
						{
							double OffSetX	=	-MoveSizeY * m_nAspect;

							GetAt(0).Create ( GetAt(3).dX,				NMouseY );
							GetAt(1).Create ( GetAt(3).dX + OffSetX,	NMouseY );
							GetAt(2).Create ( GetAt(3).dX + OffSetX,	GetAt(3).dY );
						}

						if ( SizeY >= 0.0 )
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY + 30.0 );
						}
						else
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY - 30.0 );
						}

						GetAt(5).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
					}
#pragma endregion
#pragma region ("point2")
					if ( 2 == IndPoint )
					{
						double SizeX		=	 ( GetAt(2).dX - GetAt(0).dX );
						double SizeY		=	 ( GetAt(2).dY - GetAt(0).dY );

						double MoveSizeX	=	 ( NMouseX - GetAt(0).dX );
						double MoveSizeY	=	 ( NMouseY - GetAt(0).dY );
						
						//ATLTRACE ( "NMouseX : %f, NMouseY : %f, GetAt(0).dX : %f, GetAt(0).dY : %f\n", NMouseX, NMouseY, GetAt(0).dX, GetAt(0).dY );
						//ATLTRACE ( "SizeX : %f, SizeY : %f, MoveSizeX : %f, MoveSizeY : %f\n", SizeX, SizeY, MoveSizeX, MoveSizeY );

						if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
						{
							double OffSetY	=	MoveSizeX / m_nAspect;

							GetAt(1).Create ( NMouseX,		GetAt(0).dY );
							GetAt(2).Create ( NMouseX,		GetAt(0).dY + OffSetY );
							GetAt(3).Create ( GetAt(0).dX,	GetAt(0).dY + OffSetY );
						}
						else
						{
							double OffSetX	=	MoveSizeY * m_nAspect;

							GetAt(1).Create ( GetAt(0).dX + OffSetX,	GetAt(0).dY );
							GetAt(2).Create ( GetAt(0).dX + OffSetX,	NMouseY );
							GetAt(3).Create ( GetAt(0).dX,				NMouseY );
						}

						if ( SizeY <= 0.0 )
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY + 30.0 );
						}
						else
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY - 30.0 );
						}

						GetAt(5).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
					}
#pragma endregion
#pragma region ("point3")
					if ( 3 == IndPoint )
					{
						double SizeX		=	GetAt(3).dX - GetAt(1).dX;
						double SizeY		=	GetAt(3).dY - GetAt(1).dY;

						double MoveSizeX	=	NMouseX - GetAt(1).dX;
						double MoveSizeY	=	NMouseY - GetAt(1).dY;

						if ( abs ( MoveSizeX / MoveSizeY ) > abs ( SizeX / SizeY ) )
						{
							double OffSetY	=	-MoveSizeX / m_nAspect;

							GetAt(0).Create ( NMouseX,		GetAt(1).dY );
							GetAt(2).Create ( GetAt(1).dX,	GetAt(1).dY + OffSetY );
							GetAt(3).Create ( NMouseX,		GetAt(1).dY + OffSetY );
						}
						else
						{
							double OffSetX	=	MoveSizeY * m_nAspect;

							GetAt(0).Create ( GetAt(1).dX - OffSetX,	GetAt(1).dY );
							GetAt(2).Create ( GetAt(1).dX,				NMouseY );
							GetAt(3).Create ( GetAt(1).dX - OffSetX,	NMouseY );
						}

						if ( SizeY <= 0.0 )
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY + 30.0 );
						}
						else
						{
							GetAt(4).Create ( ( GetAt(1).dX + GetAt(0).dX ) * 0.5, GetAt(1).dY - 30.0 );
						}

						GetAt(5).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
					}
#pragma endregion		
					for ( int i = 0; i < 6; ++i )
					{
						Geometry::RotatePoint( GetAt(i).dX, GetAt(i).dY, RotateX, RotateY, dAngle );
					}
				}
			}
		}

	private:
		bool	m_IsFirstResize;
		double	m_dStickAngle;

		double	m_nAspect;
		BOOL	m_bMoveCapturePoint;	// точка по диагонали, по отношению к текущей захваченной остается на месте
		BOOL	m_bSetNoResize;			// трек можно только вращать

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;
	};

	//------------------------------------------------------------------------------------------------------------------
	// Text In None Rotate Rectangle
	//------------------------------------------------------------------------------------------------------------------
	/*class CTrackObjectText : public CTrackObject
	{
	public:
		CTrackObjectText ()
		{

		}

		virtual ~CTrackObjectText()
		{
			m_arrStrings.RemoveAll ();
		}

	public:

		inline void				SetString	( CString string )		{ m_String	 = string;	 }
		inline void				SetFontName	( CString fontName )	{ m_FontName = fontName; }
		inline void				SetCursorPos( int currPos )			{ m_CurrPos  = currPos;	 }
		inline void				SetFontSize ( int fontSize )		{ m_FontSize = fontSize; }
		inline void				SetFontType ( int fontType )		{ m_FontType = fontType; }
		inline const CString&	GetString ()		const	{ return m_String; }
		inline const CString&	GetFontName	()		const	{ return m_FontName; }
		inline const int&		GetCurrPos ()		const  	{ return m_CurrPos; }
		inline const int&		GetFontSize ()		const  	{ return m_FontSize; }
		inline const int&		GetFontType ()		const	{ return m_FontType; }
		inline const int&		GetFontColor ()		const	{ return m_FontColor; }
		inline const bool&		GetBlockEditing ()	const	{ return m_bBlockEditing; }
	protected:

		virtual int GetPointUnderCursor( int nX, int nY )
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0 || nTrackIndex > 7)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect(0, 7);
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
				else
					return -1;
			}

			return nTrackIndex;
		}
		virtual int GetStringByPos( int &caretPos )
		{
			int arrSize = m_arrStrings.GetSize();
			for( int index = 0; index < arrSize; ++index )
			{
				if( caretPos < m_arrStrings[index].GetLength() || ( index == arrSize - 1 ) ) return index;
				caretPos -= m_arrStrings[index].GetLength();
			}
			return -1;
		}

		virtual int PosOnMouse( int dX, int dY )
		{
			if ( NULL == m_hDC ) 
				return -1;

			int countLines = m_arrStrings.GetSize();

			if ( 0 == countLines )
				return -1;

			Gdiplus::Graphics graphics( m_hDC );

			BSTR fontName = m_FontName.AllocSysString();

			int fontStyle = Gdiplus::FontStyleRegular;

			if(	m_FontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
			if( m_FontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( m_FontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( m_FontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			Gdiplus::RectF			orig, currStrRect;
			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );
			SysFreeString( fontName );

			if( !m_ScaleText )
			{
				switch( GetFontAlignedW( m_FontType ) )
				{
				case ALIGNED_LEFT:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				case ALIGNED_CENTER:
					stringFormat.SetAlignment( StringAlignmentCenter ); 
					break;
				case ALIGNED_RIGHT:
					stringFormat.SetAlignment( StringAlignmentFar ); 
					break;
				default:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				}
			}

			Structures::RECTD rectD( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectD.Normalize();

			if( dY > rectD.bottom ) return -1;

			double dWidth	= rectD.GetWidth();
			double dWidth_2 = dWidth / 2.0;

			Structures::POINTI pointTrack( dX, dY );

			int tmpStrLen = 0;
			double fontHeight = Rendering::Utils::GetFontHeight( &font );

			BSTR line = m_arrStrings[0].AllocSysString();
			graphics.MeasureString ( line, -1, &font, orig, &stringFormat, &currStrRect );
			currStrRect.Height = (Gdiplus::REAL)fontHeight;
			SysFreeString( line );

			double displaceByY = countLines * currStrRect.Height;

			for( int i = 0; i < countLines; ++i )
			{
				BSTR line = m_arrStrings[i].AllocSysString();
				graphics.MeasureString( line, -1, &font, orig, &stringFormat, &currStrRect );	
				SysFreeString( line );
				currStrRect.Height = (Gdiplus::REAL)fontHeight;

				currStrRect.X		= (Gdiplus::REAL)rectD.left;
				currStrRect.Width	= (Gdiplus::REAL)dWidth;

				if( !m_ScaleText )
				{
					switch( m_FontAlignW )
					{
					case byTop:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
							break;
						}
					case byMiddle:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.GetCenter().y - (Gdiplus::REAL)displaceByY / 2.0f );
							break;	
						}
					case byBottom:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.bottom - (Gdiplus::REAL)displaceByY );
							break;	
						}
					default:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
						}
					}
				}
				else
				{
					currStrRect.Height = (Gdiplus::REAL)fontHeight * (Gdiplus::REAL)m_Scale.y * (Gdiplus::REAL)countLines;
					currStrRect.Offset( 0.0, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
				}

				if ( currStrRect.Contains ( (Gdiplus::REAL)dX, (Gdiplus::REAL)dY ) )
				{
					BSTR line = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( line, -1, &font, orig, &stringFormat, &currStrRect );
					SysFreeString( line );
					currStrRect.Height = (Gdiplus::REAL)fontHeight;
					currStrRect.Width  *= (Gdiplus::REAL)m_Scale.x;
					currStrRect.Height *= (Gdiplus::REAL)m_Scale.y;

					if( stringFormat.GetAlignment() == StringAlignmentNear )
						currStrRect.Offset( (Gdiplus::REAL)rectD.left, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					else
						if( stringFormat.GetAlignment() == StringAlignmentCenter )
							currStrRect.Offset( (Gdiplus::REAL)rectD.left + (Gdiplus::REAL)dWidth_2, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
						else 
							if( stringFormat.GetAlignment() == StringAlignmentFar )
								currStrRect.Offset ( (Gdiplus::REAL)rectD.right, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );

					double partOfChar = (double)font.GetSize() * m_Scale.x / 5.0;

					StringFormat localStringFormat;
					localStringFormat.SetAlignment(StringAlignmentNear);
					BSTR lineStringAll = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( lineStringAll, -1, &font, orig, &localStringFormat, &currStrRect );
					SysFreeString( lineStringAll );

					CStringW lineString = m_arrStrings[i];
					lineString.Replace( L"\r\n", L"" );

					if( stringFormat.GetAlignment() == StringAlignmentNear )
					{
						for( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.left )
								return tmpStrLen + j;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;
						}	
					}
					else if( stringFormat.GetAlignment() == StringAlignmentCenter )
					{
						for( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= ( dX - (rectD.left + rectD.GetWidth() / 2 ) ) + currStrRect.Width / 2 )
								return tmpStrLen + j;
							else if( dX < currStrRect.X ) 
								return tmpStrLen;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;
						}	
					}
					else if( stringFormat.GetAlignment() == StringAlignmentFar )
					{				
						for( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.right + currStrRect.Width )
								return tmpStrLen + j;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;
						}
					}
					return tmpStrLen + m_arrStrings[i].GetLength();// -2 for skeep "\r\n"
				}
				tmpStrLen += m_arrStrings[i].GetLength();
			}
			return -1;				
		}

		inline void SelectByDblClick ()
		{
			m_SelectStart = m_SelectEnd = 0;
			int tmpBegin = 0;

			while ( m_SelectStart <= m_CurrPos )
			{
				tmpBegin = m_String.Find ( ' ', m_SelectStart );

				if ( -1 == tmpBegin )
				{
					m_SelectEnd = m_String.GetLength();
					return;
				}
				else if ( tmpBegin >= m_CurrPos )
				{
					m_SelectEnd = tmpBegin;
					return;
				}
				else if ( tmpBegin > m_String.GetLength() - 1 )
				{
					m_SelectEnd		=	tmpBegin;
					return;
				}
				else
				{
					m_SelectStart	=	tmpBegin + 1;
				}
			}
		}

		virtual void SplitString( void )
		{
			m_arrStrings.RemoveAll();

			if( NULL == m_hDC ) return;
			if( 0 == m_String.GetLength() && m_AutoSize )
			{
				GetAt(1).Create( GetAt(0).dX + m_FontSize, GetAt(0).dY );
				GetAt(2).Create( GetAt(0).dX + m_FontSize, GetAt(0).dY + m_FontSize );
				GetAt(3).Create( GetAt(0).dX, GetAt(0).dY + m_FontSize );

				GetAt(4).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(0).dY );
				GetAt(5).Create( GetAt(1).dX, 0.5 * ( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(2).dY );
				GetAt(7).Create( GetAt(0).dX, 0.5 * ( GetAt(0).dY + GetAt(3).dY ) );

				return;
			}
			else if( 0 == m_String.GetLength() )
				return;

			Graphics graphics( m_hDC );

			Structures::RECTD rectD( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectD.Normalize();

			double dWidth  = rectD.GetWidth();

			RectF orig, rectF, strBB;

			BSTR fontName = m_FontName.AllocSysString();
			int fontStyle = Gdiplus::FontStyleRegular;
			if(	m_FontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
			if( m_FontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( m_FontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( m_FontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );
			SysFreeString( fontName );

			if( dWidth < font.GetSize() && !m_ScaleText && !m_AutoSize ) return;

			double maxWidth		= 0.0;
			double maxHeight	= 0.0;

			int strLen = m_String.GetLength();

			int		strLine		= 0;
			int		countChar	= 1;
			double  displaceX	= 0.0;
			double  displaceY	= 0.0;
			bool	isEnd		= false;

			BSTR fullString = m_String.AllocSysString();

			for( int i = 0; i < strLen; ++i )
			{
				if( *(fullString + strLine + countChar - 1) == '\r' )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );
					i++;
					tmpStr.Delete( countChar + 1, strLen - i + 2);
					m_arrStrings.Add( tmpStr );
					strLine		= i + 1;
					countChar	= 1;
					isEnd = true;
					continue;
				}
				graphics.MeasureString( fullString + strLine, countChar, &font, orig, &stringFormat, &strBB );
				displaceX = strBB.Width;
				maxWidth = max( maxWidth, displaceX );
				if( displaceX > dWidth && !m_AutoSize && !m_ScaleText )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );					
					tmpStr.Delete( countChar - 1, strLen - i );
					BSTR subString = tmpStr.AllocSysString();
					BSTR rSpace = wcsrchr( subString, ' ' );
					if( NULL != rSpace )
					{						
						int strLenSpace = (int)wcslen( rSpace );
						i -= strLenSpace - 1;
						tmpStr.Delete( countChar - strLenSpace, strLenSpace);
					}
					m_arrStrings.Add( tmpStr );
					strLine		= i;
					countChar	= 1;
					isEnd = true;
					SysFreeString( subString );
				}	
				countChar++;
				isEnd = false;
			}
			if( !isEnd )
			{
				m_arrStrings.Add ( CStringW ( fullString + strLine ) + L"\r\n" );
			}
			m_String.FreeExtra();

			double fontHeight = Rendering::Utils::GetFontHeight( &font );
			if( m_ScaleText )
			{
				double dHeight = rectD.GetHeight();

				maxHeight = fontHeight * m_arrStrings.GetSize();

				if( m_arrStrings.GetSize() > 0 && maxHeight > 0)
				{
					m_Scale.x = dWidth / max( 0.001, maxWidth );
					m_Scale.y = dHeight / maxHeight / m_arrStrings.GetSize();
				}
			}
			else if( m_AutoSize )
			{
				maxHeight = fontHeight * m_arrStrings.GetSize();

				GetAt(2).dX = GetAt(0).dX + maxWidth;
				GetAt(2).dY = GetAt(0).dY + maxHeight;
				GetAt(1).dX = GetAt(0).dX + maxWidth;
				GetAt(3).dY = GetAt(0).dY + maxHeight;

				GetAt(4).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(0).dY );
				GetAt(5).Create( GetAt(1).dX, 0.5 * ( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(2).dY );
				GetAt(7).Create( GetAt(0).dX, 0.5 * ( GetAt(0).dY + GetAt(3).dY ) );
			}
			SysFreeString( fullString );
		}
		virtual void EndEditText()
		{
			if( m_bEditingModeEnabled )
			{
				m_bEditingModeEnabled = false;
				SendMessage( Constants::sc_nMessageTrackTextEnd, GetCurrentPointIndex() );
			}
		}
		void CopyToClipboard( bool isCut = false )
		{
			LPWSTR 	pstrCopy;
			HGLOBAL	glblCopy;

			if( m_SelectStart == m_SelectEnd ) return;

			CString strSelected = m_String;
			int beginSelect = min( m_SelectStart, m_SelectEnd );
			int endSelect	= max( m_SelectStart, m_SelectEnd );

			strSelected = m_String;
			strSelected.Delete( endSelect, strSelected.GetLength() - endSelect );
			strSelected.Delete( 0, beginSelect );

			if( isCut )
			{
				m_String.Delete( beginSelect, endSelect - beginSelect );
				SplitString();
				m_CurrPos = beginSelect; 
				m_SelectEnd = m_SelectStart = m_CurrPos;
				SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
			}

			int strLength = strSelected.GetLength();

			glblCopy = GlobalAlloc( GMEM_MOVEABLE, strLength * sizeof(WCHAR) + 2 );
			pstrCopy = (LPWSTR)GlobalLock( glblCopy );
			BSTR	srcStringW = strSelected.AllocSysString();
			wcscpy( pstrCopy, srcStringW );
			SysFreeString( srcStringW );
			GlobalUnlock( glblCopy );
			OpenClipboard( NULL );
			EmptyClipboard();
			SetClipboardData( CF_UNICODETEXT, glblCopy );
			CloseClipboard();
		}

		void PastToClipboard()
		{
			LPWSTR	pstrPast;
			HGLOBAL glblPast;

			if( !IsClipboardFormatAvailable( CF_UNICODETEXT ) )
				return;
			if( !OpenClipboard( NULL ) ) 
				return;

			if( glblPast = GetClipboardData( CF_UNICODETEXT ) )
			{
				if( pstrPast = (LPWSTR)GlobalLock( glblPast ) )
				{
					int beginSelect = min( m_SelectStart, m_SelectEnd );
					int endSelect	= max( m_SelectStart, m_SelectEnd );

					if( beginSelect != endSelect )
					{
						m_String.Delete( beginSelect, endSelect - beginSelect );
						m_CurrPos = beginSelect;
						m_SelectEnd = m_SelectStart = m_CurrPos;
					}

					CString strPast = CString( pstrPast );
					m_String.Insert( m_CurrPos, strPast );
					m_CurrPos = m_SelectEnd + strPast.GetLength();
					m_SelectEnd = m_SelectStart = m_CurrPos;
					GlobalUnlock( glblPast );
					SplitString();
					SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
				}
			}
			CloseClipboard();
		}


		virtual void OnKeyDown ( unsigned int Key )
		{
			if( !m_bEditingModeEnabled ) return;
			bool isShift	= false;
			bool isCtrl		= false;

			if(	Key == VK_ESCAPE )
			{
				EndEditText();
				m_CurrPos		= m_SelectEnd;
				m_SelectStart	= m_SelectEnd;
			}
			else if( Key == VK_CONTROL )
			{
				isCtrl = true;
			}
			else if( Key == VK_LEFT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos > 0 ) m_CurrPos--;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;
			}
			else if( Key == VK_RIGHT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos < m_String.GetLength() ) m_CurrPos++;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos++;
			}
			else if( Key == VK_UP )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( currIdxString < 1 ) return;

				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();

				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if( m_arrStrings[currIdxString - 1].GetLength() - 2 < posInLine ) // - 2 for "\r\n"
						m_CurrPos -= posInLine + 2; // + 2 for "\r\n"
					else
						m_CurrPos -= m_arrStrings[currIdxString - 1].GetLength();	
				}
				else
				{
					if( m_arrStrings[currIdxString - 1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine ) // - 2 for "\r\n"	
						m_CurrPos -= posInLine + m_arrStrings[currIdxString - 1].GetLength();
					else
						m_CurrPos -= m_arrStrings[currIdxString].GetLength();
				}
			}
			else if( Key == VK_DOWN )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( -1 == currIdxString || currIdxString == m_arrStrings.GetSize() - 1 ) return;
				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();
				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if( m_arrStrings[currIdxString + 1].GetLength() - 2 < posInLine ) 
						m_CurrPos += sumLineLenght - m_CurrPos + m_arrStrings[currIdxString + 1].GetLength() - 2;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength();
				}
				else
				{
					if( m_arrStrings[currIdxString+1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine )
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine + (m_arrStrings[currIdxString+1].GetLength() - m_arrStrings[currIdxString].GetLength() + posInLine );
				}
			}
			else if ( Key == VK_SHIFT )
			{
				//if( !m_IsSelect ) m_SelectStart = m_SelectEnd = m_CurrPos;
				isShift = true;
			}
			else if( Key == VK_DELETE )
			{
				if( m_CurrPos == m_String.GetLength() && m_SelectStart == m_SelectEnd ) return;

				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				if( m_SelectStart != m_SelectEnd )
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos + 2 <= m_String.GetLength() )
						if( m_String[ m_CurrPos + 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos, 2 );
							deleteRN = true;
						}
						if( !deleteRN )
							m_String.Delete( m_CurrPos );
				}

				SplitString();
			}
			else if( Key == VK_BACK )
			{
				if( m_CurrPos == 0 && m_SelectStart == m_SelectEnd ) return;

				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos > 0 )
						if( m_String[ m_CurrPos - 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos - 2, 2 );
							m_CurrPos -= 2;
							deleteRN = true;
						}
						if( !deleteRN )
						{
							m_String.Delete( m_CurrPos - 1 );
							m_CurrPos--;
						}
				}

				SplitString();

			}
			else if( Key == VK_SPACE )
			{
				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += _T(" ");
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, _T(" ") );
					m_CurrPos++;
				}

				SplitString();

			}
			else if( Key == VK_RETURN )
			{
				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += _T("\r\n");
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, _T("\r\n") );
					m_CurrPos += 2;
				}

				SplitString();

			}
			//else if( Key == VK_TAB )
			//{
			//}
			else if( Key == VK_HOME )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				m_CurrPos = 0;
			}
			else if( Key == VK_END )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				m_CurrPos = m_String.GetLength();
			}
			else if( ( Key >= 'A' && Key <= 'Z' ) || ( Key >= '0' && Key <= '9' ) ||
				( Key >= VK_OEM_1 && Key <= VK_OEM_3 ) || ( Key >= VK_OEM_4 && Key <= VK_OEM_8 ) )
			{
				if( GetAsyncKeyState( VK_CONTROL ) < 0 )
				{
					if( Key == 'X' )
						CopyToClipboard( true );
					else if( Key == 'C' )
						CopyToClipboard();
					else if( Key == 'V' )
						PastToClipboard();
					return;
				}
				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
					m_IsSelect = false;
				}
				if(m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				BYTE kbdState[256];
				ZeroMemory( kbdState, 256 );
				WORD wKey;
				GetKeyboardState( kbdState );

				if( ToAscii( Key, 0, kbdState, &wKey, 0) != 1 ) wKey = 0;

				char key[] = { wKey & 0xFF, '\0' };

				wchar_t wstr[2];
				MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, key, -1, wstr, 1 );
				wstr[1] = '\0';

				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += CString( wstr );
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, CString( wstr ) );
					m_CurrPos++;
				}

				SplitString();

			}
			if( !isShift && !isCtrl )
			{
				if( m_IsSelect )	m_SelectEnd = m_CurrPos;
				else				m_SelectStart = m_SelectEnd = m_CurrPos;
			}
			SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
		}
		virtual void OnKeyUp( unsigned int Key )
		{
			if( Key == VK_SHIFT )
			{
				m_IsSelect	= false;
			}
		}


	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if ( nMessage == WM_KEYDOWN )
				OnKeyDown( (UINT)wParam );
			else if ( nMessage == WM_KEYUP )
				OnKeyUp ( (UINT)wParam );
			else if ( nMessage == WM_DESTROY )
				m_arrStrings.RemoveAll();
			else if ( nMessage == WM_KILLFOCUS )
				EndEditText();
			else if ( nMessage == WM_MOUSELEAVE )	
				m_IsSelect	= false;
			else if ( nMessage == WM_LBUTTONDBLCLK )
			{
				//ATLTRACE ("DOUBLE CLICK \n");

				if ( m_bBlockEditing )
					return FALSE;

				// for EnabkeUseOnlyRgnDblClk 

				CTrackPoint MinPoint	=	GetAt ( 0 );
				CTrackPoint MaxPoint	=	GetAt ( 0 );

				double iMouseX			=	MapToDataX ( (int)GET_X_LPARAM ( lParam ) );
				double iMouseY			=	MapToDataY ( (int)GET_Y_LPARAM ( lParam ) );

				//ATLTRACE ( "DOUBLE CLICK: %d, %d \n", (int)GET_X_LPARAM ( lParam ), (int)GET_Y_LPARAM ( lParam ) );

				if ( m_bEnabkeUseOnlyRgnDblClk )
				{
					for ( int i = 0; i < 4; ++i )
					{
						double dX		=	GetAt ( i ).dX;
						double dY		=	GetAt ( i ).dY;

						if ( MinPoint.dX > dX )
							MinPoint.dX = dX;
						if ( MinPoint.dY > dY )
							MinPoint.dY = dY;

						if ( MaxPoint.dX < dX )
							MaxPoint.dX = dX;
						if ( MaxPoint.dY < dY )
							MaxPoint.dY = dY;
					}
				}

				if ( m_bEditingModeEnabled ) 
				{
					if ( m_bEnabkeUseOnlyRgnDblClk )
					{
						if ( false == ( MinPoint.dX <= iMouseX && MaxPoint.dX >= iMouseX && MinPoint.dY <= iMouseY && MaxPoint.dY >= iMouseY ) )
						{
							EndEditText();

							m_CurrPos		= m_SelectEnd;
							m_SelectStart	= m_SelectEnd;

							return FALSE;
						}
					}

					SelectByDblClick(); 
					OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
					return FALSE;
				}
				else
				{
					if ( m_bEnabkeUseOnlyRgnDblClk )
					{
						if ( false == ( MinPoint.dX <= iMouseX && MaxPoint.dX >= iMouseX && MinPoint.dY <= iMouseY && MaxPoint.dY >= iMouseY ) )
						{
							return FALSE;
						}
					}

					m_bEditingModeEnabled = true;
					SendMessage( Constants::sc_nMessageTrackTextBegin, GetCurrentPointIndex() );
					SplitString();
					m_SelectStart = m_SelectEnd = m_CurrPos;
					OnLButtonDown ( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
				}
			}

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ((nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 && !m_AutoSize )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			if( PosOnMouse ( (int)MapToDataX( nX ), (int)MapToDataY( nY ) ) != -1 && m_bEditingModeEnabled && !m_IsTransfom )
				nPointStatus = c_nTrackPointStatusIBeam;

			return FALSE;
		}
		virtual void GetZoom( Tracking::CTrackPoints* pTrack )
		{
			if (!pTrack || pTrack->GetCount() < 1 ) return;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray( pPoints, 10 );

			Structures::RECTD rectD( pPoints->x, pPoints->y, (pPoints+2)->x, (pPoints+2)->y );
			rectD.Normalize();	

			Structures::RECTD rectDTracking( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectDTracking.Normalize();

			if( rectDTracking.GetWidth() > 0 )
				m_dZoom = rectD.GetWidth() / rectDTracking.GetWidth();

			if( pPoints )
			{
				delete []pPoints;
				pPoints = NULL;
			}
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			BSTR string   = m_String.AllocSysString();
			BSTR fontName = m_FontName.AllocSysString();
			POINT selectInfo;
			selectInfo.x = m_SelectStart;
			selectInfo.y = m_SelectEnd;
			GetZoom( this );
			if( m_hDC != oPainter.GetDC() ) m_hDC = oPainter.GetDC();
			if( m_AutoSize && m_IsFirstResize &&
				( GetAt(1).dX == GetAt(0).dX + m_FontSize ) && 
				( GetAt(2).dY == GetAt(1).dY + m_FontSize ) ) 
			{
				SplitString();
				if( wcslen( string ) > 0 ) return;
			}
			oPainter.DrawEditTextW ( this, m_dZoom, m_arrStrings, string, m_CurrPos, selectInfo, fontName, 
				m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText );
			SysFreeString( string ); 
			SysFreeString( fontName ); 
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if( !m_AutoSize ) oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}
		inline void EnableBlockEditing ( bool Value )
		{
			m_bBlockEditing				=	Value;
		}

		inline void EnabkeUseOnlyRgnDblClk ( bool Value )
		{
			m_bEnabkeUseOnlyRgnDblClk	=	Value;
		}

	public:
		virtual void Create( double dLeft, double dTop, double dRight, double dBottom, WCHAR* string, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool bBlockEditing = false )
		{
			m_String   		= CStringW ( string );
			m_FontName 		= CStringW ( fontName );
			m_CurrPos  		= m_String.GetLength();
			m_FontSize 		= fontSize;
			m_FontType		= fontType;
			m_FontColor		= FontColor;
			m_bEditingModeEnabled		= false;
			m_IsSelect		= false;
			m_IsTransfom	= true;
			m_SelectStart	= 0;
			m_SelectEnd		= 0;
			m_hDC			= NULL;
			m_dZoom			= 1.0;
			m_bBlockEditing				=	bBlockEditing;
			m_bEnabkeUseOnlyRgnDblClk	=	false;

			if( dRight < 0 && dBottom < 0 ) 
				m_AutoSize = true;
			else					
				m_AutoSize = false;

			//m_ScaleText = (bool)GetFontScale( m_FontType );

			if ( GetFontScale( m_FontType ) != 0 )
				m_ScaleText = true;
			else
				m_ScaleText = false;

			m_Scale.x = 1.0;
			m_Scale.y = 1.0;

			switch( GetFontAlignedH( m_FontType ) )
			{
			case ALIGNED_TOP:
				m_FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				m_FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				m_FontAlignW = byBottom;
				break;
			default:
				m_FontAlignW = byTop;
			}

			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 || m_AutoSize )	m_IsFirstResize = true;
			else																		m_IsFirstResize = false;

			SetType( Constants::sc_nTrackTypeText );

			Clear();

			if( m_AutoSize )
			{
				dRight = dLeft + fontSize;
				dBottom = dTop + fontSize;
			}

			Add( dLeft,  dTop );
			Add( dRight, dTop );
			Add( dRight, dBottom );
			Add( dLeft,  dBottom );

			Add( 0.5 * ( dLeft + dRight ), dTop );
			Add( dRight, 0.5 * ( dTop + dBottom ) );
			Add( 0.5 * ( dLeft + dRight ), dBottom );
			Add( dLeft, 0.5 * ( dTop + dBottom ) );

			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			SetCurrentPointIndex(-1);
		}	
	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if( !m_IsTransfom )
			{
				int tmpPos = PosOnMouse ( (int)dX, (int)dY );
				if( tmpPos != -1 && m_bEditingModeEnabled )
				{
					if( m_IsSelect && !(GetAsyncKeyState( VK_SHIFT ) < 0) ) 
					{
						m_CurrPos	= tmpPos;
						m_SelectEnd = tmpPos;
					}

					if (false == IsTrackChild () ) 
						SendMessage(Constants::sc_nMessageTrackInvalidate, GetCurrentPointIndex());

					return false;
				}
			}

			if (!IsCurrentPointValid())
				return false;

			int nCurrentPoint = ( m_IsFirstResize && !m_AutoSize ) ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if (false == IsTrackChild () ) 
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				if( m_AutoSize ) 
					return true;

				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;
				}
				else if (nCurrentPoint == 4)
				{
					GetAt(4).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 5)
				{
					GetAt(5).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}
				else if (nCurrentPoint == 6)
				{
					GetAt(6).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 7)
				{
					GetAt(7).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 4)
				{
					GetAt(4).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(0).dY);
					GetAt(5).Create(GetAt(2).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
					GetAt(6).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(2).dY);
					GetAt(7).Create(GetAt(0).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
				}

				// recompute corners
				if (nCurrentPoint >= 4 && nCurrentPoint < 8)
				{
					GetAt(0).Create(GetAt(7).dX, GetAt(4).dY);
					GetAt(1).Create(GetAt(5).dX, GetAt(4).dY);
					GetAt(2).Create(GetAt(5).dX, GetAt(6).dY);
					GetAt(3).Create(GetAt(7).dX, GetAt(6).dY);
				}

				SplitString();

				if (false == IsTrackChild () ) 
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int tmpPos = PosOnMouse ( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );

			if( tmpPos != -1 && m_bEditingModeEnabled ) 
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 )
				{
					m_CurrPos	= tmpPos;
					m_SelectEnd	= tmpPos;
				}
				else
				{
					m_CurrPos		= tmpPos;
					m_SelectStart	= tmpPos;
				}
				m_IsSelect		= true;
				m_IsTransfom	= false;

				// it's for repaint
				OnMouseMove( MouseX, MouseY );
				return true;
			}

			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY );

			if( nTrackIndex >= 0 && nTrackIndex < 8 )
				m_IsTransfom = true;

			if ( -1 == nTrackIndex )
				return false;

			if( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create ( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			if( -1 == GetPointUnderCursor( MouseX, MouseY ) )
				EndEditText();

			if( m_AutoSize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			SetCurrentPointIndex( nTrackIndex );

			if ( false== IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return false; 
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			m_IsFirstResize = false;

			if( GetAsyncKeyState( VK_SHIFT ) >= 0 ) 
			{
				if( m_IsSelect )
				{
					int tmpPos = PosOnMouse ( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
					if( tmpPos != -1 && m_bEditingModeEnabled ) 
					{
						m_CurrPos		= tmpPos;
						m_SelectEnd 	= tmpPos;
					}
					m_IsSelect		= false;
				}
			}

			if (!IsCurrentPointValid())
				return false;

			if ( false== IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			m_IsTransfom = false;

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	
	protected:
		
		HDC		m_hDC;
		double	m_dZoom;
		bool	m_IsFirstResize;
		bool	m_AutoSize;
		bool	m_ScaleText;

		Structures::POINTD	m_Scale;

		CString	m_String;
		CString	m_FontName;
		int		m_CurrPos;
		int		m_FontSize;
		int		m_FontType;
		int		m_FontColor;
		bool	m_bEditingModeEnabled;
		bool	m_IsSelect;
		bool	m_IsTransfom;
		int		m_SelectStart;
		int		m_SelectEnd;
		int		m_FontAlignW;

		//CSimpleArray<CString> m_arrStrings;

		CSimpleArray < CStringW > m_arrStrings;

		bool	m_bBlockEditing;				// блокировка редактирования
		bool	m_bEnabkeUseOnlyRgnDblClk;		// использовать двойнок клик в области трека, клик в зоне - активация, вне зоны сброс.
	};
	//------------------------------------------------------------------------------------------------------------------
	// TextRotatable
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectTextRotatable : public CTrackObjectText
	{
	protected:

		int GetPointUnderCursor( int nX, int nY )
		{
			int nTrackIndex = FindByInteger( nX, nY, c_nTrackPointSizeTouch, FALSE );

			if( nTrackIndex < 0 )
			{
				Structures::RECTI rectD;

				double dX = MapToDataX( nX );
				double dY = MapToDataY( nY );

				double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );

				double tX, tY;
				double cosA = cos( -(dAngle + M_PI_2) );
				double sinA = sin( -(dAngle + M_PI_2) );

				tX = dX - GetAt(9).dX;
				tY = dY - GetAt(9).dY;				
				dX = GetAt(9).dX + tX * cosA - tY * sinA;
				dY = GetAt(9).dY + tX * sinA + tY * cosA;

				Structures::POINTI pointTrack ( (int)dX, (int)dY );

				rectD.left   = (int)GetAt(0).dX;
				rectD.top    = (int)GetAt(0).dY;
				rectD.right  = (int)GetAt(2).dX;
				rectD.bottom = (int)GetAt(2).dY;

				tX = GetAt(0).dX - GetAt(9).dX;
				tY = GetAt(0).dY - GetAt(9).dY;				
				rectD.left  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.top   = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );	

				tX = GetAt(2).dX - GetAt(9).dX;
				tY = GetAt(2).dY - GetAt(9).dY;
				rectD.right  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.bottom = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

				if( rectD.right < rectD.left ) 
				{ 
					double tmpRight		= rectD.right; 
					rectD.right			= rectD.left;
					rectD.left			= (int)tmpRight;
				}
				if( rectD.top > rectD.bottom ) 
				{ 
					double tmpBottom	= rectD.bottom; 
					rectD.bottom		= rectD.top;
					rectD.top			= (int)tmpBottom;
				}

				if( rectD.IsPointInside( pointTrack ) )
					nTrackIndex = GetCount() - 1;
				else
					return -1;
			}

			return nTrackIndex;
		}

		void RotateToAlpha( double alpha )
		{
			for( int index = 0; index < 9; ++index )
				Geometry::RotatePoint( GetAt(index).dX, GetAt(index).dY, GetAt(9).dX, GetAt(9).dY, alpha );
		}
		int GetStringByPos( int &caretPos )
		{
			int arrSize = m_arrStrings.GetSize();
			for( int index = 0; index < arrSize; ++index )
			{
				if( caretPos < m_arrStrings[index].GetLength() || ( index == arrSize - 1 ) ) return index;
				caretPos -= m_arrStrings[index].GetLength();
			}
			return -1;
		}
		int PosOnMouse( int dX, int dY )
		{
			if( NULL == m_hDC ) return -1;
			int countLines = m_arrStrings.GetSize();
			if( 0 == countLines ) return -1;

			Gdiplus::Graphics graphics( m_hDC );

			BSTR fontName = m_FontName.AllocSysString();
			Gdiplus::RectF			orig, currStrRect;
			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font( &fontFamily, (Gdiplus::REAL)m_FontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
			SysFreeString( fontName );

			if( !m_ScaleText )
			{
				switch( GetFontAlignedW( m_FontType ) )
				{
				case ALIGNED_LEFT:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				case ALIGNED_CENTER:
					stringFormat.SetAlignment( StringAlignmentCenter ); 
					break;
				case ALIGNED_RIGHT:
					stringFormat.SetAlignment( StringAlignmentFar ); 
					break;
				default:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				}
			}

			double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
			Structures::RECTD rectD = GetUnrotatableRect( dAngle );

			double tX, tY;
			double cosA = cos( -dAngle );
			double sinA = sin( -dAngle );

			tX = dX - GetAt(9).dX;
			tY = dY - GetAt(9).dY;
			dX = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
			dY = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

			if( dY > rectD.bottom ) return -1;

			double dWidth	= rectD.GetWidth();
			double dWidth_2 = dWidth / 2.0;

			Structures::POINTI pointTrack( dX, dY );

			int tmpStrLen = 0;

			double fontHeight = Rendering::Utils::GetFontHeight( &font );

			double displaceByY = countLines * fontHeight;

			for( int i = 0; i < countLines; ++i )
			{
				BSTR line = m_arrStrings[i].AllocSysString();
				graphics.MeasureString( line, -1, &font, orig, &stringFormat, &currStrRect );	
				SysFreeString( line );

				currStrRect.X		= (Gdiplus::REAL)rectD.left;
				currStrRect.Width	= (Gdiplus::REAL)dWidth;
				if( !m_ScaleText )
				{
					switch( m_FontAlignW )
					{
					case byTop:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.top );
							break;
						}
					case byMiddle:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.GetCenter().y - (Gdiplus::REAL)displaceByY / 2.0f );
							break;	
						}
					case byBottom:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.bottom - (Gdiplus::REAL)displaceByY );
							break;	
						}
					default:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.top );
						}
					}
				}
				else
				{
					currStrRect.Height = (Gdiplus::REAL)fontHeight * (Gdiplus::REAL)m_Scale.y * (Gdiplus::REAL)countLines;
					currStrRect.Offset( 0.0f, (Gdiplus::REAL) i * (Gdiplus::REAL)currStrRect.Height + (Gdiplus::REAL)rectD.top );
				}

				if( currStrRect.Contains ( (Gdiplus::REAL)dX, (Gdiplus::REAL)dY ) )
				{
					BSTR line = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( line, -1, &font, orig, &stringFormat, &currStrRect );
					SysFreeString( line );
					currStrRect.Height = (Gdiplus::REAL)fontHeight;

					currStrRect.Width  *= (Gdiplus::REAL)m_Scale.x;
					currStrRect.Height *= (Gdiplus::REAL)m_Scale.y;

					if( stringFormat.GetAlignment() == StringAlignmentNear )
						currStrRect.Offset( (Gdiplus::REAL)rectD.left, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					else if( stringFormat.GetAlignment() == StringAlignmentCenter )
						currStrRect.Offset( (Gdiplus::REAL)rectD.left + (Gdiplus::REAL)dWidth_2, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					else if( stringFormat.GetAlignment() == StringAlignmentFar )
						currStrRect.Offset( (Gdiplus::REAL)rectD.right, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );

					double partOfChar = font.GetSize() * m_Scale.x / 5.0;

					StringFormat localStringFormat;
					localStringFormat.SetAlignment(StringAlignmentNear);
					BSTR lineStringAll = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( lineStringAll, -1, &font, orig, &localStringFormat, &currStrRect );
					SysFreeString( lineStringAll );

					CStringW lineString = m_arrStrings[i];
					lineString.Replace( L"\r\n", L"" );

					if( stringFormat.GetAlignment() == StringAlignmentNear )
					{
						for( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.left )
								return tmpStrLen + j;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;

						}	
					}
					else if( stringFormat.GetAlignment() == StringAlignmentCenter )
					{
						for( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= ( dX - (rectD.left + rectD.GetWidth() / 2 ) ) + currStrRect.Width / 2 )
								return tmpStrLen + j;
							else if( dX < currStrRect.X ) 
								return tmpStrLen;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;
						}
					}
					else if( stringFormat.GetAlignment() == StringAlignmentFar )
					{
						for( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, &font, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.right + currStrRect.Width )
								return tmpStrLen + j;
							if ( j == lineString.GetLength() - 1 )
								return tmpStrLen + j + 1;

						}
					}
					return tmpStrLen + m_arrStrings[i].GetLength();// -2 for skeep "\r\n";
				}
				tmpStrLen += m_arrStrings[i].GetLength();
			}
			return -1;				
		}
		void SplitString( void )
		{
			m_arrStrings.RemoveAll();

			if( 0 == m_String.GetLength() || NULL == m_hDC ) return;

			Graphics graphics( m_hDC );

			double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
			Structures::RECTD rectD = GetUnrotatableRect( dAngle );

			double dWidth  = rectD.GetWidth();

			RectF	orig, rectF, strBB;

			BSTR fontName = m_FontName.AllocSysString();
			int fontStyle = Gdiplus::FontStyleRegular;
			if(	m_FontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
			if( m_FontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( m_FontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( m_FontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			Gdiplus::FontFamily		fontFamily( fontName );
			Gdiplus::Font			font( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );
			Gdiplus::StringFormat	stringFormat;
			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
			SysFreeString( fontName );

			double maxWidth		= 0;
			double maxHeight	= 0;

			Gdiplus::REAL nFontSize = font.GetSize();
			if ( dWidth + 0.5 < nFontSize && !m_ScaleText )
				return;

			int strLen = m_String.GetLength();

			int		strLine		= 0;
			int		countChar	= 1;
			double  displaceX	= 0.0;
			double  displaceY	= 0.0;
			bool	isEnd		= false;

			BSTR fullString = m_String.AllocSysString();

			for( int i = 0; i < strLen; ++i )
			{
				if( *(fullString + strLine + countChar - 1) == '\r' )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );
					i++;
					tmpStr.Delete ( countChar + 1, strLen - i + 2);
					
					m_arrStrings.Add ( tmpStr );
					//m_arrStrings.Add ( Rendering::TextRendering::NormNonBreakingSpaces ( tmpStr ) );
					
					strLine		= i + 1;
					countChar	= 1;
					isEnd = true;


					continue;
				}
				graphics.MeasureString( fullString + strLine, countChar, &font, orig, &stringFormat, &strBB );
				displaceX = strBB.Width;
				maxWidth = max( maxWidth, displaceX );
				if( displaceX > dWidth && !m_AutoSize && !m_ScaleText )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );					
					tmpStr.Delete( countChar - 1, strLen - i );
					BSTR subString = tmpStr.AllocSysString();
					BSTR rSpace = wcsrchr( subString, ' ' );
					if( NULL != rSpace )
					{						
						int strLenSpace = (int)wcslen( rSpace );
						i -= strLenSpace - 1;
						tmpStr.Delete( countChar - strLenSpace, strLenSpace);
					}
					m_arrStrings.Add( tmpStr );
					strLine		= i;
					countChar	= 1;
					isEnd = true;
					SysFreeString( subString );
				}	
				countChar++;
				isEnd = false;
			}
			if( !isEnd )
			{
				m_arrStrings.Add ( CStringW ( fullString + strLine ) + L"\r\n" );
			}
			m_String.FreeExtra();

			double fontHeight = Rendering::Utils::GetFontHeight( &font );
			if( m_ScaleText )
			{
				double dHeight = rectD.GetHeight();

				maxHeight = fontHeight * m_arrStrings.GetSize();

				if( m_arrStrings.GetSize() > 0 && maxHeight > 0)
				{
					m_Scale.x = dWidth / max( 0.001, maxWidth );
					m_Scale.y = dHeight / maxHeight / m_arrStrings.GetSize();
				}
			}
			else if( m_AutoSize )
			{
				maxHeight = fontHeight * m_arrStrings.GetSize();

				double eX1, eX2, eY1, eY2;

				eX1 = 1.0; eY1 = 0.0;
				eX2 = 0.0; eY2 = 1.0;

				double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
				Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
				Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );

				GetAt(2).dX = GetAt(0).dX + eX1 * maxWidth + eX2 * maxHeight;
				GetAt(2).dY = GetAt(0).dY + eY1 * maxWidth + eY2 * maxHeight;
				GetAt(1).dX = GetAt(0).dX + eX1 * maxWidth;
				GetAt(1).dY = GetAt(0).dY + eY1 * maxWidth;
				GetAt(3).dX = GetAt(0).dX + eX2 * maxHeight;
				GetAt(3).dY = GetAt(0).dY + eY2 * maxHeight;

				GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
				GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
				GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

				double vX = GetAt(4).dX - GetAt(6).dX;
				double vY = GetAt(4).dY - GetAt(6).dY;

				double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
				if( length < 1 )
				{
					vX = -eX2; 
					vY = -eY2;
				}
				else
				{
					vX /= length;
					vY /= length;
				}

				GetAt(8).dX = GetAt(4).dX + vX * 30;
				GetAt(8).dY = GetAt(4).dY + vY * 30;
				GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );

			}
			SysFreeString( fullString );
		}

		void EndEditText()
		{
			if( m_bEditingModeEnabled )
			{
				m_bEditingModeEnabled = false;
				SendMessage( Constants::sc_nMessageTrackTextEnd, GetCurrentPointIndex() );
			}
		}
		void StickAngle( double &dAngle, double angleStick )
		{
			int		countStick	 = 0;
			double  tmpBaseAngle = dAngle;
			double  rangeStick	 = rad(2);
			if( 0 == angleStick || rangeStick > angleStick ) angleStick = M_PI_2;
			if( dAngle < 0 ) angleStick = -angleStick;
			while( (dAngle < 0) ? (tmpBaseAngle < angleStick + rangeStick) : (tmpBaseAngle > angleStick - rangeStick) )
			{ 
				tmpBaseAngle -= angleStick;
				countStick++;
			}
			if( abs( tmpBaseAngle ) < rangeStick ) 
			{
				tmpBaseAngle = countStick * angleStick;
				dAngle = tmpBaseAngle;
			}
		}
		void UpdateMinimizeRegion ( int CapturePoint )
		{
			double CharSize		=	(double)m_FontSize * (double)GetDeviceCaps ( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
			double CharSizeRotX	=	CharSize;
			double CharSizeRotY	=	CharSize;

			double RotateAngle = Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
			Geometry::RotatePoint ( CharSizeRotX, CharSizeRotY, 0.0, 0.0, RotateAngle );

			double BoundWidth		=	Geometry::GetLength ( GetAt(0).dX, GetAt(0).dY, GetAt(1).dX, GetAt(1).dY );
			double BoundHeight		=	Geometry::GetLength ( GetAt(0).dX, GetAt(0).dY, GetAt(3).dX, GetAt(3).dY );

			double BoundWidthRot	=	BoundWidth;
			double BoundHeightRot	=	BoundHeight;

			//-------------------------------------------------------------------------------------------

			if ( CapturePoint == 0 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;

					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;

					GetAt(0).dY		=	GetAt(1).dY - OffsetY;
					GetAt(7).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(5).dY );
					GetAt(3).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(2).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;

					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;

					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

			// TODO_

			if ( CapturePoint == 1 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;

					GetAt(1).dX		=	GetAt(0).dX + OffsetX;
					GetAt(5).dX		=	GetAt(7).dX + OffsetX;
					GetAt(2).dX		=	GetAt(3).dX + OffsetX;

					GetAt(1).dY		=	GetAt(0).dY + OffsetY;
					GetAt(5).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(7).dY );
					GetAt(2).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(3).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;

					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;

					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

			if ( CapturePoint == 2 || CapturePoint == 5 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;

					GetAt(1).dX		=	GetAt(0).dX + OffsetX;
					GetAt(5).dX		=	GetAt(7).dX + OffsetX;
					GetAt(2).dX		=	GetAt(3).dX + OffsetX;

					GetAt(1).dY		=	GetAt(0).dY + OffsetY;
					GetAt(5).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(7).dY );
					GetAt(2).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(3).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;

					GetAt(3).dY		=	GetAt(0).dY + OffsetY;
					GetAt(6).dY		=	GetAt(4).dY + OffsetY;
					GetAt(2).dY		=	GetAt(1).dY + OffsetY;

					GetAt(3).dX		=	GetAt(0).dX + OffsetX;
					GetAt(6).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(4).dX );
					GetAt(2).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(1).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

			if ( CapturePoint == 3 || CapturePoint == 6 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;

					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;

					GetAt(3).dY		=	GetAt(2).dY - OffsetY;
					GetAt(7).dY		=	GetAt(2).dY - OffsetY - ( GetAt(2).dY - GetAt(5).dY );
					GetAt(0).dY		=	GetAt(2).dY - OffsetY - ( GetAt(2).dY - GetAt(1).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;

					GetAt(3).dY		=	GetAt(0).dY + OffsetY;
					GetAt(6).dY		=	GetAt(4).dY + OffsetY;
					GetAt(2).dY		=	GetAt(1).dY + OffsetY;

					GetAt(3).dX		=	GetAt(0).dX + OffsetX;
					GetAt(6).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(4).dX );
					GetAt(2).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(1).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

			if ( CapturePoint == 4 || CapturePoint == 7 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;

					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;

					GetAt(0).dY		=	GetAt(1).dY - OffsetY;
					GetAt(7).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(5).dY );
					GetAt(3).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(2).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;

					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;

					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

		}
		//void OnLButtonDown( int nX, int nY )
		//{
		//	int tmpPos = PosOnMouse( MapToDataX( nX ), MapToDataY( nY ) );
		//	if( tmpPos != -1 && m_bEditingModeEnabled ) 
		//	{
		//		if( GetAsyncKeyState( VK_SHIFT ) < 0 )
		//		{
		//			m_CurrPos	= tmpPos;
		//			m_SelectEnd	= tmpPos;
		//		}
		//		else
		//		{
		//			m_CurrPos		= tmpPos;
		//			m_SelectStart	= tmpPos;
		//		}
		//		m_IsSelect		= true;
		//		m_IsTransfom	= false;

		//		// it's for repaint
		//		OnMouseMove( nX, nY );
		//		return;
		//	}

		//	int nTrackIndex = GetPointUnderCursor( nX, nY );

		//	if( nTrackIndex >= 0 && nTrackIndex <= 9 )
		//		m_IsTransfom = true;

		//	if( nTrackIndex == GetCount() - 1 )
		//		GetAt(nTrackIndex).Create( MapToDataX( nX ), MapToDataY( nY ) );

		//	if( -1 == GetPointUnderCursor( nX, nY ) ) EndEditText();

		//	if( m_AutoSize && nTrackIndex >=0 && nTrackIndex < 8 ) nTrackIndex = -1;

		//	SetCurrentPointIndex( nTrackIndex );

		//	SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );
		//}
		//void OnLButtonUp( int nX, int nY )
		//{
		//	m_IsFirstResize = false;
		//	if( GetAsyncKeyState( VK_SHIFT ) >= 0 ) 
		//	{
		//		if( m_IsSelect )
		//		{
		//			int tmpPos = PosOnMouse( MapToDataX( nX ), MapToDataY( nY ) );
		//			if( tmpPos != -1 && m_bEditingModeEnabled ) 
		//			{
		//				m_CurrPos		= tmpPos;
		//				m_SelectEnd 	= tmpPos;
		//			}
		//			m_IsSelect		= false;
		//		}
		//	}
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

		//	SetCurrentPointIndex(-1);

		//	m_IsTransfom = false;
		//}
		//void OnMouseMove( int nX, int nY )
		//{
		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	if( !m_IsTransfom )
		//	{
		//		int tmpPos = PosOnMouse( dX, dY );
		//		if( tmpPos != -1 && m_bEditingModeEnabled )
		//		{
		//			if( m_IsSelect && !(GetAsyncKeyState( VK_SHIFT ) < 0) ) 
		//			{
		//				m_CurrPos	= tmpPos;
		//				m_SelectEnd = tmpPos;
		//			}
		//			SendMessage( Constants::sc_nMessageTrackInvalidate, GetCurrentPointIndex() );
		//			return;
		//		}
		//	}

		//	if (!IsCurrentPointValid())
		//		return;

		//	int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

		//	if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		double dx, dy;
		//		double eX1, eX2, eY1, eY2;

		//		if (nCurrentPoint <= 8)
		//		{	
		//			eX1 = 1.0; eY1 = 0.0;
		//			eX2 = 0.0; eY2 = 1.0;

		//			double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
		//			Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
		//			Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );
		//		}

		//		if (nCurrentPoint == 0)
		//		{
		//			dX -= GetAt(0).dX;
		//			dY -= GetAt(0).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(0).dX += dX;
		//			GetAt(0).dY += dY;
		//			GetAt(1).dX += eX2*dy;
		//			GetAt(1).dY += eY2*dy;
		//			GetAt(3).dX += eX1*dx;
		//			GetAt(3).dY += eY1*dx;
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			dX -= GetAt(1).dX;
		//			dY -= GetAt(1).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(1).dX += dX;
		//			GetAt(1).dY += dY;
		//			GetAt(0).dX += eX2*dy;
		//			GetAt(0).dY += eY2*dy;
		//			GetAt(2).dX += eX1*dx;
		//			GetAt(2).dY += eY1*dx;

		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			dX -= GetAt(2).dX;
		//			dY -= GetAt(2).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(2).dX += dX;
		//			GetAt(2).dY += dY;
		//			GetAt(1).dX += eX1*dx;
		//			GetAt(1).dY += eY1*dx;
		//			GetAt(3).dX += eX2*dy;
		//			GetAt(3).dY += eY2*dy;
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			dX -= GetAt(3).dX;
		//			dY -= GetAt(3).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(3).dX += dX;
		//			GetAt(3).dY += dY;
		//			GetAt(0).dX += eX1*dx;
		//			GetAt(0).dY += eY1*dx;
		//			GetAt(2).dX += eX2*dy;
		//			GetAt(2).dY += eY2*dy;

		//		}
		//		else if (nCurrentPoint == 4)
		//		{
		//			dX -= GetAt(4).dX;
		//			dY -= GetAt(4).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(4).dX += dX;
		//			GetAt(4).dY += dY;
		//			GetAt(0).dX += eX2*dy;
		//			GetAt(0).dY += eY2*dy; 
		//			GetAt(1).dX += eX2*dy;
		//			GetAt(1).dY += eY2*dy;

		//		}
		//		else if (nCurrentPoint == 5)
		//		{
		//			dX -= GetAt(5).dX;
		//			dY -= GetAt(5).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(5).dX += dX;
		//			GetAt(5).dY += dY;
		//			GetAt(1).dX += eX1*dx;
		//			GetAt(1).dY += eY1*dx;
		//			GetAt(2).dX += eX1*dx;
		//			GetAt(2).dY += eY1*dx;					
		//		}
		//		else if (nCurrentPoint == 6)
		//		{
		//			dX -= GetAt(6).dX;
		//			dY -= GetAt(6).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(6).dX += dX;
		//			GetAt(6).dY += dY;
		//			GetAt(2).dX += eX2*dy;
		//			GetAt(2).dY += eY2*dy; 
		//			GetAt(3).dX += eX2*dy;
		//			GetAt(3).dY += eY2*dy;
		//		}
		//		else if (nCurrentPoint == 7)
		//		{
		//			dX -= GetAt(7).dX;
		//			dY -= GetAt(7).dY;

		//			dx = dX*eX1 + dY*eY1;
		//			dy = dX*eX2 + dY*eY2;

		//			GetAt(7).dX += dX;
		//			GetAt(7).dY += dY;
		//			GetAt(0).dX += eX1*dx;
		//			GetAt(0).dY += eY1*dx;
		//			GetAt(3).dX += eX1*dx;
		//			GetAt(3).dY += eY1*dx;	
		//		}				
		//		else if (nCurrentPoint == 8)
		//		{
		//			double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
		//			StickAngle( baseAngle, m_StickAngle );
		//			double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );

		//			RotateToAlpha( mAngle );
		//		}

		//		// recompute centers
		//		if (nCurrentPoint >= 0 && nCurrentPoint < 8)
		//		{
		//			GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
		//			GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
		//			GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
		//			GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

		//			double vX = GetAt(4).dX - GetAt(6).dX;
		//			double vY = GetAt(4).dY - GetAt(6).dY;

		//			double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
		//			if( length < 1 )
		//			{
		//				vX = -eX2; 
		//				vY = -eY2;
		//			}
		//			else
		//			{
		//				vX /= length;
		//				vY /= length;
		//			}

		//			GetAt(8).dX = GetAt(4).dX + vX * 30;
		//			GetAt(8).dY = GetAt(4).dY + vY * 30;
		//			GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );

		//			SplitString();				
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if ( nMessage == WM_KEYDOWN )
				CTrackObjectText::OnKeyDown( (UINT)wParam );
			else if ( nMessage == WM_KEYUP )
				CTrackObjectText::OnKeyUp( (UINT)wParam );
			else if ( nMessage == WM_DESTROY )
				m_arrStrings.RemoveAll();
			else if ( nMessage == WM_KILLFOCUS )
			{
				EndEditText();
				//m_arrStrings.RemoveAll();
			}
			else if ( nMessage == WM_MOUSELEAVE )	
				m_IsSelect	= false;
			else if ( nMessage == WM_LBUTTONDBLCLK )
			{
				if ( m_bBlockEditing )
					return FALSE;

				if( m_bEditingModeEnabled ) 
				{
					SelectByDblClick(); 
					OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
					return FALSE;
				}
				else
				{
					m_bEditingModeEnabled = true;
					SendMessage( Constants::sc_nMessageTrackTextBegin, GetCurrentPointIndex() );
					SplitString();
					m_SelectStart = m_SelectEnd = m_CurrPos;
					OnLButtonDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
				}
			}

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ( nTrackIndex == 9 || (nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 && !m_AutoSize )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else if (nTrackIndex == 8)
			{
				nPointStatus = c_nTrackPointStatusRotation;
				nChangeStatus = c_nTrackChangeStatusRotation;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			if ( m_bBlockResize )
			{
				if ( nTrackIndex >= 0 && nTrackIndex < 8 )
				{
					nPointStatus = c_nTrackPointStatusNone;
					nChangeStatus = c_nTrackChangeStatusNone;
				}
			}

			if( PosOnMouse( (int)MapToDataX ( nX ), (int)MapToDataY( nY ) ) != -1 && m_bEditingModeEnabled && !m_IsTransfom )
				nPointStatus = c_nTrackPointStatusIBeam;
			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			BSTR string   = m_String.AllocSysString();
			BSTR fontName = m_FontName.AllocSysString();
			double Angle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
			POINT selectInfo;
			selectInfo.x = m_SelectStart;
			selectInfo.y = m_SelectEnd;
			GetZoom( this );
			if( m_hDC != oPainter.GetDC() )
				m_hDC = oPainter.GetDC();

			oPainter.DrawEditTextRotatableW ( this, Angle, m_dZoom, m_arrStrings, string, m_CurrPos, 
				selectInfo, fontName, m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText );

			if( m_AutoSize && m_IsFirstResize )
			{
				SplitString();
				m_IsFirstResize = false;
			}
			SysFreeString( string ); 			
			SysFreeString( fontName );
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if ( m_bBlockResize )
				return;

			if( !m_AutoSize ) oPainter.DrawPoints(this, GetCount() - 3, c_nTrackPointSizePaint);
		}

		void GetZoom( Tracking::CTrackPoints* pTrack )
		{
			if (!pTrack || pTrack->GetCount() < 1 ) return;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray( pPoints, 10 );

			Structures::RECTD rectD;

			double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;

			double tX, tY;
			double cosA = cos( -dAngle );
			double sinA = sin( -dAngle );

			rectD.left   = (pPoints + 0)->x;
			rectD.top    = (pPoints + 0)->y;
			rectD.right  = (pPoints + 2)->x;
			rectD.bottom = (pPoints + 2)->y;

			tX = (pPoints + 0)->x - (pPoints + 9)->x;
			tY = (pPoints + 0)->y - (pPoints + 9)->y;				
			rectD.left  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.top   = (pPoints + 9)->y + tX * sinA + tY * cosA;	

			tX = (pPoints + 2)->x - (pPoints + 9)->x;
			tY = (pPoints + 2)->y - (pPoints + 9)->y;
			rectD.right  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.bottom = (pPoints + 9)->y + tX * sinA + tY * cosA;

			if( rectD.right < rectD.left ) 
			{ 
				double tmpRight		= rectD.right; 
				rectD.right			= rectD.left;
				rectD.left			= tmpRight;
			}
			if( rectD.top > rectD.bottom ) 
			{ 
				double tmpBottom	= rectD.bottom; 
				rectD.bottom		= rectD.top;
				rectD.top			= tmpBottom;
			}		

			Structures::RECTD rectDTracking = GetUnrotatableRect( dAngle );

			if( rectDTracking.GetWidth() > 0 )
				m_dZoom = rectD.GetWidth() / rectDTracking.GetWidth();

			if( pPoints )
			{
				delete []pPoints;
				pPoints = NULL;
			}
		}

		inline void EnableBlockEditing ( bool Value )
		{
			m_bBlockEditing		=	Value;
		}

		inline void EnableBlockResize ( bool Value )
		{
			m_bBlockResize		=	Value;
		}

	public:
		void Create( double dLeft, double dTop, double dRight, double dBottom, double dAngle, double dStickAngle,
			WCHAR* string, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool bBlockEditing = false, bool bBlockResize = false )
		{
			m_String   		= CString( string );
			m_FontName 		= CString( fontName );
			m_CurrPos  		= m_String.GetLength();
			m_FontSize 		= fontSize;
			m_FontType		= fontType;
			m_FontColor		= FontColor;
			m_bEditingModeEnabled		= false;
			m_IsSelect		= false;
			m_IsTransfom	= true;
			m_SelectStart	= 0;
			m_SelectEnd		= 0;
			m_hDC			= NULL;
			m_dZoom			= 1.0;
			m_StickAngle	= ( dStickAngle < 0 || dStickAngle > 360 ) ? 0.0 : rad( dStickAngle );
			m_bBlockEditing	=	bBlockEditing;
			m_bBlockResize	=	bBlockResize;

			if( dRight < 0 && dBottom < 0 ) 
				m_AutoSize = true;
			else					
				m_AutoSize = false;

			//m_ScaleText = (bool)GetFontScale( m_FontType );
			if ( GetFontScale( m_FontType ) != 0 )
				m_ScaleText = true;
			else
				m_ScaleText = false;

			m_Scale.x = 1.0;
			m_Scale.y = 1.0;

			switch( GetFontAlignedH( m_FontType ) )
			{
			case ALIGNED_TOP:
				m_FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				m_FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				m_FontAlignW = byBottom;
				break;
			default:
				m_FontAlignW = byTop;
			}

			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 || m_AutoSize )	m_IsFirstResize = true;
			else																		m_IsFirstResize = false;

			SetType( Constants::sc_nTrackTypeTextRotatable );

			Clear();

			if( m_AutoSize )
			{
				dRight = dLeft + fontSize;
				dBottom = dTop + fontSize;
			}

			Add( dLeft,  dTop );																//	0
			Add( dRight, dTop );																//	1
			Add( dRight, dBottom );																//	2
			Add( dLeft,  dBottom );																//	3

			Add( 0.5 * ( dLeft + dRight ), dTop );												//	4
			Add( dRight, 0.5 * ( dTop + dBottom ) );											//	5
			Add( 0.5 * ( dLeft + dRight ), dBottom );											//	6
			Add( dLeft, 0.5 * ( dTop + dBottom ) );												//	7

			Add( 0.5 * ( dLeft + dRight ), dTop - 30 );					//	rotate selector		//	8
			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );	//	center				//	9

			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );							//	10

			SetCurrentPointIndex(-1);

			RotateToAlpha( rad( dAngle ) );
		}

		inline double GetStickAngle () { return m_StickAngle; }
	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if( !m_IsTransfom )
			{
				int tmpPos = PosOnMouse ( (int)dX, (int)dY );
				if( tmpPos != -1 && m_bEditingModeEnabled )
				{
					if( m_IsSelect && !(GetAsyncKeyState( VK_SHIFT ) < 0) ) 
					{
						m_CurrPos	= tmpPos;
						m_SelectEnd = tmpPos;
					}
					if ( false == IsTrackChild () )
						SendMessage( Constants::sc_nMessageTrackInvalidate, GetCurrentPointIndex() );

					return false;
				}
			}

			if (!IsCurrentPointValid())
				return false;

			int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				double dx, dy;
				double eX1, eX2, eY1, eY2;

				double CharSize		=	(double)m_FontSize * (double)GetDeviceCaps( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
				double CharSizeRotX	=	CharSize;
				double CharSizeRotY	=	CharSize;

				if (nCurrentPoint <= 8)
				{	
					eX1 = 1.0; eY1 = 0.0;
					eX2 = 0.0; eY2 = 1.0;

					double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
					Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
					Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );

					Geometry::RotatePoint ( CharSizeRotX, CharSizeRotY, 0.0, 0.0, mAngle );
				}

				if (nCurrentPoint == 0)
				{
					dX -= GetAt(0).dX;
					dY -= GetAt(0).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(0).dX += dX;
					GetAt(0).dY += dY;
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;
				}
				else if (nCurrentPoint == 1)
				{
					dX -= GetAt(1).dX;
					dY -= GetAt(1).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(1).dX += dX;
					GetAt(1).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;

				}
				else if (nCurrentPoint == 2)
				{
					dX -= GetAt(2).dX;
					dY -= GetAt(2).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(2).dX += dX;
					GetAt(2).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 3)
				{
					dX -= GetAt(3).dX;
					dY -= GetAt(3).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(3).dX += dX;
					GetAt(3).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy;

				}
				else if (nCurrentPoint == 4)
				{
					dX -= GetAt(4).dX;
					dY -= GetAt(4).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(4).dX += dX;
					GetAt(4).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy; 
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;

				}
				else if (nCurrentPoint == 5)
				{
					dX -= GetAt(5).dX;
					dY -= GetAt(5).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(5).dX += dX;
					GetAt(5).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;					
				}
				else if (nCurrentPoint == 6)
				{
					dX -= GetAt(6).dX;
					dY -= GetAt(6).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(6).dX += dX;
					GetAt(6).dY += dY;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy; 
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 7)
				{
					dX -= GetAt(7).dX;
					dY -= GetAt(7).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(7).dX += dX;
					GetAt(7).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;	
				}				
				else if (nCurrentPoint == 8)
				{
					double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
					StickAngle( baseAngle, m_StickAngle );
					double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );

					RotateToAlpha( mAngle );
				}


				UpdateMinimizeRegion ( nCurrentPoint );

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 8)
				{
					GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
					GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
					GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
					GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

					double vX = GetAt(4).dX - GetAt(6).dX;
					double vY = GetAt(4).dY - GetAt(6).dY;

					double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
					if( length < 1 )
					{
						vX = -eX2; 
						vY = -eY2;
					}
					else
					{
						vX /= length;
						vY /= length;
					}

					GetAt(8).dX = GetAt(4).dX + vX * 30;
					GetAt(8).dY = GetAt(4).dY + vY * 30;
					GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );

					SplitString();				
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int tmpPos = PosOnMouse( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
			if( tmpPos != -1 && m_bEditingModeEnabled ) 
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 )
				{
					m_CurrPos	= tmpPos;
					m_SelectEnd	= tmpPos;
				}
				else
				{
					m_CurrPos		= tmpPos;
					m_SelectStart	= tmpPos;
				}
				m_IsSelect		= true;
				m_IsTransfom	= false;

				// it's for repaint
				OnMouseMove( MouseX, MouseY );
				return true;
			}

			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;

			if( nTrackIndex >= 0 && nTrackIndex <= 9 )
				m_IsTransfom = true;

			if( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			if( -1 == GetPointUnderCursor ( MouseX, MouseY ) )
				EndEditText();

			if ( m_bBlockResize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			if( m_AutoSize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			SetCurrentPointIndex( nTrackIndex );

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			m_IsFirstResize = false;

			if( GetAsyncKeyState( VK_SHIFT ) >= 0 ) 
			{
				if( m_IsSelect )
				{
					int tmpPos = PosOnMouse( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
					if( tmpPos != -1 && m_bEditingModeEnabled ) 
					{
						m_CurrPos		= tmpPos;
						m_SelectEnd 	= tmpPos;
					}
					m_IsSelect		= false;
				}
			}
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			m_IsTransfom = false;

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	private:
		HDC					m_hDC;
		double				m_dZoom;
		bool				m_IsFirstResize;
		bool				m_AutoSize;
		bool				m_ScaleText;

		Structures::POINTD	m_Scale;
		bool				m_bBlockEditing;
		bool				m_bBlockResize;

	protected:
		double				m_StickAngle;
	};

	*/
	//------------------------------------------------------------------------------------------------------------------
	// Text In None Rotate Rectangle
	//------------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------------
	// Text In None Rotate Rectangle
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectText : public CTrackObject
	{
	public:
		CTrackObjectText ()
		{
			m_chDeadKey			=	L'\0';
			m_bEnableDrawText	=	true;
		}

		virtual ~CTrackObjectText()
		{
			m_arrStrings.RemoveAll ();
		}

	public:

		inline void				SetString	( CString string )		{ m_String	 = string;	 }
		inline void				SetFontName	( CString fontName )	{ m_FontName = fontName; }
		inline void				SetCursorPos( int currPos )			{ m_CurrPos  = currPos;	 }
		inline void				SetFontSize ( int fontSize )		{ m_FontSize = fontSize; }
		inline void				SetFontType ( int fontType )		{ m_FontType = fontType; }
		inline void				SetEnableDrawText ( bool Value )	{ m_bEnableDrawText	=	Value; }
		
		inline const CStringW&	GetString		()		const		{ return m_String; }
		inline const CStringW&	GetFontName		()		const		{ return m_FontName; }
		inline const int&		GetCurrPos		()		const		{ return m_CurrPos; }
		inline const int&		GetFontSize		()		const		{ return m_FontSize; }
		inline const int&		GetFontType		()		const		{ return m_FontType; }
		inline const int&		GetFontColor	()		const		{ return m_FontColor; }
		inline const bool&		GetBlockEditing ()		const		{ return m_bBlockEditing; }
		inline const bool		GetEnableDrawText ()	const		{ return m_bEnableDrawText; }
		inline bool				IsEditingModeEnabled ()	const		{ return m_bEditingModeEnabled; }
		inline CIMEUIController&	GetIMEUI ()						{ return m_oIMEUIController;	}
	
	protected:

		virtual int GetPointUnderCursor ( int nX, int nY )
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0 || nTrackIndex > 7)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect(0, 7);
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
				else
					return -1;
			}

			return nTrackIndex;
		}
		virtual int GetStringByPos ( int& CaretPos )
		{
			int arrSize = m_arrStrings.GetSize();
			for ( int index = 0; index < arrSize; ++index )
			{
				if ( CaretPos < m_arrStrings[index].GetLength() || ( index == arrSize - 1 ) )
					return index;

				CaretPos -= m_arrStrings[index].GetLength();
			}

			return -1;
		}

		virtual int PosOnMouse( int dX, int dY )
		{
			if( NULL == m_hDC ) 
				return -1;

			int countLines = m_arrStrings.GetSize();
			
			if( 0 == countLines )
				return -1;

			Gdiplus::Graphics graphics( m_hDC );

			//BSTR fontName = m_FontName.AllocSysString();
			
			int fontStyle = Gdiplus::FontStyleRegular;
			
			if(	m_FontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
			if( m_FontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( m_FontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( m_FontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;
			Gdiplus::RectF	orig, currStrRect;
			
			//Gdiplus::FontFamily	fontFamily ( m_FontName );
			//Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );
			
			Gdiplus::Font*			pFont		=	NULL;
			Gdiplus::FontFamily*	pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily, fontStyle ) )
				return -1;
		
			Gdiplus::StringFormat	stringFormat( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
			
			//SysFreeString ( fontName );

			if( !m_ScaleText )
			{
				switch( GetFontAlignedW ( m_FontType ) )
				{
				case ALIGNED_LEFT:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				case ALIGNED_CENTER:
					stringFormat.SetAlignment( StringAlignmentCenter ); 
					break;
				case ALIGNED_RIGHT:
					stringFormat.SetAlignment( StringAlignmentFar ); 
					break;
				default:
					stringFormat.SetAlignment( StringAlignmentNear ); 
					break;
				}
			}

			Structures::RECTD rectD( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectD.Normalize();

			if ( dY > rectD.bottom )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );

				return -1;
			}

			double dWidth	= rectD.GetWidth();
			double dWidth_2 = dWidth / 2.0;

			Structures::POINTI pointTrack( dX, dY );

			int tmpStrLen		=	0;
			double fontHeight	=	Rendering::Utils::GetFontHeight ( pFont );

			BSTR line = m_arrStrings[0].AllocSysString();
			graphics.MeasureString ( line, -1, pFont, orig, &stringFormat, &currStrRect );
			currStrRect.Height = (Gdiplus::REAL)fontHeight;
			SysFreeString( line );

			double displaceByY = countLines * currStrRect.Height;

			for( int i = 0; i < countLines; ++i )
			{
				BSTR line = m_arrStrings[i].AllocSysString();
				graphics.MeasureString ( line, -1, pFont, orig, &stringFormat, &currStrRect );	
				SysFreeString( line );
				currStrRect.Height = (Gdiplus::REAL)fontHeight;

				currStrRect.X		= (Gdiplus::REAL)rectD.left;
				currStrRect.Width	= (Gdiplus::REAL)dWidth;

				if( !m_ScaleText )
				{
					switch( m_FontAlignW )
					{
					case byTop:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
							break;
						}
					case byMiddle:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.GetCenter().y - (Gdiplus::REAL)displaceByY / 2.0f );
							break;	
						}
					case byBottom:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.bottom - (Gdiplus::REAL)displaceByY );
							break;	
						}
					default:
						{
							currStrRect.Offset ( 0.0f, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
						}
					}
				}
				else
				{
					currStrRect.Height = (Gdiplus::REAL)fontHeight * (Gdiplus::REAL)m_Scale.y * (Gdiplus::REAL)countLines;
					currStrRect.Offset( 0.0, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
				}

				if ( currStrRect.Contains ( (Gdiplus::REAL)dX, (Gdiplus::REAL)dY ) )
				{
					BSTR line = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( line, -1, pFont, orig, &stringFormat, &currStrRect );
					SysFreeString( line );
					currStrRect.Height = (Gdiplus::REAL)fontHeight;
					currStrRect.Width  *= (Gdiplus::REAL)m_Scale.x;
					currStrRect.Height *= (Gdiplus::REAL)m_Scale.y;

					if( stringFormat.GetAlignment() == StringAlignmentNear )
						currStrRect.Offset( (Gdiplus::REAL)rectD.left, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					else
						if( stringFormat.GetAlignment() == StringAlignmentCenter )
							currStrRect.Offset( (Gdiplus::REAL)rectD.left + (Gdiplus::REAL)dWidth_2, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
						else 
							if( stringFormat.GetAlignment() == StringAlignmentFar )
								currStrRect.Offset ( (Gdiplus::REAL)rectD.right, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );

					double partOfChar = (double)pFont->GetSize() * m_Scale.x / 5.0;

					StringFormat localStringFormat;
					localStringFormat.SetAlignment(StringAlignmentNear);
					BSTR lineStringAll = m_arrStrings[i].AllocSysString();
					graphics.MeasureString( lineStringAll, -1, pFont, orig, &localStringFormat, &currStrRect );
					SysFreeString( lineStringAll );

					CStringW lineString = m_arrStrings[i];
					lineString.Replace( L"\r\n", L"" );

					if( stringFormat.GetAlignment() == StringAlignmentNear )
					{
						for( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, pFont, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if ( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.left )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								return tmpStrLen + j;
							}
							
							if ( j == lineString.GetLength() - 1 )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								return tmpStrLen + j + 1;
							}
						}	
					}
					else if ( stringFormat.GetAlignment() == StringAlignmentCenter )
					{
						for ( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, pFont, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= ( dX - (rectD.left + rectD.GetWidth() / 2 ) ) + currStrRect.Width / 2 )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								
								return tmpStrLen + j;
							}
							else if( dX < currStrRect.X ) 
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								
								return tmpStrLen;
							}
							if ( j == lineString.GetLength() - 1 )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								
								return tmpStrLen + j + 1;
							}
						}	
					}
					else if( stringFormat.GetAlignment() == StringAlignmentFar )
					{				
						for( int j = 0; j < lineString.GetLength(); ++j )
						{
							RectF currCharRect;
							BSTR line = m_arrStrings[i].AllocSysString();
							Painter::GetRectCurrentCharacter( &graphics, &localStringFormat, pFont, line, -1, j, currStrRect, currCharRect );
							SysFreeString( line );

							if ( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.right + currStrRect.Width )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );

								return tmpStrLen + j;
							}
							
							if ( j == lineString.GetLength() - 1 )
							{
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );

								return tmpStrLen + j + 1;
							}
						}
					}
					
					RELEASEOBJECT ( pFont );
					RELEASEOBJECT ( pFontFamily );

					return tmpStrLen + m_arrStrings[i].GetLength();// -2 for skeep "\r\n"
				}
				tmpStrLen += m_arrStrings[i].GetLength();
			}
			
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

			return -1;				
		}
		void SelectByDblClick()
		{
			m_SelectStart = m_SelectEnd = 0;
			int tmpBegin = 0;
			while( m_SelectStart <= m_CurrPos )
			{
				tmpBegin = m_String.Find( ' ', m_SelectStart );
				if( -1 == tmpBegin )
				{
					m_SelectEnd = m_String.GetLength();
					return;
				}
				else if( tmpBegin >= m_CurrPos )
				{
					m_SelectEnd = tmpBegin;
					return;
				}
				else if( tmpBegin > m_String.GetLength() - 1 )
				{
					m_SelectEnd = tmpBegin;
					return;
				}
				else
					m_SelectStart = tmpBegin + 1;
			}
		}
		virtual void SplitString ()
		{
			m_arrStrings.RemoveAll();

			if ( NULL == m_hDC )
				return;
			
			if ( 0 == m_String.GetLength() && m_AutoSize )
			{
				GetAt(1).Create( GetAt(0).dX + m_FontSize, GetAt(0).dY );
				GetAt(2).Create( GetAt(0).dX + m_FontSize, GetAt(0).dY + m_FontSize );
				GetAt(3).Create( GetAt(0).dX, GetAt(0).dY + m_FontSize );

				GetAt(4).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(0).dY );
				GetAt(5).Create( GetAt(1).dX, 0.5 * ( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(2).dY );
				GetAt(7).Create( GetAt(0).dX, 0.5 * ( GetAt(0).dY + GetAt(3).dY ) );

				return;
			}
			else 
			if ( 0 == m_String.GetLength() )
				return;

			Graphics graphics ( m_hDC );
			
			if ( Gdiplus::Ok != graphics.GetLastStatus() )
				return;

			Structures::RECTD rectD( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectD.Normalize();

			double dWidth  = rectD.GetWidth();

			RectF orig, rectF, strBB;

			//BSTR fontName = m_FontName.AllocSysString();
			int fontStyle = Gdiplus::FontStyleRegular;
			if(	m_FontType & FONTSTYLE_BOLD )		fontStyle |= Gdiplus::FontStyleBold;
			if( m_FontType & FONTSTYLE_ITALIC )		fontStyle |= Gdiplus::FontStyleItalic;
			if( m_FontType & FONTSTYLE_STRIKEOUT )	fontStyle |= Gdiplus::FontStyleStrikeout;
			if( m_FontType & FONTSTYLE_UNDERLINE )	fontStyle |= Gdiplus::FontStyleUnderline;

			//Gdiplus::FontFamily		fontFamily( m_FontName );
			//Gdiplus::Font			font( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );
			
			Gdiplus::Font*			pFont		=	NULL;
			Gdiplus::FontFamily*	pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily, fontStyle ) )
				return;

			Gdiplus::StringFormat	stringFormat( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags( StringFormatFlagsMeasureTrailingSpaces );
			
			//SysFreeString( fontName );

			if ( dWidth < pFont->GetSize() && !m_ScaleText && !m_AutoSize )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );

				return;
			}

			double maxWidth		= 0.0;
			double maxHeight	= 0.0;

			int strLen = m_String.GetLength();

			int		strLine		= 0;
			int		countChar	= 1;
			double  displaceX	= 0.0;
			double  displaceY	= 0.0;
			bool	isEnd		= false;

			BSTR fullString = m_String.AllocSysString();

			int StringsCount	=	1;

			for ( int i = 0; i < strLen; ++i )
			{
				if ( * ( fullString + strLine + countChar - 1 ) == L'\r' )
				{
					CStringW tmpStr		=	CStringW	( fullString + strLine );
					i++;
					tmpStr.Delete ( countChar + 1, strLen - i + 2 );
					m_arrStrings.Add ( tmpStr );
					
					strLine				=	i + 1;
					countChar			=	1;
					isEnd				=	true;

					++StringsCount;

					continue;
				}
				graphics.MeasureString( fullString + strLine, countChar, pFont, orig, &stringFormat, &strBB );
				displaceX = strBB.Width;
				maxWidth = max( maxWidth, displaceX );
				if( displaceX > dWidth && !m_AutoSize && !m_ScaleText )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );					
					tmpStr.Delete( countChar - 1, strLen - i );
					BSTR subString = tmpStr.AllocSysString();
					BSTR rSpace = wcsrchr( subString, ' ' );
					if( NULL != rSpace )
					{						
						int strLenSpace = (int)wcslen( rSpace );
						i -= strLenSpace - 1;
						tmpStr.Delete( countChar - strLenSpace, strLenSpace);
					}
					m_arrStrings.Add( tmpStr );
					strLine		= i;
					countChar	= 1;
					isEnd = true;
					SysFreeString( subString );
				}	
				countChar++;
				isEnd = false;
			}
			if( !isEnd )
			{
				m_arrStrings.Add ( CStringW ( fullString + strLine ) + L"\r\n" );
			}
			m_String.FreeExtra();

			double fontHeight		=	Rendering::Utils::GetFontHeight ( pFont );
			
			if ( m_ScaleText )
			{
				double dHeight		=	rectD.GetHeight();

				maxHeight			=	fontHeight * StringsCount; // m_arrStrings.GetSize();

				if ( m_arrStrings.GetSize() > 0 && maxHeight > 0 )
				{
					m_Scale.x		=	dWidth / max ( 0.001, maxWidth );
					m_Scale.y		=	dHeight / maxHeight / m_arrStrings.GetSize ( );
				}
			}
			else if ( m_AutoSize )
			{
				double CharSize		=	(double)m_FontSize * (double)GetDeviceCaps ( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
				maxWidth			=	__max ( maxWidth, CharSize);

				maxHeight			=	fontHeight * StringsCount;	// m_arrStrings.GetSize();

				GetAt(2).dX			=	GetAt(0).dX + maxWidth;
				GetAt(2).dY			=	GetAt(0).dY + maxHeight;
				GetAt(1).dX			=	GetAt(0).dX + maxWidth;
				GetAt(3).dY			=	GetAt(0).dY + maxHeight;

				GetAt(4).Create ( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(0).dY );
				GetAt(5).Create ( GetAt(1).dX, 0.5 * ( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create ( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), GetAt(2).dY );
				GetAt(7).Create ( GetAt(0).dX, 0.5 * ( GetAt(0).dY + GetAt(3).dY ) );
			}
			
			SysFreeString( fullString );
			
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );
		}

		virtual void EndEditText()
		{
			if( m_bEditingModeEnabled )
			{
				m_bEditingModeEnabled = false;
				SendMessage( Constants::sc_nMessageTrackTextEnd, GetCurrentPointIndex() );
			}
		}
		void CopyToClipboard( bool isCut = false )
		{
			LPWSTR 	pstrCopy;
			HGLOBAL	glblCopy;

			if( m_SelectStart == m_SelectEnd ) return;

			CStringW strSelected = m_String;
			int beginSelect = min( m_SelectStart, m_SelectEnd );
			int endSelect	= max( m_SelectStart, m_SelectEnd );

			strSelected = m_String;
			strSelected.Delete( endSelect, strSelected.GetLength() - endSelect );
			strSelected.Delete( 0, beginSelect );

			if( isCut )
			{
				m_String.Delete( beginSelect, endSelect - beginSelect );
				SplitString();
				m_CurrPos = beginSelect; 
				m_SelectEnd = m_SelectStart = m_CurrPos;
				SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
			}

			int strLength = strSelected.GetLength();

			glblCopy = GlobalAlloc( GMEM_MOVEABLE, strLength * sizeof(WCHAR) + 2 );
			pstrCopy = (LPWSTR)GlobalLock( glblCopy );
			BSTR	srcStringW = strSelected.AllocSysString();
			wcscpy( pstrCopy, srcStringW );
			SysFreeString( srcStringW );
			GlobalUnlock( glblCopy );
			OpenClipboard( NULL );
			EmptyClipboard();
			SetClipboardData( CF_UNICODETEXT, glblCopy );
			CloseClipboard();
		}

		void PastToClipboard()
		{
			LPWSTR	pstrPast;
			HGLOBAL glblPast;

			if( !IsClipboardFormatAvailable( CF_UNICODETEXT ) )
				return;
			if( !OpenClipboard( NULL ) ) 
				return;

			if( glblPast = GetClipboardData( CF_UNICODETEXT ) )
			{
				if( pstrPast = (LPWSTR)GlobalLock( glblPast ) )
				{
					int beginSelect = min( m_SelectStart, m_SelectEnd );
					int endSelect	= max( m_SelectStart, m_SelectEnd );

					if( beginSelect != endSelect )
					{
						m_String.Delete( beginSelect, endSelect - beginSelect );
						m_CurrPos = beginSelect;
						m_SelectEnd = m_SelectStart = m_CurrPos;
					}

					CStringW strPast = CStringW ( pstrPast );
					m_String.Insert ( m_CurrPos, strPast );
					m_CurrPos = m_SelectEnd + strPast.GetLength();
					m_SelectEnd = m_SelectStart = m_CurrPos;
					GlobalUnlock( glblPast );
					SplitString();
					SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
				}
			}
			CloseClipboard();
		}


		virtual void OnKeyDown( unsigned int Key )
		{
			if( !m_bEditingModeEnabled ) return;
			bool isShift	= false;
			bool isCtrl		= false;
			
			wchar_t wKey[3] = { L'\0', L'\0', L'\0' };

			if(	Key == VK_ESCAPE )
			{
				EndEditText();
				m_CurrPos		= m_SelectEnd;
				m_SelectStart	= m_SelectEnd;
			}
			else if( Key == VK_CONTROL )
			{
				isCtrl = true;
			}
			else if( Key == VK_LEFT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos > 0 ) m_CurrPos--;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;
			}
			else if( Key == VK_RIGHT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos < m_String.GetLength() ) m_CurrPos++;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos++;
			}
			else if( Key == VK_UP )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( currIdxString < 1 ) return;

				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();

				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if( m_arrStrings[currIdxString - 1].GetLength() - 2 < posInLine ) // - 2 for "\r\n"
						m_CurrPos -= posInLine + 2; // + 2 for "\r\n"
					else
						m_CurrPos -= m_arrStrings[currIdxString - 1].GetLength();	
				}
				else
				{
					if( m_arrStrings[currIdxString - 1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine ) // - 2 for "\r\n"	
						m_CurrPos -= posInLine + m_arrStrings[currIdxString - 1].GetLength();
					else
						m_CurrPos -= m_arrStrings[currIdxString].GetLength();
				}
			}
			else if( Key == VK_DOWN )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( -1 == currIdxString || currIdxString == m_arrStrings.GetSize() - 1 ) return;
				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();
				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if( m_arrStrings[currIdxString + 1].GetLength() - 2 < posInLine ) 
						m_CurrPos += sumLineLenght - m_CurrPos + m_arrStrings[currIdxString + 1].GetLength() - 2;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength();
				}
				else
				{
					if( m_arrStrings[currIdxString+1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine )
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine + (m_arrStrings[currIdxString+1].GetLength() - m_arrStrings[currIdxString].GetLength() + posInLine );
				}
			}
			else if( Key == VK_SHIFT )
			{
				//if( !m_IsSelect ) m_SelectStart = m_SelectEnd = m_CurrPos;
				isShift = true;
			}
			else if( Key == VK_DELETE )
			{
				if( m_CurrPos == m_String.GetLength() && m_SelectStart == m_SelectEnd ) return;

				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				if( m_SelectStart != m_SelectEnd )
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos + 2 <= m_String.GetLength() )
						if( m_String[ m_CurrPos + 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos, 2 );
							deleteRN = true;
						}
						if( !deleteRN )
							m_String.Delete( m_CurrPos );
				}

				SplitString();
			}
			else if( Key == VK_BACK )
			{
				if( m_CurrPos == 0 && m_SelectStart == m_SelectEnd ) return;

				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos > 0 )
						if( m_String[ m_CurrPos - 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos - 2, 2 );
							m_CurrPos -= 2;
							deleteRN = true;
						}
						if( !deleteRN )
						{
							m_String.Delete( m_CurrPos - 1 );
							m_CurrPos--;
						}
				}

				SplitString();

			}
			else if( Key == VK_SPACE )
			{
				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += _T(" ");
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, L" " );
					m_CurrPos++;
				}

				SplitString();

			}
			else if ( Key == VK_RETURN )
			{
				if ( m_CurrPos == m_String.GetLength() )
				{
					m_String += L"\r\n";
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, L"\r\n" );
					m_CurrPos += 2;
				}

				SplitString();

			}
			//else if( Key == VK_TAB )
			//{
			//}
			else if( Key == VK_HOME )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				m_CurrPos = 0;
			}
			else if( Key == VK_END )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				m_CurrPos = m_String.GetLength();
			}
			else 
			if (( Key >= 'A'		&& Key <= 'Z'		) || 
				( Key >= '0'		&& Key <= '9'		) || 
				( Key >= VK_OEM_1	&& Key <= VK_OEM_3	) || 
				( Key >= VK_OEM_4	&& Key <= VK_OEM_8	) ||
				( Key >= VK_NUMPAD0 && Key <= VK_DIVIDE ) )
			{
				//if( GetAsyncKeyState( VK_CONTROL ) < 0 )
				if( GetAsyncKeyState( VK_CONTROL ) < 0 && ((GetKeyState(VK_MENU) < 0 ? 0 : 1) ) )
				{
					if( Key == 'X' )
						CopyToClipboard( true );
					else if( Key == 'C' )
						CopyToClipboard();
					else if( Key == 'V' )
						PastToClipboard();
					return;
				}
				
				if ( m_SelectStart != m_SelectEnd )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
					m_IsSelect = false;
				}
				
				if ( m_CurrPos > 0 )
				{
					if ( '\r' == m_String [ m_CurrPos - 1 ] )
					{
						--m_CurrPos;
					}
				}

				BYTE kbdState[256];
				ZeroMemory( kbdState, 256 );
				
				GetKeyboardState ( kbdState );
				
				wchar_t KeyboardState	=	wKey [ 0 ];
				
    			if ( 1 == ToUnicode ( Key, 0, kbdState, wKey, 3, 0 ) )
				{
					////if ( m_chDeadKey != L'\0' ) 
					////{
					////	wchar_t wcs_in[3];
					////	wchar_t wcs_out[3];

					////	wcs_in[0] = wKey[0];
					////	wcs_in[1] = m_chDeadKey;
					////	wcs_in[2] = L'\0';

					////	/* from accent char to unicode */
					////	if ( FoldStringW ( MAP_PRECOMPOSED, wcs_in, 3, wcs_out, 3 ) )
					////	{
					////		wKey[0] = wcs_out[0];
					////		wKey[1] = wcs_out[1];
					////		wKey[2] = wcs_out[2];
					////	}
					////}
					////else
					////{

					////}

					////m_chDeadKey = L'\0';
				}
				else
				{
					////m_chDeadKey			=	L'\0';

					////switch(wKey[0]) 
					////{
					////case 0x5e:          /* circumflex */
					////	m_chDeadKey = 0x302;  break;
					////case 0x60:          /* grave accent */
					////	m_chDeadKey = 0x300;  break;
					////case 0xa8:          /* diaeresis */
					////	m_chDeadKey = 0x308;  break;
					////case 0xb4:          /* acute accent */
					////	m_chDeadKey = 0x301;  break;
					////case 0xb8:          /* cedilla */
					////	m_chDeadKey = 0x327;  break;
					////default:
					////	m_chDeadKey	=	wKey[0];
					////	break;
					////}

					return;
				}					

				wKey [ 1 ]	=	L'\0';

				if ( m_CurrPos == m_String.GetLength() )
				{
					m_String	+=	CStringW ( wKey );
					m_CurrPos	=	m_String.GetLength ();
				}
				else
				{
					m_String.Insert ( m_CurrPos, CStringW ( wKey ) );
					++m_CurrPos;
				}

				SplitString();

			}
			
			if( !isShift && !isCtrl )
			{
				if( m_IsSelect )	m_SelectEnd = m_CurrPos;
				else				m_SelectStart = m_SelectEnd = m_CurrPos;
			}

			SendMessage(Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex());
		}
		virtual void OnKeyUp( unsigned int Key )
		{
			if( Key == VK_SHIFT )
			{
				m_IsSelect	= false;
			}
		}


		virtual void OnDeadChar ( UINT CharCode, UINT KeyData )
		{
			return;//

			wchar_t wKey[3] = { L'\0', L'\0', L'\0' };
 
			HKL dwhkl = GetKeyboardLayout((DWORD) 0);

			BYTE kbdState[256];
			ZeroMemory( kbdState, 256 );

			GetKeyboardState ( kbdState );

			wchar_t KeyboardState	=	wKey [ 0 ];

			if ( 1 != ToUnicodeEx ( CharCode, KeyData, kbdState, wKey, /* 3 */ sizeof(wKey)/sizeof(wchar_t), 0, dwhkl))
			{
				m_chDeadKey			=	L'\0';

				switch(wKey[0]) 
				{

				case 0x5e:          /* circumflex */
					m_chDeadKey = 0x302;  break;
				case 0x60:          /* grave accent */
					m_chDeadKey = 0x300;  break;
				case 0xa8:          /* diaeresis */
					m_chDeadKey = 0x308;  break;
				case 0xb4:          /* acute accent */
					m_chDeadKey = 0x301;  break;
				case 0xb8:          /* cedilla */
					m_chDeadKey = 0x327;  break;
				default:
					m_chDeadKey = wKey[0];
					break;
				}

				return;
			}

			m_chDeadKey				=	L'\0';
		}

		
		inline bool	GetFont ( Gdiplus::Font*& pFont, Gdiplus::FontFamily*& pFontFamily )
		{
			pFontFamily	=	new Gdiplus::FontFamily ( m_FontName );
			pFont		=	new Gdiplus::Font ( pFontFamily, (Gdiplus::REAL)m_FontSize, m_FontType, Gdiplus::UnitPixel );

			if ( NULL == pFont && NULL == pFontFamily )
				return false;

			if ( Gdiplus::Ok != pFont->GetLastStatus () )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );
			
				pFont	=	new Gdiplus::Font ( m_FontName, (Gdiplus::REAL)m_FontSize, m_FontType, Gdiplus::UnitPixel );
			
				if ( NULL == pFont )
					return false;
			}

			return true;
		}

		inline bool	GetFont ( Gdiplus::Font*& pFont, Gdiplus::FontFamily*& pFontFamily, int Fontstyle )
		{
			pFontFamily	=	new Gdiplus::FontFamily ( m_FontName );
			pFont		=	new Gdiplus::Font ( pFontFamily, (Gdiplus::REAL)m_FontSize, Fontstyle, Gdiplus::UnitPixel );

			if ( NULL == pFont && NULL == pFontFamily )
				return false;

			if ( Gdiplus::Ok != pFont->GetLastStatus () )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );
			
				pFont	=	new Gdiplus::Font ( m_FontName, (Gdiplus::REAL)m_FontSize, Fontstyle, Gdiplus::UnitPixel );
			
				if ( NULL == pFont )
					return false;
			}

			return true;
		}

	public:

		virtual BOOL OnMessages ( UINT nMessage, WPARAM wParam, LPARAM lParam )
		{
			if ( nMessage == WM_DEADCHAR )
			{
				OnDeadChar ( (UINT)wParam, (UINT)lParam );
			}
			else if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if ( nMessage == WM_KEYDOWN )
				OnKeyDown( (UINT)wParam );
			else if ( nMessage == WM_KEYUP )
				OnKeyUp ( (UINT)wParam );
			else if ( nMessage == WM_DESTROY )
				m_arrStrings.RemoveAll();
			else if ( nMessage == WM_KILLFOCUS )
				EndEditText();
			else if ( nMessage == WM_MOUSELEAVE )	
				m_IsSelect	= false;
			else if ( nMessage == WM_LBUTTONDBLCLK )
			{
				//ATLTRACE ("DOUBLE CLICK \n");

				if ( m_bBlockEditing )
					return FALSE;

				// for EnabkeUseOnlyRgnDblClk 

				CTrackPoint MinPoint	=	GetAt ( 0 );
				CTrackPoint MaxPoint	=	GetAt ( 0 );
				
				double iMouseX			=	MapToDataX ( (int)GET_X_LPARAM ( lParam ) );
				double iMouseY			=	MapToDataY ( (int)GET_Y_LPARAM ( lParam ) );
				
				//GetCursorPos ( 
				//ATLTRACE ( "DOUBLE CLICK: %d, %d \n", (int)GET_X_LPARAM ( lParam ), (int)GET_Y_LPARAM ( lParam ) );

				if ( m_bEnabkeUseOnlyRgnDblClk )
				{
					for ( int i = 0; i < 4; ++i )
					{
						double dX		=	GetAt ( i ).dX;
						double dY		=	GetAt ( i ).dY;

						if ( MinPoint.dX > dX )
							MinPoint.dX = dX;
						if ( MinPoint.dY > dY )
							MinPoint.dY = dY;

						if ( MaxPoint.dX < dX )
							MaxPoint.dX = dX;
						if ( MaxPoint.dY < dY )
							MaxPoint.dY = dY;
					}
				}

				if ( m_bEditingModeEnabled ) 
				{
					if ( m_bEnabkeUseOnlyRgnDblClk )
					{
						if ( false == ( MinPoint.dX <= iMouseX && MaxPoint.dX >= iMouseX && MinPoint.dY <= iMouseY && MaxPoint.dY >= iMouseY ) )
						{
							EndEditText();

							m_CurrPos		= m_SelectEnd;
							m_SelectStart	= m_SelectEnd;

							return FALSE;
						}
					}

					SelectByDblClick(); 
					OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
					return FALSE;
				}
				else
				{
					if ( m_bEnabkeUseOnlyRgnDblClk )
					{
						if ( false == ( MinPoint.dX <= iMouseX && MaxPoint.dX >= iMouseX && MinPoint.dY <= iMouseY && MaxPoint.dY >= iMouseY ) )
						{
							return FALSE;
						}
					}

					m_bEditingModeEnabled = true;
					SendMessage( Constants::sc_nMessageTrackTextBegin, GetCurrentPointIndex() );
					SplitString();
					m_SelectStart = m_SelectEnd = m_CurrPos;
					OnLButtonDown ( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
				}
			}
		
			// For East Asian languages such as Chinese, Japanese, Korean, and other languages with complex characters. 
			// - Input Method Editors (IME) 

			WCHAR* pIMEUI	=	NULL;
			LONG IMEUIBytes	=	0;

			if ( TRUE == m_oIMEUIController.ProcessMessage ( nMessage, wParam, lParam, &pIMEUI, IMEUIBytes ) )
			{
				CStringW PastleStr	=	CStringW ( pIMEUI );
				delete [] pIMEUI;

				m_String.Insert ( m_CurrPos, PastleStr );
				
				m_CurrPos		=	m_SelectEnd + PastleStr.GetLength();
				m_SelectEnd		=	m_SelectStart	=	m_CurrPos;
				
				SplitString ( );

				SendMessage ( Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex() );

				return FALSE;
			}

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ((nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 && !m_AutoSize )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			if( PosOnMouse ( (int)MapToDataX( nX ), (int)MapToDataY( nY ) ) != -1 && m_bEditingModeEnabled && !m_IsTransfom )
				nPointStatus = c_nTrackPointStatusIBeam;

			return FALSE;
		}
		virtual void GetZoom( Tracking::CTrackPoints* pTrack )
		{
			if (!pTrack || pTrack->GetCount() < 1 ) return;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray( pPoints, 10 );

			Structures::RECTD rectD( pPoints->x, pPoints->y, (pPoints+2)->x, (pPoints+2)->y );
			rectD.Normalize();	

			Structures::RECTD rectDTracking( GetAt(0).dX, GetAt(0).dY, GetAt(2).dX, GetAt(2).dY );
			rectDTracking.Normalize();

			if( rectDTracking.GetWidth() > 0 )
				m_dZoom = rectD.GetWidth() / rectDTracking.GetWidth();

			if( pPoints )
			{
				delete []pPoints;
				pPoints = NULL;
			}
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			BSTR fontName = m_FontName.AllocSysString();
			
			POINT selectInfo;
			selectInfo.x = m_SelectStart;
			selectInfo.y = m_SelectEnd;
			
			GetZoom ( this );

			if ( m_hDC != oPainter.GetDC() ) 
				m_hDC = oPainter.GetDC();

			if( m_AutoSize && m_IsFirstResize && ( GetAt(1).dX == GetAt(0).dX + m_FontSize ) && ( GetAt(2).dY == GetAt(1).dY + m_FontSize ) ) 
			{
				SplitString();
				
				if ( m_String.GetLength() > 0 )
					return;
			}

			oPainter.DrawEditTextW ( this, m_dZoom, m_arrStrings, (WCHAR*)&m_String, m_CurrPos, selectInfo, fontName, m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText, m_bEnableDrawText );

			SysFreeString( fontName ); 

			/*
			BSTR string   = m_String.AllocSysString();
			BSTR fontName = m_FontName.AllocSysString();
			POINT selectInfo;
			selectInfo.x = m_SelectStart;
			selectInfo.y = m_SelectEnd;
			GetZoom( this );
			if( m_hDC != oPainter.GetDC() ) m_hDC = oPainter.GetDC();
			if( m_AutoSize && m_IsFirstResize &&
				( GetAt(1).dX == GetAt(0).dX + m_FontSize ) && 
				( GetAt(2).dY == GetAt(1).dY + m_FontSize ) ) 
			{
				SplitString();
				if( wcslen( string ) > 0 ) return;
			}
			oPainter.DrawEditText( this, m_dZoom, m_arrStrings, string, m_CurrPos, selectInfo, fontName, 
				m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText );
			SysFreeString( string ); 
			SysFreeString( fontName ); 
			*/
		}

		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if ( !m_AutoSize )
				oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}
		inline void EnableBlockEditing ( bool Value )
		{
			m_bBlockEditing				=	Value;
		}

		inline void EnabkeUseOnlyRgnDblClk ( bool Value )
		{
			m_bEnabkeUseOnlyRgnDblClk	=	Value;
		}

	
	public:
	
		virtual void Create( double dLeft, double dTop, double dRight, double dBottom, WCHAR* string, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool bBlockEditing = false )
		{
			m_String   		= CStringW ( string );
			m_FontName 		= CStringW ( fontName );
			
			m_CurrPos  		= m_String.GetLength();
			m_FontSize 		= fontSize;
			m_FontType		= fontType;
			m_FontColor		= FontColor;
			m_bEditingModeEnabled		= false;
			m_IsSelect		= false;
			m_IsTransfom	= true;
			m_SelectStart	= 0;
			m_SelectEnd		= 0;
			m_hDC			= NULL;
			m_dZoom			= 1.0;
			m_bBlockEditing				=	bBlockEditing;
			m_bEnabkeUseOnlyRgnDblClk	=	false;

			if( dRight < 0 && dBottom < 0 ) 
				m_AutoSize = true;
			else					
				m_AutoSize = false;

			//m_ScaleText = (bool)GetFontScale( m_FontType );

			if ( GetFontScale( m_FontType ) != 0 )
				m_ScaleText = true;
			else
				m_ScaleText = false;

			m_Scale.x = 1.0;
			m_Scale.y = 1.0;

			switch ( GetFontAlignedH ( m_FontType ) )
			{
			case ALIGNED_TOP:
				m_FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				m_FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				m_FontAlignW = byBottom;
				break;
			default:
				m_FontAlignW = byTop;
			}

			// if create by mouse
			if( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 || m_AutoSize )	m_IsFirstResize = true;
			else																		m_IsFirstResize = false;

			SetType( Constants::sc_nTrackTypeText );

			Clear();

			if( m_AutoSize )
			{
				dRight = dLeft + fontSize;
				dBottom = dTop + fontSize;
			}

			Add( dLeft,  dTop );
			Add( dRight, dTop );
			Add( dRight, dBottom );
			Add( dLeft,  dBottom );

			Add( 0.5 * ( dLeft + dRight ), dTop );
			Add( dRight, 0.5 * ( dTop + dBottom ) );
			Add( 0.5 * ( dLeft + dRight ), dBottom );
			Add( dLeft, 0.5 * ( dTop + dBottom ) );

			Add( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );

			SetCurrentPointIndex(-1);
		}	
	
	public:		
	
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if( !m_IsTransfom )
			{
				int tmpPos = PosOnMouse ( (int)dX, (int)dY );
				if( tmpPos != -1 && m_bEditingModeEnabled )
				{
					if( m_IsSelect && !(GetAsyncKeyState( VK_SHIFT ) < 0) ) 
					{
						m_CurrPos	= tmpPos;
						m_SelectEnd = tmpPos;
					}

					if (false == IsTrackChild () ) 
						SendMessage(Constants::sc_nMessageTrackInvalidate, GetCurrentPointIndex());

					return false;
				}
			}

			if (!IsCurrentPointValid())
				return false;

			int nCurrentPoint = ( m_IsFirstResize && !m_AutoSize ) ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if (false == IsTrackChild () ) 
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				if( m_AutoSize ) 
					return true;

				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;
				}
				else if (nCurrentPoint == 4)
				{
					GetAt(4).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 5)
				{
					GetAt(5).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}
				else if (nCurrentPoint == 6)
				{
					GetAt(6).dY = dY;
					GetAt(7).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
					GetAt(5).dY = 0.5*(GetAt(4).dY + GetAt(6).dY);
				}
				else if (nCurrentPoint == 7)
				{
					GetAt(7).dX = dX;
					GetAt(4).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
					GetAt(6).dX = 0.5*(GetAt(5).dX + GetAt(7).dX);
				}

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 4)
				{
					GetAt(4).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(0).dY);
					GetAt(5).Create(GetAt(2).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
					GetAt(6).Create(0.5*(GetAt(0).dX + GetAt(2).dX), GetAt(2).dY);
					GetAt(7).Create(GetAt(0).dX, 0.5*(GetAt(0).dY + GetAt(2).dY));
				}

				// recompute corners
				if (nCurrentPoint >= 4 && nCurrentPoint < 8)
				{
					GetAt(0).Create(GetAt(7).dX, GetAt(4).dY);
					GetAt(1).Create(GetAt(5).dX, GetAt(4).dY);
					GetAt(2).Create(GetAt(5).dX, GetAt(6).dY);
					GetAt(3).Create(GetAt(7).dX, GetAt(6).dY);
				}

				SplitString();

				if (false == IsTrackChild () ) 
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int tmpPos = PosOnMouse ( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );

			if( tmpPos != -1 && m_bEditingModeEnabled ) 
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 )
				{
					m_CurrPos	= tmpPos;
					m_SelectEnd	= tmpPos;
				}
				else
				{
					m_CurrPos		= tmpPos;
					m_SelectStart	= tmpPos;
				}
				m_IsSelect		= true;
				m_IsTransfom	= false;

				// it's for repaint
				OnMouseMove( MouseX, MouseY );
				return true;
			}

			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY );

			if( nTrackIndex >= 0 && nTrackIndex < 8 )
				m_IsTransfom = true;

			if ( -1 == nTrackIndex )
				return false;

			if( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create ( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			if( -1 == GetPointUnderCursor( MouseX, MouseY ) )
				EndEditText();

			if( m_AutoSize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			SetCurrentPointIndex( nTrackIndex );

			if ( false== IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return false; 
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			m_IsFirstResize = false;

			if( GetAsyncKeyState( VK_SHIFT ) >= 0 ) 
			{
				if( m_IsSelect )
				{
					int tmpPos = PosOnMouse ( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
					if( tmpPos != -1 && m_bEditingModeEnabled ) 
					{
						m_CurrPos		= tmpPos;
						m_SelectEnd 	= tmpPos;
					}
					m_IsSelect		= false;
				}
			}

			if (!IsCurrentPointValid())
				return false;
		
			if ( false== IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			m_IsTransfom = false;

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	
	protected:
	
		HDC						m_hDC;
		double					m_dZoom;
		bool					m_IsFirstResize;
		bool					m_AutoSize;
		bool					m_ScaleText;

		Structures::POINTD		m_Scale;

		CStringW				m_String;
		CStringW				m_FontName;
		int						m_CurrPos;
		int						m_FontSize;
		int						m_FontType;
		int						m_FontColor;
		bool					m_bEditingModeEnabled;
		bool					m_IsSelect;
		bool					m_IsTransfom;
		int						m_SelectStart;
		int						m_SelectEnd;
		int						m_FontAlignW;

		CSimpleArray<CStringW>	m_arrStrings;

		bool					m_bBlockEditing;

		bool					m_bEnabkeUseOnlyRgnDblClk;		// использовать двойнок клик в области трека, клик в зоне - активация, вне зоны сброс.
		
		WCHAR					m_chDeadKey;
		
		bool					m_bEnableDrawText;

		CIMEUIController		m_oIMEUIController;
	};
	
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectTextRotatable : public CTrackObjectText
	{
	public:

		CTrackObjectTextRotatable ()
		{
			m_hDC						=	NULL;

			m_bResizeShapeToFitText		=	false;
			m_bPaintPoints				=	true;

			m_nCaretX					=	0.0;
			m_nCaretY					=	0.0;
		}

	protected:

		int GetPointUnderCursor( int nX, int nY )
		{
			int nTrackIndex = FindByInteger( nX, nY, c_nTrackPointSizeTouch, FALSE );

			if( nTrackIndex < 0 )
			{
				Structures::RECTI rectD;

				double dX = MapToDataX( nX );
				double dY = MapToDataY( nY );

				double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );

				double tX, tY;
				double cosA = cos( -(dAngle + M_PI_2) );
				double sinA = sin( -(dAngle + M_PI_2) );

				tX = dX - GetAt(9).dX;
				tY = dY - GetAt(9).dY;				
				dX = GetAt(9).dX + tX * cosA - tY * sinA;
				dY = GetAt(9).dY + tX * sinA + tY * cosA;

				Structures::POINTI pointTrack ( (int)dX, (int)dY );

				rectD.left   = (int)GetAt(0).dX;
				rectD.top    = (int)GetAt(0).dY;
				rectD.right  = (int)GetAt(2).dX;
				rectD.bottom = (int)GetAt(2).dY;

				tX = GetAt(0).dX - GetAt(9).dX;
				tY = GetAt(0).dY - GetAt(9).dY;				
				rectD.left  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.top   = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );	

				tX = GetAt(2).dX - GetAt(9).dX;
				tY = GetAt(2).dY - GetAt(9).dY;
				rectD.right  = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
				rectD.bottom = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

				if( rectD.right < rectD.left ) 
				{ 
					double tmpRight		= rectD.right; 
					rectD.right			= rectD.left;
					rectD.left			= (int)tmpRight;
				}
				if( rectD.top > rectD.bottom ) 
				{ 
					double tmpBottom	= rectD.bottom; 
					rectD.bottom		= rectD.top;
					rectD.top			= (int)tmpBottom;
				}

				if( rectD.IsPointInside( pointTrack ) )
					nTrackIndex = GetCount() - 1;
				else
					return -1;
			}

			return nTrackIndex;
		}

		void RotateToAlpha( double alpha )
		{
			for( int index = 0; index < 9; ++index )
				Geometry::RotatePoint( GetAt(index).dX, GetAt(index).dY, GetAt(9).dX, GetAt(9).dY, alpha );
		}
		inline int GetStringByPos ( int& caretPos )
		{
			int arrSize		=	m_arrStrings.GetSize();
			for ( int index = 0; index < arrSize; ++index )
			{
				if ( caretPos < m_arrStrings[index].GetLength() || ( index == arrSize - 1 ) ) 
					return index;

				caretPos	-=	m_arrStrings[index].GetLength();
			}

			return -1;
		}
		int PosOnMouse( int dX, int dY )
		{
			if ( NULL == m_hDC ) 
				return -1;
			
			int countLines	=	m_arrStrings.GetSize();
			
			if ( 0 == countLines )
				return -1;

#ifdef _DEBUG
			//CTraceTimer oTimer ( L"PosOnMouse" );
#endif
			Gdiplus::Graphics graphics ( m_hDC );

			Gdiplus::RectF			orig, currStrRect;
			
			//Gdiplus::FontFamily	fontFamily ( m_FontName );
			//Gdiplus::Font			font ( &fontFamily, (Gdiplus::REAL)m_FontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel );
			
			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily ) )
				return -1;
					
			Gdiplus::StringFormat	stringFormat ( Gdiplus::StringAlignmentNear );
			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );

			if ( !m_ScaleText )
			{
				switch ( GetFontAlignedW ( m_FontType ) )
				{
				case ALIGNED_LEFT:
					stringFormat.SetAlignment ( StringAlignmentNear ); 
					break;
				case ALIGNED_CENTER:
					stringFormat.SetAlignment ( StringAlignmentCenter ); 
					break;
				case ALIGNED_RIGHT:
					stringFormat.SetAlignment ( StringAlignmentFar ); 
					break;
				default:
					stringFormat.SetAlignment ( StringAlignmentNear ); 
					break;
				}
			}

			double dAngle			=	Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
			Structures::RECTD rectD	=	GetUnrotatableRect ( dAngle );

			double tX, tY;
			double cosA = cos( -dAngle );
			double sinA = sin( -dAngle );

			tX = dX - GetAt(9).dX;
			tY = dY - GetAt(9).dY;
			dX = (int) ( GetAt(9).dX + tX * cosA - tY * sinA );
			dY = (int) ( GetAt(9).dY + tX * sinA + tY * cosA );

			if ( dY > rectD.bottom )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );

				return -1;
			}

			double dWidth	= rectD.GetWidth();
			double dWidth_2 = dWidth / 2.0;

			Structures::POINTI pointTrack( dX, dY );

			int tmpStrLen = 0;

			double fontHeight	=	Rendering::Utils::GetFontHeight( pFont );

			double displaceByY	=	countLines * fontHeight;

			for ( int i = 0; i < countLines; ++i )
			{
				graphics.MeasureString ( (WCHAR*)m_arrStrings[i].GetString(), -1, pFont, orig, &stringFormat, &currStrRect );	

				currStrRect.X		= (Gdiplus::REAL)rectD.left;
				currStrRect.Width	= (Gdiplus::REAL)dWidth;
				
				if ( !m_ScaleText )
				{
					switch( m_FontAlignW )
					{
					case byTop:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.top );
							break;
						}
					case byMiddle:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.GetCenter().y - (Gdiplus::REAL)displaceByY / 2.0f );
							break;	
						}
					case byBottom:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.bottom - (Gdiplus::REAL)displaceByY );
							break;	
						}
					default:
						{
							currStrRect.Offset( 0.0f, (Gdiplus::REAL)i * (Gdiplus::REAL)fontHeight + (Gdiplus::REAL)rectD.top );
						}
					}
				}
				else
				{
					currStrRect.Height = (Gdiplus::REAL)fontHeight * (Gdiplus::REAL)m_Scale.y * (Gdiplus::REAL)countLines;
					currStrRect.Offset ( 0.0f, (Gdiplus::REAL) i * (Gdiplus::REAL)currStrRect.Height + (Gdiplus::REAL)rectD.top );
				}

				if ( currStrRect.Contains ( (Gdiplus::REAL)dX, (Gdiplus::REAL)dY ) )
				{
					graphics.MeasureString ( (WCHAR*)m_arrStrings[i].GetString(), -1, pFont, orig, &stringFormat, &currStrRect );

					currStrRect.Height = (Gdiplus::REAL)fontHeight;

					currStrRect.Width  *= (Gdiplus::REAL)m_Scale.x;
					currStrRect.Height *= (Gdiplus::REAL)m_Scale.y;

					if ( stringFormat.GetAlignment() == StringAlignmentNear )
					{
						currStrRect.Offset ( (Gdiplus::REAL)rectD.left, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					}
					else 
					if ( stringFormat.GetAlignment() == StringAlignmentCenter )
					{
						currStrRect.Offset ( (Gdiplus::REAL)rectD.left + (Gdiplus::REAL)dWidth_2, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					}
					else 
					if ( stringFormat.GetAlignment() == StringAlignmentFar )
					{
						currStrRect.Offset ( (Gdiplus::REAL)rectD.right, (Gdiplus::REAL)i * currStrRect.Height + (Gdiplus::REAL)rectD.top );
					}

					double partOfChar = pFont->GetSize() * m_Scale.x / 5.0;

					Gdiplus::StringFormat localStringFormat;
					localStringFormat.SetAlignment ( StringAlignmentNear );

					graphics.MeasureString ( (WCHAR*)m_arrStrings[i].GetString(), -1, pFont, orig, &localStringFormat, &currStrRect );

					CStringW lineString = m_arrStrings[i];
					lineString.Replace( L"\r\n", L"" );

					if ( stringFormat.GetAlignment () == StringAlignmentNear )
					{
						for ( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;

							GetRectCurrentCharacter ( &graphics, &localStringFormat, pFont, (WCHAR*)m_arrStrings[i].GetString(), -1, j, currStrRect, currCharRect );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.left )
							{	
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								return tmpStrLen + j;
							}
							if ( j == lineString.GetLength() - 1 )
							{	
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								return tmpStrLen + j + 1;
							}

						}	
					}
					else if ( stringFormat.GetAlignment() == StringAlignmentCenter )
					{
						for ( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;
							
							GetRectCurrentCharacter ( &graphics, &localStringFormat, pFont, (WCHAR*)m_arrStrings[i].GetString(), -1, j, currStrRect, currCharRect );

							if( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= ( dX - (rectD.left + rectD.GetWidth() / 2 ) ) + currStrRect.Width / 2 )
							{	
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								
								return tmpStrLen + j;
							}
							else 
								//	if( dX < currStrRect.X ) 
								//	return tmpStrLen;
								if ( j == lineString.GetLength() - 1 )
								{	
									RELEASEOBJECT ( pFont );
									RELEASEOBJECT ( pFontFamily );
									return tmpStrLen + j + 1;
								}
						}
					}
					else if ( stringFormat.GetAlignment() == StringAlignmentFar )
					{
						for ( int j = 0; j < m_arrStrings[i].GetLength(); ++j )
						{
							RectF currCharRect;

							GetRectCurrentCharacter( &graphics, &localStringFormat, pFont, (WCHAR*)m_arrStrings[i].GetString(), -1, j, currStrRect, currCharRect );

							if ( ( currCharRect.X + currCharRect.Width * 0.5f ) * m_Scale.x >= dX - rectD.right + currStrRect.Width )
							{	
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );
								
								return tmpStrLen + j;
							}
							
							if ( j == lineString.GetLength() - 1 )
							{	
								RELEASEOBJECT ( pFont );
								RELEASEOBJECT ( pFontFamily );

								return tmpStrLen + j + 1;
							}

						}
					}
					
					RELEASEOBJECT ( pFont );
					RELEASEOBJECT ( pFontFamily );

					return tmpStrLen + m_arrStrings[i].GetLength();	// -2 for skeep "\r\n";
				}
				
				tmpStrLen += m_arrStrings[i].GetLength();
			}
		
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

			return -1;				
		}

		inline void SplitString ()
		{
			m_arrStrings.RemoveAll();

			if ( 0 == m_String.GetLength() || NULL == m_hDC ) 
				return;

			Graphics graphics ( m_hDC );

			double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
			Structures::RECTD rectD = GetUnrotatableRect( dAngle );

			double dWidth  = rectD.GetWidth();

			if ( m_bResizeShapeToFitText )
			{
				dWidth  = 65535.0;
			}

			RectF	orig, rectF, strBB;

			int fontStyle	= Gdiplus::FontStyleRegular;
			
			if ( m_FontType & FONTSTYLE_BOLD )		
				fontStyle |= Gdiplus::FontStyleBold;
			
			if ( m_FontType & FONTSTYLE_ITALIC )		
				fontStyle |= Gdiplus::FontStyleItalic;
			
			if ( m_FontType & FONTSTYLE_STRIKEOUT )	
				fontStyle |= Gdiplus::FontStyleStrikeout;
			
			if ( m_FontType & FONTSTYLE_UNDERLINE )	
				fontStyle |= Gdiplus::FontStyleUnderline;

			//Gdiplus::FontFamily		fontFamily ( m_FontName );
			//Gdiplus::Font			font( &fontFamily, (Gdiplus::REAL)m_FontSize, fontStyle, Gdiplus::UnitPixel );

			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily, fontStyle ) )
				return;

			Gdiplus::StringFormat	stringFormat;
			stringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
			
			double maxWidth		= 0;
			double maxHeight	= 0;

			Gdiplus::REAL nFontSize = pFont->GetSize();
		
			if ( dWidth + 0.5 < nFontSize && !m_ScaleText )
			{
				RELEASEOBJECT ( pFont );
				RELEASEOBJECT ( pFontFamily );

				return;
			}

			int strLen = m_String.GetLength();

			int		strLine		= 0;
			int		countChar	= 1;
			double  displaceX	= 0.0;
			double  displaceY	= 0.0;
			bool	isEnd		= false;

			BSTR fullString = m_String.AllocSysString();
			
			int StringsCount	=	1;

			for( int i = 0; i < strLen; ++i )
			{
				if( *( fullString + strLine + countChar - 1 ) == _T('\r') )
				{
					CStringW Source = CStringW ( fullString + strLine );
					++i;
					Source.Delete ( countChar + 1, strLen - i + 2 );
					
					m_arrStrings.Add ( Source );
					
					strLine		=	i + 1;
					countChar	=	1;
					isEnd		=	true;
					
					++StringsCount;
					
					continue;
				}

				graphics.MeasureString ( fullString + strLine, countChar, pFont, orig, &stringFormat, &strBB );
				displaceX = strBB.Width;
				maxWidth = max( maxWidth, displaceX );
				
				if( displaceX > dWidth && !m_AutoSize && !m_ScaleText )
				{
					CStringW tmpStr = CStringW ( fullString + strLine );					
					tmpStr.Delete( countChar - 1, strLen - i );
					BSTR subString = tmpStr.AllocSysString();
					BSTR rSpace = wcsrchr( subString, _T(' ') );
					
					if( NULL != rSpace )
					{						
						int strLenSpace	=	(int)wcslen( rSpace );
						i				-=	strLenSpace - 1;
						tmpStr.Delete ( countChar - strLenSpace, strLenSpace );
					}
					m_arrStrings.Add( tmpStr );
					strLine		= i;
					countChar	= 1;
					isEnd = true;
					SysFreeString( subString );
				}	
				countChar++;
				isEnd = false;
			}
			
			if( !isEnd )
			{
				m_arrStrings.Add ( CStringW ( fullString + strLine ) + L"\r\n" );
			}
			
			m_String.FreeExtra();

			if ( IsLastLineEmpty () )
			{
				m_arrStrings.Add ( L"\r\n" );
			}

#ifdef _DEBUG

			/*
			
			ATLTRACE (_T("==============================================\r\n") );
			
			for ( int i = 0; i < m_arrStrings.GetSize (); ++i )
			{
				CStringW Str = m_arrStrings[i];
				Str.Replace ( L'\r', L'_' );
				Str.Replace ( L'\n', L'_' );
				ATLTRACE ( Str );

				ATLTRACE ( L"\n" );
			}
			
			ATLTRACE (_T("==============================================\r\n") );
			
			*/
#endif

			double fontHeight = Rendering::Utils::GetFontHeight( pFont );
			if( m_ScaleText )
			{
				double dHeight = rectD.GetHeight();

				maxHeight = fontHeight * StringsCount; // m_arrStrings.GetSize();

				if( m_arrStrings.GetSize() > 0 && maxHeight > 0)
				{
					m_Scale.x = dWidth / max( 0.001, maxWidth );
					m_Scale.y = dHeight / maxHeight / m_arrStrings.GetSize();
				}
			}
			else if( m_AutoSize )
			{
				maxHeight = fontHeight * StringsCount; // m_arrStrings.GetSize();

				double eX1, eX2, eY1, eY2;

				eX1 = 1.0; eY1 = 0.0;
				eX2 = 0.0; eY2 = 1.0;

				double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
				Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
				Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );

				GetAt(2).dX = GetAt(0).dX + eX1 * maxWidth + eX2 * maxHeight;
				GetAt(2).dY = GetAt(0).dY + eY1 * maxWidth + eY2 * maxHeight;
				GetAt(1).dX = GetAt(0).dX + eX1 * maxWidth;
				GetAt(1).dY = GetAt(0).dY + eY1 * maxWidth;
				GetAt(3).dX = GetAt(0).dX + eX2 * maxHeight;
				GetAt(3).dY = GetAt(0).dY + eY2 * maxHeight;

				GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
				GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
				GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
				GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

				double vX = GetAt(4).dX - GetAt(6).dX;
				double vY = GetAt(4).dY - GetAt(6).dY;

				double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
				if( length < 1 )
				{
					vX = -eX2; 
					vY = -eY2;
				}
				else
				{
					vX /= length;
					vY /= length;
				}

				GetAt(8).dX = GetAt(4).dX + vX * 30;
				GetAt(8).dY = GetAt(4).dY + vY * 30;
				GetAt(9).Create ( 0.5 * ( GetAt(0).dX + GetAt(2).dX ), 0.5 * ( GetAt(2).dY + GetAt(0).dY ) );

			}
			
			SysFreeString ( fullString );

			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );
		}

		void EndEditText()
		{
			if( m_bEditingModeEnabled )
			{
				m_bEditingModeEnabled = false;
				SendMessage( Constants::sc_nMessageTrackTextEnd, GetCurrentPointIndex() );
			}
		}
		void StickAngle( double &dAngle, double angleStick )
		{
			int		countStick	 = 0;
			double  tmpBaseAngle = dAngle;
			double  rangeStick	 = rad(2);
			if( 0 == angleStick || rangeStick > angleStick ) angleStick = M_PI_2;
			if( dAngle < 0 ) angleStick = -angleStick;
			while( (dAngle < 0) ? (tmpBaseAngle < angleStick + rangeStick) : (tmpBaseAngle > angleStick - rangeStick) )
			{ 
				tmpBaseAngle -= angleStick;
				countStick++;
			}
			if( abs( tmpBaseAngle ) < rangeStick ) 
			{
				tmpBaseAngle = countStick * angleStick;
				dAngle = tmpBaseAngle;
			}
		}
		void UpdateMinimizeRegion ( int CapturePoint )
		{
			double CharSize		=	(double)m_FontSize * (double)GetDeviceCaps ( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
			double CharSizeRotX	=	CharSize;
			double CharSizeRotY	=	CharSize;

			double RotateAngle = Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
			Geometry::RotatePoint ( CharSizeRotX, CharSizeRotY, 0.0, 0.0, RotateAngle );
		
			double BoundWidth		=	Geometry::GetLength ( GetAt(0).dX, GetAt(0).dY, GetAt(1).dX, GetAt(1).dY );
			double BoundHeight		=	Geometry::GetLength ( GetAt(0).dX, GetAt(0).dY, GetAt(3).dX, GetAt(3).dY );

			double BoundWidthRot	=	BoundWidth;
			double BoundHeightRot	=	BoundHeight;
			
			//-------------------------------------------------------------------------------------------

			if ( CapturePoint == 0 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;
					
					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;

					GetAt(0).dY		=	GetAt(1).dY - OffsetY;
					GetAt(7).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(5).dY );
					GetAt(3).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(2).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;
			
					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;
					
					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------
			
			// TODO_

			if ( CapturePoint == 1 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;
					
					GetAt(1).dX		=	GetAt(0).dX + OffsetX;
					GetAt(5).dX		=	GetAt(7).dX + OffsetX;
					GetAt(2).dX		=	GetAt(3).dX + OffsetX;

					GetAt(1).dY		=	GetAt(0).dY + OffsetY;
					GetAt(5).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(7).dY );
					GetAt(2).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(3).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;
			
					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;
					
					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------
			
			if ( CapturePoint == 2 || CapturePoint == 5 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;
					
					GetAt(1).dX		=	GetAt(0).dX + OffsetX;
					GetAt(5).dX		=	GetAt(7).dX + OffsetX;
					GetAt(2).dX		=	GetAt(3).dX + OffsetX;

					GetAt(1).dY		=	GetAt(0).dY + OffsetY;
					GetAt(5).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(7).dY );
					GetAt(2).dY		=	GetAt(0).dY + OffsetY - ( GetAt(0).dY - GetAt(3).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;
			
					GetAt(3).dY		=	GetAt(0).dY + OffsetY;
					GetAt(6).dY		=	GetAt(4).dY + OffsetY;
					GetAt(2).dY		=	GetAt(1).dY + OffsetY;
					
					GetAt(3).dX		=	GetAt(0).dX + OffsetX;
					GetAt(6).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(4).dX );
					GetAt(2).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(1).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------
			
			if ( CapturePoint == 3 || CapturePoint == 6 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;
					
					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;
				
					GetAt(3).dY		=	GetAt(2).dY - OffsetY;
					GetAt(7).dY		=	GetAt(2).dY - OffsetY - ( GetAt(2).dY - GetAt(5).dY );
					GetAt(0).dY		=	GetAt(2).dY - OffsetY - ( GetAt(2).dY - GetAt(1).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;
					
					GetAt(3).dY		=	GetAt(0).dY + OffsetY;
					GetAt(6).dY		=	GetAt(4).dY + OffsetY;
					GetAt(2).dY		=	GetAt(1).dY + OffsetY;
					
					GetAt(3).dX		=	GetAt(0).dX + OffsetX;
					GetAt(6).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(4).dX );
					GetAt(2).dX		=	GetAt(0).dX + OffsetX - ( GetAt(0).dX - GetAt(1).dX );

					RegionMinimize	=	true;
				}
			}
		
			//-------------------------------------------------------------------------------------------
		
			if ( CapturePoint == 4 || CapturePoint == 7 )
			{
				bool RegionMinimize	=	false;

				if ( BoundWidth <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle ) * CharSize;
					double OffsetY	=	sin ( RotateAngle ) * CharSize;
					
					GetAt(0).dX		=	GetAt(1).dX - OffsetX;
					GetAt(7).dX		=	GetAt(5).dX - OffsetX;
					GetAt(3).dX		=	GetAt(2).dX - OffsetX;

					GetAt(0).dY		=	GetAt(1).dY - OffsetY;
					GetAt(7).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(5).dY );
					GetAt(3).dY		=	GetAt(1).dY - OffsetY - ( GetAt(1).dY - GetAt(2).dY );

					RegionMinimize	=	true;
				}

				if ( BoundHeight <= CharSize )
				{
					double OffsetX	=	cos ( RotateAngle + M_PI / 2.0 ) * CharSize;
					double OffsetY	=	sin ( RotateAngle + M_PI / 2.0 ) * CharSize;
			
					GetAt(0).dY		=	GetAt(3).dY - OffsetY;
					GetAt(4).dY		=	GetAt(6).dY - OffsetY;
					GetAt(1).dY		=	GetAt(2).dY - OffsetY;
					
					GetAt(0).dX		=	GetAt(3).dX - OffsetX;
					GetAt(4).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(6).dX );
					GetAt(1).dX		=	GetAt(3).dX - OffsetX - ( GetAt(3).dX - GetAt(2).dX );

					RegionMinimize	=	true;
				}
			}

			//-------------------------------------------------------------------------------------------

		}
		virtual void OnKeyDown ( unsigned int Key )
		{
			if ( false == m_bEditingModeEnabled )
				return;

			bool isShift	= false;
			bool isCtrl		= false;
			
			if ( Key == VK_ESCAPE )
			{
				EndEditText();
				m_CurrPos		= m_SelectEnd;
				m_SelectStart	= m_SelectEnd;
			}
			else if ( Key == VK_CONTROL )
			{
				isCtrl = true;
			}
			else if ( Key == VK_LEFT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos > 0 ) m_CurrPos--;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;
			}
			else if ( Key == VK_RIGHT )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				if( m_CurrPos < m_String.GetLength() ) m_CurrPos++;
				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos++;
			}
			else if ( Key == VK_UP )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( currIdxString < 1 ) return;

				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();

				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if( m_arrStrings[currIdxString - 1].GetLength() - 2 < posInLine ) // - 2 for "\r\n"
						m_CurrPos -= posInLine + 2; // + 2 for "\r\n"
					else
						m_CurrPos -= m_arrStrings[currIdxString - 1].GetLength();	
				}
				else
				{
					if( m_arrStrings[currIdxString - 1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine ) // - 2 for "\r\n"	
						m_CurrPos -= posInLine + m_arrStrings[currIdxString - 1].GetLength();
					else
						m_CurrPos -= m_arrStrings[currIdxString].GetLength();
				}
			}
			else if ( Key == VK_DOWN )
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 ) m_IsSelect = true;
				int tmpCaretPos = m_CurrPos;
				int currIdxString = GetStringByPos( tmpCaretPos );
				if( -1 == currIdxString || currIdxString == m_arrStrings.GetSize() - 1 ) return;
				int sumLineLenght = 0;
				for( int i = 0; i <= currIdxString; ++i ) sumLineLenght += m_arrStrings[i].GetLength();
				int posInLine = m_arrStrings[currIdxString].GetLength() - sumLineLenght + m_CurrPos;

				if( GetFontAlignedW( m_FontType ) != ALIGNED_RIGHT )
				{
					if ( m_arrStrings[currIdxString + 1].GetLength() - 2 < posInLine ) 
						m_CurrPos += sumLineLenght - m_CurrPos + m_arrStrings[currIdxString + 1].GetLength() - 2;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength();
				}
				else
				{
					if( m_arrStrings[currIdxString+1].GetLength() < m_arrStrings[currIdxString].GetLength() - posInLine )
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine;
					else
						m_CurrPos += m_arrStrings[currIdxString].GetLength() - posInLine + (m_arrStrings[currIdxString+1].GetLength() - m_arrStrings[currIdxString].GetLength() + posInLine );
				}
			}
			else if ( Key == VK_SHIFT )
			{
				//if( !m_IsSelect ) m_SelectStart = m_SelectEnd = m_CurrPos;
				isShift = true;
			}
			else if ( Key == VK_DELETE )
			{
				if ( m_CurrPos == m_String.GetLength() && m_SelectStart == m_SelectEnd )
				{
					UpdateResizeShapeToFitText	( );

					return;
				}

				if ( m_CurrPos > 0 )
				{
					if ( m_CurrPos - 1 < m_String.GetLength () )	// ASSERT LENGTH
					{
						if ( m_String [ m_CurrPos - 1 ] == '\r' )
							m_CurrPos--;
					}
				}

				if ( m_SelectStart != m_SelectEnd )
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos + 2 <= m_String.GetLength() )
						if( m_String[ m_CurrPos + 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos, 2 );
							deleteRN = true;
						}
						if( !deleteRN )
							m_String.Delete( m_CurrPos );
				}

				SplitString();
				
				UpdateResizeShapeToFitText	( );
			}
			else if ( Key == VK_BACK )
			{
				if( m_CurrPos == 0 && m_SelectStart == m_SelectEnd ) return;

				if( m_CurrPos > 0 )
					if( m_String[ m_CurrPos - 1 ] == '\r' ) m_CurrPos--;

				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				else
				{
					bool deleteRN = false;
					if( m_CurrPos > 0 )
						if( m_String[ m_CurrPos - 1 ] == '\n' )
						{
							m_String.Delete( m_CurrPos - 2, 2 );
							m_CurrPos -= 2;
							deleteRN = true;
						}
						if( !deleteRN )
						{
							m_String.Delete( m_CurrPos - 1 );
							m_CurrPos--;
						}
				}

				SplitString();

			}
			else if ( Key == VK_SPACE )
			{
				if( m_SelectStart != m_SelectEnd  )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
				}
				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += _T(" ");
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, L" " );
					m_CurrPos++;
				}

				SplitString();

			}
			else if ( Key == VK_RETURN )
			{
				if( m_CurrPos == m_String.GetLength() )
				{
					m_String += L"\r\n";
					m_CurrPos = m_String.GetLength();
				}
				else
				{
					m_String.Insert( m_CurrPos, L"\r\n" );
					m_CurrPos += 2;
				}

				SplitString();

			}
			//else if( Key == VK_TAB )
			//{
			//}
			else if ( Key == VK_HOME )
			{
				if ( GetAsyncKeyState( VK_SHIFT ) < 0 ) 
					m_IsSelect = true;

				m_CurrPos = 0;
			}
			else if ( Key == VK_END )
			{
				if ( GetAsyncKeyState( VK_SHIFT ) < 0 ) 
					m_IsSelect = true;

				m_CurrPos = m_String.GetLength();
			}
			else 
			if (( Key >= 'A'		&& Key <= 'Z'		) || 
				( Key >= '0'		&& Key <= '9'		) || 
				( Key >= VK_OEM_1	&& Key <= VK_OEM_3	) || 
				( Key >= VK_OEM_4	&& Key <= VK_OEM_8	) ||
				( Key >= VK_NUMPAD0 && Key <= VK_DIVIDE ) )
			{
				
				/*
				int nShift		=	(GetKeyState(VK_SHIFT) < 0 ? 1 : 0);// + (GetKeyState(VK_CONTROL) < 0 ? 2 : 0) + (GetKeyState(VK_MENU) < 0 ? 4 : 0);

				ATLTRACE (_T("Ctr : {%d}, Shift : {%d}, Alt : {%d}\n"),
					(GetKeyState(VK_CONTROL) < 0 ? 1 : 0),
					(GetKeyState(VK_SHIFT) < 0 ? 1 : 0),
					(GetKeyState(VK_MENU) < 0 ? 1 : 0) );
					*/

				if ( GetAsyncKeyState( VK_CONTROL ) < 0 && ((GetKeyState(VK_MENU) < 0 ? 0 : 1) ) )
				{
					if ( Key == 'X' )
					{
						CopyToClipboard ( true );
					}

					if ( Key == 'C' )
					{
						CopyToClipboard ( );
					}

					if ( Key == 'V' )
					{
						PastToClipboard ( );

						if ( m_bResizeShapeToFitText )
						{
							UpdateResizeShapeToFitText	( );
						
							SendMessage ( Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex() );
						}
					}

					if ( Key == 'A' )
					{
						SelectTextAll ( );
				
						SendMessage ( Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex() );
					}

					return;
				}
				
				if ( m_SelectStart != m_SelectEnd )	
				{
					m_CurrPos = min( m_SelectEnd, m_SelectStart );
					m_String.Delete( m_CurrPos, abs( m_SelectEnd - m_SelectStart ) );
					m_IsSelect = false;
				}
				
				if ( m_CurrPos > 0 )
				{
					if ( m_CurrPos - 1 < m_String.GetLength () )	// ASSERT LENGTH
					{
						if ( '\r' == m_String [ m_CurrPos - 1 ] )
						{
							--m_CurrPos;
						}
					}
				}
				
				wchar_t wKey [ 3 ]			=	{ L'\0', L'\0', L'\0' };

				BYTE kbdState[256];
				ZeroMemory( kbdState, 256 );
				
				GetKeyboardState ( kbdState );
				
				wchar_t KeyboardState	=	wKey [ 0 ];
				
    			if ( 1 == ToUnicode ( Key, 0, kbdState, wKey, 3, 0 ) )
				{
					if ( m_chDeadKey != L'\0' ) 
					{
						wchar_t wcs_in[3];
						wchar_t wcs_out[3];

						wcs_in[0] = wKey[0];
						wcs_in[1] = m_chDeadKey;
						wcs_in[2] = L'\0';

						/* from accent char to unicode */
						if ( FoldStringW ( MAP_PRECOMPOSED, wcs_in, 3, wcs_out, 3 ) )
						{
							wKey[0] = wcs_out[0];
							wKey[1] = wcs_out[1];
							wKey[2] = wcs_out[2];
						}
					}
					else
					{

					}

					m_chDeadKey = L'\0';
				}
				else
				{
					m_chDeadKey			=	L'\0';

					switch(wKey[0]) 
					{
					case 0x5e:          /* circumflex */
						m_chDeadKey = 0x302;  break;
					case 0x60:          /* grave accent */
						m_chDeadKey = 0x300;  break;
					case 0xa8:          /* diaeresis */
						m_chDeadKey = 0x308;  break;
					case 0xb4:          /* acute accent */
						m_chDeadKey = 0x301;  break;
					case 0xb8:          /* cedilla */
						m_chDeadKey = 0x327;  break;
					default:
						m_chDeadKey	=	wKey[0];
						break;
					}

					return;
				}					

				wKey [ 1 ]	=	L'\0';

				if ( m_CurrPos == m_String.GetLength() )
				{
					m_String	+=	CStringW ( wKey );
					m_CurrPos	=	m_String.GetLength ();
				}
				else
				{
					m_String.Insert ( m_CurrPos, CStringW ( wKey ) );
					++m_CurrPos;
				}

				SplitString();
			}
			
			if ( !isShift && !isCtrl )
			{
				if( m_IsSelect )	m_SelectEnd = m_CurrPos;
				else				m_SelectStart = m_SelectEnd = m_CurrPos;
			}
			
			UpdateResizeShapeToFitText	( );

			SendMessage ( Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex() );
		}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if ( nMessage == WM_DEADCHAR )
			{
				OnDeadChar ( (UINT)wParam, (UINT)lParam );
			}
			else 
				if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if ( nMessage == WM_KEYDOWN )
				CTrackObjectTextRotatable::OnKeyDown( (UINT)wParam );
			else if ( nMessage == WM_KEYUP )
				CTrackObjectText::OnKeyUp( (UINT)wParam );
			else if ( nMessage == WM_DESTROY )
				m_arrStrings.RemoveAll();
			else if ( nMessage == WM_KILLFOCUS )
			{
				EndEditText();
				//m_arrStrings.RemoveAll();
			}
			else if ( nMessage == WM_MOUSELEAVE )	
				m_IsSelect	= false;
			else if ( nMessage == WM_LBUTTONDBLCLK )
			{
				if ( m_bBlockEditing )
					return FALSE;

				if( m_bEditingModeEnabled ) 
				{
					SelectByDblClick(); 
					OnMouseMove( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
					return FALSE;
				}
				else
				{
					m_bEditingModeEnabled = true;
					SendMessage( Constants::sc_nMessageTrackTextBegin, GetCurrentPointIndex() );
					SplitString();
					m_SelectStart = m_SelectEnd = m_CurrPos;
					OnLButtonDown( GET_X_LPARAM( lParam ), GET_Y_LPARAM( lParam ) );
				}
			}
			
			// For East Asian languages such as Chinese, Japanese, Korean, and other languages with complex characters. 
			// - Input Method Editors (IME) 

			WCHAR* pIMEUI	=	NULL;
			LONG IMEUIBytes	=	0;

			if ( TRUE == m_oIMEUIController.ProcessMessage ( nMessage, wParam, lParam, &pIMEUI, IMEUIBytes ) )
			{
				CStringW PastleStr	=	CStringW ( pIMEUI );
				delete [] pIMEUI;

				m_String.Insert ( m_CurrPos, PastleStr );
				
				m_CurrPos		=	m_SelectEnd + PastleStr.GetLength();
				m_SelectEnd		=	m_SelectStart	=	m_CurrPos;
				
				SplitString ( );

				SendMessage ( Constants::sc_nMessageTrackTextChange, GetCurrentPointIndex() );

				return FALSE;
			}

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ( nTrackIndex == 9 || (nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else if ( nTrackIndex >= 0 && nTrackIndex < 8 && !m_AutoSize )
			{
				nPointStatus = c_nTrackPointStatusPoint;
				nChangeStatus = c_nTrackChangeStatusSize;
			
				if ( m_bResizeShapeToFitText )
				{
					nPointStatus	=	c_nTrackPointStatusNone;
					nChangeStatus	=	c_nTrackChangeStatusNone;
				}
			}
			else if (nTrackIndex == 8)
			{
				nPointStatus = c_nTrackPointStatusRotation;
				nChangeStatus = c_nTrackChangeStatusRotation;
			}
			else
			{
				nPointStatus = c_nTrackPointStatusNone;
				nChangeStatus = c_nTrackChangeStatusNone;
			}

			if ( m_bBlockResize )
			{
				if ( nTrackIndex >= 0 && nTrackIndex < 8 )
				{
					nPointStatus = c_nTrackPointStatusNone;
					nChangeStatus = c_nTrackChangeStatusNone;
				}
			}

			if( PosOnMouse( (int)MapToDataX ( nX ), (int)MapToDataY( nY ) ) != -1 && m_bEditingModeEnabled && !m_IsTransfom )
				nPointStatus = c_nTrackPointStatusIBeam;
			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			double Angle	=	Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
			
			POINT selectInfo;
			selectInfo.x	=	m_SelectStart;
			selectInfo.y	=	m_SelectEnd;

			GetZoom ( this );
			
			if ( m_hDC != oPainter.GetDC() )
			{
				m_hDC		=	oPainter.GetDC();
			}

			if ( ( m_AutoSize && m_IsFirstResize ) || ( m_bResizeShapeToFitText && m_IsFirstResize ) )
			{
				SplitString ( );
				UpdateResizeShapeToFitText	( );

				m_IsFirstResize		=	false;
			
				// update tracking point coordinates
				MapByDouble ( m_rectData.left, m_rectData.top, m_rectData.right, m_rectData.bottom, m_rectWindow.left, m_rectWindow.top, m_rectWindow.right, m_rectWindow.bottom );
			}

			oPainter.DrawEditTextRotatableW ( this, Angle, m_dZoom, m_arrStrings, (WCHAR*)m_String.GetString(), m_CurrPos, 
				selectInfo, m_FontName, m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText, m_bEnableDrawText );
			
			/*
			
			BSTR string   = m_String.AllocSysString();
			BSTR fontName = m_FontName.AllocSysString();
			double Angle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );
			POINT selectInfo;
			selectInfo.x = m_SelectStart;
			selectInfo.y = m_SelectEnd;
			GetZoom( this );
			if( m_hDC != oPainter.GetDC() )
				m_hDC = oPainter.GetDC();

			oPainter.DrawEditTextRotatable( this, Angle, m_dZoom, m_arrStrings, string, m_CurrPos, 
				selectInfo, fontName, m_FontSize, m_FontType, m_FontColor, m_bEditingModeEnabled, m_ScaleText );
			
			if( m_AutoSize && m_IsFirstResize )
			{
				SplitString();
				m_IsFirstResize = false;
			}
			SysFreeString( string ); 			
			SysFreeString( fontName );
			
			*/
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			if ( m_bBlockResize )
				return;

			if ( m_bPaintPoints )
			{
				if ( !m_AutoSize )
					oPainter.DrawPoints ( this, GetCount() - 3, c_nTrackPointSizePaint );
			}
		}

		inline void GetZoom( Tracking::CTrackPoints* pTrack )
		{
			if (!pTrack || pTrack->GetCount() < 1 ) return;

			POINT* pPoints = new POINT[10];
			pTrack->GetPointArray( pPoints, 10 );

			Structures::RECTD rectD;

			double dAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;

			double tX, tY;
			double cosA = cos( -dAngle );
			double sinA = sin( -dAngle );

			rectD.left   = (pPoints + 0)->x;
			rectD.top    = (pPoints + 0)->y;
			rectD.right  = (pPoints + 2)->x;
			rectD.bottom = (pPoints + 2)->y;

			tX = (pPoints + 0)->x - (pPoints + 9)->x;
			tY = (pPoints + 0)->y - (pPoints + 9)->y;				
			rectD.left  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.top   = (pPoints + 9)->y + tX * sinA + tY * cosA;	

			tX = (pPoints + 2)->x - (pPoints + 9)->x;
			tY = (pPoints + 2)->y - (pPoints + 9)->y;
			rectD.right  = (pPoints + 9)->x + tX * cosA - tY * sinA;
			rectD.bottom = (pPoints + 9)->y + tX * sinA + tY * cosA;

			if( rectD.right < rectD.left ) 
			{ 
				double tmpRight		= rectD.right; 
				rectD.right			= rectD.left;
				rectD.left			= tmpRight;
			}
			if( rectD.top > rectD.bottom ) 
			{ 
				double tmpBottom	= rectD.bottom; 
				rectD.bottom		= rectD.top;
				rectD.top			= tmpBottom;
			}		

			Structures::RECTD rectDTracking = GetUnrotatableRect( dAngle );

			if( rectDTracking.GetWidth() > 0 )
				m_dZoom = rectD.GetWidth() / rectDTracking.GetWidth();

			if( pPoints )
			{
				delete []pPoints;
				pPoints = NULL;
			}
		}

		inline void EnableBlockEditing ( bool Value )
		{
			m_bBlockEditing		=	Value;
		}

		inline void EnableBlockResize ( bool Value )
		{
			m_bBlockResize		=	Value;
		}

	public:
		
		inline void Create	( double dLeft, double dTop, double dRight, double dBottom, double dAngle, double dStickAngle, WCHAR* string, WCHAR* fontName, int fontSize, int fontType, int FontColor, bool bBlockEditing = false, bool bBlockResize = false )
		{
			m_String   		= CStringW ( string );
			m_FontName 		= CStringW ( fontName );

			m_CurrPos  		= m_String.GetLength();
			m_FontSize 		= fontSize;
			m_FontType		= fontType;
			m_FontColor		= FontColor;
			m_bEditingModeEnabled		= false;
			m_IsSelect		= false;
			m_IsTransfom	= true;
			m_SelectStart	= 0;
			m_SelectEnd		= 0;
			m_hDC			= NULL;
			m_dZoom			= 1.0;
			m_StickAngle	= ( dStickAngle < 0 || dStickAngle > 360 ) ? 0.0 : rad( dStickAngle );
			m_bBlockEditing	=	bBlockEditing;
			m_bBlockResize	=	bBlockResize;
			
			//Gdiplus::FontFamily	oFontFamily	( L"Berlin Sans FB Demi" );
			//Gdiplus::Font			oFont		( &oFontFamily, (Gdiplus::REAL)m_FontSize, 0, Gdiplus::UnitPixel );
			//Gdiplus::Font			oFont		( L"Berlin Sans FB Demi", (Gdiplus::REAL)m_FontSize, 0, Gdiplus::UnitPixel );

			if ( dRight < 0 && dBottom < 0 ) 
				m_AutoSize = true;
			else					
				m_AutoSize = false;

			//m_ScaleText = (bool)GetFontScale( m_FontType );
			if ( GetFontScale( m_FontType ) != 0 )
				m_ScaleText = true;
			else
				m_ScaleText = false;

			m_Scale.x = 1.0;
			m_Scale.y = 1.0;

			switch ( GetFontAlignedH ( m_FontType ) )
			{
			case ALIGNED_TOP:
				m_FontAlignW = byTop;
				break;
			case ALIGNED_MIDDLE:
				m_FontAlignW = byMiddle;
				break;
			case ALIGNED_BOTTOM:
				m_FontAlignW = byBottom;
				break;
			default:
				m_FontAlignW = byTop;
			}

			// if create by mouse
			if ( abs( dRight - dLeft ) < 1 || abs( dBottom - dTop ) < 1 || m_AutoSize )
			{
				m_IsFirstResize = true;
			}
			else		
			{
				m_IsFirstResize = false;
			}

			if ( m_bResizeShapeToFitText )
			{
				m_IsFirstResize	=	true;
			}

			SetType( Constants::sc_nTrackTypeTextRotatable );

			Clear();

			if ( m_AutoSize )
			{
				dRight	=	dLeft + fontSize;
				dBottom	=	dTop + fontSize;
			}

			Add ( dLeft,  dTop );																//	0
			Add ( dRight, dTop );																//	1
			Add ( dRight, dBottom );															//	2
			Add ( dLeft,  dBottom );															//	3

			Add ( 0.5 * ( dLeft + dRight ), dTop );												//	4
			Add ( dRight, 0.5 * ( dTop + dBottom ) );											//	5
			Add ( 0.5 * ( dLeft + dRight ), dBottom );											//	6
			Add ( dLeft, 0.5 * ( dTop + dBottom ) );											//	7

			Add ( 0.5 * ( dLeft + dRight ), dTop - 30 );				//	rotate selector		//	8
			Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );	//	center				//	9

			Add ( 0.5 * ( dLeft + dRight ), 0.5 * ( dTop + dBottom ) );							//	10

			SetCurrentPointIndex(-1);

			RotateToAlpha( rad( dAngle ) );

			SplitString ( );
		}
		
		inline double	GetStickAngle () 
		{
			return m_StickAngle; 
		}

		//	весь текст в трекинге рисуется, форматирование по горизонтали, трек подстраивает свою высоту по ширине текста
		inline void		SetResizeShapeToFitText		( bool Value )
		{
			m_bResizeShapeToFitText		=	Value;
			m_bPaintPoints				=	!m_bResizeShapeToFitText;
		}

		inline void		UpdateResizeShapeToFitText	( )
		{
			if ( NULL == m_hDC )
				return;

			if ( m_bResizeShapeToFitText )
			{
				if ( ALIGNED_TOP == m_FontAlignW )
				{
					UpdateAllignedTopRSTFT ();
				}

				if ( ALIGNED_MIDDLE == m_FontAlignW )
				{
					UpdateAllignedMiddleRSTFT ();
				}

				if ( ALIGNED_BOTTOM == m_FontAlignW )
				{
					UpdateAllignedBottomRSTFT ();
				}
			}
		}

		inline void		SetPaintPoints				( bool Value )
		{
			m_bPaintPoints				=	Value;
		}

		// операции с выделениями
		inline void		SelectTextAll				( )
		{
			//m_IsSelect		=	true;
			m_CurrPos		=	m_String.GetLength ();

			m_SelectStart	=	0;
			m_SelectEnd		=	m_CurrPos;
		}

		// 
		inline bool		UpdateAllignedTopRSTFT		( )
		{
			const double cn_StickPointOffsetY		=	30.0;

			int StringsCount	=	__max ( 1, m_arrStrings.GetSize () );
			int StrCount		=	1;

			for ( int i = 0; i < m_String.GetLength(); ++i )
			{
				if ( m_String [ i ] == _T('\r') )
				{
					++StrCount;
					
					continue;
				}
			}

			StringsCount		=	__max ( StrCount, StringsCount );

			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily ) )
				return false;

			double FontHeight	=	Rendering::Utils::GetFontHeight ( pFont );

			double AngleRot		=	Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
			double SizeRotX		=	0.0;
			double SizeRotY		=	0.0;

			double BoundWidth	=	Geometry::GetLength ( (double)GetAt(0).nX, (double)GetAt(0).nY, (double)GetAt(1).nX, (double)GetAt(1).nY );
			double BoundHeight	=	__max ( 1.0, (double) ( StringsCount ) * FontHeight);

			double AX			=	( GetAt(4).nX - GetAt(0).nX ) / BoundWidth	* 0.5;
			double AY 			=	( GetAt(4).nY - GetAt(0).nY ) / BoundHeight * 0.5;
			double BX 			=	( GetAt(4).nX - GetAt(9).nX ) / BoundWidth	* 0.5;
			double BY 			=	( GetAt(4).nY - GetAt(9).nY ) / BoundHeight * 0.5;
			
			if ( AX * BY - AY * BX <= 0 )
			{
				double MaxTextWidth	=	GetMaxWidthTrackText ( *pFont );

				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	0.0;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(1).Create ( GetAt(0).dX + SizeRotX, GetAt(0).dY + SizeRotY );
				}
				
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(2).Create ( GetAt(0).dX + SizeRotX, GetAt(0).dY + SizeRotY );
				}
			
				{
					SizeRotX	=	0.0;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(3).Create ( GetAt(0).dX + SizeRotX, GetAt(0).dY + SizeRotY );
				}

				GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
				GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
				GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
				GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );

				GetAt(9).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
			
				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}
			}
			else
			{
				double MaxTextWidth	=	GetMaxWidthTrackText ( *pFont );

				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	0.0;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(0).Create ( GetAt(1).dX + SizeRotX, GetAt(1).dY + SizeRotY );
				}
			
				{
					SizeRotX	=	0.0;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(2).Create ( GetAt(1).dX + SizeRotX, GetAt(1).dY + SizeRotY );
				}
				
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(3).Create ( GetAt(1).dX + SizeRotX, GetAt(1).dY + SizeRotY );
				}
			
				GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
				GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
				GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
				GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );

				GetAt(9).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
			
				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}
			}
			
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

			return true;
		}

		inline bool		UpdateAllignedMiddleRSTFT	( )
		{
			const double cn_StickPointOffsetY		=	30.0;

			int StringsCount	=	__max ( 1, m_arrStrings.GetSize () );

			int StrCount		=	1;

			for ( int i = 0; i < m_String.GetLength(); ++i )
			{
				if ( m_String [ i ] == _T('\r') )
				{
					++StrCount;
					
					continue;
				}
			}

			StringsCount		=	__max ( StrCount, StringsCount );
		
			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily ) )
				return false;

			double FontHeight	=	Rendering::Utils::GetFontHeight ( pFont );

			static const double M_PI_H2		=	M_PI / 2.0;

			double AngleRot		=	Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_H2;
			double SizeRotX		=	0.0;
			double SizeRotY		=	0.0;

			double BoundWidth	=	Geometry::GetLength ( (double)GetAt(0).nX, (double)GetAt(0).nY, (double)GetAt(1).nX, (double)GetAt(1).nY );
			double BoundHeight	=	__max ( 1.0, (double) ( StringsCount ) * FontHeight );

			double AX			=	( GetAt(4).nX - GetAt(0).nX ) / BoundWidth	* 0.5;
			double AY 			=	( GetAt(4).nY - GetAt(0).nY ) / BoundHeight * 0.5;
			double BX 			=	( GetAt(4).nX - GetAt(9).nX ) / BoundWidth	* 0.5;
			double BY 			=	( GetAt(4).nY - GetAt(9).nY ) / BoundHeight * 0.5;
			
			if ( AX * BY - AY * BX <= 0 )
			{
				{
					BoundWidth	=	GetMaxWidthTrackText ( *pFont );	// Geometry::GetLength ( (double)GetAt(0).dX, (double)GetAt(0).dY, (double)GetAt(1).dX, (double)GetAt(1).dY );
			
					SizeRotX	=	BoundWidth * 0.5;
					SizeRotY	=	BoundHeight * 0.5;

					GetAt(0).Create ( GetAt(9).dX - SizeRotX, GetAt(9).dY - SizeRotY );
					GetAt(1).Create ( GetAt(9).dX + SizeRotX, GetAt(9).dY - SizeRotY );
					GetAt(2).Create ( GetAt(9).dX + SizeRotX, GetAt(9).dY + SizeRotY );
					GetAt(3).Create ( GetAt(9).dX - SizeRotX, GetAt(9).dY + SizeRotY );

					GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
					GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
					GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
					GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );
				}

				BoundWidth	=	Geometry::GetLength ( (double)GetAt(0).nX, (double)GetAt(0).nY, (double)GetAt(1).nX, (double)GetAt(1).nY );

				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}

				RotateToAlpha ( AngleRot );
			}
			else
			{
				{
					BoundWidth	=	GetMaxWidthTrackText ( *pFont );	// Geometry::GetLength ( (double)GetAt(0).dX, (double)GetAt(0).dY, (double)GetAt(1).dX, (double)GetAt(1).dY );
			
					SizeRotX	=	BoundWidth * 0.5;
					SizeRotY	=	BoundHeight * 0.5;

					GetAt(0).Create ( GetAt(9).dX + SizeRotX, GetAt(9).dY - SizeRotY );
					GetAt(1).Create ( GetAt(9).dX - SizeRotX, GetAt(9).dY - SizeRotY );
					GetAt(2).Create ( GetAt(9).dX - SizeRotX, GetAt(9).dY + SizeRotY );
					GetAt(3).Create ( GetAt(9).dX + SizeRotX, GetAt(9).dY + SizeRotY );

					GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
					GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
					GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
					GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );
				}

				BoundWidth	=	Geometry::GetLength ( (double)GetAt(0).nX, (double)GetAt(0).nY, (double)GetAt(1).nX, (double)GetAt(1).nY );

				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}

				RotateToAlpha ( AngleRot );
			}
		
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

			return true;
		}
		inline bool		UpdateAllignedBottomRSTFT	( )
		{
			const double cn_StickPointOffsetY		=	30.0;

			int StringsCount	=	__max ( 1, m_arrStrings.GetSize () );
			int StrCount		=	1;

			for ( int i = 0; i < m_String.GetLength(); ++i )
			{
				if ( m_String [ i ] == _T('\r') )
				{
					++StrCount;
					
					continue;
				}
			}

			StringsCount		=	__max ( StrCount, StringsCount );

			Gdiplus::Font* pFont				=	NULL;
			Gdiplus::FontFamily* pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily ) )
				return false;

			double FontHeight	=	Rendering::Utils::GetFontHeight ( pFont );

			double AngleRot		=	Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
			double SizeRotX		=	0.0;
			double SizeRotY		=	0.0;

			double BoundWidth	=	Geometry::GetLength ( (double)GetAt(0).nX, (double)GetAt(0).nY, (double)GetAt(1).nX, (double)GetAt(1).nY );
			double BoundHeight	=	__max ( 1.0, (double) ( StringsCount ) * FontHeight );

			double AX			=	( GetAt(4).nX - GetAt(0).nX ) / BoundWidth	* 0.5;
			double AY 			=	( GetAt(4).nY - GetAt(0).nY ) / BoundHeight * 0.5;
			double BX 			=	( GetAt(4).nX - GetAt(9).nX ) / BoundWidth	* 0.5;
			double BY 			=	( GetAt(4).nY - GetAt(9).nY ) / BoundHeight * 0.5;

			if ( AX * BY - AY * BX <= 0 )
			{
				double MaxTextWidth	=	GetMaxWidthTrackText ( *pFont );
			
				{
					SizeRotX	=	0.0;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(0).Create ( GetAt(3).dX - SizeRotX, GetAt(3).dY - SizeRotY );
				}
			
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	0.0;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(1).Create ( GetAt(0).dX + SizeRotX, GetAt(0).dY + SizeRotY );
				}
				
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(2).Create ( GetAt(0).dX + SizeRotX, GetAt(0).dY + SizeRotY );
				}

				GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
				GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
				GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
				GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );

				GetAt(9).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
			
				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}
			}
			else
			{
				double MaxTextWidth	=	GetMaxWidthTrackText ( *pFont );
			
				{
					SizeRotX	=	0.0;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(1).Create ( GetAt(2).dX - SizeRotX, GetAt(2).dY - SizeRotY );
				}
			
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	0.0;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );

					GetAt(0).Create ( GetAt(1).dX + SizeRotX, GetAt(1).dY + SizeRotY );
				}
				
				{
					SizeRotX	=	MaxTextWidth;
					SizeRotY	=	BoundHeight;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(3).Create ( GetAt(1).dX + SizeRotX, GetAt(1).dY + SizeRotY );
				}

				GetAt(4).Create ( ( GetAt(0).dX + GetAt(1).dX ) * 0.5, ( GetAt(0).dY + GetAt(1).dY ) * 0.5 );
				GetAt(5).Create ( ( GetAt(1).dX + GetAt(2).dX ) * 0.5, ( GetAt(1).dY + GetAt(2).dY ) * 0.5 );
				GetAt(6).Create ( ( GetAt(3).dX + GetAt(2).dX ) * 0.5, ( GetAt(3).dY + GetAt(2).dY ) * 0.5 );
				GetAt(7).Create ( ( GetAt(0).dX + GetAt(3).dX ) * 0.5, ( GetAt(0).dY + GetAt(3).dY ) * 0.5 );

				GetAt(9).Create ( ( GetAt(0).dX + GetAt(2).dX ) * 0.5, ( GetAt(0).dY + GetAt(2).dY ) * 0.5 );
			
				{
					SizeRotX	=	0;
					SizeRotY	=	cn_StickPointOffsetY;

					Geometry::RotatePoint ( SizeRotX, SizeRotY, 0.0, 0.0, AngleRot );
				
					GetAt(8).Create ( GetAt(4).dX - SizeRotX, GetAt(4).dY - SizeRotY );
				}
			}
			
			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

			return true;
		}

		//	в конце есть пустая строка ( переход каретки )
		inline bool		IsLastLineEmpty				( )
		{	
			if ( m_String.GetLength () > 2 )
			{
#ifdef _DEBUG
				WCHAR EndOff	=	m_String [ m_String.GetLength () - 1 ];
				WCHAR EndOffOff	=	m_String [ m_String.GetLength () - 2 ];
#endif
				if ( m_String [ m_String.GetLength () - 2 ] == L'\r' &&  m_String [ m_String.GetLength () - 1 ] == L'\n' )
				{
					return true;
				}
			}

			return false;
		}
	
		//	ширина трекинга максиум по всем строчкам, иначе ограничение на размер по ширине буквы шрифта
		inline double	GetMaxWidthTrackText ( Gdiplus::Font& oFont )
		{
			double MaxWidth	=	(double)m_FontSize * (double)GetDeviceCaps ( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
			
			Graphics oGraphics ( m_hDC );

			Gdiplus::StringFormat	oStringFormat;
			oStringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
			
			Gdiplus::RectF	LayoutRect, BoundingBox;

			for ( int i = 0; i < m_arrStrings.GetSize (); ++i )
			{
				if ( Gdiplus::Ok == oGraphics.MeasureString ( m_arrStrings[i], -1, &oFont, LayoutRect, &oStringFormat, &BoundingBox ) )
				{
					MaxWidth	=	__max ( MaxWidth, BoundingBox.Width );
				}
			}

			return	MaxWidth;
		}

		
		inline double	GetCaretX ()
		{
			if ( NULL == m_hDC )
				return 0.0;

			int CountLines = m_arrStrings.GetSize();
			if ( 0 == CountLines ) 
				return 0.0;
		
			CTimeMeasurer oTimer;
			oTimer.Reset ();

			double Angle					=	Geometry::GetAngle ( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI_2;
			Structures::RECTD TrackBound	=	GetUnrotatableRect ( Angle );

			int FontAlignedH				=	GetFontAlignedH ( m_FontType );
			
			Gdiplus::Graphics graphics( m_hDC );

			Gdiplus::Font*			pFont		=	NULL;
			Gdiplus::FontFamily*	pFontFamily	=	NULL;

			if ( false == GetFont ( pFont, pFontFamily ) )
				return 0.0;

			Gdiplus::StringFormat	oStringFormat ( Gdiplus::StringAlignmentNear );
			oStringFormat.SetFormatFlags ( StringFormatFlagsMeasureTrailingSpaces );
   
			if ( false == m_ScaleText )
			{
				oStringFormat.SetAlignment ( (StringAlignment)GetFontAlignedW ( m_FontType ) );
			}
			
			Gdiplus::RectF oTextBounding;
			Gdiplus::RectF oLayoutRect ( 0.0f, 0.0f, 65535.0f, 65535.0f );

			double FontHeight	=	Rendering::Utils::GetFontHeight( pFont );
			double LinesOffSetY	=	CountLines * FontHeight;

			int Symbols			=	0;

			double OffSetX		=	0.0;
			double OffSetY		=	0.0;

			for ( int i = 0; i < CountLines; ++i )
			{
				if ( Symbols <= m_CurrPos && m_CurrPos < ( Symbols + m_arrStrings[i].GetLength() ) )		// Определяем строку в которой находится каретка
				{
					int LineY	=	i;
					int LineX	=	m_CurrPos - Symbols;

					// смещение по вертикали 
					if ( ALIGNED_TOP == FontAlignedH )
					{
						OffSetY		=	(double)i * FontHeight + FontHeight * 0.5;
					}

					if ( ALIGNED_MIDDLE == FontAlignedH )
					{
						OffSetY		=	(double)i * FontHeight + TrackBound.GetHeight() * 0.5 - LinesOffSetY * 0.5  + FontHeight * 0.5;
					}

					if ( ALIGNED_BOTTOM == FontAlignedH )
					{
						OffSetY		=	(double)i * FontHeight + TrackBound.GetHeight() - LinesOffSetY  + FontHeight * 0.5;
					}

					Gdiplus::StringFormat oDefStringFormat;
					oDefStringFormat.SetAlignment ( StringAlignmentNear );
					
					Gdiplus::RectF CharBound;
					Gdiplus::RectF StringBound ( 0.0f, 0.0f, 65535.0f, 65535.0f );

					int CharsCount	=	m_arrStrings[i].GetLength();
					
					// относительное расстояние до каретки
					for ( int j = 0; j < CharsCount && j <= LineX; ++j )
					{
						GetRectCurrentCharacter ( &graphics, &oDefStringFormat, pFont, (WCHAR*)m_arrStrings[i].GetString(), -1, j, StringBound, CharBound );

						OffSetX		=	CharBound.X;	// + CharBound.Width;
					}	

					// смещение по горизонтали 
					//if ( StringAlignmentNear == oStringFormat.GetAlignment() )
					//{
					//}

					if ( StringAlignmentCenter == oStringFormat.GetAlignment() )
					{
						graphics.MeasureString ( m_arrStrings[i], -1, pFont, oLayoutRect, &oStringFormat, &oTextBounding );	
						
						OffSetX		=	CharBound.X + TrackBound.GetWidth() * 0.5 - oTextBounding.Width * 0.5;	// + CharBound.Width;
					}

					if ( StringAlignmentFar == oStringFormat.GetAlignment() )
					{
						graphics.MeasureString ( m_arrStrings[i], -1, pFont, oLayoutRect, &oStringFormat, &oTextBounding );	
						
						OffSetX		=	CharBound.X + TrackBound.GetWidth() - oTextBounding.Width;	// + CharBound.Width;
					}

#ifdef _DEBUG
					// ATLTRACE ( _T("OffSetX : %f, OffSetY : %f, Time : %f \n"), OffSetX, OffSetY, oTimer.GetTimeInterval() );
#endif
					double DX0	=	OffSetX - TrackBound.GetWidth() * 0.5;
					double DY0	=	OffSetY - TrackBound.GetHeight() * 0.5;

					Geometry::RotatePoint ( DX0, DY0, 0.0, 0.0, Angle );
					
					m_nCaretX	=	TrackBound.left	+ TrackBound.GetWidth()	* 0.5	+	DX0;
					m_nCaretY	=	TrackBound.top	+ TrackBound.GetHeight() * 0.5	+	DY0;
			
					RELEASEOBJECT ( pFont );
					RELEASEOBJECT ( pFontFamily );
#ifdef _DEBUG
					ATLTRACE ( _T("CaretX : %f "), m_nCaretX );
#endif

					return m_nCaretX;
				}

				Symbols	+=	m_arrStrings[i].GetLength();
			}

			RELEASEOBJECT ( pFont );
			RELEASEOBJECT ( pFontFamily );

#ifdef _DEBUG
			ATLTRACE ( _T("CaretX : %f "), 0.0 );
#endif			
			return 0.0;				
		}

		inline double	GetCaretY ()
		{
#ifdef _DEBUG
			ATLTRACE ( _T("CaretY : %f \n"), m_nCaretY );
#endif
			return m_nCaretY;
		}

		// get rect of current(index) character
		inline void		GetRectCurrentCharacter ( Gdiplus::Graphics* graphics, Gdiplus::StringFormat* stringFormat, Gdiplus::Font* font, WCHAR* pText, int length, int index, Gdiplus::RectF& rectString, Gdiplus::RectF& rectCharacter)
		{
			CharacterRange charRanges[1] = {CharacterRange(index, 1)};
			Region pCharRangeRegions;
			stringFormat->SetMeasurableCharacterRanges(1, charRanges);
			graphics->MeasureCharacterRanges(pText, length, font, rectString, stringFormat, 1, &pCharRangeRegions);

			int count = 1;
			Matrix matrix;
			pCharRangeRegions.GetRegionScans(&matrix, &rectCharacter, &count);
		}


	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if ( !m_IsTransfom )
			{
				int tmpPos = PosOnMouse ( (int)dX, (int)dY );
				
				// ATLTRACE ( L"MOUSE : %d, %d POINT %d\n",(int)dX, (int)dY , tmpPos);

				if( tmpPos != -1 && m_bEditingModeEnabled )
				{
					if( m_IsSelect && !(GetAsyncKeyState( VK_SHIFT ) < 0) ) 
					{
						m_CurrPos	= tmpPos;
						m_SelectEnd = tmpPos;
					}
					if ( false == IsTrackChild () )
						SendMessage( Constants::sc_nMessageTrackInvalidate, GetCurrentPointIndex() );

					return false;
				}
			}

			if (!IsCurrentPointValid())
				return false;

			int nCurrentPoint = m_IsFirstResize ? 2 : GetCurrentPointIndex();

			if (nCurrentPoint == 9 || nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				if ( m_bResizeShapeToFitText )
				{
					if ( nCurrentPoint != 8 )
						return false;
				}

				// correct rectangle points
				double dx, dy;
				double eX1, eX2, eY1, eY2;

				double CharSize		=	(double)m_FontSize * (double)GetDeviceCaps( GetWindowDC (NULL), LOGPIXELSY ) / 72.0;
				double CharSizeRotX	=	CharSize;
				double CharSizeRotY	=	CharSize;
			
				if (nCurrentPoint <= 8)
				{	
					eX1 = 1.0; eY1 = 0.0;
					eX2 = 0.0; eY2 = 1.0;

					double mAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY ) + M_PI / 2.0;
					Geometry::RotatePoint( eX1, eY1, 0.0, 0.0, mAngle );
					Geometry::RotatePoint( eX2, eY2, 0.0, 0.0, mAngle );
				
					Geometry::RotatePoint ( CharSizeRotX, CharSizeRotY, 0.0, 0.0, mAngle );
				}
				
				if (nCurrentPoint == 0)
				{
					dX -= GetAt(0).dX;
					dY -= GetAt(0).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(0).dX += dX;
					GetAt(0).dY += dY;
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;
				}
				else if (nCurrentPoint == 1)
				{
					dX -= GetAt(1).dX;
					dY -= GetAt(1).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(1).dX += dX;
					GetAt(1).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;

				}
				else if (nCurrentPoint == 2)
				{
					dX -= GetAt(2).dX;
					dY -= GetAt(2).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(2).dX += dX;
					GetAt(2).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 3)
				{
					dX -= GetAt(3).dX;
					dY -= GetAt(3).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(3).dX += dX;
					GetAt(3).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy;

				}
				else if (nCurrentPoint == 4)
				{
					dX -= GetAt(4).dX;
					dY -= GetAt(4).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(4).dX += dX;
					GetAt(4).dY += dY;
					GetAt(0).dX += eX2*dy;
					GetAt(0).dY += eY2*dy; 
					GetAt(1).dX += eX2*dy;
					GetAt(1).dY += eY2*dy;

				}
				else if (nCurrentPoint == 5)
				{
					dX -= GetAt(5).dX;
					dY -= GetAt(5).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(5).dX += dX;
					GetAt(5).dY += dY;
					GetAt(1).dX += eX1*dx;
					GetAt(1).dY += eY1*dx;
					GetAt(2).dX += eX1*dx;
					GetAt(2).dY += eY1*dx;					
				}
				else if (nCurrentPoint == 6)
				{
					dX -= GetAt(6).dX;
					dY -= GetAt(6).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(6).dX += dX;
					GetAt(6).dY += dY;
					GetAt(2).dX += eX2*dy;
					GetAt(2).dY += eY2*dy; 
					GetAt(3).dX += eX2*dy;
					GetAt(3).dY += eY2*dy;
				}
				else if (nCurrentPoint == 7)
				{
					dX -= GetAt(7).dX;
					dY -= GetAt(7).dY;

					dx = dX*eX1 + dY*eY1;
					dy = dX*eX2 + dY*eY2;

					GetAt(7).dX += dX;
					GetAt(7).dY += dY;
					GetAt(0).dX += eX1*dx;
					GetAt(0).dY += eY1*dx;
					GetAt(3).dX += eX1*dx;
					GetAt(3).dY += eY1*dx;	
				}				
				else if (nCurrentPoint == 8)
				{
					double baseAngle = Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, dX, dY );
					StickAngle( baseAngle, m_StickAngle );
					double mAngle	 = baseAngle - Geometry::GetAngle( GetAt(9).dX, GetAt(9).dY, GetAt(8).dX, GetAt(8).dY );

					RotateToAlpha( mAngle );
				}

				
				UpdateMinimizeRegion ( nCurrentPoint );

				// recompute centers
				if (nCurrentPoint >= 0 && nCurrentPoint < 8)
				{
					GetAt(4).Create( 0.5*( GetAt(0).dX + GetAt(1).dX ), 0.5*( GetAt(1).dY + GetAt(0).dY ) );
					GetAt(5).Create( 0.5*( GetAt(1).dX + GetAt(2).dX ), 0.5*( GetAt(1).dY + GetAt(2).dY ) );
					GetAt(6).Create( 0.5*( GetAt(3).dX + GetAt(2).dX ), 0.5*( GetAt(3).dY + GetAt(2).dY ) );
					GetAt(7).Create( 0.5*( GetAt(3).dX + GetAt(0).dX ), 0.5*( GetAt(3).dY + GetAt(0).dY ) );

					double vX = GetAt(4).dX - GetAt(6).dX;
					double vY = GetAt(4).dY - GetAt(6).dY;

					double length = Geometry::GetLength( GetAt(4).dX, GetAt(4).dY,GetAt(6).dX, GetAt(6).dY );
					if( length < 1 )
					{
						vX = -eX2; 
						vY = -eY2;
					}
					else
					{
						vX /= length;
						vY /= length;
					}

					GetAt(8).dX = GetAt(4).dX + vX * 30;
					GetAt(8).dY = GetAt(4).dY + vY * 30;
					GetAt(9).Create(0.5*(GetAt(0).dX + GetAt(2).dX), 0.5*( GetAt(2).dY + GetAt(0).dY ) );

					SplitString();				
				}
				
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int tmpPos = PosOnMouse( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
			if ( tmpPos != -1 && m_bEditingModeEnabled ) 
			{
				if( GetAsyncKeyState( VK_SHIFT ) < 0 )
				{
					m_CurrPos	= tmpPos;
					m_SelectEnd	= tmpPos;
				}
				else
				{
					m_CurrPos		= tmpPos;
					m_SelectStart	= tmpPos;
				}
				m_IsSelect		= true;
				m_IsTransfom	= false;

				// it's for repaint
				OnMouseMove( MouseX, MouseY );
				return true;
			}

			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;

			if( nTrackIndex >= 0 && nTrackIndex <= 9 )
				m_IsTransfom = true;

			if( nTrackIndex == GetCount() - 1 )
				GetAt(nTrackIndex).Create( MapToDataX( MouseX ), MapToDataY( MouseY ) );

			if( -1 == GetPointUnderCursor ( MouseX, MouseY ) )
				EndEditText();

			if ( m_bBlockResize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			if( m_AutoSize && nTrackIndex >=0 && nTrackIndex < 8 )
				nTrackIndex = -1;

			SetCurrentPointIndex( nTrackIndex );

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			m_IsFirstResize = false;

			if( GetAsyncKeyState( VK_SHIFT ) >= 0 ) 
			{
				if( m_IsSelect )
				{
					int tmpPos = PosOnMouse( (int)MapToDataX( MouseX ), (int)MapToDataY( MouseY ) );
					if( tmpPos != -1 && m_bEditingModeEnabled ) 
					{
						m_CurrPos		= tmpPos;
						m_SelectEnd 	= tmpPos;
					}
					m_IsSelect		= false;
				}
			}
			if (!IsCurrentPointValid())
				return false;

			if ( m_bResizeShapeToFitText )
			{
				if ( 8 != m_nCurrentPoint )
				{
					UpdateResizeShapeToFitText	( );
				}
			}

			if ( false == IsTrackChild () )
				SendMessage( Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex() );

			SetCurrentPointIndex(-1);

			m_IsTransfom = false;

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	private:
		HDC						m_hDC;
		double					m_dZoom;
		bool					m_IsFirstResize;
		bool					m_AutoSize;
		bool					m_ScaleText;

		Structures::POINTD		m_Scale;
		bool					m_bBlockEditing;
		bool					m_bBlockResize;

	protected:
	
		double					m_StickAngle;

		bool					m_bResizeShapeToFitText;	//	весь текст в трекинге рисуется, форматирование по горизонтали, трек подстраивает свою высоту по ширине текста
		bool					m_bPaintPoints;

		double					m_nCaretX;					// Позиция каретки
		double					m_nCaretY;
	};//
	//Rectangle Fixed Aspect
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectRectangleFixedSize : public CTrackObject
	{
	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = -1;
		//	Structures::RECTI rectBounds = GetIntegerBoundingRect();
		//	Structures::POINTI pointTrack(nX, nY);

		//	if (rectBounds.IsPointInside(pointTrack))
		//	{
		//		nTrackIndex = GetCount() - 1;

		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));
		//	}

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	double dDeltaX = dX - GetCurrentPoint().dX;
		//	double dDeltaY = dY - GetCurrentPoint().dY;

		//	for (int index = 0; index < GetCurrentPointIndex(); ++index)
		//	{
		//		GetAt(index).dX += dDeltaX;
		//		GetAt(index).dY += dDeltaY;							
		//	}

		//	GetCurrentPoint().Create(dX, dY);

		//	SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			Structures::RECTI rectBounds = GetIntegerBoundingRect();
			Structures::POINTI pointTrack(nX, nY);

			if (rectBounds.IsPointInside(pointTrack))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangle(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
		}

		virtual int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = -1;
			
			Structures::RECTI rectBounds = GetIntegerBoundingRect();
			Structures::POINTI pointTrack(nX, nY);

			if (rectBounds.IsPointInside(pointTrack))
			{
				nTrackIndex = GetCount() - 1;
				return nTrackIndex;
			}

			return -1;
		}	

	public:

		void Create(double dLeft, double dTop, double dRight, double dBottom)
		{
			SetType(Constants::sc_nTrackTypeRectangleFixedSize);

			Clear();
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);
			Add(0.5*(dLeft + dRight), 0.5*(dTop + dBottom));

			SetCurrentPointIndex(-1);
		}
	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			double dDeltaX = dX - GetCurrentPoint().dX;
			double dDeltaY = dY - GetCurrentPoint().dY;

			for (int index = 0; index < GetCurrentPointIndex(); ++index)
			{
				GetAt(index).dX += dDeltaX;
				GetAt(index).dY += dDeltaY;							
			}

			GetCurrentPoint().Create(dX, dY);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex = -1;
			Structures::RECTI rectBounds = GetIntegerBoundingRect();
			Structures::POINTI pointTrack(MouseX, MouseY);

			if (rectBounds.IsPointInside(pointTrack))
			{
				nTrackIndex = GetCount() - 1;

				GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));
			}

			if ( -1 == nTrackIndex )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return false;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};
	//------------------------------------------------------------------------------------------------------------------
	// Rectangle Fixed Aspect
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectRectangleFixedAspect : public CTrackObject
	{
	protected:

		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, TRUE);

		//	if (nTrackIndex < 0)
		//	{
		//		Structures::RECTI rectBounds = GetIntegerBoundingRect();
		//		Structures::POINTI pointTrack(nX, nY);

		//		if (rectBounds.IsPointInside(pointTrack))
		//		{
		//			nTrackIndex = GetCount() - 1;

		//			GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));
		//		}
		//	}

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		double dCenterX = 0.5*(GetAt(0).dX + GetAt(2).dX);
		//		double dCenterY = 0.5*(GetAt(0).dY + GetAt(2).dY);
		//		double dSizeX = 0.5*(GetAt(2).dX - GetAt(0).dX);
		//		double dSizeY = 0.5*(GetAt(2).dY - GetAt(0).dY);

		//		double dDistanceOld = _hypot(dSizeX, dSizeY);
		//		double dDistanceNew = _hypot(dX - dCenterX, dY - dCenterY);

		//		double dNewSizeX = dSizeX*dDistanceNew/max(0.001, dDistanceOld);
		//		double dNewSizeY = dSizeY*dDistanceNew/max(0.001, dDistanceOld);

		//		GetAt(0).Create(dCenterX - dNewSizeX, dCenterY - dNewSizeY);
		//		GetAt(1).Create(dCenterX + dNewSizeX, dCenterY - dNewSizeY);
		//		GetAt(2).Create(dCenterX + dNewSizeX, dCenterY + dNewSizeY);
		//		GetAt(3).Create(dCenterX - dNewSizeX, dCenterY + dNewSizeY);
		//		GetAt(4).Create(dCenterX, dCenterY);

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if ((nTrackIndex == GetCount() - 1) || (nTrackIndex >= 0 && GetAsyncKeyState(VK_CONTROL) < 0))
			{
				nPointStatus = c_nTrackPointStatusSizeAll;
				nChangeStatus = c_nTrackChangeStatusMove;
			}
			else
			{
				nChangeStatus = c_nTrackChangeStatusSize;

				if (nTrackIndex == 0 || nTrackIndex == 2)
					nPointStatus = c_nTrackPointStatusSize1;
				else if (nTrackIndex == 1 || nTrackIndex == 3)
					nPointStatus = c_nTrackPointStatusSize2;
				else
				{
					nPointStatus = c_nTrackPointStatusNone;
					nChangeStatus = c_nTrackChangeStatusNone;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawRectangle(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}

	public:

		void Create(double dLeft, double dTop, double dRight, double dBottom)
		{
			SetType(Constants::sc_nTrackTypeRectangleFixedAspect);

			Clear();
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);
			Add(0.5*(dLeft + dRight), 0.5*(dTop + dBottom));

			SetCurrentPointIndex(-1);

			//m_bEnableMinBounding	=	true;
			////m_bEnableMaxBounding	=	true;
			//
			//m_nBoundMaxWidth		=	400;
			//m_nBoundMaxHeight		=	400;

			//m_nBoundMinWidth		=	100;
			//m_nBoundMinHeight		=	100;
		}

	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				m_oSticking.DoWork ( m_nType, this );

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				double dCenterX = 0.5*(GetAt(0).dX + GetAt(2).dX);
				double dCenterY = 0.5*(GetAt(0).dY + GetAt(2).dY);
				double dSizeX = 0.5*(GetAt(2).dX - GetAt(0).dX);
				double dSizeY = 0.5*(GetAt(2).dY - GetAt(0).dY);

				double dDistanceOld = _hypot(dSizeX, dSizeY);
				double dDistanceNew = _hypot(dX - dCenterX, dY - dCenterY);

				double dNewSizeX = dSizeX*dDistanceNew/max(0.001, dDistanceOld);
				double dNewSizeY = dSizeY*dDistanceNew/max(0.001, dDistanceOld);

				CTrackPoint Points[5]	=	{	GetAt(0),	GetAt(1),	GetAt(2),	GetAt(3),	GetAt(4)	};

				GetAt(0).Create(dCenterX - dNewSizeX, dCenterY - dNewSizeY);
				GetAt(1).Create(dCenterX + dNewSizeX, dCenterY - dNewSizeY);
				GetAt(2).Create(dCenterX + dNewSizeX, dCenterY + dNewSizeY);
				GetAt(3).Create(dCenterX - dNewSizeX, dCenterY + dNewSizeY);
				GetAt(4).Create(dCenterX, dCenterY);

				if ( IsUpdateSizes () )
				{
					GetAt(0).Create ( Points[0] );
					GetAt(1).Create ( Points[1] );
					GetAt(2).Create ( Points[2] );
					GetAt(3).Create ( Points[3] );
					GetAt(4).Create ( Points[4] );
				}

				// m_oSticking.DoWork ( m_nType, this );

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex = FindByInteger(MouseX, MouseY, c_nTrackPointSizeTouch, TRUE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(MouseX, MouseY);

				if (rectBounds.IsPointInside(pointTrack))
				{
					nTrackIndex = GetCount() - 1;

					GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));
				}
			}

			if ( -1 == nTrackIndex )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}


		
		virtual bool IsUpdateSizes ()
		{
			double SizeX = 0.5 * ( GetAt(2).dX - GetAt(0).dX );
			double SizeY = 0.5 * ( GetAt(2).dY - GetAt(0).dY );

			if ( m_bEnableMaxBounding )
			{
				if ( SizeX >= m_nBoundMaxWidth || SizeY >= m_nBoundMaxHeight )
					return true;
			}

			if ( m_bEnableMinBounding )
			{
				if ( SizeX <= m_nBoundMinWidth || SizeY <= m_nBoundMinHeight )
					return true;
			}

			return false;
		}
	};

	//------------------------------------------------------------------------------------------------------------------
	// Polyline
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectPolyline : public CTrackObject
	{
	protected:

		int GetPointUnderCursor(int nX, int nY, BOOL& bCanAdd)
		{
			bCanAdd = FALSE;

			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				for (int index = 1; index < GetCount(); ++index)
				{
					if (Geometry::PointOnSegment(GetAt(index - 1).nX, GetAt(index - 1).nY, GetAt(index).nX, GetAt(index).nY, nX, nY, 10))
					{
						bCanAdd = TRUE;

						nTrackIndex = index;

						break;
					}
				}
			}

			return nTrackIndex;
		}

		virtual int GetPointUnderCursor ( int MouseX, int MouseY )
		{
			BOOL bCanAdd = FALSE;
			return GetPointUnderCursor ( MouseX, MouseY,  bCanAdd );
		}	
	protected:

		//virtual void OnLButtonDown(int nX, int nY)
		//{
		//	BOOL bCanAdd = FALSE;
		//	int nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

		//	if (nTrackIndex >= 0 && bCanAdd)
		//	{
		//		CTrackPoint oPoint(MapToDataX(nX), MapToDataY(nY), nX, nY);

		//		InsertAt(nTrackIndex, oPoint);
		//	}

		//	SetCurrentPointIndex(nTrackIndex);

		//	if (!bCanAdd)
		//		SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//	else
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnRButtonDown(int nX, int nY)
		//{
		//	if (GetCount() < 3)
		//		return;

		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, TRUE);

		//	if (nTrackIndex >= 0)
		//	{
		//		Remove(nTrackIndex);

		//		SetCurrentPointIndex(-1);

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
		//	}
		//}
		//void OnRButtonUp(int nX, int nY)
		//{
		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	if (GetAsyncKeyState(VK_CONTROL) < 0)
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		GetCurrentPoint().Create(dX, dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_RBUTTONDOWN)
				OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_RBUTTONUP)
				OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else if (bCanAdd)
					nPointStatus = c_nTrackPointStatusCanAdd;
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPolyline(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount(), c_nTrackPointSizePaint);
		}

	public:

		virtual void Create(Structures::POINTD* pPoints, int nCount)
		{
			SetType(Constants::sc_nTrackTypePolyline);

			Clear();
			SetSize(nCount);

			for (int index = 0; index < nCount; ++index)
				GetAt(index).Create(pPoints[index].x, pPoints[index].y);

			SetCurrentPointIndex(-1);
		}
	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				GetCurrentPoint().Create(dX, dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetPointUnderCursor(MouseX, MouseY, bCanAdd);

			// TODO : переделать под больший размер точек
			if ( GetCount () >= 110 )
			{
				if ( TRUE == bCanAdd && -1 != nTrackIndex )
					return true;

				bCanAdd = FALSE;
			}

			if (nTrackIndex >= 0 && bCanAdd)
			{
				CTrackPoint oPoint(MapToDataX(MouseX), MapToDataY(MouseY), MouseX, MouseY);

				InsertAt(nTrackIndex, oPoint);
			}

			if ( nTrackIndex == -1 && FALSE == bCanAdd )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
			{
				if (!bCanAdd)
					SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
				else
					SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			if (GetCount() < 3)
				return false;

			int nTrackIndex = FindByInteger(MouseX, MouseY, c_nTrackPointSizeTouch, TRUE);
			if ( -1 == nTrackIndex )
				return false;

			if (nTrackIndex >= 0)
			{
				Remove(nTrackIndex);

				SetCurrentPointIndex(-1);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
			}

			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			if ( FALSE == IsCurrentPointValid () )
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};
	//------------------------------------------------------------------------------------------------------------------
	// Polygon
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectPolygon : public CTrackObject
	{
	protected:

		int GetPointUnderCursor(int nX, int nY, BOOL& bCanAdd)
		{
			bCanAdd = FALSE;

			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				int nPointsCount = GetCount();

				for (int index = 1; index < nPointsCount + 1; ++index)
				{
					if (Geometry::PointOnSegment(GetAt(index - 1).nX, GetAt(index - 1).nY, GetAt(index % nPointsCount).nX, GetAt(index % nPointsCount).nY, nX, nY, 10))
					{
						bCanAdd = TRUE;

						nTrackIndex = index;

						break;
					}
				}
			}

			return nTrackIndex;
		}

		virtual int GetPointUnderCursor ( int MouseX, int MouseY )
		{
			BOOL bCanAdd = FALSE;
			return GetPointUnderCursor ( MouseX, MouseY,  bCanAdd );
		}

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	BOOL bCanAdd = FALSE;
		//	int nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

		//	if (nTrackIndex >= 0 && bCanAdd)
		//	{
		//		CTrackPoint oPoint(MapToDataX(nX), MapToDataY(nY), nX, nY);

		//		nTrackIndex = nTrackIndex % GetCount();

		//		InsertAt(nTrackIndex, oPoint);
		//	}

		//	SetCurrentPointIndex(nTrackIndex);

		//	if (!bCanAdd)
		//		SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//	else
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnRButtonDown(int nX, int nY)
		//{
		//	if (GetCount() < 4)
		//		return;

		//	int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, TRUE);

		//	if (nTrackIndex >= 0)
		//	{
		//		Remove(nTrackIndex);

		//		SetCurrentPointIndex(-1);

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
		//	}
		//}
		//void OnRButtonUp(int nX, int nY)
		//{
		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	if (GetAsyncKeyState(VK_CONTROL) < 0)
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		GetCurrentPoint().Create(dX, dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_RBUTTONDOWN)
				OnRButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_RBUTTONUP)
				OnRButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else if (bCanAdd)
					nPointStatus = c_nTrackPointStatusCanAdd;
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPolygon(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount(), c_nTrackPointSizePaint);
		}

	public:

		void Create(Structures::POINTD* pPoints, int nCount)
		{
			SetType(Constants::sc_nTrackTypePolygon);

			Clear();
			SetSize(nCount);

			for (int index = 0; index < nCount; ++index)
				GetAt(index).Create(pPoints[index].x, pPoints[index].y);

			SetCurrentPointIndex(-1);
		}

	public:		
	
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			if (GetAsyncKeyState(VK_CONTROL) < 0)
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				GetCurrentPoint().Create(dX, dY);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}
			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 

			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetPointUnderCursor(MouseX, MouseY, bCanAdd);

			// TODO : переделать под больший размер точек
			if ( GetCount () >= 110 )
			{
				if ( TRUE == bCanAdd && -1 != nTrackIndex )
					return true;

				bCanAdd = FALSE;
			}

			if (nTrackIndex >= 0 && bCanAdd)
			{
				CTrackPoint oPoint(MapToDataX(MouseX), MapToDataY(MouseY), MouseX, MouseY);

				nTrackIndex = nTrackIndex % GetCount();

				InsertAt(nTrackIndex, oPoint);
			}

			if ( nTrackIndex == -1 && FALSE == bCanAdd )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
			{
				if (!bCanAdd)
					SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
				else
					SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			if (GetCount() < 4)
				return false;

			int nTrackIndex = FindByInteger ( MouseX, MouseY, c_nTrackPointSizeTouch, TRUE);

			if (nTrackIndex >= 0)
			{
				Remove(nTrackIndex);

				SetCurrentPointIndex(-1);

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, nTrackIndex);

				return true;
			}

			return false;
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			if ( FALSE == IsCurrentPointValid () )
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};
	//------------------------------------------------------------------------------------------------------------------
	// Pie
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectPie : public CTrackObject
	{
	public:
		
		void Create ( double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle )
		{
			SetType ( Constants::sc_nTrackTypePie );

			Clear ( );

			Add	( dLeft,	dTop );
			Add ( dRight,	dTop );
			Add ( dRight,	dBottom );
			Add ( dLeft,	dBottom );

			double dCenterX	 =	 0.5 * ( dRight		+ dLeft );
			double dCenterY	 =	 0.5 * ( dBottom	+ dTop );
			double dRadiusX	 =	 abs ( dCenterX - dLeft );
			double dRadiusY	 =	 abs ( dCenterY - dTop );
			
			double dX1, dY1, dX2, dY2;
			
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusX, dRadiusY, rad(dStartAngle + dSweepAngle));	
			
			//if ( dRadiusX >= dRadiusY )
			//{
			//	Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, rad(dStartAngle));
			//	Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusX, dRadiusY, rad(dStartAngle + dSweepAngle));	
			//}
			//else
			//{
			//	Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusY, dRadiusX, rad(dStartAngle));
			//	Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusY, dRadiusX, rad(dStartAngle + dSweepAngle));
			//}

			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);
			Add(dCenterX, dCenterY);

			SetCurrentPointIndex(-1);

			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;

			// углы сохраняем
			m_nStartAngle		=	rad ( dStartAngle );
			m_nSweepAngle		=	rad ( dStartAngle + dSweepAngle );
		}

	protected:

		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// get angles
		//		double dCenterX = 0.5*(GetAt(0).dX + GetAt(1).dX);
		//		double dCenterY = 0.5*(GetAt(0).dY + GetAt(3).dY);
		//		double dAngle1 = Geometry::GetAngle(dCenterX, dCenterY, GetAt(4).dX, GetAt(4).dY);
		//		double dAngle2 = Geometry::GetAngle(dCenterX, dCenterY, GetAt(5).dX, GetAt(5).dY);

		//		// correct rectangle points
		//		if (nCurrentPoint == 0)
		//		{
		//			GetAt(0).dX = dX;
		//			GetAt(0).dY = dY;
		//			GetAt(1).dY = dY;
		//			GetAt(3).dX = dX;
		//			UpdatePoints(dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			GetAt(1).dX = dX;
		//			GetAt(1).dY = dY;
		//			GetAt(0).dY = dY;
		//			GetAt(2).dX = dX;
		//			UpdatePoints(dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			GetAt(2).dX = dX;
		//			GetAt(2).dY = dY;
		//			GetAt(1).dX = dX;
		//			GetAt(3).dY = dY;
		//			UpdatePoints(dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			GetAt(3).dX = dX;
		//			GetAt(3).dY = dY;
		//			GetAt(2).dY = dY;
		//			GetAt(0).dX = dX;
		//			UpdatePoints(dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 4)
		//		{
		//			SetNewRect(nCurrentPoint, dX, dY);
		//			UpdatePoints(dX, dY, 5, 4);
		//		}
		//		else if (nCurrentPoint == 5)
		//		{
		//			SetNewRect(nCurrentPoint, dX, dY);
		//			UpdatePoints(dX, dY, 4, 5);
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

		void UpdatePoints(double dAngle1, double dAngle2)
		{
			// get center
			double dCenterX = 0.5*(GetAt(0).dX + GetAt(1).dX);
			double dCenterY = 0.5*(GetAt(0).dY + GetAt(3).dY);

			//change beg point for ellipse
			double dX, dY;
			double dRadiusX = fabs(dCenterX - GetAt(0).dX);
			double dRadiusY = fabs(dCenterY - GetAt(0).dY);
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle1);
			GetAt(4).dX = dCenterX + dX;
			GetAt(4).dY = dCenterY + dY;

			//change end point for first ellipse
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle2);
			GetAt(5).dX = dCenterX + dX;
			GetAt(5).dY = dCenterY + dY;
		}
		void SetNewRect(int indexPoint, double dX, double dY)
		{
			// get center
			double dCenterX = 0.5*(GetAt(0).dX + GetAt(1).dX);
			double dCenterY = 0.5*(GetAt(0).dY + GetAt(3).dY);

			// get ratio rect
			double dRadius1 = Geometry::GetLength(dCenterX, dCenterY, GetAt(indexPoint).dX, GetAt(indexPoint).dY);
			double dRadius2 = Geometry::GetLength(dCenterX, dCenterY, dX, dY);
			double dRatioRect = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;

			// change rect
			double dRadiusX = dRatioRect*fabs(dCenterX - GetAt(0).dX);
			double dRadiusY = dRatioRect*fabs(dCenterY - GetAt(0).dY);
			GetAt(0).dX = dCenterX - dRadiusX;
			GetAt(0).dY = dCenterY - dRadiusY;
			GetAt(1).dX = dCenterX + dRadiusX;
			GetAt(1).dY = dCenterY - dRadiusY;
			GetAt(2).dX = dCenterX + dRadiusX;
			GetAt(2).dY = dCenterY + dRadiusY;
			GetAt(3).dX = dCenterX - dRadiusX;
			GetAt(3).dY = dCenterY + dRadiusY;
		}
		//change beg or end point
		void UpdatePoints(double dX, double dY, int indexPointOldAngle, int indexPointNewAngle)
		{
			// get center
			double dCenterX = 0.5*(GetAt(0).dX + GetAt(1).dX);
			double dCenterY = 0.5*(GetAt(0).dY + GetAt(3).dY);

			double dX1, dY1;
			double dRadiusX = fabs(dCenterX - GetAt(3).dX);
			double dRadiusY = fabs(dCenterY - GetAt(3).dY);

			double dOldAngle = Geometry::GetAngle(dCenterX, dCenterY, GetAt(indexPointOldAngle).dX, GetAt(indexPointOldAngle).dY);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dOldAngle);
			GetAt(indexPointOldAngle).dX = dCenterX + dX1;
			GetAt(indexPointOldAngle).dY = dCenterY + dY1;

			double dNewAngle = Geometry::GetAngle(dCenterX, dCenterY, dX, dY);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dNewAngle);
			GetAt(indexPointNewAngle).dX = dCenterX + dX1;
			GetAt(indexPointNewAngle).dY = dCenterY + dY1;
		}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == GetCount() - 1)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPie(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}


		
		// углы высчитываем 
		inline double GetStartAngle ()
		{	
			return m_nStartAngle;
		}

		inline double GetSweepAngle ()	
		{	
			double Angle	=	 m_nSweepAngle - m_nStartAngle;
			return Angle	=	( Angle >= 0 ) ? Angle : 2.0 * M_PI + Angle;
		}

	public:	

		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX ( MouseX );
			double dY = MapToDataY ( MouseY );

			int nCurrentPoint = GetCurrentPointIndex();

			if ( nCurrentPoint == GetCount() - 1 || GetAsyncKeyState ( VK_CONTROL ) )
			{
				if ( m_oSticking.IsEnableSticking () )
				{
					// т.к. перемещаем весь трекинг то углы сохраняются, их нужно сосчитать заранее
					double dCenterX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
					double dCenterY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

					double dAngle1		=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 4 ).dX, GetAt ( 4 ).dY );
					double dAngle2		=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 5 ).dX, GetAt ( 5 ).dY );

					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );

					m_oSticking.DoWork ( m_nType, this );

					// цент рассчить после применения прилипания
					GetAt ( 6 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
					GetAt ( 6 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );
				
					//m_nStartAngle		=	dAngle2;
					//m_nSweepAngle		=	dAngle1;

					UpdatePoints ( dAngle1, dAngle2 );
				}
				else
				{
					OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// get angles
				double dCenterX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
				double dCenterY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

				double dAngle1		=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 4 ).dX, GetAt ( 4 ).dY );
				double dAngle2		=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 5 ).dX, GetAt ( 5 ).dY );
				
				if ( dAngle1 < 0.0 )
					dAngle1 += 2.0 * M_PI;
				if ( dAngle2 < 0.0 )
					dAngle2 += 2.0 * M_PI;

				/*

				double Length01 = Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 1 ).dX, GetAt ( 1 ).dY );
				double Length02 = Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 3 ).dX, GetAt ( 3 ).dY );
				
				ATLTRACE ( "angle : %f, angle : %f, Length0 : %f, Length02 : %f \n", dAngle1, dAngle2, Length01, Length02 );

				*/

				// запоминаем углы на случай схлопывания
				if ( Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 1 ).dX, GetAt ( 1 ).dY ) <= 0.0001 || 
					Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 3 ).dX, GetAt ( 3 ).dY ) <= 0.0001 )
				{
					dAngle2			=	m_nStartAngle;
					dAngle1			=	m_nSweepAngle;
				}
				else
				{
					m_nStartAngle	=	dAngle2;
					m_nSweepAngle	=	dAngle1;
				}

				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;

					UpdatePoints(dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;
					UpdatePoints(dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;

					UpdatePoints(dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;

					UpdatePoints(dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 4)
				{
					SetNewRect(nCurrentPoint, dX, dY);
					UpdatePoints(dX, dY, 5, 4);
				}
				else if (nCurrentPoint == 5)
				{
					SetNewRect(nCurrentPoint, dX, dY);
					UpdatePoints(dX, dY, 4, 5);
				}

				// прилипание только для BB точек
				if ( m_oSticking.IsEnableSticking () )
				{
					if ( nCurrentPoint != 4 && nCurrentPoint != 5 )
					{
						m_oSticking.DoWork ( m_nType, this );

						// цент рассчить после применения прилипания
						GetAt ( 6 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
						GetAt ( 6 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

						UpdatePoints ( dAngle1, dAngle2 );
					}
				}
			
				// запоминаем углы на случай схлопывания
				if ( Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 1 ).dX, GetAt ( 1 ).dY ) >= 0.0001 && 
					Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 3 ).dX, GetAt ( 3 ).dY ) >= 0.0001 )
				{
					dCenterX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
					dCenterY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

					dAngle1			=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 4 ).dX, GetAt ( 4 ).dY );
					dAngle2			=	Geometry::GetAngle ( dCenterX, dCenterY, GetAt ( 5 ).dX, GetAt ( 5 ).dY );

					m_nStartAngle	=	dAngle2;
					m_nSweepAngle	=	dAngle1;
				}

				if ( false == IsTrackChild () )
					SendMessage ( Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex() );
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex		=	GetPointUnderCursor ( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;
			
			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			if ( nTrackIndex == GetCount() - 1 )
				GetAt ( nTrackIndex ).Create ( MapToDataX ( MouseX ), MapToDataY ( MouseY ) );

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage ( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	private:

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;

		double	m_nStartAngle;
		double	m_nSweepAngle;
	};

	//------------------------------------------------------------------------------------------------------------------
	// Pie Simple
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectPieSimple : public CTrackObject
	{
	public:

		void Create(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle)
		{
			SetType(Constants::sc_nTrackTypePieSimple);

			Clear();

			// get center first ellipse and half axle 
			double dCenterX = 0.5*(dRight + dLeft);
			double dCenterY = 0.5*(dBottom + dTop);
			double dRadiusX = fabs(dCenterX - dLeft);
			double dRadiusY = fabs(dCenterY - dTop);
			double dX1, dY1, dX2, dY2;

			// add beg point and end point on ellipse 
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusX, dRadiusY, rad(dStartAngle + dSweepAngle));
			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);

			// add center
			Add(dCenterX, dCenterY);

			// add invisible rectangle
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);

			SetCurrentPointIndex(-1);
		}

		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);
			if (nTrackIndex > 2)
				nTrackIndex = -1;

			return nTrackIndex;
		}
	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == 2 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		double dRadius = GetPointRadius(nCurrentPoint, 2);
		//		SetNewRect(dX, dY, dRadius);

		//		// correct values
		//		if (nCurrentPoint == 0)
		//			UpdatePoints(dX, dY, 1, 0);
		//		else if (nCurrentPoint == 1)
		//			UpdatePoints(dX, dY, 0, 1);

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

		void SetNewRect(double dX, double dY, double dRadius)
		{
			// get ratio rect
			double dRadius2 = Geometry::GetLength(GetAt(2).dX, GetAt(2).dY, dX, dY);
			double dRatioRect = (dRadius < 0.001)? 1. : dRadius2/dRadius;

			// change rect
			double dRadiusX = dRatioRect * max( 0.01, fabs( GetAt(2).dX - GetAt(3).dX ) );
			double dRadiusY = dRatioRect * max( 0.01, fabs( GetAt(2).dY - GetAt(3).dY ) );
			GetAt(3).dX = GetAt(2).dX - dRadiusX;
			GetAt(3).dY = GetAt(2).dY - dRadiusY;
			GetAt(4).dX = GetAt(2).dX + dRadiusX;
			GetAt(4).dY = GetAt(2).dY - dRadiusY;
			GetAt(5).dX = GetAt(2).dX + dRadiusX;
			GetAt(5).dY = GetAt(2).dY + dRadiusY;
			GetAt(6).dX = GetAt(2).dX - dRadiusX;
			GetAt(6).dY = GetAt(2).dY + dRadiusY;
		}

		//change beg or end point
		void UpdatePoints(double dX, double dY, int indexPointOldAngle, int indexPointNewAngle)
		{
			double dX1, dY1;
			double dRadiusX = fabs(GetAt(2).dX - GetAt(3).dX);
			double dRadiusY = fabs(GetAt(2).dY - GetAt(3).dY);

			double dOldAngle = GetPointAngle(indexPointOldAngle, 2);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dOldAngle);
			GetAt(indexPointOldAngle).dX = GetAt(2).dX + dX1;
			GetAt(indexPointOldAngle).dY = GetAt(2).dY + dY1;

			double dNewAngle = Geometry::GetAngle(GetAt(2).dX, GetAt(2).dY, dX, dY);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dNewAngle);
			GetAt(indexPointNewAngle).dX = GetAt(2).dX + dX1;
			GetAt(indexPointNewAngle).dY = GetAt(2).dY + dY1;
		}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == 2)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPieSimple(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, 3, c_nTrackPointSizePaint);
		}

	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == 2 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);

				if ( false == IsTrackChild () )		
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				double dRadius = GetPointRadius(nCurrentPoint, 2);
				SetNewRect(dX, dY, dRadius);

				// correct values
				if (nCurrentPoint == 0)
					UpdatePoints(dX, dY, 1, 0);
				else if (nCurrentPoint == 1)
					UpdatePoints(dX, dY, 0, 1);

				if ( false == IsTrackChild () )		
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex = GetPointUnderCursor ( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )		
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )		
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);
			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};
	//------------------------------------------------------------------------------------------------------------------
	// Sector Simple
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectSectorSimple : public CTrackObject
	{
	public:
		void Create(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
		{
			SetType(Constants::sc_nTrackTypeSectorSimple);

			Clear();

			// get center first ellipse and half axle 
			double dCenterX = 0.5*(dRight + dLeft);
			double dCenterY = 0.5*(dBottom + dTop);
			double dRadiusX = fabs(dCenterX - dLeft);
			double dRadiusY = fabs(dCenterY - dTop);

			// add beg point and end point on first ellipse 
			double dX1, dY1, dX2, dY2;
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusX, dRadiusY, rad(dStartAngle + dSweepAngle));
			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);

			// get half axle of second ellipse
			double dRadiusXSmall = dRadiusX*dRatio;
			double dRadiusYSmall = dRadiusY*dRatio;

			// add beg point and end point on second ellipse
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusXSmall, dRadiusYSmall, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusXSmall, dRadiusYSmall, rad(dStartAngle + dSweepAngle));
			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);

			// add center
			Add(dCenterX, dCenterY);

			// add invisible rectangle of first ellipse
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);

			// add invisible rectangle of second ellipse
			Add(dCenterX - dRadiusXSmall, dCenterY - dRadiusYSmall);
			Add(dCenterX + dRadiusXSmall, dCenterY - dRadiusYSmall);
			Add(dCenterX + dRadiusXSmall, dCenterY + dRadiusYSmall);
			Add(dCenterX - dRadiusXSmall, dCenterY + dRadiusYSmall);

			SetCurrentPointIndex(-1);
		}

		//  5---/--------------\---6
		//  |  /                \  |
		//  | /   9-/----\-10    \ |
		//  |/    |/      \ |     \|
		//  |\    |\   4  / |     /|
		//  | 1---|--3   2- |----0 |
		//  |     12-------11      |
		//  |                      |
		//  8----------------------7
		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);
			if (nTrackIndex > 4)
				nTrackIndex = -1;

			return nTrackIndex;
		}
	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == 4 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// get angles and radiuses
		//		double dAngle1 = GetPointAngle(0, 4);
		//		double dAngle2 = GetPointAngle(1, 4);

		//		// correct beg-end points
		//		if (nCurrentPoint == 0)
		//		{
		//			SetNewRect(5, dX, dY, dAngle1, dAngle2, GetPointRadius(0, 4));
		//			SetNewAngle(0, 2, dX, dY);
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			SetNewRect(5, dX, dY, dAngle1, dAngle2, GetPointRadius(1, 4));
		//			SetNewAngle(1, 3, dX, dY);
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			SetNewRect(9, dX, dY, dAngle1, dAngle2, GetPointRadius(2, 4));
		//			SetNewAngle(0, 2, dX, dY);
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			SetNewRect(9, dX, dY, dAngle1, dAngle2, GetPointRadius(3, 4));
		//			SetNewAngle(1, 3, dX, dY);
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}
	public:
		void UpdateAllPoints(double dAngle1, double dAngle2)
		{
			double dX, dY;

			//change beg and end points for first ellipse
			double dRadiusX = fabs(GetAt(4).dX - GetAt(5).dX);
			double dRadiusY = fabs(GetAt(4).dY - GetAt(5).dY);
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle1);
			GetAt(0).dX = GetAt(4).dX + dX;
			GetAt(0).dY = GetAt(4).dY + dY;
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle2);
			GetAt(1).dX = GetAt(4).dX + dX;
			GetAt(1).dY = GetAt(4).dY + dY;

			//change beg and end points for second ellipse
			double dRadiusXSmall = fabs(GetAt(4).dX - GetAt(9).dX);
			double dRadiusYSmall = fabs(GetAt(4).dY - GetAt(9).dY);
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusXSmall, dRadiusYSmall, dAngle1);
			GetAt(2).dX = GetAt(4).dX + dX;
			GetAt(2).dY = GetAt(4).dY + dY;
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusXSmall, dRadiusYSmall, dAngle2);
			GetAt(3).dX = GetAt(4).dX + dX;
			GetAt(3).dY = GetAt(4).dY + dY;
		}

		void SetNewAngle(int indexPointOneEllipse, int indexPointTwoEllipse, double dX, double dY)
		{
			double dX1, dY1;
			double dNewAngle = Geometry::GetAngle(GetAt(4).dX, GetAt(4).dY, dX, dY);

			//change beg and end points for first ellipse
			double dRadiusX = fabs(GetAt(4).dX - GetAt(5).dX);
			double dRadiusY = fabs(GetAt(4).dY - GetAt(5).dY);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dNewAngle);
			GetAt(indexPointOneEllipse).dX = GetAt(4).dX + dX1;
			GetAt(indexPointOneEllipse).dY = GetAt(4).dY + dY1;

			//change beg and end points for second ellipse
			double dRadiusXSmall = fabs(GetAt(4).dX - GetAt(9).dX);
			double dRadiusYSmall = fabs(GetAt(4).dY - GetAt(9).dY);
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusXSmall, dRadiusYSmall, dNewAngle);
			GetAt(indexPointTwoEllipse).dX = GetAt(4).dX + dX1;
			GetAt(indexPointTwoEllipse).dY = GetAt(4).dY + dY1;
		}

		void SetNewRect(int indexRect, double dX, double dY, double dAngle1, double dAngle2, double dRadius)
		{
			// get ratio rect
			double dRadius2 = Geometry::GetLength(GetAt(4).dX, GetAt(4).dY, dX, dY);
			double dRatioRect = (dRadius < 0.001)? 1. : dRadius2/dRadius;

			// change rect
			double dRadiusX = dRatioRect * max( 0.01, fabs(GetAt(4).dX - GetAt(indexRect).dX) );
			double dRadiusY = dRatioRect * max( 0.01, fabs(GetAt(4).dY - GetAt(indexRect).dY) );
			GetAt(indexRect).dX = GetAt(4).dX - dRadiusX;
			GetAt(indexRect).dY = GetAt(4).dY - dRadiusY;
			GetAt(indexRect + 1).dX = GetAt(4).dX + dRadiusX;
			GetAt(indexRect + 1).dY = GetAt(4).dY - dRadiusY;
			GetAt(indexRect + 2).dX = GetAt(4).dX + dRadiusX;
			GetAt(indexRect + 2).dY = GetAt(4).dY + dRadiusY;
			GetAt(indexRect + 3).dX = GetAt(4).dX - dRadiusX;
			GetAt(indexRect + 3).dY = GetAt(4).dY + dRadiusY;

			UpdateAllPoints(dAngle1, dAngle2);
		}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex < 0)
				nTrackIndex = FindByInteger(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == 4)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawSectorSimple(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, 5, c_nTrackPointSizePaint);
		}

	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == 4 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// get angles and radiuses
				double dAngle1 = GetPointAngle(0, 4);
				double dAngle2 = GetPointAngle(1, 4);

				// correct beg-end points
				if (nCurrentPoint == 0)
				{
					SetNewRect(5, dX, dY, dAngle1, dAngle2, GetPointRadius(0, 4));
					SetNewAngle(0, 2, dX, dY);
				}
				else if (nCurrentPoint == 1)
				{
					SetNewRect(5, dX, dY, dAngle1, dAngle2, GetPointRadius(1, 4));
					SetNewAngle(1, 3, dX, dY);
				}
				else if (nCurrentPoint == 2)
				{
					SetNewRect(9, dX, dY, dAngle1, dAngle2, GetPointRadius(2, 4));
					SetNewAngle(0, 2, dX, dY);
				}
				else if (nCurrentPoint == 3)
				{
					SetNewRect(9, dX, dY, dAngle1, dAngle2, GetPointRadius(3, 4));
					SetNewAngle(1, 3, dX, dY);
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex = GetPointUnderCursor(MouseX, MouseY);
			if ( -1 == nTrackIndex )
				return false;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};

	//------------------------------------------------------------------------------------------------------------------
	// Sector
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectSector : public CTrackObject
	{
		//  0---/--------------\---1
		//  |  /                \  |
		//  | /   8-/----\--9    \ |
		//  |/    |/      \ |     \|
		//  |\    |\  12  / |     /|
		//  | 7---|--5   4- |----6 |
		//  |     11-------10      |
		//  |                      |
		//  3----------------------2
		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);
			if (nTrackIndex > 7)
				nTrackIndex = -1;

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect(0,3);
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

	protected:
		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// get angles and radiuses
		//		double dAngle1 = GetPointAngle(6, 12);
		//		double dAngle2 = GetPointAngle(7, 12);
		//		double dRadius1 = GetPointRadius(6, 12);
		//		double dRadius2 = GetPointRadius(4, 12);
		//		double dRatio = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;

		//		// correct visible rectangle points
		//		if (nCurrentPoint == 0)
		//		{
		//			// change big rect
		//			GetAt(0).dX = dX;
		//			GetAt(0).dY = dY;
		//			GetAt(1).dY = dY;
		//			GetAt(3).dX = dX;

		//			// correct center, invisible rectangle and beg-end points
		//			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			GetAt(1).dX = dX;
		//			GetAt(1).dY = dY;
		//			GetAt(0).dY = dY;
		//			GetAt(2).dX = dX;

		//			// correct center, invisible rectangle and beg-end points
		//			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			GetAt(2).dX = dX;
		//			GetAt(2).dY = dY;
		//			GetAt(1).dX = dX;
		//			GetAt(3).dY = dY;

		//			// correct center, invisible rectangle and beg-end points
		//			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			GetAt(3).dX = dX;
		//			GetAt(3).dY = dY;
		//			GetAt(2).dY = dY;
		//			GetAt(0).dX = dX;

		//			// correct center, invisible rectangle and beg-end points
		//			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		//		}

		//		// beg-end points
		//		else if (nCurrentPoint == 6)
		//		{
		//			SetNewRect(dX, dY, dRatio, dAngle1, dAngle2, dRadius1);
		//			SetNewAngle(6, 4, dX, dY, dRatio);
		//		}
		//		else if (nCurrentPoint == 7)
		//		{
		//			SetNewRect(dX, dY, dRatio, dAngle1, dAngle2, GetPointRadius(7, 12));
		//			SetNewAngle(7, 5, dX, dY, dRatio);
		//		}
		//		else if (nCurrentPoint == 4)
		//		{
		//			SetNewRatio(dX, dY, dAngle1, dAngle2, dRadius1, dRatio);
		//			SetNewAngle(6, 4, dX, dY, dRatio);
		//		}
		//		else if (nCurrentPoint == 5)
		//		{
		//			SetNewRatio(dX, dY, dAngle1, dAngle2, GetPointRadius(7, 12), dRatio);
		//			SetNewAngle(7, 5, dX, dY, dRatio);
		//		}


		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

		void UpdateAllPoints(double dRatio, double dAngle1, double dAngle2)
		{
			// change center
			GetAt(12).dX = 0.5*(GetAt(0).dX + GetAt(1).dX);
			GetAt(12).dY = 0.5*(GetAt(0).dY + GetAt(3).dY);

			// change small rect
			double dRadiusX = fabs(GetAt(12).dX - GetAt(0).dX);
			double dRadiusY = fabs(GetAt(12).dY - GetAt(0).dY);
			double dRadiusXSmall = dRadiusX*dRatio;
			double dRadiusYSmall = dRadiusY*dRatio;
			GetAt(8).dX = GetAt(12).dX - dRadiusXSmall;
			GetAt(8).dY = GetAt(12).dY - dRadiusYSmall;
			GetAt(9).dX = GetAt(12).dX + dRadiusXSmall;
			GetAt(9).dY = GetAt(12).dY - dRadiusYSmall;
			GetAt(10).dX = GetAt(12).dX + dRadiusXSmall;
			GetAt(10).dY = GetAt(12).dY + dRadiusYSmall;
			GetAt(11).dX = GetAt(12).dX - dRadiusXSmall;
			GetAt(11).dY = GetAt(12).dY + dRadiusYSmall;

			//change beg point for first ellipse
			double dX, dY;
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle1);
			GetAt(6).dX = GetAt(12).dX + dX;
			GetAt(6).dY = GetAt(12).dY + dY;
			//change end point for first ellipse
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusX, dRadiusY, dAngle2);
			GetAt(7).dX = GetAt(12).dX + dX;
			GetAt(7).dY = GetAt(12).dY + dY;
			//change beg point for second ellipse
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusXSmall, dRadiusYSmall, dAngle1);
			GetAt(4).dX = GetAt(12).dX + dX;
			GetAt(4).dY = GetAt(12).dY + dY;
			//change end point for second ellipse
			Geometry::GetEllipsePointCoord(dX, dY, dRadiusXSmall, dRadiusYSmall, dAngle2);
			GetAt(5).dX = GetAt(12).dX + dX;
			GetAt(5).dY = GetAt(12).dY + dY;
		}

		void SetNewAngle(int indexPointOneEllipse, int indexPointTwoEllipse, double dX, double dY, double dRatio)
		{
			double dNewAngle = Geometry::GetAngle(GetAt(12).dX, GetAt(12).dY, dX, dY);
			double dRadiusX = fabs(GetAt(12).dX - GetAt(0).dX);
			double dRadiusY = fabs(GetAt(12).dY - GetAt(0).dY);
			double dRadiusXSmall = dRadiusX*dRatio;
			double dRadiusYSmall = dRadiusY*dRatio;

			//change current point for first ellipse
			double dX1, dY1;
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, dNewAngle);
			GetAt(indexPointOneEllipse).dX = GetAt(12).dX + dX1;
			GetAt(indexPointOneEllipse).dY = GetAt(12).dY + dY1;
			//change current point for second ellipse
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusXSmall, dRadiusYSmall, dNewAngle);
			GetAt(indexPointTwoEllipse).dX = GetAt(12).dX + dX1;
			GetAt(indexPointTwoEllipse).dY = GetAt(12).dY + dY1;
		}

		void SetNewRatio(double dX, double dY, double dAngle1, double dAngle2, double dRadius, double& dRatio)
		{
			double dRadius2	=	Geometry::GetLength ( GetAt(12).dX, GetAt(12).dY, dX, dY );
			dRatio			=	( dRadius < 0.001 ) ? 1.0 : dRadius2 / dRadius;
			
			if ( m_bEnableRangeRatio )
			{
				if ( dRatio > m_nMaxRatio )
				{
					dRatio	=	m_nMaxRatio;
				}

				if ( dRatio < m_nMinRatio )
				{
					dRatio	=	m_nMinRatio;
				}
			}

			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		}

		void SetNewRect(double dX, double dY, double dRatio, double dAngle1, double dAngle2, double dRadius)
		{
			// get ratio rect
			double dRadius2 = Geometry::GetLength(GetAt(12).dX, GetAt(12).dY, dX, dY);
			double dRatioRect = (dRadius < 0.001)? 1. : dRadius2/dRadius;

			// change rect
			double dRadiusX = dRatioRect*fabs(GetAt(12).dX - GetAt(0).dX);
			double dRadiusY = dRatioRect*fabs(GetAt(12).dY - GetAt(0).dY);
			GetAt(0).dX = GetAt(12).dX - dRadiusX;
			GetAt(0).dY = GetAt(12).dY - dRadiusY;
			GetAt(1).dX = GetAt(12).dX + dRadiusX;
			GetAt(1).dY = GetAt(12).dY - dRadiusY;
			GetAt(2).dX = GetAt(12).dX + dRadiusX;
			GetAt(2).dY = GetAt(12).dY + dRadiusY;
			GetAt(3).dX = GetAt(12).dX - dRadiusX;
			GetAt(3).dY = GetAt(12).dY + dRadiusY;

			UpdateAllPoints(dRatio, dAngle1, dAngle2);
		}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == GetCount() - 1)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawSector(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, 8, c_nTrackPointSizePaint);
		}

	public:

		void Create(double dLeft, double dTop, double dRight, double dBottom, double dStartAngle, double dSweepAngle, double dRatio)
		{
			m_bEnableRangeRatio		=	false;
			m_nMaxRatio				=	0.0;
			m_nMinRatio				=	10000.0;

			SetType(Constants::sc_nTrackTypeSector);

			Clear();

			// add visible rectangle of first ellipse
			Add(dLeft, dTop);
			Add(dRight, dTop);
			Add(dRight, dBottom);
			Add(dLeft, dBottom);

			// get center first ellipse and half axle 
			double dCenterX = 0.5*(dRight + dLeft);
			double dCenterY = 0.5*(dBottom + dTop);
			double dRadiusX = fabs(dCenterX - dLeft);
			double dRadiusY = fabs(dCenterY - dTop);

			// get half axle of second ellipse
			double dRadiusXSmall = dRadiusX*dRatio;
			double dRadiusYSmall = dRadiusY*dRatio;

			// add beg point and end point on second ellipse
			double dX1, dY1, dX2, dY2;
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusXSmall, dRadiusYSmall, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusXSmall, dRadiusYSmall, rad(dStartAngle + dSweepAngle));
			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);

			// add beg point and end point on first ellipse 
			Geometry::GetEllipsePointCoord(dX1, dY1, dRadiusX, dRadiusY, rad(dStartAngle));
			Geometry::GetEllipsePointCoord(dX2, dY2, dRadiusX, dRadiusY, rad(dStartAngle + dSweepAngle));
			Add(dCenterX + dX2, dCenterY + dY2);
			Add(dCenterX + dX1, dCenterY + dY1);

			// add invisible rectangle of second ellipse
			Add(dCenterX - dRadiusXSmall, dCenterY - dRadiusYSmall);
			Add(dCenterX + dRadiusXSmall, dCenterY - dRadiusYSmall);
			Add(dCenterX + dRadiusXSmall, dCenterY + dRadiusYSmall);
			Add(dCenterX - dRadiusXSmall, dCenterY + dRadiusYSmall);

			// add center
			Add(dCenterX, dCenterY);

			// add center for moving
			Add(dCenterX, dCenterY);

			SetCurrentPointIndex(-1);

			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;
		
			// углы сохраняем
			m_nStartAngle		=	rad ( dStartAngle );
			m_nSweepAngle		=	rad ( dStartAngle + dSweepAngle );
			m_nRatio			=	dRatio;

			// SetRangeRation ( true, 0.0, 0.9 );
		}

	
		// углы высчитываем 
		inline double GetStartAngle ()
		{	
			return m_nStartAngle;
		}

		inline double GetSweepAngle ()	
		{	
			double Angle	=	 m_nSweepAngle - m_nStartAngle;
			return Angle	=	( Angle >= 0 ) ? Angle : 2.0 * M_PI + Angle;
		}

		inline double GetRatio ()
		{
			return m_nRatio;
		}
		inline void SetRangeRation ( bool Enable, double Min = 0.0, double Max = 0.95 )
		{
			m_bEnableRangeRatio		=	Enable;

			m_nMaxRatio				=	Max;
			m_nMinRatio				=	Min;

			if ( m_nMaxRatio <= m_nMinRatio || m_nMaxRatio < 0.0 || m_nMinRatio < 0.0 )
			{
				m_bEnableRangeRatio	=	false;
			}
		}
	
	public:	

		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );

				if ( m_oSticking.IsEnableSticking () )
				{
					// т.к. перемещаем весь трекинг то углы сохраняются, их нужно сосчитать заранее
					double dAngle1	=	GetPointAngle ( 6, 12 );
					double dAngle2	=	GetPointAngle ( 7, 12 );
					double dRadius1	=	GetPointRadius ( 6, 12 );
					double dRadius2	=	GetPointRadius ( 4, 12 );
					double dRatio	=	(dRadius1 < 0.001 ) ? 1. : dRadius2 / dRadius1;

					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );

					m_oSticking.DoWork ( m_nType, this );

					// центр рассчить после применения прилипания
					GetAt ( 12 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
					GetAt ( 12 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );
					GetAt ( 13 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
					GetAt ( 13 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

					// correct center, invisible rectangle and beg-end points
					UpdateAllPoints ( dRatio, dAngle1, dAngle2 );
				}
				else
				{
					m_oSticking.DoWork ( m_nType, this );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// get angles and radiuses
				double dAngle1	=	GetPointAngle	( 6, 12 );
				double dAngle2	=	GetPointAngle	( 7, 12 );
				double dRadius1	=	GetPointRadius	( 6, 12 );
				double dRadius2	=	GetPointRadius	( 4, 12 );

				double dRatio	=	( dRadius1 < 0.001 ) ? 1.0 : dRadius2 / dRadius1;

				if ( dAngle1 < 0.0 )
					dAngle1 += 2.0 * M_PI;
				if ( dAngle2 < 0.0 )
					dAngle2 += 2.0 * M_PI;
				
				double BoundWidth	=	Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 1 ).dX, GetAt ( 1 ).dY );
				double BoundHeight	=	Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 3 ).dX, GetAt ( 3 ).dY );

				if ( BoundWidth <= 0.0001 || BoundHeight <= 0.0001 )
				{
					dAngle2			=	m_nStartAngle;
					dAngle1			=	m_nSweepAngle;
					dRatio			=	m_nRatio;
				}
				else
				{
					m_nStartAngle	=	dAngle2;
					m_nSweepAngle	=	dAngle1;
					m_nRatio		=	dRatio;
				}

				// correct visible rectangle points
				if (nCurrentPoint == 0)
				{
					// change big rect
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;

					// correct center, invisible rectangle and beg-end points
					UpdateAllPoints(dRatio, dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;

					// correct center, invisible rectangle and beg-end points
					UpdateAllPoints(dRatio, dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;

					// correct center, invisible rectangle and beg-end points
					UpdateAllPoints(dRatio, dAngle1, dAngle2);
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;

					// correct center, invisible rectangle and beg-end points
					UpdateAllPoints(dRatio, dAngle1, dAngle2);
				}

				// beg-end points
				else if (nCurrentPoint == 6)
				{
					SetNewRect(dX, dY, dRatio, dAngle1, dAngle2, dRadius1);
					SetNewAngle(6, 4, dX, dY, dRatio);
				}
				else if (nCurrentPoint == 7)
				{
					SetNewRect(dX, dY, dRatio, dAngle1, dAngle2, GetPointRadius(7, 12));
					SetNewAngle(7, 5, dX, dY, dRatio);
				}
				else if (nCurrentPoint == 4)
				{
					SetNewRatio(dX, dY, dAngle1, dAngle2, dRadius1, dRatio);
					SetNewAngle(6, 4, dX, dY, dRatio);
				}
				else if (nCurrentPoint == 5)
				{
					SetNewRatio(dX, dY, dAngle1, dAngle2, GetPointRadius(7, 12), dRatio);
					SetNewAngle(7, 5, dX, dY, dRatio);
				}

				// прилипание только для BB точек
				if ( m_oSticking.IsEnableSticking () )
				{
					if ( nCurrentPoint != 4 && nCurrentPoint != 5 && nCurrentPoint != 6 && nCurrentPoint != 7 )
					{
						m_oSticking.DoWork ( m_nType, this );

						// центр рассчить после применения прилипания
						GetAt ( 12 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
						GetAt ( 12 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );
						GetAt ( 13 ).dX		=	0.5 * ( GetAt ( 0 ).dX + GetAt ( 1 ).dX );
						GetAt ( 13 ).dY		=	0.5 * ( GetAt ( 0 ).dY + GetAt ( 3 ).dY );

						// correct center, invisible rectangle and beg-end points
						UpdateAllPoints ( dRatio, dAngle1, dAngle2 );
					}
				}

				BoundWidth			=	Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 1 ).dX, GetAt ( 1 ).dY );
				BoundHeight			=	Geometry::GetLength ( GetAt ( 0 ).dX, GetAt ( 0 ).dY, GetAt ( 3 ).dX, GetAt ( 3 ).dY );

				if ( BoundWidth >= 0.0001 && BoundHeight >= 0.0001 )
				{
					dAngle2			=	m_nStartAngle;
					dAngle1			=	m_nSweepAngle;
					dRatio			=	m_nRatio;

					// get angles and radiuses
					double dAngle1	=	GetPointAngle	( 6, 12 );
					double dAngle2	=	GetPointAngle	( 7, 12 );
					double dRadius1	=	GetPointRadius	( 6, 12 );
					double dRadius2	=	GetPointRadius	( 4, 12 );

					double dRatio	=	( dRadius1 < 0.001 ) ? 1.0 : dRadius2 / dRadius1;

					if ( dAngle1 < 0.0 )
						dAngle1 += 2.0 * M_PI;
					if ( dAngle2 < 0.0 )
						dAngle2 += 2.0 * M_PI;

					m_nStartAngle	=	dAngle2;
					m_nSweepAngle	=	dAngle1;
					m_nRatio		=	dRatio;	
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}
			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex		=	GetPointUnderCursor ( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;
		
			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			if ( nTrackIndex == GetCount() - 1 )
				GetAt ( nTrackIndex ).Create ( MapToDataX ( MouseX ), MapToDataY ( MouseY ) );

			SetCurrentPointIndex ( nTrackIndex );

			if ( false == IsTrackChild () )
				SendMessage ( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true; 
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);
			return true; 
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	
	private:

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;
		
		double	m_nStartAngle;
		double	m_nSweepAngle;
		double	m_nRatio;

		bool	m_bEnableRangeRatio;	// ограничение на ratio для сектора
		double	m_nMaxRatio;
		double	m_nMinRatio;
	};

	//------------------------------------------------------------------------------------------------------------------
	// Circle
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectCircle : public CTrackObject
	{
	protected:

		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		SetNewRect(nCurrentPoint, dX, dY);

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

		void SetNewRect ( int indexPoint, double dX, double dY )
		{
			// get center
			double dCenterX	= 0.5*(GetAt(0).dX + GetAt(1).dX);
			double dCenterY = 0.5*(GetAt(0).dY + GetAt(3).dY);

			// get ratio rect
			double dRadius1 = Geometry::GetLength(dCenterX, dCenterY, GetAt(indexPoint).dX, GetAt(indexPoint).dY);
			double dRadius2 = Geometry::GetLength(dCenterX, dCenterY, dX, dY);
			double dRatioRect = (dRadius1 < 0.001)? 1. : dRadius2/dRadius1;

			// change rect
			double dRadiusX = dRatioRect*fabs(dCenterX - GetAt(0).dX);
			double dRadiusY = dRatioRect*fabs(dCenterY - GetAt(0).dY);

			if (dRadiusX < 0.01)
				dRadiusX = dRadius2;
			if (dRadiusY < 0.01)
				dRadiusY = dRadius2;

			GetAt(0).dX = dCenterX - dRadiusX;
			GetAt(0).dY = dCenterY - dRadiusY;
			GetAt(1).dX = dCenterX + dRadiusX;
			GetAt(1).dY = dCenterY - dRadiusY;
			GetAt(2).dX = dCenterX + dRadiusX;
			GetAt(2).dY = dCenterY + dRadiusY;
			GetAt(3).dX = dCenterX - dRadiusX;
			GetAt(3).dY = dCenterY + dRadiusY;
		}
	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == GetCount() - 1)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else
				{
					nChangeStatus = c_nTrackChangeStatusSize;

					if (nTrackIndex == 0 || nTrackIndex == 2)
						nPointStatus = c_nTrackPointStatusSize1;
					else
						nPointStatus = c_nTrackPointStatusSize2;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawCircle(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}

	public:

		void Create(double dLeft, double dTop, double dRight, double dBottom)
		{
			SetType ( Constants::sc_nTrackTypeCircleInRectangle );

			Clear ();

			Add ( dLeft,	dTop );
			Add ( dRight,	dTop );
			Add ( dRight,	dBottom );
			Add ( dLeft,	dBottom );

			double dCenterX		=	0.5 * ( dRight	+ dLeft );
			double dCenterY		=	0.5 * ( dBottom	+ dTop );

			Add ( dCenterX, dCenterY );

			SetCurrentPointIndex(-1);

			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;
		}

	public:	

		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX ( MouseX );
			double dY = MapToDataY ( MouseY );

			int nCurrentPoint = GetCurrentPointIndex();

			if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
			{
				// делаем по нулевой точки
				if ( m_oSticking.IsEnableSticking () )
				{
					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );

					m_oSticking.DoWork ( m_nType, this );
				}
				else
				{
					OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				SetNewRect ( nCurrentPoint, dX, dY );

				//if ( m_oSticking.IsEnableSticking () )
				//{
				//	m_oSticking.DoWork ( m_nType, this );
				//}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true; 
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex		=	GetPointUnderCursor ( MouseX, MouseY );

			if ( -1 == nTrackIndex )
				return false;
		
			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			if (nTrackIndex == GetCount() - 1)
				GetAt(nTrackIndex).Create(MapToDataX(MouseX), MapToDataY(MouseY));

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true; 
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	private:

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;
	};
	//------------------------------------------------------------------------------------------------------------------
	// Ellipse In Rectangle
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectEllipseInRectangle : public CTrackObject
	{
	protected:

		int GetPointUnderCursor(int nX, int nY)
		{
			int nTrackIndex = FindByInteger(nX, nY, c_nTrackPointSizeTouch, FALSE);

			if (nTrackIndex < 0)
			{
				Structures::RECTI rectBounds = GetIntegerBoundingRect();
				Structures::POINTI pointTrack(nX, nY);

				if (rectBounds.IsPointInside(pointTrack))
					nTrackIndex = GetCount() - 1;
			}

			return nTrackIndex;
		}

	protected:

		//void OnLButtonDown(int nX, int nY)
		//{
		//	int nTrackIndex = GetPointUnderCursor(nX, nY);

		//	if (nTrackIndex == GetCount() - 1)
		//		GetAt(nTrackIndex).Create(MapToDataX(nX), MapToDataY(nY));

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}
		//void OnLButtonUp(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

		//	SetCurrentPointIndex(-1);
		//}
		//void OnMouseMove(int nX, int nY)
		//{
		//	if (!IsCurrentPointValid())
		//		return;

		//	double dX = MapToDataX(nX);
		//	double dY = MapToDataY(nY);

		//	int nCurrentPoint = GetCurrentPointIndex();

		//	if (nCurrentPoint == GetCount() - 1 || GetAsyncKeyState(VK_CONTROL))
		//	{
		//		OffsetByDouble(dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY);
		//		SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
		//	}
		//	else
		//	{
		//		// correct rectangle points
		//		if (nCurrentPoint == 0)
		//		{
		//			GetAt(0).dX = dX;
		//			GetAt(0).dY = dY;
		//			GetAt(1).dY = dY;
		//			GetAt(3).dX = dX;
		//		}
		//		else if (nCurrentPoint == 1)
		//		{
		//			GetAt(1).dX = dX;
		//			GetAt(1).dY = dY;
		//			GetAt(0).dY = dY;
		//			GetAt(2).dX = dX;
		//		}
		//		else if (nCurrentPoint == 2)
		//		{
		//			GetAt(2).dX = dX;
		//			GetAt(2).dY = dY;
		//			GetAt(1).dX = dX;
		//			GetAt(3).dY = dY;
		//		}
		//		else if (nCurrentPoint == 3)
		//		{
		//			GetAt(3).dX = dX;
		//			GetAt(3).dY = dY;
		//			GetAt(2).dY = dY;
		//			GetAt(0).dX = dX;
		//		}

		//		SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
		//	}
		//}

	public:

		virtual BOOL OnMessages(UINT nMessage, WPARAM wParam, LPARAM lParam)
		{
			if (nMessage == WM_LBUTTONDOWN)
				OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_LBUTTONUP)
				OnLButtonUp(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			else if (nMessage == WM_MOUSEMOVE)
				OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

			return FALSE;
		}
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nChangeStatus = c_nTrackChangeStatusNone;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0 || nTrackIndex == GetCount() - 1)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;

					int	nIndex = GetPointUnderCursor(nX, nY);
				}
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}
		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawEllipseInRectangle(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawPoints(this, GetCount() - 1, c_nTrackPointSizePaint);
		}

	public:

		void Create ( double dLeft, double dTop, double dRight, double dBottom )
		{
			SetType ( Constants::sc_nTrackTypeEllipseInRectangle );

			Clear ();

			Add ( dLeft,	dTop );
			Add ( dRight,	dTop );
			Add ( dRight,	dBottom );
			Add ( dLeft,	dBottom );

			double dCenterX		=	0.5 * ( dRight	+ dLeft );
			double dCenterY		=	0.5 * ( dBottom	+ dTop );

			Add ( dCenterX, dCenterY );

			SetCurrentPointIndex ( -1 );

			m_nMouseOffsetX		=	0.0;
			m_nMouseOffsetY		=	0.0;
		}

	public :

		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			if (!IsCurrentPointValid())
				return false;

			double dX = MapToDataX(MouseX);
			double dY = MapToDataY(MouseY);

			int nCurrentPoint = GetCurrentPointIndex();

			if ( nCurrentPoint == GetCount() - 1 || GetAsyncKeyState ( VK_CONTROL ) )
			{
				// делаем по нулевой точки
				if ( m_oSticking.IsEnableSticking () )
				{
					OffsetByDouble ( dX - GetAt ( 0 ).dX - m_nMouseOffsetX, dY - GetAt ( 0 ).dY - m_nMouseOffsetY );

					m_oSticking.DoWork ( m_nType, this );
				}
				else
				{
					OffsetByDouble ( dX - GetCurrentPoint().dX, dY - GetCurrentPoint().dY );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcessAll, GetCurrentPointIndex());
			}
			else
			{
				// correct rectangle points
				if (nCurrentPoint == 0)
				{
					GetAt(0).dX = dX;
					GetAt(0).dY = dY;
					GetAt(1).dY = dY;
					GetAt(3).dX = dX;
				}
				else if (nCurrentPoint == 1)
				{
					GetAt(1).dX = dX;
					GetAt(1).dY = dY;
					GetAt(0).dY = dY;
					GetAt(2).dX = dX;
				}
				else if (nCurrentPoint == 2)
				{
					GetAt(2).dX = dX;
					GetAt(2).dY = dY;
					GetAt(1).dX = dX;
					GetAt(3).dY = dY;
				}
				else if (nCurrentPoint == 3)
				{
					GetAt(3).dX = dX;
					GetAt(3).dY = dY;
					GetAt(2).dY = dY;
					GetAt(0).dX = dX;
				}

				GetAt(4).dX = 0.5 * ( GetAt(0).dX + GetAt(1).dX );
				GetAt(4).dY = 0.5 * ( GetAt(0).dY + GetAt(3).dY );

				if ( m_oSticking.IsEnableSticking () )
				{
					m_oSticking.DoWork ( m_nType, this );
				}

				if ( false == IsTrackChild () )
					SendMessage(Constants::sc_nMessageTrackPointProcess, GetCurrentPointIndex());
			}

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			int nTrackIndex		=	GetPointUnderCursor ( MouseX, MouseY );
			if ( -1 == nTrackIndex )
				return false;

			// смещение относительно нулевой точки
			m_nMouseOffsetX		=	MapToDataX ( MouseX ) - GetAt ( 0 ).dX;
			m_nMouseOffsetY		=	MapToDataY ( MouseY ) - GetAt ( 0 ).dY;

			if (nTrackIndex == GetCount() - 1)
				GetAt ( nTrackIndex ).Create ( MapToDataX ( MouseX ), MapToDataY ( MouseY ) );

			SetCurrentPointIndex ( nTrackIndex );

			if ( false == IsTrackChild () )
				SendMessage ( Constants::sc_nMessageTrackPointBegin, nTrackIndex );

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			if (!IsCurrentPointValid())
				return false;

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointEnd, GetCurrentPointIndex());

			SetCurrentPointIndex(-1);

			return true;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}

	private:

		double	m_nMouseOffsetX;
		double	m_nMouseOffsetY;
	};

	//------------------------------------------------------------------------------------------------------------------
	// Trajectory Poly line
	//------------------------------------------------------------------------------------------------------------------
	class CTrackObjectTrajectoryPolyline : public CTrackObjectPolyline
	{
	public:
		virtual BOOL OnSetCursor(int nX, int nY, int& nPointStatus, int& nChangeStatus)
		{
			nPointStatus = c_nTrackPointStatusNone;
			nChangeStatus = c_nTrackChangeStatusNone;

			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetCurrentPointIndex();

			if (nTrackIndex < 0)
				nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

			if (nTrackIndex >= 0)
			{
				if (GetAsyncKeyState(VK_CONTROL) < 0)
				{
					nPointStatus = c_nTrackPointStatusSizeAll;
					nChangeStatus = c_nTrackChangeStatusMove;
				}
				else if (bCanAdd)
					nPointStatus = c_nTrackPointStatusNone;
				else
				{
					nPointStatus = c_nTrackPointStatusPoint;
					nChangeStatus = c_nTrackChangeStatusSize;
				}
			}

			return FALSE;
		}

		//virtual void OnLButtonDown(int nX, int nY)
		//{
		//	BOOL bCanAdd = FALSE;
		//	int nTrackIndex = GetPointUnderCursor(nX, nY, bCanAdd);

		//	if( nTrackIndex >= 0 && bCanAdd ) return;

		//	SetCurrentPointIndex(nTrackIndex);

		//	SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);
		//}

		virtual void OnPaintLines(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawTrajectoryPolyline(this);
		}
		virtual void OnPaintPoints(Tracking::CTrackPainter& oPainter)
		{
			oPainter.DrawTrajectoryPoints(this, GetCount(), 6 );
		}

		void Create(Structures::POINTD* pPoints, int nCount)
		{
			CTrackObjectPolyline::Create( pPoints, nCount );
			SetType( Constants::sc_nTrackTypeTrajectoryPolyline );
		}
	public:		
		bool IMouseButtonHandlers::OnMouseMove ( int MouseX, int MouseY )				
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnLButtonDown ( int MouseX, int MouseY )	
		{ 
			BOOL bCanAdd = FALSE;
			int nTrackIndex = GetPointUnderCursor( MouseX, MouseY, bCanAdd);

			if ( -1 == nTrackIndex && bCanAdd == FALSE )
				return false;

			if( nTrackIndex >= 0 && bCanAdd )
				return true;

			SetCurrentPointIndex(nTrackIndex);

			if ( false == IsTrackChild () )
				SendMessage(Constants::sc_nMessageTrackPointBegin, nTrackIndex);

			return true;
		}

		bool IMouseButtonHandlers::OnLButtonUp ( int MouseX, int MouseY )		
		{
			return false;
		}

		bool IMouseButtonHandlers::OnRButtonDown ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnRButtonUp ( int MouseX, int MouseY )		
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonDown ( int MouseX, int MouseY )	
		{
			return false; 
		}

		bool IMouseButtonHandlers::OnMButtonUp ( int MouseX, int MouseY )			
		{
			return false; 
		}
	};
}