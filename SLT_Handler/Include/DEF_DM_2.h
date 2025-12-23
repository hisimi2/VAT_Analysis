#pragma once

#define SZ_DM_NAME_2		_T("DM_PRESS_UNIT_UP")


#define NDM2_StationStatus_1	(0x000)
#define NDM2_StationStatus_2	(0x001)
#define NDM2_StationStatus_3	(0x002)
#define NDM2_StationStatus_4	(0x003)
#define NDM2_StationStatus_5	(0x004)
#define NDM2_StationStatus_6	(0x005)
#define NDM2_StationStatus_7	(0x006)
#define NDM2_StationStatus_8	(0x007)
#define NDM2_StationStatus_9	(0x008)
#define NDM2_StationStatus_10	(0x009)
#define NDM2_StationStatus_11	(0x00A)
#define NDM2_StationStatus_12	(0x00B)
#define NDM2_StationStatus_13	(0x00C)
#define NDM2_StationStatus_14	(0x00D)
#define NDM2_StationStatus_15	(0x00E)
#define NDM2_StationStatus_16	(0x00F)

// 128para
// Station 1  : 0x000 ~ 0x007 / Station 2  : 0x008 ~ 0x00F 
// Station 3  : 0x010 ~ 0x017 / Station 4  : 0x018 ~ 0x01F
// Station 5  : 0x020 ~ 0x027 / Station 6  : 0x028 ~ 0x02F 
// Station 7  : 0x030 ~ 0x037 / Station 8  : 0x038 ~ 0x03F
// Station 9  : 0x040 ~ 0x047 / Station 10 : 0x048 ~ 0x04F
// Station 11 : 0x050 ~ 0x057 / Station 12 : 0x058 ~ 0x05F
// Station 13 : 0x060 ~ 0x067 / Station 14 : 0x068 ~ 0x06F
// Station 15 : 0x070 ~ 0x077 / Station 16 : 0x078 ~ 0x07F
#define NDM2_SiteHistoryHW1_1	(0x100)
#define NDM2_SiteHistoryHW16_8 	(0x17F)

// 128para
// Station 1  : 0x180 ~ 0x187 / Station 2  : 0x188 ~ 0x18F 
// Station 3  : 0x190 ~ 0x197 / Station 4  : 0x198 ~ 0x19F
// Station 5  : 0x1A0 ~ 0x1A7 / Station 6  : 0x1A8 ~ 0x1AF 
// Station 7  : 0x1B0 ~ 0x1B7 / Station 8  : 0x1B8 ~ 0x1BF
// Station 9  : 0x1C0 ~ 0x1C7 / Station 10 : 0x1C8 ~ 0x1CF
// Station 11 : 0x1D0 ~ 0x1D7 / Station 12 : 0x1D8 ~ 0x1DF
// Station 13 : 0x1E0 ~ 0x1E7 / Station 14 : 0x1E8 ~ 0x1EF
// Station 15 : 0x1F0 ~ 0x1F7 / Station 16 : 0x1F8 ~ 0x1FF
#define NDM2_1STFailBinCnt_Press1_1              (0x180)
#define NDM2_1STFailBinCnt_Press16_8             (0x1FF)

// 128para
// Station 1  : 0x200 ~ 0x207 / Station 2  : 0x208 ~ 0x20F 
// Station 3  : 0x210 ~ 0x217 / Station 4  : 0x218 ~ 0x21F
// Station 5  : 0x220 ~ 0x227 / Station 6  : 0x228 ~ 0x22F 
// Station 7  : 0x230 ~ 0x237 / Station 8  : 0x238 ~ 0x23F
// Station 9  : 0x240 ~ 0x247 / Station 10 : 0x248 ~ 0x24F
// Station 11 : 0x250 ~ 0x257 / Station 12 : 0x258 ~ 0x25F
// Station 13 : 0x260 ~ 0x267 / Station 14 : 0x268 ~ 0x26F
// Station 15 : 0x270 ~ 0x277 / Station 16 : 0x278 ~ 0x27F
#define NDM2_2DIDFailBinCnt_Press1_1		(0x200)
#define NDM2_2DIDFailBinCnt_Press16_8		(0x27F)


#define NDM2_PRESS_1_PRE_CONTACT_STATUS			(0x300)
#define NDM2_PRESS_2_PRE_CONTACT_STATUS			(0x301)
#define NDM2_PRESS_3_PRE_CONTACT_STATUS			(0x302)
#define NDM2_PRESS_4_PRE_CONTACT_STATUS			(0x303)
#define NDM2_PRESS_5_PRE_CONTACT_STATUS			(0x304)
#define NDM2_PRESS_6_PRE_CONTACT_STATUS			(0x305)
#define NDM2_PRESS_7_PRE_CONTACT_STATUS			(0x306)
#define NDM2_PRESS_8_PRE_CONTACT_STATUS			(0x307)

#define NDM2_TEST_TIME_ACC_TIME_AVG_STATION1	(0x308)
#define NDM2_TEST_TIME_ACC_TIME_AVG_STATION8	(0x30F)
#define NDM2_TEST_TIME_ACC_CNT_STATION1			(0x310)
#define NDM2_TEST_TIME_ACC_CNT_STATION8			(0x317)