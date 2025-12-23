#include "stdafx.h"
#include "VAT_DataCopy.h"


CVAT_DataCopy::CVAT_DataCopy()
{
}


CVAT_DataCopy::~CVAT_DataCopy()
{
}





//===================================================================================== Z Auto Teaching.
void CVAT_DataCopy::_dc_m4_GUiToReal(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand)
{
	switch (_eType)
	{
		case eZAutoTeaching_ZTeachBasePos:
		{

		}break;
		case eZAutoTeaching_PickPlaceOffset:
		{

		}break;
		case eZAutoTeaching_PickerFlatnessOffset:
		{

		}break;
		case eZAutoTeaching_PickerVacPos:
		{

		}break;
		default:
			break;
	}
}


void CVAT_DataCopy::_dc_m4_RealToGUI(_eVAT_MODE_EACH_DATA_TYPE _eType, int nHand)
{
	switch (_eType)
	{
	case eZAutoTeaching_ZTeachBasePos:
	{

	}break;
	case eZAutoTeaching_PickPlaceOffset:
	{

	}break;
	case eZAutoTeaching_PickerFlatnessOffset:
	{

	}break;
	case eZAutoTeaching_PickerVacPos:
	{

	}break;
	default:
		break;
	}
}