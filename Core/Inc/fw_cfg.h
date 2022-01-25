/*
 * fw_cfg.h
 *
 *  Created on: Jan 24, 2022
 *      Author: madop
 */

/******************************************************************************************************************//**
 * @file fw_cfg.h
 *
 * @brief This file is generated from Yocta-61y.xlsm (Macro version: 1.2.0)
 *
 * @copyright Copyright (c) STMicroelectronics 2011 - 2017
 *            All rights reserved.
 *********************************************************************************************************************/
#ifndef FW_CFG_H_
#define FW_CFG_H_
#include <stdint.h>

typedef struct fwCfg_t
{
    uint32_t u32_crc;                               //000 - crc - crc
    uint32_t u32_len;                               //004 - len - len
    uint32_t u32_crcN;                              //008 - crc_n - crc_n
    uint32_t u32_lenN;                              //00C - len_n - len_n
    uint16_t u16_cfgProjectId;                      //010 - cfg_project_id - cfg_project_id
    uint16_t u16_cfgVer;                            //012 - cfg_ver - cfg_ver
    uint8_t u8_cfgPartId;                           //014 - cfg_part_id - cfg_part_id
    uint8_t u8_cfgAfeVer;                           //015 - cfg_afe_ver - cfg_afe_ver
    uint8_t u8_reserved016;                         //016 - reserved
    uint8_t u8_reserved017;                         //017 - reserved
    uint8_t u8_extReleaseByte0;                     //018 - external_release_config0 - ext_release_byte0
    uint8_t u8_extReleaseByte1;                     //018 - external_release_config0 - ext_release_byte1
    uint8_t u8_extReleaseByte2;                     //018 - external_release_config0 - ext_release_byte2
    uint8_t u8_extReleaseByte3;                     //018 - external_release_config0 - ext_release_byte3
    uint8_t u8_extReleaseByte4;                     //01C - external_release_config1 - ext_release_byte4
    uint8_t u8_extReleaseByte5;                     //01C - external_release_config1 - ext_release_byte5
    uint8_t u8_extReleaseByte6;                     //01C - external_release_config1 - ext_release_byte6
    uint8_t u8_extReleaseByte7;                     //01C - external_release_config1 - ext_release_byte7
    uint8_t u8_sense0En;                            //020 - sense0_en - sense0_en
    uint8_t u8_sense1En;                            //021 - sense1_en - sense1_en
    uint8_t u8_sense2En;                            //022 - sense2_en - sense2_en
    uint8_t u8_sense3En;                            //023 - sense3_en - sense3_en
    uint8_t u8_sense4En;                            //024 - sense4_en - sense4_en
    uint8_t u8_sense5En;                            //025 - sense5_en - sense5_en
    uint8_t u8_sense6En;                            //026 - sense6_en - sense6_en
    uint8_t u8_sense7En;                            //027 - sense7_en - sense7_en
    uint8_t u8_force0En;                            //028 - force0_en - force0_en
    uint8_t u8_force1En;                            //029 - force1_en - force1_en
    uint8_t u8_force2En;                            //02A - force2_en - force2_en
    uint8_t u8_force3En;                            //02B - force3_en - force3_en
    uint8_t u8_force4En;                            //02C - force4_en - force4_en
    uint8_t u8_force5En;                            //02D - force5_en - force5_en
    uint8_t u8_force6En;                            //02E - force6_en - force6_en
    uint8_t u8_force7En;                            //02F - force7_en - force7_en
    uint8_t u8_senseLen;                            //030 - sense_len - sense_len
    uint8_t u8_forceLen;                            //031 - force_len - force_len
    uint16_t u16_scrXRes;                           //032 - scr_x_res - scr_x_res
    uint16_t u16_scrYRes;                           //034 - scr_y_res - scr_y_res
    uint8_t b1_orientSwap          : 1;    			//036 - scr_orient - orient_swap
    uint8_t b1_orientForce         : 1;    			//036 - scr_orient - orient_force
    uint8_t b1_orientSense         : 1;    			//036 - scr_orient - orient_sense
    uint8_t b1_rptResScaleEn       : 1;    			//036 - scr_orient - rpt_res_scale_en
    uint8_t b4_reserved036_4_7     : 4;    			//036 - scr_orient - reserved
    uint16_t u16_rptXRes;                  			//037 - rpt_x_res - rpt_x_res
    uint16_t u16_rptYRes;                  			//039 - rpt_y_res - rpt_y_res
    uint16_t u16_scrPpi;                   			//03B - scr_ppi - scr_ppi
    uint8_t u8_orientRawdata;                       //03D - orient_rawdata - orient_rawdata
    uint8_t u8_scrMultiple;                         //03E - scr_multiple - scr_multiple
    uint8_t u8_reserved03F;                         //03F - reserved
}fwCfg_t;


#define CFG_U32_CRC                         (fwCfgp->u32_crc)           	//000 - crc
#define CFG_U32_LEN                         (fwCfgp->u32_len)           	//004 - len
#define CFG_U32_CRC_N                       (fwCfgp->u32_crcN)          	//008 - crc_n
#define CFG_U32_LEN_N                       (fwCfgp->u32_lenN)          	//00C - len_n
#define CFG_U16_CFG_PROJECT_ID              (fwCfgp->u16_cfgProjectId)  	//010 - cfg_project_id
#define CFG_U16_CFG_VER                     (fwCfgp->u16_cfgVer)        	//012 - cfg_ver
#define CFG_U8_CFG_PART_ID                  (fwCfgp->u8_cfgPartId)      	//014 - cfg_part_id
#define CFG_U8_CFG_AFE_VER                  (fwCfgp->u8_cfgAfeVer)     		//015 - cfg_afe_ver
#define CFG_U8_EXT_RELEASE_BYTE0            (fwCfgp->u8_extReleaseByte0)    //018 - external_release_config0
#define CFG_U8_EXT_RELEASE_BYTE1            (fwCfgp->u8_extReleaseByte1)    //018 - external_release_config0
#define CFG_U8_EXT_RELEASE_BYTE2            (fwCfgp->u8_extReleaseByte2)    //018 - external_release_config0
#define CFG_U8_EXT_RELEASE_BYTE3            (fwCfgp->u8_extReleaseByte3)    //018 - external_release_config0
#define CFG_U8_EXT_RELEASE_BYTE4            (fwCfgp->u8_extReleaseByte4)    //01C - external_release_config1
#define CFG_U8_EXT_RELEASE_BYTE5            (fwCfgp->u8_extReleaseByte5)    //01C - external_release_config1
#define CFG_U8_EXT_RELEASE_BYTE6            (fwCfgp->u8_extReleaseByte6)    //01C - external_release_config1
#define CFG_U8_EXT_RELEASE_BYTE7            (fwCfgp->u8_extReleaseByte7)    //01C - external_release_config1
#define CFG_U8_SENSE0_EN                    (fwCfgp->u8_sense0En)       	//020 - sense0_en
#define CFG_U8_SENSE1_EN                    (fwCfgp->u8_sense1En)       	//021 - sense1_en
#define CFG_U8_SENSE2_EN                    (fwCfgp->u8_sense2En)       	//022 - sense2_en
#define CFG_U8_SENSE3_EN                    (fwCfgp->u8_sense3En)       	//023 - sense3_en
#define CFG_U8_SENSE4_EN                    (fwCfgp->u8_sense4En)       	//024 - sense4_en
#define CFG_U8_SENSE5_EN                    (fwCfgp->u8_sense5En)       	//025 - sense5_en
#define CFG_U8_SENSE6_EN                    (fwCfgp->u8_sense6En)       	//026 - sense6_en
#define CFG_U8_SENSE7_EN                    (fwCfgp->u8_sense7En)       	//027 - sense7_en
#define CFG_U8_FORCE0_EN                    (fwCfgp->u8_force0En)       	//028 - force0_en
#define CFG_U8_FORCE1_EN                    (fwCfgp->u8_force1En)       	//029 - force1_en
#define CFG_U8_FORCE2_EN                    (fwCfgp->u8_force2En)       	//02A - force2_en
#define CFG_U8_FORCE3_EN                    (fwCfgp->u8_force3En)       	//02B - force3_en
#define CFG_U8_FORCE4_EN                    (fwCfgp->u8_force4En)       	//02C - force4_en
#define CFG_U8_FORCE5_EN                    (fwCfgp->u8_force5En)       	//02D - force5_en
#define CFG_U8_FORCE6_EN                    (fwCfgp->u8_force6En)       	//02E - force6_en
#define CFG_U8_FORCE7_EN                    (fwCfgp->u8_force7En)       	//02F - force7_en
#define CFG_U8_SENSE_LEN                    (fwCfgp->u8_senseLen)       	//030 - sense_len
#define CFG_U8_FORCE_LEN                    (fwCfgp->u8_forceLen)       	//031 - force_len
#define CFG_U16_SCR_X_RES                   (fwCfgp->u16_scrXRes)       	//032 - scr_x_res
#define CFG_U16_SCR_Y_RES                   (fwCfgp->u16_scrYRes)       	//034 - scr_y_res
#define CFG_B1_ORIENT_SWAP                  (fwCfgp->b1_orientSwap)     	//036 - scr_orient
#define CFG_B1_ORIENT_FORCE                 (fwCfgp->b1_orientForce)    	//036 - scr_orient
#define CFG_B1_ORIENT_SENSE                 (fwCfgp->b1_orientSense)    	//036 - scr_orient
#define CFG_B1_RPT_RES_SCALE_EN             (fwCfgp->b1_rptResScaleEn)  	//036 - scr_orient
#define CFG_U16_RPT_X_RES                   (fwCfgp->u16_rptXRes)       	//037 - rpt_x_res
#define CFG_U16_RPT_Y_RES                   (fwCfgp->u16_rptYRes)       	//039 - rpt_y_res
#define CFG_U16_SCR_PPI                     (fwCfgp->u16_scrPpi)        	//03B - scr_ppi
#define CFG_U8_ORIENT_RAWDATA               (fwCfgp->u8_orientRawdata)  	//03D - orient_rawdata
#define CFG_U8_SCR_MULTIPLE                 (fwCfgp->u8_scrMultiple)    	//03E - scr_multiple


#endif /* INC_FW_CFG_H_ */
