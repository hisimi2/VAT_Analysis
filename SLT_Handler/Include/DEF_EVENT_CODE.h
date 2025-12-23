#pragma once


//===========================
// BASE
//===========================

#define CEID_SEQ_EVENT_NONE							(-1)

#define CEID_STACKER_BASE				          (0000)
#define CEID_TRANSFER_BASE				          (1000)
#define CEID_EX_STACKER_BASE			          (2000)
#define CEID_SETPLATE_BASE				          (3000)
#define CEID_TRAYPP_BASE				          (4000)
#define CEID_LOAD_TBL_BASE				          (5000)
#define CEID_TESTPP_BASE			              (6000)
#define CEID_PRESS_UNIT_BASE		              (7000)
#define CEID_UNDER_STACKER_BASE		              (8000)
#define CEID_SYSCTRL_BASE		                  (9000)
#define CEID_AUTO_STACKER_BASE		              (9500)



//===========================
// Stacker (000~999)
//===========================
#define CEID_STACKER_CLAMPED				      (1)
#define CEID_STACKER_UNCLAMPED				      (2)
#define CEID_STACKER_INIT_START				      (3)
#define CEID_STACKER_INIT_NORMAL_END	          (4)


#define CEID_STACKER_EV_MOVE_START                (10)
#define CEID_STACKER_EV_MOVE_END                  (11)
												  
#define CEID_STACKER_EV_PUSH_DOWN_START           (20)
#define CEID_STACKER_EV_PUSH_DOWN_END             (21)

#define CEID_STACKER_EV_PUSH_UP_START             (30)
#define CEID_STACKER_EV_PUSH_UP_END               (31)

#define CEID_LOAD_STACKER_LOT_END					(40)

//===========================
// Transfer (1000~1999)
//===========================
#define CEID_TRANSFER_CLAMPED					  (1001)
#define CEID_TRANSFER_UNCLAMPED					  (1002)

#define CEID_TRANSFER_INIT_START                  (1010)
#define CEID_TRANSFER_INIT_END                    (1011)

#define CEID_TRANSFER_EXCHG_START                 (1020)
#define CEID_TRANSFER_EXCHG_END                   (1021)

#define CEID_TRANSFER_MOV_PICK_START              (1030)
#define CEID_TRANSFER_MOV_PICK_END                (1031)

#define CEID_TRANSFER_MOV_PLACE_START             (1040)
#define CEID_TRANSFER_MOV_PLACE_END               (1041)

#define CEID_TRANSFER_MOV_START                   (1050)
#define CEID_TRANSFER_MOV_END                     (1051)

#define CEID_TRANSFER_PICK_START                  (1060)
#define CEID_TRANSFER_PICK_END                    (1061)

#define CEID_TRANSFER_PLACE_START                 (1070)
#define CEID_TRANSFER_PLACE_END                   (1071)

#define CEID_TRANSFER_TRAY_CHK_START              (1080)
#define CEID_TRANSFER_TRAY_CHK_END                (1081)

#define CEID_TRANSFER_LAST_POS_START              (1090)
#define CEID_TRANSFER_LAST_POS_END                (1091)

#define CEID_TRANSFER_REQUEST_EMPTY_TRAY			(1200)

//===========================
// Ex Stacker (2000~2999)
//===========================

//===========================
// Setplate (3000~3999)
//===========================
#define CEID_SETPLATE_UP_START					  (3001)
#define CEID_SETPLATE_UP_END					  (3002)
#define CEID_SETPLATE_DOWN_START				  (3003)
#define CEID_SETPLATE_DOWN_END					  (3004)

//===========================
// Tray PP (4000~4999)
//===========================
#define CEID_TRAY_PP_PICKER_START				  (4001)
#define CEID_TRAY_PP_PICKER_END   				  (4002)
#define CEID_TRAY_PP_INIT_START				      (4003)
#define CEID_TRAY_PP_INIT_END   				  (4004)
#define CEID_TRAY_PP_PICK_PLACE_START			  (4005)
#define CEID_TRAY_PP_PICK_PLACE_END   			  (4006)
#define CEID_TRAY_PP_MOVE_START			          (4007)
#define CEID_TRAY_PP_MOVE_END   			      (4008)
#define CEID_TRAY_PP_EMPTY_CHECK_START			  (4009)
#define CEID_TRAY_PP_EMPTY_CHECK_END   			  (4010)
#define CEID_TRAY_PP_MOVE_PICK_PLACE_START		  (4011)
#define CEID_TRAY_PP_MOVE_PICK_PLACE_END		  (4012)
#define CEID_TRAY_PP_ALARMCOMPLETE_START          (4013)
#define CEID_TRAY_PP_ALARMCOMPLETE_END            (4014)

//===========================
// Loading Table (5000~5999)
//===========================
#define CEID_LOAD_TBL_INIT_START				  (5001)
#define CEID_LOAD_TBL_INIT_END					  (5002)
#define CEID_LOAD_TBL_MOVE_START				  (5003)
#define CEID_LOAD_TBL_MOVE_END					  (5004)
#define CEID_LOAD_TBL_VISION_TEACH_START		  (5005)
#define CEID_LOAD_TBL_VISION_TEACH_END			  (5006)

//===========================
// Test PP (6000~6999)
//===========================
#define CEID_TEST_PP_PICKER_UP					    (6001)
#define CEID_TEST_PP_PICKER_DOWN				    (6002)
#define CEID_TEST_PP_INIT_START  				    (6003)
#define CEID_TEST_PP_INIT_END   				    (6004)
#define CEID_TEST_PP_MOVE_START  				    (6005)
#define CEID_TEST_PP_MOVE_END   				    (6006)
#define CEID_TEST_PP_EMPTY_CHECK_START  		    (6007)
#define CEID_TEST_PP_EMPTY_CHECK_END   			    (6008)
#define CEID_TEST_PP_EXCHANGE_START  		        (6009)
#define CEID_TEST_PP_EXCHANGE_END   			    (6010)
#define CEID_TEST_PP_MOVE_EXCHANGE_START  		    (6011)
#define CEID_TEST_PP_MOVE_EXCHANGE_END   		    (6012)
#define CEID_TEST_PP_MOVE_PICK_PLACE_START          (6013)
#define CEID_TEST_PP_MOVE_PICK_PLACE_END   		    (6014)
#define CEID_TEST_PP_PICK_PLACE_START               (6015)
#define CEID_TEST_PP_PICK_PLACE_END   		        (6016)
#define CEID_TEST_PP_ALARMCOMPLETE_START            (6017)
#define CEID_TEST_PP_ALARMCOMPLETE_END              (6018)
#define CEID_TEST_PP_MOVE_ALREADY_START 		    (6019)
#define CEID_TEST_PP_MOVE_ALREADY_END			    (6020)
#define CEIO_TEST_PP_MOVE_SAMETIME_PICK_PLACE_START (6021)
#define CEIO_TEST_PP_MOVE_SAMETIME_PICK_PLACE_END   (6022)

//===========================
// Press Unit (7000~7999)
//===========================
#define CEID_PRESS_UNIT_INIT_START				  (7001)
#define CEID_PRESS_UNIT_INIT_END				  (7002)
#define CEID_PRESS_UNIT_CONTACT_START			  (7003)
#define CEID_PRESS_UNIT_CONTACT_END				  (7004)
#define CEID_PRESS_UNIT_DISCONTACT_START		  (7005)
#define CEID_PRESS_UNIT_DISCONTACT_END			  (7006)
#define CEID_PRESS_UNIT_TEST_START    			  (7007)
#define CEID_PRESS_UNIT_TEST_END    			  (7008)
#define CEID_PRESS_UNIT_VISION_CHECK_START		  (7009)
#define CEID_PRESS_UNIT_VISION_CHECK_END		  (7010)
#define CEID_PRESS_UNIT_ALARMCOMPLETE_START       (7011)
#define CEID_PRESS_UNIT_ALARMCOMPLETE_END         (7012)
#define CEID_PRESS_UNIT_SOC_CLEAN_CONTACT_START   (7013)
#define CEID_PRESS_UNIT_SOC_CLEAN_CONTACT_END     (7014)

//===========================
// Under Stacker (8000~8999)
//===========================
#define CEID_UNDER_STACKER_FWD_START			  (8001)
#define CEID_UNDER_STACKER_FWD_END				  (8002)
#define CEID_UNDER_STACKER_BWD_START			  (8003)
#define CEID_UNDER_STACKER_BWD_END				  (8004)
#define CEID_UNDER_STACKER_LOAD_SW_ON			  (8005)
#define CEID_UNDER_STACKER_LOAD_SW_OFF			  (8006)
#define CEID_UNDER_STACKER_UNLOAD_SW_ON			  (8007)
#define CEID_UNDER_STACKER_UNLOAD_SW_OFF		  (8008)

//===========================
// System Control (9000~9999)
//===========================
#define CEID_EMO_SW_ON                            (9001)
#define CEID_EMO_SW_OFF                           (9002)
#define CEID_DOORLOCK_SW_ON                       (9003)
#define CEID_DOORLOCK_SW_OFF                      (9004)
#define CEID_MAIN_POWER_ON                        (9005)
#define CEID_MAIN_POWER_OFF                       (9006)
#define CEID_TEST_SITE_SLIDER_DOCK                (9007)
#define CEID_TEST_SITE_SLIDER_UNDOCK              (9008)
#define CEID_START_SW_ON                          (9009)
#define CEID_START_SW_OFF                         (9010)
#define CEID_STOP_SW_ON                           (9011)
#define CEID_STOP_SW_OFF                          (9012)
#define CEID_COVER_SW_ON                          (9013)
#define CEID_COVER_SW_OFF                         (9014)
#define CEID_SOUND_SW_DOWN                        (9015)
#define CEID_SOUND_SW_UP                          (9016)
#define CEID_LAMP_SW_ON                           (9017)
#define CEID_LAMP_SW_OFF                          (9018)
#define CEID_SERVO_POWER_SIG_ON                   (9019)
#define CEID_SERVO_POWER_SIG_OFF                  (9020)
#define CEID_ION_ALARM_ON                         (9021)
#define CEID_ION_ALARM_OFF                        (9022)
#define CEID_ION_FAN_ALARM_ON                     (9023)
#define CEID_ION_FAN_ALARM_OFF                    (9024)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_UP_12        (9030)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_UP_34        (9031)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_UP_56        (9032)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_UP_78        (9033)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_DN_12        (9034)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_DN_34        (9035)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_DN_56        (9036)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_DN_78        (9037)
#define CEID_SLIDE_SEN_OFF_TEST_SITE_(x)          (CEID_SLIDE_SEN_OFF_TEST_SITE_UP_12 + x)
#define CEID_SLIDE_SEN_ON_TEST_SITE_UP_12         (9040)
#define CEID_SLIDE_SEN_ON_TEST_SITE_UP_34         (9041)
#define CEID_SLIDE_SEN_ON_TEST_SITE_UP_56         (9042)
#define CEID_SLIDE_SEN_ON_TEST_SITE_UP_78         (9043)
#define CEID_SLIDE_SEN_ON_TEST_SITE_DN_12         (9044)
#define CEID_SLIDE_SEN_ON_TEST_SITE_DN_34         (9045)
#define CEID_SLIDE_SEN_ON_TEST_SITE_DN_56         (9046)
#define CEID_SLIDE_SEN_ON_TEST_SITE_DN_78         (9047)
#define CEID_SLIDE_SEN_ON_TEST_SITE_(x)           (CEID_SLIDE_SEN_OFF_TEST_SITE_UP_12 + x)

#define CEID_MAIN_AIR_SIG_IS_ON                   (9050)
#define CEID_MAIN_AIR_SIG_IS_OFF                  (9051)
#define CEID_CP_TRIP_SIG_IS_ON                    (9052)
#define CEID_CP_TRIP_SIG_IS_OFF                   (9053)

#define CEID_DOWN_DOOR_SIG_IS_ON                  (9054)
#define CEID_DOWN_DOOR_SIG_IS_OFF                 (9055)

#define CEID_MAIN_AIR_REG_SIG_IS_ON               (9056)
#define CEID_MAIN_AIR_REG_SIG_IS_OFF              (9057)


#define CEID_LOT_STATUS_CHANGED                   (9100)

#define CEID_CHANGE_NORMAL_COMPLETE					(9200)

// 9500 ~
#define CEID_AUTO_STACKER_STS_CHANGED             (9500)
#define CEID_AUTO_TESTSITE_STS_CHANGED            (9501)

#define CEID_MULTI_ORG_START                      (9502)
#define CEID_MULTI_ORG_END                        (9503)