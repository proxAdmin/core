#include "crecordodraw.h"

#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtDgContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFDG.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFSPGR.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFSP.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFOPT.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtClientAnchorSheet.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtClientAnchorChart.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtClientAnchorHF.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFRITContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtFDGGBlock.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtColorMRUContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtSplitMenuColorContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/SimpleOfficeArtContainers.h"
//#include <../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/Binary/CFRecord.h>
//#include "Exception/WrongBiffRecord.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtBStoreContainer.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Logic/Biff_structures/ODRAW/OfficeArtTertiaryFOPT.h"

using namespace ODRAW;

CRecordODraw::CRecordODraw()
{

}

void CRecordODraw::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
    m_oHeader = oHeader;

    XLS::CFStreamPtr stream = XLS::CFStreamPtr(new CFStream(pStream));
    LONG lPos; StreamUtils::StreamPosition(lPos, pStream);
    XLS::CFRecord record(stream);


    auto anchor_type_ = OfficeArtRecord::CA_HF;
    switch ((USHORT)m_oHeader.RecType)
    {
        case OfficeArtRecord::DggContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtDggContainer(anchor_type_));	break;
        case OfficeArtRecord::DgContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtDgContainer(anchor_type_));	break;
        case OfficeArtRecord::FDG:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFDG);							break;
        case OfficeArtRecord::FRITContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFRITContainer);				break;
        case OfficeArtRecord::SpgrContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtSpgrContainer(anchor_type_));	break;
        case OfficeArtRecord::SpContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtSpContainer(anchor_type_));	break;
        case OfficeArtRecord::SolverContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtSolverContainer);				break;
        case OfficeArtRecord::FSPGR:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFSPGR);						break;
        case OfficeArtRecord::FSP:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFSP);							break;
        case OfficeArtRecord::FOPT:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFOPT);							break;
        case OfficeArtRecord::ChildAnchor:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtChildAnchor);					break;
        case OfficeArtRecord::ClientAnchor:
            switch(anchor_type_)
            {
                case OfficeArtRecord::CA_Chart:
                    pArtRecord = OfficeArtRecordPtr(new OfficeArtClientAnchorChart);	break;
                case OfficeArtRecord::CA_Sheet:
                    pArtRecord = OfficeArtRecordPtr(new OfficeArtClientAnchorSheet);	break;
                case OfficeArtRecord::CA_HF:
                    pArtRecord = OfficeArtRecordPtr(new OfficeArtClientAnchorHF);		break;
            }
            break;
        case OfficeArtRecord::ClientData:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtClientData);					break;
        case OfficeArtRecord::FConnectorRule:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFConnectorRule);				break;
        case OfficeArtRecord::FArcRule:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFArcRule);						break;
        case OfficeArtRecord::FCalloutRule:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFCalloutRule);					break;
        case OfficeArtRecord::ClientTextbox:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtClientTextbox);				break;
        case OfficeArtRecord::FDGGBlock:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtFDGGBlock);					break;
        case OfficeArtRecord::ColorMRUContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtColorMRUContainer);			break;
        case OfficeArtRecord::SplitMenuColorContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtSplitMenuColorContainer);		break;
        case OfficeArtRecord::BStoreContainer:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtBStoreContainer);				break;
        case OfficeArtRecord::TertiaryFOPT:
            pArtRecord = OfficeArtRecordPtr(new OfficeArtTertiaryFOPT);					break;
        case OfficeArtRecord::SecondaryFOPT:

        case OfficeArtRecord::FPSPL:
        case OfficeArtRecord::FDGSL:
        case OfficeArtRecord::FBSE:
        case OfficeArtRecord::BlipJPEG_clone:
        case OfficeArtRecord::BlipDIB:
        case OfficeArtRecord::BlipTIFF:
        case OfficeArtRecord::BlipEMF:
        case OfficeArtRecord::BlipWMF:
        case OfficeArtRecord::BlipPICT:
        case OfficeArtRecord::BlipJPEG:
        case OfficeArtRecord::BlipPNG:
            break;
        default:
            break;
    }
    pArtRecord->loadFields(record);

}
